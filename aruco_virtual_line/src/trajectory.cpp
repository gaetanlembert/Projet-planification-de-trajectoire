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
    
    float s;
    int j,k;
    int p=3;
    float B[10*n][p+1];
    float M[10*n][n+2]
    
    for(s=0, s=1, s=s+1/(10*n)
    {
        for(j=0, j=10*n, j++)
        {
            if(x>msg.points[j-1].x and x<=msg.points[j].x)
            {
                B[j][0]=1
            }
            else
            {
                B[j][0]=0;
            }
        }
        for(k=1, k=p+1, k++)
        {
            for(i=0, i=n, i++)
            {
                for(j=0, j=10*n, j++)
                {
                    if(x>msg.points[j-1].x and x<=msg.points[j].x)
                    {
                        B[j][k]=(x-msg.points[i].x)/(msg.points[i+k].x-msg.points[i].x)*B[j][k-1]+(msg.points[i+k+1].x-x)/[msg.points[i+k+1].x-msg.points[i+1].x)*B[j+1][k-1];
                    }
                    else
                    {
                        
                    }
                }
            }
        }for
    }
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


