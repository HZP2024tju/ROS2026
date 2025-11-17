#include "ros/ros.h"


int main(int argc,char* argv[])
{
    ros::init(argc,argv,"data_srv");

    ros::NodeHandle nh;
//方案 1
    nh.setParam("type","ZX-01");
    nh.setParam("radius",0.15);
//方案 2
    ros::param::set("type_param","ZX-01");

    ros::param::set("radius_param",0.15);
    
    return 0;
}