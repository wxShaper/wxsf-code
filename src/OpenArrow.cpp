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
static const wxRealPoint arrow[3]={wxRealPoint(0,0), wxRealPoint(10,4), wxRealPoint(10,-4)};

using namespace wxSFCommonFcn;

XS_IMPLEMENT_CLONABLE_CLASS(wxSFOpenArrow, wxSFArrowBase);

wxSFOpenArrow::wxSFOpenArrow(void)
: wxSFArrowBase()
{
}

wxSFOpenArrow::wxSFOpenArrow(wxSFShapeBase* parent)
: wxSFArrowBase(parent)
{
}

wxSFOpenArrow::wxSFOpenArrow(const wxSFOpenArrow& obj)
: wxSFArrowBase(obj)
{
}

wxSFOpenArrow::~wxSFOpenArrow(void)
{
}

//----------------------------------------------------------------------------------//
// public virtual functions
//----------------------------------------------------------------------------------//

void wxSFOpenArrow::Draw(const wxRealPoint &from, const wxRealPoint &to, wxDC& dc)
{
	wxPoint rarrow[3];
	
	TranslateArrow( rarrow, arrow, 3, from, to );

	dc.DrawLine(rarrow[0], rarrow[1]);
    dc.DrawLine(rarrow[0], rarrow[2]);
}
