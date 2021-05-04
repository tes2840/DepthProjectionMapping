#pragma once

#include <opencv2/opencv.hpp>
#include <Windows.h>

class DesktopCapture
{
public:
	DesktopCapture();
	~DesktopCapture();
	void mouseCallback(int event, int x, int y, int flags, void *userdata);
	void getDesktopImage(cv::Mat *dst);

private:
	// Capture area setting
	bool m_caputureAreaFixed = false;	// Is the capture area fixed?
	cv::Rect m_captureArea;				// Capture area
	cv::Point m_startPoint;				// Start point of capture area
	cv::Point m_endPoint;				// End point of capture area

	// Desktop capture variables depends on Widows 
	BITMAPINFO m_bitmapInfo;
	HWND m_desktop;
	HDC m_hDC;
	HBITMAP m_hBitmap;
	HDC m_hMemDC;
	int m_desktopWidth;					// Width of the desktop
	int m_desktopHeight;				// Height of the desktop
};

