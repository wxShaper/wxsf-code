/***************************************************************
 * Name:      ShapeBase.h
 * Purpose:   Implements shape base class
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "wx_pch.h"

#ifdef _DEBUG_MSVC
#define new DEBUG_NEW
#endif

#include "wx/wxsf/ShapeBase.h"
#include "wx/wxsf/ShapeCanvas.h"
#include "wx/wxsf/TextShape.h"
#include "wx/wxsf/GridShape.h"

#include <wx/listimpl.cpp>

WX_DEFINE_EXPORTED_LIST(ShapeList);

ShapeList m_lstProcessed;

XS_IMPLEMENT_CLONABLE_CLASS(wxSFShapeBase, xsSerializable);

wxSFShapeBase::wxSFShapeBase(void)
{
	// initialize data members
	m_fSelected = false;
	m_fMouseOver = false;
	m_fFirstMove = false;
	m_fHighlighParent = false;
	m_pUserData = NULL;

	// archived properties
	m_fVisible = sfdvBASESHAPE_VISIBILITY;
	m_fActive = sfdvBASESHAPE_ACTIVITY;
	m_nStyle = sfdvBASESHAPE_DEFAULT_STYLE;
	m_nRelativePosition = sfdvBASESHAPE_POSITION;
	m_nHoverColor = sfdvBASESHAPE_HOVERCOLOUR;
	m_nVAlign = sfdvBASESHAPE_VALIGN;
	m_nHAlign = sfdvBASESHAPE_HALIGN;
	m_nVBorder = sfdvBASESHAPE_VBORDER;
	m_nHBorder = sfdvBASESHAPE_HBORDER;

    // mark serialized properties
	MarkSerializableDataMembers();


	m_lstHandles.DeleteContents(true);
}

wxSFShapeBase::wxSFShapeBase(const wxRealPoint& pos, wxSFDiagramManager* manager)
{
	wxASSERT_MSG(manager, wxT("Shape manager has not been properly set in shape constructor."));

	// initialize data members
	SetParentManager(manager);
    m_pUserData = NULL;

	if(m_pParentManager)
	{
		if( GetShapeManager()->GetShapeCanvas() )m_nHoverColor = GetShapeManager()->GetShapeCanvas()->GetHoverColour();
		else
            m_nHoverColor = sfdvBASESHAPE_HOVERCOLOUR;
	}
	else
	{
		m_nHoverColor = sfdvBASESHAPE_HOVERCOLOUR;
	}

	m_fSelected = false;
	m_fMouseOver = false;
	m_fFirstMove = false;
	m_fHighlighParent = false;

	// archived properties
	m_fVisible = sfdvBASESHAPE_VISIBILITY;
	m_fActive = sfdvBASESHAPE_ACTIVITY;
	m_nStyle = sfdvBASESHAPE_DEFAULT_STYLE;
	m_nVAlign = sfdvBASESHAPE_VALIGN;
	m_nHAlign = sfdvBASESHAPE_HALIGN;
	m_nVBorder = sfdvBASESHAPE_VBORDER;
	m_nHBorder = sfdvBASESHAPE_HBORDER;

	wxSFShapeBase* m_pParentShape = GetParentShape();
	if(m_pParentShape)m_nRelativePosition = pos - m_pParentShape->GetAbsolutePosition();
	else
		m_nRelativePosition = sfdvBASESHAPE_POSITION;

	// mark serialized properties
	MarkSerializableDataMembers();

	m_lstHandles.DeleteContents(true);
}

wxSFShapeBase::wxSFShapeBase(const wxSFShapeBase& obj) : xsSerializable(obj)
{
	// initialize data members
	m_fSelected = false;
	m_fMouseOver = false;
	m_fFirstMove = false;
	m_fHighlighParent = false;

	m_fVisible = obj.m_fVisible;
	m_fActive = obj.m_fActive;
	m_nStyle = obj.m_nStyle;
	m_nVAlign = obj.m_nVAlign;
	m_nHAlign = obj.m_nHAlign;
	m_nVBorder = obj.m_nVBorder;
	m_nHBorder = obj.m_nHBorder;

	m_nHoverColor = obj.m_nHoverColor;
	m_nRelativePosition = obj.m_nRelativePosition;

	WX_APPEND_ARRAY(m_arrAcceptedChildren, obj.m_arrAcceptedChildren);
	WX_APPEND_ARRAY(m_arrAcceptedSrcNeighbours, obj.m_arrAcceptedSrcNeighbours);
	WX_APPEND_ARRAY(m_arrAcceptedTrgNeighbours, obj.m_arrAcceptedTrgNeighbours);
	WX_APPEND_ARRAY(m_arrAcceptedConnections, obj.m_arrAcceptedConnections);

	if( obj.m_pUserData )
	{
		SetUserData((xsSerializable*)obj.m_pUserData->Clone());
	}
	else
		m_pUserData = NULL;

	// copy handles
	wxSFShapeHandle *pHandle;
	HandleList::compatibility_iterator node = obj.m_lstHandles.GetFirst();
	while(node)
	{
		pHandle = new wxSFShapeHandle(*node->GetData());
		pHandle->SetParentShape(this);
		m_lstHandles.Append(pHandle);

		node = node->GetNext();
	}
    m_lstHandles.DeleteContents(true);

	// mark serialized properties
	MarkSerializableDataMembers();
}

wxSFShapeBase::~wxSFShapeBase(void)
{
	// clear handles
	m_lstHandles.Clear();
	// delete user data
	if(m_pUserData && (m_nStyle & sfsDELETE_USER_DATA))delete m_pUserData;
}

void wxSFShapeBase::MarkSerializableDataMembers()
{
    // mark serialized properties
    XS_SERIALIZE_EX(m_fActive, wxT("active"), sfdvBASESHAPE_ACTIVITY);
    XS_SERIALIZE_EX(m_fVisible, wxT("visibility"), sfdvBASESHAPE_VISIBILITY);
    XS_SERIALIZE_EX(m_nStyle, wxT("style"), sfdvBASESHAPE_DEFAULT_STYLE);
    XS_SERIALIZE(m_arrAcceptedChildren, wxT("accepted_children"));
    XS_SERIALIZE(m_arrAcceptedConnections, wxT("accepted_connections"));
    XS_SERIALIZE(m_arrAcceptedSrcNeighbours, wxT("accepted_src_neighbours"));
    XS_SERIALIZE(m_arrAcceptedTrgNeighbours, wxT("accepted_trg_neighbours"));
    XS_SERIALIZE_EX(m_nHoverColor, wxT("hover_color"), sfdvBASESHAPE_HOVERCOLOUR);
    XS_SERIALIZE(m_nRelativePosition, wxT("relative_position"));
    XS_SERIALIZE_LONG_EX(m_nHAlign, wxT("halign"), sfdvBASESHAPE_HALIGN);
    XS_SERIALIZE_LONG_EX(m_nVAlign, wxT("valign"), (long)sfdvBASESHAPE_VALIGN);
    XS_SERIALIZE_EX(m_nHBorder, wxT("hborder"), sfdvBASESHAPE_HBORDER);
    XS_SERIALIZE_EX(m_nVBorder, wxT("vborder"), sfdvBASESHAPE_VBORDER);
    XS_SERIALIZE(m_pUserData, wxT("user_data"));
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
    m_lstProcessed.Clear();
    _GetCompleteBoundingBox(rct, mask);
}

void wxSFShapeBase::_GetCompleteBoundingBox(wxRect &rct, int mask)
{
    //wxASSERT(m_pParentManager);
    if(!m_pParentManager)return;

    if( m_lstProcessed.IndexOf(this) != wxNOT_FOUND )return;
    else
        m_lstProcessed.Append(this);

	ShapeList lstChildren;
	//SerializableList lstConnections;

	// firts, get bounding box of the current shape
	if(mask & bbSELF)
	{
		if(rct.IsEmpty())rct = this->GetBoundingBox().Inflate((int)m_nHBorder, (int)m_nVBorder);
		else
			rct.Union(this->GetBoundingBox().Inflate((int)m_nHBorder, (int)m_nVBorder));

		// add also shadow offset if neccessary
        if( (mask & bbSHADOW) && (m_nStyle & sfsSHOW_SHADOW) && GetParentCanvas() )
        {
            wxRealPoint nOffset = GetParentCanvas()->GetShadowOffset();

            if( nOffset.x < 0 )
            {
                rct.SetX(rct.GetX() + (int)nOffset.x);
                rct.SetWidth(rct.GetWidth() - (int)nOffset.x);
            }
            else
                rct.SetWidth(rct.GetWidth() + (int)nOffset.x);

            if( nOffset.y < 0 )
            {
                rct.SetY(rct.GetY() + (int)nOffset.y);
                rct.SetHeight(rct.GetHeight() - (int)nOffset.y);
            }
            else
                rct.SetHeight(rct.GetHeight() + (int)nOffset.y);;
        }
	}
	else
		mask |= bbSELF;

	// get list of all connection lines assigned to the shape and find their child shapes
	if(mask & bbCONNECTIONS)
	{
		wxSFShapeBase *pLine;

        ShapeList lstLines;
        GetShapeManager()->GetAssignedConnections(this, CLASSINFO(wxSFLineShape), lineBOTH, lstLines);

		ShapeList::compatibility_iterator node = lstLines.GetFirst();
		while(node)
		{
			pLine = node->GetData();

			//rct.Union(pLine->GetBoundingBox());
			lstChildren.Append(pLine);

			// get children of the connections
			pLine->GetChildShapes(lstChildren);

			node = node->GetNext();
		}
	}

	// get children of this shape
	if(mask & bbCHILDREN)
	{
		this->GetChildShapes(lstChildren);

		// now, call this function for all children recursively...
		ShapeList::compatibility_iterator node = lstChildren.GetFirst();
		while(node)
		{
		    node->GetData()->_GetCompleteBoundingBox(rct, mask);
			node = node->GetNext();
		}
	}
}

bool wxSFShapeBase::IsInside(const wxPoint& pos)
{
	// HINT: overload it for custom actions...

	return this->GetBoundingBox().Contains(pos);
}

bool wxSFShapeBase::IsInside(const wxRect& rct)
{
	// HINT: overload it for custom actions...

	return rct.Contains(this->GetBoundingBox());
}

bool wxSFShapeBase::Intersects(const wxRect& rct)
{
	// HINT: overload it for custom actions...

	return rct.Intersects(this->GetBoundingBox());
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

wxRealPoint wxSFShapeBase::GetBorderPoint(const wxRealPoint& WXUNUSED(start), const wxRealPoint& WXUNUSED(end))
{
    // HINT: override it for custom actions

    return wxRealPoint();
}

void wxSFShapeBase::ShowHandles(bool show)
{
	HandleList::compatibility_iterator node = m_lstHandles.GetFirst();
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

    //this->Update();
}

void wxSFShapeBase::Scale(const wxRealPoint& scale, bool children)
{
	this->Scale(scale.x, scale.y, children);
}

void wxSFShapeBase::ScaleChildren(double x, double y)
{
	ShapeList m_lstChildren;
	GetChildShapes(m_lstChildren, sfRECURSIVE);

	ShapeList::compatibility_iterator node = m_lstChildren.GetFirst();
	while(node)
	{
		wxSFShapeBase* pShape = node->GetData();

        if((pShape->GetStyle() & sfsSIZE_CHANGE) && !pShape->IsKindOf(CLASSINFO(wxSFTextShape)))
		{
		    pShape->Scale(x, y, sfWITHOUTCHILDREN);
		}

		if( (pShape->GetStyle() & sfsPOSITION_CHANGE) && ((pShape->GetVAlign() == valignNONE) || (pShape->GetHAlign() == halignNONE)) )
		{
            pShape->SetRelativePosition(pShape->m_nRelativePosition.x*x, pShape->m_nRelativePosition.y*y);
		}

        // re-align shapes which have set any alignment mode
		pShape->DoAlignment();

		node = node->GetNext();
	}
}

void wxSFShapeBase::Update()
{
    // do self-alignment
    DoAlignment();

    // do alignment of shape's children (if required)$(IntermediateDirectory)/$(Project$(IntermediateDirectory)/$(ProjectName).soName).so
    if( !this->IsKindOf(CLASSINFO(wxSFLineShape)) )
    {
        SerializableList::compatibility_iterator node = GetFirstChildNode();
        while(node)
        {
            ((wxSFShapeBase*)node->GetData())->DoAlignment();
            node = node->GetNext();
        }
    }

    // fit the shape to its children
    this->FitToChildren();

    // do it recursively on all parent shapes
    if( GetParentShape() )GetParentShape()->Update();
}

bool wxSFShapeBase::AcceptCurrentlyDraggedShapes()
{
    //wxASSERT(m_pParentManager);
    //wxASSERT(m_pParentManager->GetShapeCanvas());

	if(!m_pParentManager || !GetShapeManager()->GetShapeCanvas())return false;

    if(!IsChildAccepted(wxT("All")))
    {
        ShapeList lstSelection;
        GetShapeManager()->GetShapeCanvas()->GetSelectedShapes(lstSelection);

        ShapeList::compatibility_iterator node = lstSelection.GetFirst();
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

	/*wxSFShapeBase* m_pParentShape = GetParentShape();
	if(m_pParentShape)m_pParentShape->FitToChildren();*/
}

