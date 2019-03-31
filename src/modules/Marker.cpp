#include "Header.h"
#include "Marker.hpp"

/**
 *
 */
Marker::Marker() : m_squaresX(9), m_squaresY(5), m_squareLength(0.04), m_markerLength(0.02), m_dictionaryName(cv::aruco::DICT_4X4_50)
{
	// create CharucoBoard
	m_dictionary = cv::aruco::getPredefinedDictionary(m_dictionaryName);
	m_board = cv::aruco::CharucoBoard::create(m_squaresX, m_squaresY, m_squareLength, m_markerLength, m_dictionary);

	createCharucoBoard(&m_markerImage);
}

/**
 *
 */
Marker::Marker( int squaresX, int squaresY, float squareLength, float markerLength, cv::aruco::PREDEFINED_DICTIONARY_NAME dictionaryName) :
	m_squaresX(squaresX), m_squaresY(squaresY), m_squareLength(squareLength), m_markerLength(markerLength), m_dictionaryName(dictionaryName)
{
	// create CharucoBoard
	m_dictionary = cv::aruco::getPredefinedDictionary(m_dictionaryName);
	m_board = cv::aruco::CharucoBoard::create(m_squaresX, m_squaresY, m_squareLength, m_markerLength, m_dictionary);

	createCharucoBoard(&m_markerImage);
}

/**
 *
 */
Marker::~Marker() {}

void Marker::setCameraParams(const cv::Mat cameraMatrix, const cv::Mat distCoeffs) {
	m_camMatrix = cameraMatrix;
	m_distCoeffs = distCoeffs;
}

/**
  * @brief Get marker image
  * @return marker image
  */
cv::Mat Marker::getMarkerImage() {
	return m_markerImage;
}

/**
  * @brief Create a new charuco board image
  * @param[out] boardImage - boad image to calibrate camera
  * @return void
  */
void Marker::createCharucoBoard(cv::Mat* boardImage) {
	// create charuco board
	m_board->draw(cv::Size(DISPLAY_SIZE_WIDTH, DISPLAY_SIZE_HEIGHT), *boardImage, 0, 1);
}

/**
  * @brief detect charuco board from input image
  * @param[in] inputImage - image
  * @param[out] markerCorners - vector of detected marker corners
  * @param[out] markerIds - vector of identifiers of the detected markers
  * @param[out] rejectedMarkers - contains the imgPoints of those squares whose inner code has not a correct codification
  * @return void
  */
void Marker::detectCharucoBoard(const cv::Mat inputImage, std::vector<std::vector<cv::Point2f>>* markerCorners, std::vector<int>* markerIds, std::vector<std::vector<cv::Point2f>>* rejectedMarkers) {
	cv::Ptr<cv::aruco::DetectorParameters> parameters = cv::aruco::DetectorParameters::create();
	cv::Ptr<cv::aruco::Board> board = m_board.staticCast<cv::aruco::Board>();

	// detect marker
	cv::aruco::detectMarkers(inputImage, m_dictionary, *markerCorners, *markerIds, parameters, *rejectedMarkers);
	
	// refind strategy to detect more markers
	cv::aruco::refineDetectedMarkers(inputImage, board, *markerCorners, *markerIds, *rejectedMarkers, m_camMatrix, m_distCoeffs);
}

/**
  * @brief estimate pose for a charuco board given some of their corners
  * @param[in] inputImage - image
  * @param[in] markerCorners - vector of detected marker corners
  * @param[in] markerIds - vector of identifiers of the detected markers
  * @param[in] rejectedMarkers - contains the imgPoints of those squares whose inner code has not a correct codification
  * @param[in] cameraMatrix - output 3x3 floating-point camera matrix
  * @param[in] distCoeffs -  output vector of distortion coefficients (k1,k2,p1,p2[,k3[,k4,k5,k6],[s1,s2,s3,s4]]) of 4, 5, 8 or 12 elements
  * @param[out] rvec - output vector corresponding to the rotation vector of the board
  * @param[out] tvec - Output vector corresponding to the translation vector of the board
  * @return pose estimation result
  */
