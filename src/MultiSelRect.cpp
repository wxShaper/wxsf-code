/***************************************************************
 * Name:      MultiSelRect.cpp
 * Purpose:   Implements aux. multiselection shape class
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

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

	if(GetParentCanvas())
	{
		CShapeList lstShapes;
		GetParentCanvas()->GetSelectedShapes(lstShapes);

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

	if(GetParentCanvas())
	{
		CShapeList lstShapes;
		GetParentCanvas()->GetSelectedShapes(lstShapes);

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
	if(GetParentCanvas())
	{
	    wxSFShapeBase* pShape;
		CShapeList m_lstSelection;
		GetParentCanvas()->GetSelectedShapes(m_lstSelection);

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
	if(GetParentCanvas())
	{
	    wxSFShapeBase* pShape;
		CShapeList m_lstSelection;
		GetParentCanvas()->GetSelectedShapes(m_lstSelection);

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

//----------------------------------------------------------------------------------//
// public virtual functions
//----------------------------------------------------------------------------------//

void wxSFMultiSelRect::OnRightHandle(wxSFShapeHandle& handle)
{
	if(GetParentCanvas() && !AnyWidthExceeded(handle.GetDelta()))
	{
	    wxCPointListNode* ptnode;
	    wxSFLineShape* pLine;
	    wxRealPoint* pt;

		double dx, sx = (GetRectSize().x - 2*MEOFFSET + handle.GetDelta().x)/(GetRectSize().x - 2*MEOFFSET);

		CShapeList m_lstSelection;
		GetParentCanvas()->GetSelectedShapes(m_lstSelection);

		wxCShapeListNode *node = m_lstSelection.GetFirst();
		while(node)
		{
			wxSFShapeBase* pShape = node->GetData();

			// scale main parent shape
			if(!pShape->IsKindOf(CLASSINFO(wxSFLineShape)))
			{
			    dx = (pShape->GetAbsolutePosition().x - (GetAbsolutePosition().x + MEOFFSET))/(GetRectSize().x - 2*MEOFFSET)*handle.GetDelta().x;

				pShape->Scale(sx, 1, sfWITHCHILDREN);

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

			node = node->GetNext();
		}
	}
}

void wxSFMultiSelRect::OnLeftHandle(wxSFShapeHandle& handle)
{
	if(GetParentCanvas() && !AnyWidthExceeded(wxPoint(-handle.GetDelta().x, 0)))
	{
	    wxCPointListNode* ptnode;
	    wxSFLineShape* pLine;
	    wxRealPoint* pt;

		double dx, sx = (GetRectSize().x - 2*MEOFFSET - handle.GetDelta().x)/(GetRectSize().x - 2*MEOFFSET);

		CShapeList m_lstSelection;
		GetParentCanvas()->GetSelectedShapes(m_lstSelection);

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

                pShape->Scale(sx, 1, sfWITHCHILDREN);

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

			node = node->GetNext();
		}
	}
}

void wxSFMultiSelRect::OnBottomHandle(wxSFShapeHandle& handle)
{
	if(GetParentCanvas()  && !AnyHeightExceeded(handle.GetDelta()))
	{
        wxCPointListNode* ptnode;
	    wxSFLineShape* pLine;
	    wxRealPoint* pt;

		double dy, sy = (GetRectSize().y - 2*MEOFFSET + handle.GetDelta().y)/(GetRectSize().y - 2*MEOFFSET);

		CShapeList m_lstSelection;
		GetParentCanvas()->GetSelectedShapes(m_lstSelection);

		wxCShapeListNode *node = m_lstSelection.GetFirst();
		while(node)
		{
			wxSFShapeBase* pShape = node->GetData();

            if(!pShape->IsKindOf(CLASSINFO(wxSFLineShape)))
            {
                dy = (pShape->GetAbsolutePosition().y - (GetAbsolutePosition().y + MEOFFSET))/(GetRectSize().y - 2*MEOFFSET)*handle.GetDelta().y;

                pShape->Scale(1, sy, sfWITHCHILDREN);

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

            node = node->GetNext();
		}
	}
}

void wxSFMultiSelRect::OnTopHandle(wxSFShapeHandle& handle)
{
	if(GetParentCanvas()  && !AnyHeightExceeded(wxPoint(0, -handle.GetDelta().y)))
	{
	    wxCPointListNode* ptnode;
	    wxSFLineShape* pLine;
	    wxRealPoint* pt;

		double dy, sy = (GetRectSize().y - 2*MEOFFSET - handle.GetDelta().y)/(GetRectSize().y - 2*MEOFFSET);

		CShapeList m_lstSelection;
		GetParentCanvas()->GetSelectedShapes(m_lstSelection);

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

                pShape->Scale(1, sy, sfWITHCHILDREN);
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

            node = node->GetNext();
		}
	}
}
