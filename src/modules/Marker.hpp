#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/aruco/charuco.hpp>

class Marker {
public:
	Marker();
	Marker::Marker(int squaresX, int squaresY, float squareLength, float markerLength, cv::aruco::PREDEFINED_DICTIONARY_NAME dictionaryName);
	~Marker();

	void setCameraParams(const cv::Mat cameraMatrix, const cv::Mat distCoeffs);
	cv::Mat getMarkerImage();
	bool getPoseEstimation(cv::Mat* inputImage, cv::Vec3d* rvec, cv::Vec3d* tvec);
	void getImageCorners(const cv::InputArray rvec, const cv::InputArray tvec, std::vector< cv::Point2f >* imagePoints);
	void drawImageFrame(cv::Mat* image, const std::vector< cv::Point2f > imagePoints);

private:
	void createCharucoBoard(cv::Mat* boardImage);
	void detectCharucoBoard(const cv::Mat inputImage, std::vector<std::vector<cv::Point2f>>* markerCorners, std::vector<int>* markerIds, std::vector<std::vector<cv::Point2f>>* rejectedMarkers);
	bool estimateBoardPose(const cv::Mat inputImage, const std::vector<std::vector<cv::Point2f>> markerCorners, const std::vector<int> markerIds,
		const std::vector<std::vector<cv::Point2f>> rejectedMarkers, const cv::Mat camMatrix, const cv::Mat distCoeffs, cv::Vec3d* rvec, cv::Vec3d* tvec);

private:
	const cv::aruco::PREDEFINED_DICTIONARY_NAME m_dictionaryName;	// 	dictionary name of markers indicating the type of markers
	cv::Ptr<cv::aruco::Dictionary> m_dictionary;					// dictionary of markers indicating the type of markers
	cv::Ptr<cv::aruco::CharucoBoard> m_board;						// charuco board object

	const int m_squaresX;				// number of chessboard squares in X direction
	const int m_squaresY;				// number of chessboard squares in Y direction
	const float m_squareLength;			// chessboard square side length (normally in meters)
	const float m_markerLength;			// marker side length (same unit than squareLength)
	cv::Mat m_markerImage;				// marker
	std::vector< cv::Point2f > m_markerPoints;	//corner points of marker image

	cv::Mat m_camMatrix;				// camera matrix
	cv::Mat m_distCoeffs;				// vector of distortion coefficients
};