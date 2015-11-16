#ifndef _CV_BINDINGS_ACH_
#define _CV_BINDINGS_ACH_

#ifdef __cplusplus
	cv::Ptr<cv::BackgroundSubtractor> MOG2;
	cv::Ptr<cv::SimpleBlobDetector> BLOB;
extern "C" {
#endif
	void initMOG2(int history, double threshold, int detectShadows);

	void applyMOG2(const CvArr* image, CvArr* fgmask);

	void initBlob(int minArea, int maxArea, int minThreshold, int maxThreshold);

	int detectBlobs(const CvArr* image, int* blobs);
#ifdef __cplusplus
}
#endif

#endif