/*
 * slam_gmapping
 * Copyright (c) 2008, Willow Garage, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the names of Stanford University or Willow Garage, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/* Author: Brian Gerkey */
/* Modified by: Charles DuHadway */


/**

@mainpage slam_gmapping

@htmlinclude manifest.html

@b slam_gmapping is a wrapper around the GMapping SLAM library. It reads laser
scans and odometry and computes a map. This map can be
written to a file using e.g.

  "rosrun map_server map_saver static_map:=dynamic_map"

<hr>

@section topic ROS topics

Subscribes to (name/type):
- @b "scan"/<a href="../../sensor_msgs/html/classstd__msgs_1_1LaserScan.html">sensor_msgs/LaserScan</a> : data from a laser range scanner 
- @b "/tf": odometry from the robot


Publishes to (name/type):
- @b "/tf"/tf/tfMessage: position relative to the map


@section services
 - @b "~dynamic_map" : returns the map


@section parameters ROS parameters

Reads the following parameters from the parameter server

Parameters used by our GMapping wrapper:

- @b "~throttle_scans": @b [int] throw away every nth laser scan
- @b "~base_frame": @b [string] the tf frame_id to use for the robot base pose
- @b "~map_frame": @b [string] the tf frame_id where the robot pose on the map is published
- @b "~odom_frame": @b [string] the tf frame_id from which odometry is read
- @b "~map_update_interval": @b [double] time in seconds between two recalculations of the map


Parameters used by GMapping itself:

Laser Parameters:
- @b "~/maxRange" @b [double] maximum range of the laser scans. Rays beyond this range get discarded completely. (default: maximum laser range minus 1 cm, as received in the the first LaserScan message)
- @b "~/maxUrange" @b [double] maximum range of the laser scanner that is used for map building (default: same as maxRange)
- @b "~/sigma" @b [double] standard deviation for the scan matching process (cell)
- @b "~/kernelSize" @b [int] search window for the scan matching process
- @b "~/lstep" @b [double] initial search step for scan matching (linear)
- @b "~/astep" @b [double] initial search step for scan matching (angular)
- @b "~/iterations" @b [int] number of refinement steps in the scan matching. The final "precision" for the match is lstep*2^(-iterations) or astep*2^(-iterations), respectively.
- @b "~/lsigma" @b [double] standard deviation for the scan matching process (single laser beam)
- @b "~/ogain" @b [double] gain for smoothing the likelihood
- @b "~/lskip" @b [int] take only every (n+1)th laser ray for computing a match (0 = take all rays)
- @b "~/minimumScore" @b [double] minimum score for considering the outcome of the scanmatching good. Can avoid 'jumping' pose estimates in large open spaces when using laser scanners with limited range (e.g. 5m). (0 = default. Scores go up to 600+, try 50 for example when experiencing 'jumping' estimate issues)

Motion Model Parameters (all standard deviations of a gaussian noise model)
- @b "~/srr" @b [double] linear noise component (x and y)
- @b "~/stt" @b [double] angular noise component (theta)
- @b "~/srt" @b [double] linear -> angular noise component
- @b "~/str" @b [double] angular -> linear noise component

Others:
- @b "~/linearUpdate" @b [double] the robot only processes new measurements if the robot has moved at least this many meters
- @b "~/angularUpdate" @b [double] the robot only processes new measurements if the robot has turned at least this many rads

- @b "~/resampleThreshold" @b [double] threshold at which the particles get resampled. Higher means more frequent resampling.
- @b "~/particles" @b [int] (fixed) number of particles. Each particle represents a possible trajectory that the robot has traveled

Likelihood sampling (used in scan matching)
- @b "~/llsamplerange" @b [double] linear range
- @b "~/lasamplerange" @b [double] linear step size
- @b "~/llsamplestep" @b [double] linear range
- @b "~/lasamplestep" @b [double] angular step size

Initial map dimensions and resolution:
- @b "~/xmin" @b [double] minimum x position in the map [m]
- @b "~/ymin" @b [double] minimum y position in the map [m]
- @b "~/xmax" @b [double] maximum x position in the map [m]
- @b "~/ymax" @b [double] maximum y position in the map [m]
- @b "~/delta" @b [double] size of one pixel [m]

*/



#include "slam_gmapping.h"

#include <iostream>

#include <time.h>

#include "ros/ros.h"
#include "ros/console.h"
#include "nav_msgs/MapMetaData.h"

#include "gmapping/sensor/sensor_range/rangesensor.h"
#include "gmapping/sensor/sensor_odometry/odometrysensor.h"

