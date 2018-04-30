#ifndef _CV_BINDINGS_ACH_
#define _CV_BINDINGS_ACH_

#ifdef __cplusplus
	cv::Ptr<cv::BackgroundSubtractor> MOG2;

extern "C" {
#endif

	bool calibrate(const char* srcFile, const char* dstFile, int camW, int camH);

	void initMOG2(int history, double threshold, int detectShadows);

	void applyMOG2(const CvArr* image, CvArr* fgmask);

	//void startMeasure(string srcFile)
	//detectedOb grabFrame(bool debug)
	//void stopMeasure()
#ifdef __cplusplus
}
#endif

#endif