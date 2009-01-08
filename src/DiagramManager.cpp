/***************************************************************
 * Name:      DiagramManager.cpp
 * Purpose:   Implements shape manager class
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-25
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "wx_pch.h"

#ifdef _DEBUG_MSVC
#define new DEBUG_NEW
#endif

#include <wx/wfstream.h>
#include <wx/mstream.h>
#include <wx/listimpl.cpp>

#include "wx/wxsf/DiagramManager.h"
#include "wx/wxsf/ShapeCanvas.h"

#include "wx/wxsf/CommonFcn.h"

WX_DEFINE_LIST(IDList);

XS_IMPLEMENT_CLONABLE_CLASS(wxSFDiagramManager, wxXmlSerializer);

wxSFDiagramManager::wxSFDiagramManager()
{
    m_pShapeCanvas = NULL;
    m_lstIDPairs.DeleteContents(true);

    m_sSFVersion =  wxT("1.7.2 beta");

    SetSerializerOwner(wxT("wxShapeFramework"));
    SetSerializerVersion(wxT("1.0"));
    SetSerializerRootName(wxT("chart"));
}

wxSFDiagramManager::wxSFDiagramManager(const wxSFDiagramManager &obj)
: wxXmlSerializer(obj)
{
	m_pShapeCanvas = NULL;
	m_sSFVersion = obj.m_sSFVersion;

    m_lstIDPairs.DeleteContents(true);
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
        wxSFShapeBase *pShape = (wxSFShapeBase*)shapeInfo->CreateObject();

        wxSFShapeBase *pParentShape = NULL;
		// line shapes can be assigned to root only
		if( !pShape->IsKindOf(CLASSINFO(wxSFLineShape)) ) pParentShape = GetShapeAtPosition( pos );
		
        if( pParentShape && pParentShape->IsChildAccepted(shapeInfo->GetClassName()) )
        {
            pShape = AddShape(pShape, (xsSerializable*)pParentShape, pos - Conv2Point( pParentShape->GetAbsolutePosition() ), sfINITIALIZE, saveState);
        }
        else
            pShape = AddShape(pShape, GetRootItem(), pos, sfINITIALIZE, saveState);


		if( pParentShape )pParentShape->Update();

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
		    if( m_pShapeCanvas )
		    {
                wxPoint newPos = m_pShapeCanvas->FitPositionToGrid(m_pShapeCanvas->DP2LP(pos));
                shape->SetRelativePosition(wxRealPoint(newPos.x, newPos.y));
		    }
		    else
                shape->SetRelativePosition(wxRealPoint(pos.x, pos.y));

            // add parent shape to the data manager (serializer)
            if(parent)
            {
                AddItem(parent, shape);
            }
            else
                AddItem(GetRootItem(), shape);

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
                    shape->GetChildShapes(sfANY, lstChildren, sfRECURSIVE);
                    // initialize shape's children
                    ShapeList::compatibility_iterator node = lstChildren.GetFirst();
                    while(node)
                    {
                        pChild = (wxSFShapeBase*)node->GetData();

                        // perform standard initialization
                        pChild->SetParentManager(this);
                        if( pChild->GetId() == -1 ) pChild->SetId( GetNewId() );
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
			//wxMessageBox(wxString::Format(wxT("Unable to add '%s' class object to the canvas"), shape->GetClassInfo()->GetClassName()), wxT("ShapeFramework"), wxICON_WARNING);

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
	    wxSFShapeBase *pParent = shape->GetParentShape();

        // remove connected lines (to all children)
        ShapeList lstChildren;
        ShapeList lstConnections;
        ShapeList lstRemovedConnections;

        // get all shape's children
        shape->GetChildShapes(sfANY, lstChildren, sfRECURSIVE);
        lstChildren.Append(shape);

        // retrieve all assigned lines
        ShapeList::compatibility_iterator snode = lstChildren.GetFirst();
        while(snode)
        {
            GetAssignedConnections(snode->GetData(), CLASSINFO(wxSFLineShape), wxSFShapeBase::lineBOTH, lstConnections);
            snode = snode->GetNext();
        }

		// remove all assigne lines
		ShapeList::compatibility_iterator node = lstConnections.GetFirst();
		while(node)
		{
		    // one connection may be used by the parent and also by his child
		    if(lstRemovedConnections.IndexOf(node->GetData()) == wxNOT_FOUND)
		    {
		        lstRemovedConnections.Append(node->GetData());
                RemoveShape(node->GetData(), false);
		    }
			node = node->GetNext();
		}

		// remove the shape also from m_lstCurrentShapes list
		if( m_pShapeCanvas ) m_pShapeCanvas->RemoveFromTemporaries( shape );
		
        // remove the shape
		RemoveItem(shape);
		
        if( pParent ) pParent->Update();

		if( refresh && m_pShapeCanvas ) m_pShapeCanvas->Refresh();
	}
}

void wxSFDiagramManager::RemoveShapes(const ShapeList& selection)
{
    wxSFShapeBase* pShape;
	ShapeList::compatibility_iterator node = selection.GetFirst();
	while(node)
	{
	    pShape = node->GetData();
	    // it is important to check whether double-linked shapes already exist before
	    // they are deleted
	    if(Contains(pShape))RemoveShape(pShape, false);
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
        //m_pShapeCanvas->MoveShapesFromNegatives();
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
			pShape = AddShape((wxSFShapeBase*)wxCreateDynamicObject(shapeNode->GetPropVal(wxT("type"), wxT(""))), parent, wxPoint(0, 0), true, sfDONT_SAVE_STATE);
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
					m_lstIDPairs.Append(new IDPair(pShape->GetId(), newId));
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

void wxSFDiagramManager::GetAssignedConnections(wxSFShapeBase* parent, wxClassInfo* shapeInfo, wxSFShapeBase::CONNECTMODE mode, ShapeList& lines)
{
	wxSFLineShape* pLine;

	SerializableList lstLines;
	// lines are children of root item only so we have not to search recursively...
	GetRootItem()->GetChildren( shapeInfo, lstLines );
	
	if( !lstLines.IsEmpty() )
    {
        SerializableList::compatibility_iterator node = lstLines.GetFirst();
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
}

void wxSFDiagramManager::GetShapes(wxClassInfo* shapeInfo, ShapeList& shapes, xsSerializable::SEARCHMODE mode)
{
    GetItems(shapeInfo, (SerializableList&)shapes, mode);
}

wxSFShapeBase* wxSFDiagramManager::GetShapeAtPosition(const wxPoint& pos, int zorder, SEARCHMODE mode)
{
	int nCounter = 0;
	ShapeList m_lstSortedShapes;
	wxSFShapeBase* pShape;

    // sort shapes list in the way that the line shapes will be at the top of the list
    ShapeList shapes;
    GetShapes(CLASSINFO(wxSFShapeBase), shapes, xsSerializable::searchBFS);

	ShapeList::compatibility_iterator node = shapes.GetFirst();
	while(node)
	{
	    pShape = node->GetData();
	    if(pShape->IsKindOf(CLASSINFO(wxSFLineShape)))
	    {
	        m_lstSortedShapes.Insert(pShape);
	        nCounter++;
	    }
	    else
            m_lstSortedShapes.Insert(nCounter, pShape);

        node = node->GetNext();
	}

    // find the topmost shape according to the given rules
    nCounter = 1;
	node = m_lstSortedShapes.GetFirst();
	while(node)
	{
		pShape = (wxSFShapeBase*)node->GetData();
		if(pShape->IsVisible() && pShape->IsActive() && pShape->IsInside(pos))
		{
			switch(mode)
			{
			case searchSELECTED:
				if(pShape->IsSelected())
				{
					if(nCounter == zorder)return pShape;
					else
						nCounter++;
				}
				break;

			case searchUNSELECTED:
				if(!pShape->IsSelected())
				{
					if(nCounter == zorder)return pShape;
					else
						nCounter++;
				}
				break;

			case searchBOTH:
				if(nCounter == zorder)return pShape;
				else
					nCounter++;
				break;
			}
		}
		node = node->GetNext();
	}

	return NULL;
}

void wxSFDiagramManager::GetShapesAtPosition(const wxPoint& pos, ShapeList& shapes)
{
	shapes.Clear();
	wxSFShapeBase *pShape;

    ShapeList lstShapes;
    GetShapes(CLASSINFO(wxSFShapeBase), lstShapes);

	ShapeList::compatibility_iterator node = lstShapes.GetFirst();
	while(node)
	{
		pShape = node->GetData();
		if(pShape->IsVisible() && pShape->IsActive() && pShape->IsInside(pos))shapes.Append(pShape);
		node = node->GetNext();
	}
}

void wxSFDiagramManager::GetShapesInside(const wxRect& rct, ShapeList& shapes)
{
	shapes.Clear();
	wxSFShapeBase* pShape;

    ShapeList lstShapes;
    GetShapes(CLASSINFO(wxSFShapeBase), lstShapes);

	ShapeList::compatibility_iterator node = lstShapes.GetFirst();
	while(node)
	{
		pShape = node->GetData();
		if(pShape->IsVisible() && pShape->IsActive() && pShape->Intersects(rct))shapes.Append(pShape);
		node = node->GetNext();
	}
}

wxSFShapeBase* wxSFDiagramManager::FindShape(long id)
{
    if(id == -1)return NULL;
    else
        return (wxSFShapeBase*)GetItem(id);
}

void wxSFDiagramManager::GetNeighbours(wxSFShapeBase* parent, ShapeList& neighbours, wxClassInfo *shapeInfo, wxSFShapeBase::CONNECTMODE condir, bool direct)
{
    if(parent)
    {
        parent->GetNeighbours(neighbours, shapeInfo, condir, direct);
    }
    else
    {
        wxASSERT(GetRootItem());

		wxSFShapeBase* pShape;

		SerializableList::compatibility_iterator node = GetRootItem()->GetFirstChildNode();
		while(node)
		{
			pShape = (wxSFShapeBase*)node->GetData();
            pShape->GetNeighbours(neighbours, shapeInfo, condir, direct);
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
	IDPair* pIDPair;

	if( !m_lstLinesForUpdate.IsEmpty() )
	{
	    // check whether line's src and trg shapes realy exists
        ShapeList::compatibility_iterator node = m_lstLinesForUpdate.GetFirst();
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
	    IDList::compatibility_iterator idnode = m_lstIDPairs.GetFirst();
	    while(idnode)
	    {
	        pIDPair = idnode->GetData();
	        if(pIDPair->m_nNewID != pIDPair->m_nOldID)
	        {
                node = m_lstLinesForUpdate.GetFirst();
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
