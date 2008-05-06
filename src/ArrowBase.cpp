/***************************************************************
 * Name:      ArrowBase.cpp
 * Purpose:   Implements line arrow base class
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

#include "wx/wxsf/ArrowBase.h"
#include "wx/wxsf/LineShape.h"

XS_IMPLEMENT_CLONABLE_CLASS(wxSFArrowBase, xsSerializable);

wxSFArrowBase::wxSFArrowBase(void)
{
	m_pParentShape = NULL;
}

wxSFArrowBase::wxSFArrowBase(wxSFShapeBase* parent)
{
	m_pParentShape = parent;
}

wxSFArrowBase::wxSFArrowBase(const wxSFArrowBase& obj)
: xsSerializable(obj)
{
	m_pParentShape = obj.m_pParentShape;
}

wxSFArrowBase::~wxSFArrowBase(void)
{
}

//----------------------------------------------------------------------------------//
// public virtual functions
//----------------------------------------------------------------------------------//

void wxSFArrowBase::Draw(const wxRealPoint& WXUNUSED(from), const wxRealPoint& WXUNUSED(to), wxDC& WXUNUSED(dc))
{
	// HINT: override it for custom drawing...
}
