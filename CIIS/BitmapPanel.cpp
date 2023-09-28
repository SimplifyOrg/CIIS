#include <wx/wx.h>
#include <wx/colordlg.h>
#include <wx/dcbuffer.h>
#include <wx/fontdlg.h>

#include "BitmapPanel.h"

BitmapPanel::BitmapPanel(wxWindow* parent, wxWindowID id = wxID_ANY)
    : wxScrolledCanvas(parent, id, wxDefaultPosition, wxSize(600, 480), wxFULL_REPAINT_ON_RESIZE)
{
    m_overlayTextColour = *wxGREEN;
    m_overlayFont = GetFont();

    SetBackgroundColour(*wxBLACK);
    SetBackgroundStyle(wxBG_STYLE_PAINT);

    SetScrollRate(FromDIP(8), FromDIP(8));
    // We need to do this to prevent drawing artefacts
    // due to the info "overlay" which does not scroll with the bitmap.
    EnableScrolling(false, false);

    Bind(wxEVT_PAINT, &BitmapPanel::OnPaint, this);

    Bind(wxEVT_LEFT_DCLICK, &BitmapPanel::OnChangeOverlayTextColour, this);
    Bind(wxEVT_RIGHT_DCLICK, &BitmapPanel::OnChangeOverlayFont, this);
}

bool BitmapPanel::SetBitmap(const wxBitmap& bitmap, const long timeGet, const long timeConvert)
{
    m_bitmap = bitmap;
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    if (m_bitmap.IsOk())
    {
        if (m_bitmap.GetSize() != GetVirtualSize())
        {
            InvalidateBestSize();
            //SetVirtualSize(m_bitmap.GetSize());
            m_bitmap.Rescale(m_bitmap, GetVirtualSize());
            //m_bitmap.SetHeight(GetVirtualSize().GetHeight());
            //m_bitmap.SetWidth(GetVirtualSize().GetWidth());
        }
    }
    else
    {
        InvalidateBestSize();
        SetVirtualSize(1, 1);
    }

    m_timeGetCVBitmap = timeGet;
    m_timeConvertBitmap = timeConvert;

    Refresh(); Update();
    return true;
}

wxSize BitmapPanel::DoGetBestClientSize() const
{
    if (!m_bitmap.IsOk())
        return FromDIP(wxSize(112, 92)); // completely arbitrary

    return m_bitmap.GetSize();
}

void BitmapPanel::OnPaint(wxPaintEvent&)
{
    wxAutoBufferedPaintDC dc(this);

    dc.Clear();

    if (!m_bitmap.IsOk())
        return;

    const wxSize clientSize = GetClientSize();
    wxPoint      offset = GetViewStart();
    int          pixelsPerUnitX = 0, pixelsPerUnitY = 0;
    wxStopWatch  stopWatch;

    stopWatch.Start();

    DoPrepareDC(dc);

    dc.DrawBitmap(m_bitmap, 0, 0, false);

    GetScrollPixelsPerUnit(&pixelsPerUnitX, &pixelsPerUnitY);
    offset.x *= pixelsPerUnitX; offset.y *= pixelsPerUnitY;

    // Draw info "overlay", always at the top left corner of the window
    // regardless of how the bitmap is scrolled.
    const long            drawTime = stopWatch.Time();
    wxDCTextColourChanger textColourChanger(dc, m_overlayTextColour);
    wxDCFontChanger       fontChanger(dc, m_overlayFont);

    /*dc.DrawText(wxString::Format("GetCVBitmap: %ld ms\nConvertCVtoWXBitmap: %ld ms\nDrawWXBitmap: %ld ms\n",
        m_timeGetCVBitmap, m_timeConvertBitmap, drawTime),
        offset);*/
}


void BitmapPanel::OnChangeOverlayTextColour(wxMouseEvent&)
{
    const wxColour colour = wxGetColourFromUser(this, m_overlayTextColour,
        "Color for text overlay");

    if (!colour.IsOk())
        return;

    m_overlayTextColour = colour;
    Refresh(); Update();
}

void BitmapPanel::OnChangeOverlayFont(wxMouseEvent&)
{
    const wxFont font = wxGetFontFromUser(this, m_overlayFont,
        "Font for text overlay");

    if (!font.IsOk())
        return;

    m_overlayFont = font;
    Refresh(); Update();
}
