// g++ `pkg-config --cflags opencv`   `pkg-config --libs opencv` face_detection_and_tracking.cpp -o face_detection_and_tracking
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <cmath>

#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/cv.h"
#include "cv.h"
#include <opencv2/imgproc/imgproc.hpp> 
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <stdio.h>
#include <ctype.h>

#include <iostream>
#include <stdio.h>
#include <unistd.h>

#include <ros/ros.h>
#include "std_msgs/Float64MultiArray.h"
#include "std_msgs/Float64.h"
#include "std_msgs/String.h"
#include "std_msgs/Int64.h"
#include "std_msgs/Int32.h"
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>
#include "rn_face/head_cords_m.h"
#include "rn_face/head_cords_m_array.h"

#include <sys/time.h>
#include <chrono>
#include <ctime>

//#define CV_NO_BACKWARD_COMPATIBILITY
using namespace std;
namespace enc = sensor_msgs::image_encodings;
ofstream file;

int REC = 0;

struct timeval start1, end1;
//struct timeval start2, end2;
long mtime1, seconds1_, useconds1;
//long mtime2, seconds2_, useconds2;      
time_t t;
//time_t t2;
long int timer ;
//long int timer2;
double seconds=0 ;
double sec_temp=0;
//double sec_temp2=0;
//double seconds2=0;
std::chrono::time_point<std::chrono::system_clock> p1;//, p2;
ros::Time rgb_cur_time, rgb_last_time;
//ros::Time depth_cur_time, depth_last_time;
ros::Time total_cur_time, total_time;
std::string path ="/home/skel/.ros/image_";
//std::string path2 ="/home/skel/.ros/depth_";
std::string png = ".png";
//std::string txt = ".dep.npy";
std::string path_ts;
//std::string path2_ts;
std::stringstream sstream ;
//std::stringstream sstream2;

 
 class DepthInfo
{
ros::NodeHandle nh_;

image_transport::ImageTransport it_;
image_transport::Subscriber image_sub_;
//image_transport::Subscriber depth_sub_;
//image_transport::Publisher depth_pub_;
ros::Publisher pub;
//ros::Publisher dep_pub;

public:

cv::Mat rgb_frame;
//cv::Mat depth_frame;
//cv::Mat depth_frame_scaled;

DepthInfo()
: it_(nh_)
{
    //depth_pub_ = it_.advertise("out", 1);
    pub = nh_.advertise<std_msgs::String>("img_addr", 1);
	//dep_pub = nh_.advertise<std_msgs::String>("dep_addr", 1);
	//depth_sub_ = it_.subscribe("camera/depth/image_raw", 1, &DepthInfo::depthInfoCb, this);
	image_sub_ = it_.subscribe("camera/rgb/image_color", 1, &DepthInfo::imageCb, this);
}
void imageCb(const sensor_msgs::ImageConstPtr & msg)
{	

	if(REC==1)
	{
	//TIME INTERVAL STUFF	
	total_cur_time=ros::Time::now();
	//if(((total_cur_time.toSec())-(total_time.toSec()))>=10000)
	//{
	//		REC=0;
	//		ros::shutdown();
	//}
				std_msgs::String msg_img;
				//printf("IN RGB LOOP\n");
                rgb_cur_time = ros::Time::now();
                seconds=(rgb_cur_time.toSec())-(rgb_last_time.toSec());
                if(seconds>0.005)
                {	
				
                    //time(&t);
                    //timer = (long)t;
                    //gettimeofday(&end1, NULL);
					//seconds1_ = end1.tv_sec-start1.tv_sec;
					//useconds1 = end1.tv_usec-start1.tv_usec;
					//mtime1=((seconds1_) * 1000 + useconds1/1000.0) + 0.5;
					
					p1 = std::chrono::system_clock::now();
					sstream << std::chrono::duration_cast<std::chrono::milliseconds>(
					p1.time_since_epoch()).count() ;
                    //sstream << timer << std::setprecision(5) << seconds;
                    //sstream.str(std::string());
					//sstream.clear();
					//sstream<<seconds;
                    path_ts = path_ts+sstream.str();
                    path_ts=path+path_ts+png;
                    //printf("path 2 %s\n",path2_ts.c_str());
                    const char* cstr = path_ts.c_str();
                    //printf("BEFORE BRIDGE\n");
                    
					cv_bridge::CvImagePtr cv_ptr_rgb;
					try
					{
						//printf("IN BRIDGE\n");
						cv_ptr_rgb = cv_bridge::toCvCopy(msg, enc::BGR8/*"bgr8"*/);
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
					//FILE *imageFile = fopen(cstr, "wb");
					//fwrite(depth_frame.data,  640*480, sizeof(u_int16_t), imageFile);
					//printf("path 2 %s\n",path_ts.c_str());
					//ros::Duration(1).sleep();
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
/*
void depthInfoCb(const sensor_msgs::ImageConstPtr & msg)
{
   std_msgs::String msg_dep;
    if (REC==1)
    {
        //printf("IN DEPTH LOOP\n");
        depth_cur_time = ros::Time::now();
		seconds2=depth_cur_time.toSec()-depth_last_time.toSec();
		if(seconds2>0.1)
		{	
					
			p2 = std::chrono::system_clock::now();
			sstream2 << std::chrono::duration_cast<std::chrono::milliseconds>(
			p2.time_since_epoch()).count() ; 
			//sstream2 << timer2<<std::setprecision(5)<<seconds2;
			path2_ts = sstream2.str();
			path2_ts=path2+path2_ts+txt;
			//printf("path 2 %s\n",path2_ts.c_str());
			const char* cstr2 = path2_ts.c_str();
			//printf("BEFORE DEPTH BRIDGE\n");
            file.open (cstr2, ios::out | ios::binary);
            unsigned int height = msg->height;
            unsigned int width = msg->width;
            unsigned int step = msg->step;
            unsigned int datasize = height*step;
            vector<unsigned short> depth;
            int endian = msg->is_bigendian;
            std::string encoding = msg->encoding;

			cv_bridge::CvImagePtr cv_depth = cv_bridge::toCvCopy(msg,sensor_msgs::image_encodings::TYPE_16UC1);
			for(int i=0; i<cv_depth->image.rows; i++) {
			  for(int j=0; j<cv_depth->image.cols; j++) {
				unsigned short dd = *(cv_depth->image.ptr<unsigned short>(i,j));
				depth.push_back(dd);
				//cout<<dd<<"\n";
			  }
			}
			file.write((char *)&depth[0], sizeof(unsigned short) * datasize );
			file<<endl;
            file.close();
            msg_dep.data =  path2_ts;
            dep_pub.publish(msg_dep);
			depth_last_time=depth_cur_time;
            depth_cur_time=ros::Time::now();
            path_ts="";
            sstream2.str(std::string());
            sstream2.clear();
        }
    }
}
*/
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
    else if ((REC==1)) /*&&(start==0))*/
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
	//depth_last_time = ros::Time::now();ghijoyuiguo
	gettimeofday(&start1, NULL);
	//gettimeofday(&start2, NULL);
    // printf("Waiting gamepad Input\n");
    //gp_in =n.subscribe("gp_functions", 1, chatterCallback);
    gp_in =n.subscribe("coms",5,callback);
    DepthInfo ic;
    while (ros::ok())
    {
        ros::spin();
    }
    return 0;

}
