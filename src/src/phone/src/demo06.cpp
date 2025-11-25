#include "ros/ros.h"
#include "phone/link.h"



int main(int argc,char *argv[])
{
    setlocale(LC_ALL,"");

    //传参为文件路径 参数一 参数二
    if(argc != 3)
    {
        ROS_ERROR("乱交数据闹麻了");
        return 1;
    }


    ros::init(argc,argv,"srv_acc");
    ros::NodeHandle nh;
    ros::ServiceClient usr =nh.serviceClient<phone::link>("link");//usr端需要范形 srv端需要

    ros::service::waitForService("link");

    phone::link srv;
    srv.request.num1 = atoi(argv[1]);
    srv.request.num2 = atoi(argv[2]);
    
    bool flag = usr.call(srv);
     if (flag)
    {
        ROS_INFO("请求正常处理,响应结果:%d",srv.response.sum);
    }
    else
    {
        ROS_ERROR("请求处理失败....");
        return 1;
    }
    return 0;
}