#include "ros/ros.h"
#include "tf2_ros/buffer.h"
#include "tf2_ros/transform_listener.h"
#include "geometry_msgs/PointStamped.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.h"

int main(int argc,char *argv[])
{
    setlocale(LC_ALL,"");
    ros::init(argc,argv,"tf_sub");

    tf2_ros::Buffer buf;
    tf2_ros::TransformListener lis(buf);
    geometry_msgs::PointStamped pot;
    pot.header.stamp = ros::Time::now();
    pot.header.frame_id = "radar";
    pot.point.x = -1.0;
    pot.point.y = -3.0;
    pot.point.z = -2.0;
   // ros::Duration(2).sleep();
    ros::Rate rate(1);
    while (ros::ok())
    {
        geometry_msgs::PointStamped rec;
        rate.sleep();
        rec = buf.transform(pot,"main");

        ROS_INFO("-->%f   %f    %f",rec.point.x,rec.point.y,rec.point.z);
        ros::spinOnce();
    }

    return 0;
}