#include "BackgroundSubtractorMOG2.h"

SubtractorMOG2 createMOG2(int history, double threshold, bool detectShadows) {
	return cv::createBackgroundSubtractorMOG2(history, threshold, detectShadows);
}

void applyMOG2(SubtractorMOG2 mog2, const CvArr* image, CvArr* fgmask, double learning) {
	cv::Mat img = cv::cvarrToMat(image);
	cv::Mat fgm = cv::cvarrToMat(fgmask);

	mog2->apply(img, fgm, learning);
}
