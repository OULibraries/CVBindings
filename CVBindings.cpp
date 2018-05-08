
#include "CVBindings.h"
#include <vector>
#include <unistd.h>

using namespace std;
using namespace cv;

cv::VideoCapture getCapture(const char* filename, int frameW, int frameH) {
	if (strcmp("", filename) == 0) {
		// Capture from webcam.
		cv::VideoCapture camera = cv::VideoCapture(0);
		camera.set(CV_CAP_PROP_FRAME_WIDTH, frameW);
		camera.set(CV_CAP_PROP_FRAME_HEIGHT, frameH);
		camera.set(CV_CAP_PROP_BUFFERSIZE, 1);

		return camera;
	} else {
		// Capture from file.
		return cv::VideoCapture(filename);
	}
}

extern "C" bool calibrate(const char* srcFile, const char* dstFile, int camW, int camH) {
	// It takes a little while for the white balance to stabalise on the logitech webCam
	// So grab a frame, wait to stabalise for white balance to stabalise, then grab again
	// for and save as the calibration frame.
	cv::VideoCapture cam = getCapture(srcFile, camW, camH);
	Mat frame;
	cam >> frame;

	usleep(1250);

	cam.release();
	cam = getCapture(srcFile, camW, camH);
	cam >> frame;

	cv::imwrite(dstFile, frame);

	return true;
}

extern "C" bool startMeasure(const char* srcFile, const char* calFile,
							 int camW, int camH,
							 int mogHistory, double mogThreshold, int mogDetectShadows) {
	measureCap = getCapture(srcFile, camW, camH);
	calFrame = cv::imread(srcFile);
	maskFrame = cv::Mat(camH, camW, CV_8U);

	MOG2 = createBackgroundSubtractorMOG2(mogHistory, mogThreshold, mogDetectShadows);
	MOG2->apply(calFrame, maskFrame);

	frameNum = 0;

	return true;
}

extern "C" int* grabFrame(int* numObjects, bool debug, double gaussianSmooth, double foregroundThresh, int dilationIterations, double minArea, double maxArea) {
	Mat nextFrame;
	measureCap >> nextFrame;

	// Subtract the calibration frame from the current frame.
	MOG2->apply(nextFrame, maskFrame);

	// Filter the foreground mask to clean up any noise or holes (morphological-closing).
	GaussianBlur(maskFrame, maskFrame, cv::Size(3, 3), gaussianSmooth);
	threshold(maskFrame, maskFrame, foregroundThresh, 255, THRESH_BINARY);
	dilate(maskFrame, maskFrame, Mat(), Point(-1, -1), dilationIterations);

	// Detect contours in filtered foreground mask
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(maskFrame, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

	//int num = cvFindContours(mask, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, offset);
	int* objects = (int*) malloc(4*contours.size()*sizeof(int));
	*numObjects = 0;

	for (int i = 0; i < contours.size(); i++) {
		double area = contourArea(contours[i]);

		if (area > minArea && area < maxArea) {
			Rect boundingBox = boundingRect(contours[i]);

			objects[*numObjects] = boundingBox.width / 2;
			objects[*numObjects + 1] = boundingBox.height / 2;
			objects[*numObjects + 2] = boundingBox.x + objects[*numObjects];
			objects[*numObjects + 3] = boundingBox.y + objects[*numObjects + 1];

			*numObjects = *numObjects + 1;

			if (debug) {
	 			Point pt1(boundingBox.x, boundingBox.y);
	 			Point pt2(boundingBox.x+boundingBox.width, boundingBox.y+boundingBox.height);

	 			drawContours(nextFrame, contours, -1, Scalar(12.0, 212.0, 250.0), 5);
	 			rectangle(nextFrame, pt1, pt2, Scalar(16.0, 8.0, 186.0), 5, 8, 0);

	 			imwrite(to_string(frameNum)+".jpg", nextFrame);
			}
		}
	}
	frameNum++;

	return objects;
}

extern "C" void stopMeasure() {
}