/***************************************************************
 * Name:      SolidArrow.cpp
 * Purpose:   Implements solid arrow for line shapes
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

#include "wx/wxsf/SolidArrow.h"
#include "wx/wxsf/CommonFcn.h"

XS_IMPLEMENT_CLONABLE_CLASS(wxSFSolidArrow, wxSFOpenArrow);

wxSFSolidArrow::wxSFSolidArrow(void)
: wxSFOpenArrow()
{
	m_Fill = sfdvARROW_FILL;

	MarkSerializableDataMembers();
}

wxSFSolidArrow::wxSFSolidArrow(wxSFShapeBase* parent)
: wxSFOpenArrow(parent)
{
	m_Fill = sfdvARROW_FILL;

	MarkSerializableDataMembers();
}

wxSFSolidArrow::wxSFSolidArrow(const wxSFSolidArrow& obj)
: wxSFOpenArrow(obj)
{
	m_Fill = obj.m_Fill;

	MarkSerializableDataMembers();
}

wxSFSolidArrow::~wxSFSolidArrow(void)
{
}

void wxSFSolidArrow::MarkSerializableDataMembers()
{
	XS_SERIALIZE_EX(m_Fill, wxT("fill"), sfdvARROW_FILL);
}

//----------------------------------------------------------------------------------//
// protected virtual functions
//----------------------------------------------------------------------------------//

void wxSFSolidArrow::DrawArrowShape(int n, wxPoint pts[], wxDC& dc)
{
    dc.SetBrush(m_Fill);
    dc.DrawPolygon(n, pts);
    dc.SetBrush(wxNullBrush);
}
