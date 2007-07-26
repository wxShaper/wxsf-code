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

wxSFDiagramManager::wxSFDiagramManager()
{
    m_pShapeCanvas = NULL;
    m_lstIDPairs.DeleteContents(true);

    m_sVersion =  wxT("1.2.0 alpha");
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
    wxASSERT_MSG( m_pShapeCanvas, wxT("Shape canvas is not set properly in diagram manager.") );

	wxRect crect = m_pShapeCanvas->GetClientRect();
	const wxPoint shapePos = wxPoint((crect.GetRight() - crect.GetLeft())/2,
			(crect.GetBottom() - crect.GetTop())/2);

	wxSFShapeBase* pShape = AddShape(shapeInfo, shapePos, saveState);

	return pShape;
}

wxSFShapeBase* wxSFDiagramManager::AddShape(wxClassInfo* shapeInfo, const wxPoint& pos, bool saveState)
{
    if( IsShapeAccepted(shapeInfo->GetClassName()) )
    {
        // create shape object from class info
        wxSFShapeBase* pShape = (wxSFShapeBase*)shapeInfo->CreateObject();

        pShape = AddShape(pShape, pos, true, saveState);

        return pShape;
    }
    else
        return NULL;
}

wxSFShapeBase* wxSFDiagramManager::AddShape(wxSFShapeBase* shape, const wxPoint& pos, bool initialize, bool saveState)
{
	if(shape)
	{
	    wxASSERT_MSG( m_pShapeCanvas, wxT("Shape canvas is not set properly in diagram manager.") );

		if( shape->IsKindOf(CLASSINFO(wxSFShapeBase)) && IsShapeAccepted(shape->GetClassInfo()->GetClassName()) )
		{
			wxPoint newPos = m_pShapeCanvas->FitPositionToGrid(m_pShapeCanvas->DP2LP(pos));
			shape->SetRelativePosition(wxRealPoint(newPos.x, newPos.y));

			if(initialize)
			{
				// set shape's properties
				shape->SetParentCanvas(m_pShapeCanvas);
				shape->SetParentShapeId(-1);
				shape->SetId(GetNewId());
				shape->SetHoverColour(m_pShapeCanvas->GetHoverColour());
				shape->CreateHandles();
			}

			// insert new shape into list at the first position
            m_lstShapes.Insert(shape);

            // store canvas state
            if(saveState)m_pShapeCanvas->SaveCanvasState();

			// repaint canvas
			shape->Refresh();
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
    wxASSERT_MSG( m_pShapeCanvas, wxT("Shape canvas is not set properly in diagram manager.") );

    wxSFShapeBase* pShape = AddShape(CLASSINFO(wxSFLineShape), sfDONT_SAVE_STATE);
    if(pShape)
    {
        wxSFLineShape *pLine = (wxSFLineShape*)pShape;
        pLine->SetSrcShapeId(srcId);
        pLine->SetTrgShapeId(trgId);

		if(saveState)m_pShapeCanvas->SaveCanvasState();

		pLine->Refresh();

    }
    return pShape;
}

void wxSFDiagramManager::RemoveShape(wxSFShapeBase* shape, bool refresh)
{
	if(shape)
	{
	    wxASSERT_MSG( m_pShapeCanvas, wxT("Shape canvas is not set properly in diagram manager.") );

		// delete shape from list
		if(m_lstShapes.DeleteObject(shape))
		{
            // delete the shape
            delete shape;
		}

		if(refresh)m_pShapeCanvas->Refresh();
	}
}

void wxSFDiagramManager::RemoveShapes(const CShapeList& selection)
{
    wxSFShapeBase* pShape;
	wxCShapeListNode *node = selection.GetFirst();
	while(node)
	{
	    pShape = node->GetData();
	    // one shape can delete also parent or conection shape so it is
	    // important whether double-linked shapes already exist before
	    // their deletion
	    if(m_lstShapes.IndexOf(pShape) != wxNOT_FOUND)RemoveShape(pShape, false);
		node = node->GetNext();
	}
}

void wxSFDiagramManager::Clear()
{
    wxASSERT_MSG( m_pShapeCanvas, wxT("Shape canvas is not set properly in diagram manager.") );

	wxCShapeListNode *node = m_lstShapes.GetFirst();
	while(node)
	{
		wxSFShapeBase* pShape = node->GetData();
		if(m_lstShapes.DeleteObject(pShape))delete pShape;

		node = m_lstShapes.GetFirst();
	}

	m_pShapeCanvas->GetMultiselectionBox().Show(false);
	m_pShapeCanvas->UpdateVirtualSize();
}

//----------------------------------------------------------------------------------//
// Serialization/deserialization functions
//----------------------------------------------------------------------------------//

void wxSFDiagramManager::SerializeChartToXml(const wxString& file)
{
	wxFileOutputStream outstream(file);

	if(outstream.IsOk())
	{
		SerializeChartToXml(outstream);
	}
	else
		wxMessageBox(wxT("Unable to initialize output file stream."), wxT("ShapeFramework"), wxICON_ERROR);
}

void wxSFDiagramManager::SerializeChartToXml(wxOutputStream& outstream)
{
	// create root node
	wxXmlNode *root = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("chart"));

	if(root)
	{
		// serialize shapes recursively
		SerializeShapes(NULL, root, serWITHOUT_PARENTS);

		// create XML document
		try
		{
			wxXmlDocument xmlDoc;
			xmlDoc.SetRoot(root);

			xmlDoc.Save(outstream, 2);
		}
		catch (...)
		{
			wxMessageBox(wxT("Unable to save XML document."), wxT("ShapeFramework"), wxICON_ERROR);
		}
	}
}

void wxSFDiagramManager::SerializeShapes(wxSFShapeBase* parent, wxXmlNode* node, bool withparent)
{
	wxXmlNode* shapeNode = NULL;
	wxSFShapeBase* pChild;

	CShapeList lstChildren;
	GetChildren(parent, lstChildren);

	// serialize parent shape
	if(withparent && parent)
	{
		shapeNode = parent->SerializeToXml(NULL);
		if(shapeNode)
		{
			SerializeShapes(parent, shapeNode, serWITHOUT_PARENTS);
			node->AddChild(shapeNode);
		}
	}
	else
	{
		// serialize parent's children
		wxCShapeListNode* snode = lstChildren.GetLast();
		while(snode)
		{
			pChild = snode->GetData();

			shapeNode = pChild->SerializeToXml(NULL);
			if(shapeNode)
			{
				SerializeShapes(pChild, shapeNode, serWITHOUT_PARENTS);
				node->AddChild(shapeNode);
			}

			snode = snode->GetPrevious();
		}
	}
}

void wxSFDiagramManager::DeserializeChartFromXml(const wxString& file)
{
	wxFileInputStream instream(file);
	if(instream.IsOk())
	{
        m_pShapeCanvas->ClearCanvasHistory();

		DeserializeChartFromXml(instream);

        m_pShapeCanvas->SaveCanvasState();
	}
	else
		wxMessageBox(wxT("Unable to initialize input stream."), wxT("ShapeFramework"), wxICON_ERROR);
}

void wxSFDiagramManager::DeserializeChartFromXml(wxInputStream& instream)
{
    wxASSERT_MSG( m_pShapeCanvas, wxT("Shape canvas is not set properly in diagram manager.") );

	// load an XML file
	try
	{
		wxXmlDocument xmlDoc;
		xmlDoc.Load(instream);

		wxXmlNode* root = xmlDoc.GetRoot();
		if(root && (root->GetName() == wxT("chart")))
		{
			// clear list of ID pairs
			m_lstIDPairs.Clear();
			m_lstLinesForUpdate.Clear();

			// read shape objects from XML recursively
			DeserializeShapes(NULL, root);

			// update IDs in connection lines
			UpdateConnections();

			m_pShapeCanvas->MoveShapesFromNegatives();
			m_pShapeCanvas->UpdateVirtualSize();
		}
		else
			wxMessageBox(wxT("Unknown file format."), wxT("ShapeFramework"), wxICON_WARNING);
	}
	catch (...)
	{
		wxMessageBox(wxT("Unable to load XML file."), wxT("ShapeFramework"), wxICON_ERROR);
	}
}

void wxSFDiagramManager::DeserializeShapes(wxSFShapeBase* parent, wxXmlNode* node)
{
	wxSFShapeBase *pShape;

	wxXmlNode* shapeNode = node->GetChildren();
	while(shapeNode)
	{
		if(shapeNode->GetName() == wxT("object"))
		{
			pShape = AddShape((wxSFShapeBase*)wxCreateDynamicObject(shapeNode->GetPropVal(wxT("type"), wxT(""))), wxDefaultPosition, true, sfDONT_SAVE_STATE);
			if(pShape)
			{
				// store new assigned ID
				long newId = pShape->GetId();
				pShape->DeserializeFromXml(shapeNode);

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

				if(parent)pShape->SetParentShapeId(parent->GetId());
				else
					pShape->SetParentShapeId(-1);

				// deserialize child objects
				DeserializeShapes(pShape, shapeNode);
			}
		}
		shapeNode = shapeNode->GetNext();
	}
}

//----------------------------------------------------------------------------------//
// Shape handling functions
//----------------------------------------------------------------------------------//

bool wxSFDiagramManager::IsShapeAccepted(const wxString& type)
{
    if( m_arrAcceptedShapes.Index(type) != wxNOT_FOUND )return true;
    else if( m_arrAcceptedShapes.Index(wxT("All")) != wxNOT_FOUND )return true;
    else
        return false;
}

int wxSFDiagramManager::GetAssignedConnections(wxSFShapeBase* parent, wxSFShapeBase::CONNECTMODE mode, CShapeList& lines)
{
	wxSFLineShape* pLine;

    CShapeList m_lstLines;
    if(GetShapes(CLASSINFO(wxSFLineShape), m_lstLines))
    {
        wxCShapeListNode *node = m_lstLines.GetFirst();
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

int wxSFDiagramManager::GetShapes(wxClassInfo* shapeInfo, CShapeList& shapes)
{
	shapes.Clear();
	wxSFShapeBase* pShape;

	wxCShapeListNode *node = m_lstShapes.GetFirst();
	while(node)
	{
	    pShape = node->GetData();
	    if(pShape->IsKindOf(shapeInfo))shapes.Append(pShape);
	    node = node->GetNext();
	}

	return (int)shapes.GetCount();
}

wxSFShapeBase* wxSFDiagramManager::FindShape(long id)
{
	wxCShapeListNode *node = m_lstShapes.GetFirst();
	while(node)
	{
		if(node->GetData()->GetId() == id)return node->GetData();
		node = node->GetNext();
	}
	return NULL;
}

void wxSFDiagramManager::GetChildren(wxSFShapeBase* parent, CShapeList& children, bool recursive)
{
	if(parent)
	{
		parent->GetChildren(children, recursive);
	}
	else
	{
		wxSFShapeBase* pShape;

		wxCShapeListNode *node = m_lstShapes.GetFirst();
		while(node)
		{
			pShape = node->GetData();
			if(pShape->GetParentShapeId() == -1)
			{
				children.Append(pShape);
				if(recursive)pShape->GetChildren(children, recursive);
			}
			node = node->GetNext();
		}
	}
}

void wxSFDiagramManager::GetNeighbours(wxSFShapeBase* parent, CShapeList& neighbours, wxSFShapeBase::CONNECTMODE condir, bool direct)
{
    if(parent)
    {
        parent->GetNeighbours(neighbours, condir, direct);
    }
    else
    {
		wxSFShapeBase* pShape;

		wxCShapeListNode *node = m_lstShapes.GetFirst();
		while(node)
		{
			pShape = node->GetData();
			if(pShape->GetParentShapeId() == -1)
			{
				pShape->GetNeighbours(neighbours, condir, direct);
			}
			node = node->GetNext();
		}
    }
}

bool wxSFDiagramManager::HasChildren(wxSFShapeBase* parent)
{
	CShapeList children;
	GetChildren(parent, children);
	return !children.IsEmpty();
}

void wxSFDiagramManager::UpdateConnections()
{
	wxSFLineShape* pLine;
	CIDPair* pIDPair;

	if(m_lstLinesForUpdate.GetCount() > 0)
	{
	    wxCIDListNode* idnode = m_lstIDPairs.GetFirst();
	    while(idnode)
	    {
	        pIDPair = idnode->GetData();
	        if(pIDPair->m_nNewID != pIDPair->m_nOldID)
	        {
                wxCShapeListNode* node = m_lstLinesForUpdate.GetFirst();
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

bool wxSFDiagramManager::IsIdUsed(long id)
{
	return (GetIDCount(id) > 0);
}

int wxSFDiagramManager::GetIDCount(long id)
{
	int nCount = 0;

	wxCShapeListNode *node = m_lstShapes.GetFirst();
	while(node)
	{
		if(node->GetData()->GetId() == id)nCount++;
		node = node->GetNext();
	}

	return nCount;
}

long wxSFDiagramManager::GetNewId()
{
	long nId = 1;

	while(IsIdUsed(nId))nId++;

	return nId;
}
