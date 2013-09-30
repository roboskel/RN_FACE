#include <stdio.h>
#include <time.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <cmath>
#include <ros/ros.h>
#include "std_msgs/String.h"
#include "std_msgs/Float64MultiArray.h"
#include "std_msgs/Float64.h"
#include <std_msgs/Float32.h>
#include <std_msgs/Header.h>
#include <sensor_msgs/LaserScan.h>
#include <chrono>
#include <ctime>

#define NUM_READINGS 1128

using namespace std;

float hok_read[NUM_READINGS] = {};
int REC = 0;
long int timer;
double seconds ,seconds_ ;

time_t t;
ros::Time hok_cur_time, hok_last_time;
ros::Time total_cur_time, total_time;
ofstream file;
std::stringstream sstream ;
std::string hok_ts;
std::chrono::time_point<std::chrono::system_clock> p1;

void hok_cb(const sensor_msgs::LaserScan::ConstPtr& scan)
  {
    //uint32_t timestamp = msg->header.stamp.sec;
	//uint32_t timestamp2 = msg->header.stamp.nsec;
	//float min = (float)msg->angle_min;
	
	
	std::vector<float> ranges = scan->ranges;
	if(REC==1)
	{	
	//TIME INTERVAL STUFF	
		total_cur_time=ros::Time::now();
		if(((total_cur_time.toSec())-(total_time.toSec()))>=10000)
		{
			float seconds_ = (total_cur_time.toSec())-(total_time.toSec());
			printf("sasfasf %f\n",seconds_);
			REC=0;
			ros::NodeHandle n;
			ros::Publisher turn_off;
			turn_off = n.advertise<std_msgs::Float64>("coms", 1);
			std_msgs::Float64 msg;
			msg.data=0;
			turn_off.publish(msg);
			turn_off.publish(msg);
			turn_off.publish(msg);
			turn_off.publish(msg);
			printf("Stopped Recording Hok\n");
			ros::shutdown();
		}
		
		
		hok_cur_time = ros::Time::now();
		seconds=hok_cur_time.toSec()-hok_last_time.toSec();
        if(seconds>0.1)
        {	
			file<<"Time ";
			p1 = std::chrono::system_clock::now();
			sstream << std::chrono::duration_cast<std::chrono::milliseconds>(
			p1.time_since_epoch()).count() ;
			//time(&t);
			//timer = (long)t;
            //sstream << timer << std::setprecision(9) << seconds;
            hok_ts = sstream.str();
            file<<hok_ts<<" ";
			for( std::vector<float>::size_type i = 0; i != ranges.size(); i++) 
			{
				file<<ranges[i]<<" ";
			}
			file<<endl ;
			hok_ts.clear();
			sstream.str(std::string());
			sstream.clear();
			hok_last_time=hok_cur_time;
			hok_cur_time=ros::Time::now();
	
		}
		
	}

  }


void chatterCallback(const std_msgs::Float64MultiArray::ConstPtr& msg)
{
    int start = msg->data[11]; //START gia na 3ekinhsei to record
    int cancel = msg->data[4]; //X gia na stamathsei
    //printf("Press Start to Start Recording \n");
    if (start==1)
    {	
		printf("Started Recording Hok\n");
        REC = 1;
        ros::Duration(1).sleep();

    }
    else if ((REC==1)&&(cancel==1))
    {
        REC = 0;

        printf("Stopped Recording Hok \n");
		ros::shutdown();
    }
}

void callback(const std_msgs::Float64::ConstPtr& msg)
{
    float start = msg->data; //START gia na 3ekinhsei to record
		//X gia na stamathsei
    //printf("Press Start to Start Recording \n");
    		

    if (start==1)
    {	
		printf("Started Recording Hok\n");
		//printf("Started Recording Skeleton\n");
		total_time = ros::Time::now();
        REC = 1;
        ros::Duration(1).sleep();

    }
    else if ((REC==1)&&(start==0))
    {	
		printf("Stopped Recording Hok\n");
        REC = 0;
        //printf("Stopped Recording Skeleton \n");
		ros::shutdown();
    }
}

int main(int argc, char** argv)
{	
    file.open ("./_laser.data", ios::out | ios::binary);
    ros::init(argc, argv, "recorder");
    ros::NodeHandle n;
    ros::Subscriber gp_in,hok_in;
    ros::Publisher turn_off;
	//printf("Waiting 10 Seconds to Initialize Tracker \n");
	//ros::Duration(2).sleep();
	printf("Waiting gamepad Input\n");
    //gp_in =n.subscribe("gp_functions2", 1, chatterCallback);
    gp_in =n.subscribe("coms",1,callback);
    hok_in = n.subscribe("/scan",1,hok_cb);
	//turn_off = n.advertise<std_msgs::Float64>("coms", 1);
    while (ros::ok())
    {
        ros::spin();

	}
	file.close();
	return 0;
}
