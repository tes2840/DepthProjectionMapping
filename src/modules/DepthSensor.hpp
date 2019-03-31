#pragma once
#include <iostream>

#include <librealsense2/rs.hpp>
#include <opencv2/opencv.hpp>

class DepthSensor
{
public:
	DepthSensor();
	DepthSensor(int width, int height, int cameraId);
	~DepthSensor();

	int getDepthImageWidth();
	int getDepthImageHeight();

	int getDepthImage(cv::Mat *depthImage);
	int getColorImage(cv::Mat *image);
	cv::VideoCapture getVideoCapture();

private:
	const int m_frameWidth;		// Image width
	const int m_frameHeight;	// Image height
	const int m_cameraId;		// The camera id of video interface

	// Declare depth colorizer for pretty visualization of depth data
	rs2::colorizer m_color_map;

	// Declare RealSense pipeline, encapsulating the actual device and sensors
	rs2::pipeline m_pipe;

	// OpenCV video caputre interface
	cv::VideoCapture m_video;
};