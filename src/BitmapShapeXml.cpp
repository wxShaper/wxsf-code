#include "BitmapShape.h"
#include "CommonFcn.h"

//----------------------------------------------------------------------------------//
// Serialization
//----------------------------------------------------------------------------------//

wxXmlNode* wxSFBitmapShape::Serialize(wxXmlNode* node)
{
	// HINT: overload it for custom actions...

	if(node)
	{
		node = wxSFRectShape::Serialize(node);

		// save bitmap path
		AddPropertyNode(node, wxT("path"), m_sBitmapPath);

		// save 'm_fCanScale' property
		if(m_fCanScale != sfdvBITMAPSHAPE_SCALEIMAGE)
		{
			AddPropertyNode(node, wxT("scale_image"), BoolToString(m_fCanScale));
		}
	}

	return node;
}

void wxSFBitmapShape::Deserialize(wxXmlNode* node)
{
	// HINT: overload it for custom actions...

	wxSFRectShape::Deserialize(node);

	wxRealPoint prevSize = m_nRectSize;

	wxXmlNode* propNode = node->GetChildren();
	while(propNode)
	{
		if(propNode->GetName() == wxT("path"))
		{
			CreateFromFile(propNode->GetNodeContent());
		}
		else if(propNode->GetName() == wxT("scale_image"))
		{
			m_fCanScale = StringToBool(propNode->GetNodeContent());
		}

		propNode = propNode->GetNext();
	}

	if(m_fCanScale)
	{
		if(m_nRectSize != prevSize)
		{
			m_nRectSize = prevSize;
			RescaleImage(prevSize);
		}
		else
			Scale(1, 1);
	}
}
