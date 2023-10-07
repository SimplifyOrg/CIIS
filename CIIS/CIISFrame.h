#pragma once
#include <wx/wx.h>
#include <opencv2/core/mat.hpp>
#include "BitmapPanel.h"
#include <opencv2/videoio.hpp>
#include <map>

class Recognizer;
class DeviceEnumerator;

enum {
	ID_Train = wxID_HIGHEST + 1,
	ID_New_Customer,
	ID_Camera_Chooser,
	ID_Recognizer_panel,
	ID_Trainer_panel,
	ID_Train_Event,
	wxID_Train_ID_Label,
	wxID_Train_ID,
	wxID_Train_Customer_Name,
	wxID_Train_Customer_Name_Enter,
	wxID_Train_Customer_Phone_Number,
	wxID_Train_Phone_Enter,
	wxID_Train_Button,
	wxID_Customers_In_Shop
};

class CIISFrame : public wxFrame {
public:
	CIISFrame();
	~CIISFrame();

	void OnCameraFrameRecognition(wxThreadEvent& evt);
	//bool isFrameShown() { return m_isShown; }

private:

	void DeleteRecognizerThread();

	bool StartRecognizerThread();

	void OnRecognize();
	void OnNewCustomer(wxCommandEvent& event);
	void OnListBoxSelection(wxCommandEvent& event);
	void OnListBoxDoubleClick(wxCommandEvent& event);
	void OnCameraChooserSelect(wxCommandEvent& event);
	void OnClose(wxCloseEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	//void OnClose(wxCloseEvent& event);
	//void onIdle(wxIdleEvent& evt);

private:
	BitmapPanel* m_bitmapPanelRecognizer{nullptr};
	wxBoxSizer* m_mainPanelSizer{ nullptr };
	Recognizer* m_recognizerThread{ nullptr };
	DeviceEnumerator* m_cameraEnumerator{ nullptr };
	wxChoice* m_chooseCamera{ nullptr };
	int m_camera{ 0 };
	wxListBox* m_customersInShop{ nullptr };
	std::map<int, cv::Ptr<cv::VideoCapture>> m_cameraIndexMap;

	//wxDECLARE_EVENT_TABLE();
};

