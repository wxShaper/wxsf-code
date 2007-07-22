/***************************************************************
 * Name:      CurveShapeXml.cpp
 * Purpose:   Implements curve shape's serialization capability
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "CurveShape.h"
#include "CommonFcn.h"

//----------------------------------------------------------------------------------//
// Serialization functions
//----------------------------------------------------------------------------------//

wxXmlNode* wxSFCurveShape::Serialize(wxXmlNode* node)
{
    // HINT: overload it for custom actions...

	if(node)
	{
		node = wxSFLineShape::Serialize(node);

		// save 'm_nMaxSteps'
		if(m_nMaxSteps != sfdvCURVESHAPE_MAXSTEPS)
		{
		    AddPropertyNode(node, wxT("max_steps"), LongToString((long)m_nMaxSteps));
		}
	}

	return node;
}

void wxSFCurveShape::Deserialize(wxXmlNode* node)
{
    // HINT: overload it for custom actions...

    wxSFLineShape::Deserialize(node);

	wxXmlNode *propNode = node->GetChildren();
	while(propNode)
	{
	    if(propNode->GetName() == wxT("max_steps"))
	    {
	        m_nMaxSteps = StringToLong(node->GetNodeContent());
	    }
	    propNode = propNode->GetNext();
	}
}
