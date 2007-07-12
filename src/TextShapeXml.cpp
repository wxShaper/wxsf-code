#include "TextShape.h"
#include "CommonFcn.h"

//----------------------------------------------------------------------------------//
// Serialization
//----------------------------------------------------------------------------------//

wxXmlNode * wxSFTextShape::Serialize(wxXmlNode* node)
{
	// HINT: overload it for custom actions...

	if(node)
	{
		node = wxSFShapeBase::Serialize(node);

		// save 'm_sText' property
		AddPropertyNode(node, wxT("text"), m_sText);

		// save 'm_Font' property
		AddPropertyNode(node, wxT("font"), FontToString(m_Font));

		// save 'm_TextColor' property
		if(m_TextColor != sfdvTEXTSHAPE_TEXTCOLOR)
		{
            AddPropertyNode(node, wxT("color"), ColourToString(m_TextColor));
		}
	}

	return node;
}

void wxSFTextShape::Deserialize(wxXmlNode* node)
{
	// HINT: overload it for custom actions...

	wxSFShapeBase::Deserialize(node);

	wxXmlNode* propNode = node->GetChildren();
	while(propNode)
	{
		if(propNode->GetName() == wxT("text"))
		{
			m_sText = propNode->GetNodeContent();
		}
		else if(propNode->GetName() == wxT("font"))
		{
			m_Font = StringToFont(propNode->GetNodeContent());
		}
        else if(propNode->GetName() == wxT("color"))
		{
		    m_TextColor = StringToColour(propNode->GetNodeContent());
		}

		propNode = propNode->GetNext();
	}

	UpdateRectSize();
}


