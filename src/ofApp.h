#pragma once

#include "ofMain.h"
#include "scenes/libs/ofxStateMachine.h" 
#include "scenes/SharedData.h"

#include "modules/Marker.hpp"
#include "modules/CameraCalibration.hpp"
#include "modules/DepthSensor.hpp"

const std::string CAM_PARAMS_FILE_NAME = "cameraParam.xml";

class ofApp : public ofBaseApp {
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    void keyPressed(int key);
private:
	bool getCameraParam(cv::Mat* camMatrix, cv::Mat* distCoeffs);
	bool readCameraParameters(std::string filename, cv::Mat* camMatrix, cv::Mat* distCoeffs);

private:
	DepthSensor m_depth;							// depth sensor object
	itg::ofxStateMachine<SharedData> m_scenManager;	// managment scens

};
