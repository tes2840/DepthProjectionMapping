#include "DesktopCapture.h"

/**
  * @brief Callback function for mouse events.
  * @param[in] event - 	One of the cv::MouseEventTypes constants.
  * @param[in] x - 	The x-coordinate of the mouse event.
  * @param[in] y - The y-coordinate of the mouse event.
  * @param[in] flags - 	One of the cv::MouseEventFlags constants.
  * @param[in] *param - The optional parameter.
  */
static void caputure_mouse_Callback(int event, int x, int y, int flag, void* param)
{
	// Cast as a pointer to the ProjectionAreaDetector
	DesktopCapture* desktopCap = static_cast<DesktopCapture*>(param);
	// call mouseCallback
	desktopCap->mouseCallback(event, x, y, flag, nullptr);
}

/**
  * @brief DesktopCapture class constructor.
  */
DesktopCapture::DesktopCapture() {
	// Size of the desktop
	m_desktop = GetDesktopWindow();
	RECT rect;
	GetWindowRect(m_desktop, &rect);
	m_desktopWidth = rect.right;
	m_desktopHeight = rect.bottom;

	// Set the DIB information
	m_bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_bitmapInfo.bmiHeader.biWidth = m_desktopWidth;
	m_bitmapInfo.bmiHeader.biHeight = -m_desktopHeight;
	m_bitmapInfo.bmiHeader.biPlanes = 1;
	m_bitmapInfo.bmiHeader.biBitCount = 24;
	m_bitmapInfo.bmiHeader.biCompression = BI_RGB;

	// Create DIBSection
	LPDWORD lpPixel;
	m_hDC = GetDC(m_desktop);
	m_hBitmap = CreateDIBSection(m_hDC, &m_bitmapInfo, DIB_RGB_COLORS, (void**)&lpPixel, NULL, 0);
	m_hMemDC = CreateCompatibleDC(m_hDC);
	SelectObject(m_hMemDC, m_hBitmap);

	// Init capture area
	m_startPoint = cv::Point(0, 0);
	m_endPoint = cv::Point(m_desktopWidth, m_desktopHeight);
	m_captureArea = cv::Rect(m_startPoint, m_endPoint);
}

/**
  * @brief DesktopCapture class destructor.
  */
DesktopCapture::~DesktopCapture() {
	// release
	ReleaseDC(m_desktop, m_hDC);
	DeleteDC(m_hMemDC);
	DeleteObject(m_hBitmap);
}

/**
  * @brief Callback function for mouse events.
  * @param[in] event - 	One of the cv::MouseEventTypes constants.
  * @param[in] x - 	The x-coordinate of the mouse event.
  * @param[in] y - The y-coordinate of the mouse event.
  * @param[in] flags - 	One of the cv::MouseEventFlags constants.
  * @param[in] *userdata - The optional parameter.
  */
void DesktopCapture::mouseCallback(int event, int x, int y, int flags, void *userdata) {
	if (event == cv::EVENT_LBUTTONDOWN) {
		m_startPoint = cv::Point(x, y);
	}
	else if (event == cv::EVENT_LBUTTONUP) {
		m_endPoint = cv::Point(x, y);
		m_captureArea = cv::Rect(m_startPoint, m_endPoint);	// set the capture area
	}
	else if (event == cv::EVENT_RBUTTONDOWN) {
	}
}

void DesktopCapture::getDesktopImage(cv::Mat *dst) {
	static bool initFlag = false;
	
	// Create a capture image
	cv::Mat screenImage;
	screenImage.create(m_desktopHeight, m_desktopWidth, CV_8UC3);

	// Copy from hwindowCompatibleDC to cv::Mat
	BitBlt(m_hMemDC, 0, 0, m_desktopWidth, m_desktopHeight, m_hDC, 0, 0, SRCCOPY);
	GetDIBits(m_hMemDC, m_hBitmap, 0, m_desktopHeight, screenImage.data, (BITMAPINFO *)&m_bitmapInfo, DIB_RGB_COLORS);  //copy from hwindowCompatibleDC to hbwindow

	cv::Mat tempImage = screenImage.clone();
	*dst = tempImage(cv::Range(m_captureArea.y, m_captureArea.y + m_captureArea.height), cv::Range(m_captureArea.x, m_captureArea.x + m_captureArea.width));

	// Draw the capture area
	cv::rectangle(screenImage, m_captureArea, cv::Scalar(0, 0, 255), 2);

	// Show screen image
	cv::imshow("screen", screenImage);
	cv::resizeWindow("screen", 640, 480);
	if (initFlag == false) {
		cv::setMouseCallback("screen", caputure_mouse_Callback, this);
		initFlag = true;
	}
}