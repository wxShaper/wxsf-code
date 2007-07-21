#include "MultiSelRect.h"
#include "ShapeCanvas.h"
#include "TextShape.h"
#include <math.h>

wxSFMultiSelRect::wxSFMultiSelRect(void)
{
	SetBorder(wxPen(wxColour(100, 100, 100), 1, wxDOT));
	SetFill(*wxTRANSPARENT_BRUSH);
}

wxSFMultiSelRect::~wxSFMultiSelRect(void)
{
}

//----------------------------------------------------------------------------------//
// public virtual functions
//----------------------------------------------------------------------------------//

void wxSFMultiSelRect::OnBeginHandle(wxSFShapeHandle& handle)
{
	// inform all selected shapes about begin of the handle dragging

	if(m_pParentCanvas)
	{
		CShapeList lstShapes;
		m_pParentCanvas->GetSelectedShapes(lstShapes);

		wxCShapeListNode* node = lstShapes.GetFirst();
		while(node)
		{
			node->GetData()->OnBeginHandle(handle);
			node = node->GetNext();
		}
	}
}

void wxSFMultiSelRect::OnEndHandle(wxSFShapeHandle& handle)
{
	// inform all selected shapes about end of the handle dragging

	if(m_pParentCanvas)
	{
		CShapeList lstShapes;
		m_pParentCanvas->GetSelectedShapes(lstShapes);

		wxCShapeListNode* node = lstShapes.GetFirst();
		while(node)
		{
			node->GetData()->OnEndHandle(handle);
			node = node->GetNext();
		}
	}
}

//----------------------------------------------------------------------------------//
// protected functions
//----------------------------------------------------------------------------------//

bool wxSFMultiSelRect::AnyWidthExceeded(const wxPoint& delta)
{
	if(m_pParentCanvas)
	{
	    wxSFShapeBase* pShape;
		CShapeList m_lstSelection;
		m_pParentCanvas->GetSelectedShapes(m_lstSelection);

		// first determine whether any shape in the selection exceeds its bounds
		wxCShapeListNode *node = m_lstSelection.GetFirst();
		while(node)
		{
		    pShape = node->GetData();

		    if(!pShape->IsKindOf(CLASSINFO(wxSFLineShape)))
                if((pShape->GetBoundingBox().GetWidth() + delta.x) <= 1)return true;

			node = node->GetNext();
		}
		return false;
	}
	return true;
}

bool wxSFMultiSelRect::AnyHeightExceeded(const wxPoint& delta)
{
	if(m_pParentCanvas)
	{
	    wxSFShapeBase* pShape;
		CShapeList m_lstSelection;
		m_pParentCanvas->GetSelectedShapes(m_lstSelection);

		// first determine whether any shape in the selection exceeds its bounds
		wxCShapeListNode *node = m_lstSelection.GetFirst();
		while(node)
		{
		    pShape = node->GetData();

		    if(!pShape->IsKindOf(CLASSINFO(wxSFLineShape)))
                if((pShape->GetBoundingBox().GetHeight() + delta.y) <= 1)return true;

			node = node->GetNext();
		}
		return false;
	}
	return true;
}

void wxSFMultiSelRect::ScaleChildren(wxSFShapeBase *parent, double sx, double sy)
{
	CShapeList m_lstChildren;
	parent->GetChildren(m_lstChildren, true);

	wxCShapeListNode *node = m_lstChildren.GetFirst();
	while(node)
	{
		wxSFShapeBase* pShape = node->GetData();

		if(CanScale(pShape))
		{
		    pShape->Scale(sx, sy);
            pShape->SetRelativePosition(pShape->GetRelativePosition().x*sx, pShape->GetRelativePosition().y*sy);
		}

        // re-align shapes which have set any alignment mode
		pShape->DoAlignment();

		node = node->GetNext();
	}
}

bool wxSFMultiSelRect::CanScale(wxSFShapeBase* shape)
{
	if(shape)
	{
		return shape->CanChangeSize() && !shape->IsKindOf(CLASSINFO(wxSFTextShape));
	}
	else
		return false;
}

//----------------------------------------------------------------------------------//
// public virtual functions
//----------------------------------------------------------------------------------//

