#include <ros/ros.h>
#include <aruco_msgs/MarkerArray.h>
#include <geometry_msgs/Transform.h>
#include <aruco_virtual_line/Transforms.h>
#include <aruco_virtual_line/WayPoints.h>

ros::Publisher pub_points;

// fonction appeler des qu'on recoit un message de aruco
void callback(const aruco_virtual_line::Transforms& msg)
{
    aruco_virtual_line::WayPoints P;
    
    geometry_msgs::Vector3 p1,p2,p3;
    p1.x = 0;
    p1.y = 0;
    p1.z = 0;

    p2.x = 0;
    p2.y = 1;
    p2.z = 0;

    p3.x = 1;
    p3.y = 1;
    p3.z = 0;
    
    P.points.push_back(p1);
    P.points.push_back(p2);
    P.points.push_back(p3);
    pub_points.publish(P);
}


int main(int argc, char** argv){

    
    ros::init(argc, argv, "way_points");

    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe("/aruco_virtual_line/transformations", 10, callback);    
    
    pub_points = nh.advertise<aruco_virtual_line::WayPoints>("/aruco_virtual_line/way_points", 1000);
    
    //à ajouter publication des points passage pour RViz
    
    ros::spin();
    return 0;
};