#include <rosbag/bag.h>
#include <rosbag/view.h>
#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

// compute linear index for given map coords
#define MAP_IDX(sx, i, j) ((sx) * (j) + (i))
//计算地图上的坐标

SlamGMapping::SlamGMapping():
  map_to_odom_(tf::Transform(tf::createQuaternionFromRPY( 0, 0, 0 ), tf::Point(0, 0, 0 ))),
  laser_count_(0), private_nh_("~"), scan_filter_sub_(NULL), scan_filter_(NULL), transform_thread_(NULL)
{//这里的map to odom创造了一个初始的里程计相对于全局固定坐标系的变换 即在全局坐标系中标注odom的位置
  seed_ = time(NULL);
  init();
}

SlamGMapping::SlamGMapping(ros::NodeHandle& nh, ros::NodeHandle& pnh):
  map_to_odom_(tf::Transform(tf::createQuaternionFromRPY( 0, 0, 0 ), tf::Point(0, 0, 0 ))),
  laser_count_(0),node_(nh), private_nh_(pnh), scan_filter_sub_(NULL), scan_filter_(NULL), transform_thread_(NULL)
{
  seed_ = time(NULL);
  init();
}

SlamGMapping::SlamGMapping(long unsigned int seed, long unsigned int max_duration_buffer):
  map_to_odom_(tf::Transform(tf::createQuaternionFromRPY( 0, 0, 0 ), tf::Point(0, 0, 0 ))),
  laser_count_(0), private_nh_("~"), scan_filter_sub_(NULL), scan_filter_(NULL), transform_thread_(NULL),
  seed_(seed), tf_(ros::Duration(max_duration_buffer))
{
  init();
}


void SlamGMapping::init()
{
  // log4cxx::Logger::getLogger(ROSCONSOLE_DEFAULT_NAME)->setLevel(ros::console::g_level_lookup[ros::console::levels::Debug]);

  // The library is pretty chatty
  //gsp_ = new GMapping::GridSlamProcessor(std::cerr);
  gsp_ = new GMapping::GridSlamProcessor();//创建一个栅格进程类
  ROS_ASSERT(gsp_);

  tfB_ = new tf::TransformBroadcaster();//tf发布
  ROS_ASSERT(tfB_);

  gsp_laser_ = NULL;
  gsp_odom_ = NULL;

  got_first_scan_ = false;
  got_map_ = false;
  

  //以下是根据launch文件获取参数
  // Parameters used by our GMapping wrapper
  if(!private_nh_.getParam("throttle_scans", throttle_scans_))
    throttle_scans_ = 1;
  if(!private_nh_.getParam("base_frame", base_frame_))
    base_frame_ = "base_link";
  if(!private_nh_.getParam("map_frame", map_frame_))
    map_frame_ = "map";
  if(!private_nh_.getParam("odom_frame", odom_frame_))
    odom_frame_ = "odom";

  private_nh_.param("transform_publish_period", transform_publish_period_, 0.05);

  double tmp;
  if(!private_nh_.getParam("map_update_interval", tmp))
    tmp = 5.0;
  map_update_interval_.fromSec(tmp);
  
  // Parameters used by GMapping itself
  maxUrange_ = 0.0;  maxRange_ = 0.0; // preliminary default, will be set in initMapper()
  if(!private_nh_.getParam("minimumScore", minimum_score_))
    minimum_score_ = 0;
  if(!private_nh_.getParam("sigma", sigma_))
    sigma_ = 0.05;
  if(!private_nh_.getParam("kernelSize", kernelSize_))
    kernelSize_ = 1;
  if(!private_nh_.getParam("lstep", lstep_))
    lstep_ = 0.05;
  if(!private_nh_.getParam("astep", astep_))
    astep_ = 0.05;
  if(!private_nh_.getParam("iterations", iterations_))
    iterations_ = 5;
  if(!private_nh_.getParam("lsigma", lsigma_))
    lsigma_ = 0.075;
  if(!private_nh_.getParam("ogain", ogain_))
    ogain_ = 3.0;
  if(!private_nh_.getParam("lskip", lskip_))
    lskip_ = 0;
  if(!private_nh_.getParam("srr", srr_))
    srr_ = 0.1;
  if(!private_nh_.getParam("srt", srt_))
    srt_ = 0.2;
  if(!private_nh_.getParam("str", str_))
    str_ = 0.1;
  if(!private_nh_.getParam("stt", stt_))
    stt_ = 0.2;
  if(!private_nh_.getParam("linearUpdate", linearUpdate_))
    linearUpdate_ = 1.0;
  if(!private_nh_.getParam("angularUpdate", angularUpdate_))
    angularUpdate_ = 0.5;
  if(!private_nh_.getParam("temporalUpdate", temporalUpdate_))
    temporalUpdate_ = -1.0;
  if(!private_nh_.getParam("resampleThreshold", resampleThreshold_))
    resampleThreshold_ = 0.5;
  if(!private_nh_.getParam("particles", particles_))
    particles_ = 30;
  if(!private_nh_.getParam("xmin", xmin_))
    xmin_ = -100.0;
  if(!private_nh_.getParam("ymin", ymin_))
    ymin_ = -100.0;
  if(!private_nh_.getParam("xmax", xmax_))
    xmax_ = 100.0;
  if(!private_nh_.getParam("ymax", ymax_))
    ymax_ = 100.0;
  if(!private_nh_.getParam("delta", delta_))
    delta_ = 0.05;
  if(!private_nh_.getParam("occ_thresh", occ_thresh_))
    occ_thresh_ = 0.25;
  if(!private_nh_.getParam("llsamplerange", llsamplerange_))
    llsamplerange_ = 0.01;
  if(!private_nh_.getParam("llsamplestep", llsamplestep_))
    llsamplestep_ = 0.01;
  if(!private_nh_.getParam("lasamplerange", lasamplerange_))
    lasamplerange_ = 0.005;
  if(!private_nh_.getParam("lasamplestep", lasamplestep_))
    lasamplestep_ = 0.005;
    
  if(!private_nh_.getParam("tf_delay", tf_delay_))
    tf_delay_ = transform_publish_period_;

}


