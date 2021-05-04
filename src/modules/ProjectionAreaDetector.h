#pragma once

#include <opencv2/opencv.hpp>
#include "Header.h"

#define ENABLE_MASK_VALUE	0		// Value to enable the mask
#define DISABLE_MASK_VALUE	255		// Value to disable the mask
#define MASK_COLOR			0		// Color of the mask

class ProjectionAreaDetector
{
public:
	ProjectionAreaDetector();
	ProjectionAreaDetector(int _binThresh, int _thick);
	~ProjectionAreaDetector();

	void getMask(cv::Mat image1, cv::Mat image2);
	void applyMask(cv::Mat* image);
	void mouseCallback(int event, int x, int y, int flags, void *userdata);
	void combineImage(cv::Mat foreImage, cv::Mat backImage, cv::Mat* dst);
	void warpImage(cv::Mat image, cv::Mat* dst, cv::Mat perspectiveMatrix);

	void setBinarizeThreshold(int thrshold);
	void setThickSize(int size);

private:
	void setSurroundingPixels(cv::Mat* image, int x, int y, int range, int value);
	void scaleToHeight(cv::Mat srcImage, cv::Mat* dstImage, int height);
	void scaleToWidth(cv::Mat srcImage, cv::Mat* dstImage, int width);

private:
	int m_binThresh;	// binarization threshold
	int m_thick;		// thickness
	
	cv::Mat m_mask;		// mask
	cv::Rect m_rect;	// mask area

	bool m_LbuttonClicked, m_RbuttonClicked = false;	// Is the button clicked?
};

