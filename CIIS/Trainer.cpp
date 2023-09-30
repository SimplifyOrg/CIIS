#include "Trainer.h"
#include <iostream>

#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "FisherTrainator.h"
#include "EigenTrainator.h"
#include "CIISFrame.h"
#include "CameraFrame.h"

#include <chrono>

// A frame was retrieved from WebCam or IP Camera.
wxDEFINE_EVENT(wxEVT_CAMERA_TRAIN, wxThreadEvent);

void Trainer::captureTrainingImages() {
	
	//createCustomerIdentity();
	m_customer->getCustomerIdentification();
	std::filesystem::path path = getImagePath();
	// TODO: capture video from selcted camera
	cv::VideoCapture capture(0, cv::CAP_MSMF);
	/*double fps = capture.get(cv::CAP_PROP_FPS);
	wxLogMessage("FPS: %f", fps);*/
	//SetStatusText("%d", fps);
	cv::Mat frame;
	std::cout << "Press any key to terminate" << std::endl;
	while (!TestDestroy()) {
		if (areEnoughFaces(path) == true) {
			break;
		}
		++m_timer;
		capture.read(frame);
		if (frame.empty() == true) {
			// blank frame
			std::cerr << "ERROR! blank frame grabbed\n";
			return;
		}
		cv::Mat outImg = processImage(frame);
		CameraFrame* camFrame = new CameraFrame;
		wxStopWatch  stopWatch;
		stopWatch.Start();
		camFrame->timeGet = stopWatch.Time();
		camFrame->matBitmap = outImg;
		wxThreadEvent* evt = new wxThreadEvent(wxEVT_CAMERA_TRAIN);
		evt->SetPayload(camFrame);
		try {
			m_eventSink->QueueEvent(evt);
		}
		catch (std::exception ex) {
			std::cerr << "ERROR! Frame is dead\n";
		}
		// In a real code, the duration to sleep would normally
		// be computed based on the camera framerate, time taken
		// to process the image, and system clock tick resolution.
		//wxMilliSleep(1);
	}
	
}

cv::Mat Trainer::processImage(cv::Mat inImg) {
	cv::Mat frame;
	cv::flip(inImg, frame, 1);
	int resizedWidth = 112;
	int resizedHeight = 92;

	/*using std::chrono::high_resolution_clock;
	using std::chrono::duration_cast;
	using std::chrono::duration;
	using std::chrono::milliseconds;*/
	/*auto t1 = high_resolution_clock::now();
	auto t2 = high_resolution_clock::now();
	auto ms_int = duration_cast<milliseconds>(t2 - t1);
	wxLogMessage("ms_int: %d", ms_int.count());*/
		
	cv::Mat gray;
	cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
		
	int width = gray.size().width * 25 / 100;
	int height = gray.size().height * 25 / 100;
		
	cv::Mat grayResized;
		
	cv::resize(gray, grayResized, cv::Size(width, height));
		
		
	std::vector<cv::Rect> faces;
		
	m_cascade->detectMultiScale(
					grayResized,
					faces,
					1.1,
					5,
					cv::CASCADE_SCALE_IMAGE,
					cv::Size(30, 30)
				);
		
	if (faces.size() > 0) {
		std::vector<int> areas;
		for (auto& face : faces) {
			areas.emplace_back(face.width * face.height);
		}
		std::pair<int, int> maxAreaIndex{0,0};
		for (int index = 0; index < areas.size(); ++index) {
			if (areas[index] > maxAreaIndex.first) {
				maxAreaIndex.first = areas[index];
				maxAreaIndex.second = index;
			}
		}
		cv::Rect selectedFace = faces[maxAreaIndex.second];

		int x = selectedFace.x * 4;
		int y = selectedFace.y * 4;
		int w = selectedFace.width * 4;
		int h = selectedFace.height * 4;

		cv::Mat face = gray(cv::Rect(x, y, w, h));

		cv::Mat faceResized;
		cv::resize(face, faceResized, cv::Size(resizedWidth, resizedHeight));

		std::string imageDir = m_trainingPath + "/" + m_customer->getCustomerIdentification();
		int imgNumber = getImageNumber(imageDir);

		if (m_timer % 5 == 0) {
			cv::imwrite(imageDir + "/" + std::to_string(imgNumber) + ".png", faceResized);
			++imgNumber;
			std::cout << "Captured image: " << imgNumber << std::endl;
		}

		// Draw rectangle
		cv::rectangle(frame, cv::Point(x, y), cv::Point(x + w, y + h), (0, 255, 0), 3);

		// Put text
		cv::putText(frame, m_customer->getName().empty() ? m_customer->getCustomerIdentification() : m_customer->getName(), cv::Point(x - 10, y - 10), cv::FONT_HERSHEY_PLAIN, 1, (0, 255, 0));
	}

	return frame;
}

