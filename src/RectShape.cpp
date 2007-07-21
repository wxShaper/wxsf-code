#include "RectShape.h"
#include "ShapeCanvas.h"
#include "CommonFcn.h"

IMPLEMENT_DYNAMIC_CLASS(wxSFRectShape, wxSFShapeBase);

wxSFRectShape::wxSFRectShape(void) : wxSFShapeBase()
{
	m_nRectSize = sfdvRECTSHAPE_SIZE;
	m_Border = sfdvRECTSHAPE_BORDER;
	m_Fill = sfdvRECTSHAPE_FILL;
}

wxSFRectShape::wxSFRectShape(const wxRealPoint& pos, const wxRealPoint& size, long parentId, wxSFShapeCanvas* canvas)
: wxSFShapeBase(pos, parentId, canvas)
{
	m_nRectSize = size;
	m_Border = sfdvRECTSHAPE_BORDER;
	m_Fill = sfdvRECTSHAPE_FILL;
}

wxSFRectShape::wxSFRectShape(wxSFRectShape& obj) : wxSFShapeBase(obj)
{
	m_nRectSize = obj.m_nRectSize;
	m_Border = obj.m_Border;
	m_Fill = obj.m_Fill;
}

wxSFRectShape::~wxSFRectShape(void)
{
}

//----------------------------------------------------------------------------------//
// public virtual functions
//----------------------------------------------------------------------------------//

wxRect wxSFRectShape::GetBoundingBox()
{
	return wxRect(wxPoint((int)GetAbsolutePosition().x, (int)GetAbsolutePosition().y), wxSize((int)m_nRectSize.x, (int)m_nRectSize.y));
}

bool wxSFRectShape::IsInside(const wxPoint& pos)
{
	return wxRect(wxPoint((int)GetAbsolutePosition().x, (int)GetAbsolutePosition().y), wxSize((int)m_nRectSize.x, (int)m_nRectSize.y)).Contains(pos);
}

bool wxSFRectShape::Intersects(const wxRect& rct)
{
	return wxRect(wxPoint((int)GetAbsolutePosition().x, (int)GetAbsolutePosition().y), wxSize((int)m_nRectSize.x, (int)m_nRectSize.y)).Intersects(rct);
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

	wxRect chBB;
	GetCompleteBoundingBox(chBB, bbSELF | bbCHILDREN);

	if(!chBB.IsEmpty())
	{
		wxRect shpBB = GetBoundingBox();

		if(!shpBB.Contains(chBB))
		{
			wxSFShapeBase* pChild;
			double dx = chBB.GetLeft() - shpBB.GetLeft();
			double dy = chBB.GetTop() - shpBB.GetTop();

			// resize parent shape
			shpBB.Union(chBB);
			MoveTo(shpBB.GetPosition().x, shpBB.GetPosition().y);
			m_nRectSize = wxRealPoint(shpBB.GetSize().x, shpBB.GetSize().y);

			// move its "1st level" children if neccessary
			if((dx < 0) || (dy < 0))
			{
				CShapeList m_lstChildren;
				GetChildren(m_lstChildren);

				wxCShapeListNode* node = m_lstChildren.GetFirst();
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
		wxSFShapeBase* m_pParentShape = GetParentShape();
		if(m_pParentShape)m_pParentShape->FitToChildren();
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

	/*wxRect chBB;
	GetCompleteBoundingBox(chBB, bbCHILDREN | bbOMITALIGNED);

	if(!chBB.IsEmpty())
	{
		if(handle.GetPosition().x <= chBB.GetRight())return;
	}*/

	m_nRectSize.x = handle.GetPosition().x - GetAbsolutePosition().x;
	//m_nRectSize.x = m_nPrevSize.x + handle.GetTotalDelta().x;
}

void wxSFRectShape::OnLeftHandle(wxSFShapeHandle& handle)
{
	// HINT: overload it for custom actions...

    wxSFShapeBase *pChild;
	/*wxRect chBB;
	GetCompleteBoundingBox(chBB, bbCHILDREN | bbOMITALIGNED);

	if(!chBB.IsEmpty())
	{
		if(handle.GetPosition().x > chBB.GetLeft())return;
	}*/

	double dx = (double)handle.GetPosition().x - GetAbsolutePosition().x;

	// update position of children
	CShapeList m_lstChildren;
	GetChildren(m_lstChildren);

	wxCShapeListNode *node = m_lstChildren.GetFirst();
	while(node)
	{
	    pChild = node->GetData();
	    if( pChild->GetHAlign() == halignNONE )
	    {
            node->GetData()->MoveBy(-dx, 0);
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
	/*wxRect chBB;
	GetCompleteBoundingBox(chBB, bbCHILDREN | bbOMITALIGNED);

	if(!chBB.IsEmpty())
	{
		if(handle.GetPosition().y > chBB.GetTop())return;
	}*/

	double dy = (double)handle.GetPosition().y - GetAbsolutePosition().y;

	// update position of children
	CShapeList m_lstChildren;
	GetChildren(m_lstChildren);

	wxCShapeListNode *node = m_lstChildren.GetFirst();
	while(node)
	{
	    pChild = node->GetData();
	    if( pChild->GetVAlign() == valignNONE )
	    {
            node->GetData()->MoveBy(0, -dy);
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

	/*wxRect chBB;
	GetCompleteBoundingBox(chBB, bbCHILDREN | bbOMITALIGNED);

	if(!chBB.IsEmpty())
	{
		if(handle.GetPosition().y <= chBB.GetBottom())return;
	}*/

	m_nRectSize.y = handle.GetPosition().y - GetAbsolutePosition().y;
}

wxRealPoint wxSFRectShape::GetBorderPoint(const wxRealPoint& to)
{
    // HINT: override it for custom actions ...

    // the function calculates intesection of line leading from the shape center to
    // given point with the shape's bounding box;

    wxRealPoint intersection, center = GetCenter();
    wxRect bbRct = this->GetBoundingBox();

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
