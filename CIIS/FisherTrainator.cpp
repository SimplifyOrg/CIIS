#include "FisherTrainator.h"
#include <iostream>
#include <filesystem>
#include <wx/log.h>

void FisherTrainator::train(std::vector<cv::Mat> imgs, std::vector<int> tags) {
	try {
		m_model = cv::face::FisherFaceRecognizer::create();
		m_model->train(imgs, tags);
		m_model->save("fisher_trained_data.xml");
	}
	catch (std::exception& ex) {
		wxLogError("ERROR: %s", ex.what());
	}
	

	//std::cout << "Training completed successfully" << std::endl;
}

void FisherTrainator::loadData() {
	m_model = cv::face::FisherFaceRecognizer::create();
	std::filesystem::path currPath = std::filesystem::current_path();
	currPath.append("fisher_trained_data.xml");
	if (std::filesystem::exists(currPath) == true) {
		m_model->read("fisher_trained_data.xml");
	}
}

void FisherTrainator::predict(const cv::Mat& face_resized, int& label, double& confidence) {
	std::filesystem::path currPath = std::filesystem::current_path();
	currPath.append("fisher_trained_data.xml");
	if (std::filesystem::exists(currPath) == true) {
		m_model->predict(face_resized, label, confidence);
	}
	else {
		confidence = 5000;
	}
}

cv::Ptr<cv::CascadeClassifier> FisherTrainator::getCascadeClassifier() {
	if (m_cascade == nullptr) {
		m_cascade = new cv::CascadeClassifier("haarcascade_frontalface_default.xml");
	}
	return m_cascade;//cv::CascadeClassifier("haarcascade_frontalface_default.xml");
}

cv::Ptr<cv::face::BasicFaceRecognizer> FisherTrainator::getModel() {
	return static_cast<cv::Ptr<cv::face::BasicFaceRecognizer>>(m_model);
}
