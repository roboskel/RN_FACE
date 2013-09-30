#!/usr/bin/env python
import rospy
import sys
import os
import alsaaudio, time, audioop
import matplotlib.pyplot as plt
import scipy.io.wavfile as wavfile
import wave
import numpy
from sys import exit
from std_msgs.msg import String
from std_msgs.msg import Float64MultiArray
from std_msgs.msg import Float64
from std_msgs.msg import Int32

REC = 1
CANCEL = 0

Fs = 16000

def recordAudioSegments(RecordPath):	
	# This function is used for recording audio segments (until ctr+c is pressed)
	# ARGUMENTS:
	# - RecordPath:		the path where the wav segments will be stored
	# 
	# NOTE: filenames are based on clock() value
	global Fs
	d = os.path.dirname(RecordPath)
	if os.path.exists(d):
		shutil.rmtree(RecordPath)	
	os.makedirs(RecordPath)
	
	inp = alsaaudio.PCM(alsaaudio.PCM_CAPTURE,alsaaudio.PCM_NONBLOCK)
	inp.setchannels(1)
	inp.setrate(Fs)
	inp.setformat(alsaaudio.PCM_FORMAT_S16_LE)
	inp.setperiodsize(512)
	midTermBufferSize = Fs
	midTermBuffer = []
	curWindow = []
	elapsedTime = "%08.3f" % (time.time())
	while 1:
			l,data = inp.read()		   
		    	if l:
				for i in range(len(data)/2):
					curWindow.append(audioop.getsample(data, 2, i))
		
				if (len(curWindow)+len(midTermBuffer)>midTermBufferSize):
					samplesToCopyToMidBuffer = midTermBufferSize - len(midTermBuffer)
				else:
					samplesToCopyToMidBuffer = len(curWindow)

				midTermBuffer = midTermBuffer + curWindow[0:samplesToCopyToMidBuffer];
				del(curWindow[0:samplesToCopyToMidBuffer])
			

			if len(midTermBuffer) == midTermBufferSize:
				# allData = allData + midTermBuffer				
				curWavFileName = RecordPath + os.sep + str(elapsedTime) + ".wav"				
				midTermBufferArray = numpy.int16(midTermBuffer)
				wavfile.write(curWavFileName, Fs, midTermBufferArray)
				print "AUDIO  OUTPUT: Saved " + curWavFileName
				midTermBuffer = []
				elapsedTime = "%08.3f" % (time.time())
				#####	
				pub = rospy.Publisher('audio_adr',String)
				str = curWavFileName
				rospy.loginfo(str)
				pub.publish(String(str))
				####

def callback(data):
	global REC
	#functions=data.data
	#functions = []
	#for x in data.data:
	#functions.append(x)
	#functions=np.array(functions)
	functions=data.data
	if (functions==1):
	#if (functions[11]==1):
			REC=1
			functions=3
			if (REC==1):
				print "Start Audio Recording"
				recordAudioSegments("rec/audio/")
	else if (functions==0 and REC = 1) :
		print "Stopping Audio Recording"
		rospy.signal_shutdown("pewpew")
		exit(0)

def callback2(data):
	#functions2 = []
	#for x in data.data:
	#	functions2.append(x)
	#functions=np.array(functions)
	#if ((data.functions[5]==1)and(REC==1)):
	functions2=data.data
	if(functions2==0):
		print "Stopping Audio Recording"
		rospy.signal_shutdown("pewpew")
		exit(0)
		
		

def listener():
    rospy.init_node('listener', anonymous=True)
    #rospy.Subscriber("gpfunctions2", Float64MultiArray, callback)
    #rospy.Subscriber("gpfunctions2", Float64MultiArray, callback2)
    print "Waiting Gamepad Input"
    rospy.Subscriber("coms", Float64, callback)
    rospy.Subscriber("coms", Float64, callback2)
    rospy.spin()


if __name__ == '__main__':
    listener()
