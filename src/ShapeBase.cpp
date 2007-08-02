/***************************************************************
 * Name:      ShapeBase.h
 * Purpose:   Implements shape base class
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

// TODO: wxSFShapeBase: Implement LockAspectRation() function

#include "ShapeBase.h"
#include "ShapeCanvas.h"
#include "TextShape.h"

#include <wx/listimpl.cpp>

WX_DEFINE_LIST(CShapeList);

CShapeList m_lstProcessed;

IMPLEMENT_DYNAMIC_CLASS(wxSFShapeBase, wxObject);

wxSFShapeBase::wxSFShapeBase(void)
{
	// initialize data members
	m_fSelected = false;
	m_fMouseOver = false;
	m_fFirstMove = false;
	m_fHighlighParent = false;
	m_pParentManager = NULL;
	m_nParentShapeId = -1;
	m_nId=-1;

	// archived properties
	m_fVisible = sfdvBASESHAPE_VISIBILITY;
	m_fActive = sfdvBASESHAPE_ACTIVITY;
	m_fParentChange = sfdvBASESHAPE_PARENTCHANGE;
	m_fSizeChange = sfdvBASESHAPE_SIZECHANGE;
	m_fPositionChange = sfdvBASESHAPE_POSITIONCHANGE;
	m_fHighlighting = sfdvBASESHAPE_HIGHLIGHTING;
	m_fHovering = sfdvBASESHAPE_HOVERING;
	m_fAlwaysInsideParent = sfdvBASESHAPE_ALWAYSINSIDE;

	m_nRelativePosition = sfdvBASESHAPE_POSITION;
	m_nHoverColor = sfdvBASESHAPE_HOVERCOLOUR;
	m_nVAlign = sfdvBASESHAPE_VALIGN;
	m_nHAlign = sfdvBASESHAPE_HALIGN;
	m_nVBorder = sfdvBASESHAPE_VBORDER;
	m_nHBorder = sfdvBASESHAPE_HBORDER;

	m_nSerializeMask = sfsfBASESHAPE_POSITION | sfsfBASESHAPE_ACCEPTEDSHAPES | sfsfBASESHAPE_ACCEPTEDCONNECTIONS;

	m_lstHandles.DeleteContents(true);
}

wxSFShapeBase::wxSFShapeBase(const wxRealPoint& pos, long parentId, wxSFDiagramManager* manager)
{
	wxASSERT_MSG(manager, wxT("Shape manager has not been properly set in shape constructor."));

	// initialize data members
	m_nParentShapeId = parentId;
	m_pParentManager = manager;

	if(m_pParentManager)
	{
		if( m_pParentManager->GetShapeCanvas() )m_nHoverColor = m_pParentManager->GetShapeCanvas()->GetHoverColour();
		else
            m_nHoverColor = sfdvBASESHAPE_HOVERCOLOUR;

		m_nId = m_pParentManager->GetNewId();
	}
	else
	{
		m_nHoverColor = sfdvBASESHAPE_HOVERCOLOUR;
		m_nId = -1;
	}

	m_fSelected = false;
	m_fMouseOver = false;
	m_fFirstMove = false;
	m_fHighlighParent = false;

	// archived properties
	m_fVisible = sfdvBASESHAPE_VISIBILITY;
	m_fActive = sfdvBASESHAPE_ACTIVITY;
	m_fParentChange = sfdvBASESHAPE_PARENTCHANGE;
	m_fSizeChange = sfdvBASESHAPE_SIZECHANGE;
	m_fPositionChange = sfdvBASESHAPE_POSITIONCHANGE;
	m_fHighlighting = sfdvBASESHAPE_HIGHLIGHTING;
	m_fHovering = sfdvBASESHAPE_HOVERING;
	m_fAlwaysInsideParent = sfdvBASESHAPE_ALWAYSINSIDE;

	m_nVAlign = sfdvBASESHAPE_VALIGN;
	m_nHAlign = sfdvBASESHAPE_HALIGN;
	m_nVBorder = sfdvBASESHAPE_VBORDER;
	m_nHBorder = sfdvBASESHAPE_HBORDER;

	wxSFShapeBase* m_pParentShape = GetParentShape();
	if(m_pParentShape)m_nRelativePosition = pos - m_pParentShape->GetAbsolutePosition();
	else
		m_nRelativePosition = sfdvBASESHAPE_POSITION;

	m_nSerializeMask = sfsfBASESHAPE_POSITION | sfsfBASESHAPE_ACCEPTEDSHAPES | sfsfBASESHAPE_ACCEPTEDCONNECTIONS;

	m_lstHandles.DeleteContents(true);
}

wxSFShapeBase::wxSFShapeBase(wxSFShapeBase& obj)
{
	// initialize data members
	m_fSelected = obj.m_fSelected;
	m_fMouseOver = obj.m_fMouseOver;
	m_fFirstMove = obj.m_fFirstMove;
	m_fVisible = obj.m_fVisible;
	m_fActive = obj.m_fActive;
	m_fHighlighParent = obj.m_fHighlighParent;
	m_fParentChange = obj.m_fParentChange;
	m_fSizeChange = obj.m_fSizeChange;
	m_fPositionChange = obj.m_fPositionChange;
	m_fHighlighting = obj.m_fHighlighting;
	m_fHovering = obj.m_fHovering;
	m_fAlwaysInsideParent = obj.m_fAlwaysInsideParent;

	m_nVAlign = obj.m_nVAlign;
	m_nHAlign = obj.m_nHAlign;
	m_nVBorder = obj.m_nVBorder;
	m_nHBorder = obj.m_nHBorder;

	WX_APPEND_ARRAY(m_arrAcceptedChildren, obj.m_arrAcceptedChildren);
	WX_APPEND_ARRAY(m_arrAcceptedSrcNeighbours, obj.m_arrAcceptedSrcNeighbours);
	WX_APPEND_ARRAY(m_arrAcceptedTrgNeighbours, obj.m_arrAcceptedTrgNeighbours);
	WX_APPEND_ARRAY(m_arrAcceptedConnections, obj.m_arrAcceptedConnections);

	m_nHoverColor = obj.m_nHoverColor;

	m_nRelativePosition = obj.m_nRelativePosition;

	m_pParentManager = obj.m_pParentManager;
	m_nParentShapeId = obj.m_nParentShapeId;

	m_nId = obj.m_nId;

	m_nSerializeMask = obj.m_nSerializeMask;

    m_lstHandles.DeleteContents(true);
}

wxSFShapeBase::~wxSFShapeBase(void)
{
    wxASSERT(m_pParentManager);

	// clear handles
	m_lstHandles.Clear();

	// remove children and connected lines
	if(m_pParentManager)
	{
		CShapeList m_lstChildren;
		GetChildren(m_lstChildren);
		m_pParentManager->GetAssignedConnections(this, lineBOTH, m_lstChildren);

		wxCShapeListNode *node = m_lstChildren.GetFirst();
		while(node)
		{
			wxSFShapeBase *pShape = node->GetData();

			if(m_pParentManager->GetShapeList().DeleteObject(pShape))delete pShape;

			node = node->GetNext();
		}
	}
}


//----------------------------------------------------------------------------------//
// Public functions
//----------------------------------------------------------------------------------//

wxRect wxSFShapeBase::GetBoundingBox()
{
	// HINT: overload it for custom actions...

	return wxRect();
}

void wxSFShapeBase::GetCompleteBoundingBox(wxRect &rct, int mask)
{
    wxASSERT(m_pParentManager);
    if(!m_pParentManager)return;

	CShapeList lstChildren;
	CShapeList lstConnections;

	// firts, get bounding box of the current shape
	if(mask & bbSELF)
	{
		if(rct.IsEmpty())rct = this->GetBoundingBox();
		else
			rct.Union(this->GetBoundingBox());
	}
	else
		mask |= bbSELF;

	// get list of all connection lines assigned to the shape and find their child shapes
	if(mask & bbCONNECTIONS)
	{
		wxSFShapeBase *pLine;

        CShapeList lstLines;
        m_pParentManager->GetAssignedConnections(this, lineBOTH, lstLines);

		wxCShapeListNode* node = lstLines.GetFirst();
		while(node)
		{
			pLine = node->GetData();

			rct.Union(pLine->GetBoundingBox());

			// get also BB of connections assigned to this connection
			lstConnections.Clear();
			m_pParentManager->GetAssignedConnections(pLine, lineBOTH, lstConnections);

            wxCShapeListNode* cnode = lstConnections.GetFirst();
            while(cnode)
            {
                rct.Union(cnode->GetData()->GetBoundingBox());
                cnode = cnode->GetNext();
            }

			node = node->GetNext();
		}
	}

	// get children of this shape
	if(mask & bbCHILDREN)
	{
		this->GetChildren(lstChildren);

		// now, call this function for all children recursively...
		wxCShapeListNode* node = lstChildren.GetFirst();
		while(node)
		{
		    node->GetData()->GetCompleteBoundingBox(rct, mask);
			node = node->GetNext();
		}
	}
}

bool wxSFShapeBase::IsInside(const wxPoint& pos)
{
	// HINT: overload it for custom actions...

	return false;
}

bool wxSFShapeBase::Intersects(const wxRect& rct)
{
	// HINT: overload it for custom actions...

	return false;
}

wxRealPoint wxSFShapeBase::GetAbsolutePosition()
{
	// HINT: overload it for custom actions...

	wxSFShapeBase* m_pParentShape = GetParentShape();
	if(m_pParentShape)
	{
		return m_nRelativePosition + m_pParentShape->GetAbsolutePosition();
	}
	else
		return m_nRelativePosition;
}

wxRealPoint wxSFShapeBase::GetCenter()
{
    // HINT: override it for custom actions

    wxRect shpBB = this->GetBoundingBox();
    return wxRealPoint(shpBB.GetLeft() + shpBB.GetWidth()/2, shpBB.GetTop() + shpBB.GetHeight()/2);
}

wxRealPoint wxSFShapeBase::GetBorderPoint(const wxRealPoint& to)
{
    // HINT: override it for custom actions

    return wxRealPoint();
}

void wxSFShapeBase::ShowHandles(bool show)
{
	wxCHandleListNode *node = m_lstHandles.GetFirst();
	while(node)
	{
		node->GetData()->Show(show);
		node = node->GetNext();
	}
}

void wxSFShapeBase::MoveTo(double x, double y)
{
	// HINT: overload it for custom actions...

	wxSFShapeBase* m_pParentShape = GetParentShape();
	if(m_pParentShape)
	{
		m_nRelativePosition = wxRealPoint(x, y) - m_pParentShape->GetAbsolutePosition();
	}
	else
	{
		m_nRelativePosition.x = x;
		m_nRelativePosition.y = y;
	}
}

void wxSFShapeBase::MoveTo(const wxRealPoint& pos)
{
	this->MoveTo(pos.x, pos.y);
}

void wxSFShapeBase::MoveBy(double x, double y)
{
	// HINT: overload it for custom actions...

	m_nRelativePosition.x += x;
	m_nRelativePosition.y += y;
}

void wxSFShapeBase::MoveBy(const wxRealPoint& delta)
{
	this->MoveBy(delta.x, delta.y);
}

void wxSFShapeBase::Scale(double x, double y, bool children)
{
	// HINT: overload it for custom actions...

	if(children)
	{
	    ScaleChildren(x, y);
	}

    Update();
}

void wxSFShapeBase::Scale(const wxRealPoint& scale, bool children)
{
	this->Scale(scale.x, scale.y, children);
}

void wxSFShapeBase::ScaleChildren(double x, double y)
{
	CShapeList m_lstChildren;
	GetChildren(m_lstChildren, sfRECURSIVE);

	wxCShapeListNode *node = m_lstChildren.GetFirst();
	while(node)
	{
		wxSFShapeBase* pShape = node->GetData();

        if(pShape->CanChangeSize() && !pShape->IsKindOf(CLASSINFO(wxSFTextShape)))
		{
		    pShape->Scale(x, y, sfWITHOUTCHILDREN);
		}

		if( (pShape->GetVAlign() == valignNONE) || (pShape->GetHAlign() == halignNONE) )
		{
            pShape->SetRelativePosition(pShape->GetRelativePosition().x*x, pShape->GetRelativePosition().y*y);
		}

        // re-align shapes which have set any alignment mode
		pShape->DoAlignment();

		node = node->GetNext();
	}
}

void wxSFShapeBase::Update()
{
    // fit the shape to its children
    this->FitToChildren();

    // do self-alignment
    DoAlignment();

    // do alignment of shape's children (if required)
    CShapeList lstChildren;
    GetChildren(lstChildren, sfNORECURSIVE);

    wxCShapeListNode* node = lstChildren.GetFirst();
    while(node)
    {
        node->GetData()->DoAlignment();
        node = node->GetNext();
    }
}

bool wxSFShapeBase::AcceptCurrentlyDraggedShapes()
{
    wxASSERT(m_pParentManager);
    wxASSERT(m_pParentManager->GetShapeCanvas());

    if(!m_pParentManager || !m_pParentManager->GetShapeCanvas())return false;

    if(!IsChildAccepted(wxT("All")))
    {
        CShapeList lstSelection;
        m_pParentManager->GetShapeCanvas()->GetSelectedShapes(lstSelection);

        wxCShapeListNode* node = lstSelection.GetFirst();
        while(node)
        {
            if(m_arrAcceptedChildren.Index(node->GetData()->GetClassInfo()->GetClassName()) == wxNOT_FOUND)return false;
            node = node->GetNext();
        }
    }
	return true;
}

bool wxSFShapeBase::IsChildAccepted(const wxString& type)
{
    if( m_arrAcceptedChildren.Index(type) != wxNOT_FOUND )return true;
    else if( m_arrAcceptedChildren.Index(wxT("All")) != wxNOT_FOUND )return true;
    else
        return false;
}

bool wxSFShapeBase::IsSrcNeighbourAccepted(const wxString& type)
{
    if( m_arrAcceptedSrcNeighbours.Index(type) != wxNOT_FOUND )return true;
    else if( m_arrAcceptedSrcNeighbours.Index(wxT("All")) != wxNOT_FOUND )return true;
    else
        return false;
}

bool wxSFShapeBase::IsTrgNeighbourAccepted(const wxString& type)
{
    if( m_arrAcceptedTrgNeighbours.Index(type) != wxNOT_FOUND )return true;
    else if( m_arrAcceptedTrgNeighbours.Index(wxT("All")) != wxNOT_FOUND )return true;
    else
        return false;
}

bool wxSFShapeBase::IsConnectionAccepted(const wxString& type)
{
    if( m_arrAcceptedConnections.Index(type) != wxNOT_FOUND )return true;
    else if( m_arrAcceptedConnections.Index(wxT("All")) != wxNOT_FOUND )return true;
    else
        return false;
}

void wxSFShapeBase::FitToChildren()
{
	// HINT: overload it for custom actions...

	wxSFShapeBase* m_pParentShape = GetParentShape();
	if(m_pParentShape)m_pParentShape->FitToChildren();
}

wxSFShapeBase* wxSFShapeBase::GetParentShape()
{
    wxASSERT(m_pParentManager);
    if( !m_pParentManager )return NULL;

	if(m_nParentShapeId != -1)
	{
		return m_pParentManager->FindShape(m_nParentShapeId);
	}
	return NULL;
}

//----------------------------------------------------------------------------------//
// Drawing functions
//----------------------------------------------------------------------------------//

void wxSFShapeBase::Refresh()
{
    Refresh(this->GetBoundingBox());
}

void wxSFShapeBase::Draw(wxSFScaledPaintDC& dc)
{
    wxASSERT(m_pParentManager);
    wxASSERT(m_pParentManager->GetShapeCanvas());

    if(!m_pParentManager || !m_pParentManager->GetShapeCanvas())return;

	// first, draw myself
	if(m_fMouseOver)
	{
		if(m_fHighlighParent)
		{
			this->DrawHighlighted(dc);
		}
		else
			if(m_fHovering)this->DrawHover(dc);
	}
	else this->DrawNormal(dc);

	if(m_fSelected)this->DrawSelected(dc);

	// then draw child shapes...
	CShapeList m_lstChildren;
	GetChildren(m_lstChildren);

	wxCShapeListNode *node = m_lstChildren.GetLast();
	while(node)
	{
		node->GetData()->Draw(dc);
		node = node->GetPrevious();
	}

	// draw assigned connection
	/*m_lstChildren.Clear();
	m_pParentCanvas->GetAssignedConnections(this, wxSFShapeCanvas::lineSTARTING, m_lstChildren);

	node = m_lstChildren.GetLast();
	while(node)
	{
		node->GetData()->Draw(dc);
		node = node->GetPrevious();
	}*/
}

