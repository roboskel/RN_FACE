RN_FACE

A face detection and tracking ROS node written by SKEL for Researcher's Night 2013. It presents an improvement on using simple OpenCV
face detection (which utilizes the Viola–Jones object detection framework) by integrating the head coordinates of each person in front of the camera provided by
the Microsoft Kinect mounted on Sek's mast. By using the OpenNI libraries the program is used to limit the search area in the approximate
area of each user's head, increasing overall accuracy and limiting the lighting and environment effects. The package consists of the following nodes :

CAP_IMG_D

Captures the images from the camera and stores them in ~/.ros using the timestamp as a file name as well as publishing the filename 
over the topic "img_addr"

TRANS_SKEL

Loops over /tf looking for the coordinates of up to 6 users' heads provided by the openni_tracker. For each user found, a message is created
consisting of the head coordinates and the user id. If the user has the same coordinates in two consecutive frames, which means he/she
has left the camera's field of view, the the coordinates are set to zero. After each loop, the messages are pushed back into a vector along 
with the number of users (number of non-zero coordinates), which is then published over the topic "head_cords"

FDATI

A Face Detection And Tracking node that uses the Viola–Jones object detection framework similar to the one used in the face_rec
package. It listens to the "head_cords" topic for the number of users needed to detect and searches for that particular number of faces
on the whole image provided by the "img_addr" topic. It was used simultaneously with the FDC2 node to show the improvement provided
by the latter

FDC2

An improved version of the above node, it uses the "head_cords" topic not only to identify the number of persons in front of 
the camera but also the approximate location of their heads (and faces). It then proceeds to project that location to the 2D image
provided by the "img_addr" topic, and create smaller images by cropping the bigger images at the specified coordinates. For each 
smaller image created, the node searches for a face and if it is detected it is drawn and tracked on the initial larger image.

LAUNCH FILES

REC2.LAUNCH
Launches all the above files as well as the openni_tracker and the Hokuyo recording node from the face_rec package, and records the 
output in a bag named recordings.bag. The images as well as the .bag file are located in ~/.ros directory

LIVE2.LAUNCH
Launches the following nodes:
CAP_IMG_D
TRANS_SKEL
OPENNI_TRACKER
IMAGE_VIEW (for depth imaging)
FDC2
FDATI

In order for the nodes to run a 1 must by published in the "comms" topic or a 0 for the nodes to stop. This is made easier with the
following node, and the start/stop scripts included

CONTROLLER
Using an third party PS3 like controller this node is used to facilitate the testing and running of the package:

START BUTTON : Publishes a start messsage for the CAP_IMG_D and TRANS_SKEL nodes
SELECT BUTTON : Publishes a stop message for the CAP_IMG_D and TRANS_SKEL nodes and kills the rest of the nodes launched
TRIANGLE BUTTON : Launches live2.launch
CIRCLE BUTTON : Launches re2.launch
X BUTTON : Kills and relaunches openni_launch
SQUARE BUTON : Kills openni_launch
LEFT TRIGGER (L2) : Shuts down the controller node
LEFT BUMPER (L1) : Starts the remote control node
RIGHT BUMPER (R1) : Deletes recorded data
UP BUTTON (D-PAD): Stops the face detection nodes, but not openni_launch or the tracker
DOWN BUTTON : Starts/Stop the hokuyo node




