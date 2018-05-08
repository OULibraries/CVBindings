#ifndef _CV_BINDINGS_ACH_
#define _CV_BINDINGS_ACH_

#include <stdbool.h>

#ifdef __cplusplus
	#include "opencv/cv.h"
	#include "opencv/highgui.h"
    #include "opencv2/opencv.hpp"

	cv::Ptr<cv::BackgroundSubtractor> MOG2;
	cv::VideoCapture measureCap;
	cv::Mat calFrame;
	cv::Mat maskFrame;
	uint frameNum;

extern "C" {
#endif
	bool calibrate(const char* srcFile, const char* dstFile, int camW, int camH);

	bool startMeasure(const char* srcFile, const char* calFile,
					  int camW, int camH,
					  int mogHistory, double mogThreshold, int mogDetectShadows);

	int* grabFrame(int* numObjects, bool debug, double gaussianSmooth, double foregroundThresh, int dilationIterations, double minArea, double maxArea);

	void stopMeasure();

	//void renderDebug()

#ifdef __cplusplus
}
#endif

#endif