bool Marker::estimateBoardPose(const cv::Mat inputImage, const std::vector<std::vector<cv::Point2f>> markerCorners, const std::vector<int> markerIds, const std::vector<std::vector<cv::Point2f>> rejectedMarkers,
	const cv::Mat camMatrix, const cv::Mat distCoeffs, cv::Vec3d* rvec, cv::Vec3d* tvec ) {
	
	bool validPose = false;

	// interpolate charuco corners
	std::vector< cv::Point2f > charucoCorners;
	std::vector< int > charucoIds;
	int interpolatedCorners = 0;
	if (markerIds.size() > 0) {
		interpolatedCorners = cv::aruco::interpolateCornersCharuco(markerCorners, markerIds, inputImage, m_board, charucoCorners, charucoIds, camMatrix, distCoeffs);
	}

	// estimate charuco board pose
	if (camMatrix.total() != 0) {
		validPose = cv::aruco::estimatePoseCharucoBoard(charucoCorners, charucoIds, m_board, camMatrix, distCoeffs, *rvec, *tvec);
	}

	return validPose;
}

/**
  * @brief get pose estimation for a charuco board 
  * @param[in/out] inputImage - the marker borders are painted if provided
  * @param[out] rvec - output vector corresponding to the rotation vector of the board
  * @param[out] tvec - Output vector corresponding to the translation vector of the board
  * @return pose estimation result
  */
bool Marker::getPoseEstimation(cv::Mat* inputImage, cv::Vec3d* rvec, cv::Vec3d* tvec)
{
	bool validPose = false;

	std::vector<int> markerIds;
	std::vector<std::vector<cv::Point2f>> markerCorners, rejectedMarkers;

	// detect charuco board
	detectCharucoBoard(*inputImage, &markerCorners, &markerIds, &rejectedMarkers);
		
	// estimate board pose
	validPose = estimateBoardPose(*inputImage, markerCorners, markerIds, rejectedMarkers, m_camMatrix, m_distCoeffs, rvec, tvec);

	// draw detected markers
	cv::aruco::drawDetectedMarkers(*inputImage, markerCorners, markerIds);

	return validPose;
}


/**
  * @brief get corners for a charuco board in image
  * @param[in] rvec - the rotation vector of the board
  * @param[in] tvec - the translation vector of the board
  * @param[in] imagePoints - corners of the board in image
  * @return void
  */
void Marker::getImageCorners(const cv::InputArray rvec, const cv::InputArray tvec, std::vector< cv::Point2f >* imagePoints){

	// project points
	std::vector< cv::Point3f > projectionPoints;
	projectionPoints.push_back(cv::Point3f(0							, 0								, 0));
	projectionPoints.push_back(cv::Point3f(m_squaresX * m_squareLength	, 0								, 0));
	projectionPoints.push_back(cv::Point3f(0							, m_squaresY * m_squareLength	, 0));
	projectionPoints.push_back(cv::Point3f(m_squaresX * m_squareLength	, m_squaresY * m_squareLength	, 0));

	projectPoints(projectionPoints, rvec, tvec, m_camMatrix, m_distCoeffs, *imagePoints);
}

/**
  * @brief draw frame of the board
  * @param[in/out] image - camera image
  * @param[in] imagePoints - corners of the board in image
  * @return void
  */
void Marker::drawImageFrame(cv::Mat* image, const std::vector< cv::Point2f > imagePoints) {

	// draw lines
	line(*image, imagePoints[0], imagePoints[1], cv::Scalar(0, 0, 255), 3);
	line(*image, imagePoints[0], imagePoints[2], cv::Scalar(0, 255, 0), 3);
	line(*image, imagePoints[1], imagePoints[3], cv::Scalar(255, 0, 0), 3);
	line(*image, imagePoints[2], imagePoints[3], cv::Scalar(0, 128, 0), 3);
}