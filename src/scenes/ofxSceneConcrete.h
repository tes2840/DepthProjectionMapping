#pragma once
#include "libs/ofxState.h"
#include "SharedData.h"

class ofxSceneConcrete : public itg::ofxState<SharedData>
{
protected:
	/**
	  * @brief convert Mat to ofImage
	  * @param[out] image - matrix
	  * @param[in] ofImage - ofImage
	  * @return void
	  */
	void convertMatToOfImage(const cv::Mat image, ofImage* ofImage) {
		// convert Mat to ofImage
		cv::Mat tempImage;
		cv::cvtColor(image, tempImage, cv::COLOR_RGB2BGR);
		ofImage->allocate(tempImage.cols, tempImage.rows, OF_IMAGE_COLOR);
		ofImage->setFromPixels(tempImage.ptr(), tempImage.cols, tempImage.rows, OF_IMAGE_COLOR, false);
	}
};