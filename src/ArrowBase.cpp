/***************************************************************
 * Name:      ArrowBase.cpp
 * Purpose:   Implements line arrow base class
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "ArrowBase.h"
#include "LineShape.h"
#include "CommonFcn.h"

IMPLEMENT_DYNAMIC_CLASS(wxSFArrowBase, wxObject);

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

//----------------------------------------------------------------------------------//
// Serialization
//----------------------------------------------------------------------------------//

wxXmlNode* wxSFArrowBase::Serialize(wxXmlNode* node)
{
	// HINT: overload it for custom actions...

	return node;
}

void wxSFArrowBase::Deserialize(wxXmlNode* node)
{
	// HINT: overload it for custom actions...
}
