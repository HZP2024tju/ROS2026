#include "ros/ros.h"
#include "std_msgs/String.h"
#include "phone/person.h"


int main(int argc,char* argv[])
{
    setlocale(LC_ALL,"");
    ros::init(argc,argv,"msg_test");
    ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<phone::person>("liaotian",10);
    ROS_INFO("now we publish message");
    phone::person damn;
    damn.name = "is2";
    damn.age = 122;
    damn.hight = 183;

    ros::Rate rate(1);

    while (ros::ok())
    {
        damn.age++;
        pub.publish(damn);
        ROS_INFO("publishing: %s,%d,%.3f",damn.name.c_str(),damn.age,damn.hight);
        rate.sleep();

        ros::spinOnce();
        /* code */
    }
    
    return 0;
}