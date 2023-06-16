#include <ros/ros.h>
#include <aruco_msgs/MarkerArray.h>
#include <geometry_msgs/Transform.h>
#include <aruco_virtual_line/Trajectory.h>
#include <aruco_virtual_line/Error.h>

ros::Publisher pub_error;

// fonction appeler des qu'on recoit un message de aruco
void callback_traj(const aruco_virtual_line::Trajectory& msg)
{
    aruco_virtual_line::Trajectory T;
    
    aruco_virtual_line::Error E;
    E.distance = 0.1;
    E.alpha = 0.0;
    
    pub_error.publish(E);
}

// change get the position of therobot throuht TF
// void callback_robot(const geometry_msgs::Transform& msg)
// {
// 
// }


int main(int argc, char** argv){

    
    ros::init(argc, argv, "localisation");

    ros::NodeHandle nh;

    ros::Subscriber sub_traj = nh.subscribe("/aruco_virtual_line/trajectory", 10, callback_traj);
    
    pub_error = nh.advertise<aruco_virtual_line::Error>("/aruco_virtual_line/error", 1000);
    
    //à ajouter publication des points passage pour RViz
    
    ros::spin();
    return 0;
};


