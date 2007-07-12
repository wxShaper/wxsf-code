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

		// store "m_nCornerRadius" property
		if(m_nRadius != sfdvROUNDRECTSHAPE_RADIUS)
		{
			AddPropertyNode(node, wxT("radius"), DoubleToString(m_nRadius));
		}
	}

	return node;
}

void wxSFRoundRectShape::Deserialize(wxXmlNode* node)
{
	// HINT: overload it for custom actions...

	wxSFRectShape::Deserialize(node);

	wxXmlNode* propNode = node->GetChildren();
	while(propNode)
	{
		if(propNode->GetName() == wxT("radius"))
		{
			m_nRadius = StringToDouble(propNode->GetNodeContent());
		}

		propNode = propNode->GetNext();
	}
}
