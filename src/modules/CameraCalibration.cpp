#include "CameraCalibration.hpp"

/**
 *
 */
CameraCalibration::CameraCalibration() : markersX(3), markersY(2), markerLength(100), markerSeparation(20), dictionary_name(cv::aruco::DICT_4X4_50)
{
	/* create board object */
	m_dictionary = cv::aruco::getPredefinedDictionary(dictionary_name);
	m_gridboard =	cv::aruco::GridBoard::create(markersX, markersY, markerLength, markerSeparation, m_dictionary);
	m_board = m_gridboard.staticCast<cv::aruco::Board>();
}

/**
 *
 */
CameraCalibration::CameraCalibration(int _markersX, int _markersY, float _markerLength, float _markerSeparation, cv::aruco::PREDEFINED_DICTIONARY_NAME _dictionary_name)
	: markersX(_markersX), markersY(_markersY), markerLength(_markerLength), markerSeparation(_markerSeparation), dictionary_name(_dictionary_name)
{
	/* create board object */
	m_dictionary = cv::aruco::getPredefinedDictionary(dictionary_name);
	m_gridboard = cv::aruco::GridBoard::create(markersX, markersY, markerLength, markerSeparation, m_dictionary);
	m_board = m_gridboard.staticCast<cv::aruco::Board>();
}

/**
 *
 */
CameraCalibration::~CameraCalibration()
{
}

/**
  * @brief Create a new board image to calibrate camera
  * @param[out] boardImage - boad image to calibrate camera 
  * @return void
  */
void CameraCalibration::createBoard(cv::Mat* boardImage) 
{
	// draw board image
	const int margins = 40;	// minimum margins (in pixels) of the board in the output image
	cv::Size img_size;
	img_size.width = markersX * (markerLength + markerSeparation) - markerSeparation + 2 * margins;
	img_size.height = markersY * (markerLength + markerSeparation) - markerSeparation + 2 * margins;
	m_gridboard->draw(img_size, *boardImage, margins);
}

/**
  * @brief Calibrate camera
  * @param[in] inputVideo - openCV camera interface 
  * @param[in] aspectRatio - aspect ratio of the camera
  * @param[out] outputFile - filename of the camera params
  * @return calibration result
  */
bool CameraCalibration::calibrateCamera(cv::VideoCapture inputVideo, const float aspectRatio, std::string outputFile )
{
	int calibrationFlags = 0;
	
	int waitTime;

	/* show calibaration pattern */
	cv::Mat calibrationPattern;

	createBoard(&calibrationPattern);
	cv::imshow("Calibration Pattern", calibrationPattern);

	/* collected frames for calibration */
	cv::Ptr<cv::aruco::DetectorParameters> detectorParams = cv::aruco::DetectorParameters::create();
	std::vector< std::vector< std::vector< cv::Point2f > > > allCorners;
	std::vector< std::vector< int > > allIds;
	cv::Size imgSize;

	while (inputVideo.grab()) {
		cv::Mat image, imageCopy;
		inputVideo.retrieve(image);

		std::vector< int > ids;
		std::vector< std::vector< cv::Point2f > > corners, rejected;

		// detect markers
		cv::aruco::detectMarkers(image, m_dictionary, corners, ids, detectorParams, rejected);

		// draw results
		image.copyTo(imageCopy);
		if (ids.size() > 0) cv::aruco::drawDetectedMarkers(imageCopy, corners, ids);
		putText(imageCopy, "Press 'c' to add current frame. 'ESC' to finish and calibrate",
			cv::Point(10, 20), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 0, 0), 2);

		imshow("out", imageCopy);
		char key = (char)cv::waitKey(10);
		if (key == 27) break;
		if (key == 'c' && ids.size() > 0) {
			std::cout << "Frame captured" << std::endl;
			allCorners.push_back(corners);
			allIds.push_back(ids);
			imgSize = image.size();
		}
	}
	cv::destroyWindow("out");

	if (allIds.size() < 1) {
		std::cerr << "Not enough captures for calibration" << std::endl;
		return false;
	}

	/* prepare data for calibration */
	cv::Mat cameraMatrix, distCoeffs;
	std::vector< cv::Mat > rvecs, tvecs;
	double repError;
	std::vector< std::vector< cv::Point2f > > allCornersConcatenated;
	std::vector< int > allIdsConcatenated;
	std::vector< int > markerCounterPerFrame;

	markerCounterPerFrame.reserve(allCorners.size());
	for (unsigned int i = 0; i < allCorners.size(); i++) {
		markerCounterPerFrame.push_back((int)allCorners[i].size());
		for (unsigned int j = 0; j < allCorners[i].size(); j++) {
			allCornersConcatenated.push_back(allCorners[i][j]);
			allIdsConcatenated.push_back(allIds[i][j]);
		}
	}

	/* calibrate camera */
	repError = cv::aruco::calibrateCameraAruco(allCornersConcatenated, allIdsConcatenated,
		markerCounterPerFrame, m_board, imgSize, cameraMatrix,
		distCoeffs, rvecs, tvecs, calibrationFlags);

	/* save camera params */
	bool saveOk = saveCameraParams(outputFile, imgSize, aspectRatio, calibrationFlags, cameraMatrix,
		distCoeffs, repError);

	if (!saveOk) {
		std::cerr << "Cannot save output file" << std::endl;
		return false;
	}

	std::cout << "Rep Error: " << repError << std::endl;
	std::cout << "Calibration saved to " << outputFile << std::endl;

	cv::destroyWindow("Calibration Pattern");
}

/**
  * @brief Calibrate camera
  * @param[in] filename - filename 
  * @param[in] imageSize - 	size of the image used only to initialize the intrinsic camera matrix
  * @param[in] aspectRatio - aspect ration of the camera
  * @param[in] flags - flags Different flags for the calibration process
  * @param[in] cameraMatrix - output 3x3 floating-point camera matrix
  * @param[in] distCoeffs - output vector of distortion coefficients (k1,k2,p1,p2[,k3[,k4,k5,k6],[s1,s2,s3,s4]]) of 4, 5, 8 or 12 elements
  * @return saving camera params result
  */
bool CameraCalibration::saveCameraParams(const std::string &filename, const cv::Size imageSize, const float aspectRatio, const int flags,
	const cv::Mat &cameraMatrix, const cv::Mat &distCoeffs, const double totalAvgErr) {
	cv::FileStorage fs(filename, cv::FileStorage::WRITE);
	if (!fs.isOpened())
		return false;

	time_t tt;
	time(&tt);
	struct tm *t2 = localtime(&tt);
	char buf[1024];
	strftime(buf, sizeof(buf) - 1, "%c", t2);

	fs << "calibration_time" << buf;

	fs << "image_width" << imageSize.width;
	fs << "image_height" << imageSize.height;

	if (flags & cv::CALIB_FIX_ASPECT_RATIO) fs << "aspectRatio" << aspectRatio;

	if (flags != 0) {
		sprintf(buf, "flags: %s%s%s%s",
			flags & cv::CALIB_USE_INTRINSIC_GUESS ? "+use_intrinsic_guess" : "",
			flags & cv::CALIB_FIX_ASPECT_RATIO ? "+fix_aspectRatio" : "",
			flags & cv::CALIB_FIX_PRINCIPAL_POINT ? "+fix_principal_point" : "",
			flags & cv::CALIB_ZERO_TANGENT_DIST ? "+zero_tangent_dist" : "");
	}

	fs << "flags" << flags;

	fs << "camera_matrix" << cameraMatrix;
	fs << "distortion_coefficients" << distCoeffs;

	fs << "avg_reprojection_error" << totalAvgErr;

	return true;
}