wxSFShapeBase* wxSFShapeBase::GetParentShape()
{
    //wxASSERT(m_pParentManager);

    if(m_pParentManager && (m_pParentManager->GetRootItem() != m_pParentItem) )
    {
        return (wxSFShapeBase*)m_pParentItem;
    }
    else
        return NULL;
}

wxSFShapeBase* wxSFShapeBase::GetGrandParentShape()
{
	wxSFShapeBase *pGrandPa = NULL;

    if(m_pParentManager)
	{
		if( m_pParentItem && (m_pParentManager->GetRootItem() != m_pParentItem) )
		{
			pGrandPa = ((wxSFShapeBase*)m_pParentItem)->GetGrandParentShape();
		}
		else if( m_pParentItem && (m_pParentManager->GetRootItem() == m_pParentItem) )
		{
			pGrandPa = this;
		}
    }

	return pGrandPa;
}

void wxSFShapeBase::SetUserData(xsSerializable* data)
{
    m_pUserData = data;
    if( m_pUserData )
    {
        // let the associated data to know its parent shape
        m_pUserData->SetParent(this);
    }
}

//----------------------------------------------------------------------------------//
// Drawing functions
//----------------------------------------------------------------------------------//

void wxSFShapeBase::Refresh()
{
    Refresh(this->GetBoundingBox());
}

