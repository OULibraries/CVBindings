
#include "opencv2/opencv.hpp"
#include "CVBindings.h"
#include <Vector>

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

extern "C" void initBlob(int minArea, int maxArea, int minThreshold, int maxThreshold) {
	SimpleBlobDetector::Params params;

	params.minThreshold = minThreshold;
	//params.minThreshold = maxThreshold;

	//params.filterByColor = true;
	//params.blobColor = 255;

	params.filterByArea = true;
	params.minArea = minArea;
	params.maxArea = maxArea;

	BLOB = SimpleBlobDetector::create();
}

extern "C" int detectBlobs(const CvArr* image, int* blobs) {
	Mat img = cvarrToMat(image);
	vector<cv::KeyPoint> detected;

	BLOB->detect(img, detected);

	for (int i = 0; i < detected.size(); i++) {
		int bi = i * 3;

		blobs[bi] = (int) detected.at(i).pt.x;
		blobs[bi+1] = (int) detected.at(i).pt.y;
		blobs[bi+2] = (int) detected.at(i).size;
	}

	return detected.size();
}