void wxSFMultiSelRect::OnRightHandle(wxSFShapeHandle& handle)
{
	if(m_pParentCanvas && !AnyWidthExceeded(handle.GetDelta()))
	{
	    wxCPointListNode* ptnode;
	    wxSFLineShape* pLine;
	    wxRealPoint* pt;

		double dx, sx = (GetRectSize().x - 2*MEOFFSET + handle.GetDelta().x)/(GetRectSize().x - 2*MEOFFSET);

		CShapeList m_lstSelection;
		m_pParentCanvas->GetSelectedShapes(m_lstSelection);

		wxCShapeListNode *node = m_lstSelection.GetFirst();
		while(node)
		{
			wxSFShapeBase* pShape = node->GetData();

			// scale main parent shape
			if(!pShape->IsKindOf(CLASSINFO(wxSFLineShape)))
			{
			    dx = (pShape->GetAbsolutePosition().x - (GetAbsolutePosition().x + MEOFFSET))/(GetRectSize().x - 2*MEOFFSET)*handle.GetDelta().x;
                if(CanScale(pShape))
				{
					pShape->Scale(sx, 1);
				}
                if(pShape->CanChangePosition())pShape->MoveBy(dx, 0);
			}
			else
			{
			    if(pShape->CanChangePosition())
			    {
                    pLine = (wxSFLineShape*)pShape;
                    ptnode = pLine->GetControlPoints().GetFirst();
                    while(ptnode)
                    {
                        pt = ptnode->GetData();
                        dx = ( pt->x - (GetAbsolutePosition().x + MEOFFSET))/(GetRectSize().x - 2*MEOFFSET)*handle.GetDelta().x;
                        pt->x += dx;
                        pt->x = floor(pt->x);
                        ptnode = ptnode->GetNext();
                    }
			    }
			}

			// scale its children as well
			ScaleChildren(pShape, sx, 1);

			pShape->DoAlignment();

			node = node->GetNext();
		}
	}
}

void wxSFMultiSelRect::OnLeftHandle(wxSFShapeHandle& handle)
{
	if(m_pParentCanvas && !AnyWidthExceeded(wxPoint(-handle.GetDelta().x, 0)))
	{
	    wxCPointListNode* ptnode;
	    wxSFLineShape* pLine;
	    wxRealPoint* pt;

		double dx, sx = (GetRectSize().x - 2*MEOFFSET - handle.GetDelta().x)/(GetRectSize().x - 2*MEOFFSET);

		CShapeList m_lstSelection;
		m_pParentCanvas->GetSelectedShapes(m_lstSelection);

		wxCShapeListNode *node = m_lstSelection.GetFirst();
		while(node)
		{
			wxSFShapeBase* pShape = node->GetData();

            if(!pShape->IsKindOf(CLASSINFO(wxSFLineShape)))
            {
                if(pShape->CanChangePosition())
                {
                    if(pShape->GetParentShapeId() != -1)
                    {
                        pShape->SetRelativePosition(pShape->GetRelativePosition().x*sx, pShape->GetRelativePosition().y);
                    }
                    else
                    {
                        double dx = handle.GetDelta().x - (pShape->GetAbsolutePosition().x - (GetAbsolutePosition().x + MEOFFSET))/(GetRectSize().x - 2*MEOFFSET)*handle.GetDelta().x;
                        pShape->MoveBy(dx, 0);
                    }
                }
                if(CanScale(pShape))
				{
					pShape->Scale(sx, 1);
				}
            }
            else
			{
			    if(pShape->CanChangePosition())
			    {
                    pLine = (wxSFLineShape*)pShape;
                    ptnode = pLine->GetControlPoints().GetFirst();
                    while(ptnode)
                    {
                        pt = ptnode->GetData();
                        dx = handle.GetDelta().x - (pt->x - (GetAbsolutePosition().x + MEOFFSET))/(GetRectSize().x - 2*MEOFFSET)*handle.GetDelta().x;
                        pt->x += dx;
                        pt->x = floor(pt->x);
                        ptnode = ptnode->GetNext();
                    }
			    }
			}

			// scale its children as well
			ScaleChildren(pShape, sx, 1);

			pShape->DoAlignment();

			node = node->GetNext();
		}
	}
}

