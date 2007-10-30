/***************************************************************
 * Name:      TextShapeXml.cpp
 * Purpose:   Implements text shape's serialization capability
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "wx/wxsf/TextShape.h"
#include "wx/wxsf/CommonFcn.h"

//----------------------------------------------------------------------------------//
// Serialization
//----------------------------------------------------------------------------------//

wxXmlNode * wxSFTextShape::Serialize(wxXmlNode* node)
{
	// HINT: overload it for custom actions...

	if(node)
	{
		node = wxSFShapeBase::Serialize(node);
	}

	return node;
}

void wxSFTextShape::Deserialize(wxXmlNode* node)
{
	// HINT: overload it for custom actions...

	wxSFShapeBase::Deserialize(node);

	UpdateRectSize();
}