void SlamGMapping::startLiveSlam()
{
  entropy_publisher_ = private_nh_.advertise<std_msgs::Float64>("entropy", 1, true);  //ros句柄
  sst_ = node_.advertise<nav_msgs::OccupancyGrid>("map", 1, true);            //发布map话题的publisher
  sstm_ = node_.advertise<nav_msgs::MapMetaData>("map_metadata", 1, true);    //发布map_metadata话题
  ss_ = node_.advertiseService("dynamic_map", &SlamGMapping::mapCallback, this);                    //挂起地图服务器,接收请求时返回nav_msgs类型的map_数据
  scan_filter_sub_ = new message_filters::Subscriber<sensor_msgs::LaserScan>(node_, "scan", 5);       //订阅雷达数据
  scan_filter_ = new tf::MessageFilter<sensor_msgs::LaserScan>(*scan_filter_sub_, tf_, odom_frame_, 5); //指明按tf发布过滤雷达数据，检测frame_id,也就是说必须odom已经转换完成了才接收本次雷达数据
  scan_filter_->registerCallback([this](auto msg){ laserCallback(msg); });                              //创建一个匿名函数,每当接收到消息时调用laserCallback,并放入消息作为参数,并可以访问slamgmapping类

  transform_thread_ = new boost::thread(boost::bind(&SlamGMapping::publishLoop, this, transform_publish_period_)); //另起一个线程，将需要参数的函数publishloop与this,发布间隔绑定以便函数调用时使用参数
}


//rosbag重放功能,略过,暂时不需要这个
void SlamGMapping::startReplay(const std::string & bag_fname, std::string scan_topic)
{
  double transform_publish_period;
  ros::NodeHandle private_nh_("~");
  entropy_publisher_ = private_nh_.advertise<std_msgs::Float64>("entropy", 1, true);
  sst_ = node_.advertise<nav_msgs::OccupancyGrid>("map", 1, true);
  sstm_ = node_.advertise<nav_msgs::MapMetaData>("map_metadata", 1, true);
  ss_ = node_.advertiseService("dynamic_map", &SlamGMapping::mapCallback, this);
  
  rosbag::Bag bag;
  bag.open(bag_fname, rosbag::bagmode::Read);
  
  std::vector<std::string> topics;
  topics.push_back(std::string("/tf"));
  topics.push_back(scan_topic);
  rosbag::View viewall(bag, rosbag::TopicQuery(topics));

  // Store up to 5 messages and there error message (if they cannot be processed right away)
  std::queue<std::pair<sensor_msgs::LaserScan::ConstPtr, std::string> > s_queue;
  foreach(rosbag::MessageInstance const m, viewall)
  {
    tf::tfMessage::ConstPtr cur_tf = m.instantiate<tf::tfMessage>();
    if (cur_tf != NULL) {
      for (size_t i = 0; i < cur_tf->transforms.size(); ++i)
      {
        geometry_msgs::TransformStamped transformStamped;
        tf::StampedTransform stampedTf;
        transformStamped = cur_tf->transforms[i];
        tf::transformStampedMsgToTF(transformStamped, stampedTf);
        tf_.setTransform(stampedTf);
      }
    }

    sensor_msgs::LaserScan::ConstPtr s = m.instantiate<sensor_msgs::LaserScan>();
    if (s != NULL) {
      if (!(ros::Time(s->header.stamp)).is_zero())
      {
        s_queue.push(std::make_pair(s, ""));
      }
      // Just like in live processing, only process the latest 5 scans
      if (s_queue.size() > 5) {
        ROS_WARN_STREAM("Dropping old scan: " << s_queue.front().second);
        s_queue.pop();
      }
      // ignoring un-timestamped tf data 
    }

    // Only process a scan if it has tf data
    while (!s_queue.empty())
    {
      try
      {
        tf::StampedTransform t;
        tf_.lookupTransform(s_queue.front().first->header.frame_id, odom_frame_, s_queue.front().first->header.stamp, t);
        this->laserCallback(s_queue.front().first);
        s_queue.pop();
      }
      // If tf does not have the data yet
      catch(tf2::TransformException& e)
      {
        // Store the error to display it if we cannot process the data after some time
        s_queue.front().second = std::string(e.what());
        break;
      }
    }
  }

  bag.close();
}

