/***************************************************************
 * Name:      EllipseShape.cpp
 * Purpose:   Implements ellipse shape class
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

#include "wx/wxsf/EllipseShape.h"

XS_IMPLEMENT_CLONABLE_CLASS(wxSFEllipseShape, wxSFRectShape);

wxSFEllipseShape::wxSFEllipseShape(void)
:wxSFRectShape()
{

}

wxSFEllipseShape::wxSFEllipseShape(const wxRealPoint& pos, const wxRealPoint& size, wxSFDiagramManager* manager)
: wxSFRectShape(pos, size, manager)
{

}

wxSFEllipseShape::wxSFEllipseShape(const wxSFEllipseShape& obj)
: wxSFRectShape(obj)
{

}

wxSFEllipseShape::~wxSFEllipseShape()
{

}

//----------------------------------------------------------------------------------//
// public virtual functions
//----------------------------------------------------------------------------------//

wxRealPoint wxSFEllipseShape::GetBorderPoint(const wxRealPoint& to)
{
    // HINT: overload it for custom actions...

    // TODO: wxSFEllipseShape: Improve 'GetBorderPoint' function

    return wxSFRectShape::GetBorderPoint(to);
}

bool wxSFEllipseShape::IsInside(const wxPoint& pos)
{
    // HINT: overload it for custom actions...

    double m, n, a, b;
    wxRealPoint apos;

    a = GetRectSize().x/2;
    b = GetRectSize().y/2;
    apos = GetAbsolutePosition();

    m = apos.x + a;
    n = apos.y + b;

    if((((pos.x - m)*(pos.x - m))/(a*a) + ((pos.y - n)*(pos.y - n))/(b*b)) < 1)return true;
    else
        return false;
}

//----------------------------------------------------------------------------------//
// protected virtual functions
//----------------------------------------------------------------------------------//

void wxSFEllipseShape::DrawNormal(wxSFScaledPaintDC& dc)
{
	// HINT: overload it for custom actions...

	dc.SetPen(m_Border);
	dc.SetBrush(m_Fill);
	dc.DrawEllipse(GetAbsolutePosition(), m_nRectSize);
	dc.SetBrush(wxNullBrush);
	dc.SetPen(wxNullPen);
}

void wxSFEllipseShape::DrawHover(wxSFScaledPaintDC& dc)
{
	// HINT: overload it for custom actions...

	dc.SetPen(wxPen(m_nHoverColor, 1));
	dc.SetBrush(m_Fill);
	dc.DrawEllipse(GetAbsolutePosition(), m_nRectSize);
	dc.SetBrush(wxNullBrush);
	dc.SetPen(wxNullPen);
}

void wxSFEllipseShape::DrawHighlighted(wxSFScaledPaintDC& dc)
{
	// HINT: overload it for custom actions...

	dc.SetPen(wxPen(m_nHoverColor, 2));
	dc.SetBrush(m_Fill);
	dc.DrawEllipse(GetAbsolutePosition(), m_nRectSize);
	dc.SetBrush(wxNullBrush);
	dc.SetPen(wxNullPen);
}


