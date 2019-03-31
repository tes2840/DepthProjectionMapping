#include "Header.h"
#include "SceneDepthProjection.h"

/**
  * @brief initialize variables
  * @return void
  */
void SceneDepthProjection::setup() {
	m_pDepth = getSharedData().pDepth;
}

/**
  * @brief update scene's parameters
  * @detail get depth image and perform perspective transformation of it
  * @return void
  */
void SceneDepthProjection::update() {
	// Applies a perspective transformation to an image
	cv::Mat depthImage, colorImage;

	cv::Mat outputImage;
	cv::Mat perspectiveMatrix = getSharedData().perspectiveMatrix;

	m_pDepth->getDepthImage(&depthImage);
	m_pDepth->getColorImage(&colorImage);

	cv::warpPerspective(depthImage, depthImage, perspectiveMatrix, cv::Size(m_pDepth->getDepthImageWidth(), m_pDepth->getDepthImageHeight()), cv::INTER_LINEAR);
	cv::resize(depthImage, depthImage, cv::Size(DISPLAY_SIZE_WIDTH, DISPLAY_SIZE_HEIGHT) );

	//debug
	//cv::warpPerspective(colorImage, colorImage, perspectiveMatrix, cv::Size(m_pDepth->getDepthImageWidth(), m_pDepth->getDepthImageHeight()), cv::INTER_LINEAR);
	cv::imshow("colorImage", colorImage);

	// convert cv::Mat to ofImage
	convertMatToOfImage(depthImage, &m_outputImage);
}

/**
  * @brief draw depth image
  * @return void
  */
void SceneDepthProjection::draw() {
	m_outputImage.draw(0,0);
}

/**
  * @brief gets called when a key is pressed
  * @param[in] key - the key of pressed
  * @return void
  */
void SceneDepthProjection::keyPressed(int key) {

	if (key == 'q') {
		cv::destroyWindow("DepthProjection");
	}
}

/**
  * @brief gets called when scene transition
  * @return this scene's name
  */
string SceneDepthProjection::getName() {
	return "DepthProjection";
}
