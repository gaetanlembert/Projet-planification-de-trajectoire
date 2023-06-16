#include <ros/ros.h>
#include "Transformation.h"
#include <aruco_msgs/MarkerArray.h>
#include <visualization_msgs/MarkerArray.h>

ros::Publisher pub_points;

void init_marker( visualization_msgs::Marker& m)
{
    m.header.frame_id = "map";
    m.type = 2;
    
    m.pose.position.x = 0;
    m.pose.position.y = 0;
    m.pose.position.z = 0;
    
    m.pose.orientation.x = 0;
    m.pose.orientation.y = 0;
    m.pose.orientation.z = 0;
    m.pose.orientation.w = 1;

    m.scale.x = 0.05;
    m.scale.y = 0.05;
    m.scale.z = 0.05; 
    
    m.color.r = 1.0;
    m.color.g = 0.0;
    m.color.b = 0.0;
    m.color.a = 1.0;
    
}

// fonction appeler des qu'on recoit un message de aruco
void callback(const aruco_msgs::MarkerArray& msg)
{
    visualization_msgs::MarkerArray Marray;
    
    for (int i=0;i<msg.markers.size();i++)
    {
        Transformation T(msg.markers[i].pose.pose);
        Eigen::Matrix<double,3,1> Ecart(-0.5,0,0);
        Eigen::Matrix<double,3,1> P = T.position + T.rotation*Ecart;
        
        visualization_msgs::Marker m;
        init_marker(m);
        m.id = msg.markers[i].id;
        m.pose.position.x = P(0);
        m.pose.position.y = P(1);
        m.pose.position.z = P(2);
        
        Marray.markers.push_back(m);
    }
    
    pub_points.publish(Marray);
}


int main(int argc, char** argv){

    
    ros::init(argc, argv, "way_points");

    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe("/aruco_virtual_line/transformations", 10, callback);    
    
    pub_points = nh.advertise<visualization_msgs::MarkerArray>("/aruco_virtual_line/way_points", 1000);
    
    //à ajouter publication des points passage pour RViz
    
    ros::spin();
    return 0;
};


