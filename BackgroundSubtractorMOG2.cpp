
#include "opencv2/opencv.hpp"
#include "BackgroundSubtractorMOG2.h"

extern "C" void initMOG2(int history, double threshold, int detectShadows) {
	MOG2 = cv::createBackgroundSubtractorMOG2(history, threshold, detectShadows);
}

extern "C" void applyMOG2(const CvArr* image, CvArr* fgmask) {	
	cv::Mat img = cv::cvarrToMat(image);
	cv::Mat mask = cv::cvarrToMat(fgmask);

	MOG2->apply(img, mask);
}
