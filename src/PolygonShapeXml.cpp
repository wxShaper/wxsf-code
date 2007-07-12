#include "PolygonShape.h"
#include "CommonFcn.h"

//----------------------------------------------------------------------------------//
// Serialization
//----------------------------------------------------------------------------------//

wxXmlNode* wxSFPolygonShape::Serialize(wxXmlNode* node)
{
	// HINT: overload it for custom actions...

	if(node)
	{
		node = wxSFRectShape::Serialize(node);

		if(m_fConnectToVertex != sfdvPOLYGONSHAPE_VERTEXCONNECTIONS)
		{
			AddPropertyNode(node, wxT("connect_to_vertex"), BoolToString(m_fConnectToVertex));
		}

		if((m_nSerializeMask & sfsfPOLYGONSHAPE_VERTICES) && (m_arrVertices.Count() > 0))
		{
			wxXmlNode* child = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("vertices"));
			for(size_t i = 0; i < m_arrVertices.Count(); i++)
			{
				AddPropertyNode(child, wxT("point"), RealPointToString(m_arrVertices[i]));
			}
			node->AddChild(child);
		}
	}

	return node;
}

void wxSFPolygonShape::Deserialize(wxXmlNode* node)
{
	// HINT: overload it for custom actions...

	wxSFRectShape::Deserialize(node);

	wxXmlNode* propNode = node->GetChildren();
	while(propNode)
	{
		if(propNode->GetName() == wxT("connect_to_vertex"))
		{
			m_fConnectToVertex = StringToBool(propNode->GetNodeContent());
		}
		else if(propNode->GetName() == wxT("vertices"))
		{
			m_arrVertices.Clear();

			wxXmlNode* child = propNode->GetChildren();
			while(child)
			{
				if(child->GetName() == wxT("point"))
				{
					m_arrVertices.Add(StringToRealPoint(child->GetNodeContent()));
				}
				child = child->GetNext();
			}

		}

		propNode = propNode->GetNext();
	}

	NormalizeVertices();
	FitVerticesToBoundingBox();
}
