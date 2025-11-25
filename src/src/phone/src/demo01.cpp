#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>

int main(int argc,char *argv[])
{
    ros::init(argc,argv,"Call");
    ros::NodeHandle nh;//node handlers
   //creat publisher it is topic call
    ros::Publisher pub = nh.advertise<std_msgs::String>("calling",10);//topic nameB
    std::string msg_front = "hello ---->";
    std_msgs::String msg;
    ros::Rate rate(1);//HZ

    
    ros::Duration(3).sleep();
    int count = 0;
    while(ros::ok)
    {
        rate.sleep();
        count++;
    std::stringstream ss;
        ss<<msg_front<< count;
        msg.data = ss.str();
        pub.publish(msg);
        ROS_INFO("sending data is  %s",ss.str().c_str());
        ros::spinOnce();
    }

    return 0;
}