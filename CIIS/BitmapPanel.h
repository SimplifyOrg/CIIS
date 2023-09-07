#pragma once
#include <wx/wx.h>
#include <wx/scrolwin.h>

class BitmapPanel : public wxScrolledCanvas {
public:
    BitmapPanel(wxWindow* parent, wxWindowID id);

    bool SetBitmap(const wxBitmap& bitmap, const long timeGet, const long timeConvert);

    const wxBitmap& GetBitmap() { return m_bitmap; }

private:
    wxBitmap m_bitmap;
    wxColour m_overlayTextColour;
    wxFont   m_overlayFont;
    long     m_timeGetCVBitmap{ 0 };   // time to obtain bitmap from OpenCV in ms
    long     m_timeConvertBitmap{ 0 }; // time to convert Mat to wxBitmap in ms

    wxSize DoGetBestClientSize() const override;

    void OnPaint(wxPaintEvent&);

    void OnChangeOverlayTextColour(wxMouseEvent&);
    void OnChangeOverlayFont(wxMouseEvent&);
};

