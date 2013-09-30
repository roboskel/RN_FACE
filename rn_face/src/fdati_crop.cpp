// g++ `pkg-config --cflags opencv`   `pkg-config --libs opencv` face_detection_and_tracking.cpp -o face_detection_and_tracking
#include <stdlib.h>

#include <stdio.h>
#include <time.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <cmath>
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


#include "cv.h"
#include "highgui.h"
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <stdio.h>
#include <ctype.h>

#include <iostream>
#include <stdio.h>
#include <unistd.h>

#include "std_msgs/Float64MultiArray.h"
#include "std_msgs/String.h"
#include "std_msgs/Int64.h"
#include "std_msgs/Int32.h"
#include "rn_face/head_cords_m.h"
#include "rn_face/head_cords_m_array.h"

#include <sys/time.h>
#include <chrono>
#include <ctime>

#define CV_NO_BACKWARD_COMPATIBILITY
#define PI 3.14159265

using namespace std;
using namespace cv;

struct face {
	int fid;
	int x;
	int y;
	int r;
	//int detected;
	face():fid(0),x(0),y(0),r(0)/*,detected(0)*/{}
};

struct rgb_map
{
	int rgb_ar[10][3];
	
	rgb_map():rgb_ar({	{220,20,60},
				{0,255,255},
				{255,200,100},
				{255,130,171},
				{138,43,226},
				{65,105,225},
				{162,181,205},
				{0,255,127},
				{61,145,64},
				{255,255,0}	}){}
};

struct kinect_mesg
{
	double time;
	double id[6];
	double x[6];
	double y[0];
	double z[0];
};
rgb_map rgb_map_;
const int uid = 10;
const int rgbval = 3;

int NO_FACES = 1;
int PREV_NO_FACES = 1;
int SPEAKER_ID = 1;
int MAX_PERSONS = 6;
int SESSION_MAX = 0;
int SKEL_SET = 0 ;
int ADDR_SET = 0 ;
int DOMINANT_ID = 0;
int WIDTH = 180;
int HEIGHT = 250 ;
double DMAX = 0 ;
double DTEMP = 0 ;
double DISTANCE = 2.1;

//int detected = 0;
int flashing = 1 ;
int occlusion = 1;
int count_occlusion = 0;
int temp_id;
std::string data_adr="";
IplImage *img ;

struct face_{
//-------------------
//original toy alex
	IplImage *image,*hsv,*hue,*mask,*backproject,*histimg;
	CvHistogram *hist;
	CvRect roi;
	Mat frame,frameCopy;
	long int number;
	int backproject_mode ;//= 0;
	int select_object ;//= 0;
	int track_object ;//= 0;
	int show_hist ;//= 1;
	int bin_w;
	int hdims ;//= 42;
	int vmin ;//= 10;
	int vmax ;//= 250; 
	int smin ;//= 30;
	int id_;
	int fid;
	int detected;
	//int x_,y_,z_;
	int occlusion ; //=0;
	int count_occlusion; //=1;
	float hranges_arr[2];// = {0,500};
	float* hranges ;//= hranges_arr;
	float max_val;
	//double head_stick[2];
	//double head_stick_before_occlusion[2];
	double head_stick;
	double head_stick_before_occlusion;
	double x,y,z;
	double x_prev,y_prev,z_prev;
	bool image_created;
	//toy alex dhlwnontan sth main
	//pleon melos tis domhs
	//char number[10],id[2],x[10],y[10],z[10];
	CvPoint origin;
	CvRect selection;
	CvRect track_window;
	CvRect box_area;
	CvBox2D track_box;
	CvBox2D tb2;
	CvConnectedComp track_comp;
	CvPoint2D32f new_center;
	
