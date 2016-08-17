
#include "opencv2/opencv.hpp"
#include "CVBindings.h"
#include <vector>

using namespace std;
using namespace cv;

extern "C" void initMOG2(int history, double threshold, int detectShadows) {
	MOG2 = createBackgroundSubtractorMOG2(history, threshold, detectShadows);
}

extern "C" void applyMOG2(const CvArr* image, CvArr* fgmask) {
	Mat img = cvarrToMat(image);
	Mat mask = cvarrToMat(fgmask);

	MOG2->apply(img, mask);
}
