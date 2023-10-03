#include "CIISFrame.h"
#include "Trainer.h"
#include "Recognizer.h"
#include "CameraFrame.h"
#include "MatToBitmap.h"
#include "TrainingDialog.h"
#include <wx/splitter.h>
#include <wx/dataview.h>
#include "CustomerDatabase.h"

// define a custom event type (we don't need a separate declaration here but
// usually you would use a matching wxDECLARE_EVENT in a header)
//wxDEFINE_EVENT(EVT_MY_CUSTOM_COMMAND, wxCommandEvent);


CIISFrame::CIISFrame() : wxFrame(nullptr, wxID_ANY, "Customer Identity and Information System") {
	wxMenu* menuFile = new wxMenu;
	/*menuFile->Append(ID_Hello, "&Hello...\tCtrl-H", "Help string shown in status bar for this menu item");
	menuFile->AppendSeparator();*/
	menuFile->Append(wxID_EXIT);

	wxMenu* menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT);

	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuHelp, "&Help");

	SetMenuBar(menuBar);

	CreateStatusBar();
	SetStatusText("Initializing...");

	Bind(wxEVT_MENU, &CIISFrame::OnAbout, this, wxID_ABOUT);
	Bind(wxEVT_MENU, &CIISFrame::OnExit, this, wxID_EXIT);

	m_mainPanelSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* bottomPanelSizer = new wxBoxSizer(wxHORIZONTAL);

	// Add Buttons
	wxPanel* panel = new wxPanel(this);
	wxButton* newCustomer = new wxButton(panel, ID_New_Customer, "New Customer"/*, wxPoint(150, 100), wxSize(100, 35)*/);
	buttonSizer->Add(newCustomer, wxSizerFlags().Proportion(0.2).Expand().Border());

	wxSplitterWindow* bottomPanelSplitter = new wxSplitterWindow(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_BORDER | wxSP_LIVE_UPDATE);
	wxSplitterWindow* bottomPanelLeftSplitter = new wxSplitterWindow(bottomPanelSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_BORDER | wxSP_LIVE_UPDATE);
	
	m_bitmapPanelRecognizer = new BitmapPanel(bottomPanelSplitter, ID_Recognizer_panel);
	bottomPanelLeftSplitter->SetMinimumPaneSize(200);
	m_customersInShop = new wxListBox(bottomPanelLeftSplitter, wxID_Customers_In_Shop, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_SINGLE | wxLB_NEEDED_SB | wxLB_SORT);
	bottomPanelSplitter->SetMinimumPaneSize(200);
	wxDataViewCtrl* dataViewCtrl1 = new wxDataViewCtrl(bottomPanelLeftSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
	bottomPanelLeftSplitter->SplitHorizontally(m_customersInShop, dataViewCtrl1);
	bottomPanelSplitter->SplitVertically(bottomPanelLeftSplitter, m_bitmapPanelRecognizer, 200);
	bottomPanelSizer->Add(bottomPanelSplitter, 1, wxEXPAND | wxALL, FromDIP(5));
	//bottomPanelSizer->Add(customersInShop, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 5);//wxSizerFlags().Proportion(0.2).Expand().Border());
	//bottomPanelSizer->Add(m_bitmapPanelRecognizer, 0, wxEXPAND | wxALL, 5);//wxSizerFlags().Proportion(0.2).Expand().Border());
	m_mainPanelSizer->Add(buttonSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, FromDIP(5)); //wxSizerFlags().Expand().Border());
	m_mainPanelSizer->Add(bottomPanelSizer, 1, wxEXPAND | wxALL, FromDIP(5)); //wxSizerFlags().Proportion(1).Expand());
	//m_mainPanelSizer->Add(m_bitmapPanelTrainer, wxSizerFlags().Proportion(1).Expand());
	panel->SetSizerAndFit(m_mainPanelSizer);

	OnRecognize();
	CustomerDatabase::CheckCustomerDatabase();
	
	Bind(wxEVT_BUTTON, &CIISFrame::OnNewCustomer, this, ID_New_Customer);
	Bind(wxEVT_LISTBOX, &CIISFrame::OnListBoxSelection, this, wxID_Customers_In_Shop);
	Bind(wxEVT_LISTBOX_DCLICK, &CIISFrame::OnListBoxDoubleClick, this, wxID_Customers_In_Shop);
	/*EVT_CLOSE(CIISFrame::OnClose);
	m_isShown = true;*/
	//Bind(wxEVT_BUTTON, &CIISFrame::OnRecognize, this, ID_Recognize);
}

