/***************************************************************
 * Name:      LineShapeXml.cpp
 * Purpose:   Implements line shape's serialization capability
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "LineShape.h"
#include "CommonFcn.h"

//----------------------------------------------------------------------------------//
// Serialization functions
//----------------------------------------------------------------------------------//

wxXmlNode* wxSFLineShape::Serialize(wxXmlNode* node)
{
	// HINT: overload it for custom actions...

	if(node)
	{
		node = wxSFShapeBase::Serialize(node);
	}

	return node;
}

void wxSFLineShape::Deserialize(wxXmlNode* node)
{
	wxSFShapeBase::Deserialize(node);

}
