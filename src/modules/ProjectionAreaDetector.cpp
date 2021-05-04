#include "modules/ProjectionAreaDetector.h"

/**
  * @brief Constractor of the ProjectionAreaDetector class.
  */
ProjectionAreaDetector::ProjectionAreaDetector() : m_binThresh(20), m_thick(20)
{
}

/**
  * @brief Costractor of the ProjectionAreaDetector class
  */
ProjectionAreaDetector::ProjectionAreaDetector(int _binThresh, int _thick)
	: m_binThresh(_binThresh), m_thick(_thick)
{
}

/**
  * @brief Destractor of the ProjectionAreaDetector class.
  */
ProjectionAreaDetector::~ProjectionAreaDetector()
{
}

/**
  * @brief Create the mask image by calculating the difference of absolute values of each element of the two arrays.
  * @param[in] image1 - first input array.
  * @param[in] image2 - second input array or a scalar.
  */
void ProjectionAreaDetector::getMask(cv::Mat image1, cv::Mat image2)
{
	// Convert to grayscale
	cv::Mat grayImage1, grayImage2;
	cv::cvtColor(image1, grayImage1, cv::COLOR_BGR2GRAY);
	cv::cvtColor(image2, grayImage2, cv::COLOR_BGR2GRAY);

	// Calculates the per-element absolute difference between two arrays.
	cv::Mat diffImage;
	cv::absdiff(grayImage1, grayImage2, diffImage);

	// Binarization
	cv::threshold(diffImage, m_mask, m_binThresh, 255, cv::THRESH_BINARY);
}

/**
  * @brief Create the mask image by calculating the difference of absolute values of each element of the two arrays.
  * @param[in/out] image - Image with mask applied.
  */
void ProjectionAreaDetector::applyMask(cv::Mat* image)
{
	if (m_mask.empty()) {
		return;
	}

	for (int i = 0; i < image->rows; i++) {
		for (int j = 0; j < image->cols; j++) {
			if (m_mask.at<unsigned char>(i, j) == ENABLE_MASK_VALUE) {
				setSurroundingPixels(image, i, j, 1, MASK_COLOR);
			}
		}
	}

	// Calculate the mask area by approximating the mask to a rectangle.
	m_rect = cv::boundingRect(m_mask);
	cv::rectangle(*image, m_rect, cv::Scalar(0, 255, 0), 2);
}

/**
  * @brief Callback function for mouse events.
  * @details Disable masking of pixels around the left-clicked point and enable around the left-clicked point.
  * @param[in] event - 	One of the cv::MouseEventTypes constants.
  * @param[in] x - 	The x-coordinate of the mouse event.
  * @param[in] y - The y-coordinate of the mouse event.
  * @param[in] flags - 	One of the cv::MouseEventFlags constants.
  * @param[in] *userdata - The optional parameter.
  */
void ProjectionAreaDetector::mouseCallback(int event, int x, int y, int flags, void *userdata) {
	// Disable masking of pixels around the left-clicked point.
	// Enable masking of pixels around the left-clicked point.
	if (event == cv::EVENT_LBUTTONDOWN) {
		m_LbuttonClicked = true;
		cv::circle(m_mask, cv::Point(x, y), m_thick, (DISABLE_MASK_VALUE, DISABLE_MASK_VALUE, DISABLE_MASK_VALUE), -1);
	}
	else if (event == cv::EVENT_LBUTTONUP) {
		m_LbuttonClicked = false;
	}
	else if (event == cv::EVENT_RBUTTONDOWN) {
		m_RbuttonClicked = true;
		cv::circle(m_mask, cv::Point(x, y), m_thick, (ENABLE_MASK_VALUE, ENABLE_MASK_VALUE, ENABLE_MASK_VALUE), -1);
	}
	else if (event == cv::EVENT_RBUTTONUP) {
		m_RbuttonClicked = false;
	}
	else if (event == cv::EVENT_MOUSEMOVE) {
		if (m_LbuttonClicked == true) {
			// Disable masking of the area if the left click is hld down.
			cv::circle(m_mask, cv::Point(x, y), m_thick, (DISABLE_MASK_VALUE, DISABLE_MASK_VALUE, DISABLE_MASK_VALUE), -1);
		} else if(m_RbuttonClicked == true){
			// Enable masking of the area if the right click is hld down.
			cv::circle(m_mask, cv::Point(x, y), m_thick, (ENABLE_MASK_VALUE, ENABLE_MASK_VALUE, ENABLE_MASK_VALUE), -1);
		}
	}
}

/**
  * @brief Sets the specified value around the specified position of the image.
  * @param[in/out] image - 	image array.
  * @param[in] x - x-coordinate.
  * @param[in] y - y-coordinate.
  * @param[in] range - Range of the surrounding area to set the value.
  * @param[in] value - Value to set for the image.
  */
