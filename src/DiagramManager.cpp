/***************************************************************
 * Name:      DiagramManager.cpp
 * Purpose:   Implements shape manager class
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-25
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include <wx/wfstream.h>
#include <wx/mstream.h>
#include <wx/listimpl.cpp>

#include "DiagramManager.h"
#include "ShapeCanvas.h"

WX_DEFINE_LIST(CIDList);

IMPLEMENT_DYNAMIC_CLASS(wxSFDiagramManager, wxXmlSerializer);

wxSFDiagramManager::wxSFDiagramManager()
{
    m_pShapeCanvas = NULL;
    m_lstIDPairs.DeleteContents(true);

    m_sVersion =  wxT("1.3.0 alpha");

    SetSerializerOwner(wxT("wxShapeFramework"));
    SetSerializerVersion(wxT("1.0"));
    SetSerializerRootName(wxT("chart"));
}

wxSFDiagramManager::~wxSFDiagramManager()
{
    Clear();
}

//----------------------------------------------------------------------------------//
// Adding/removing shapes functions
//----------------------------------------------------------------------------------//

wxSFShapeBase* wxSFDiagramManager::AddShape(wxClassInfo* shapeInfo, bool saveState)
{
    wxPoint shapePos;

    if(m_pShapeCanvas)
    {
        wxRect crect = m_pShapeCanvas->GetClientRect();
        shapePos = wxPoint((crect.GetRight() - crect.GetLeft())/2,
                (crect.GetBottom() - crect.GetTop())/2);
    }

	wxSFShapeBase* pShape = AddShape(shapeInfo, shapePos, saveState);

	return pShape;
}

wxSFShapeBase* wxSFDiagramManager::AddShape(wxClassInfo* shapeInfo, const wxPoint& pos, bool saveState)
{
    if( IsShapeAccepted(shapeInfo->GetClassName()) )
    {
        // create shape object from class info
        wxSFShapeBase* pShape = (wxSFShapeBase*)shapeInfo->CreateObject();

        pShape = AddShape(pShape, GetRootItem(), pos, true, saveState);
        if(pShape)pShape->Refresh();

        return pShape;
    }
    else
        return NULL;
}

wxSFShapeBase* wxSFDiagramManager::AddShape(wxSFShapeBase* shape, xsSerializable* parent, const wxPoint& pos, bool initialize, bool saveState)
{
	if(shape)
	{
		if( shape->IsKindOf(CLASSINFO(wxSFShapeBase)) && IsShapeAccepted(shape->GetClassInfo()->GetClassName()) )
		{
		    shape->SetParentManager(this);

		    if( m_pShapeCanvas )
		    {
                wxPoint newPos = m_pShapeCanvas->FitPositionToGrid(m_pShapeCanvas->DP2LP(pos));
                shape->SetRelativePosition(wxRealPoint(newPos.x, newPos.y));
		    }
		    else
                shape->SetRelativePosition(wxRealPoint(pos.x, pos.y));

            // initialize added shape
			if(initialize)
			{
				shape->CreateHandles();
				if( m_pShapeCanvas )
				{
                    shape->SetHoverColour(m_pShapeCanvas->GetHoverColour());
				}

                if(HasChildren(shape))
                {
                    wxSFShapeBase* pChild;
                    ShapeList lstChildren;

                    // get shape's children (if exist)
                    shape->GetChildren(lstChildren, sfRECURSIVE);
                    // initialize shape's children
                    wxShapeListNode* node = lstChildren.GetFirst();
                    while(node)
                    {
                        pChild = node->GetData();

                        // perform standard initialization
                        pChild->SetParentManager(this);
                        pChild->SetId(GetNewId());
                        pChild->CreateHandles();
                        pChild->Update();
                        if( m_pShapeCanvas )
                        {
                            pChild->SetHoverColour(m_pShapeCanvas->GetHoverColour());
                        }

                        node = node->GetNext();
                    }
                }
			}

            // add parent shape to the manager (serializer)
            if(parent)
            {
                AddItem(parent, shape);
            }
            else
                AddItem(GetRootItem(), shape);

            if( m_pShapeCanvas )
            {
                if( saveState )
                {
                    m_pShapeCanvas->SaveCanvasState();
                }
            }
		}
		else
		{
			wxMessageBox(wxString::Format(wxT("Unable to add '%s' class object to the canvas"), shape->GetClassInfo()->GetClassName()), wxT("ShapeFramework"), wxICON_WARNING);

			delete shape;
			shape = NULL;
		}
	}

	return shape;
}

wxSFShapeBase* wxSFDiagramManager::CreateConnection(long srcId, long trgId, bool saveState)
{
    wxSFShapeBase* pShape = AddShape(CLASSINFO(wxSFLineShape), sfDONT_SAVE_STATE);
    if(pShape)
    {
        wxSFLineShape *pLine = (wxSFLineShape*)pShape;
        pLine->SetSrcShapeId(srcId);
        pLine->SetTrgShapeId(trgId);


        if( m_pShapeCanvas )
        {
            if(saveState)m_pShapeCanvas->SaveCanvasState();
            pLine->Refresh();
        }
    }
    return pShape;
}

void wxSFDiagramManager::RemoveShape(wxSFShapeBase* shape, bool refresh)
{
	if(shape)
	{
        // remove connected lines
		ShapeList m_lstConnections;
		GetAssignedConnections(shape, wxSFShapeBase::lineBOTH, m_lstConnections);

		wxShapeListNode *node = m_lstConnections.GetFirst();
		while(node)
		{
			RemoveShape(node->GetData(), false);
			node = node->GetNext();
		}

        // remove the shape
		RemoveItem(shape);

		if(refresh && m_pShapeCanvas)m_pShapeCanvas->Refresh();
	}
}

void wxSFDiagramManager::RemoveShapes(const ShapeList& selection)
{
    wxSFShapeBase* pShape;
	wxShapeListNode *node = selection.GetFirst();
	while(node)
	{
	    pShape = node->GetData();
	    // one shape can delete also parent or conection shape so it is
	    // important whether double-linked shapes already exist before
	    // their deletion
	    if(GetItem(pShape->GetId()))RemoveShape(pShape, false);
		node = node->GetNext();
	}
}

void wxSFDiagramManager::Clear()
{
	RemoveAll();

    if( m_pShapeCanvas )
    {
        m_pShapeCanvas->GetMultiselectionBox().Show(false);
        m_pShapeCanvas->UpdateVirtualSize();
    }
}

//----------------------------------------------------------------------------------//
// Serialization/deserialization functions
//----------------------------------------------------------------------------------//

void wxSFDiagramManager::SerializeToXml(const wxString& file)
{
    wxXmlSerializer::SerializeToXml(file);
}

void wxSFDiagramManager::SerializeToXml(wxOutputStream& outstream)
{
    wxXmlSerializer::SerializeToXml(outstream);
}

void wxSFDiagramManager::DeserializeFromXml(const wxString& file)
{
	wxFileInputStream instream(file);
	if(instream.IsOk())
	{
        m_pShapeCanvas->ClearCanvasHistory();

		DeserializeFromXml(instream);

        m_pShapeCanvas->SaveCanvasState();
	}
	else
		wxMessageBox(wxT("Unable to initialize input stream."), wxT("ShapeFramework"), wxICON_ERROR);

}

void wxSFDiagramManager::DeserializeFromXml(wxInputStream& instream)
{
	// load an XML file
	try
	{
		wxXmlDocument xmlDoc;
		xmlDoc.Load(instream);

		wxXmlNode* root = xmlDoc.GetRoot();
		if(root && (root->GetName() == wxT("chart")))
		{
			// read shape objects from XML recursively
			DeserializeObjects(NULL, root);
		}
		else
			wxMessageBox(wxT("Unknown file format."), wxT("ShapeFramework"), wxICON_WARNING);
	}
	catch (...)
	{
		wxMessageBox(wxT("Unable to load XML file."), wxT("ShapeFramework"), wxICON_ERROR);
	}
}

void wxSFDiagramManager::DeserializeObjects(xsSerializable* parent, wxXmlNode* node)
{
    // clear list of ID pairs
    m_lstIDPairs.Clear();
    m_lstLinesForUpdate.Clear();

    _DeserializeObjects(parent, node);

    // update IDs in connection lines
    UpdateConnections();

    if( m_pShapeCanvas )
    {
        m_pShapeCanvas->MoveShapesFromNegatives();
        m_pShapeCanvas->UpdateVirtualSize();
    }
}

void wxSFDiagramManager::_DeserializeObjects(xsSerializable* parent, wxXmlNode* node)
{
	wxSFShapeBase *pShape;

	wxXmlNode* shapeNode = node->GetChildren();
	while(shapeNode)
	{
		if(shapeNode->GetName() == wxT("object"))
		{
			pShape = AddShape((wxSFShapeBase*)wxCreateDynamicObject(shapeNode->GetPropVal(wxT("type"), wxT(""))), parent, wxDefaultPosition, true, sfDONT_SAVE_STATE);
			if(pShape)
			{
				// store new assigned ID
				long newId = pShape->GetId();
				pShape->DeserializeObject(shapeNode);

				// update handle in line shapes
				if(pShape->IsKindOf(CLASSINFO(wxSFLineShape)))
				{
					pShape->CreateHandles();
					m_lstLinesForUpdate.Append(pShape);
				}

				// check whether the new ID is duplicated
				if(GetIDCount(pShape->GetId()) > 1)
				{
					// store information about ID's change and re-assign shape's id
					m_lstIDPairs.Append(new CIDPair(pShape->GetId(), newId));
					pShape->SetId(newId);
				}

				// deserialize child objects
				_DeserializeObjects(pShape, shapeNode);
			}
		}
		shapeNode = shapeNode->GetNext();
	}
}

//----------------------------------------------------------------------------------//
// Shape handling functions
//----------------------------------------------------------------------------------//

void wxSFDiagramManager::AcceptShape(const wxString& type)
{
    if(m_arrAcceptedShapes.Index(type) == wxNOT_FOUND)
    {
        m_arrAcceptedShapes.Add(type);
    }
}
bool wxSFDiagramManager::IsShapeAccepted(const wxString& type)
{
    if( m_arrAcceptedShapes.Index(type) != wxNOT_FOUND )return true;
    else if( m_arrAcceptedShapes.Index(wxT("All")) != wxNOT_FOUND )return true;
    else
        return false;
}

int wxSFDiagramManager::GetAssignedConnections(wxSFShapeBase* parent, wxSFShapeBase::CONNECTMODE mode, ShapeList& lines)
{
	wxSFLineShape* pLine;

    ShapeList m_lstLines;
    if(GetShapes(CLASSINFO(wxSFLineShape), m_lstLines))
    {
        wxShapeListNode *node = m_lstLines.GetFirst();
        while(node)
        {
            pLine = (wxSFLineShape*)node->GetData();
            switch(mode)
            {
                case wxSFShapeBase::lineSTARTING:
                    if(pLine->GetSrcShapeId() == parent->GetId())lines.Append(pLine);
                    break;

                case wxSFShapeBase::lineENDING:
                    if(pLine->GetTrgShapeId() == parent->GetId())lines.Append(pLine);
                    break;

                case wxSFShapeBase::lineBOTH:
                    if((pLine->GetSrcShapeId() == parent->GetId())
                    || (pLine->GetTrgShapeId() == parent->GetId()))lines.Append(pLine);
                    break;
            }
            node = node->GetNext();
        }
    }

    return (int)lines.GetCount();
}

int wxSFDiagramManager::GetShapes(wxClassInfo* shapeInfo, ShapeList& shapes)
{
	return GetItems(shapeInfo, (SerializableList&)shapes);;
}

wxSFShapeBase* wxSFDiagramManager::FindShape(long id)
{
    if(id == -1)return NULL;
    else
        return (wxSFShapeBase*)GetItem(id);
}

void wxSFDiagramManager::GetNeighbours(wxSFShapeBase* parent, ShapeList& neighbours, wxSFShapeBase::CONNECTMODE condir, bool direct)
{
    if(parent)
    {
        parent->GetNeighbours(neighbours, condir, direct);
    }
    else
    {
        wxASSERT(GetRootItem());

		wxSFShapeBase* pShape;

		wxSerializableListNode *node = GetRootItem()->GetFirstChildNode();
		while(node)
		{
			pShape = (wxSFShapeBase*)node->GetData();
            pShape->GetNeighbours(neighbours, condir, direct);
			node = node->GetNext();
		}
    }
}

bool wxSFDiagramManager::HasChildren(wxSFShapeBase* parent)
{
	if(parent->GetFirstChildNode())return true;
	else
        return false;
}

void wxSFDiagramManager::UpdateConnections()
{
	wxSFLineShape* pLine;
	CIDPair* pIDPair;

	if(m_lstLinesForUpdate.GetCount() > 0)
	{
	    // check whether line's src and trg shapes realy exists
        wxShapeListNode* node = m_lstLinesForUpdate.GetFirst();
        while(node)
        {
            pLine = (wxSFLineShape*)node->GetData();
            if(!GetItem(pLine->GetSrcShapeId()) || !GetItem(pLine->GetTrgShapeId()))
            {
                m_lstLinesForUpdate.DeleteObject(pLine);
                RemoveItem(pLine);

				node = m_lstLinesForUpdate.GetFirst();
            }
			else
				node = node->GetNext();
        }

        // now check ids
	    wxCIDListNode* idnode = m_lstIDPairs.GetFirst();
	    while(idnode)
	    {
	        pIDPair = idnode->GetData();
	        if(pIDPair->m_nNewID != pIDPair->m_nOldID)
	        {
                wxShapeListNode* node = m_lstLinesForUpdate.GetFirst();
                while(node)
                {
                    pLine = (wxSFLineShape*)node->GetData();
                    if(pLine->GetSrcShapeId() == pIDPair->m_nOldID)pLine->SetSrcShapeId(pIDPair->m_nNewID);
                    if(pLine->GetTrgShapeId() == pIDPair->m_nOldID)pLine->SetTrgShapeId(pIDPair->m_nNewID);
                    node = node->GetNext();
                }
	        }
	        idnode = idnode->GetNext();
	    }
    }

	m_lstIDPairs.Clear();
	m_lstLinesForUpdate.Clear();
}