void SlamGMapping::publishLoop(double transform_publish_period){
  if(transform_publish_period == 0)
    return;

  ros::Rate r(1.0 / transform_publish_period);    //计算发布频率
  while(ros::ok()){
    publishTransform();           //在另外一个线程中运行此函数                
    r.sleep();
  }
}

SlamGMapping::~SlamGMapping()
{
  //析构函数
  if(transform_thread_){
    transform_thread_->join();
    delete transform_thread_;
  }

  delete gsp_;
  if(gsp_laser_)
    delete gsp_laser_;
  if(gsp_odom_)
    delete gsp_odom_;
  if (scan_filter_)
    delete scan_filter_;
  if (scan_filter_sub_)
    delete scan_filter_sub_;
}

bool
SlamGMapping::getOdomPose(GMapping::OrientedPoint& gmap_pose, const ros::Time& t)
{
  // Get the pose of the centered laser at the right time
  centered_laser_pose_.stamp_ = t;//记录需要查询的时刻
  // Get the laser's pose that is centered
  tf::Stamped<tf::Transform> odom_pose;//用于记录转换的点
  try
  {
    tf_.transformPose(odom_frame_, centered_laser_pose_, odom_pose);//将一个点转化至里程计坐标系
  }
  catch(tf::TransformException e)
  {
    ROS_WARN("Failed to compute odom pose, skipping scan (%s)", e.what());//如果tf变换还没到位则返回
    return false;
  }
  double yaw = tf::getYaw(odom_pose.getRotation());

  gmap_pose = GMapping::OrientedPoint(odom_pose.getOrigin().x(),
                                      odom_pose.getOrigin().y(),
                                      yaw);//通过相对位移记录落点到里程计的方位
  return true;
}