//void CIISFrame::OnClose(wxCloseEvent& event) {
//	m_isShown = false;
//}

void CIISFrame::OnCameraFrameRecognition(wxThreadEvent& evt) {
	CameraFrame* frame = evt.GetPayload<CameraFrame*>();

	// After deleting the camera thread we may still get a stray frame
	// from yet unprocessed event, just silently drop it.
	/*if (m_mode != IPCamera && m_mode != WebCam)
	{
		delete frame;
		return;
	}*/

	static bool first = true;
	if (first) {
		SetStatusText("Ready");
		first = false;
	}

	long     timeConvert = 0;
	wxBitmap bitmap = MatToBitmap::ConvertMatToBitmap(frame->matBitmap, timeConvert);

	if (bitmap.IsOk()) {
		std::vector<wxString> vec;
		for (wxString customer : frame->customers) {
			if (wxNOT_FOUND == m_customersInShop->FindString(customer)) {
				vec.push_back(customer);
			}
		}
		if (vec.size() > 0) {
			m_customersInShop->EnsureVisible(m_customersInShop->Append(vec));
		}
		m_bitmapPanelRecognizer->SetBitmap(bitmap, frame->timeGet, timeConvert);
	}
	else
		m_bitmapPanelRecognizer->SetBitmap(wxBitmap(), 0, 0);

	delete frame;
	//StartRecognizerThread();
}

void CIISFrame::DeleteRecognizerThread()
{
	if (m_recognizerThread)
	{
		m_recognizerThread->Delete(nullptr, wxTHREAD_WAIT_BLOCK);
		m_recognizerThread = nullptr;
		//wxDELETE(m_recognizerThread);
	}
}

bool CIISFrame::StartRecognizerThread()
{
	DeleteRecognizerThread();
	std::string path("face_data");

	m_recognizerThread = new Recognizer(this, path);
	if (m_recognizerThread->Run() != wxTHREAD_NO_ERROR)
	{
		wxDELETE(m_recognizerThread);
		wxLogError("Could not create the thread needed to retrieve the images from a camera.");
		return false;
	}

	return true;
}

void CIISFrame::OnRecognize() {
	//wxLogMessage("Start recognition!");
	Bind(wxEVT_CAMERA_RECOGNIZE, &CIISFrame::OnCameraFrameRecognition, this);
	StartRecognizerThread();
}

void CIISFrame::OnNewCustomer(wxCommandEvent& event) {
	DeleteRecognizerThread();
	TrainingDialog* frame = new TrainingDialog(this);
	frame->SetClientSize(800, 600);
	frame->Center();
	frame->ShowModal();
	StartRecognizerThread();
	//std::cout << ret << std::endl;
}

void CIISFrame::OnListBoxSelection(wxCommandEvent& event) {
	
}

void CIISFrame::OnListBoxDoubleClick(wxCommandEvent& event) {
	wxArrayInt selections;
	m_customersInShop->GetSelections(selections);

	for (auto selection : selections) {
		m_customersInShop->Delete(selection);
	}
}

void CIISFrame::OnClose(wxCloseEvent& event) {
	StartRecognizerThread();
}

void CIISFrame::OnExit(wxCommandEvent& event) {
	Close(true);
}

void CIISFrame::OnAbout(wxCommandEvent& event) {
	wxMessageBox("This is a wxWidgets Hello World example",
		"About Hello World", wxOK | wxICON_INFORMATION);
}

//void CIISFrame::onIdle(wxIdleEvent& evt) {
//	std::string name("Abhishek");
//	std::unique_ptr<Trainer> train = std::make_unique<Trainer>(name);
//	train->train();
//	evt.RequestMore();
//}
