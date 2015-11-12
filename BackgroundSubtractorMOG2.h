#ifndef _BACKGROUND_SUBTRACTOR_MOG_ACH_
#define _BACKGROUND_SUBTRACTOR_MOG_ACH_

#include "cv.h"
#include "opencv2/opencv.hpp"

typedef cv::BackgroundSubtractorMOG2* SubtractorMOG2;

extern "C" {
	SubtractorMOG2 createMOG2(int history, double threshold, bool detectShadows);

	void applyMOG2(SubtractorMOG2 mog2, const CvArr* image, CvArr* fgmask, double learning);
}

#endif