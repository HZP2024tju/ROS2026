#include "ros/ros.h"
#include "turtlesim/Pose.h"
#include "tf2_ros/buffer.h"
#include "tf2_ros/transform_broadcaster.h"
#include "geometry_msgs/TransformStamped.h"
#include "tf2/LinearMath/QuadWord.h"

void do_pose(const turtlesim::Pose::ConstPtr& pose)
{
    static tf2_ros::TransformBroadcaster pub;
//动态坐标转换发布
    geometry_msgs::TransformStamped tf;
    tf.header.frame_id = "world";
    tf.header.stamp = ros::Time::now();
    tf.child_frame_id = "turtle1";
    tf.transform.translation.x = pose->x;
    tf.transform.translation.y = pose->y;
    tf.transform.translation.z = 0.0;

    tf2::Quaternion qtn;
    qtn.setRPY(0,0,pose->theta);

    tf.transform.rotation.w = qtn.getW();
    tf.transform.rotation.x = qtn.getX();
    tf.transform.rotation.y = qtn.getY();
    tf.transform.rotation.z = qtn.getZ();

    pub.sendTransform(tf);//当不重入时为静态转换

}


int main(int argc,char * argv[])
{
    setlocale(LC_ALL,"");
    ros::init(argc,argv,"follow");

    ros::NodeHandle nh;
    ros::Time::init();
    ros::Duration(3).sleep();
    ros::Subscriber sub = nh.subscribe("/turtle1/pose",10,do_pose);
    
    ros::spin();
    return 0;
}