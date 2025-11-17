#include "ros/ros.h"

int main(int argc,char *argv[])
{
    setlocale(LC_ALL,"");
    ros::init(argc,argv,"get_data");
    ros::NodeHandle nh;
    double radiu = nh.param("radius",1.00);//注意默认值会影响取值
    //ros::spin();
    double redius2 = 0;
    bool result = nh.getParamCached("rdius",redius2);//getParam


    if(result)
    {
      ROS_INFO("Radius : %.2f\n",redius2);
    }
    else
    {
        ROS_INFO("变量不存在\n");
    }

    std::vector<std::string> names;
    nh.getParamNames(names);
    for(auto && name:names)
    {
        ROS_INFO("遍历到 :%s",name.c_str());
    }

    bool flag1=nh.hasParam("radius");
    bool flag2=nh.hasParam("radiusxx");

    
    return 0;
}