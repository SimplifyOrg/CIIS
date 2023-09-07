#include "MatToBitmap.h"

wxBitmap MatToBitmap::ConvertMatToBitmap(const cv::Mat& matBitmap, long& timeConvert)
{
	wxCHECK(!matBitmap.empty(), wxBitmap());

	wxBitmap    bitmap(matBitmap.cols, matBitmap.rows, 24);
	bool        converted = false;
	wxStopWatch stopWatch;
	long        time = 0;

	stopWatch.Start();
	converted = ConvertMatBitmapTowxBitmap(matBitmap, bitmap);
	time = stopWatch.Time();

	if (!converted)
	{
		wxLogError("Could not convert Mat to wxBitmap.");
		return wxBitmap();
	}

	timeConvert = time;
	return bitmap;
}

bool MatToBitmap::ConvertMatBitmapTowxBitmapMSW(const cv::Mat& matBitmap, wxBitmap& bitmap)
{
	const HDC  hScreenDC = ::GetDC(nullptr);
	BITMAPINFO bitmapInfo{ 0 };
	bool       success;

	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFO) - sizeof(RGBQUAD);
	bitmapInfo.bmiHeader.biWidth = bitmap.GetWidth();
	bitmapInfo.bmiHeader.biHeight = 0 - bitmap.GetHeight();
	bitmapInfo.bmiHeader.biPlanes = 1;
	bitmapInfo.bmiHeader.biBitCount = 24;
	bitmapInfo.bmiHeader.biCompression = BI_RGB;

	success = ::SetDIBits(hScreenDC, bitmap.GetHBITMAP(), 0, bitmap.GetHeight(),
		matBitmap.data, &bitmapInfo, DIB_RGB_COLORS) != 0;
	::ReleaseDC(nullptr, hScreenDC);

	return success;
}

bool MatToBitmap::ConvertMatBitmapTowxBitmap(const cv::Mat& matBitmap, wxBitmap& bitmap)
{
	wxCHECK(!matBitmap.empty(), false);
	wxCHECK(matBitmap.type() == CV_8UC3, false);
	wxCHECK(matBitmap.dims == 2, false);
	wxCHECK(bitmap.IsOk(), false);
	wxCHECK(bitmap.GetWidth() == matBitmap.cols && bitmap.GetHeight() == matBitmap.rows, false);
	wxCHECK(bitmap.GetDepth() == 24, false);

	if (bitmap.IsDIB()
		&& matBitmap.isContinuous()
		&& matBitmap.cols % 4 == 0)
	{
		return ConvertMatBitmapTowxBitmapMSW(matBitmap, bitmap);
	}
	return false;
}