void wxSFShapeBase::DrawNormal(wxSFScaledPaintDC& dc)
{
	// HINT: overload it for custom actions...
}

void wxSFShapeBase::DrawSelected(wxSFScaledPaintDC& dc)
{
	// HINT: overload it for custom actions...

	wxCHandleListNode *node = m_lstHandles.GetFirst();
	while(node)
	{
		node->GetData()->Draw(dc);
		node = node->GetNext();
	}
}

void wxSFShapeBase::DrawHover(wxSFScaledPaintDC& dc)
{
	// HINT: overload it for custom actions...
}

void wxSFShapeBase::DrawHighlighted(wxSFScaledPaintDC& dc)
{
	// HINT: overload it for custom actions...
}

//----------------------------------------------------------------------------------//
// other functions
//----------------------------------------------------------------------------------//

void wxSFShapeBase::CreateHandles()
{
	// HINT: overload it for custom actions...
}

void wxSFShapeBase::AssignChild(wxSFShapeBase* child)
{
    wxASSERT(child);
    wxASSERT(m_pParentManager);

    if(child && m_pParentManager)
    {
        // set parent shape ID
        child->SetParentShapeId(this->GetId());

        // add the child shape to shape list if neccessary
        if( m_pParentManager->GetShapeList().IndexOf(child) == wxNOT_FOUND )
        {
            m_pParentManager->GetShapeList().Append(child);
        }

        // update parent shape
        Update();
    }
}

