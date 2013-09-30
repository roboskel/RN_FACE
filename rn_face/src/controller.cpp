#include <signal.h>
#include <stdio.h>
#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <std_msgs/Float64.h>
#include <vector>
#include "std_msgs/Float64MultiArray.h"
#include <cmath>
#include <math.h>
#include "/home/skel/roboskel_workspace/sandbox/robot_skel/sdc2130_skel/src/robodev.cpp"
#include <sdc2130_skel/RoboteqDevice.h>
#include <sdc2130_skel/ErrorCodes.h>
#include <sdc2130_skel/Constants.h>

#include "sdc2130_skel/teleop_skel.h"

#define PI 3.14159265358979323846264338
#define DIAMETER 15.24
#define RESOLUTION 2000
#define WHEEL_BASE_WIDTH 40
#define TWOPI 360
#define RADS 57.2958


//double functions[14]={0};
//double seconds;
int LM = 0 ;
int RM = 0 ;
int HB = 0 ; //handbrake signal
int ESD = 0 ; //emergency shutdown signal
int MODE = 0 ;
int REC = 0 ;
int REMOTE = -1;
int HOK = -1;
string response = "";
RoboteqDevice device;
int status = -1;
//int i ;

void chatterCallback(const sensor_msgs::Joy::ConstPtr& msg)
{
	if(REMOTE == -1)
	{
		if((msg->buttons[9]==1)&&(msg->buttons[8]==0))//START
		{
			ROS_INFO("Giving Start Signal");
			system("/home/skel/start");
			return;
		}
		else if((msg->buttons[8]==1)&&(msg->buttons[9]==0))//SELECT
		{
			ROS_INFO("Giving Stop Signal");
			system("/home/skel/stop");
			system("rosnode kill fdati");
			system("rosnode kill fdati_crop");
			system("rosnode kill image_view");
			system("rosnode kill openni_tracker");
			system("ps -ef | sed -n '/record/{/grep/!p;}' | awk '{print$2}' | xargs -i kill {}");
			system("ps -ef | sed -n '/fdati/{/grep/!p;}' | awk '{print$2}' | xargs -i kill {}");
			system("ps -ef | sed -n '/fdati_crop/{/grep/!p;}' | awk '{print$2}' | xargs -i kill {}");
			system("ps -ef | sed -n '/openni_launch/{/grep/!p;}' | awk '{print$2}' | xargs -i kill {}");
			ros::shutdown();

		}
		else if(msg->buttons[0]==1)//TRIANGLE
		{
			ROS_INFO("Starting Face Recognition & Tracking, Press Start to Begin");
			system("/home/skel/stop");
			system("roslaunch rn_face live2.launch &");
		}
		else if(msg->buttons[1]==1)//CIRCLE
		{
			ROS_INFO("Starting Recording Session, Press Start to Begin");
			system("roslaunch rn_face rec2.launch &");
		}
		else if(msg->buttons[2]==1)//X
		{
			ROS_INFO("Restarting Openni_launch");
			system("ps -ef | sed -n '/openni_launch/{/grep/!p;}' | awk '{print$2}' | xargs -i kill {}");
			system("killall XnSensorServer");
			system("sleep 3");
			system("roslaunch openni_launch openni.launch &");
		}
		else if(msg->buttons[3]==1)//SQUARE
		{
			ROS_INFO("Shutting Down Openni_Launch");
			system("ps -ef | sed -n '/openni_launch/{/grep/!p;}' | awk '{print$2}' | xargs -i kill {}");
		}
		else if(msg->buttons[4]==1)//LEFT TRIGGER 
		{
		//	ROS_INFO("Playing Rosbag");
			ros::shutdown();
		}
		
		else if(msg->buttons[6]==1)//LEFT BUMPER
		{
			ROS_INFO("Startin Remote Control Node");
			REMOTE = 1;
			system("rosrun rn_face move_script &");
			
		}
		else if(msg->buttons[7]==1)//RIGHT BUMPER
		{
			ROS_INFO("Deleting Recorded Images");
			system("rm /home/skel/.ros/image*");
			system("rm /home/skel/.ros/_laser.data");
			system("rm /home/skel/.ros/rec_bag.bag");
		}
		else if(msg->axes[5]==1)//Left Button
		{	
			ROS_INFO("*********");
			ROS_INFO("*********");
		}
		else if(msg->axes[5]==-1)
		{
			ROS_INFO("*********");//Right Button
			ROS_INFO("*********");
		}
		else if(msg->axes[6]==1)
		{
			ROS_INFO("*********");//Up Button
			ROS_INFO("Stoppping Face Detection And Tracking");
			system("rosnode kill openni_tracker");
			system("rosnode kill fdati");
			system("rosnode kill fdc2");
			ROS_INFO("*********");
		}
		else if(msg->axes[6]==-1)
		{
			if(HOK == -1 )
			{
				ROS_INFO("*********");//Down Button
				system("rosrun hokuyo_node hokuyo_node &");
				ROS_INFO("*********");
				HOK = 1;
			}
			else
			{	
				HOK=-1;
				system("rosnode kill /hokuyo_node &");
			}
		}
	}
	else if ((REMOTE == 1) && (msg->buttons[6]==1))//LEFT BUMPER
	{
		ROS_INFO("Stopping Remote Control Node");
		REMOTE = -1;
		system("rosnode kill /move_script");
	}
	/*
	ROS_INFO("Waiting For Input");
	ROS_INFO("Face Tracking Options");
	ROS_INFO("Select : Send Stop Signal, Kill FD & Openni Launch");
	ROS_INFO("Start : Send Start Signal");
	ROS_INFO("Triangle : Start FD (Press Start to Begin)");
	ROS_INFO("Circle : Start Recording Session (Press Start to Begin)");
	ROS_INFO("X : Kill & Restart Openni_Launch");
	ROS_INFO("Square : Shut Down Openni_Launch");
	ROS_INFO("Right Bumper (R1) : Delete Recorded Images");
	ROS_INFO("Right Trigger (R2) : Shut Down Remote Control Node");
	*/
	return;
}

int main(int argc, char* argv[])
{	
	ros::init(argc, argv, "controller");
	ros::NodeHandle nh;
	ros::Rate loop_rate(1000);
	ros::Subscriber gp_in =nh.subscribe("joy", 1, chatterCallback);
	/*
	ROS_INFO("Waiting For Input");
	ROS_INFO("Face Tracking Options");
	ROS_INFO("Select : Send Stop Signal, Kill FD & Openni Launch");
	ROS_INFO("Start : Send Start Signal");
	ROS_INFO("Triangle : Start FD (Press Start to Begin)");
	ROS_INFO("Circle : Start Recording Session (Press Start to Begin)");
	ROS_INFO("X : Kill & Restart Openni_Launch");
	ROS_INFO("Square : Shut Down Openni_Launch");
	ROS_INFO("Right Bumper (R1) : Delete Recorded Images");
	ROS_INFO("Right Trigger (R2) : Shut Down Remote Control Node");
	*/
	while (ros::ok())
		{
			ros::spinOnce();
		}
	printf( "Closing...\n" );
	return 0;
}
