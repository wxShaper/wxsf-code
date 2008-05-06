/***************************************************************
 * Name:      ScaledPaintDC.cpp
 * Purpose:   Implements scaled DC class
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "wx_pch.h"

#ifdef _DEBUG_MSVC
#define new DEBUG_NEW
#endif

#include "wx/wxsf/ScaledPaintDC.h"

wxSFScaledPaintDC::wxSFScaledPaintDC(wxBitmap& outbmp, double scale)
: wxMemoryDC(outbmp)
{
	m_nScale = scale;
}

wxSFScaledPaintDC::~wxSFScaledPaintDC(void)
{
    SelectObject(wxNullBitmap);
}

//----------------------------------------------------------------------------------//
// protected virtual functions
//----------------------------------------------------------------------------------//

void wxSFScaledPaintDC::DoDrawPoint(wxCoord x, wxCoord y)
{
    wxMemoryDC::DoDrawPoint(Scale(x), Scale(y));
}

void wxSFScaledPaintDC::DoDrawLine(wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2)
{
    wxMemoryDC::DoDrawLine(Scale(x1), Scale(y1), Scale(x2), Scale(y2));
}

void wxSFScaledPaintDC::DoDrawArc(wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2, wxCoord xc, wxCoord yc)
{
    wxMemoryDC::DoDrawArc(Scale(x1), Scale(y1), Scale(x2), Scale(y2), Scale(xc), Scale(yc));
}

void wxSFScaledPaintDC::DoDrawCheckMark(wxCoord x, wxCoord y, wxCoord width, wxCoord height)
{
    wxMemoryDC::DoDrawCheckMark(Scale(x), Scale(y), Scale(width), Scale(height));
}

void wxSFScaledPaintDC::DoDrawEllipticArc(wxCoord x, wxCoord y, wxCoord w, wxCoord h, double sa, double ea)
{
    wxMemoryDC::DoDrawEllipticArc(Scale(x), Scale(y), Scale(w), Scale(h), sa, ea);
}

void wxSFScaledPaintDC::DoDrawRectangle(wxCoord x, wxCoord y, wxCoord width, wxCoord height)
{
    wxMemoryDC::DoDrawRectangle(Scale(x), Scale(y), Scale(width), Scale(height));
}

void wxSFScaledPaintDC::DoDrawRoundedRectangle(wxCoord x, wxCoord y, wxCoord width, wxCoord height, double radius)
{
    wxMemoryDC::DoDrawRoundedRectangle(Scale(x), Scale(y), Scale(width), Scale(height), radius*m_nScale);
}

void wxSFScaledPaintDC::DoDrawEllipse(wxCoord x, wxCoord y, wxCoord width, wxCoord height)
{
    wxMemoryDC::DoDrawEllipse(Scale(x), Scale(y), Scale(width), Scale(height));
}

void wxSFScaledPaintDC::DoCrossHair(wxCoord x, wxCoord y)
{
    wxMemoryDC::DoCrossHair(Scale(x), Scale(y));
}

void wxSFScaledPaintDC::DoDrawIcon(const wxIcon& icon, wxCoord x, wxCoord y)
{
    wxMemoryDC::DoDrawIcon(icon, Scale(x), Scale(y));
}

void wxSFScaledPaintDC::DoDrawBitmap(const wxBitmap &bmp, wxCoord x, wxCoord y, bool useMask)
{
    wxMemoryDC::DoDrawBitmap(bmp, Scale(x), Scale(y), useMask);
}

void wxSFScaledPaintDC::DoDrawText(const wxString& text, wxCoord x, wxCoord y)
{
	wxFont font = GetFont();
	wxFont prevfont = font;

	if(font != wxNullFont)
	{
		font.SetPointSize(int(font.GetPointSize()*m_nScale));
		SetFont(font);
	}
    wxMemoryDC::DoDrawText(text, Scale(x), Scale(y));

    SetFont(prevfont);
}

void wxSFScaledPaintDC::DoDrawRotatedText(const wxString& text, wxCoord x, wxCoord y, double angle)
{
	wxFont font = GetFont();
	wxFont prevfont = font;

	if(font != wxNullFont)
	{
		font.SetPointSize(int(font.GetPointSize()*m_nScale));
		SetFont(font);
	}
    wxMemoryDC::DoDrawRotatedText(text, Scale(x), Scale(y), angle);

    SetFont(prevfont);
}

void wxSFScaledPaintDC::DoDrawLines(int n, wxPoint points[], wxCoord xoffset, wxCoord yoffset)
{
    wxPoint *updPoints = new wxPoint[n];

    memcpy(updPoints, points, n*sizeof(wxPoint));
    for(int i = 0; i < n; i++)
    {
        updPoints[i].x = (int)((double)updPoints[i].x*m_nScale);
        updPoints[i].y = (int)((double)updPoints[i].y*m_nScale);
    }

    wxMemoryDC::DoDrawLines(n, updPoints, Scale(xoffset), Scale(yoffset));

    delete [] updPoints;
}

void wxSFScaledPaintDC::DoDrawPolygon(int n, wxPoint points[], wxCoord xoffset, wxCoord yoffset, int fillStyle)
{
    wxPoint *updPoints = new wxPoint[n];

    memcpy(updPoints, points, n*sizeof(wxPoint));
    for(int i = 0; i < n; i++)
    {
        updPoints[i].x = (int)((double)updPoints[i].x*m_nScale);
        updPoints[i].y = (int)((double)updPoints[i].y*m_nScale);
    }

    wxMemoryDC::DoDrawPolygon(n, updPoints, Scale(xoffset), Scale(yoffset), fillStyle);

    delete [] updPoints;
}

void wxSFScaledPaintDC::DoDrawPolyPolygon(int n, int count[], wxPoint points[], wxCoord xoffset, wxCoord yoffset, int fillStyle)
{
    int nTotalPoints = 0;

    for(int i = 0; i < n; i++)nTotalPoints += count[i];

    wxPoint *updPoints = new wxPoint[nTotalPoints];

    memcpy(updPoints, points, nTotalPoints*sizeof(wxPoint));
    for(int i = 0; i < nTotalPoints; i++)
    {
        updPoints[i].x = (int)((double)updPoints[i].x*m_nScale);
        updPoints[i].y = (int)((double)updPoints[i].y*m_nScale);
    }

    wxMemoryDC::DoDrawPolyPolygon(n, count, updPoints, Scale(xoffset), Scale(yoffset));

    delete [] updPoints;
}