void wxSFShapeBase::Draw(wxDC& dc, bool children)
{
	if(!m_pParentManager || !GetShapeManager()->GetShapeCanvas())return;
    if(!m_fVisible)return;

    // draw the shape shadow if required
    if( !m_fSelected && (m_nStyle & sfsSHOW_SHADOW) )
    {
        this->DrawShadow(dc);
    }

	// first, draw itself
	if(m_fMouseOver && ( m_fHighlighParent || (m_nStyle & sfsHOVERING) ))
	{
		if(m_fHighlighParent)
		{
			this->DrawHighlighted(dc);
		}
		else
			this->DrawHover(dc);
	}
	else
	    this->DrawNormal(dc);

	if(m_fSelected)this->DrawSelected(dc);

	// ... then draw child shapes
	if(children)
	{
        SerializableList::compatibility_iterator node = GetFirstChildNode();
        while(node)
        {
            ((wxSFShapeBase*)node->GetData())->Draw(dc);
            node = node->GetNext();
        }
	}
}

void wxSFShapeBase::DrawNormal(wxDC& WXUNUSED(dc))
{
	// HINT: overload it for custom actions...
}

void wxSFShapeBase::DrawSelected(wxDC& dc)
{
	// HINT: overload it for custom actions...

	if( m_nStyle & sfsSHOW_HANDLES )
	{
		HandleList::compatibility_iterator node = m_lstHandles.GetFirst();
		while(node)
		{
			node->GetData()->Draw(dc);
			node = node->GetNext();
		}
	}
}