	face_():number(9),image(0),hsv(0),hue(0),mask(0),backproject(0),histimg(0),backproject_mode(0),select_object(0),track_object(0),
	show_hist(1),hdims(42),detected(0),occlusion(1),count_occlusion(0),
	vmin(40),vmax(256),smin(90),id_(-1),fid(0)/*,x_(0),y_(0),z_(0)*/,x(0),y(0),z(0),x_prev(0),z_prev(0),y_prev(0),
	hranges_arr({0,500}),hranges(hranges_arr),max_val(0),head_stick(0),head_stick_before_occlusion(0),image_created(FALSE){}/*head_stick({0,0}),head_stick_before_occlusion({0,0})*/
};

face *Faces = new face[10];

face* detectAndDraw( Mat& img,
					CascadeClassifier& cascade, CascadeClassifier& nestedCascade,
					double scale ,int i);

String cascadeName = "./haarcascades/haarcascade_frontalface_alt.xml";
String nestedCascadeName = "./haarcascades/haarcascade_eye_tree_eyeglasses.xml";



CvScalar hsv2rgb( float hue ){
    int rgb[3], p, sector;
    static const int sector_data[][3]={{0,2,1}, {1,2,0}, {1,0,2}, {2,0,1}, {2,1,0}, {0,1,2}};
    hue *= 0.033333333333333333333333333333333f;
    sector = cvFloor(hue);
    p = cvRound(255*(hue - sector));
    p ^= sector & 1 ? 255 : 0;
    rgb[sector_data[sector][0]] = 255;
    rgb[sector_data[sector][1]] = 0;
    rgb[sector_data[sector][2]] = p;
    return cvScalar(rgb[2], rgb[1], rgb[0],0);
}

void box_area(double x,double y, double z,int &py, int &pz, int &width, int &height)
{
	//double tempx;
	double tempy;
	double tempz;
	if (y > 0)
	{
		tempy = 319 - ((y)/x)*640;
	}
	else
	{
		tempy = 319 + (-y/x)*640;
	}
	py = static_cast<int>(tempy+0.5)-90;
	/*
	if (z > 0)
	{
		tempz = 239 - ((z)/x)*480;
	}
	else
	{
		tempz = 239 + (-z/x)*480;
	}
	*/ 
	pz = 50;//80/*static_cast<int>(tempz+0.5)-90*/;
	width = WIDTH;
	height = HEIGHT;
	//printf("TempY = %f\nTempZ = %f\n",tempy,tempz);
	//printf("SkelY = %f\nSkelZ = %f\n",y,z);
	//printf("X = %d\nY = %d\nW = %d\nH = %d\n\n",py,pz,width,height);
	return ;
}
IplImage* crop( IplImage* src,  CvRect roi)
{
	cvSetImageROI( src, roi );
	// Must have dimensions of output image
	IplImage* cropped = cvCreateImage( cvGetSize(src)/*roi.width,roi.height*/, 8, 3 );
	//cropped->origin=src->origin;
	// Say what the source region is
	//cvSetImageROI( src, roi );
	// Do the copy
	//ROS_INFO("COPYING");
	cvCopy( src, cropped );
	//ROS_INFO("COPIED");
	cvResetImageROI( src );
	//ROS_INFO("RETURNED");
	return cropped;
}

void set_factors(double factors[])
{	
	//calculate y factor
	factors[0] = 320/(DISTANCE*(sin(28.5*PI/180))/sin(61.5*PI/180));
	//calculate z factor
	factors[1] = 240/(DISTANCE*(sin(21.5*PI/180))/sin(68.5*PI/180));
}
void no_faces_cb(const std_msgs::Int64::ConstPtr& msg)
{
	//ROS_INFO("SET NO_FACES");
	NO_FACES=msg->data;
	return;
}
void speaker_id_cb(const std_msgs::Int64::ConstPtr& msg)
{
	//ROS_INFO("SET SPEAKER_ID");
	SPEAKER_ID=msg->data;
	return;
}
void skelCallback(const rn_face::head_cords_m_array::ConstPtr& msg,struct face_ face_sa[])
{	
	//ROS_INFO("READING STICK DATA");
	NO_FACES = msg->no_faces;
	if (NO_FACES != PREV_NO_FACES)
	{
		PREV_NO_FACES = NO_FACES ;
		//detected=0;
		//ros::Duration(0.5).sleep();
	}
	for (int i=0; i<NO_FACES; ++i)
    {
		const rn_face::head_cords_m &data = msg->head_co[i];
		if (face_sa[i].x==0)
		{
			if(face_sa[i].x_prev!= 0)
			{
				face_sa[i].detected = 0;  
			}
		}
		face_sa[i].x_prev = face_sa[i].x;
		face_sa[i].y_prev = face_sa[i].y;
		face_sa[i].z_prev = face_sa[i].z;
		face_sa[i].number = data.id;
		face_sa[i].x = data.x;
		face_sa[i].y = data.y;
		face_sa[i].z = data.z;
	}
    //ROS_INFO("NO_FACES::%d",NO_FACES);
    SKEL_SET=1;
	return ;
}

