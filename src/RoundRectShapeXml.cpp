/***************************************************************
 * Name:      RoundRectShapeXml.cpp
 * Purpose:   Implements rounded rect shape's serialization
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "RoundRectShape.h"
#include "CommonFcn.h"

//----------------------------------------------------------------------------------//
// Serialization
//----------------------------------------------------------------------------------//

wxXmlNode* wxSFRoundRectShape::Serialize(wxXmlNode* node)
{
	// HINT: overload it for custom actions...

	if(node)
	{
		node = wxSFRectShape::Serialize(node);
	}

	return node;
}

void wxSFRoundRectShape::Deserialize(wxXmlNode* node)
{
	// HINT: overload it for custom actions...

	wxSFRectShape::Deserialize(node);
}
