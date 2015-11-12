#ifndef _BACKGROUND_SUBTRACTOR_MOG_ACH_
#define _BACKGROUND_SUBTRACTOR_MOG_ACH_

#ifdef __cplusplus
extern "C" {
#endif
	void* createMOG2(int history, double threshold, int detectShadows);

	void applyMOG2(void* mog2, const CvArr* image, CvArr* fgmask, double learning);
#ifdef __cplusplus
}
#endif

#endif