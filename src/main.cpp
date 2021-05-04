#include "ofMain.h"
#include "ofApp.h"

#include "Header.h"

int main()
{
	ofSetupOpenGL(DISPLAY_SIZE_WIDTH+PROJECTOR_SIZE_WIDTH, PROJECTOR_SIZE_HEIGHT, OF_WINDOW);
	ofSetFullscreen(true);
	ofSetWindowShape(DISPLAY_SIZE_WIDTH+PROJECTOR_SIZE_WIDTH, PROJECTOR_SIZE_HEIGHT);
	ofSetWindowPosition(DISPLAY_SIZE_WIDTH, 0);
	//ofSetWindowPosition(0, 0);
	ofRunApp(std::make_shared<ofApp>());
}