void wxSFShapeBase::GetChildren(CShapeList &children, bool recursive)
{
    wxASSERT(m_pParentManager);

	if(m_pParentManager)
	{
		wxCShapeListNode *node = m_pParentManager->GetShapeList().GetFirst();
		while(node)
		{
			wxSFShapeBase *pShape = node->GetData();

			if(pShape->GetParentShapeId() == m_nId)
			{
				children.Append(pShape);
				if(recursive)pShape->GetChildren(children, recursive);
			}

			node = node->GetNext();
		}
	}
}

void wxSFShapeBase::GetNeighbours(CShapeList& neighbours, CONNECTMODE condir, bool direct)
{
    if( !this->IsKindOf(CLASSINFO(wxSFLineShape)) )
    {
        m_lstProcessed.Clear();
        this->_GetNeighbours(neighbours, condir, direct);
        // delete starting object if necessary (can be added in a case of complex connection network)
        neighbours.DeleteObject(this);
    }
}

void wxSFShapeBase::_GetNeighbours(CShapeList& neighbours, CONNECTMODE condir, bool direct)
{
    wxASSERT(m_pParentManager);

    if( m_pParentManager )
    {
        if( m_lstProcessed.IndexOf(this) != wxNOT_FOUND )return;

        CShapeList lstConnections;
        wxSFLineShape *pLine;
        wxSFShapeBase *pOposite = NULL;

        m_pParentManager->GetAssignedConnections(this, condir, lstConnections);

        // find oposite shpes in direct branches
        wxCShapeListNode *node = lstConnections.GetFirst();
        while(node)
        {
            pLine = (wxSFLineShape*)node->GetData();
            switch(condir)
            {
                case lineSTARTING:
                    pOposite = m_pParentManager->FindShape(pLine->GetTrgShapeId());
                    break;

                case lineENDING:
                    pOposite = m_pParentManager->FindShape(pLine->GetSrcShapeId());
                    break;

                case lineBOTH:
                    {
                        if(m_nId == pLine->GetSrcShapeId())pOposite = m_pParentManager->FindShape(pLine->GetTrgShapeId());
                        else
                            pOposite = m_pParentManager->FindShape(pLine->GetSrcShapeId());
                    }
            }

            // add oposite shape to the list (if applicable)
            if(pOposite && !pOposite->IsKindOf(CLASSINFO(wxSFLineShape)) && ( neighbours.IndexOf(pOposite) == wxNOT_FOUND )) neighbours.Append(pOposite);

            // find next shapes
            if( !direct && pOposite )
            {
                // in the case of indirect branches we must differentiate between connections
                // and ordinary shapes
                m_lstProcessed.Append(this);

                if( pOposite->IsKindOf(CLASSINFO(wxSFLineShape)) )
                {
                    pLine = (wxSFLineShape*)pOposite;
                    switch(condir)
                    {
                        case lineSTARTING:
                            {
                                pOposite = m_pParentManager->FindShape( pLine->GetSrcShapeId() );

                                if( pOposite->IsKindOf(CLASSINFO(wxSFLineShape)) )pOposite->_GetNeighbours(neighbours, condir, direct);
                                else if( neighbours.IndexOf(pOposite) == wxNOT_FOUND )neighbours.Append(pOposite);
                            }
                            break;

                        case lineENDING:
                            {
                                pOposite = m_pParentManager->FindShape( pLine->GetTrgShapeId() );

                                if( pOposite->IsKindOf(CLASSINFO(wxSFLineShape)) )pOposite->_GetNeighbours(neighbours, condir, direct);
                                else if( neighbours.IndexOf(pOposite) == wxNOT_FOUND )neighbours.Append(pOposite);
                            }
                            break;

                        case lineBOTH:
                            {
                                pOposite = m_pParentManager->FindShape( pLine->GetSrcShapeId() );
                                if( pOposite->IsKindOf(CLASSINFO(wxSFLineShape)) )pOposite->_GetNeighbours(neighbours, condir, direct);
                                else if( neighbours.IndexOf(pOposite) == wxNOT_FOUND )neighbours.Append(pOposite);

                                pOposite = m_pParentManager->FindShape( pLine->GetTrgShapeId() );
                                if( pOposite->IsKindOf(CLASSINFO(wxSFLineShape)) )pOposite->_GetNeighbours(neighbours, condir, direct);
                                else if( neighbours.IndexOf(pOposite) == wxNOT_FOUND )neighbours.Append(pOposite);
                            }
                            break;
                    }
                }
                else
                    pLine->_GetNeighbours(neighbours, condir, direct);
            }

            node = node->GetNext();
        }
    }
}

