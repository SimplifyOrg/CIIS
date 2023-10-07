#include "Recognizer.h"
#include <iostream>
#include <filesystem>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "FisherTrainator.h"
#include "EigenTrainator.h"
#include "CameraFrame.h"
#include "CustomerDatabase.h"

// A frame was retrieved from WebCam or IP Camera.
wxDEFINE_EVENT(wxEVT_CAMERA_RECOGNIZE, wxThreadEvent);

void Recognizer::recognize() {
    loadTrainedData();
    //std::cout << "Press any key to quit" << std::endl;
    showVideo();
}

wxThread::ExitCode Recognizer::Entry() {
    recognize();
    return wxThread::ExitCode();
}

Recognizer::Recognizer(wxEvtHandler* eventSink, cv::VideoCapture* videoCapture, std::string trainingDir) {
    m_recognizerModel.reset(new FisherTrainator());
    m_cascade = m_recognizerModel->getCascadeClassifier();
    m_eventSink = eventSink;
    m_videoCapture = videoCapture;
    std::filesystem::path currPath = std::filesystem::current_path();
    //std::string path = currPath.generic_string() + "\\" +p.generic_string();
    std::filesystem::path path = currPath.generic_string() + std::string("/") + trainingDir;
    m_trainingDirectory.assign(path.generic_string());
    //recognizerModel = std::make_shared<FisherTrainator>(new FisherTrainator());
}

void Recognizer::loadTrainedData() {
    int key = 0;
    for (const auto& entry : std::filesystem::directory_iterator(m_trainingDirectory)) {
        if (entry.is_directory()) {
            m_names[key] = entry.path().filename().string();
            key++;
        }
    }
    m_recognizerModel->loadData();
}

void Recognizer::showVideo() {
    //cv::VideoCapture video_capture(m_camera);
    if (m_videoCapture == nullptr || false == m_videoCapture->isOpened()) {
        std::cerr << "Error opening video capture." << std::endl;
        return;
    }

    cv::Mat frame;
    while (!TestDestroy()) {
        m_videoCapture->read(frame);
        cv::Mat inImg = frame.clone();
        std::pair<cv::Mat, std::map<wxString, Customer*>> resp = processImage(inImg);
        CameraFrame* camFrame = new CameraFrame;
        wxStopWatch  stopWatch;
        stopWatch.Start();
        camFrame->timeGet = stopWatch.Time();
        camFrame->matBitmap = resp.first;
        camFrame->customers = resp.second;
        camFrame->isBitmap = false;

        // TODO: Check if the frame is destroyed
        // Do not add event if frame is dead
        //if (m_eventSink->IsUnlinked() == false) {
        wxThreadEvent* evt = new wxThreadEvent(wxEVT_CAMERA_RECOGNIZE);
        evt->SetPayload(camFrame);
        try {
            m_eventSink->QueueEvent(evt);
        }
        catch (std::exception ex) {
            std::cerr << "ERROR! Frame is dead\n";
        }
        /*}
        else {
            break;
        }*/
        
        //cv::imshow("Video", resp.first);

        //// When everything is done, release the capture on pressing any key
        //if (cv::waitKey(1)) {
        //    m_videoCapture->release();
        //    cv::destroyAllWindows();
        //    return;
        //}
    }
}

std::pair<cv::Mat, std::map<wxString, Customer*>> Recognizer::processImage(const cv::Mat& inImg) {
    cv::Mat frame;
    cv::flip(inImg, frame, 1);
    int resized_width = 112;
    int resized_height = 92;

    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

    int width = static_cast<int>(gray.cols * 0.25);
    int height = static_cast<int>(gray.rows * 0.25);
    cv::Size dsize(width, height);
    cv::Mat gray_resized;
    cv::resize(gray, gray_resized, dsize);

    std::vector<cv::Rect> faces;
    // Use a cascade classifier to detect faces in gray_resized
    m_cascade->detectMultiScale(gray_resized, faces, 1.1, 5, cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));
    std::map<wxString, Customer*> persons;
    for (size_t i = 0; i < faces.size(); ++i) {
        cv::Rect face_i = faces[i];
        
        int x = static_cast<int>(face_i.x * 4);
        int y = static_cast<int>(face_i.y * 4);
        int w = static_cast<int>(face_i.width * 4);
        int h = static_cast<int>(face_i.height * 4);

        // Round off to nearest 10
        /*int lenx = log10(x); int leny = log10(y); int lenw = log10(w); int lenh = log10(h);
        int divx = pow(10, lenx); int divy = pow(10, leny); int divw = pow(10, lenw); int divh = pow(10, lenh);
        int rect_x = ceil(x / divx)* divx; int rect_y = ceil(y / divy) * divy; int rect_w = ceil(w / divw) * divw; int rect_h = ceil(h / divh) * divh;*/
        cv::Mat face = gray(cv::Rect(x, y, w, h));
        cv::Mat face_resized;
        cv::resize(face, face_resized, cv::Size(resized_width, resized_height));

        int label = -1;
        double confidence = 0.0;
        m_recognizerModel->predict(face_resized, label, confidence);

        if (confidence < 3500) {
            std::string person = m_names[label];
            std::shared_ptr<Customer> customer = std::make_shared<Customer>(*(CustomerDatabase::GetCustomerWithID(person)));
            
            persons[customer->getCustomerIdentification()] = (CustomerDatabase::GetCustomerWithID(person));
            cv::rectangle(frame, cv::Point(x, y), cv::Point(x + w, y + h), cv::Scalar(255, 0, 0), 1);
            cv::putText(frame, customer->getName(),
                cv::Point(x, y-10), cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(0, 255, 0));
        }
        else {
            std::string person = "Unknown";
            cv::rectangle(frame, cv::Point(x, y), cv::Point(x + w, y + h), cv::Scalar(0, 0, 255), 3);
            cv::putText(frame, person, cv::Point(x - 10, y - 10), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0));
            //persons.insert(person);
        }
    }

    return std::make_pair(frame, persons);
}
