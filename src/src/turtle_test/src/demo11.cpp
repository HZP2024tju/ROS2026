#include "ros/ros.h"
#include "turtlesim/Pose.h"

void doturtle(const turtlesim::Pose::ConstPtr& pose)
{
    ROS_INFO("derection of turtle: (%.2f,%.2f),delta:%.2f,W:%.2f",pose->x,pose->y,pose->theta,pose->angular_velocity);

}


int main(int argc,char* argv[])
{
    ros::init(argc,argv,"tur_sub");

    ros::NodeHandle nh;
    ros::Subscriber sub =  nh.subscribe("/turtle1/pose",100,doturtle);  
    ros::spin();
    return 0;
}