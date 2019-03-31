#pragma once
#include "ofxSceneConcrete.h"
class SceneDepthProjection : public ofxSceneConcrete
{
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	string getName();
private:
	DepthSensor* m_pDepth;	// pointer of depth sensor object
	cv::Mat m_colorImage;	// color image
	ofImage m_outputImage;	// image to project
};