bool
SlamGMapping::initMapper(const sensor_msgs::LaserScan& scan)
{
  laser_frame_ = scan.header.frame_id;
  // Get the laser's pose, relative to base.
  tf::Stamped<tf::Pose> ident;       
  tf::Stamped<tf::Transform> laser_pose;  //两个带时间戳与frame_id的位姿
  ident.setIdentity();          //初始化为单位位姿
  ident.frame_id_ = laser_frame_;//声明参考坐标系为雷达坐标
  ident.stamp_ = scan.header.stamp;
  try
  {
    tf_.transformPose(base_frame_, ident, laser_pose);         //从基于laser的单位位姿ident到baseframe的变换,结果存入laser_pose
  }
  catch(tf::TransformException e)
  {
    ROS_WARN("Failed to compute laser pose, aborting initialization (%s)",
             e.what());
    return false;
  }

  // create a point 1m above the laser position and transform it into the laser-frame
  tf::Vector3 v;
  v.setValue(0, 0, 1 + laser_pose.getOrigin().z());    //创建一个在雷达上方1m的点(?)
  tf::Stamped<tf::Vector3> up(v, scan.header.stamp,
                                      base_frame_);     //基于base_frame,给以扫描的时间戳
  try
  {
    tf_.transformPoint(laser_frame_, up, up);         //执行转换,从base_fram帧到雷达帧,并赋值给自己
    ROS_DEBUG("Z-Axis in sensor frame: %.3f", up.z());
  }
  catch(tf::TransformException& e)
  {
    ROS_WARN("Unable to determine orientation of laser: %s",
             e.what());
    return false;
  }
  
  // gmapping doesnt take roll or pitch into account. So check for correct sensor alignment.
  if (fabs(fabs(up.z()) - 1) > 0.001)                  //主要是要检测雷达水平放置(是否高于水平面)
  {
    ROS_WARN("Laser has to be mounted planar! Z-coordinate has to be 1 or -1, but gave: %.5f",
                 up.z());
    return false;
  }

  gsp_laser_beam_count_ = scan.ranges.size();  //扫描光线个数

  double angle_center = (scan.angle_min + scan.angle_max)/2;

  if (up.z() > 0)
  {
    do_reverse_range_ = scan.angle_min > scan.angle_max;
    centered_laser_pose_ = tf::Stamped<tf::Pose>(tf::Transform(tf::createQuaternionFromRPY(0,0,angle_center),
                                                               tf::Vector3(0,0,0)), ros::Time::now(), laser_frame_);  //确定转换中心雷达位置
    ROS_INFO("Laser is mounted upwards.");
  }
  else
  {
    do_reverse_range_ = scan.angle_min < scan.angle_max;
    centered_laser_pose_ = tf::Stamped<tf::Pose>(tf::Transform(tf::createQuaternionFromRPY(M_PI,0,-angle_center),
                                                               tf::Vector3(0,0,0)), ros::Time::now(), laser_frame_);
    ROS_INFO("Laser is mounted upside down.");
  }
//以上是判断雷达建模是否水平
  // Compute the angles of the laser from -x to x, basically symmetric and in increasing order
  laser_angles_.resize(scan.ranges.size());                         //初始化扫描角度数组,确定一帧所含的角度数
  // Make sure angles are started so that they are centered
  double theta = - std::fabs(scan.angle_min - scan.angle_max)/2;//永远是从负开始扫描
  for(unsigned int i=0; i<scan.ranges.size(); ++i)
  {
    laser_angles_[i]=theta;
    theta += std::fabs(scan.angle_increment);              //更新theta:angle_increment为间隔角度值来确定点云绝对相对位置 scan作为一种消息类型,这是一种硬件参数
  }
//这样就确定了一帧内数组中各个点对应的角度
//有一点隐患
  ROS_DEBUG("Laser angles in laser-frame: min: %.3f max: %.3f inc: %.3f", scan.angle_min, scan.angle_max,
            scan.angle_increment);
  ROS_DEBUG("Laser angles in top-down centered laser-frame: min: %.3f max: %.3f inc: %.3f", laser_angles_.front(),
            laser_angles_.back(), std::fabs(scan.angle_increment));

  GMapping::OrientedPoint gmap_pose(0, 0, 0);

  // setting maxRange and maxUrange here so we can set a reasonable default
  ros::NodeHandle private_nh_("~");
  if(!private_nh_.getParam("maxRange", maxRange_))
    maxRange_ = scan.range_max - 0.01;
  if(!private_nh_.getParam("maxUrange", maxUrange_))
    maxUrange_ = maxRange_;

  //向GMapping内核注册雷达
  //向内核介绍雷达的光束个数,角度分辨率,雷达相对odom的位姿,最大扫描范围
  //由于内核是按照名字查照与处理设备,所有名字必为"FLASER"
  gsp_laser_ = new GMapping::RangeSensor("FLASER",
                                         gsp_laser_beam_count_,
                                         fabs(scan.angle_increment),
                                         gmap_pose,
                                         0.0,
                                         maxRange_);  //这个地方曾经报过错,被改了
  ROS_ASSERT(gsp_laser_);

  GMapping::SensorMap smap;   //创建一个传感器图
  smap.insert(make_pair(gsp_laser_->getName(), gsp_laser_)); //插入雷达信息
  gsp_->setSensorMap(smap);

  gsp_odom_ = new GMapping::OdometrySensor(odom_frame_);      //创建gmapping内核odom类,由于odom是全局共用的,不需要进入传感器表
  ROS_ASSERT(gsp_odom_);


  /// @todo Expose setting an initial pose
  GMapping::OrientedPoint initialPose;
  if(!getOdomPose(initialPose, scan.header.stamp))
  {
    ROS_WARN("Unable to determine inital pose of laser! Starting point will be set to zero.");
    initialPose = GMapping::OrientedPoint(0.0, 0.0, 0.0);
  }                               
                                        //以下是神秘的初始化函数,或许我们不需要关心它们的实现?
  gsp_->setMatchingParameters(maxUrange_, maxRange_, sigma_,
                              kernelSize_, lstep_, astep_, iterations_,
                              lsigma_, ogain_, lskip_);  //设置扫描匹配参数

  gsp_->setMotionModelParameters(srr_, srt_, str_, stt_);//设置里程计运动噪声
  gsp_->setUpdateDistances(linearUpdate_, angularUpdate_, resampleThreshold_);//检测移动更新扫描 以及重采样阈值
  gsp_->setUpdatePeriod(temporalUpdate_);//检测时间更新扫描
  gsp_->setgenerateMap(false);//不更新粒子地图
  gsp_->GridSlamProcessor::init(particles_, xmin_, ymin_, xmax_, ymax_,
                                delta_, initialPose);

    //设置似然采样参数
  gsp_->setllsamplerange(llsamplerange_);
  gsp_->setllsamplestep(llsamplestep_);
  /// @todo Check these calls; in the gmapping gui, they use
  /// llsamplestep and llsamplerange intead of lasamplestep and
  /// lasamplerange.  It was probably a typo, but who knows.
  
 // 检查这些调用；在 gmapping GUI 中，他们使用的是
/// llsamplestep 和 llsamplerange，而不是 lasamplestep 和
/// lasamplerange。这可能是一个拼写错误，但谁知道呢。          你认真的？
  gsp_->setlasamplerange(lasamplerange_);
  gsp_->setlasamplestep(lasamplestep_);


  gsp_->setminimumScore(minimum_score_);  //扫描匹配的最低得分阈值

  // Call the sampling function once to set the seed.
  GMapping::sampleGaussian(1,seed_); //设置随机数

  ROS_INFO("Initialization complete");

  return true;
}

