#include "ros/ros.h"
#include "tf2_ros/static_transform_broadcaster.h"
#include "geometry_msgs/TransformStamped.h"
#include "tf2/LinearMath/Quaternion.h"

int main(int argc, char * argv[])
{
    setlocale(LC_ALL,"");
    ros::init(argc,argv,"TranF");
    ros::NodeHandle nh;
    tf2_ros::StaticTransformBroadcaster pub;
    geometry_msgs::TransformStamped tfs;
    tfs.header.stamp = ros::Time::now();
    tfs.header.frame_id = "main";
    tfs.child_frame_id = "camera";
    tfs.transform.translation.x = -1.0;
    tfs.transform.translation.y = 1.0;
    tfs.transform.translation.z = 1.0;

    tf2::Quaternion qtn;

    qtn.setRPY(0,0,7.1415926);
    


    tfs.transform.rotation.w = qtn.getW();
    tfs.transform.rotation.x = qtn.getX();
    tfs.transform.rotation.y = qtn.getY();
    tfs.transform.rotation.z = qtn.getZ();

    pub.sendTransform(tfs);
    ros::spin();
    return 0;
}