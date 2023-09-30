#pragma once
#include <opencv2/core/mat.hpp>
#include <set>

struct CameraFrame
{
	cv::Mat matBitmap;
	long timeGet{ 0 };
	std::set<wxString> customers;
};
