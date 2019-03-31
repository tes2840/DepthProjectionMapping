#include "DepthSensor.hpp"
#include <iostream>
#include <librealsense2/rs_advanced_mode.hpp>
#include "Header.h"

/**
 *
 */
DepthSensor::DepthSensor() : m_frameWidth(CAMERA_SIZE_WIDTH), m_frameHeight(CAMERA_SIZE_HEIGHT), m_cameraId(0) {
	//Create a configuration for configuring the pipeline with a non default profile
	rs2::config cfg;

	//Add desired streams to configuration
	cfg.enable_stream(RS2_STREAM_INFRARED, m_frameWidth, m_frameHeight, RS2_FORMAT_Y8, 30);
	cfg.enable_stream(RS2_STREAM_DEPTH, m_frameWidth, m_frameHeight, RS2_FORMAT_Z16, 30);
	cfg.enable_stream(RS2_STREAM_COLOR, m_frameWidth, m_frameHeight, RS2_FORMAT_BGR8, 30);

	// Start streaming with default recommended configuration
	rs2::pipeline_profile profile = m_pipe.start(cfg);

	// Set advanced controls
	rs400::advanced_mode advanced_device(profile.get_device());
	std::ifstream t("setting.json");
	std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
	advanced_device.load_json(str);

	// Set coloor scheme
	m_color_map.set_option(RS2_OPTION_COLOR_SCHEME, 8.f); // Pattern

	// Set video caputure interface
	m_video = cv::VideoCapture(cv::VideoCaptureAPIs::CAP_INTELPERC);
	m_video.open(m_cameraId);
	if (!m_video.isOpened()) {
		std::cout << "camera open error" << std::endl;
	}
}

/**
 *
 */
DepthSensor::DepthSensor(int width, int height, int cameraId) : m_frameWidth(width), m_frameHeight(height), m_cameraId(cameraId) {
	//Create a configuration for configuring the pipeline with a non default profile
	rs2::config cfg;
	
	//Add desired streams to configuration
	cfg.enable_stream(RS2_STREAM_INFRARED, m_frameWidth, m_frameHeight, RS2_FORMAT_Y8, 30);
	cfg.enable_stream(RS2_STREAM_DEPTH, m_frameWidth, m_frameHeight, RS2_FORMAT_Z16, 30);
	cfg.enable_stream(RS2_STREAM_COLOR, m_frameWidth, m_frameHeight, RS2_FORMAT_BGR8, 30);

	// Start streaming with default recommended configuration
	m_pipe.start(cfg);

	// Set video caputure interface
	m_video = cv::VideoCapture(cv::VideoCaptureAPIs::CAP_INTELPERC);
	m_video.open(cameraId);
	if (!m_video.isOpened()) {
		std::cout << "camera open error" << std::endl;
	}
}

/**
 *
 */
DepthSensor::~DepthSensor() {
	m_pipe.stop();
}

/**
  * @brief Get video interface
  * @return video interface
  */
cv::VideoCapture DepthSensor::getVideoCapture() {
	return m_video;
}

/**
  * @brief Get image capture width
  * @return image width
  */
int DepthSensor::getDepthImageWidth() {
	return m_frameWidth;
}

/**
  * @brief Get image capture heigth
  * @return image height
  */
int DepthSensor::getDepthImageHeight() {
	return m_frameHeight;
}

/**
  * @brief Get depth image
  * @param[out] depthImage - depth image of aligned with color image
  * @return func result
  */
int DepthSensor::getDepthImage(cv::Mat *depthImage) try {
	rs2::frameset data = m_pipe.wait_for_frames();	// Wait for next set of frames from the camera

	// Create aligner to align depth image with color image
	rs2::align align_to(RS2_STREAM_COLOR);
	rs2::frameset align_frames = align_to.process(data);						// aglin depth image with color image
//	rs2::frame depth = align_frames.get_depth_frame().apply_filter(m_color_map);

	/*
	* 0 - fill_from_left - Use the value from the left neighbor pixel to fill the hole
	* 1 - farest_from_around - Use the value from the neighboring pixel which is furthest away from the sensor
	* 2 - nearest_from_around - -Use the value from the neighboring pixel closest to the sensor
	*/
	rs2::hole_filling_filter  hole_filling(1);
	rs2::frameset depth_frames = hole_filling.process(align_frames);
	rs2::frame depth = depth_frames.get_depth_frame().apply_filter(m_color_map);

	// Create OpenCV matrix of size (w,h) from the colorized depth data
	cv::Mat image(cv::Size(m_frameWidth, m_frameHeight), CV_8UC3, (void*)depth.get_data(), cv::Mat::AUTO_STEP);

	*depthImage = image.clone();

	return EXIT_SUCCESS;
}
catch (const rs2::error & e)
{
	std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
	return EXIT_FAILURE;
}
catch (const std::exception& e)
{
	std::cerr << e.what() << std::endl;
	return EXIT_FAILURE;
}

/**
  * @brief Get color image
  * @param[out] ColorImage - color image
  * @return func result
  */
int DepthSensor::getColorImage(cv::Mat *colorImage) try {
	rs2::frameset data = m_pipe.wait_for_frames(); // Wait for next set of frames from the camera
	rs2::frame colorFrame = data.get_color_frame();

	// Create OpenCV matrix of size (w,h) from the colorized depth data
	cv::Mat image(cv::Size(m_frameWidth, m_frameHeight), CV_8UC3, (void*)colorFrame.get_data(), cv::Mat::AUTO_STEP);
	*colorImage = image.clone();

	return EXIT_SUCCESS;
}
catch (const rs2::error & e)
{
	std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
	return EXIT_FAILURE;
}
catch (const std::exception& e)
{
	std::cerr << e.what() << std::endl;
	return EXIT_FAILURE;
}
