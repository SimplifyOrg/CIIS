#pragma once
#include "ITrainator.h"
#include "Customer.h"
#include <opencv2/core/mat.hpp>
#include "wx/wx.h"
#include <string>
#include <map>
#include <set>

wxDECLARE_EVENT(wxEVT_CAMERA_RECOGNIZE, wxThreadEvent);

class Recognizer : public wxThread {
public:

	void recognize();

	Recognizer(wxEvtHandler* eventSink, std::string trainingDir);

	// Don't Allow default constructor
	Recognizer() = delete;

protected:
	wxEvtHandler* m_eventSink{ nullptr };
	wxThread::ExitCode Entry() override;

private:
	std::shared_ptr<ITrainator> m_recognizerModel;
	std::map<int, std::string> m_names;
	std::string m_trainingDirectory;
	cv::Ptr<cv::CascadeClassifier> m_cascade;

private:
	void loadTrainedData();
	void showVideo();
	std::pair<cv::Mat, std::set<wxString>> processImage(const cv::Mat& inImg);

};