void mcb (const std_msgs::String::ConstPtr & msg)
{
	//ROS_INFO("SET IMG_ADDR");
	std::string pew = msg->data.c_str();
	if(!pew.empty())
			{data_adr=pew;
			ADDR_SET=1;}
	else {data_adr="";
			ADDR_SET=0;}
}
int main( int argc, char** argv )
{	
	if(argc>1)
    {
		SESSION_MAX=atoi(argv[1]);
	}
	if(argc==2)
	{
		DISTANCE = atof(argv[2]);
	}
	else {SESSION_MAX=1;}
	//Hardcoded gia dokimes
	//SESSION_MAX=6;
	NO_FACES=2;
	SPEAKER_ID=1;
	int c;
	int i;
	int sum = 0;
	int result;
	int _vmin[NO_FACES];
	int _vmax[NO_FACES];
	float _max_val[NO_FACES];
	int val;
	const char *str_ ;
	char head[20] ="Face_";
	//set_factors(factors);
	//ROS_INFO("FACTORS[0] = %f\nFACTORS[1] = %f \n",factors[0],factors[1]);
	face_  *face_sa = new face_[6];
	for (c=0;c<NO_FACES;c++)
	{
		face_sa[c].fid=c;
	}
	//printf("Persons Tracked : %d\n",SESSION_MAX);
	//ROS
	ros::init(argc, argv, "fdati_crop");
    ros::NodeHandle n;
	ros::Publisher dom_id;
	ros::Subscriber no_faces,im_adr,speaker_id,kinect_co;
	no_faces = n.subscribe<std_msgs::Int64>("no_faces",1,no_faces_cb);
	speaker_id = n.subscribe<std_msgs::Int64>("speaker_id",1,speaker_id_cb);
	kinect_co = n.subscribe<rn_face::head_cords_m_array>("head_cords",1,boost::bind(skelCallback, _1, face_sa));
	im_adr = n.subscribe<std_msgs::String>("img_addr", 1, mcb);
	std_msgs::Int32 dom_id_msg;
	//ros::Duration(100).sleep();
	//ROS
	//OPENCV.Mat frame, frameCopy;
	Mat frame, frameCopy;
	CvFont font;
	cvNamedWindow("Enhanced Face Detection & Tracking",1);
	cvMoveWindow("Enhanced Face Detection & Tracking",0,0);
	//cvNamedWindow("Face 1",1);
	//cvMoveWindow("Face 1",0,530);
	//cvNamedWindow("Face 2",1);
	//cvMoveWindow("Face 2",350,530);
	cvInitFont(&font,CV_FONT_HERSHEY_PLAIN|CV_FONT_ITALIC,1,2,2,2);
	
	const String scaleOpt = "--scale=";
	size_t scaleOptLen = scaleOpt.length();
	const String cascadeOpt = "--cascade=";
	size_t cascadeOptLen = cascadeOpt.length();
	const String nestedCascadeOpt = "--nested-cascade";
	size_t nestedCascadeOptLen = nestedCascadeOpt.length();
	CascadeClassifier cascade, nestedCascade;
	double scale = 1;
	
	if( !cascade.load( cascadeName ) )
	{
			cerr << "ERROR: Could not load classifier cascade" << endl;
			cerr << "Usage: facedetect [--cascade=<cascade_path>]\n"
			"   [--nested-cascade[=nested_cascade_path]]\n"
			"   [--scale[=<image scale>\n"
			"   [filename|camera_index]\n" << endl ;
			return -1;
	}
	
	IplImage* iplImg;
	//OPENCV
	while (ros::ok())
    {	
		//ROS_INFO("DETECTED : %d" ,detected);
		//ROS_INFO("IN LOOP");
		ros::spinOnce();
		
		if(ADDR_SET==1)
		{
			//cvMoveWindow("Face Detection & Tracking",100,100);
			str_ = data_adr.c_str();
			result=access(str_, F_OK);
			if (result==0)
			{
					//ROS_INFO("IN SECOND LOOP");
					iplImg = cvLoadImage(str_,1);
					//ROS_INFO("**********************************************************");
						frame = iplImg;
						if( frame.empty() )
							return 0;
						if( iplImg->origin == IPL_ORIGIN_TL )
							frame.copyTo( frameCopy );
						else
							{flip( frame, frameCopy, 0 );}
							//ROS_INFO("SHOWING");
					cvShowImage("Enhanced Face Detection & Tracking",iplImg);
					//cvReleaseImage(&img);
					cvWaitKey(10);
				}

		}
		if ((SKEL_SET==1)&&(ADDR_SET==1))
		{
			//ROS_INFO("CHECKING DATA_ADDR");
			if(data_adr.empty()){continue;}
			//ROS_INFO("CHECKED DATA_ADDR");
			//MY_LOOP
			if (result==0)
			{
				//ROS_INFO("CREATED IPL IMG");	
				//ROS_INFO("INITILIAZING HUES ETC");
				for(c=0;c<NO_FACES;c++)
				{	
					//printf("\nC = %d\n",c);	
					//ROS_INFO("#1");
					if (face_sa[c].x!=0)
					{
						if(face_sa[c].image_created==FALSE)
						{
							sprintf(head,"Face_%d",c+1);
							cvNamedWindow(head,1);
							cvMoveWindow (head,(180*c),530);
							strcpy(head,"Face_");
							face_sa[c].image_created = TRUE; 
						}
						box_area(face_sa[c].x, face_sa[c].y, face_sa[c].z,/*factors,*/
								face_sa[c].box_area.x, face_sa[c].box_area.y,
								face_sa[c].box_area.width, face_sa[c].box_area.height);
						//printf("X = %d\nY = %d\nW = %d\n H = %d\n",face_sa[c].box_area.x,face_sa[c].box_area.y
						//	,face_sa[c].box_area.width,face_sa[c].box_area.height);
						//ros::Duration(100).sleep();
						face_sa[c].image=crop(iplImg,/*cvRect((640/NO_FACES)*c,0,640/NO_FACES,cut_height)*/face_sa[c].box_area);
						//ROS_INFO("#2");
						face_sa[c].frame = face_sa[c].image;
						if( face_sa[c].frame.empty() )
							return 0;
						if( face_sa[c].image->origin == IPL_ORIGIN_TL )
							face_sa[c].frame.copyTo( face_sa[c].frameCopy );
						else
						{
							flip( face_sa[c].frame, face_sa[c].frameCopy, 0 );
						}
						face_sa[c].image->origin=iplImg->origin;
						//ROS_INFO("#3");
						face_sa[c].hsv=cvCreateImage( cvGetSize(face_sa[c].image), 8, 3 );
						//ROS_INFO("#4");
						face_sa[c].hue=cvCreateImage( cvGetSize(face_sa[c].image), 8, 1 );
						//ROS_INFO("#5");
						face_sa[c].mask=cvCreateImage( cvGetSize(face_sa[c].image), 8, 1 );
						//ROS_INFO("#6");
						face_sa[c].backproject=cvCreateImage( cvGetSize(face_sa[c].image), 8, 1);
						//ROS_INFO("#7");
						face_sa[c].hist=cvCreateHist(1 , &face_sa[c].hdims, CV_HIST_ARRAY, &face_sa[c].hranges, 1);//Creates a histogram.
						//ROS_INFO("#8" );
						face_sa[c].histimg = cvCreateImage( cvSize(320,200), 8, 3 ); //Creates an image header and allocates the image data. //CvSize size, int depth, int channels)
						//ROS_INFO("#9");
						cvZero( face_sa[c].histimg );
						//ROS_INFO("#10");
						cvCvtColor(face_sa[c].image, face_sa[c].hsv, CV_BGR2HSV );
						if(face_sa[c].image_created == TRUE)
						{
							//ROS_INFO("#11");
							sprintf(head,"Face_%d",c+1);
							cvShowImage(head,face_sa[c].image);
							strcpy(head,"Face_");
							cvWaitKey(10);
						}
					}
		}
		//ROS_INFO("INITILIAZED HUES ETC");
		if (sum != NO_FACES)
		{	
			//ROS_INFO("BEFORE DETECT AND DRAW");
			for(c=0;c<NO_FACES;c++)
			{	
				if(face_sa[c].detected==0)
				{
					detectAndDraw ( face_sa[c].frameCopy, cascade, nestedCascade, scale/*,face_sa*/,c);
					if ((Faces[c].x>0)&&(Faces[c].y>0))
					{	
						face_sa[c].detected = 1;
						//ROS_INFO("FACES[%d].X = %d  FACES[%d].Y = %d",c,Faces[c].x,c,Faces[c].y);
						//ROS_INFO("FACES[%d].DETECTED = %d" ,c,face_sa[c].detected);
						face_sa[c].occlusion = 1;
						face_sa[c].track_object = -1;
						face_sa[c].selection = cvRect(Faces[c].x-Faces[c].r,Faces[c].y-Faces[c].r,Faces[c].x+Faces[c].r,Faces[c].y+Faces[c].r);				
						//ROS_INFO("SX : %d, SY : %d",face_sa[c].selection.x, face_sa[c].selection.y);
						cvRectangle(face_sa[c].image, cvPoint(face_sa[c].selection.x, face_sa[c].selection.y), cvPoint(face_sa[c].selection.width, face_sa[c].selection.height),
						CV_RGB(rgb_map_.rgb_ar[c][0],rgb_map_.rgb_ar[c][1],rgb_map_.rgb_ar[c][2]), 3, CV_AA, 0);
						/*
						if(c==1)
						{
							cv::imshow( "Face 2", face_sa[c].frameCopy );
						}
						else
						{
							cv::imshow( "Face 1", face_sa[c].frameCopy );
						}
						cvWaitKey(1);
						*/
					}
					else
					{
						face_sa[c].detected = 0;
					}
				}
			//ROS_INFO("FACE_SA[%d].TRACK_WINDON : X = %d   Y = %d WIDTH = %d HEIGHT = %d DETECTED = %d",
			//c,face_sa[c].track_window.x,face_sa[c].track_window.y, face_sa[c].track_window.width, face_sa[c].track_window.height,
			//face_sa[c].detected);
			}
		}
		sum=0;
		for (c=0;c<NO_FACES;c++)
		{
			if (face_sa[c].detected!=0)
			{
				sum++;
			}
		}
		
		//ROS_INFO("GIATI DE MPAINEIS EDW MESA ?");
		if (sum>0)
		{  	
			//sum=0;
			//ROS_INFO("MPHKE");
			for (c=0;c<NO_FACES;c++)
			{
				if(face_sa[c].detected==1)
				{
					_vmin[c]=face_sa[c].vmin;
					_vmax[c]=face_sa[c].vmax;
					//ROS_INFO("BEFORE CV_INRANGES");
					cvInRangeS( face_sa[c].hsv, cvScalar(0,face_sa[c].smin,MIN(_vmin[c],_vmax[c]),0),
					cvScalar(180,256,MAX(_vmin[c],_vmax[c]),0), face_sa[c].mask );

					cvSplit( face_sa[c].hsv, face_sa[c].hue, 0, 0, 0 );
					//ROS_INFO("AFTER CV_SPLIT");
					//ROS_INFO("FACE[%d].TRACK_OBJECT=%d",c,face_sa[c].track_object);
					if(face_sa[c].track_object<0)
					{	
						//ROS_INFO("IN TRACK LOOP");
						//face_sa[c].max_val = 0.f;
						_max_val[c] = 0.f;
						//ROS_INFO("SET IMAGE ROI");
						cvSetImageROI( face_sa[c].hue, face_sa[c].selection );
						cvSetImageROI( face_sa[c].mask, face_sa[c].selection );
						//printf("%d ")
						//ros::Duration(5).sleep();
						//ROS_INFO("CALC HIST");
						cvCalcHist( &face_sa[c].hue, face_sa[c].hist, 0, face_sa[c].mask );
						//ROS_INFO("GETMINMAX");
						cvGetMinMaxHistValue( face_sa[c].hist, 0, &_max_val[c], 0, 0 );
						//ROS_INFO("CONVERT SCALE");
						cvConvertScale( face_sa[c].hist->bins, face_sa[c].hist->bins, _max_val[c] ? 255. / _max_val[c] : 0., 0 );
						//ROS_INFO("RESET IMAGE ROI");
						cvResetImageROI( face_sa[c].hue );
						cvResetImageROI( face_sa[c].mask );
						//ROS_INFO("MOVING IMAGE");
						//MOVE SELECTION BOX TO ORIGINAL IMAGE
						//(640/NO_FACES)*c,0,640/NO_FACES,cut_height));
						//ROS_INFO("C = %d",c);
						//face_sa[c].selection = cvRect(Faces[c].x-Faces[c].r/*+(640/NO_FACES)*c*/,Faces[c].y-Faces[c].r, 
						face_sa[c].track_window = face_sa[c].selection;
						//ROS_INFO("track_window_%d-------------%d %d %d %d",c,face_sa[c].track_window.x, face_sa[c].track_window.y,
						//face_sa[c].track_window.width, face_sa[c].track_window.height);
						
						face_sa[c].track_object = 1;
						cvZero( face_sa[c].histimg );
						face_sa[c].bin_w = face_sa[c].histimg->width / face_sa[c].hdims;
						//ROS_INFO("BEFORE HDIMS LOOP");
						for( i = 0; i < face_sa[c].hdims; i++ )
						{	//ROS_INFO("IN HDIMS LOOP");
							val = cvRound( cvGetReal1D(face_sa[c].hist->bins,i)*face_sa[c].histimg->height/255 );
							//ROS_INFO("CVSCALAR");
							///CvScalar color = hsv2rgb(i*180.f/face_sa[c].hdims);
							//ROS_INFO("CVRECT");
							///cvRectangle( face_sa[c].histimg, cvPoint(i*face_sa[c].bin_w,face_sa[c].histimg->height),
							///	cvPoint((i+1)*face_sa[c].bin_w,face_sa[c].histimg->height - val), color, -1, 8, 0 );
						}
					}
					//ROS_INFO("cvCalcBackProject");
					//printf("\n\nCamshift1\n\n");
					cvCalcBackProject( &face_sa[c].hue, face_sa[c].backproject, face_sa[c].hist ); //Calculates the back projection. --> ypologizei to kentro mazas me bash ta barh
					//ROS_INFO("cvAnd");
					cvAnd( face_sa[c].backproject, face_sa[c].mask, face_sa[c].backproject, 0 );
					//ROS_INFO("BEFORE cvCamshift");
					//ROS_INFO("FACE_SA[%d].TRACK_WINDON : X = %d   Y = %d WIDTH = %d HEIGHT = %d",c,face_sa[c].track_window.x, 
					//	face_sa[c].track_window.y, face_sa[c].track_window.width, face_sa[c].track_window.height);
					cvCamShift( face_sa[c].backproject, face_sa[c].track_window,cvTermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ),
						&face_sa[c].track_comp, &face_sa[c].track_box );
					//ROS_INFO("AFTER CAMSHIFT");
					//ROS_INFO("FACE_SA[%d].TRACK_WINDON : X = %d   Y = %d WIDTH = %d HEIGHT = %d",c,face_sa[c].track_window.x, 
					//	face_sa[c].track_window.y, face_sa[c].track_window.width, face_sa[c].track_window.height);
					//face_sa[c].track_window = face_sa[c].track_comp.rect;
					//ROS_INFO("AFTER CAMSHIFT");
					//ROS_INFO("FACE_SA[%d].TRACK_WINDON : X = %d   Y = %d WIDTH = %d HEIGHT = %d",c,face_sa[c].track_window.x, 
					//	face_sa[c].track_window.y, face_sa[c].track_window.width, face_sa[c].track_window.height);
					if( face_sa[c].backproject_mode )
					{
						cvCvtColor( face_sa[c].backproject, face_sa[c].image, CV_GRAY2BGR ); //Converts an image from one color space to another.
					}
					
					if( !face_sa[c].image->origin )
					{
						face_sa[c].track_box.angle = -face_sa[c].track_box.angle; // CvBox2D - track_box
					}
					//ROS_INFO("AFTER IFS");
				}
			}
				
			//ros::Duration(3).sleep();
			for(c=0;c<NO_FACES;c++)
			{	
				if(face_sa[c].detected==1)
				{
					face_sa[c].new_center = face_sa[c].track_box.center;
					face_sa[c].new_center.x = face_sa[c].box_area.x + face_sa[c].track_box.center.x/*face_sa[c].box_area.width/2*/;
					face_sa[c].new_center.y = face_sa[c].box_area.y + face_sa[c].track_box.center.y/*face_sa[c].box_area.height/2*/;
					//face_sa[c].new_center.y = face_sa[c].track_box.center.x + (640/NO_FACES)*c  ;
					face_sa[c].tb2.center = face_sa[c].new_center;
					face_sa[c].tb2.size.width = face_sa[c].track_box.size.width;
					face_sa[c].tb2.size.height = face_sa[c].track_box.size.height;
					face_sa[c].tb2.angle = face_sa[c].track_box.angle;
					DTEMP=(face_sa[c].tb2.size.height*face_sa[c].tb2.size.width);
					if (DTEMP>DMAX)
					{
						DMAX=DTEMP;
						DOMINANT_ID=c+1;
					}
					//printf("C DURING ELLIPSE = %d",c);
					if(SPEAKER_ID==Faces[c].fid-1)
					{	
						if(flashing==1)
						{	
							//ROS_INFO("POINT #1");
							//cvEllipseBox(face_sa[c].image,face_sa[c].track_box,
							cvEllipseBox( iplImg /*face_sa[c].image*/, face_sa[c].tb2, 
							//cvCircle(iplImg,cvPointFrom32f(face_sa[c].tb2.center),
							//static_cast<int>(sqrt((pow(2,face_sa[c].tb2.size.width/2)) +
							//(pow(2,face_sa[c].tb2.size.height/2)))+0.5),
							CV_RGB(rgb_map_.rgb_ar[Faces[c].fid-1][0],rgb_map_.rgb_ar[Faces[c].fid-1][1],
									rgb_map_.rgb_ar[Faces[c].fid-1][2]), 3, CV_AA, 0 );
							flashing=-flashing;
						}
						else
						{
							flashing=-flashing;
						}
					}
					else
					{	
						//ROS_INFO("POINT #2");
						//cvEllipseBox(face_sa[c].image,face_sa[c].track_box,
						cvEllipseBox(iplImg,face_sa[c].tb2,
						//cvCircle(iplImg,cvPointFrom32f(face_sa[c].tb2.center),
						//	static_cast<int>(sqrt((pow(2,face_sa[c].tb2.size.width/2)) +
						//	(pow(2,face_sa[c].tb2.size.height/2)))+0.5),
						CV_RGB(rgb_map_.rgb_ar[Faces[c].fid-1][0],rgb_map_.rgb_ar[Faces[c].fid-1][1],
							rgb_map_.rgb_ar[Faces[c].fid-1][2]), 3, CV_AA, 0 );
					}
					sprintf(head,"Face_%d",c+1);
					cvShowImage(head,face_sa[c].image);
					strcpy(head,"Face_");
					cvWaitKey(1);
				}
			}
			//ros::Duration(10).sleep();
			//cvCircle(iplImg,cvPoint(100,100),10,CV_RGB(rgb_map_.rgb_ar[Faces[c].fid-1][0],rgb_map_.rgb_ar[Faces[c].fid-1][1],rgb_map_.rgb_ar[Faces[c].fid-1][2]),1,8,0);
			cvShowImage( "Enhanced Face Detection & Tracking", iplImg);
			cvWaitKey(1); // o xronos gia kathe frame
			//------------------------ press a key -----------------------
			//c = cvWaitKey(100);
			if( (char) c == 27 )
			{
				continue;
			}
			switch( (char) c )
			{
				case 'f':
				//detected = 0;
				for(c=0;c<NO_FACES;c++)
				{	
					cvZero( face_sa[c].histimg );
				}
				sleep(1);
				//return 0;
				case 's':
					sleep(5);
					continue;	
				default:
						;
			}
					
			cvReleaseImage(&iplImg);
			}
		}
	}
	//ROS_INFO("STILL INSIDE LOOP");
	}
	//ROS_INFO("WUT");    
	dom_id_msg.data=4;
	dom_id.publish(dom_id_msg);
    cvDestroyWindow("Enhanced Face Detection & Tracking");
	return 0;
}