wxSFShapeCanvas* wxSFShapeBase::GetParentCanvas()
{
    wxASSERT(m_pParentManager);
    if( !m_pParentManager )return NULL;

    return m_pParentManager->GetShapeCanvas();
}

void wxSFShapeBase::Refresh(const wxRect& rct)
{
    wxASSERT(m_pParentManager);
    wxASSERT(m_pParentManager->GetShapeCanvas());

	if(m_pParentManager && m_pParentManager->GetShapeCanvas())
	{
		m_pParentManager->GetShapeCanvas()->RefreshCanvas(false, rct);
	}
}

wxSFShapeHandle* wxSFShapeBase::GetHandle(wxSFShapeHandle::HANDLETYPE type, long id)
{
	wxCHandleListNode *node = m_lstHandles.GetFirst();
	while(node)
	{
		wxSFShapeHandle *hnd = node->GetData();
		if((hnd->GetType() == type) && ((id == -1)||(id == hnd->GetId())))return hnd;
		node = node->GetNext();
	}
	return NULL;
}

void wxSFShapeBase::AddHandle(wxSFShapeHandle::HANDLETYPE type, long id)
{
	wxSFShapeHandle* pHnd = GetHandle(type, id);
	if(!pHnd)
	{
		m_lstHandles.Append(new wxSFShapeHandle(this, type, id));
	}
}

