#pragma once
#include <opencv2/core/mat.hpp>
#include <map>

struct CameraFrame
{
	cv::Mat matBitmap;
	wxBitmap bitmap;
	bool isBitmap;
	long timeGet{ 0 };
	std::map<wxString, Customer*> customers;
};
