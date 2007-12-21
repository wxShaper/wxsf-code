/***************************************************************
 * Name:      OpenArrow.cpp
 * Purpose:   Implements open arrow for line shapes
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

#include "wx/wxsf/OpenArrow.h"
#include "wx/wxsf/CommonFcn.h"

// arrow shape
const wxRealPoint arrow[3]={wxRealPoint(0,0), wxRealPoint(10,4), wxRealPoint(10,-4)};

XS_IMPLEMENT_CLONABLE_CLASS(wxSFOpenArrow, wxSFArrowBase);

wxSFOpenArrow::wxSFOpenArrow(void)
: wxSFArrowBase()
{
}

wxSFOpenArrow::wxSFOpenArrow(wxSFShapeBase* parent)
: wxSFArrowBase(parent)
{
}

wxSFOpenArrow::wxSFOpenArrow(wxSFOpenArrow& obj)
: wxSFArrowBase(obj)
{
}

wxSFOpenArrow::~wxSFOpenArrow(void)
{
}

//----------------------------------------------------------------------------------//
// public virtual functions
//----------------------------------------------------------------------------------//

void wxSFOpenArrow::Draw(const wxRealPoint &from, const wxRealPoint &to, wxSFScaledPaintDC &dc)
{
	wxRealPoint rarrow[3];
	double cosa, sina, dist;

	// calculate distance between line points
	dist = Distance(from, to);

	// calculate sin and cos of given line segment
	sina = (from.y - to.y)/dist;
	cosa = (from.x - to.x)/dist;

    // rotate arrow
	for(int i = 0; i<3; i++)
	{
		rarrow[i].x = ((arrow[i].x*cosa-arrow[i].y*sina)+to.x);
		rarrow[i].y = ((arrow[i].x*sina+arrow[i].y*cosa)+to.y);
	}

	DrawArrowShape(3, rarrow, dc);
}

//----------------------------------------------------------------------------------//
// protected virtual functions
//----------------------------------------------------------------------------------//

void wxSFOpenArrow::DrawArrowShape(int n, wxRealPoint pts[], wxSFScaledPaintDC &dc)
{
	dc.DrawLine(pts[0], pts[1]);
    dc.DrawLine(pts[0], pts[2]);
}
