/***************************************************************
 * Name:      ShapeBaseXml.cpp
 * Purpose:   Implements basic shape's serialization capability
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "ShapeBase.h"
#include "CommonFcn.h"

//----------------------------------------------------------------------------------//
// Serialization
//----------------------------------------------------------------------------------//

wxXmlNode* wxSFShapeBase::Serialize(wxXmlNode* node)
{
	// HINT: overload it for custom actions...

	node = xsSerializable::Serialize(node);

	return node;
}

void wxSFShapeBase::Deserialize(wxXmlNode* node)
{
	// HINT: overload it for custom actions...

	xsSerializable::Deserialize(node);

	if( m_pUserData )
	{
	    m_pUserData->SetParent(this);
	}
}
