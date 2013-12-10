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

int battery = 0;
int amp = 0;
RoboteqDevice device;
int status = -1;
void chatterCallback(const sensor_msgs::Joy::ConstPtr& msg)
{
	if(msg->buttons[4]==1)//X 
	{
		if(device.GetValue(_V,battery)!=RQ_SUCCESS)
		{
			ROS_INFO("Failed to read battery voltage\n");
		}
		else
		{
			ROS_INFO("Battery Level (V) : %d", battery);
		}
		if(device.GetValue(_A,amp)!=RQ_SUCCESS)
		{
			ROS_INFO("Failed to read battery amps\n");
		}
		else
		{
			ROS_INFO("Battery Level (A) : %d", amp);
		}
		  
	}
	else if(msg->buttons[5]==1)//RIGHT TRIGGER
	{
		ROS_INFO("Shutting Down Remote Control Node");
		ros::shutdown();
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
	
	ros::init(argc, argv, "sdc2130_skel");

	ros::NodeHandle n;
	ros::Duration(0.1).sleep();
	ros::Subscriber sub = n.subscribe("joy", 1, chatterCallback);
	
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
while (ros::ok())
    {	
		ros::spin();
	}
	
	device.Disconnect();
	return 0;
}
