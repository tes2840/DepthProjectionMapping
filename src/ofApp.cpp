#include "ofApp.h"
#include "scenes/ScenePoseEstimation.h"
#include "scenes/SceneDepthProjection.h"
#include "scenes/SceneProjectionAreaDetection.h"

/**
 * @brief initialize variables
 * @return void
 */
void ofApp::setup() {
	// set background color
	ofSetBackgroundColor(0, 0, 0);

	cv::Mat camMatrix, distCoeffs;
	getCameraParam(&camMatrix, &distCoeffs);

	// init shared data
	m_scenManager.getSharedData().pDepth = &m_depth;
	camMatrix.copyTo(m_scenManager.getSharedData().camMatrix);
	distCoeffs.copyTo(m_scenManager.getSharedData().distCoeffs );

	// add the scenes
	m_scenManager.addState<ScenePoseEstimation>();
	m_scenManager.addState<SceneDepthProjection>();
	m_scenManager.addState<SceneProjectionAreaDetection>();
	
	// change the scene
	m_scenManager.changeState("PoseEstimation");
	//m_scenManager.changeState("ProjectionAreaDetection"); 
}

/**
  *
  */
void ofApp::update(){
}

/**
  *
  */
void ofApp::draw() {
}

void ofApp::exit() {
}

/**
  *
  */
void ofApp::keyPressed(int key) {
}

/**
  * @brief get camera parameters and calibrate camera if there are no camera parameters
  * @param[out] camMatrix - camera matrix
  * @param[out] distCoeffs - matrix of distortion coefficients
  * @return result
  */
bool ofApp::getCameraParam(cv::Mat* camMatrix, cv::Mat* distCoeffs) {

	// read camera params
	bool isRead = readCameraParameters(CAM_PARAMS_FILE_NAME, camMatrix, distCoeffs);
	if (isRead == false) {
		std::cout << "Camera Matrix read error. " << std::endl;
		std::cout << "Start camera calibration. " << std::endl;

		cv::VideoCapture video = m_depth.getVideoCapture();
		CameraCalibration calib;
		int cameraId = 0;
		int cameraAspectRatio = 1;

		// calibrate camera
		calib.calibrateCamera(video, cameraAspectRatio, CAM_PARAMS_FILE_NAME);
		isRead = readCameraParameters(CAM_PARAMS_FILE_NAME, camMatrix, distCoeffs);

		if (isRead == false) {
			std::cout << "Cannot read Camera Matrix. " << std::endl;
		}
	}

	return isRead;
}

/**
  * @brief read camera parameters from the file
  * @param[out] filename - the file name of saved camera parameters
  * @param[out] camMatrix - camera matrix
  * @param[out] distCoeffs - matrix of distortion coefficients
  * @return result
  */
bool ofApp::readCameraParameters(std::string filename, cv::Mat* camMatrix, cv::Mat* distCoeffs) {
	cv::FileStorage fs(filename, cv::FileStorage::READ);
	if (!fs.isOpened())
		return false;
	fs["camera_matrix"] >> *camMatrix;
	fs["distortion_coefficients"] >> *distCoeffs;
	return true;
}