void wxSFMultiSelRect::OnBottomHandle(wxSFShapeHandle& handle)
{
	if(m_pParentCanvas  && !AnyHeightExceeded(handle.GetDelta()))
	{
        wxCPointListNode* ptnode;
	    wxSFLineShape* pLine;
	    wxRealPoint* pt;

		double dy, sy = (GetRectSize().y - 2*MEOFFSET + handle.GetDelta().y)/(GetRectSize().y - 2*MEOFFSET);

		CShapeList m_lstSelection;
		m_pParentCanvas->GetSelectedShapes(m_lstSelection);

		wxCShapeListNode *node = m_lstSelection.GetFirst();
		while(node)
		{
			wxSFShapeBase* pShape = node->GetData();

            if(!pShape->IsKindOf(CLASSINFO(wxSFLineShape)))
            {
                dy = (pShape->GetAbsolutePosition().y - (GetAbsolutePosition().y + MEOFFSET))/(GetRectSize().y - 2*MEOFFSET)*handle.GetDelta().y;
                if(CanScale(pShape))
				{
					pShape->Scale(1, sy);
				}
                if(pShape->CanChangePosition())pShape->MoveBy(0, dy);
            }
            else
            {
                if(pShape->CanChangePosition())
                {
                    pLine = (wxSFLineShape*)pShape;
                    ptnode = pLine->GetControlPoints().GetFirst();
                    while(ptnode)
                    {
                        pt = ptnode->GetData();
                        dy = ( pt->y - (GetAbsolutePosition().y + MEOFFSET))/(GetRectSize().y - 2*MEOFFSET)*handle.GetDelta().y;
                        pt->y += dy;
                        pt->y = floor(pt->y);
                        ptnode = ptnode->GetNext();
                    }
                }
            }

            // scale its children as well
            ScaleChildren(pShape, 1, sy);

            pShape->DoAlignment();

            node = node->GetNext();
		}
	}
}

void wxSFMultiSelRect::OnTopHandle(wxSFShapeHandle& handle)
{
	if(m_pParentCanvas  && !AnyHeightExceeded(wxPoint(0, -handle.GetDelta().y)))
	{
	    wxCPointListNode* ptnode;
	    wxSFLineShape* pLine;
	    wxRealPoint* pt;

		double dy, sy = (GetRectSize().y - 2*MEOFFSET - handle.GetDelta().y)/(GetRectSize().y - 2*MEOFFSET);

		CShapeList m_lstSelection;
		m_pParentCanvas->GetSelectedShapes(m_lstSelection);

		wxCShapeListNode *node = m_lstSelection.GetFirst();
		while(node)
		{
			wxSFShapeBase* pShape = node->GetData();

            if(!pShape->IsKindOf(CLASSINFO(wxSFLineShape)))
            {
                if(pShape->CanChangePosition())
                {
                    if(pShape->GetParentShapeId() != -1)
                    {
                        pShape->SetRelativePosition(pShape->GetRelativePosition().x, pShape->GetRelativePosition().y*sy);
                    }
                    else
                    {
                        double dy = handle.GetDelta().y - (pShape->GetAbsolutePosition().y - (GetAbsolutePosition().y + MEOFFSET))/(GetRectSize().y - 2*MEOFFSET)*handle.GetDelta().y;
                        pShape->MoveBy(0, dy);
                    }
                }
                if(CanScale(pShape))
				{
					pShape->Scale(1, sy);
				}
            }
            else
            {
                if(pShape->CanChangePosition())
                {
                    pLine = (wxSFLineShape*)pShape;
                    ptnode = pLine->GetControlPoints().GetFirst();
                    while(ptnode)
                    {
                        pt = ptnode->GetData();
                        dy = handle.GetDelta().y - (pt->y - (GetAbsolutePosition().y + MEOFFSET))/(GetRectSize().y - 2*MEOFFSET)*handle.GetDelta().y;
                        pt->y += dy;
                        pt->y = floor(pt->y);
                        ptnode = ptnode->GetNext();
                    }
                }
            }

            // scale its children as well
            ScaleChildren(pShape, 1, sy);

            pShape->DoAlignment();

            node = node->GetNext();
		}
	}
}
