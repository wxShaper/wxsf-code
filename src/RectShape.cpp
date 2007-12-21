/***************************************************************
 * Name:      RectShape.cpp
 * Purpose:   Implements rectangular shape class
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

#include "wx/wxsf/RectShape.h"
#include "wx/wxsf/ShapeCanvas.h"
#include "wx/wxsf/CommonFcn.h"

// TODO: wxSFShapeBase: Implement LockAspectRation() function

XS_IMPLEMENT_CLONABLE_CLASS(wxSFRectShape, wxSFShapeBase);

wxSFRectShape::wxSFRectShape(void) : wxSFShapeBase()
{
	m_nRectSize = sfdvRECTSHAPE_SIZE;
	m_Border = sfdvRECTSHAPE_BORDER;
	m_Fill = sfdvRECTSHAPE_FILL;

	MarkSerializableDataMembers();
}

wxSFRectShape::wxSFRectShape(const wxRealPoint& pos, const wxRealPoint& size, wxSFDiagramManager* manager)
: wxSFShapeBase(pos, manager)
{
	m_nRectSize = size;
	m_Border = sfdvRECTSHAPE_BORDER;
	m_Fill = sfdvRECTSHAPE_FILL;

	MarkSerializableDataMembers();
}

wxSFRectShape::wxSFRectShape(wxSFRectShape& obj) : wxSFShapeBase(obj)
{
	m_nRectSize = obj.m_nRectSize;
	m_Border = obj.m_Border;
	m_Fill = obj.m_Fill;

	MarkSerializableDataMembers();
}

wxSFRectShape::~wxSFRectShape(void)
{
}

void wxSFRectShape::MarkSerializableDataMembers()
{
	XS_SERIALIZE_EX(m_nRectSize, wxT("size"),sfdvRECTSHAPE_SIZE);
	XS_SERIALIZE_EX(m_Border, wxT("border"), sfdvRECTSHAPE_BORDER);
	XS_SERIALIZE_EX(m_Fill, wxT("fill"),sfdvRECTSHAPE_FILL);
}

//----------------------------------------------------------------------------------//
// public virtual functions
//----------------------------------------------------------------------------------//

wxRect wxSFRectShape::GetBoundingBox()
{
    wxRealPoint apos = GetAbsolutePosition();
	return wxRect(wxPoint((int)apos.x, (int)apos.y), wxSize((int)m_nRectSize.x, (int)m_nRectSize.y ));
}

void wxSFRectShape::Scale(double x, double y, bool children)
{
	// HINT: overload it for custom actions...

	if((x > 0) && (y > 0))
	{
		SetRectSize(m_nRectSize.x * x, m_nRectSize.y * y);

        // call default function implementation (needed for scaling of shape's children)
		wxSFShapeBase::Scale(x, y, children);
	}
}

void wxSFRectShape::FitToChildren()
{
    // HINT: overload it for custom actions...

    wxSFShapeBase* pChild;

    // get bounding box of the shape and children set be inside it
    wxRect chBB = GetBoundingBox();

    wxShapeListNode* node = (wxShapeListNode*)GetFirstChildNode();
    while(node)
    {
        pChild = node->GetData();

        if( pChild->GetStyle() & sfsALWAYS_INSIDE )
        {
            pChild->GetCompleteBoundingBox(chBB, bbSELF | bbCHILDREN);
        }
        node = node->GetNext();
    }

	//GetCompleteBoundingBox(chBB, bbSELF | bbCHILDREN);

	if(!chBB.IsEmpty())
	{
		wxRect shpBB = GetBoundingBox();

		if(!shpBB.Contains(chBB))
		{
			double dx = chBB.GetLeft() - shpBB.GetLeft();
			double dy = chBB.GetTop() - shpBB.GetTop();

			// resize parent shape
			shpBB.Union(chBB);
			MoveTo(shpBB.GetPosition().x, shpBB.GetPosition().y);
			m_nRectSize = wxRealPoint(shpBB.GetSize().x, shpBB.GetSize().y);

			// move its "1st level" children if neccessary
			if((dx < 0) || (dy < 0))
			{
				node = (wxShapeListNode*)GetFirstChildNode();
				while(node)
				{
					pChild = node->GetData();
					if(dx < 0)pChild->MoveBy(abs((int)dx), 0);
					if(dy < 0)pChild->MoveBy(0, abs((int)dy));

					node = node->GetNext();
				}
			}
		}

		// perform the action recursively
		/*wxSFShapeBase* m_pParentShape = GetParentShape();
		if(m_pParentShape)m_pParentShape->FitToChildren();*/
	}
}

