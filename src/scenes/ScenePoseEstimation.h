#pragma once
#include "ofxSceneConcrete.h"

#include "../modules/Marker.hpp"

class ScenePoseEstimation : public ofxSceneConcrete
{
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	string getName();
private:
	DepthSensor* m_pDepth;			// pointer of depth sensor object
	Marker m_marker;				// marker object

	ofImage m_markerImage;			// marker image 
	cv::Mat m_perspectiveMatrix;	// perspetive matrix

};

