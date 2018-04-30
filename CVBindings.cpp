
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


extern "C" void initMOG2(int history, double threshold, int detectShadows) {
	MOG2 = createBackgroundSubtractorMOG2(history, threshold, detectShadows);
}

extern "C" void applyMOG2(const CvArr* image, CvArr* fgmask) {
	Mat img = cvarrToMat(image);
	Mat mask = cvarrToMat(fgmask);

	MOG2->apply(img, mask);
}