void wxSFShapeBase::DrawHover(wxDC& WXUNUSED(dc))
{
	// HINT: overload it for custom actions...
}

void wxSFShapeBase::DrawHighlighted(wxDC& WXUNUSED(dc))
{
	// HINT: overload it for custom actions...
}

void wxSFShapeBase::DrawShadow(wxDC& WXUNUSED(dc))
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

void wxSFShapeBase::GetChildShapes(ShapeList& children, bool recursive)
{
    if( recursive ) GetChildrenRecursively(NULL, (SerializableList&)children);
    else
        GetChildren(NULL, (SerializableList&)children);
}

void wxSFShapeBase::GetNeighbours(ShapeList& neighbours, wxClassInfo *shapeInfo, CONNECTMODE condir, bool direct)
{
    if( !this->IsKindOf(CLASSINFO(wxSFLineShape)) )
    {
        m_lstProcessed.Clear();
        this->_GetNeighbours(neighbours, shapeInfo, condir, direct);
        // delete starting object if necessary (can be added in a case of complex connection network)
        neighbours.DeleteObject(this);
    }
}

void wxSFShapeBase::_GetNeighbours(ShapeList& neighbours, wxClassInfo *shapeInfo, CONNECTMODE condir, bool direct)
{
    //wxASSERT(m_pParentManager);

    if( m_pParentManager )
    {
        if( m_lstProcessed.IndexOf(this) != wxNOT_FOUND )return;

        ShapeList lstConnections;
        wxSFLineShape *pLine;
        wxSFShapeBase *pOposite = NULL;

        GetShapeManager()->GetAssignedConnections(this, shapeInfo, condir, lstConnections);

        // find oposite shpes in direct branches
        ShapeList::compatibility_iterator node = lstConnections.GetFirst();
        while(node)
        {
            pLine = (wxSFLineShape*)node->GetData();
            switch(condir)
            {
                case lineSTARTING:
                    pOposite = GetShapeManager()->FindShape(pLine->GetTrgShapeId());
                    break;

                case lineENDING:
                    pOposite = GetShapeManager()->FindShape(pLine->GetSrcShapeId());
                    break;

                case lineBOTH:
                    {
                        if(m_nId == pLine->GetSrcShapeId())pOposite = GetShapeManager()->FindShape(pLine->GetTrgShapeId());
                        else
                            pOposite = GetShapeManager()->FindShape(pLine->GetSrcShapeId());
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
                                pOposite = GetShapeManager()->FindShape( pLine->GetSrcShapeId() );

                                if( pOposite->IsKindOf(CLASSINFO(wxSFLineShape)) )pOposite->_GetNeighbours(neighbours, shapeInfo, condir, direct);
                                else if( neighbours.IndexOf(pOposite) == wxNOT_FOUND )neighbours.Append(pOposite);
                            }
                            break;

                        case lineENDING:
                            {
                                pOposite = GetShapeManager()->FindShape( pLine->GetTrgShapeId() );

                                if( pOposite->IsKindOf(CLASSINFO(wxSFLineShape)) )pOposite->_GetNeighbours(neighbours, shapeInfo, condir, direct);
                                else if( neighbours.IndexOf(pOposite) == wxNOT_FOUND )neighbours.Append(pOposite);
                            }
                            break;

                        case lineBOTH:
                            {
                                pOposite = GetShapeManager()->FindShape( pLine->GetSrcShapeId() );
                                if( pOposite->IsKindOf(CLASSINFO(wxSFLineShape)) )pOposite->_GetNeighbours(neighbours, shapeInfo, condir, direct);
                                else if( neighbours.IndexOf(pOposite) == wxNOT_FOUND )neighbours.Append(pOposite);

                                pOposite = GetShapeManager()->FindShape( pLine->GetTrgShapeId() );
                                if( pOposite->IsKindOf(CLASSINFO(wxSFLineShape)) )pOposite->_GetNeighbours(neighbours, shapeInfo, condir, direct);
                                else if( neighbours.IndexOf(pOposite) == wxNOT_FOUND )neighbours.Append(pOposite);
                            }
                            break;
                    }
                }
                else
                    pLine->_GetNeighbours(neighbours, shapeInfo, condir, direct);
            }

            node = node->GetNext();
        }
    }
}

