#include "ros/ros.h"

int main(int argc,char*argv[])
{
    ros::init(argc,argv,"delet" );
    ros::NodeHandle nh;

    bool flag1 = nh.deleteParam("radius");

    bool flag2 =ros::param::del("type");

    if(flag1)
    {
        ROS_INFO("success");
    }
    else
    {
        ROS_INFO("fail");
    }
    if(flag2)
    {
        ROS_INFO("success");
    }
    else
    {
        ROS_INFO("fail");
    }
}