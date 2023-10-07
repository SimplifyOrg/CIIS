#pragma once
#include "ITrainator.h"
#include <opencv2/core/mat.hpp>
#include <opencv2/videoio.hpp>
#include <filesystem>
#include "wx/wx.h"
#include "Customer.h"

wxDECLARE_EVENT(wxEVT_CAMERA_TRAIN, wxThreadEvent);

class Trainer : public wxThread {
public:	
	void train();

	void captureTrainingImages();

	void createCustomerIdentity();

	std::shared_ptr<Customer> getCustomer() { return m_customer; }

	void setTrainingFlag(bool flag) { m_startTraining = flag; }

	Trainer(wxEvtHandler* eventSink, cv::VideoCapture* videoCapture, std::shared_ptr<Customer> customer, std::filesystem::path trainingDirectory);

	// Don't Allow default constructor
	Trainer() = delete;

protected:
	wxEvtHandler* m_eventSink{ nullptr };
	//std::string m_person; // Convert to CustomerIDentity Object later
	wxThread::ExitCode Entry() override;

private:
	std::string m_trainingPath;
	int m_captureCount;
	int m_timer;
	std::shared_ptr<ITrainator> trainerModel;
	std::shared_ptr<Customer> m_customer;
	cv::Ptr<cv::CascadeClassifier> m_cascade;
	bool m_startTraining{ false };
	cv::VideoCapture* m_videoCapture{ nullptr };

private:
	cv::Mat processImage(cv::Mat inImg);

	int getImageNumber(std::string& path);

	bool areEnoughFaces(const std::filesystem::path& path);

	void trainRecognizer();

	std::filesystem::path getImagePath();

	//wxDECLARE_EVENT_TABLE();

};

