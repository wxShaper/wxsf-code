/***************************************************************
 * Name:      SolidArrow.cpp
 * Purpose:   Implements solid arrow for line shapes
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "SolidArrow.h"
#include "CommonFcn.h"

IMPLEMENT_DYNAMIC_CLASS(wxSFSolidArrow, wxSFOpenArrow);

wxSFSolidArrow::wxSFSolidArrow(void)
: wxSFOpenArrow()
{
	m_Fill = sfdvARROW_FILL;
}

wxSFSolidArrow::wxSFSolidArrow(wxSFShapeBase* parent)
: wxSFOpenArrow(parent)
{
	m_Fill = sfdvARROW_FILL;
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

//----------------------------------------------------------------------------------//
// Serialization
//----------------------------------------------------------------------------------//

wxXmlNode* wxSFSolidArrow::Serialize(wxXmlNode* node)
{
	// HINT: overload it for custom actions...

	// save 'm_Fill' property
	if(m_Fill != sfdvARROW_FILL)
	{
		wxXmlNode* child = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("fill"));
		AddPropertyNode(child, wxT("color"), ColourToString(m_Fill.GetColour()));
		AddPropertyNode(child, wxT("style"), ColourToString(m_Fill.GetStyle()));
		node->AddChild(child);
	}

	return node;
}

void wxSFSolidArrow::Deserialize(wxXmlNode* node)
{
	// HINT: overload it for custom actions...

	wxXmlNode* propNode = node->GetChildren();
	while(propNode)
	{
		if(propNode->GetName() == wxT("fill"))
		{
			wxXmlNode* child = propNode->GetChildren();
			while(child)
			{
				if(child->GetName() == wxT("color"))
				{
					m_Fill.SetColour(StringToColour(child->GetNodeContent()));
				}
				else if(child->GetName() == wxT("style"))
				{
					m_Fill.SetStyle(StringToLong(child->GetNodeContent()));
				}
				child = child->GetNext();
			}
		}
		propNode = propNode->GetNext();
	}
}
