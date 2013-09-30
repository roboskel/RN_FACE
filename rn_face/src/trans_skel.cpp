#include <stdio.h>
#include <time.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <cmath>
#include <iterator>
#include <vector>
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Float64MultiArray.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Header.h>
#include <math.h>
#include <tf/transform_listener.h>
#include <tf/transform_broadcaster.h>
#include <cmath>
#include <chrono>
#include <ctime>
#include "rn_face/head_cords_m.h"
#include "rn_face/head_cords_m_array.h"


#define NUM_READINGS 1128;

std::chrono::time_point<std::chrono::system_clock> p1;
using namespace std;
time_t t;

long int timer ;
double seconds ;

ros::Time skel_cur_time, skel_last_time;
ofstream file;
int rec = 1 ;
int REC = 0 ;
//std::string pathskel=".rec/skel_data.txt";
std::string head ="head_";

std::string skel_ts;
std::stringstream sstream ;
std::stringstream sstream2 ;


struct head_cords 
{
	double x[6];
	double y[6];
	double z[6];
	double x_prev[6];
	double y_prev[6];
	double z_prev[6];
	head_cords(): x({0,0,0,0,0,0}),
				y({0,0,0,0,0,0}),
				z({0,0,0,0,0,0}),
				x_prev({0,0,0,0,0,0}),
				y_prev({0,0,0,0,0,0}),
				z_prev({0,0,0,0,0,0}){}
}heads;

void callback(const std_msgs::Float64::ConstPtr& msg)
{
    float start = msg->data;
     //START gia na 3ekinhsei to record
		//X gia na stamathsei
    //printf("Press Start to Start Recording \n");
    ROS_INFO("STARTED SKEL REC");
    if (start==1)
    {	
		//printf("Started Recording Skeleton\n");
        REC = 1;
       //ros::Duration().sleep();

    }
    else if ((REC==1)&&(start==0))
    {
        REC = 0;
        //printf("Stopped Recording Skeleton \n");
		ros::shutdown();
    }
}



int main (int argc, char** argv)
{	
	ros::init(argc, argv, "head_cords_recorder");
	ros::NodeHandle node;
	ros::Subscriber gp_in;
	ros::Publisher  skel_data;
	tf::TransformListener listener;
	tf::Vector3 origin;
	tf::StampedTransform transform;
	//geometry_msgs::Vector3 cord;
	gp_in = node.subscribe("coms",5,callback);
	skel_data = node.advertise<rn_face::head_cords_m_array>("head_cords",1);
	skel_last_time = ros::Time::now();
	rn_face::head_cords_m_array msg_ar;
	int i ;
	int rec[6]={0,0,0,0,0,0};
	int no_faces=0;
	while (node.ok())
	{	
		//ros::Duration(0.01).sleep();
		msg_ar.head_co.clear();
		//ROS_INFO("IN WHILE");
		skel_cur_time=ros::Time::now();
		seconds=skel_cur_time.toSec()-skel_last_time.toSec();
		if(true)
		{	
			//ROS_INFO("STARTED");
		no_faces=0;		
		p1 = std::chrono::system_clock::now();
		sstream2 << std::chrono::duration_cast<std::chrono::milliseconds>(p1.time_since_epoch()).count() ;
		sstream2>>skel_ts;
		
			for(i=1;i<=6;i++)
			{
				sstream << i;
				head=head+sstream.str();
				//std::cout << "Follow this command:" << head << endl;
				//ROS_INFO("OEOEOEOE");
				//ros::Duration(1).sleep();
				rn_face::head_cords_m msg;
				try{
				  listener.lookupTransform("/openni_depth_frame", head,
										   ros::Time(0), transform);
					
					origin =transform.getOrigin();
					heads.x[i-1]=origin.x();
					heads.y[i-1]=origin.y();
					heads.z[i-1]=origin.z();
					//printf("X:%f  Y:%f   Z:%f",origin.x(),origin.y(),origin.z());
					ros::Duration(1).sleep();
					//ROS_INFO("TRY");
					//ros::Duration(1).sleep();
					if ((heads.x[i-1]!=heads.x_prev[i-1])&&(heads.y[i-1]!=heads.y_prev[i-1])&&(heads.z[i-1]!=heads.z_prev[i-1]))
					{
						//ROS_INFO("IF");
						rec[i-1] =1;
						heads.x_prev[i-1] = heads.x[i-1];
						heads.y_prev[i-1] = heads.y[i-1];
						heads.z_prev[i-1] = heads.z[i-1];
						no_faces++;
						//ros::Duration(1).sleep();
					}
					else
					{	//ROS_INFO("ELSE");
						heads.x_prev[i-1] = heads.x[i-1];
						heads.y_prev[i-1] = heads.y[i-1];
						heads.z_prev[i-1] = heads.z[i-1];
						rec[i-1]=0;
						//ros::Duration(1).sleep();

					}
				}
				catch (tf::TransformException ex)
				{
					//ROS_INFO("CATCH");
					rec[i-1]=0;
				}
				if(rec[i-1]==0)
				{
					msg.timestamp=stol(skel_ts);
					msg.id=i-1;
					msg.x=0;
					msg.y=0;
					msg.z=0;
				}
				else
				{
				msg.timestamp=stol(skel_ts);
				msg.id=i-1;
				msg.x=heads.x[i-1];
				msg.y=heads.y[i-1];
				msg.z=heads.z[i-1];
				}
				msg_ar.head_co.push_back(msg);
				head ="head_";
				sstream.str(std::string());
				sstream.clear();
			
		  }
		  msg_ar.no_faces=no_faces;
		  skel_data.publish(msg_ar);
		  skel_ts.clear();
		  sstream2.str(std::string());
		  sstream2.clear();
		//ROS_INFO("AFTER CALL");
		}
		skel_last_time=skel_cur_time;
		//ROS_INFO("OUTSIDE LOOP");
		skel_cur_time=ros::Time::now();
		ros::spinOnce();
		//ROS_INFO("SPINNED");	
	}
	ros::shutdown();
//file.close();
  return 0;
}
