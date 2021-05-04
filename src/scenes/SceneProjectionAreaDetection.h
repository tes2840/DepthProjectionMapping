#pragma once
#include "ofxSceneConcrete.h"
#include "modules/ProjectionAreaDetector.h"
#include "modules/DesktopCapture.h"

class SceneProjectionAreaDetection : public ofxSceneConcrete
{
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	string getName();

private:
	void updateParamter();

private:
	DepthSensor* m_pDepth;						// pointer of depth sensor object
	ProjectionAreaDetector *m_areaDetector;		// pointer of area detector object
	DesktopCapture m_desktopCapture;			// desktop capture object

	cv::Mat m_currentFrame;						// current frame
	cv::Mat m_backgroundImage;					// background image for composition
	cv::Mat m_mask;								// mask for composite area
	ofImage m_outputImage;						// image for projection

	// state
	bool m_isMaskFiexd = false;					// Is the mask fixed?
	bool m_mask_compositing = false;			// Start composite?

	// parameter
	cv::Mat m_gui;								// gui array for tuning parameter
	int m_binThreshold = 20;					// binarization threshold
	int m_thickSize = 20;						// thickness
};

