#include "FisherTrainator.h"
#include <iostream>

void FisherTrainator::train(std::vector<cv::Mat> imgs, std::vector<int> tags) {
	m_model = cv::face::FisherFaceRecognizer::create();
	m_model->train(imgs, tags);
	m_model->save("fisher_trained_data.xml");

	std::cout << "Training completed successfully" << std::endl;
}

void FisherTrainator::loadData() {
	m_model = cv::face::FisherFaceRecognizer::create();
	m_model->read("fisher_trained_data.xml");
}

void FisherTrainator::predict(const cv::Mat& face_resized, int& label, double& confidence) {
	m_model->predict(face_resized, label, confidence);
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