void wxSFShapeBase::RemoveHandle(wxSFShapeHandle::HANDLETYPE type, long id)
{
	wxSFShapeHandle* pHnd = GetHandle(type, id);
	if(pHnd)
	{
		m_lstHandles.DeleteObject(pHnd);
	}
}

wxXmlNode* wxSFShapeBase::SerializeToXml(wxXmlNode* node)
{
	if(!node || (node->GetName() != wxT("object")))
	{
		node = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("object"));
		node->AddProperty(new wxXmlProperty(wxT("type"), this->GetClassInfo()->GetClassName()));
	}

	if(node)
	{
		node =  this->Serialize(node);
	}

	return node;
}

void wxSFShapeBase::DeserializeFromXml(wxXmlNode* node)
{
	if(node && (node->GetName()==wxT("object")))
	{
		this->Deserialize(node);
	}
}

void wxSFShapeBase::DoAlignment()
{
    wxSFShapeBase *pParent = this->GetParentShape();
    if(pParent)
    {
        wxRect parentBB = pParent->GetBoundingBox();
        wxRect shapeBB = this->GetBoundingBox();

        // do vertical alignment
        switch(m_nVAlign)
        {
            case valignTOP:
                m_nRelativePosition.y = m_nVBorder;
                break;

            case valignMIDDLE:
                m_nRelativePosition.y = parentBB.GetHeight()/2 - shapeBB.GetHeight()/2;
                break;

            case valignBOTTOM:
                m_nRelativePosition.y = parentBB.GetHeight() - shapeBB.GetHeight() - m_nVBorder;

            default:
                break;
        }

        // do horizontal alignment
        switch(m_nHAlign)
        {
            case halignLEFT:
                m_nRelativePosition.x = m_nHBorder;
                break;

            case halignCENTER:
                m_nRelativePosition.x = parentBB.GetWidth()/2 - shapeBB.GetWidth()/2;
                break;

            case halignRIGHT:
                m_nRelativePosition.x = parentBB.GetWidth() - shapeBB.GetWidth() - m_nHBorder;

            default:
                break;
        }
    }
}