int Trainer::getImageNumber(std::string& path) {
	int img_no = 0;

	std::vector<int> numbers;

	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		std::string filename = entry.path().filename().string();
		if (filename[0] != '.') {
			size_t dotPos = filename.find('.');
			if (dotPos != std::string::npos) {
				int number = std::stoi(filename.substr(0, dotPos));
				numbers.push_back(number);
			}
		}
	}

	if (!numbers.empty()) {
		std::sort(numbers.begin(), numbers.end());
		img_no = numbers.back() + 1;
	}

	return img_no;

}

bool Trainer::areEnoughFaces(const std::filesystem::path& path) {
	int existingFaces = 0;

	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		if (entry.is_regular_file()) {
			existingFaces++;
			if (existingFaces > 99) {
				return true;
			}
		}		
	}

	return existingFaces > 99;
}

void Trainer::trainRecognizer() {
	std::vector<cv::Mat> imgs;
	std::vector<int> tags;
	int index = 0;

	for (const auto& entry : std::filesystem::directory_iterator(m_trainingPath)) {
		if (entry.is_directory()) {
			const std::string subdir = entry.path().string();
			for (const auto& imgFile : std::filesystem::directory_iterator(subdir)) {
				const std::string path = imgFile.path().string();
				int tag = index;
				imgs.push_back(cv::imread(path, cv::IMREAD_GRAYSCALE));
				tags.push_back(tag);
			}
			index++;
		}
	}
	try {
		trainerModel->train(imgs, tags);
	}
	catch (std::exception ex) {
		wxLogError("ERROR: %s", ex.what());
	}
}

std::filesystem::path Trainer::getImagePath() {
	std::filesystem::path p = m_customer->getCustomerIdentification();
	std::filesystem::path currPath = m_trainingPath;
	std::filesystem::path path = currPath.generic_string() + std::string("/") + p.generic_string();
	std::filesystem::create_directories(path);
	return path;
}

void Trainer::train() {
	if (areEnoughFaces(getImagePath()) == true) {
		trainRecognizer();
	}
	/*wxCommandEvent eventCustom(EVT_MY_CUSTOM_COMMAND, ID_Train_Event);
	eventCustom.SetString(name);
	wxPostEvent(this, eventCustom);*/
}

Trainer::Trainer(wxEvtHandler* eventSink, std::shared_ptr<Customer> customer, std::filesystem::path trainingDirectory) {
	trainerModel.reset(new EigenTrainator());
	/*wxASSERT(eventSink);
	wxASSERT(!nameOfPerson.empty());*/
	m_eventSink = eventSink;
	m_customer = customer;
	m_cascade = trainerModel->getCascadeClassifier();
	//Bind(EVT_MY_CUSTOM_COMMAND, &Trainer::OnProcessCustom, this, ID_Train_Event);
	//trainerModel = std::make_shared<FisherTrainator>(new FisherTrainator());
	// Add face directory
	std::filesystem::create_directories(trainingDirectory);
	m_trainingPath = std::move(trainingDirectory.generic_string());
	m_captureCount = 0;
	m_timer = 0;
}

wxThread::ExitCode Trainer::Entry() {
	//train();
	if (m_startTraining == true) {
		train();
	}
	else {
		captureTrainingImages();
	}
	
	return wxThread::ExitCode();
}
