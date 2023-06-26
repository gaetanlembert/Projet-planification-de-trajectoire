#include <ros/ros.h>
#include <aruco_msgs/MarkerArray.h>
#include <geometry_msgs/Transform.h>
#include <visualization_msgs/MarkerArray.h>
#include <aruco_virtual_line/Trajectory.h>
#include <nav_msgs/Path.h>

ros::Publisher pub_traj, pub_path;

// fonction appeler des qu'on recoit un message de aruco
void callback(const visualization_msgs::MarkerArray& msg)
{
    aruco_virtual_line::Trajectory Traj;
    Traj.nbs = msg.markers.size();
    
//    Traj.X.push_back(0);
//    Traj.X.push_back(1);
//    Traj.Y.push_back(0);
//    Traj.Y.push_back(2);
    
    float xi[Traj.nbs];
    float yi[Traj.nbs];
    
    float xs[100*Traj.nbs];
    float ys[100*Traj.nbs];
    
    int s;
    int p=3;
    
    float B[Traj.nbs][p+1][100*Traj.nbs];
    
    float M[100*Traj.nbs][Traj.nbs];
    float N[Traj.nbs][Traj.nbs];
    float NI[Traj.nbs][Traj.nbs];
    
    float detM;
    
    float Pi[Traj.nbs]
    
    //constructions des vecteurs x(i) et y(i)
    for(int i=0; i<Traj.nbs; i++)
    {
        xi[i]=msg.markers[i].pose.pose.x;
        yi[i]=msg.markers[i].pose.pose.x;
    }
    
    //construction du vecteur x(s) et coeff d'ordre 0
    for(int i=0; i<Traj.nbs; i++)
    {
        for(int j=0; j<100; j++)
        {
            s=i+j;
            
            xs[s]=xi[i-1]+(xi[i]-xi[i-1])*j/100;
            
            if(xs[s]<xi[i] && xs[s]>=xi[i-1])
            {
                B[i][0][s]=1;
            }
            else
            {
                B[i][0][s]=0;
            }
        }
    }
    
    //calcul des coeff d'ordre 1 à p
    for(int q=1; q<p+1; q++)
    {
        for(int i=0; i<Traj.nbs; i++)
        {
            for(int j=0; j<100; j++)
            {
                s=i+j;
                
                B[i][q][s]=(xs[s]-xi[i])/(xi[i-q]-xi[i])*B[i][q-1][s]+(xi[i+q+1]-xs[s])/(xi[i+q+1]-x[i+1])*B[i+1][q-1][s];
            }
        }
    }
    
    //construction des matrices M et N
    for(int i=0; i<Traj.nbs; i++)
    {
        for(int j=0; j<100; j++)
        {
            s=i+j;
                
            M[s][i]=B[i][p][s];
                
            if(j==0)
            {
                for(int q=0; q<=p+1; q++)
                {
                    N[i][i-q]=B[i-q][p][s];
                }
            }
        }
    }
    
    NI=inv(N);
    
    Pi=yi*NI;
    
    ys=Pi*M;
    
    for(int s=0; s<100*Traj.nbs; s++)
    {
        Traj.X[s]=xs[s];
        Traj.X[s̉]ys[s];
    }
    
    pub_traj.publish(Traj);
    
    
    
    // creation du message
    nav_msgs::Path p;
    p.header.frame_id = "map";
    
    uint nb = msg.markers.size();
    for (int i=0;i<nb;i++)
    {
        // fake equation
        geometry_msgs::PoseStamped pose;
        pose.pose = msg.markers[i].pose;
       
        p.poses.push_back(pose);
    }
    pub_path.publish(p);
}


int main(int argc, char** argv){

    
    ros::init(argc, argv, "trajectory");

    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe("/aruco_virtual_line/way_points", 10, callback);    
    
    pub_traj = nh.advertise<aruco_virtual_line::Trajectory>("/aruco_virtual_line/trajectory", 1000);
    
    pub_path = nh.advertise<nav_msgs::Path>("/aruco_virtual_line/path", 1000);
    
    //à ajouter publication d'une trajectoire lisible par RVIZ
    
    ros::spin();
    return 0;
};