void ProjectionAreaDetector::setSurroundingPixels(cv::Mat* image, int x, int y, int range, int value) {
	int rows = image->rows;
	int cols = image->cols;

	for (int i = -range; i < range; i++) {
		for (int j = -range; j < range; j++) {
			int row = x + i;
			int col = y + j;

			if ( ((row < 0) || (rows <= row)) || ((col < 0) || (cols <= col)) ) {
				// Skip if outside the image area.
				continue;
			}
			if (image->channels() == 1) {
				image->at<unsigned char>(row, col) = value;
			}
			else if(image->channels() == 3){
				image->at<cv::Vec3b>(row, col)[0] = value;	//Blue
				image->at<cv::Vec3b>(row, col)[1] = value;	//Green
				image->at<cv::Vec3b>(row, col)[2] = value;	//Red
			}
			else {
				// channel 1, 3ˆÈŠO‚Í‰½‚à‚µ‚È‚¢
			}
		}
	}
}

/**
  * @brief Resize the image to the height becomes the specified value with the fixed aspect ratio.
  * @param[in] srcImage - input image array.
  * @param[out] dstImage - output image array.
  * @param[in] height - Height of the image to resize.
  */
void ProjectionAreaDetector::scaleToHeight(cv::Mat srcImage, cv::Mat* dstImage, int height) {
		int h = srcImage.rows;
		int w = srcImage.cols;
		int width = round( (float)w * ((float)height / (float)h) );

		cv::resize(srcImage, *dstImage, cv::Size(width, height));
}

/**
  * @brief Resize the image to the width becomes the specified value with the fixed aspect ratio.
  * @param[in] srcImage - input image array.
  * @param[out] dstImage - output image array.
  * @param[in] width - width of the image to resize.
  */
void ProjectionAreaDetector::scaleToWidth(cv::Mat srcImage, cv::Mat* dstImage, int width) {
	int h = srcImage.rows;
	int w = srcImage.cols;
	int height = round((float)h * ((float)width / (float)w));

	cv::resize(srcImage, *dstImage, cv::Size(width, height));
}

/**
  * @brief Combine two images using a mask.
  * @param[in] foreImage - fore image array.
  * @param[in] backImage - back image array.
  * @param[out] dst - composite image array.
  */
void ProjectionAreaDetector::combineImage(cv::Mat foreImage, cv::Mat backImage, cv::Mat* dst) {
	// Create a black monochromatic image
	*dst = cv::Mat::zeros(cv::Size(backImage.cols, backImage.rows), CV_8UC3);

	// Resize the foreground image to fit the mask
	cv::Mat resizeForeImage;
	scaleToWidth(foreImage, &resizeForeImage, m_rect.height);
	
	// The width and height are taken from the common area of the foreground and background images
	int width = cv::min(resizeForeImage.cols, backImage.cols - m_rect.x);
	int height = cv::min(resizeForeImage.rows, backImage.rows - m_rect.y);

	// Overlay area
	cv::Mat fgRoi = resizeForeImage(cv::Range(0, height), cv::Range(0, width));

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			int row = m_rect.y + j;
			int col = m_rect.x + i;

			// Replace the unmasked portion of the background image with the foreground image
			if (m_mask.at<unsigned char>(row, col) == DISABLE_MASK_VALUE) {
				try {
					dst->at<cv::Vec3b>(row, col)[0] = fgRoi.at<cv::Vec3b>(j, i)[0];	//Blue
					dst->at<cv::Vec3b>(row, col)[1] = fgRoi.at<cv::Vec3b>(j, i)[1];	//Green
					dst->at<cv::Vec3b>(row, col)[2] = fgRoi.at<cv::Vec3b>(j, i)[2];	//Red
				}catch (cv::Exception& e) {
					const char* err_msg = e.what();
				}
			}
			// Replace masked areas of the background image with MASK_COLOR.
			else {
				setSurroundingPixels(dst, row, col, 1, MASK_COLOR);
			}
		}
	}

}

/**
  * @brief Applies a perspective transformation to an image.
  * @param[in] image - input image array.
  * @param[out] dst - output image array.
  * @param[om] perspectiveMatrix - transformation matrix.
  */
void ProjectionAreaDetector::warpImage(cv::Mat image, cv::Mat* dst, cv::Mat perspectiveMatrix) {
	cv::warpPerspective(image, image, perspectiveMatrix, cv::Size(image.cols, image.rows), cv::INTER_LINEAR);
	cv::resize(image, *dst, cv::Size(DISPLAY_SIZE_WIDTH, DISPLAY_SIZE_HEIGHT));
}

/**
  * @brief Set threshold of the binarization.
  * @param[in] threshold - Binarize threshold.
  */
void ProjectionAreaDetector::setBinarizeThreshold(int threshold) {
	m_binThresh = threshold;
}

/**
  * @brief Set thick size.
  * @param[in] size - thick size.
  */
void ProjectionAreaDetector::setThickSize(int size) {
	m_thick = size;
}