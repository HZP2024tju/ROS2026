#include "ros/ros.h"
#include "tf2_ros/transform_broadcaster.h"
#include "turtlesim/Pose.h"
#include "geometry_msgs/TransformStamped.h"
#include "tf2/LinearMath/Quaternion.h"

std::string turtle_name;

void do_pose(const turtlesim::Pose::ConstPtr pose)
{
     static tf2_ros::TransformBroadcaster broadcaster;
    //  6-2.将 pose 信息转换成 TransFormStamped
    geometry_msgs::TransformStamped tfs;
    tfs.header.frame_id = "world";
    tfs.header.stamp = ros::Time::now();
    tfs.child_frame_id = turtle_name;
    tfs.transform.translation.x = pose->x;
    tfs.transform.translation.y = pose->y;
    tfs.transform.translation.z = 0.0;
    tf2::Quaternion qtn;
    qtn.setRPY(0,0,pose->theta);
    tfs.transform.rotation.x = qtn.getX();
    tfs.transform.rotation.y = qtn.getY();
    tfs.transform.rotation.z = qtn.getZ();
    tfs.transform.rotation.w = qtn.getW();
    //  6-3.发布
    broadcaster.sendTransform(tfs);
}


int main(int argc,char* argv[])
{
    setlocale(LC_ALL,"");
    ros::init(argc,argv,"T_tran");
    if (argc != 2)
    {
        ROS_ERROR("launch: 请传入正确的参数");
    }
    else
    {
        turtle_name = argv[1];
    }
    
    ros::NodeHandle nh;
    ros::Subscriber sub = nh.subscribe<turtlesim::Pose>(turtle_name + "/pose",1000,do_pose);
    ros::spin();
    return 0;
}