//----------------------------------------------------------------------------------//
// Events handling
//----------------------------------------------------------------------------------//

//----------------------------------------------------------------------------------//
// Public virtual event handlers

void wxSFShapeBase::OnLeftClick(const wxPoint& pos)
{
	// HINT: overload it for custom actions...
}

void wxSFShapeBase::OnRightClick(const wxPoint& pos)
{
	// HINT: overload it for custom actions...
}

void wxSFShapeBase::OnLeftDoubleClick(const wxPoint& pos)
{
	// HINT: overload it for custom actions...
}

void wxSFShapeBase::OnRightDoubleClick(const wxPoint& pos)
{
	// HINT: overload it for custom actions...
}

void wxSFShapeBase::OnBeginDrag(const wxPoint& pos)
{
	// HINT: overload it for custom actions...
}

void wxSFShapeBase::OnDragging(const wxPoint& pos)
{
	// HINT: overload it for custom actions...
}

void wxSFShapeBase::OnEndDrag(const wxPoint& pos)
{
	// HINT: overload it for custom actions...
}

void wxSFShapeBase::OnHandle(wxSFShapeHandle& handle)
{
	// HINT: overload it for custom actions...
}

void wxSFShapeBase::OnBeginHandle(wxSFShapeHandle& handle)
{
	// HINT: overload it for custom actions...
}

