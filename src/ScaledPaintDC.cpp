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
// public functions
//----------------------------------------------------------------------------------//

void wxSFScaledPaintDC::DrawCircle(double x, double y, double radius)
{
    wxMemoryDC::DrawCircle(int(x*m_nScale) , int(y*m_nScale), int(radius*m_nScale));
}

void wxSFScaledPaintDC::DrawCircle(const wxRealPoint& pt, double radius)
{
    DrawCircle(pt.x, pt.y, radius);
}

//----------------------------------------------------------------------------------//

void wxSFScaledPaintDC::DrawEllipse(double x, double y, double width, double height)
{
	wxMemoryDC::DrawEllipse(int(x*m_nScale), int(y*m_nScale), int(width*m_nScale), int(height*m_nScale));
}

void wxSFScaledPaintDC::DrawEllipse(const wxRealPoint& pt, const wxRealPoint& size)
{
	DrawEllipse(pt.x, pt.y, size.x, size.y);
}

void wxSFScaledPaintDC::DrawEllipse(const wxRect& rct)
{
	DrawEllipse(rct.GetLeft(), rct.GetTop(), rct.GetWidth(), rct.GetHeight());
}

//----------------------------------------------------------------------------------//

void wxSFScaledPaintDC::DrawRectangle(double x, double y, double width, double height)
{
	wxMemoryDC::DrawRectangle(int(x*m_nScale), int(y*m_nScale), int(width*m_nScale), int(height*m_nScale));
}

void wxSFScaledPaintDC::DrawRectangle(const wxRealPoint& pos, const wxRealPoint& size)
{
	DrawRectangle(pos.x, pos.y, size.x, size.y);
}

void wxSFScaledPaintDC::DrawRectangle(const wxRect& rct)
{
	DrawRectangle(rct.GetLeft(), rct.GetTop(), rct.GetWidth(), rct.GetHeight());
}

//----------------------------------------------------------------------------------//

void wxSFScaledPaintDC::DrawRoundedRectangle(double x, double y, double width, double height, double radius)
{
	wxMemoryDC::DrawRoundedRectangle(int(x*m_nScale), int(y*m_nScale), int(width*m_nScale), int(height*m_nScale), radius*m_nScale);
}

void wxSFScaledPaintDC::DrawRoundedRectangle(const wxRealPoint& pos, const wxRealPoint& size, double radius)
{
	DrawRoundedRectangle(pos.x, pos.y, size.x, size.y, radius);
}

void wxSFScaledPaintDC::DrawRoundedRectangle(const wxRect& rct, double radius)
{
	DrawRoundedRectangle(rct.GetLeft(), rct.GetTop(), rct.GetWidth(), rct.GetHeight(), radius);
}

//----------------------------------------------------------------------------------//

void wxSFScaledPaintDC::DrawPoint(double x, double y)
{
	wxMemoryDC::DrawPoint(int(x*m_nScale), int(y*m_nScale));
}

void wxSFScaledPaintDC::DrawPoint(const wxRealPoint& pt)
{
	DrawPoint(pt.x, pt.y);
}

//----------------------------------------------------------------------------------//

void wxSFScaledPaintDC::DrawLine(double x1, double y1, double x2, double y2)
{
	wxMemoryDC::DrawLine(int(x1*m_nScale), int(y1*m_nScale), int(x2*m_nScale), int(y2*m_nScale));
}

void wxSFScaledPaintDC::DrawLine(const wxRealPoint& pt1, const wxRealPoint& pt2)
{
	DrawLine(pt1.x, pt1.y, pt2.x, pt2.y);
}

//----------------------------------------------------------------------------------//

void wxSFScaledPaintDC::DrawLines(wxList* points, wxCoord xoffset, wxCoord yoffset)
{
    if(points && (points->GetCount() > 0))
    {
        wxRealPoint* pt;
        size_t i = 0, pointCnt = points->GetCount();

        wxPoint* pts = new wxPoint[pointCnt];

        wxList::compatibility_iterator node = points->GetFirst();
        while(node)
        {
            pt = (wxRealPoint*)node->GetData();
            pts[i].x = int(pt->x*m_nScale);
            pts[i].y = int(pt->y*m_nScale);
            node = node->GetNext();
        }

        wxMemoryDC::DrawLines((int)pointCnt, pts, xoffset, yoffset);

        delete [] pts;
    }
}

