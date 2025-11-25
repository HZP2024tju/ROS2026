#include "ros/ros.h"
#include "turtlesim/Spawn.h"

int main(int argc,char* argv[])
{
    ros::init(argc,argv,"more_tur");
    ros::NodeHandle nh;
    
    //创建客户端
    ros::ServiceClient client = nh.serviceClient<turtlesim::Spawn>("/spawn");
    ros::service::waitForService("/spawn");

    turtlesim::Spawn turtle;
    turtle.request.x = 1.0;
    turtle.request.y = 1.0;
    turtle.request.theta = 1;
    turtle.request.name = "pet";  
    
    bool flag = client.call(turtle);
    if(flag)
    {
        ROS_INFO("success");
    }
    else{
        ROS_INFO("fail");
    }
    return 0;
}