wxSFShapeCanvas* wxSFShapeBase::GetParentCanvas()
{
    //wxASSERT(m_pParentManager);
    if( !m_pParentManager )return NULL;

    return GetShapeManager()->GetShapeCanvas();
}

void wxSFShapeBase::Refresh(const wxRect& rct)
{
    //wxASSERT(m_pParentManager);
    //wxASSERT(m_pParentManager->GetShapeCanvas());

	if(m_pParentManager && GetShapeManager()->GetShapeCanvas())
	{
		GetShapeManager()->GetShapeCanvas()->RefreshCanvas(false, rct);
	}
}

wxSFShapeHandle* wxSFShapeBase::GetHandle(wxSFShapeHandle::HANDLETYPE type, long id)
{
	HandleList::compatibility_iterator node = m_lstHandles.GetFirst();
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

void wxSFShapeBase::DoAlignment()
{
    wxSFShapeBase *pParent = this->GetParentShape();

    if(pParent && !pParent->IsKindOf(CLASSINFO(wxSFGridShape)))
    {
        wxRect parentBB;

        if(pParent->IsKindOf(CLASSINFO(wxSFLineShape)))
        {
            wxRealPoint pos = pParent->GetAbsolutePosition();
            parentBB = wxRect((int)pos.x, (int)pos.y, 1, 1);
        }
        else
            parentBB = pParent->GetBoundingBox();

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
                break;

            case valignEXPAND:
                if( ContainsStyle( sfsSIZE_CHANGE ) )
                {
                    m_nRelativePosition.y = m_nVBorder;
                    this->Scale( 1.f, double(parentBB.GetHeight() - 2*m_nVBorder)/shapeBB.GetHeight() );
                }
                break;

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
                break;

            case halignEXPAND:
                if( ContainsStyle( sfsSIZE_CHANGE ) )
                {
                    m_nRelativePosition.x = m_nHBorder;
                    this->Scale( double(parentBB.GetWidth() - 2*m_nHBorder)/shapeBB.GetWidth(), 1.f );
                }
                break;



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

void wxSFShapeBase::OnLeftClick(const wxPoint& WXUNUSED(pos))
{
	// HINT: overload it for custom actions...
}

void wxSFShapeBase::OnRightClick(const wxPoint& WXUNUSED(pos))
{
	// HINT: overload it for custom actions...
}

void wxSFShapeBase::OnLeftDoubleClick(const wxPoint& WXUNUSED(pos))
{
	// HINT: overload it for custom actions...
}

void wxSFShapeBase::OnRightDoubleClick(const wxPoint& WXUNUSED(pos))
{
	// HINT: overload it for custom actions...
}

void wxSFShapeBase::OnBeginDrag(const wxPoint& WXUNUSED(pos))
{
	// HINT: overload it for custom actions...
}

void wxSFShapeBase::OnDragging(const wxPoint& WXUNUSED(pos))
{
	// HINT: overload it for custom actions...
}

void wxSFShapeBase::OnEndDrag(const wxPoint& WXUNUSED(pos))
{
	// HINT: overload it for custom actions...
}

void wxSFShapeBase::OnHandle(wxSFShapeHandle& WXUNUSED(handle))
{
	// HINT: overload it for custom actions...
}

void wxSFShapeBase::OnBeginHandle(wxSFShapeHandle& WXUNUSED(handle))
{
	// HINT: overload it for custom actions...
}

void wxSFShapeBase::OnEndHandle(wxSFShapeHandle& WXUNUSED(handle))
{
	// HINT: overload it for custom actions...
}

bool wxSFShapeBase::OnKey(int WXUNUSED(key))
{
	// HINT: overload it for custom actions...

    return TRUE;
}

void wxSFShapeBase::OnMouseEnter(const wxPoint& WXUNUSED(pos))
{
	// HINT: overload it for custom actions...
}

void wxSFShapeBase::OnMouseOver(const wxPoint& WXUNUSED(pos))
{
	// HINT: overload it for custom actions...
}

void wxSFShapeBase::OnMouseLeave(const wxPoint& WXUNUSED(pos))
{
	// HINT: overload it for custom actions...
}

void wxSFShapeBase::OnChildDropped(const wxRealPoint& WXUNUSED(pos), wxSFShapeBase* WXUNUSED(child))
{
	// HINT: overload it for custom actions...
}

//----------------------------------------------------------------------------------//
// Original private event handlers

void wxSFShapeBase::_OnBeginDrag(const wxPoint& pos)
{
	if ( !m_fActive ) return;

	m_fFirstMove = true;
	this->OnBeginDrag(pos);
}

void wxSFShapeBase::_OnDragging(const wxPoint& pos)
{
    //wxASSERT(m_pParentManager);
    if( !m_pParentManager )return;

	if(m_fVisible && m_fActive && (m_nStyle & sfsPOSITION_CHANGE))
	{
		if(m_fFirstMove)
		{
			m_nMouseOffset = wxRealPoint(pos.x, pos.y) - this->GetAbsolutePosition();
		}

        // get shape BB BEFORE movement and combine it with BB of assigned lines
		wxRect prevBB;
		GetCompleteBoundingBox(prevBB, bbSELF | bbCONNECTIONS | bbCHILDREN | bbSHADOW);

		this->MoveTo(pos.x - m_nMouseOffset.x, pos.y - m_nMouseOffset.y);
        this->OnDragging(pos);

        // get shape BB AFTER movement and combine it with BB of assigned lines
		wxRect currBB;
		GetCompleteBoundingBox(currBB, bbSELF | bbCONNECTIONS | bbCHILDREN | bbSHADOW);

		// update canvas
		if(GetShapeManager()->GetShapeCanvas())
		{
			const wxRect unionRect = prevBB.Union(currBB);
			Refresh(unionRect);
		}

		m_fFirstMove = false;
	}
}

void wxSFShapeBase::_OnEndDrag(const wxPoint& pos)
{
	if ( !m_fActive ) return;

    this->OnEndDrag(pos);
}

void wxSFShapeBase::_OnMouseMove(const wxPoint& pos)
{
    //wxASSERT(m_pParentManager);
    if( !m_pParentManager )return;

	if(m_fVisible && m_fActive)
	{
		bool fUpdateShape = false;
		wxSFShapeCanvas *pCanvas = GetShapeManager()->GetShapeCanvas();

		// send the event to the shape handles too...
		HandleList::compatibility_iterator node = m_lstHandles.GetFirst();
		while(node)
		{
			node->GetData()->_OnMouseMove(pos);
			node = node->GetNext();
		}

		// determine, whether the shape should be highlighted for any reason
		if(pCanvas)
		{
		    switch(pCanvas->GetMode())
		    {
            case wxSFShapeCanvas::modeSHAPEMOVE:
                {
					if(ContainsStyle(sfsHIGHLIGHTING) && pCanvas->ContainsStyle(wxSFShapeCanvas::sfsHIGHLIGHTING))
                    {
                        if(pCanvas->GetShapeUnderCursor(wxSFShapeCanvas::searchUNSELECTED) == this)
                        {
                            fUpdateShape = m_fHighlighParent = AcceptCurrentlyDraggedShapes();
                        }
                    }
                }
                break;

            case wxSFShapeCanvas::modeHANDLEMOVE:
                {
                    if(ContainsStyle(sfsHOVERING) && pCanvas->ContainsStyle(wxSFShapeCanvas::sfsHOVERING))
                    {
                        if(pCanvas->GetShapeUnderCursor(wxSFShapeCanvas::searchUNSELECTED) == this)fUpdateShape = true;
                        m_fHighlighParent = false;
                    }
                }
                break;

            default:
                {
                    if(ContainsStyle(sfsHOVERING) && pCanvas->ContainsStyle(wxSFShapeCanvas::sfsHOVERING))
                    {
                        if(pCanvas->GetShapeUnderCursor() == this)fUpdateShape = true;
                        m_fHighlighParent = false;
                    }
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
    //wxASSERT(m_pParentManager);

    if(!m_pParentManager)return;

    wxSFShapeCanvas *pCanvas = GetShapeManager()->GetShapeCanvas();

    //wxASSERT(pCanvas);

    if(!pCanvas)return;

	if(m_fVisible && m_fActive && m_fSelected)
	{
		double dx = 1, dy = 1;
		bool fRefreshAll = false;
		wxRect prevBB;


		if( pCanvas->ContainsStyle(wxSFShapeCanvas::sfsGRID_USE) )
		{
			dx = pCanvas->GetGrid().x;
			dy = pCanvas->GetGrid().y;
		}

		ShapeList lstSelection;
		pCanvas->GetSelectedShapes(lstSelection);
		if((lstSelection.GetCount() > 1) && (lstSelection.IndexOf(this) != wxNOT_FOUND))
		{
		    fRefreshAll = true;
		}

		if(!fRefreshAll)
		{
            GetCompleteBoundingBox(prevBB, bbSELF | bbCONNECTIONS | bbCHILDREN | bbSHADOW);
		}

        if(this->OnKey(key))
        {
            switch(key)
            {
            case WXK_LEFT:
                if(ContainsStyle(sfsPOSITION_CHANGE))MoveBy(-dx, 0);
                break;

            case WXK_RIGHT:
                if(ContainsStyle(sfsPOSITION_CHANGE))MoveBy(dx, 0);
                break;

            case WXK_UP:
                if(ContainsStyle(sfsPOSITION_CHANGE))MoveBy(0, -dy);
                break;

            case WXK_DOWN:
                if(ContainsStyle(sfsPOSITION_CHANGE))MoveBy(0, dy);
                break;
            }
        }

        if(!fRefreshAll)
        {
            wxRect currBB;
            GetCompleteBoundingBox(currBB, bbSELF | bbCONNECTIONS | bbCHILDREN | bbSHADOW);

            prevBB.Union(currBB);
            Refresh(prevBB);
        }
        else
            pCanvas->Refresh(false);
	}
}

void wxSFShapeBase::_OnHandle(wxSFShapeHandle& handle)
{
   // wxASSERT(m_pParentManager);
    wxSFShapeBase *pChild;
    wxSFShapeCanvas *pCanvas = GetShapeManager()->GetShapeCanvas();

    if( !m_pParentManager )return;

    // call appropriate user-defined handler
	this->OnHandle(handle);

    // align children
    SerializableList::compatibility_iterator node = GetFirstChildNode();
    while(node)
    {
        pChild = (wxSFShapeBase*)node->GetData();

        if((pChild->GetVAlign() != valignNONE) || (pChild->GetHAlign() != halignNONE))
        {
            pChild->DoAlignment();
        }
        node = node->GetNext();
    }
    // update shape
    this->Update();

    // refresh canvas
    if( pCanvas ) pCanvas->Refresh(false);

}
