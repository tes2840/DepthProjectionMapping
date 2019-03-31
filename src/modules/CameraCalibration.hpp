#pragma once
/**
 * @file CameraCalibration.h
 * @brief camera calibration class
 * @date 2018/3/9
 */
#include <opencv2/opencv.hpp>   // Include OpenCV API
#include <opencv2/aruco/charuco.hpp>   // Include OpenCV API

class CameraCalibration
{
public:
	CameraCalibration();
	CameraCalibration(int _markersX, int _markersY, float _markerLength, float _markerSeparation, cv::aruco::PREDEFINED_DICTIONARY_NAME _dictionary_name);
	~CameraCalibration();

	bool calibrateCamera(cv::VideoCapture inputVideo, const float aspectRatio, std::string outputFile);

private:
	void createBoard(cv::Mat* boardImage);
	bool saveCameraParams(const std::string &filename, const cv::Size imageSize, const float aspectRatio, const int flags,
		const cv::Mat &cameraMatrix, const cv::Mat &distCoeffs, const double totalAvgErr);

private:
	const int markersX;				// number of markers in X direction
	const int markersY;				// number of markers in Y direction
	const float markerLength;		// marker side length (normally in meters)
	const float markerSeparation;	// separation between two markers (same unit as markerLength)
	const cv::aruco::PREDEFINED_DICTIONARY_NAME dictionary_name;	// dictionary of markers indicating the type of markers
	const std::string outputFile;	// filename of outputted cameraMatrix

	// board
	cv::Ptr<cv::aruco::Dictionary> m_dictionary;
	cv::Ptr<cv::aruco::GridBoard> m_gridboard;
	cv::Ptr<cv::aruco::Board> m_board;

};

