#pragma once
#include "wx/wx.h"
#include <opencv2/core/mat.hpp>
#include "BitmapPanel.h"
#include "Trainer.h"

class Trainer;
class TrainingDialog :
    public wxDialog
{
public:
    TrainingDialog(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(500, 300), long style = wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL);

    void OnCameraFrameTrainer(wxThreadEvent& evt);

private:
    void DeleteTrainerThread();
    bool StartTrainerThread();
    void OnTrain(wxCommandEvent& event);

private:
    BitmapPanel* m_bitmapPanelTrainer{ nullptr };
    Trainer* m_trainerThread{ nullptr };

    wxStaticText* m_IDLabel;
    wxStaticText* m_ID;
    wxStaticText* m_name;
    wxTextCtrl* m_customerName;
    wxStaticText* m_customerPhone;
    wxTextCtrl* m_phone;
    wxButton* m_trainingButton;

    std::string m_customerID;
    std::shared_ptr<Customer> m_customer;
};