bool
SlamGMapping::addScan(const sensor_msgs::LaserScan& scan, GMapping::OrientedPoint& gmap_pose)
{
  //这个函数是将本轮扫描信息放入gmapping内核

  if(!getOdomPose(gmap_pose, scan.header.stamp))                     
     return false;//获取在当前时间戳下odom的位置
  
  if(scan.ranges.size() != gsp_laser_beam_count_) 
    return false;//光束数检测

  // GMapping wants an array of doubles...
  double* ranges_double = new double[scan.ranges.size()];                 //建立一个double类型的读取数组
  // If the angle increment is negative, we have to invert the order of the readings.
  // 如果角度增量为负，我们必须颠倒读取的顺序。
  if (do_reverse_range_)                                            //不是哥们你一定要在initmap里面定义这个?不觉得耦合过度了吗
  {
    ROS_DEBUG("Inverting scan");
    int num_ranges = scan.ranges.size();
    for(int i=0; i < num_ranges; i++)
    {
      // Must filter out short readings, because the mapper won't
      if(scan.ranges[num_ranges - i - 1] < scan.range_min)
        ranges_double[i] = (double)scan.range_max;
      else
        ranges_double[i] = (double)scan.ranges[num_ranges - i - 1];
    }
  } else 
  {
    for(unsigned int i=0; i < scan.ranges.size(); i++)
    {
      // Must filter out short readings, because the mapper won't
      if(scan.ranges[i] < scan.range_min)
        ranges_double[i] = (double)scan.range_max;          //过短的扫描视为最长,即视为没有障碍物
      else
        ranges_double[i] = (double)scan.ranges[i]; 
    }
  }
 
  GMapping::RangeReading reading(scan.ranges.size(),
                                 ranges_double,
                                 gsp_laser_,
                                 scan.header.stamp.toSec());            //套了一层类的壳子,实际上是把扫描数组大小 /ranges_double(扫描数据数组) laser句柄 时间戳扔进reading类里面

  // ...but it deep copies them in RangeReading constructor, so we don't
  // need to keep our array around.
  delete[] ranges_double; //防止内存泄漏

  reading.setPose(gmap_pose);           //设定当前位置为odom_pose

  /*
  ROS_DEBUG("scanpose (%.3f): %.3f %.3f %.3f\n",
            scan.header.stamp.toSec(),
            gmap_pose.x,
            gmap_pose.y,
            gmap_pose.theta);
            */
  ROS_DEBUG("processing scan");

  return gsp_->processScan(reading);         //**********************核心算法***************************
}

