#ifndef _CV_BINDINGS_ACH_
#define _CV_BINDINGS_ACH_

#ifdef __cplusplus
	cv::Ptr<cv::BackgroundSubtractor> MOG2;

	CvCapture *measureCam;
	IplImage *calibrationFrame;
	IplImage *mask;
	bool measuring;

extern "C" {
#endif
	bool calibrate(const char* srcFile, const char* dstFile, int camW, int camH);

	bool startMeasure(const char* srcFile, const char* calFile,
					  int camW, int camH,
					  int mogHistory, double mogThreshold, int mogDetectShadows);

	int* grabFrame(int* numObjects, bool debug, double gaussianSmooth, double foregroundThresh, double dilationIterations, double minArea, double maxArea);

	void stopMeasure();

	void initMOG2(int history, double threshold, int detectShadows);

	void applyMOG2(const CvArr* image, CvArr* fgmask);

	//detectedOb grabFrame(bool debug)
	//
#ifdef __cplusplus
}
#endif

#endif