//----------------------------------------------------------------------------------//
// protected virtual functions
//----------------------------------------------------------------------------------//

void wxSFRectShape::DrawNormal(wxSFScaledPaintDC &dc)
{
	// HINT: overload it for custom actions...

	dc.SetPen(m_Border);
	dc.SetBrush(m_Fill);
	dc.DrawRectangle(GetAbsolutePosition(), m_nRectSize);
	dc.SetBrush(wxNullBrush);
	dc.SetPen(wxNullPen);
}

void wxSFRectShape::DrawHover(wxSFScaledPaintDC &dc)
{
	// HINT: overload it for custom actions...

	dc.SetPen(wxPen(m_nHoverColor, 1));
	dc.SetBrush(m_Fill);
	dc.DrawRectangle(GetAbsolutePosition(), m_nRectSize);
	dc.SetBrush(wxNullBrush);
	dc.SetPen(wxNullPen);
}

void wxSFRectShape::DrawHighlighted(wxSFScaledPaintDC &dc)
{
	// HINT: overload it for custom actions...

	dc.SetPen(wxPen(m_nHoverColor, 2));
	dc.SetBrush(m_Fill);
	dc.DrawRectangle(GetAbsolutePosition(), m_nRectSize);
	dc.SetBrush(wxNullBrush);
	dc.SetPen(wxNullPen);
}

void wxSFRectShape::OnRightHandle(wxSFShapeHandle& handle)
{
	// HINT: overload it for custom actions...

	m_nRectSize.x = handle.GetPosition().x - GetAbsolutePosition().x;
}

void wxSFRectShape::OnLeftHandle(wxSFShapeHandle& handle)
{
	// HINT: overload it for custom actions...

    wxSFShapeBase *pChild;

	double dx = (double)handle.GetPosition().x - GetAbsolutePosition().x;

	// update position of children
	wxShapeListNode *node = (wxShapeListNode*)GetFirstChildNode();
	while(node)
	{
	    pChild = node->GetData();
	    if( pChild->GetHAlign() == halignNONE )
	    {
            pChild->MoveBy(-dx, 0);
	    }
		node = node->GetNext();
	}
	// update position and size of the shape
	m_nRectSize.x -= dx;
	m_nRelativePosition.x += dx;
}

void wxSFRectShape::OnTopHandle(wxSFShapeHandle& handle)
{
	// HINT: overload it for custom actions...

    wxSFShapeBase *pChild;

	double dy = (double)handle.GetPosition().y - GetAbsolutePosition().y;

	// update position of children
	wxShapeListNode *node = (wxShapeListNode*)GetFirstChildNode();
	while(node)
	{
	    pChild = node->GetData();
	    if( pChild->GetVAlign() == valignNONE )
	    {
            pChild->MoveBy(0, -dy);
	    }
		node = node->GetNext();
	}
	// update position and size of the shape
	m_nRectSize.y -= dy;
	m_nRelativePosition.y += dy;
}

void wxSFRectShape::OnBottomHandle(wxSFShapeHandle& handle)
{
	// HINT: overload it for custom actions...

	m_nRectSize.y = handle.GetPosition().y - GetAbsolutePosition().y;
}

