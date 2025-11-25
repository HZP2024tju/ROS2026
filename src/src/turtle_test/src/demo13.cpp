#include "ros/ros.h"

int main(int argc,char* argv[])
{
    ros::init(argc,argv,"tursrv");
  ros::NodeHandle nh("turtle");
    //ros::NodeHandle nh;

     //ros::param::set("/turtlesim/background_r",0);
     //ros::param::set("/turtlesim/background_g",0);
     //ros::param::set("/turtlesim/background_b",0);

    nh.setParam("background_r",100);
    nh.setParam("background_g",0);
    nh.setParam("background_b",0);

}