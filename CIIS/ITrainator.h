#pragma once
#include <vector>
#include <opencv2/face/facerec.hpp>

class ITrainator {
public:

	virtual void train(std::vector<cv::Mat> imgs, std::vector<int> tags) = 0;

	virtual void loadData() = 0;

	virtual void predict(const cv::Mat& face_resized, int& label, double& confidence) = 0;

	virtual cv::Ptr<cv::CascadeClassifier> getCascadeClassifier() = 0;

	virtual cv::Ptr<cv::face::BasicFaceRecognizer> getModel() = 0;
};
