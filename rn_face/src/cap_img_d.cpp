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
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include "std_msgs/Float64MultiArray.h"
#include "std_msgs/String.h"
#include "std_msgs/Float64.h"
#include <sys/time.h>
#include <chrono>
#include <ctime>
using namespace std;
ofstream file;

int REC = 0;

struct timeval start1, end1;
long mtime1, seconds1_, useconds1;
time_t t;
long int timer ;
double seconds=0 ;
double sec_temp=0;
std::chrono::time_point<std::chrono::system_clock> p1;
ros::Time rgb_cur_time, rgb_last_time;
ros::Time total_cur_time, total_time;
std::string path ="/home/skel/.ros/image_";
std::string png = ".png";
std::string path_ts;
std::stringstream sstream ;

 
 class DepthInfo
{
ros::NodeHandle nh_;

image_transport::ImageTransport it_;
image_transport::Subscriber image_sub_;
ros::Publisher pub;


public:

cv::Mat rgb_frame;

DepthInfo()
: it_(nh_)
{
	pub = nh_.advertise<std_msgs::String>("img_addr", 1);
	image_sub_ = it_.subscribe("camera/rgb/image_color", 1, &DepthInfo::imageCb, this);
}
void imageCb(const sensor_msgs::ImageConstPtr & msg)
{	

	if(REC==1)
	{
	//TIME INTERVAL STUFF	
	total_cur_time=ros::Time::now();
				std_msgs::String msg_img;
				//printf("IN RGB LOOP\n");
                rgb_cur_time = ros::Time::now();
                seconds=(rgb_cur_time.toSec())-(rgb_last_time.toSec());
                if(seconds>0.005)
                {	
					p1 = std::chrono::system_clock::now();
					sstream << std::chrono::duration_cast<std::chrono::milliseconds>(
					p1.time_since_epoch()).count() ;
                    path_ts = path_ts+sstream.str();
                    path_ts=path+path_ts+png;
                    //printf("path 2 %s\n",path2_ts.c_str());
                    const char* cstr = path_ts.c_str();
                    //printf("BEFORE BRIDGE\n");
                    
					cv_bridge::CvImagePtr cv_ptr_rgb;
					try
					{
						//printf("IN BRIDGE\n");
						cv_ptr_rgb = cv_bridge::toCvCopy(msg, "bgr8");
					}
					catch (cv_bridge::Exception & e)
					{
						ROS_ERROR("cv_bridge exception: %s", e.what());
						return;
					}
					
					//printf("OUTSIDE BRIDGE\n");
					rgb_frame = cv_ptr_rgb->image;
					
					//printf("BEFORE WRITE\n");
					//printf("path 2 %s\n",path_ts.c_str());
					cv::imwrite(cstr,rgb_frame);
					msg_img.data=path_ts;
					pub.publish(msg_img);
					rgb_last_time=rgb_cur_time;
					rgb_cur_time=ros::Time::now();
					path_ts="";
					sstream.str(std::string());
					sstream.clear();
				}
			
			
			
	}
	//cv::waitKey(3);
}

};


void chatterCallback(const std_msgs::Float64MultiArray::ConstPtr& msg)
{
    float start = msg->data[11]; //START gia na 3ekinhsei to record
    int cancel = msg->data[4]; //X gia na stamathsei
    //printf("Press Start to Start Recording \n");
    if (start==1)
    {	
		//printf("Started Recording Skeleton\n");
        REC = 1;
        total_time = ros::Time::now();
        ros::Duration(0.5).sleep();

    }
    else if ((REC==1)&&(cancel==1))
    {
        REC = 0;
        //printf("Stopped Recording Skeleton \n");
		ros::shutdown();
    }
}

void callback(const std_msgs::Float64::ConstPtr& msg)
{
	        total_time = ros::Time::now();

    float start = msg->data; //START gia na 3ekinhsei to record
		//X gia na stamathsei
    //printf("Press Start to Start Recording \n");
    if (start==1)
    {	
		//printf("Started Recording Skeleton\n");
        REC = 1;
       //ros::Duration(1).sleep();

    }
    else if ((REC==1))
    {
        REC = 0;
        //printf("Stopped Recording Skeleton \n");
		ros::shutdown();
    }
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "img_dep_rec");
    ros::NodeHandle n;
    ros::Subscriber gp_in;
   
    rgb_last_time = ros::Time::now();
	gettimeofday(&start1, NULL);
	// printf("Waiting gamepad Input\n");
    gp_in =n.subscribe("coms",5,callback);
    DepthInfo ic;
    while (ros::ok())
    {
        ros::spin();
    }
    return 0;

}
