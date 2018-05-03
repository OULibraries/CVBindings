
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

cv::VideoCapture getCapture(const char* filename, int frameW, int frameH) {
	if (strcmp("", filename) == 0) {
		// Capture from webcam.
		cv::VideoCapture camera = cv::VideoCapture(0);
		camera.set(CV_CAP_PROP_FRAME_WIDTH, frameW);
		camera.set(CV_CAP_PROP_FRAME_HEIGHT, frameH);
		//cvSetCaptureProperty(camera, CV_CAP_PROP_BUFFERSIZE, 1);

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

	cam = getCapture(srcFile, camW, camH);
	cam >> frame;

	cv::imwrite(dstFile, frame);

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

extern "C" int* grabFrame(int* numObjects, bool debug, double gaussianSmooth, double foregroundThresh, int dilationIterations, double minArea, double maxArea) {
	cvGrabFrame(measureCam);
	IplImage *nextFrame = cvRetrieveFrame(measureCam, 0);

	// Subtract the calibration frame from the current frame.
	MOG2->apply(cvarrToMat(nextFrame), cvarrToMat(mask));

	// Filter the foreground mask to clean up any noise or holes (morphological-closing).
	cvSmooth(mask, mask, CV_GAUSSIAN, 3, 0, 0.0, gaussianSmooth);
	cvThreshold(mask, mask, foregroundThresh, 255, 0);
	cvDilate(mask, mask, NULL, dilationIterations);

	// Detect contours in filtered foreground mask
	CvMemStorage *storage = cvCreateMemStorage(0);
	CvSeq *contours = cvCreateSeq(0, sizeof(CvSeq), sizeof(CvPoint), storage);
	CvPoint offset = cvPoint(0, 0);

	int num = cvFindContours(mask, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, offset);
	int* objects = (int*) malloc(4*num*sizeof(int));
	*numObjects = 0;

	while (contours != NULL) {
		double area = cvContourArea(contours, cvSlice(0, 0x3fffffff), 0);

		// Only track appropriately sized objects.
		if (area > minArea && area < maxArea) {
			CvRect boundingBox = cvBoundingRect(contours, 0);

			objects[*numObjects] = boundingBox.width / 2;
			objects[*numObjects + 1] = boundingBox.height / 2;
			objects[*numObjects + 2] = boundingBox.x + objects[*numObjects];
			objects[*numObjects + 3] = boundingBox.y + objects[*numObjects + 1];

			*numObjects = *numObjects + 1;

			if (debug) {
				CvPoint pt1 = cvPoint(boundingBox.x, boundingBox.y);
				CvPoint pt2 = cvPoint(boundingBox.x+boundingBox.width, boundingBox.y+boundingBox.height);
				cvDrawContours(nextFrame, contours, cvScalar(12.0, 212.0, 250.0, 255), cvScalar(0, 0, 0, 0), 2, 1, 8, offset);
				cvRectangle(nextFrame, pt1, pt2, cvScalar(16.0, 8.0, 186.0, 255), 5, 8, 0);
			}
		} else {
			num--;
		}

		contours = contours->h_next;
	}

	cvClearMemStorage(storage);
	cvReleaseMemStorage(&storage);

	return objects;
}