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

#include "wx/wxsf/ArrowBase.h"
#include "wx/wxsf/LineShape.h"

IMPLEMENT_DYNAMIC_CLASS(wxSFArrowBase, xsSerializable);

wxSFArrowBase::wxSFArrowBase(void)
{
	m_pParentShape = NULL;
}

wxSFArrowBase::wxSFArrowBase(wxSFShapeBase* parent)
{
	m_pParentShape = parent;
}

wxSFArrowBase::wxSFArrowBase(wxSFArrowBase& obj)
{
	m_pParentShape = obj.m_pParentShape;
}

wxSFArrowBase::~wxSFArrowBase(void)
{
}

//----------------------------------------------------------------------------------//
// public virtual functions
//----------------------------------------------------------------------------------//

void wxSFArrowBase::Draw(const wxRealPoint &from, const wxRealPoint &to, wxSFScaledPaintDC &dc)
{
	// HINT: override it for custom drawing...
}

