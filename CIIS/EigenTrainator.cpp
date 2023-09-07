#include "EigenTrainator.h"
#include <iostream>
#include <filesystem>

void EigenTrainator::train(std::vector<cv::Mat> imgs, std::vector<int> tags) {
	m_model = cv::face::EigenFaceRecognizer::create();
	m_model->train(imgs, tags);
	m_model->save("eigen_trained_data.xml");

	std::cout << "Training completed successfully" << std::endl;
}

void EigenTrainator::loadData() {
	m_model = cv::face::EigenFaceRecognizer::create();
	std::filesystem::path currPath = std::filesystem::current_path();
	currPath.append("eigen_trained_data.xml");
	if (std::filesystem::exists(currPath) == true) {
		m_model->read("eigen_trained_data.xml");
	}	
}

void EigenTrainator::predict(const cv::Mat& face_resized, int& label, double& confidence) {
	std::filesystem::path currPath = std::filesystem::current_path();
	currPath.append("eigen_trained_data.xml");
	if (std::filesystem::exists(currPath) == true) {
		m_model->predict(face_resized, label, confidence);
	}
	else {
		confidence = 5000;
	}
	
}

cv::Ptr<cv::CascadeClassifier> EigenTrainator::getCascadeClassifier() {
	if (m_cascade == nullptr) {
		m_cascade = new cv::CascadeClassifier("haarcascade_frontalface_default.xml");
	}
	return m_cascade;//cv::CascadeClassifier("haarcascade_frontalface_default.xml");
}

cv::Ptr<cv::face::BasicFaceRecognizer> EigenTrainator::getModel() {
	return static_cast<cv::Ptr<cv::face::BasicFaceRecognizer>>(m_model);
}

