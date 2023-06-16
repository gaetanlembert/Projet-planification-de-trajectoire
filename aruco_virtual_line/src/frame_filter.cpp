#include <ros/ros.h>
#include <aruco_msgs/MarkerArray.h>
#include <geometry_msgs/Transform.h>
#include <aruco_virtual_line/Transforms.h>
#include "Transformation.h"

ros::Publisher pub_localisation;
ros::Publisher pub_transform;


aruco_msgs::MarkerArray Ts;
tf::TransformBroadcaster *br;
std::vector<tf::StampedTransform> frame_vector;

// fonction appeler des qu'on recoit un message de aruco
void callback(const aruco_msgs::MarkerArray& msg)
{
    frame_vector.clear();       // send to TF
    
    
//     std::cout<<"We found "<< msg.markers.size()<< " markers." <<std::endl;
    bool camera_localized = false;
    Transformation camera_pose;
    
    for (int i=0;i<msg.markers.size();i++)
    {
        const aruco_msgs::Marker &m = msg.markers[i];
        
        if (m.id == 162)
        {
            std::cout<<"ID 162 : indice : "<< m.confidence<<std::endl;
        }
        
        // check if it is already known;
        for (uint j=0;j<Ts.markers.size();j++)
        {
            if (m.id == Ts.markers[j].id && !camera_localized)
            {
//                 std::cout<<"We localise the camera thaks to marker "<< m.id <<std::endl;
                Transformation MarkerInCamera(m.pose);
                Transformation MarkerInWorld(Ts.markers[j].pose);
                camera_pose = MarkerInWorld * MarkerInCamera.inverse();
                camera_localized = true;
                frame_vector.push_back(tf::StampedTransform(camera_pose.convertToTF(), ros::Time::now(),"map","robot")); // send to TF
            }
        }
    }
    
    if (camera_localized)
    {
        for (int i=0;i<msg.markers.size();i++)
        {
            const aruco_msgs::Marker &m = msg.markers[i];
            
            bool knwon_marker = false;
            for (uint j=0;j<Ts.markers.size();j++)
            {
                if (m.id == Ts.markers[j].id)
                {
                    knwon_marker = true;
                    break;
                }
            }
            if (!knwon_marker)
            {
//                 std::cout<<"We add new marker: "<< m.id <<std::endl;
                Transformation MarkerInCamera(m.pose);
                Transformation MarkerInWorld = camera_pose * MarkerInCamera;
                aruco_msgs::Marker M;
                M.id = m.id;
                M.pose.pose = MarkerInWorld.converToPose();
                Ts.markers.push_back(M);
            }
        }        
    }else
    {
//         std::cout<<"The camera is not localized"<<std::endl;
    }
    
    aruco_msgs::Marker Robot;
    Robot.pose.pose = camera_pose.converToPose();
    pub_localisation.publish(Robot);    
    
    pub_transform.publish(Ts);
    
    for (int i=0;i<Ts.markers.size();i++)
    {
        Transformation trans (Ts.markers[i].pose);
         frame_vector.push_back(tf::StampedTransform(trans.convertToTF(), ros::Time::now(),"map","marker_"+std::to_string(Ts.markers[i].id)));  // send to TF
    }
    
    // envoie les informations sur /tf
    br->sendTransform(frame_vector);
}


int main(int argc, char** argv){    
    ros::init(argc, argv, "frame_filter");
    
    br = new tf::TransformBroadcaster;
    
    aruco_msgs::Marker Mzero;
    Mzero.id = 0;
    Transformation zero(0,0,0,90,0,0);
    Mzero.pose.pose = zero.converToPose();
    Ts.markers.push_back(Mzero);
    
    

    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe("/aruco_marker_publisher/markers", 10, callback);    
    
    pub_localisation = nh.advertise<aruco_msgs::Marker>("/aruco_virtual_line/robot_transform", 1000);
    pub_transform = nh.advertise<aruco_msgs::MarkerArray>("/aruco_virtual_line/transformations", 1000);
    
    //à ajouter publication des marqueurs sur TF
    
    ros::spin();
    return 0;
};


