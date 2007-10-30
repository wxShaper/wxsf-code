/***************************************************************
 * Name:      SolidArrow.cpp
 * Purpose:   Implements solid arrow for line shapes
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "wx/wxsf/SolidArrow.h"
#include "wx/wxsf/CommonFcn.h"

IMPLEMENT_DYNAMIC_CLASS(wxSFSolidArrow, wxSFOpenArrow);

wxSFSolidArrow::wxSFSolidArrow(void)
: wxSFOpenArrow()
{
	m_Fill = sfdvARROW_FILL;

	XS_SERIALIZE_EX(m_Fill, wxT("fill"), sfdvARROW_FILL);
}

wxSFSolidArrow::wxSFSolidArrow(wxSFShapeBase* parent)
: wxSFOpenArrow(parent)
{
	m_Fill = sfdvARROW_FILL;

	XS_SERIALIZE_EX(m_Fill, wxT("fill"), sfdvARROW_FILL);
}

wxSFSolidArrow::wxSFSolidArrow(wxSFSolidArrow& obj)
: wxSFOpenArrow(obj)
{
	m_Fill = obj.m_Fill;
}

wxSFSolidArrow::~wxSFSolidArrow(void)
{
}

//----------------------------------------------------------------------------------//
// protected virtual functions
//----------------------------------------------------------------------------------//

void wxSFSolidArrow::DrawArrowShape(int n, wxRealPoint pts[], wxSFScaledPaintDC &dc)
{
    dc.SetBrush(m_Fill);
    dc.DrawPolygon(n, pts);
    dc.SetBrush(wxNullBrush);
}
