#include <ros/ros.h>
#include <aruco_msgs/MarkerArray.h>
#include <geometry_msgs/Transform.h>
#include <aruco_virtual_line/Transforms.h>

ros::Publisher pub_localisation;
ros::Publisher pub_transform;

// fonction appeler des qu'on recoit un message de aruco
void callback(const aruco_msgs::MarkerArray& msg)
{
    geometry_msgs::Transform T;
    pub_localisation.publish(T);
    
    aruco_virtual_line::MarkerTransform M;
    M.id = 0;
    M.transform = T;
    
    aruco_virtual_line::Transforms Ts;
    Ts.transforms.push_back(M);
    pub_transform.publish(Ts);
}


int main(int argc, char** argv){

    
    ros::init(argc, argv, "frame_filter");

    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe("/aruco_marker_publisher/markers", 10, callback);    
    
    pub_localisation = nh.advertise<geometry_msgs::Transform>("/aruco_virtual_line/robot_transform", 1000);
    pub_transform = nh.advertise<aruco_virtual_line::Transforms>("/aruco_virtual_line/transformations", 1000);
    
    //à ajouter publication des marqueurs sur TF
    
    ros::spin();
    return 0;
};