void wxSFShapeBase::OnEndHandle(wxSFShapeHandle& handle)
{
	// HINT: overload it for custom actions...
}

bool wxSFShapeBase::OnKey(int key)
{
	// HINT: overload it for custom actions...

    return TRUE;
}

void wxSFShapeBase::OnMouseEnter(const wxPoint& pos)
{
	// HINT: overload it for custom actions...
}

void wxSFShapeBase::OnMouseOver(const wxPoint& pos)
{
	// HINT: overload it for custom actions...
}

void wxSFShapeBase::OnMouseLeave(const wxPoint& pos)
{
	// HINT: overload it for custom actions...
}

//----------------------------------------------------------------------------------//
// Original private event handlers

void wxSFShapeBase::_OnBeginDrag(const wxPoint& pos)
{
	m_fFirstMove = true;

	this->OnBeginDrag(pos);
}

void wxSFShapeBase::_OnDragging(const wxPoint& pos)
{
    wxASSERT(m_pParentManager);
    if( !m_pParentManager )return;

	if(m_fVisible && m_fActive && m_fPositionChange)
	{
		if(m_fFirstMove)
		{
			m_nMouseOffset = wxRealPoint(pos.x, pos.y) - this->GetAbsolutePosition();
		}

        // get shape BB BEFORE movement and combine it with BB of assigned lines
		wxRect prevBB;
		GetCompleteBoundingBox(prevBB, bbSELF | bbCONNECTIONS | bbCHILDREN);

		this->MoveTo(pos.x - m_nMouseOffset.x, pos.y - m_nMouseOffset.y);
        this->OnDragging(pos);

        // get shape BB AFTER movement and combine it with BB of assigned lines
		wxRect currBB;
		GetCompleteBoundingBox(currBB, bbSELF | bbCONNECTIONS | bbCHILDREN);

		// update canvas
		if(m_pParentManager->GetShapeCanvas())
		{
			const wxRect unionRect = prevBB.Union(currBB);
			Refresh(unionRect);
		}

		m_fFirstMove = false;
	}
}

void wxSFShapeBase::_OnEndDrag(const wxPoint& pos)
{
    this->OnEndDrag(pos);
}

