#include "SceneProjectionAreaDetection.h"
#define CVUI_IMPLEMENTATION
#include "modules/cvui.h"

#define GUI_NAME "GUI"
#define GUI_WIDGET_COL_1_POS_X 20		// x-position of column 1 of gui
#define GUI_WIDGET_COL_2_POS_X 150		// x-position of column 2 of gui
#define GUI_OFFSET_POS_Y 50				// y-position offset between widgets
/**
  * @brief Callback function for mouse events.
  * @param[in] event - 	One of the cv::MouseEventTypes constants.
  * @param[in] x - 	The x-coordinate of the mouse event.
  * @param[in] y - The y-coordinate of the mouse event.
  * @param[in] flags - 	One of the cv::MouseEventFlags constants.
  * @param[in] *param - The optional parameter.
  */
static void mouse_Callback(int event, int x, int y, int flag, void* param)
{
	// Cast as a pointer to the ProjectionAreaDetector
	ProjectionAreaDetector* areaDetector = static_cast<ProjectionAreaDetector*>(param);
	// Call mouseCallback
	areaDetector->mouseCallback(event, x, y, flag, nullptr);
}

/**
  * @brief initialize variables
  * @return void
  */
void SceneProjectionAreaDetection::setup() {
	// init module
	m_pDepth = getSharedData().pDepth;
	m_areaDetector = new ProjectionAreaDetector(m_binThreshold, m_thickSize);

	// init gui
	m_gui = cv::Mat(300, 400, CV_8UC3);
	cvui::init(GUI_NAME);

	cv::namedWindow("ProjectionAreaDetection", cv::WINDOW_NORMAL);
	cv::resizeWindow("ProjectionAreaDetection", 1280, 800);
}

/**
  * @brief update scene's parameters
  * @return void
  */
void SceneProjectionAreaDetection::update() {
	static bool setCallbackflg = false;

	if (ofGetFrameNum() % 5 != 0) {
		// only update every 5 frames.
		return;
	}
	// update tuning parameter's
	updateParamter();

	// Get camera image
	cv::Mat image;
	m_pDepth->getColorImage(&image);
	m_areaDetector->warpImage(image, &m_currentFrame, getSharedData().perspectiveMatrix);
	image = m_currentFrame.clone();

	// Get desktop capture image
	cv::Mat frame;
	m_desktopCapture.getDesktopImage(&frame);

	if (!m_backgroundImage.empty()) {	// if background image isn't empty, get diff image from current from to it

		if (m_isMaskFiexd == false) {
			m_areaDetector->getMask(m_backgroundImage, m_currentFrame);
		}
		else {
			// Register a callback function for mask image modification
			if (setCallbackflg == false) {
				cv::setMouseCallback("ProjectionAreaDetection", mouse_Callback, m_areaDetector);
				setCallbackflg = true;
			}
		}
		// Apply mask to camera frame
		m_areaDetector->applyMask(&image);

		// Start composite
		if (m_mask_compositing == true) {
			// Get the image to be combined
			cv::Mat frame;
			m_desktopCapture.getDesktopImage(&frame);

			// composite
			cv::Mat result;
			m_areaDetector->combineImage(frame, m_currentFrame, &result);

			// convert cv::Mat to ofImage
			convertMatToOfImage(result, &m_outputImage);
		}
	}

	cv::imshow("ProjectionAreaDetection", image);
}

/**
  * @brief update parameters
  * @return void
  */
void SceneProjectionAreaDetection::updateParamter() {
	// Render UI background
	m_gui = cv::Scalar(49, 52, 49);

	// Render UI components to the frame
	int pos_y = 20;
	cvui::text(m_gui, GUI_WIDGET_COL_1_POS_X, pos_y, "Binarize threshold");
	cvui::trackbar(m_gui, GUI_WIDGET_COL_2_POS_X, pos_y - 5, 200, &m_binThreshold, 1, 150);

	pos_y += GUI_OFFSET_POS_Y;
	cvui::text(m_gui, GUI_WIDGET_COL_1_POS_X, pos_y, "Thick size");
	cvui::trackbar(m_gui, GUI_WIDGET_COL_2_POS_X, pos_y - 5, 200, &m_thickSize, 1, 100);

	pos_y += GUI_OFFSET_POS_Y;
	cvui::text(m_gui, GUI_WIDGET_COL_1_POS_X, pos_y, "Background image");
	if (cvui::button(m_gui, GUI_WIDGET_COL_2_POS_X, pos_y - 5, "Set")) {
		m_backgroundImage = m_currentFrame.clone();
	}

	pos_y += GUI_OFFSET_POS_Y;
	cvui::text(m_gui, GUI_WIDGET_COL_1_POS_X, pos_y, "Mask");
	if (cvui::button(m_gui, GUI_WIDGET_COL_2_POS_X, pos_y - 5, "Set")) {
		m_backgroundImage = m_isMaskFiexd = true;
	}

	pos_y += GUI_OFFSET_POS_Y;
	cvui::text(m_gui, GUI_WIDGET_COL_1_POS_X, pos_y, "Composition");
	if (cvui::button(m_gui, GUI_WIDGET_COL_2_POS_X, pos_y - 5, "Start")) {
		m_backgroundImage = m_mask_compositing = true;
	}
	cvui::update();

	// Update cvui stuff and show everything on the screen
	cvui::imshow(GUI_NAME, m_gui);

	// update parameter
	m_areaDetector->setBinarizeThreshold(m_binThreshold);
	m_areaDetector->setThickSize(m_thickSize);
}

/**
  * @brief draw marker image
  * @return void
  */
void SceneProjectionAreaDetection::draw() {
	m_outputImage.draw(0, 0);
}

/**
  * @brief gets called when a key is pressed
  * @param[in] key - the key of pressed
  * @return void
  */
void SceneProjectionAreaDetection::keyPressed(int key) {

	if (key == 'q') {
		cv::destroyWindow("ProjectionAreaDetection");
	}
	else {
		// It doesn't do anything when other keys are pressed.
	}
}

/**
  * @brief gets called when scene transition
  * @return this scene's name
  */
string SceneProjectionAreaDetection::getName() {
	return "ProjectionAreaDetection";
}

