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

	if(node)
	{
		wxString parValue;
		wxXmlNode* child;

		// store serialization mask
		node->AddProperty(wxT("id"), LongToString(m_nId));

		// store "m_nId" property
		node->AddProperty(wxT("serialize"), LongToString(m_nSerializeMask));

		// store "m_fActive" property
		if(m_fActive != sfdvBASESHAPE_ACTIVITY)
		{
			AddPropertyNode(node, wxT("active"), BoolToString(m_fActive));
		}

		// store "m_fVisible" property
		if(m_fVisible != sfdvBASESHAPE_VISIBILITY)
		{
			AddPropertyNode(node, wxT("visible"), BoolToString(m_fVisible));
		}

		// store "m_fParentChange" property
		if(m_fParentChange != sfdvBASESHAPE_PARENTCHANGE)
		{
			AddPropertyNode(node, wxT("parent_change"), BoolToString(m_fParentChange));
		}

		// store "m_fSizeChange" property
		if(m_fSizeChange != sfdvBASESHAPE_SIZECHANGE)
		{
			AddPropertyNode(node, wxT("size_change"), BoolToString(m_fSizeChange));
		}

        // store "m_fPositionChange" property
		if(m_fPositionChange != sfdvBASESHAPE_POSITIONCHANGE)
		{
			AddPropertyNode(node, wxT("position_change"), BoolToString(m_fPositionChange));
		}

		// store "m_fHighlighting" property
		if(m_fHighlighting != sfdvBASESHAPE_HIGHLIGHTING)
		{
			AddPropertyNode(node, wxT("highlighting"), BoolToString(m_fHighlighting));
		}

		// store "m_fHovering" property
		if(m_fHovering != sfdvBASESHAPE_HOVERING)
		{
			AddPropertyNode(node, wxT("hovering"), BoolToString(m_fHovering));
		}

        if(!m_arrAcceptedChildren.IsEmpty() || !m_arrAcceptedConnections.IsEmpty() ||
            !m_arrAcceptedSrcNeighbours.IsEmpty() || !m_arrAcceptedTrgNeighbours.IsEmpty())
        {
            wxXmlNode* policy = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("policy"));

            // store accepted object types
            if(m_nSerializeMask & sfsfBASESHAPE_ACCEPTEDSHAPES)
            {
                if(!m_arrAcceptedChildren.IsEmpty())
                {
                    child = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("accepted_children"));
                    for(size_t i = 0; i < m_arrAcceptedChildren.Count(); i++)
                    {
                        AddPropertyNode(child, wxT("type"), m_arrAcceptedChildren.Item(i));
                    }
                    policy->AddChild(child);
                }

                if(!m_arrAcceptedSrcNeighbours.IsEmpty())
                {
                    child = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("accepted_src_neighbours"));
                    for(size_t i = 0; i < m_arrAcceptedSrcNeighbours.Count(); i++)
                    {
                        AddPropertyNode(child, wxT("type"), m_arrAcceptedSrcNeighbours.Item(i));
                    }
                    policy->AddChild(child);
                }

                if(!m_arrAcceptedTrgNeighbours.IsEmpty())
                {
                    child = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("accepted_trg_neighbours"));
                    for(size_t i = 0; i < m_arrAcceptedTrgNeighbours.Count(); i++)
                    {
                        AddPropertyNode(child, wxT("type"), m_arrAcceptedTrgNeighbours.Item(i));
                    }
                    policy->AddChild(child);
                }
            }
            // store accepted connections
            if(m_nSerializeMask & sfsfBASESHAPE_ACCEPTEDCONNECTIONS)
            {
                if(!m_arrAcceptedConnections.IsEmpty())
                {
                    child = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("accepted_connections"));
                    for(size_t i = 0; i < m_arrAcceptedConnections.Count(); i++)
                    {
                        AddPropertyNode(child, wxT("type"), m_arrAcceptedConnections.Item(i));
                    }
                    policy->AddChild(child);
                }
            }

            node->AddChild(policy);
        }

		// store "m_nHoverColor" property
		if(m_nHoverColor != sfdvBASESHAPE_HOVERCOLOUR)
		{
			AddPropertyNode(node, wxT("hover_color"), ColourToString(m_nHoverColor));
		}

		// store "m_nRelativePosition" property
		if(m_nSerializeMask & sfsfBASESHAPE_POSITION)
		{
			AddPropertyNode(node, wxT("relative_position"), RealPointToString(m_nRelativePosition));
		}

		// store "m_nVAlign" property
		if(m_nVAlign != sfdvBASESHAPE_VALIGN)
		{
		    AddPropertyNode(node, wxT("valign"), LongToString(m_nVAlign));
		}

		// store "m_nHAlign" property
		if(m_nHAlign != sfdvBASESHAPE_HALIGN)
		{
		    AddPropertyNode(node, wxT("halign"), LongToString(m_nHAlign));
		}

		// store "m_nVBorder" property
		if(m_nVBorder != sfdvBASESHAPE_VBORDER)
		{
		    AddPropertyNode(node, wxT("vborder"), DoubleToString(m_nVBorder));
		}

		// store "m_nHBorder" property
		if(m_nHBorder != sfdvBASESHAPE_HBORDER)
		{
		    AddPropertyNode(node, wxT("hborder"), DoubleToString(m_nHBorder));
		}

		// store handles
		if((m_nSerializeMask & sfsfBASESHAPE_HANDLES) || (m_fSizeChange != sfdvBASESHAPE_SIZECHANGE))
		{
			child = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("handles"), wxT(""));

			if(m_fSizeChange != sfdvBASESHAPE_SIZECHANGE)
			{
				AddPropertyNode(child, wxT("enabled"), BoolToString(m_fSizeChange));
			}

			if(m_nSerializeMask & sfsfBASESHAPE_HANDLES)
			{
				parValue = wxT("");
				wxCHandleListNode* hnode = m_lstHandles.GetFirst();
				while(hnode)
				{
					parValue += wxString::Format(wxT("%d"), (int)hnode->GetData()->GetType());

					hnode = hnode->GetNext();
					if(hnode)parValue += wxT(",");
				}
				AddPropertyNode(child, wxT("types"), parValue);
			}

			node->AddChild(child);
		}
	}

	return node;
}

