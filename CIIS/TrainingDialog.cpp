#include "TrainingDialog.h"
#include "CameraFrame.h"
#include "MatToBitmap.h"
#include "CIISFrame.h"

void TrainingDialog::OnTrain(wxCommandEvent& event) {
	//wxLogMessage("Start training!");
	// TODO: Get Customer information before training
	std::shared_ptr<Customer> customer = Customer::getBuilder()
		->setCustomerIdentification(m_customerID)
		->setName(m_customerName->GetValue().ToStdString())
		->setPhoneNumber(m_phone->GetValue().ToStdString())
		->build();
	
	m_trainerThread = new Trainer(this, customer);
	m_trainerThread->setTrainingFlag(true);
	StartTrainerThread();
	
	//m_trainerThread->train();
	//Connect(wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(CIISFrame::onIdle));
	/*Bind(wxEVT_CAMERA_TRAIN, &CIISFrame::OnCameraFrame, this);
	std::string name("Abhishek");
	std::unique_ptr<Trainer> train = std::make_unique<Trainer>(this, name);
	train->Run();*/

}

bool TrainingDialog::StartTrainerThread() {
	
	Bind(wxEVT_CAMERA_TRAIN, &TrainingDialog::OnCameraFrameTrainer, this);
	if (m_trainerThread->Run() != wxTHREAD_NO_ERROR)
	{
		wxDELETE(m_trainerThread);
		wxLogError("Could not create the thread needed to retrieve the images from a camera.");
		return false;
	}

	return true;
}

TrainingDialog::TrainingDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxDialog(parent, id, title, pos, size, wxDEFAULT_DIALOG_STYLE) {
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* customerGrid;
	customerGrid = new wxBoxSizer(wxVERTICAL);

	//EVT_CLOSE(func.target())

	wxBoxSizer* customerInformation;
	customerInformation = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* customerID;
	customerID = new wxBoxSizer(wxHORIZONTAL);

	m_IDLabel = new wxStaticText(this, wxID_Train_ID_Label, wxT("Identify Number:"), wxDefaultPosition, wxDefaultSize, 0);
	//m_IDLabel->Wrap(-1);
	customerID->Add(m_IDLabel, 0, wxALL, 1);

	DeleteTrainerThread();
	std::shared_ptr<Customer> customer = std::make_shared<Customer>();
	m_trainerThread = new Trainer(this, customer);
	m_trainerThread->createCustomerIdentity();
	StartTrainerThread();
	m_customerID.assign(m_trainerThread->getCustomer()->m_customerIdentificationNumber);
	m_ID = new wxStaticText(this, wxID_Train_ID, m_trainerThread->getCustomer()->m_customerIdentificationNumber, wxDefaultPosition, wxDefaultSize, 0);
	//m_ID->Wrap(-1);
	customerID->Add(m_ID, 0, wxALL, 1);


	customerInformation->Add(customerID, 0, wxEXPAND, 0);

	wxBoxSizer* customerName;
	customerName = new wxBoxSizer(wxHORIZONTAL);

	m_name = new wxStaticText(this, wxID_Train_Customer_Name, wxT("Customer Name:"), wxDefaultPosition, wxDefaultSize, 0);
	//m_name->Wrap(-1);
	customerName->Add(m_name, 0, wxALL, 1);

	m_customerName = new wxTextCtrl(this, wxID_Train_Customer_Name_Enter, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	customerName->Add(m_customerName, 0, wxALL, 1);


	customerInformation->Add(customerName, 0, wxEXPAND, 1);

	wxBoxSizer* customerPhone;
	customerPhone = new wxBoxSizer(wxHORIZONTAL);

	m_customerPhone = new wxStaticText(this, wxID_Train_Customer_Phone_Number, wxT("Phone Number"), wxDefaultPosition, wxDefaultSize, 0);
	//m_customerPhone->Wrap(-1);
	customerPhone->Add(m_customerPhone, 0, wxALL, 1);

	m_phone = new wxTextCtrl(this, wxID_Train_Phone_Enter, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	customerPhone->Add(m_phone, 0, wxALL, 1);

	customerInformation->Add(customerPhone, 0, wxEXPAND, 0);

	wxBoxSizer* buttons;
	buttons = new wxBoxSizer(wxHORIZONTAL);

	m_trainingButton = new wxButton(this, wxID_Train_Button, wxT("Train"), wxDefaultPosition, wxDefaultSize, 0);
	buttons->Add(m_trainingButton, 0, wxALL, 5);
	Bind(wxEVT_BUTTON, &TrainingDialog::OnTrain, this, wxID_Train_Button);

	customerInformation->Add(buttons, 0, wxEXPAND, 1);
	customerGrid->Add(customerInformation, 0, wxEXPAND, 1);

	m_bitmapPanelTrainer = new BitmapPanel(this, ID_Trainer_panel);

	customerGrid->Add(m_bitmapPanelTrainer, 1, wxEXPAND | wxALL, 5);

	this->SetSizer(customerGrid);
	this->Layout();

	this->Centre(wxBOTH);
}

void TrainingDialog::OnCameraFrameTrainer(wxThreadEvent& evt) {
	CameraFrame* frame = evt.GetPayload<CameraFrame*>();

	// After deleting the camera thread we may still get a stray frame
	// from yet unprocessed event, just silently drop it.
	/*if (m_mode != IPCamera && m_mode != WebCam)
	{
		delete frame;
		return;
	}*/

	long     timeConvert = 0;
	wxBitmap bitmap = MatToBitmap::ConvertMatToBitmap(frame->matBitmap, timeConvert);

	if (bitmap.IsOk())
		m_bitmapPanelTrainer->SetBitmap(bitmap, frame->timeGet, timeConvert);
	else
		m_bitmapPanelTrainer->SetBitmap(wxBitmap(), 0, 0);

	delete frame;
	//StartRecognizerThread();
}

void TrainingDialog::DeleteTrainerThread() {
	if (m_trainerThread)
	{
		m_trainerThread->Delete(nullptr, wxTHREAD_WAIT_BLOCK);
		//wxDELETE(m_trainerThread);
	}
}
