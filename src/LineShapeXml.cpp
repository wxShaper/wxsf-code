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
		wxXmlNode* child;

		node = wxSFShapeBase::Serialize(node);

		// source shape ID
		if(m_nSrcShapeId != sfdvLINESHAPE_UNKNOWNID)
		{
			AddPropertyNode(node, wxT("source"), LongToString(m_nSrcShapeId));
		}

		// target shape ID
		if(m_nTrgShapeId != sfdvLINESHAPE_UNKNOWNID)
		{
			AddPropertyNode(node, wxT("target"), LongToString(m_nTrgShapeId));
		}

		// dock point
		if(m_nDockPoint != sfdvLINESHAPE_DOCKPOINT)
		{
			AddPropertyNode(node, wxT("dock_point"), LongToString(m_nDockPoint));
		}

		// save control points if exist
		if(m_lstPoints.GetCount() > 0)
		{
			child = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("control_points"));
			wxCPointListNode *cnode = m_lstPoints.GetFirst();
			while(cnode)
			{
				AddPropertyNode(child, wxT("point"), RealPointToString(*cnode->GetData()));
				cnode = cnode->GetNext();
			}
			node->AddChild(child);
		}

		// source arrow
		if(m_pSrcArrow)
		{
			child = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("source_arrow"));
			child->AddProperty(wxT("type"), m_pSrcArrow->GetClassInfo()->GetClassName());
			child = m_pSrcArrow->Serialize(child);
			node->AddChild(child);
		}

		// target arrow
		if(m_pTrgArrow)
		{
			child = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("target_arrow"));
			child->AddProperty(wxT("type"), m_pTrgArrow->GetClassInfo()->GetClassName());
			child = m_pTrgArrow->Serialize(child);
			node->AddChild(child);
		}			

		// line style
		if(m_Pen != sfdvLINESHAPE_PEN)
		{
			AddPropertyNode(node, wxT("line_style"), PenToString(m_Pen));
		}
	}

	return node;
}

void wxSFLineShape::Deserialize(wxXmlNode* node)
{
	wxSFArrowBase* pArrow = NULL;

	wxSFShapeBase::Deserialize(node);

	wxXmlNode *child, *propNode = node->GetChildren();
	while(propNode)
	{
		if(propNode->GetName() == wxT("source"))
		{
			m_nSrcShapeId = StringToLong(propNode->GetNodeContent());
		}
		else if(propNode->GetName() == wxT("target"))
		{
			m_nTrgShapeId = StringToLong(propNode->GetNodeContent());
		}
		else if(propNode->GetName() == wxT("dock_point"))
		{
			m_nDockPoint = StringToLong(propNode->GetNodeContent());
		}
		else if(propNode->GetName() == wxT("control_points"))
		{
			child = propNode->GetChildren();
			while(child)
			{
				if(child->GetName() == wxT("point"))
				{
					m_lstPoints.Append(new wxRealPoint(StringToRealPoint(child->GetNodeContent())));
				}
				child = child->GetNext();
			}
		}
		else if(propNode->GetName() == wxT("source_arrow"))
		{
			pArrow = (wxSFArrowBase*)wxCreateDynamicObject(propNode->GetPropVal(wxT("type"), wxT("")));
			if(pArrow)
			{
				SetSrcArrow(pArrow);
				pArrow->Deserialize(propNode);
			}
		}
		else if(propNode->GetName() == wxT("target_arrow"))
		{
			pArrow = (wxSFArrowBase*)wxCreateDynamicObject(propNode->GetPropVal(wxT("type"), wxT("")));
			if(pArrow)
			{
				SetTrgArrow(pArrow);
				pArrow->Deserialize(propNode);
			}			
		}
		else if(propNode->GetName() == wxT("line_style"))
		{
			m_Pen = StringToPen(propNode->GetNodeContent());
		}

		propNode = propNode->GetNext();
	}
}