void wxSFShapeBase::Deserialize(wxXmlNode* node)
{
	// HINT: overload it for custom actions...

	wxString propVal;

	// get object ID from root object node
	node->GetPropVal(wxT("id"), &propVal);
	m_nId = StringToLong(propVal);

	// get object serialization mask from root object node
	node->GetPropVal(wxT("serialize"), &propVal);
	m_nSerializeMask = StringToLong(propVal);

	wxXmlNode *child, *child2, *propNode = node->GetChildren();
	while(propNode)
	{
		if(propNode->GetName() == wxT("visible"))
		{
			m_fVisible = StringToBool(propNode->GetNodeContent());
		}
		else if(propNode->GetName() == wxT("active"))
		{
			m_fActive = StringToBool(propNode->GetNodeContent());
		}
		else if(propNode->GetName() == wxT("parent_change"))
		{
			m_fParentChange = StringToBool(propNode->GetNodeContent());
		}
		else if(propNode->GetName() == wxT("size_change"))
		{
			m_fSizeChange = StringToBool(propNode->GetNodeContent());
		}
		else if(propNode->GetName() == wxT("position_change"))
		{
			m_fPositionChange = StringToBool(propNode->GetNodeContent());
		}
		else if(propNode->GetName() == wxT("highlighting"))
		{
			m_fHighlighting = StringToBool(propNode->GetNodeContent());
		}
		else if(propNode->GetName() == wxT("hovering"))
		{
			m_fHovering = StringToBool(propNode->GetNodeContent());
		}
		else if(propNode->GetName() == wxT("policy"))
		{
		    child2 = propNode->GetChildren();
		    while(child2)
		    {
                if(child2->GetName() == wxT("accepted_children"))
                {
                    child = child2->GetChildren();
                    while(child)
                    {
                        if(child->GetName() == wxT("type"))
                        {
                            m_arrAcceptedChildren.Add(child->GetNodeContent());
                        }
                        child = child->GetNext();
                    }
                }
                else if(child2->GetName() == wxT("accepted_src_neighbours"))
                {
                    child = child2->GetChildren();
                    while(child)
                    {
                        if(child->GetName() == wxT("type"))
                        {
                            m_arrAcceptedSrcNeighbours.Add(child->GetNodeContent());
                        }
                        child = child->GetNext();
                    }
                }
                else if(child2->GetName() == wxT("accepted_trg_neighbours"))
                {
                    child = child2->GetChildren();
                    while(child)
                    {
                        if(child->GetName() == wxT("type"))
                        {
                            m_arrAcceptedTrgNeighbours.Add(child->GetNodeContent());
                        }
                        child = child->GetNext();
                    }
                }
                else if(child2->GetName() == wxT("accepted_connections"))
                {
                    child = child2->GetChildren();
                    while(child)
                    {
                        if(child->GetName() == wxT("type"))
                        {
                            m_arrAcceptedConnections.Add(child->GetNodeContent());
                        }
                        child = child->GetNext();
                    }
                }
                child2 = child2->GetNext();
		    }
		}
		else if(propNode->GetName() == wxT("hover_color"))
		{
			m_nHoverColor = StringToColour(propNode->GetNodeContent());
		}
		else if(propNode->GetName() == wxT("relative_position"))
		{
			m_nRelativePosition = StringToRealPoint(propNode->GetNodeContent());
		}
		else if(propNode->GetName() == wxT("valign"))
		{
			m_nVAlign = (VALIGN)StringToLong(propNode->GetNodeContent());
		}
		else if(propNode->GetName() == wxT("halign"))
		{
			m_nHAlign = (HALIGN)StringToLong(propNode->GetNodeContent());
		}
		else if(propNode->GetName() == wxT("vborder"))
		{
			m_nVBorder = StringToDouble(propNode->GetNodeContent());
		}
		else if(propNode->GetName() == wxT("hborder"))
		{
			m_nHBorder = StringToDouble(propNode->GetNodeContent());
		}
		else if(propNode->GetName() == wxT("handles"))
		{
            child = propNode->GetChildren();
			while(child)
			{
			    if(child->GetName() == wxT("enabled"))
			    {
			        m_fSizeChange = StringToBool(child->GetNodeContent());
			    }
			    else if(child->GetName() == wxT("types"))
			    {
                    wxStringTokenizer tokens(child->GetNodeContent(), wxT(","), wxTOKEN_STRTOK);

                    long val;
                    wxString handle;
                    while(!(handle = tokens.GetNextToken()).IsEmpty())
                    {
                        handle.ToLong(&val);
                        AddHandle((wxSFShapeHandle::HANDLETYPE)val);
                    }
			    }
			    child = child->GetNext();
			}
		}

		propNode = propNode->GetNext();
	}
}
