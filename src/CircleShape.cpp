/***************************************************************
 * Name:      CircleShape.cpp
 * Purpose:   Implements circle shape class
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

#include "wx/wxsf/CircleShape.h"
#include "wx/wxsf/CommonFcn.h"
#include "wx/wxsf/ShapeCanvas.h"

XS_IMPLEMENT_CLONABLE_CLASS(wxSFCircleShape, wxSFSquareShape);

wxSFCircleShape::wxSFCircleShape(void)
: wxSFSquareShape()
{
    SetRectSize(50, 50);
}

wxSFCircleShape::wxSFCircleShape(const wxRealPoint& pos, double radius, wxSFDiagramManager* manager)
: wxSFSquareShape(pos, radius, manager)
{

}

wxSFCircleShape::wxSFCircleShape(const wxSFCircleShape& obj)
: wxSFSquareShape(obj)
{

}

wxSFCircleShape::~wxSFCircleShape()
{

}

//----------------------------------------------------------------------------------//
// public virtual functions
//----------------------------------------------------------------------------------//

wxRealPoint wxSFCircleShape::GetBorderPoint(const wxRealPoint& to)
{
    wxRealPoint center = GetAbsolutePosition() + wxRealPoint(m_nRectSize.x/2, m_nRectSize.y/2);
	double dist = Distance(center, to);

	if(dist)
	{
		double srcDx = m_nRectSize.x/2*(to.x-center.x)/dist;
		double srcDy = m_nRectSize.y/2*(to.y-center.y)/dist;
		return wxRealPoint(center.x + srcDx, center.y + srcDy);
	}
	else
		return center;

}

bool wxSFCircleShape::IsInside(const wxPoint& pos)
{
    wxRealPoint center = GetAbsolutePosition() + wxRealPoint(m_nRectSize.x/2, m_nRectSize.y/2);

    if(Distance(center, wxRealPoint(pos.x, pos.y)) <= m_nRectSize.x/2) return true;
    else
        return false;

}

//----------------------------------------------------------------------------------//
// protected virtual functions
//----------------------------------------------------------------------------------//

void wxSFCircleShape::DrawNormal(wxDC& dc)
{
	// HINT: overload it for custom actions...

	wxRealPoint pos = GetAbsolutePosition();

	dc.SetPen(m_Border);
	dc.SetBrush(m_Fill);
	dc.DrawCircle(int(pos.x + m_nRectSize.x/2), int(pos.y + m_nRectSize.y/2), int(m_nRectSize.x/2));
	dc.SetBrush(wxNullBrush);
	dc.SetPen(wxNullPen);
}

void wxSFCircleShape::DrawHover(wxDC& dc)
{
	// HINT: overload it for custom actions...

	wxRealPoint pos = GetAbsolutePosition();

	dc.SetPen(wxPen(m_nHoverColor, 1));
	dc.SetBrush(m_Fill);
	dc.DrawCircle(int(pos.x + m_nRectSize.x/2), int(pos.y + m_nRectSize.y/2), int(m_nRectSize.x/2));
	dc.SetBrush(wxNullBrush);
	dc.SetPen(wxNullPen);
}

void wxSFCircleShape::DrawHighlighted(wxDC& dc)
{
	// HINT: overload it for custom actions...

    wxRealPoint pos = GetAbsolutePosition();

	dc.SetPen(wxPen(m_nHoverColor, 2));
	dc.SetBrush(m_Fill);
	dc.DrawCircle(int(pos.x + m_nRectSize.x/2), int(pos.y + m_nRectSize.y/2), int(m_nRectSize.x/2));
	dc.SetBrush(wxNullBrush);
	dc.SetPen(wxNullPen);
}

void wxSFCircleShape::DrawShadow(wxDC& dc)
{
	// HINT: overload it for custom actions...

	wxRealPoint pos = GetAbsolutePosition();

	dc.SetPen(*wxTRANSPARENT_PEN);
	dc.SetBrush(GetParentCanvas()->GetShadowFill());
	dc.DrawCircle(int(pos.x + m_nRectSize.x/2 + GetParentCanvas()->GetShadowOffset().x), int(pos.y + m_nRectSize.y/2 + GetParentCanvas()->GetShadowOffset().y), int(m_nRectSize.x/2));
	dc.SetBrush(wxNullBrush);
	dc.SetPen(wxNullPen);
}



