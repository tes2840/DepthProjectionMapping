#pragma once
#include "../modules/DepthSensor.hpp"

class SharedData
{
public:
	DepthSensor* pDepth;		// pointer of depth sensor object
	cv::Mat camMatrix;			// camera matrix
	cv::Mat distCoeffs;			// marix of distortion coefficients
	cv::Mat perspectiveMatrix;	// perspetive matrix
};

