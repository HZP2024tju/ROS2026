#include "ros/ros.h"

void damn(const ros::TimerEvent& event)
{
    ROS_INFO("damndamndamn!! %f",event.current_real.toSec());

}

int main(int argc,char * argv[])
{
    setlocale(LC_ALL,"");

    ros::init(argc,argv,"hello_time");
    ros::NodeHandle nh;

    ros::Time  right_now = ros::Time::now();
    ros::Time T34(0,0);

    ros::Timer T54 = nh.createTimer(ros::Duration(1),damn,false,false);
    T54.start();
    while (ros::ok())
    {
        ros::Time  right_now = ros::Time::now();
      //  ROS_INFO("现在是北京时间 %f",T34.toSec());

        /* code */
        ros::spinOnce();
    }
    
    //ROS_INFO("");

    return 0;
}