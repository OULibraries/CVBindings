#ifndef _CV_BINDINGS_ACH_
#define _CV_BINDINGS_ACH_

#ifdef __cplusplus
	cv::Ptr<cv::BackgroundSubtractor> MOG2;
extern "C" {
#endif
	void initMOG2(int history, double threshold, int detectShadows);

	void applyMOG2(const CvArr* image, CvArr* fgmask);
#ifdef __cplusplus
}
#endif

#endif