
#include "opencv2/opencv.hpp"
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "CVBindings.h"
#include <vector>
#include <unistd.h>

using namespace std;
using namespace cv;

CvCapture* getVideoSource(const char* filename, int frameW, int frameH) {
	if (strcmp("", filename) == 0) {
		// Capture from webcam.
		CvCapture* camera = cvCaptureFromCAM(0);
		if (camera == NULL) {
			return NULL; // Unable to get device.
		}

		cvSetCaptureProperty(camera, CV_CAP_PROP_FRAME_WIDTH, frameW);
		cvSetCaptureProperty(camera, CV_CAP_PROP_FRAME_HEIGHT, frameH);
		cvSetCaptureProperty(camera, CV_CAP_PROP_BUFFERSIZE, 1);

		return camera;
	} else {
		// Capture from file.
		return cvCaptureFromFile(filename);
	}

	return NULL;
}

extern "C" bool calibrate(const char* srcFile, const char* dstFile, int camW, int camH) {
	// It takes a little while for the white balance to stabalise on the logitech webCam
	// So grab a frame, wait to stabalise for white balance to stabalise, then grab again
	// for and save as the calibration frame.
	CvCapture *cam = getVideoSource(srcFile, camW, camH);
	if (cam == NULL) {
		return false;
	}

	IplImage *calibrationFrame = cvQueryFrame(cam);
	usleep(1250);
	cvReleaseCapture(&cam);

	cam = getVideoSource(srcFile, camW, camH);
	if (cam == NULL) {
		return false;
	}

	calibrationFrame = cvQueryFrame(cam);
	cvSaveImage(dstFile, calibrationFrame, NULL);
	cvReleaseCapture(&cam);

	return true;
}

extern "C" bool startMeasure(const char* srcFile, const char* calFile,
							 int camW, int camH,
							 int mogHistory, double mogThreshold, int mogDetectShadows) {
	measureCam = getVideoSource(srcFile, camW, camH);
	if (measureCam == NULL) {
		return false;
	}

	calibrationFrame = cvLoadImage(calFile, CV_LOAD_IMAGE_COLOR);
	mask = cvCreateImage(cvSize(calibrationFrame->width, calibrationFrame->height), IPL_DEPTH_8U, 1);

	MOG2 = createBackgroundSubtractorMOG2(mogHistory, mogThreshold, mogDetectShadows);
	MOG2->apply(cvarrToMat(calibrationFrame), cvarrToMat(mask));

	return true;
}

extern "C" void stopMeasure() {
	cvReleaseImage(&mask);
	cvReleaseImage(&calibrationFrame);
	cvReleaseCapture(&measureCam);
}

extern "C" void grabFrame(bool debug, double gaussianSmooth, double foregroundThresh, double dilationIterations, double minArea, double maxArea) {
	IplImage *nextFrame = cvRetrieveFrame(measureCam, 0);
	MOG2->apply(cvarrToMat(nextFrame), cvarrToMat(mask));

	cvSmooth(mask, mask, CV_GAUSSIAN, 3, 0, 0.0, gaussianSmooth);
	cvThreshold(mask, mask, foregroundThresh, 255, 0);
	cvDilate(mask, mask, NULL, dilationIterations);

	CvMemStorage *storage = cvCreateMemStorage(0);
	CvSeq *contours = cvCreateSeq(0, sizeof(CvSeq), sizeof(CvPoint), storage);
	CvPoint offset = cvPoint(0, 0);

	int num = cvFindContours(mask, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, offset);

	while (contours != NULL) {
		double area = cvContourArea(contours, cvSlice(0, 0x3fffffff), 0);

		// Only track appropriately sized objects.
		if (area > minArea && area < maxArea) {
			CvBoundingRect boundingBox = CvBoundingRect(contours, 0);

			// append results to something we can jam out to Go land.
		}
	}

}


extern "C" void initMOG2(int history, double threshold, int detectShadows) {
	MOG2 = createBackgroundSubtractorMOG2(history, threshold, detectShadows);
}

extern "C" void applyMOG2(const CvArr* image, CvArr* fgmask) {
	Mat img = cvarrToMat(image);
	Mat mask = cvarrToMat(fgmask);

	MOG2->apply(img, mask);
}
