
#include "opencv2/opencv.hpp"
#include "BackgroundSubtractorMOG2.h"

extern "C" void* createMOG2(int history, double threshold, int detectShadows) {
	return cv::createBackgroundSubtractorMOG2(history, threshold, detectShadows);
}

extern "C" void applyMOG2(void* mog2, const CvArr* image, CvArr* fgmask, double learning) {
	cv::Mat img = cv::cvarrToMat(image);
	cv::Mat fgm = cv::cvarrToMat(fgmask);

	cv::BackgroundSubtractorMOG2* mog = (cv::BackgroundSubtractorMOG2*) mog2;

	mog->apply(img, fgm, learning);
}