void wxSFShapeBase::_OnMouseMove(const wxPoint& pos)
{
    wxASSERT(m_pParentManager);
    if( !m_pParentManager )return;

	if(m_fVisible && m_fActive)
	{
		bool fUpdateShape = false;
		wxSFShapeCanvas *pCanvas = m_pParentManager->GetShapeCanvas();

		// send the event to child handles too...
		wxCHandleListNode *node = m_lstHandles.GetFirst();
		while(node)
		{
			node->GetData()->OnMouseMove(pos);
			node = node->GetNext();
		}

		// determine, whether the shape should be highlighted for any reason
		if(pCanvas && m_fHighlighting)
		{
		    switch(pCanvas->GetMode())
		    {
            case wxSFShapeCanvas::modeSHAPEMOVE:
                {
                    if(pCanvas->GetShapeAtPosition(pos, 1, wxSFShapeCanvas::searchUNSELECTED) == this)
                    {
                        fUpdateShape = m_fHighlighParent = AcceptCurrentlyDraggedShapes();
                    }
                }
                break;

            case wxSFShapeCanvas::modeHANDLEMOVE:
                {
    				if(pCanvas->GetShapeAtPosition(pos, 1, wxSFShapeCanvas::searchUNSELECTED) == this)fUpdateShape = true;
                    m_fHighlighParent = false;
                }
                break;

            default:
                {
                    if(pCanvas->GetShapeAtPosition(pos) == this)fUpdateShape = true;
                    m_fHighlighParent = false;
                }
                break;
		    }
		}

		if(IsInside(pos) && fUpdateShape)
		{
			if(!m_fMouseOver)
			{
				m_fMouseOver = true;
				this->OnMouseEnter(pos);
			}
			else
			    this->OnMouseOver(pos);

			Refresh();
		}
		else
		{
			if(m_fMouseOver)
			{
				m_fMouseOver = false;
				this->OnMouseLeave(pos);
				Refresh();
			}
		}
	}
}

void wxSFShapeBase::_OnKey(int key)
{
    wxASSERT(m_pParentManager);

    wxSFShapeCanvas *pCanvas = m_pParentManager->GetShapeCanvas();

    wxASSERT(pCanvas);

    if( !m_pParentManager || !pCanvas)return;

	if(m_fVisible && m_fActive && m_fSelected)
	{
		double dx = 1, dy = 1;
		bool fRefreshAll = false;
		wxRect prevBB;


		if(pCanvas->IsGridUsed())
		{
			dx = pCanvas->GetGrid().x;
			dy = pCanvas->GetGrid().y;
		}

		CShapeList lstSelection;
		pCanvas->GetSelectedShapes(lstSelection);
		if((lstSelection.GetCount() > 1) && (lstSelection.IndexOf(this) != wxNOT_FOUND))
		{
		    fRefreshAll = true;
		}

		if(!fRefreshAll)
		{
            GetCompleteBoundingBox(prevBB, bbSELF | bbCONNECTIONS | bbCHILDREN);
		}

        if(this->OnKey(key))
        {
            switch(key)
            {
            case WXK_LEFT:
                if(m_fPositionChange)MoveBy(-dx, 0);
                break;

            case WXK_RIGHT:
                if(m_fPositionChange)MoveBy(dx, 0);
                break;

            case WXK_UP:
                if(m_fPositionChange)MoveBy(0, -dy);
                break;

            case WXK_DOWN:
                if(m_fPositionChange)MoveBy(0, dy);
                break;
            }
        }

        if(!fRefreshAll)
        {
            wxRect currBB;
            GetCompleteBoundingBox(currBB, bbSELF | bbCONNECTIONS | bbCHILDREN);

            prevBB.Union(currBB);
            Refresh(prevBB);
        }
        else
            pCanvas->Refresh(false);
	}
}

void wxSFShapeBase::_OnHandle(wxSFShapeHandle& handle)
{
    wxASSERT(m_pParentManager);
    if( !m_pParentManager )return;

    // get initial bounding box
    //wxRect prevDispRct = GetParentCanvas()->GetSelectionBB();

    // call appropriate user-defined handler
	this->OnHandle(handle);

    // update shape
    Update();

    // refresh canvas
    /*wxRect currDispRct = GetParentCanvas()->GetSelectionBB();
    GetParentCanvas()->RefreshCanvas(false, prevDispRct.Union(currDispRct).Inflate(int(MEOFFSET*GetParentCanvas()->GetScale())));*/
    if( m_pParentManager->GetShapeCanvas() )m_pParentManager->GetShapeCanvas()->Refresh(false);

}