void wxSFScaledPaintDC::DrawLines(int n, wxRealPoint points[], wxCoord xoffset, wxCoord yoffset)
{
	if(n > 0)
	{
		wxPoint* pts = new wxPoint[n];

        for(int i = 0; i < n; i++)
        {
            pts[i].x = int(points[i].x*m_nScale);
            pts[i].y = int(points[i].y*m_nScale);
        }

        wxMemoryDC::DrawLines(n, pts, xoffset, yoffset);

        delete [] pts;
	}
}


//----------------------------------------------------------------------------------//

void wxSFScaledPaintDC::DrawPolygon(wxList *points, wxCoord xoffset, wxCoord yoffset, int fill_style)
{
	if(points && (points->GetCount() > 0))
	{
		wxRealPoint* pt;
        size_t i = 0, pointCnt = points->GetCount();

        wxPoint* pts = new wxPoint[pointCnt];

        wxList::compatibility_iterator node = points->GetFirst();
        while(node)
        {
            pt = (wxRealPoint*)node->GetData();
            pts[i].x = int(pt->x*m_nScale);
            pts[i].y = int(pt->y*m_nScale);
            node = node->GetNext();
        }

        wxMemoryDC::DrawPolygon((int)pointCnt, pts, xoffset, yoffset, fill_style);

        delete [] pts;
	}
}

void wxSFScaledPaintDC::DrawPolygon(size_t n, wxRealPoint points[], wxCoord xoffset, wxCoord yoffset,int fill_style)
{
    if(n > 0)
    {
        wxPoint* pts = new wxPoint[n];

        for( size_t i = 0; i < n; i ++ )
        {
            pts[i].x = int(points[i].x * m_nScale);
            pts[i].y = int(points[i].y * m_nScale);
        }

        wxMemoryDC::DrawPolygon((int)n, pts, xoffset, yoffset, fill_style);

        delete [] pts;
    }
}

//----------------------------------------------------------------------------------//

void wxSFScaledPaintDC::DrawText(const wxString& txt, double x, double y)
{
	wxFont font = GetFont();
	wxFont prevfont = font;

	if(font != wxNullFont)
	{
		font.SetPointSize(int(font.GetPointSize()*m_nScale));
		SetFont(font);
	}
    wxMemoryDC::DrawText(txt, int(x*m_nScale), int(y*m_nScale));

    SetFont(prevfont);
}

void wxSFScaledPaintDC::DrawText(const wxString& txt, const wxRealPoint& pos)
{
	DrawText(txt, pos.x, pos.y);
}

//----------------------------------------------------------------------------------//

void wxSFScaledPaintDC::DrawBitmap(const wxBitmap& bitmap, double x, double y, bool useMask)
{
	wxMemoryDC::DrawBitmap(bitmap, int(x*m_nScale), int(y*m_nScale), useMask);
}

void wxSFScaledPaintDC::DrawBitmap(const wxBitmap& bitmap, const wxRealPoint& pt, bool useMask)
{
	DrawBitmap(bitmap, pt.x, pt.y, useMask);
}

//----------------------------------------------------------------------------------//

void wxSFScaledPaintDC::DrawArc(double x1, double y1, double x2, double y2, double xc, double yc)
{
	wxMemoryDC::DrawArc(int(x1*m_nScale), int(y1*m_nScale), int(x2*m_nScale), int(y2*m_nScale), int(xc*m_nScale), int(yc*m_nScale));
}

void wxSFScaledPaintDC::DrawArc(const wxRealPoint& pt1, const wxRealPoint& pt2, const wxRealPoint& center)
{
	DrawArc(pt1.x, pt1.y, pt2.x, pt2.y, center.x, center.y);
}
