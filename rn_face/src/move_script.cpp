#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <time.h>
#include <math.h>
#include <sensor_msgs/Joy.h>
#include "/home/skel/roboskel_workspace/sandbox/robot_skel/sdc2130_skel/src/robodev.cpp"
#include <sdc2130_skel/RoboteqDevice.h>
#include <sdc2130_skel/ErrorCodes.h>
#include <sdc2130_skel/Constants.h>
#include "ros/ros.h"
#include "sdc2130_skel/teleop_skel.h"


using namespace std;

#define PI 3.14159265358979323846264338
#define DIAMETER 15.24 //6 INCHES
#define RESOLUTION 2000
#define WHEEL_BASE_WIDTH 40
#define TWOPI 360
#define RADS 57.2958
#define DISTANCE_F 300000 //Travel fwd 300cm
#define DISTANCE_CRV 300000
#define TURN 90
int circumference = PI* DIAMETER;

int LM = 0 ;
int RM = 0 ;
int HB = 0 ; //handbrake signal
int ESD = 0 ; //emergency shutdown signal
int MODE = 0 ;
int REC = 0  ;
string response = "";
RoboteqDevice device;
int status = -1;
int i ;	
//clock_t t;
int speedR = 1000;
int speedL = 1000;
int speedR_T;
int speedL_T;
int posleft;
int posright;
int rotR = 0;
int rotL = 0;
int rotRtot = 0;
int rotLtot = 0;
	
int count_ = 0;
double posx = 0.0;
double posy = 0.0;
double posth = 0.0;
double mr = 0;
double ml = 0;
double mth = 0;
double turn_deg = 0 ;
int distR = 0;
int distL = 0;
int total_dist = 0;
int battery = 0;
int seconds = 0 ;
ros::Time current_time, last_time, total_time;
//ros::Time write_time, cur_write_time, total_time, cur2;
//ofstream file;

int fwdDir = 1;
int crvDir = 1;

void mvFwd();
void mvCrv();
void teleopCallback(const sensor_msgs::Joy::ConstPtr& msg)
{	
	
	if((msg->axes[5]!=0)||(msg->axes[6]!=0))
	{
		//device.SetConfig(_RWD, 1, -1);
		//device.SetConfig(_RWD, 2, -1);
		ROS_INFO("HEAR HEAR");
		if(msg->axes[6]==1)
		{
			RM = -1000;
			LM = 1000;
		}
		else if(msg->axes[6]==-1)
		{
			RM = 1000;
			LM = -1000;
		}
		else if(msg->axes[5]!=0)
		{
			RM = -1000*msg->axes[5];
			LM = -1000*msg->axes[5];
		}
		device.SetCommand(_GO,1, RM);// RIGHT
		device.SetCommand(_GO,2, LM);//LEFT
	}
	else 
	{
		device.SetCommand(_GO,1, 0);// RIGHT
		device.SetCommand(_GO,2, 0);//LEFT
		if(msg->buttons[0]==1)
		{	
			//device.SetConfig(_RWD, 1, 0);
			//device.SetConfig(_RWD, 2, 0);
			//device.SetCommand(_GO,1, 0);// RIGHT
			//device.SetCommand(_GO,2, 0);//LEFT
		}
		if((msg->buttons[10]==1)&&(msg->buttons[11]==1))
		{	
			//device.SetConfig(_RWD, 1, 0);
			//device.SetConfig(_RWD, 2, 0); 
			//mvFwd();
		}
		else if (msg->buttons[10]==1)
		{
			//device.SetConfig(_RWD, 1, 0);
			//device.SetConfig(_RWD, 2, 0);
			//mvCrv();
		}
		else if (msg->buttons[11]==1)
		{
			//device.SetConfig(_RWD, 1, 0);
			//device.SetConfig(_RWD, 2, 0);
		}
		else if((msg->buttons[8]==1)&&(msg->buttons[9]==1))//SELECT
		{
			//device.SetConfig(_RWD, 1, 0);
			//device.SetConfig(_RWD, 2, 0);
			printf("Emergency Shutdown\n");			
			if((status = device.SetCommand(_GO, 2,  0) != RQ_SUCCESS))
			{
				cout<<"failed --> \n"<<status<<endl;
			}	
			else 
			{
			}
			ros::Duration(0.01).sleep(); 
			if((status = device.SetCommand(_GO, 1,- 0)) != RQ_SUCCESS)
			{
				cout<<"failed --> \n"<<status<<endl;
				device.Disconnect();
				ros::shutdown();
			}	
			else
			{
			}
			ROS_INFO("SHUTTING DOWN");
			ros::shutdown();
		}
	}
	
	
}



int main(int argc, char *argv[])
{	
	int status = device.Connect("/dev/ttyACM1");
	if(status != RQ_SUCCESS)
	{
		cout<<"Error connecting to device: "<<status<<"."<<endl;
		return 1;
	}
	//device.SetConfig(_RWD, 1, 1000);
	//device.SetConfig(_RWD, 2, 1000);
	device.SetConfig(_RWD, 1, -1);
	device.SetConfig(_RWD, 2, -1);
	
	ros::init(argc, argv, "move_script");

	ros::NodeHandle n;
	ros::Duration(0.1).sleep();
	ros::Subscriber sub = n.subscribe("joy", 1, teleopCallback);
	
	ROS_INFO("- SetConfig(_DINA, 1, 1)...");
    if((status = device.SetConfig(_DINA, 1, 1)) != RQ_SUCCESS)
      cout<<"failed --> "<<status<<endl;
    else
      ROS_INFO("succeeded.");
    ros::Duration(0.01).sleep(); //sleep for 10 ms

    int result;
    
    ROS_INFO("- GetConfig(_DINA, 1)...");
    if((status = device.GetConfig(_DINA, 1, result)) != RQ_SUCCESS)
      cout<<"failed --> "<<status<<endl;
    else
      cout<<"returned --> "<<result<<endl;
    ROS_INFO("Roboteq -- Successfull setup of the Roboteq SDC2130");
    printf ("Sek Operational\n\n");
    ros::Duration(0.01).sleep(); //sleep for 10 ms
    
	//file.open ("roboskel_log.txt", ios::out | ios::binary);
	//write_time = ros::Time::now();
	//total_time =ros::Time::now();
	//file<<write_time<<"\n";
	//ros::Duration(3).sleep();
	total_time =ros::Time::now();
while (ros::ok())
    {	
		ros::spin();
	}
	
	device.Disconnect();
	return 0;
}

