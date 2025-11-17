#include "ros/ros.h"
#include "std_msgs/String.h"

void doMsg(const std_msgs::String::ConstPtr &msg)
{
    ROS_INFO("recive: %s",msg->data.c_str());
}


int main(int argc,char* argv[])
{
    ros::init(argc,argv,"recive");
    ros::NodeHandle nh;
    ros::Subscriber sub = nh.subscribe("calling",10,doMsg);

    ros::spin();
    //run again
    return 0;
}