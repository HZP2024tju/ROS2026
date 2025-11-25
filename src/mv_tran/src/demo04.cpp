#include "ros/ros.h"
#include "turtlesim/Spawn.h"
#include "turtlesim/Pose.h"

int main(int argc,char* argv[])
{
    ros::init(argc,argv,"T1");
    ros::NodeHandle nh;
    //创建客户端
    ros::ServiceClient cli = nh.serviceClient<turtlesim::Spawn>("/spawn");
    ros::service::waitForService("/spawn");

    turtlesim::Spawn T1;
    T1.request.name = "turtle2";
    T1.request.theta = 3;
    T1.request.x = 1.0;
    T1.request.y = 1.2;
    
    bool flag = cli.call(T1);
}   