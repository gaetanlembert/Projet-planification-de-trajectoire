#include <ros/ros.h>
#include <aruco_msgs/MarkerArray.h>
#include <geometry_msgs/Transform.h>
#include <visualization_msgs/MarkerArray.h>
#include <aruco_virtual_line/Trajectory.h>

ros::Publisher pub_traj;

// fonction appeler des qu'on recoit un message de aruco
void callback(const visualization_msgs::MarkerArray& msg)
{
    aruco_virtual_line::Trajectory Traj;
    Traj.nbs = 2;
    Traj.X.push_back(0);
    Traj.X.push_back(1);
    Traj.Y.push_back(0);
    Traj.Y.push_back(2);
    pub_traj.publish(Traj);
}


int main(int argc, char** argv){

    
    ros::init(argc, argv, "trajectory");

    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe("/aruco_virtual_line/way_points", 10, callback);    
    
    pub_traj = nh.advertise<aruco_virtual_line::Trajectory>("/aruco_virtual_line/trajectory", 1000);
    
    //à ajouter publication d'une trajectoire lisible par RVIZ
    
    ros::spin();
    return 0;
};


