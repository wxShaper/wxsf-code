/***************************************************************
 * Name:      RectShapeXml.cpp
 * Purpose:   Implements rectangular shape's serialization cap.
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "RectShape.h"
#include "CommonFcn.h"

//----------------------------------------------------------------------------------//
// Serialization
//----------------------------------------------------------------------------------//

wxXmlNode* wxSFRectShape::Serialize(wxXmlNode* node)
{
	// HINT: overload it for custom actions...

	if(node)
	{
		node = wxSFShapeBase::Serialize(node);

		// store "m_nRectSize" property
		if(m_nRectSize != sfdvRECTSHAPE_SIZE)
		{
			AddPropertyNode(node, wxT("size"), RealPointToString(m_nRectSize));
		}

		// store fill type
		if(m_Fill != sfdvRECTSHAPE_FILL)
		{
			AddPropertyNode(node, wxT("fill"), BrushToString(m_Fill));
		}

		// store border type
		if(m_Border != sfdvRECTSHAPE_BORDER)
		{
			AddPropertyNode(node, wxT("border"), PenToString(m_Border));
		}
	}

	return node;
}

void wxSFRectShape::Deserialize(wxXmlNode* node)
{
	// HINT: overload it for custom actions...

	wxSFShapeBase::Deserialize(node);

	wxXmlNode* propNode = node->GetChildren();
	while(propNode)
	{
		if(propNode->GetName() == wxT("size"))
		{
			m_nRectSize = StringToRealPoint(propNode->GetNodeContent());
		}
		else if(propNode->GetName() == wxT("fill"))
		{
			m_Fill = StringToBrush(propNode->GetNodeContent());
		}
		else if(propNode->GetName() == wxT("border"))
		{
			m_Border = StringToPen(propNode->GetNodeContent());
		}
		propNode = propNode->GetNext();
	}
}
