#include "ros/ros.h"
#include "geometry_msgs/Twist.h" //这个一定要加
int main(int argc,char* argv[])
{
    ros::init(argc,argv,"control");
    ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel",10);
    ros::Rate rate(10);

    geometry_msgs::Twist wist;
    wist.angular.x = 0;
    wist.angular.y = 0;
    wist.angular.z = 3;
    wist.linear.x = 5;
    wist.linear.y = 0;
    wist.linear.z = 0;
    while (ros::ok())
    {

        pub.publish(wist);
        rate.sleep();
        ros::spinOnce();
        /* code */
    }
    

    return 0;
}