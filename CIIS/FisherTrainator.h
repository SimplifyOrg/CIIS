#pragma once
#include "ITrainator.h"

class FisherTrainator :
    public ITrainator
{
public:
	FisherTrainator() {}
	virtual ~ FisherTrainator() {}
	
	virtual void train(std::vector<cv::Mat> imgs, std::vector<int> tags);

	virtual void loadData();

	virtual void predict(const cv::Mat& face_resized, int& label, double& confidence);

	virtual cv::Ptr<cv::CascadeClassifier> getCascadeClassifier();

	virtual cv::Ptr<cv::face::BasicFaceRecognizer> getModel();

private:
	cv::Ptr<cv::face::FisherFaceRecognizer> m_model{ nullptr };
	cv::Ptr<cv::CascadeClassifier> m_cascade{ nullptr };
};