void mvFwd()
{	
	speedL_T = speedL;
	speedR_T = speedR ;
	rotR = 0;
	distR = 0;
	rotL = 0;
	distL = 0;
	current_time = ros::Time::now();
	device.GetConfig(_GO,1,posleft);
	device.GetConfig(_GO,2,posright);
	//ROS_INFO("POS 1 : %d",posleft);
	//ROS_INFO("POS 2 : %d",posright);
	ROS_INFO("MV FWD");
	device.SetCommand(_GO,1, -speedR);// RIGHT
	device.SetCommand(_GO,2, speedL);//LEFT
	device.GetValue(_S,1,rotR);
	device.GetValue(_S,2,rotL);
	//distR = circumference*rotR;
	//distL = circumference*rotL;
	//ROS_INFO("ROTR : %d",rotR);
	//ROS_INFO("ROTL : %d",rotL);
	seconds = (current_time - last_time).toSec();
	distR = distR + seconds*(-rotR/60.0)*(DIAMETER*PI);
	distL = distR + seconds*(rotL/60.0)*(DIAMETER*PI);
	while (distR<DISTANCE_F)
	{
		seconds = (current_time - last_time).toSec();
		device.GetValue(_S,1,rotR);
		device.GetValue(_S,2,rotL);
		distR = distR + circumference*abs(rotR);
		distL = distL + circumference*abs(rotL);
		ROS_INFO("distR : %d",distR);
		ROS_INFO("distL : %d",distL);
		last_time = current_time;
		current_time = ros::Time::now();
	}
	
	ROS_INFO("ROTR : %d",rotR);
	ROS_INFO("ROTL : %d",rotL);
	device.SetCommand(_GO,1, 0);// RIGHT
	device.SetCommand(_GO,2, 0);//LEFT
	distR = 0;
	distL = 0;
	last_time = current_time;
	//while(status = device.SetCommand(_GO,1,0)!=RQ_SUCCESS)
	//{}
	//while(status = device.SetCommand(_GO,2,0)!=RQ_SUCCESS)
	//{}
}
void mvCrv()
{
	rotR = 0;
	distR = 0;
	rotL = 0;
	distL = 0;
	turn_deg = 0 ;
	speedL_T = speedL;
	speedR_T = speedR ;
	current_time = ros::Time::now();
	device.GetConfig(_GO,1,posleft);
	device.GetConfig(_GO,2,posright);
	ROS_INFO("MV CRV");
	device.SetCommand(_GO,1, -speedR);// RIGHT
	device.SetCommand(_GO,2, speedL);//LEFT
	device.GetValue(_S,1,rotR);
	device.GetValue(_S,2,rotL);
	seconds = (current_time - last_time).toSec();
	distR = distR + seconds*(rotR/60.0)*(DIAMETER*PI);
	distL = distR + seconds*(rotL/60.0)*(DIAMETER*PI);
	
	while (distR<DISTANCE_CRV)
	{
		seconds = (current_time - last_time).toSec();
		device.GetValue(_S,1,rotR);
		device.GetValue(_S,2,rotL);
		distR = distR + circumference*abs(rotR);
		distL = distL + circumference*abs(rotL);
		ROS_INFO("distR : %d",distR);
		ROS_INFO("distL : %d",distL);
		last_time = current_time;
		current_time = ros::Time::now();
	}
	ROS_INFO("BAT : %d",battery);
	ROS_INFO("DEG : %f",turn_deg);
	ROS_INFO("TURNING");
	device.SetCommand(_GO,1, 0);// RIGHT
	device.SetCommand(_GO,2, 0);//LEFT
	ros::Duration(2).sleep();
	seconds = (current_time - last_time).toSec();
	device.SetCommand(_GO,1, 0);// RIGHT
	device.SetCommand(_GO,2, -speedL);//LEFT
	device.SetCommand(_S,1, 0);// RIGHT
	device.SetCommand(_S,2, 0);//LEFT
	device.GetValue(_S,1,rotR);
	device.GetValue(_S,2,rotL);
	rotR = 0;
	distR = 0;
	rotL = 0;
	distL = 0;
	turn_deg = 0;
	ROS_INFO("DEG : %f",turn_deg);
	while(abs(mth)<TURN)
	{
		seconds = (current_time - last_time).toSec();
		device.GetValue(_S,1,rotR);
		device.GetValue(_S,2,rotL);
		distR = distR + circumference*(-rotR);
		distL = distL + circumference*(rotL);
		ROS_INFO("MTH : %f",mth);
		mth += (distL-distR)/WHEEL_BASE_WIDTH;
		turn_deg -=(float)((int)(mth/TWOPI))*TWOPI;
		ROS_INFO("DEG : %f",turn_deg);
	}
	device.SetCommand(_GO,1, 0);// RIGHT
	device.SetCommand(_GO,2, 0);//LEFT
	distR = 0;
	distL = 0;
	last_time = current_time;
}