wxRealPoint wxSFRectShape::GetBorderPoint(const wxRealPoint& to)
{
    // HINT: override it for custom actions ...

    // the function calculates intesection of line leading from the shape center to
    // given point with the shape's bounding box;

    wxRealPoint intersection, center = GetCenter();
    wxRect bbRct = this->GetBoundingBox();

    /*wxRealPoint uto = to;

    if( bbRct.Contains(wxPoint(to.x, to.y)) )
    {
        wxRealPoint rp = center - to;

        if( abs(rp.x) > abs(rp.y) )
        {
            if( rp.x > 0 )
                uto = wxRealPoint(bbRct.GetLeft() - 5, to.y);
            else
                uto = wxRealPoint(bbRct.GetRight() + 5, to.y);
        }
        else
        {
            if( rp.y > 0 ) uto = wxRealPoint(to.x, bbRct.GetTop() - 5);
            else
                uto = wxRealPoint(to.x, bbRct.GetBottom() + 5);
        }
    }*/

    if(LinesIntersection(wxRealPoint(bbRct.GetTopLeft().x, bbRct.GetTopLeft().y),
                             wxRealPoint(bbRct.GetTopRight().x, bbRct.GetTopRight().y), center, to, intersection)) return intersection;

    else if(LinesIntersection(wxRealPoint(bbRct.GetTopRight().x, bbRct.GetTopRight().y),
                                  wxRealPoint(bbRct.GetBottomRight().x, bbRct.GetBottomRight().y + 1), center, to, intersection)) return intersection;

    else if(LinesIntersection(wxRealPoint(bbRct.GetBottomRight().x, bbRct.GetBottomRight().y + 1),
                                  wxRealPoint(bbRct.GetBottomLeft().x, bbRct.GetBottomLeft().y + 1), center, to, intersection)) return intersection;

    else if(LinesIntersection(wxRealPoint(bbRct.GetBottomLeft().x, bbRct.GetBottomLeft().y + 1),
                                  wxRealPoint(bbRct.GetTopLeft().x, bbRct.GetTopLeft().y), center, to, intersection)) return intersection;

    return center;
}

//----------------------------------------------------------------------------------//
// public virtual functions
//----------------------------------------------------------------------------------//

void wxSFRectShape::CreateHandles()
{
	// HINT: overload it for custom actions...

	AddHandle(wxSFShapeHandle::hndLEFTTOP);
	AddHandle(wxSFShapeHandle::hndTOP);
	AddHandle(wxSFShapeHandle::hndRIGHTTOP);
	AddHandle(wxSFShapeHandle::hndRIGHT);
	AddHandle(wxSFShapeHandle::hndRIGHTBOTTOM);
	AddHandle(wxSFShapeHandle::hndBOTTOM);
	AddHandle(wxSFShapeHandle::hndLEFTBOTTOM);
	AddHandle(wxSFShapeHandle::hndLEFT);
	AddHandle(wxSFShapeHandle::hndLEFTTOP);
}

void wxSFRectShape::OnHandle(wxSFShapeHandle& handle)
{
    // HINT: overload it for custom actions...

	switch(handle.GetType())
	{
	case wxSFShapeHandle::hndLEFT:
		OnLeftHandle(handle);
		break;

	case wxSFShapeHandle::hndLEFTTOP:
		OnLeftHandle(handle);
		OnTopHandle(handle);
		break;

	case wxSFShapeHandle::hndLEFTBOTTOM:
		OnLeftHandle(handle);
		OnBottomHandle(handle);
		break;

	case wxSFShapeHandle::hndRIGHT:
		OnRightHandle(handle);
		break;

	case wxSFShapeHandle::hndRIGHTTOP:
		OnRightHandle(handle);
		OnTopHandle(handle);
		break;

	case wxSFShapeHandle::hndRIGHTBOTTOM:
		OnRightHandle(handle);
		OnBottomHandle(handle);
		break;

	case wxSFShapeHandle::hndTOP:
		OnTopHandle(handle);
		break;

	case wxSFShapeHandle::hndBOTTOM:
		OnBottomHandle(handle);
		break;

    default:
        break;
	}
}

void wxSFRectShape::OnBeginHandle(wxSFShapeHandle& handle)
{
    m_nPrevPosition = m_nRelativePosition;
    m_nPrevSize = m_nRectSize;
}
