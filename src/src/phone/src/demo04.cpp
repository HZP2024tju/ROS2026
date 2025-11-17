
#include "ros/ros.h"
#include "phone/person.h"

void doPerson(const phone::person::ConstPtr& person_p){
    ROS_INFO("订阅的人信息:%s, %d, %.2f", person_p->name.c_str(), person_p->age, person_p->hight);
}

int main(int argc, char *argv[])
{   
    setlocale(LC_ALL,"");

    //1.初始化 ROS 节点
    ros::init(argc,argv,"listener_person");
    //2.创建 ROS 句柄
    ros::NodeHandle nh;
    //3.创建订阅对象
    ros::Subscriber sub = nh.subscribe<phone::person>("liaotian",10,doPerson);//必须要有回调

    //4.回调函数中处理 person

    //5.ros::spin();
    ros::spin();    
    return 0;
}