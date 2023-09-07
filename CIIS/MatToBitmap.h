#pragma once
#include "wx/wx.h"
#include <opencv2/core/mat.hpp>

class MatToBitmap
{
public:
	static wxBitmap ConvertMatToBitmap(const cv::Mat& matBitmap, long& timeConvert);
private:

	static bool ConvertMatBitmapTowxBitmapMSW(const cv::Mat& matBitmap, wxBitmap& bitmap);

	static bool ConvertMatBitmapTowxBitmap(const cv::Mat& matBitmap, wxBitmap& bitmap);
};