void
SlamGMapping::laserCallback(const sensor_msgs::LaserScan::ConstPtr& scan)       //雷达回调函数
{
  laser_count_++;
  if ((laser_count_ % throttle_scans_) != 0)
    return;
  //并不是每一帧都处理
  static ros::Time last_map_update(0,0);

  // We can't initialize the mapper until we've got the first scan
  if(!got_first_scan_)
  {
    if(!initMapper(*scan))                                       //在此处为第一次initMapper,但是每次都进行一次检查是否有点太多余了?
      return;
    got_first_scan_ = true;
  }

  GMapping::OrientedPoint odom_pose;

  if(addScan(*scan, odom_pose))                          //调用addScan
  {
    ROS_DEBUG("scan processed");

    GMapping::OrientedPoint mpose = gsp_->getParticles()[gsp_->getBestParticleIndex()].pose;        //获取最佳粒子在地图上的位姿
    ROS_DEBUG("new best pose: %.3f %.3f %.3f", mpose.x, mpose.y, mpose.theta);
    ROS_DEBUG("odom pose: %.3f %.3f %.3f", odom_pose.x, odom_pose.y, odom_pose.theta);
    ROS_DEBUG("correction: %.3f %.3f %.3f", mpose.x - odom_pose.x, mpose.y - odom_pose.y, mpose.theta - odom_pose.theta); 

    tf::Transform laser_to_map = tf::Transform(tf::createQuaternionFromRPY(0, 0, mpose.theta), tf::Vector3(mpose.x, mpose.y, 0.0)).inverse();//有取逆,是配准后的最佳位姿到map的转换
    tf::Transform odom_to_laser = tf::Transform(tf::createQuaternionFromRPY(0, 0, odom_pose.theta), tf::Vector3(odom_pose.x, odom_pose.y, 0.0));//相当于里程计到base_link的变换

    map_to_odom_mutex_.lock();                               //进行互斥锁
    map_to_odom_ = (odom_to_laser * laser_to_map).inverse(); //转换:修正里程计原点
    map_to_odom_mutex_.unlock();

    if(!got_map_ || (scan->header.stamp - last_map_update) > map_update_interval_)
    {
      updateMap(*scan);  //每隔一段时间更新一次地图
      last_map_update = scan->header.stamp;
      ROS_DEBUG("Updated the map");
    }
  } else
    ROS_DEBUG("cannot process scan");
}

double
SlamGMapping::computePoseEntropy()
{
  double weight_total=0.0;
  for(std::vector<GMapping::GridSlamProcessor::Particle>::const_iterator it = gsp_->getParticles().begin();
      it != gsp_->getParticles().end();
      ++it)
  {
    weight_total += it->weight; //计算粒子总权重
  }
  double entropy = 0.0;
  for(std::vector<GMapping::GridSlamProcessor::Particle>::const_iterator it = gsp_->getParticles().begin();
      it != gsp_->getParticles().end();
      ++it)
  {
    if(it->weight/weight_total > 0.0)
      entropy += it->weight/weight_total * log(it->weight/weight_total); //计算熵来诊断粒子耗散程度 高则噪声多,低则需要重采样(过拟合)
  }
  return -entropy;
}

