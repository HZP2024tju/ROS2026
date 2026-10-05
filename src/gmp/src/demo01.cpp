#include "nav_msgs/GetMap.h"
#include "ros/ros.h"
#include "nav_msgs/SetMap.h"
#include "nav_msgs/GetMap.h"
#include "tf/transform_broadcaster.h"
#include "tf/transform_listener.h"

#define map_hight 10
#define map_width 10

#define MAP_IDX(b,c) (map_width*b + c)

int8_t map_meta[map_hight * map_width] = {0};

int main(int argc,char* argv[])
{
    ros::init(argc,argv,"gmp_t1");
    ros::NodeHandle nh;
    ros::Publisher pub=nh.advertise<nav_msgs::OccupancyGrid>("map",10);
    nav_msgs::OccupancyGrid map;
    map.info.height = map_hight;
    map.info.width= map_width;
    map.info.resolution=1;
    map.header.frame_id = "map";
    map.header.stamp=ros::Time::now();
    map.data.reserve(map_width * map_hight);


    map.info.origin.position.x = -map_hight/2;
    map.info.origin.position.y = -map_width/2;
    map.info.origin.position.z = 0.0;
    map.info.origin.orientation.x = 0.0;
    map.info.origin.orientation.y = 0.0;
    map.info.origin.orientation.z = 0.0;
    map.info.origin.orientation.w = 1.0;


    tf::TransformListener tf_;
    tf::StampedTransform transform;
    transform.getRotation().getX();
  //  tf_.transformPose()
    
    nav_msgs::MapMetaData meta;

    for (int i = 0; i < map_hight; i++) {
        for (int j = 0; j < map_width; j++) {
            // Initialize map with some value (for example, 0 for free space and 100 for occupied space)
            if (i == j) {
                map.data.push_back(100);  // Occupied
            } else {
                map.data.push_back(-1);  // Free space
            }
        }
    }
    
    map.data[MAP_IDX(0,5)] = (100);
    while (ros::ok())
    {
        map.header.stamp = ros::Time::now();
        pub.publish(map);
        ros::spinOnce();
    }
    
  // map.data.push_back(map_meta);

}