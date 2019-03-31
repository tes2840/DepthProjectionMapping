#include "ScenePoseEstimation.h"

/**
  * @brief initialize variables
  * @return void
  */
void ScenePoseEstimation::setup() {
	m_pDepth = getSharedData().pDepth;
	m_marker.setCameraParams(getSharedData().camMatrix, getSharedData().distCoeffs);
}

/**
  * @brief update scene's parameters
  * @detail estimate pose of the marker in camera image
  * @return void
  */
void ScenePoseEstimation::update() {

	if (ofGetFrameNum() % 5 != 0) {
		// only update every 5 frames.
		return;
	}

	cv::Mat image;
	cv::Vec3d rvec, tvec;
	m_pDepth->getColorImage(&image);

	// estimate pose matrix
	bool validPose = m_marker.getPoseEstimation(&image, &rvec, &tvec);

	if (validPose == true) {
		std::vector< cv::Point2f > imagePoints;
		std::vector< cv::Point2f > dstPoints;

		m_marker.getImageCorners(rvec, tvec, &imagePoints);
		m_marker.drawImageFrame(&image, imagePoints);

		// Get perspective transform matrix
		dstPoints.push_back(cv::Point2f(0								, m_pDepth->getDepthImageHeight()	));
		dstPoints.push_back(cv::Point2f(m_pDepth->getDepthImageWidth()	, m_pDepth->getDepthImageHeight()	));
		dstPoints.push_back(cv::Point2f(0								, 0									));
		dstPoints.push_back(cv::Point2f(m_pDepth->getDepthImageWidth()	, 0									));

		m_perspectiveMatrix = cv::getPerspectiveTransform(imagePoints, dstPoints);
	}

	// draw pose estimation result
	putText(image, "Press 'q' to confirm perspective transform matrix",
		cv::Point(10, 20), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 0, 0), 2);
	cv::imshow("PoseEstimation", image);
}

/**
  * @brief draw marker image
  * @return void
  */
void ScenePoseEstimation::draw() {
	// draw marker image
	cv::Mat marker = m_marker.getMarkerImage();
	convertMatToOfImage(marker, &m_markerImage);

	m_markerImage.draw(0, 0);
}

/**
  * @brief gets called when a key is pressed
  * @param[in] key - the key of pressed
  * @return void
  */
void ScenePoseEstimation::keyPressed(int key) {

	if (key == 'q') {
		m_perspectiveMatrix.copyTo( getSharedData().perspectiveMatrix );
		cv::destroyWindow("PoseEstimation");
		changeState("DepthProjection");
	}
}

/**
  * @brief gets called when scene transition
  * @return this scene's name
  */
string ScenePoseEstimation::getName() {
	return "PoseEstimation";
}
