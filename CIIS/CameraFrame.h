#pragma once
#include <opencv2/core/mat.hpp>

struct CameraFrame
{
	cv::Mat matBitmap;
	long timeGet{ 0 };
};
