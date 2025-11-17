#include "ros/ros.h"
#include "tf2_ros/transform_listener.h"
#include "geometry_msgs/PointStamped.h"
#include "tf2_ros/buffer.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.h"

int main(int argc,char* argv[])
{
    setlocale(LC_ALL,"");
    ros::init(argc,argv,"tf_sub");
    ros::NodeHandle nh;
    //创建订阅对象
    tf2_ros::Buffer buffer;
    tf2_ros::TransformListener sub(buffer);
    //创建一个坐标点
    geometry_msgs::PointStamped p1;

    p1.header.frame_id = "son1";
    p1.header.stamp = ros::Time::now();
    p1.point.x = 1.0;
    p1.point.y = -1.0;
    p1.point.z = 3.0;
//多坐标变换
    ros::Rate rate(1);
    while(ros::ok())
    {
        rate.sleep();
        geometry_msgs::TransformStamped son1Toson2 = buffer.lookupTransform("son1","son2",ros::Time(0));     
        //计算son1中一个点在son2中的坐标
        geometry_msgs::PointStamped p1Atson2 = buffer.transform(p1,"son2");
        ROS_INFO("%f   %f    %f",p1Atson2.point.x,p1Atson2.point.y,p1Atson2.point.z);
        ros::spinOnce();
    }
    
    return 0;
}