//这个函数通过遍历粒子树构建栅格地图
void
SlamGMapping::updateMap(const sensor_msgs::LaserScan& scan)
{
  ROS_DEBUG("Update map");
  boost::mutex::scoped_lock map_lock (map_mutex_);  //奇怪的上锁方式
  GMapping::ScanMatcher matcher;//设置本地地图

  matcher.setLaserParameters(scan.ranges.size(), &(laser_angles_[0]),
                             gsp_laser_->getPose());

  matcher.setlaserMaxRange(maxRange_);
  matcher.setusableRange(maxUrange_);
  matcher.setgenerateMap(true); //设置地图更新为允许

  GMapping::GridSlamProcessor::Particle best =
          gsp_->getParticles()[gsp_->getBestParticleIndex()];//获取最佳粒子
  std_msgs::Float64 entropy;
  entropy.data = computePoseEntropy();//计算熵
  if(entropy.data > 0.0)
    entropy_publisher_.publish(entropy);

  if(!got_map_) {
    map_.map.info.resolution = delta_;
    map_.map.info.origin.position.x = 0.0;
    map_.map.info.origin.position.y = 0.0;
    map_.map.info.origin.position.z = 0.0;
    map_.map.info.origin.orientation.x = 0.0;
    map_.map.info.origin.orientation.y = 0.0;
    map_.map.info.origin.orientation.z = 0.0;
    map_.map.info.origin.orientation.w = 1.0;
  } //第一次发布地图时初始化

  GMapping::Point center; //设置地图原点
  center.x=(xmin_ + xmax_) / 2.0;
  center.y=(ymin_ + ymax_) / 2.0;

  GMapping::ScanMatcherMap smap(center, xmin_, ymin_, xmax_, ymax_, 
                                delta_);//按现在地图的参数参数创建临时空地图

  ROS_DEBUG("Trajectory tree:");
  for(GMapping::GridSlamProcessor::TNode* n = best.node;
      n;
      n = n->parent)
  {
    //按最佳粒子的树往回回溯
    ROS_DEBUG("  %.3f %.3f %.3f",
              n->pose.x,
              n->pose.y,
              n->pose.theta);
    if(!n->reading)
    {
      ROS_DEBUG("Reading is NULL");
      continue;
    }
    matcher.invalidateActiveArea(); //清空上一帧标注的活跃区域
    matcher.computeActiveArea(smap, n->pose, &((*n->reading)[0]));//计算活跃区域
    matcher.registerScan(smap, n->pose, &((*n->reading)[0])); //激光穿过的格子减概率,命中的格子加概率  注册扫描  是另一个新的matcher
  }

  // the map may have expanded, so resize ros message as well
  if(map_.map.info.width != (unsigned int) smap.getMapSizeX() || map_.map.info.height != (unsigned int) smap.getMapSizeY()) {

    // NOTE: The results of ScanMatcherMap::getSize() are different from the parameters given to the constructor
    //       so we must obtain the bounding box in a different way
    GMapping::Point wmin = smap.map2world(GMapping::IntPoint(0, 0));
    GMapping::Point wmax = smap.map2world(GMapping::IntPoint(smap.getMapSizeX(), smap.getMapSizeY()));
    xmin_ = wmin.x; ymin_ = wmin.y;
    xmax_ = wmax.x; ymax_ = wmax.y;
    
    ROS_DEBUG("map size is now %dx%d pixels (%f,%f)-(%f, %f)", smap.getMapSizeX(), smap.getMapSizeY(),
              xmin_, ymin_, xmax_, ymax_);

    map_.map.info.width = smap.getMapSizeX();
    map_.map.info.height = smap.getMapSizeY();
    map_.map.info.origin.position.x = xmin_;
    map_.map.info.origin.position.y = ymin_;
    map_.map.data.resize(map_.map.info.width * map_.map.info.height);
    //如果新地图超出边界则拓展原地图
    ROS_DEBUG("map origin: (%f, %f)", map_.map.info.origin.position.x, map_.map.info.origin.position.y);
  }

  //rviz的地图本来就是像素的,方便构建栅格地图
  for(int x=0; x < smap.getMapSizeX(); x++)
  {
    for(int y=0; y < smap.getMapSizeY(); y++)
    {
      /// @todo Sort out the unknown vs. free vs. obstacle thresholding
      GMapping::IntPoint p(x, y);
      double occ=smap.cell(p); //取点p处的活跃度
      assert(occ <= 1.0);
      if(occ < 0)
        map_.map.data[MAP_IDX(map_.map.info.width, x, y)] = -1; //未知
      else if(occ > occ_thresh_)
      {
        //map_.map.data[MAP_IDX(map_.map.info.width, x, y)] = (int)round(occ*100.0);
        map_.map.data[MAP_IDX(map_.map.info.width, x, y)] = 100;//概率超过设定阈值则记为占用
      }
      else
        map_.map.data[MAP_IDX(map_.map.info.width, x, y)] = 0;//否则视为自由
    }
  }
  got_map_ = true;

  //make sure to set the header information on the map
  map_.map.header.stamp = ros::Time::now();
  map_.map.header.frame_id = tf_.resolve( map_frame_ );

  sst_.publish(map_.map);     //发布完整地图, 给rviz显示
  sstm_.publish(map_.map.info); //发布地图元数据
}

bool 
SlamGMapping::mapCallback(nav_msgs::GetMap::Request  &req,
                          nav_msgs::GetMap::Response &res)
{
  boost::mutex::scoped_lock map_lock (map_mutex_); //发布地图后解开互斥锁
  if(got_map_ && map_.map.info.width && map_.map.info.height)
  {
    res = map_;//返回数据
    return true;
  }
  else
    return false;
}

void SlamGMapping::publishTransform()
{
  map_to_odom_mutex_.lock();
  ros::Time tf_expiration = ros::Time::now() + ros::Duration(tf_delay_);
  tfB_->sendTransform( tf::StampedTransform (map_to_odom_, tf_expiration, map_frame_, odom_frame_)); //在线程中发布在laserCallback中定义的从地图到里程计的变换 耦合这一块
  map_to_odom_mutex_.unlock();
  
}
