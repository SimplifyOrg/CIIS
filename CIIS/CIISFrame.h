#pragma once
#include <wx/wx.h>
#include <opencv2/core/mat.hpp>
#include "BitmapPanel.h"

class Recognizer;

enum {
	ID_Train = wxID_HIGHEST + 1,
	ID_New_Customer,
	ID_Recognizer_panel,
	ID_Trainer_panel,
	ID_Train_Event,
	wxID_Train_ID_Label,
	wxID_Train_ID,
	wxID_Train_Customer_Name,
	wxID_Train_Customer_Name_Enter,
	wxID_Train_Customer_Phone_Number,
	wxID_Train_Phone_Enter,
	wxID_Train_Button
};

class CIISFrame : public wxFrame {
public:
	CIISFrame();

	void OnCameraFrameRecognition(wxThreadEvent& evt);
	//bool isFrameShown() { return m_isShown; }

private:

	void DeleteRecognizerThread();

	bool StartRecognizerThread();

	void OnRecognize();
	void OnNewCustomer(wxCommandEvent& event);
	void OnClose(wxCloseEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	//void OnClose(wxCloseEvent& event);
	//void onIdle(wxIdleEvent& evt);

private:
	BitmapPanel* m_bitmapPanelRecognizer{nullptr};
	wxBoxSizer* m_mainPanelSizer{ nullptr };
	Recognizer* m_recognizerThread{ nullptr };

	//wxDECLARE_EVENT_TABLE();
};

