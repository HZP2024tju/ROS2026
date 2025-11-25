#include "ros/ros.h"
#include "geometry_msgs/TransformStamped.h"
#include "geometry_msgs/Twist.h"
#include "tf2_ros/buffer.h"
#include "geometry_msgs/PointStamped.h"
#include "tf2_ros/transform_listener.h"
#include "math.h"

int main(int argc,char *argv[])
{
    ros::init(argc,argv,"sub_tf");
    ros::NodeHandle nh;
    tf2_ros::Buffer buf;
    tf2_ros::TransformListener sub(buf);

    geometry_msgs::PointStamped T1;
    ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("/turtle2/cmd_vel",1000);
    ros::Rate rate(10);
    ros::Time::init();
    //ros::service::waitForService("tf/FrameGraph");
    ros::Duration(2).sleep();
    while (ros::ok())
    {
         rate.sleep();
         
         
            geometry_msgs::TransformStamped tfs = buf.lookupTransform("turtle2","turtle1",ros::Time(0));
           geometry_msgs::Twist twist;
           twist.linear.x = 0.9*sqrt(pow(tfs.transform.translation.x,2)+pow(tfs.transform.translation.y,2)); 
          twist.angular.z =   atan2(tfs.transform.translation.y,tfs.transform.translation.x);
        
        pub.publish(twist);
     ros::spinOnce();
    }
    
}