face* detectAndDraw( Mat& img, CascadeClassifier& cascade, CascadeClassifier& nestedCascade, 
//void detectAndDraw( Mat& img, CascadeClassifier& cascade, CascadeClassifier& nestedCascade, 
		double scale, int person)
{
	char string_faces[20];
    int i = 0;
    double t = 0;
    vector<Rect> faces;
	
    const static Scalar colors[] =  { CV_RGB(0,0,255),
        CV_RGB(0,128,255),
        CV_RGB(0,255,255),
        CV_RGB(0,255,0),
        CV_RGB(255,128,0),
        CV_RGB(255,255,0),
        CV_RGB(255,0,0),
        CV_RGB(255,0,255)} ;
    Mat gray, smallImg( cvRound (img.rows/scale), cvRound(img.cols/scale), CV_8UC1 );
	
	cvtColor( img, gray, CV_BGR2GRAY );
    resize( gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR );
    equalizeHist( smallImg, smallImg );
    t = (double)cvGetTickCount();
	//|CV_HAAR_FIND_BIGGEST_OBJECT
	//|CV_HAAR_DO_ROUGH_SEARCH
	cascade.detectMultiScale( smallImg, faces,1.02, 2, 0|CV_HAAR_SCALE_IMAGE,Size(30, 30) );
	t = (double)cvGetTickCount() - t;
	//ROS_INFO("IN D&D");
    for( vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++,i++/*, i++*/ )
    {
        Mat smallImgROI;
        vector<Rect> nestedObjects;
        Point center;
        Scalar color = colors[i%8];
        int radius;
        center.x = cvRound((r->x + r->width*0.5)*scale);
        center.y = cvRound((r->y + r->height*0.5)*scale);
        radius = cvRound((r->width + r->height)*0.25*scale);
        
		snprintf(string_faces, sizeof(string_faces), "Face_%d", person+1);
		putText(img, string_faces, center, FONT_HERSHEY_COMPLEX_SMALL, 0.8, color, 1, CV_AA);
		
		if((i>=0)&&(center.x!=0)&&(center.y!=0))
		{
			Faces[person].fid=person+1;
			Faces[person].x = center.x;
			Faces[person].y = center.y;
			Faces[person].r = radius;
			//return Faces;
			//ROS_INFO("-------------\n%d %d %d %d\n\n",Faces[i].fid,Faces[i].x,Faces[i].y,Faces[i].r);
			
		}
	}

	occlusion = 1;
//}
	return /*;*/Faces;

}


