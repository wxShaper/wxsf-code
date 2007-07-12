#include "LineShape.h"
#include "ShapeCanvas.h"
#include "CommonFcn.h"

#include <wx/listimpl.cpp>
#include <wx/arrimpl.cpp>

WX_DEFINE_LIST(CPointList);
WX_DEFINE_OBJARRAY(CLineSegmentArray);

IMPLEMENT_DYNAMIC_CLASS(wxSFLineShape, wxSFShapeBase);

// arrow shape
const wxRealPoint arrow[3]={wxRealPoint(0,0), wxRealPoint(10,4), wxRealPoint(10,-4)};

wxSFLineShape::wxSFLineShape(void)
{
	m_nSrcShapeId = sfdvLINESHAPE_UNKNOWNID;
	m_nTrgShapeId = sfdvLINESHAPE_UNKNOWNID;
	m_pSrcArrow = NULL;
	m_pTrgArrow = NULL;

	m_nDockPoint = sfdvLINESHAPE_DOCKPOINT;
	m_Pen = sfdvLINESHAPE_PEN;

	m_nMode = modeREADY;

	m_nSerializeMask &= ~sfsfBASESHAPE_POSITION;

	m_lstPoints.DeleteContents(true);
}

wxSFLineShape::wxSFLineShape(long src, long trg, const CPointList& path, wxSFShapeCanvas* canvas)
: wxSFShapeBase(wxRealPoint(0, 0), -1, canvas)
{
	m_nSrcShapeId = src;
	m_nTrgShapeId = trg;
	m_nDockPoint = sfdvLINESHAPE_DOCKPOINT;

	m_pSrcArrow = NULL;
	m_pTrgArrow = NULL;

	m_Pen = sfdvLINESHAPE_PEN;

	m_nMode = modeREADY;

	wxCPointListNode* node = path.GetFirst();
	while(node)
	{
		m_lstPoints.Append(new wxRealPoint(*node->GetData()));
		node = node->GetNext();
	}

	m_nSerializeMask &= ~sfsfBASESHAPE_POSITION;

	m_lstPoints.DeleteContents(true);
}

wxSFLineShape::wxSFLineShape(wxSFLineShape& obj)
: wxSFShapeBase(obj)
{
	m_nSrcShapeId = obj.m_nSrcShapeId;
	m_nTrgShapeId = obj.m_nTrgShapeId;
	m_nDockPoint = obj.m_nDockPoint;

	if(obj.m_pSrcArrow)
	{
		m_pSrcArrow = obj.m_pSrcArrow->Clone();
	}
	else
		m_pSrcArrow = NULL;

	if(obj.m_pTrgArrow)
	{
		m_pTrgArrow = obj.m_pTrgArrow->Clone();
	}
	else
		m_pTrgArrow = NULL;

	m_Pen = obj.m_Pen;

	m_nMode = obj.m_nMode;

	wxCPointListNode* node = obj.m_lstPoints.GetFirst();
	while(node)
	{
		m_lstPoints.Append(new wxRealPoint(*node->GetData()));
		node = node->GetNext();
	}

	m_lstPoints.DeleteContents(true);
}

wxSFLineShape::~wxSFLineShape(void)
{
	m_lstPoints.Clear();

	if(m_pSrcArrow)delete m_pSrcArrow;
	if(m_pTrgArrow)delete m_pTrgArrow;
}

//----------------------------------------------------------------------------------//
// public functions
//----------------------------------------------------------------------------------//

void wxSFLineShape::SetSrcArrow(wxSFArrowBase* arrow)
{
	if(m_pSrcArrow)delete m_pSrcArrow;

	m_pSrcArrow = arrow;
	if(m_pSrcArrow)
	{
		m_pSrcArrow->SetParentShape(this);
	}
}

wxSFArrowBase* wxSFLineShape::SetSrcArrow(wxClassInfo* arrowInfo)
{
	SetSrcArrow((wxSFArrowBase*)arrowInfo->CreateObject());

	return m_pSrcArrow;
}

void wxSFLineShape::SetTrgArrow(wxSFArrowBase* arrow)
{
	if(m_pTrgArrow)delete m_pTrgArrow;

	m_pTrgArrow = arrow;
	if(m_pTrgArrow)
	{
		m_pTrgArrow->SetParentShape(this);
	}
}

wxSFArrowBase* wxSFLineShape::SetTrgArrow(wxClassInfo* arrowInfo)
{
	SetTrgArrow((wxSFArrowBase*)arrowInfo->CreateObject());

	return m_pTrgArrow;
}

//----------------------------------------------------------------------------------//
// public virtual functions
//----------------------------------------------------------------------------------//

wxRealPoint wxSFLineShape::GetAbsolutePosition()
{
    wxCPointListNode* ptnode;

	int ptsCnt = (int)m_lstPoints.GetCount();

	if((m_nDockPoint >= 0) && (ptsCnt > m_nDockPoint))
	{
	    ptnode = m_lstPoints.Item(m_nDockPoint);
	    if( ptnode )return *ptnode->GetData();
	}
	else if(ptsCnt > 0)
	{
	    ptnode = m_lstPoints.Item(ptsCnt/2);
	    if( ptnode )return *ptnode->GetData();
	}

    return GetCenter();
}

wxRealPoint wxSFLineShape::GetBorderPoint(const wxRealPoint& to)
{
	return GetAbsolutePosition();
}

wxRect wxSFLineShape::GetBoundingBox()
{
	wxRect lineRct(0, 0, 0, 0);

    CLineSegmentArray m_arrSegments;
    GetLineSegments(m_arrSegments);

    // calculate control points area if they exist
    if(m_arrSegments.Count() > 0)
    {
        for(size_t i = 0; i < m_arrSegments.Count(); i++)
        {
            if(lineRct.IsEmpty())
            {
                lineRct = wxRect(wxPoint((int)m_arrSegments[i].m_nSrc.x, (int)m_arrSegments[i].m_nSrc.y),
                                 wxPoint((int)m_arrSegments[i].m_nTrg.x, (int)m_arrSegments[i].m_nTrg.y));
            }
            else
                lineRct.Union(wxRect(wxPoint((int)m_arrSegments[i].m_nSrc.x, (int)m_arrSegments[i].m_nSrc.y),
                              wxPoint((int)m_arrSegments[i].m_nTrg.x, (int)m_arrSegments[i].m_nTrg.y)));
        }
    }
    else
    {
        // include starting point
        if(m_nSrcShapeId != -1)
        {
           // wxRealPoint shpCenter = m_pParentCanvas->FindShape(m_nSrcShapeId)->GetCenter();
			wxRealPoint shpCenter = m_pParentCanvas->FindShape(m_nSrcShapeId)->GetBorderPoint(wxRealPoint(m_nUnfinishedPoint.x, m_nUnfinishedPoint.y));
            if(!lineRct.IsEmpty())
            {
                lineRct.Union(wxRect((int)shpCenter.x, (int)shpCenter.y, 1, 1));
            }
            else
                lineRct = wxRect((int)shpCenter.x, (int)shpCenter.y, 1, 1);
        }

        // include ending point
        if(m_nTrgShapeId != -1)
        {
            wxRealPoint shpCenter = m_pParentCanvas->FindShape(m_nTrgShapeId)->GetCenter();
            if(!lineRct.IsEmpty())
            {
                lineRct.Union(wxRect((int)shpCenter.x, (int)shpCenter.y, 1, 1));
            }
            else
                lineRct = wxRect((int)shpCenter.x, (int)shpCenter.y, 1, 1);
        }
    }

    // include unfinished point if the line is under construction
    if((m_nMode == modeUNDERCONSTRUCTION) || (m_nMode == modeSRCCHANGE) || (m_nMode == modeTRGCHANGE))
    {
	    if(!lineRct.IsEmpty())
	    {
            lineRct.Union(wxRect(m_nUnfinishedPoint.x, m_nUnfinishedPoint.y, 1, 1));
	    }
	    else
            lineRct = wxRect(m_nUnfinishedPoint.x, m_nUnfinishedPoint.y, 1, 1);
    }

	return lineRct;
}

wxRealPoint wxSFLineShape::GetDockPointPosition()
{
    if(m_lstPoints.GetCount() > (size_t)m_nDockPoint)
    {
        return *m_lstPoints.Item(m_nDockPoint)->GetData();
    }
    else if(m_lstPoints.GetCount() > 0)
    {
        return *m_lstPoints.GetFirst()->GetData();
    }
    else
        return GetCenter();
}

bool wxSFLineShape::IsInside(const wxPoint& pos)
{
    if(this->GetHitLinesegment(pos) >= 0)return true;
    else
        return false;
}

bool wxSFLineShape::Intersects(const wxRect& rct)
{
    // Get all polyline segments
    CLineSegmentArray m_arrLineSegments;
    GetLineSegments(m_arrLineSegments);

    // test whether any line segment lies inside given rectangle
    for(size_t i=0; i < m_arrLineSegments.Count(); i++)
    {
        if(rct.Contains((int)m_arrLineSegments.Item(i).m_nSrc.x, (int)m_arrLineSegments.Item(i).m_nSrc.y) &&
            rct.Contains((int)m_arrLineSegments.Item(i).m_nTrg.x, (int)m_arrLineSegments.Item(i).m_nTrg.y))
        {
            return true;
        }
    }

	return false;
}

void wxSFLineShape::Scale(double x, double y)
{
	wxRealPoint *pt;

	wxCPointListNode* node = m_lstPoints.GetFirst();
	while(node)
	{
		pt = node->GetData();

        pt->x *= x;
        pt->y *= y;

		node = node->GetNext();
	}
}

void wxSFLineShape::MoveTo(double x, double y)
{
	MoveBy(x - m_nPrevPosition.x, y - m_nPrevPosition.y);
	m_nPrevPosition.x = x;
	m_nPrevPosition.y = y;
}

void wxSFLineShape::MoveBy(double x, double y)
{
	wxRealPoint *pt;

	wxCPointListNode* node = m_lstPoints.GetFirst();
	while(node)
	{
		pt = node->GetData();

		pt->x += x;
		pt->y += y;

		node = node->GetNext();
	}
}

void wxSFLineShape::CreateHandles()
{
    // first clear all previously used handles and then create new ones
    m_lstHandles.Clear();

    // create control points handles
	for(size_t i = 0; i < m_lstPoints.GetCount(); i++)AddHandle(wxSFShapeHandle::hndLINECTRL, (int)i);

	// create border handles
	AddHandle(wxSFShapeHandle::hndLINESTART);
	AddHandle(wxSFShapeHandle::hndLINEEND);
}

void wxSFLineShape::OnHandle(wxSFShapeHandle& handle)
{
    switch(handle.GetType())
    {
    case wxSFShapeHandle::hndLINECTRL:
        {
            wxCPointListNode* node = m_lstPoints.Item(handle.GetId());
            if(node)
            {
                wxRealPoint* pt = node->GetData();
                pt->x = handle.GetPosition().x;
                pt->y = handle.GetPosition().y;
            }
        }
        break;

    case wxSFShapeHandle::hndLINEEND:
    case wxSFShapeHandle::hndLINESTART:
        {
            m_nUnfinishedPoint = handle.GetPosition();
        }
        break;

    default:
        break;
    }
}

void wxSFLineShape::OnBeginDrag(const wxPoint& pos)
{
	m_nPrevPosition = GetAbsolutePosition();
}

void wxSFLineShape::OnLeftDoubleClick(const wxPoint& pos)
{
    // HINT: override it for custom actions

    if(m_pParentCanvas)
    {
        // remove existing handle if exist otherwise create a new one at the
        // given position
        wxSFShapeHandle *pHandle = m_pParentCanvas->GetTopmostHandleAtPosition(pos);
        if(pHandle && (pHandle->GetParentShape() == this))
        {
            if(pHandle->GetType() == wxSFShapeHandle::hndLINECTRL)
                m_lstPoints.DeleteNode(m_lstPoints.Item(pHandle->GetId()));
        }
        else
        {
            m_lstPoints.Insert(this->GetHitLinesegment(pos), new wxRealPoint(pos.x, pos.y));
        }
        CreateHandles();
        ShowHandles(true);
    }
}

//----------------------------------------------------------------------------------//
// protected virtual functions
//----------------------------------------------------------------------------------//

void wxSFLineShape::DrawNormal(wxSFScaledPaintDC& dc)
{
	dc.SetPen(m_Pen);
	DrawCompleteLine(dc);
	dc.SetPen(wxNullPen);
}

void wxSFLineShape::DrawHover(wxSFScaledPaintDC& dc)
{
	dc.SetPen(wxPen(m_nHoverColor, 1));
	DrawCompleteLine(dc);
	dc.SetPen(wxNullPen);
}

void wxSFLineShape::DrawHighlighted(wxSFScaledPaintDC& dc)
{
	dc.SetPen(wxPen(m_nHoverColor, 2));
	DrawCompleteLine(dc);
	dc.SetPen(wxNullPen);
}

//----------------------------------------------------------------------------------//
// protected functions
//----------------------------------------------------------------------------------//

void wxSFLineShape::GetLineSegments(CLineSegmentArray& segments)
{
    //segments.Clear();
    segments.Alloc(m_lstPoints.GetCount()+1);

    if(m_pParentCanvas)
    {
        wxRealPoint *pt, prevPt;

        wxSFShapeBase* pSrcShape = m_pParentCanvas->FindShape(m_nSrcShapeId);
        wxSFShapeBase* pTrgShape = m_pParentCanvas->FindShape(m_nTrgShapeId);

		if(m_lstPoints.GetCount() > 0)
		{
			wxCPointListNode* node = m_lstPoints.GetFirst();
			while(node)
			{
				pt = node->GetData();

				if(node == m_lstPoints.GetFirst())
				{
					if(pSrcShape)segments.Add(new CLineSegment(pSrcShape->GetBorderPoint(*pt), *pt));
				}
				else
				{
					segments.Add(new CLineSegment(prevPt, *pt));
				}
				prevPt = *pt;

				node = node->GetNext();
			}

			if(pTrgShape)segments.Add(new CLineSegment(prevPt, pTrgShape->GetBorderPoint(prevPt)));
		}
		else
		{
			if(pSrcShape && pTrgShape)
			{
				segments.Add(new CLineSegment(pSrcShape->GetBorderPoint(pTrgShape->GetCenter()), pTrgShape->GetBorderPoint(pSrcShape->GetCenter())));
			}
		}
    }
}

void wxSFLineShape::DrawCompleteLine(wxSFScaledPaintDC& dc)
{
    size_t i;
	CLineSegmentArray arrLines;
	GetLineSegments(arrLines);

    switch(m_nMode)
    {

    case modeREADY:
        {
            // draw basic line parts
            for(i = 0; i < arrLines.Count(); i++)
            {
                dc.DrawLine(arrLines.Item(i).m_nSrc, arrLines.Item(i).m_nTrg);
            }
            // draw source arrow
			if(m_pSrcArrow)m_pSrcArrow->Draw(arrLines[0].m_nTrg, arrLines[0].m_nSrc, dc);
            // draw target arrow
            if(m_pTrgArrow)m_pTrgArrow->Draw(arrLines[arrLines.Count()-1].m_nSrc, arrLines[arrLines.Count()-1].m_nTrg, dc);
        }
        break;

    case modeUNDERCONSTRUCTION:
        {
            // draw basic line parts
            for(i = 0; i < arrLines.Count(); i++)
            {
                dc.DrawLine(arrLines.Item(i).m_nSrc, arrLines.Item(i).m_nTrg);
            }
            // draw unfinished line segment if any (for interactive line creation)
            dc.SetPen(wxPen(*wxBLACK, 1, wxDOT));
            if(arrLines.Count() > 0)
            {
                dc.DrawLine(arrLines.Item(i-1).m_nTrg, wxRealPoint(m_nUnfinishedPoint.x, m_nUnfinishedPoint.y));
            }
            else if(m_nSrcShapeId != -1)
            {
                wxSFShapeBase* pSrcShape = m_pParentCanvas->FindShape(m_nSrcShapeId);
                if(pSrcShape)
                {
                    wxRealPoint rpt = wxRealPoint(m_nUnfinishedPoint.x, m_nUnfinishedPoint.y);
                    dc.DrawLine(pSrcShape->GetBorderPoint(rpt), rpt);
                }
            }
            dc.SetPen(wxNullPen);
        }
        break;

    case modeSRCCHANGE:
        {
            // draw basic line parts
            for(i = 1; i < arrLines.Count(); i++)
            {
                dc.DrawLine(arrLines.Item(i).m_nSrc, arrLines.Item(i).m_nTrg);
            }
            // draw linesegment being updated
            dc.SetPen(wxPen(*wxBLACK, 1, wxDOT));
            dc.DrawLine(wxRealPoint(m_nUnfinishedPoint.x, m_nUnfinishedPoint.y), arrLines.Item(0).m_nTrg);
            dc.SetPen(wxNullPen);
        }
        break;

    case modeTRGCHANGE:
        {
            // draw basic line parts
            for(i = 0; i < arrLines.Count()-1; i++)
            {
                dc.DrawLine(arrLines.Item(i).m_nSrc, arrLines.Item(i).m_nTrg);
            }
            // draw linesegment being updated
            dc.SetPen(wxPen(*wxBLACK, 1, wxDOT));
            dc.DrawLine(wxRealPoint(m_nUnfinishedPoint.x, m_nUnfinishedPoint.y), arrLines.Item(arrLines.Count()-1).m_nSrc);
            dc.SetPen(wxNullPen);
        }
        break;

    }
}

int wxSFLineShape::GetHitLinesegment(const wxPoint& pos)
{
    if(!GetBoundingBox().Inflate(10, 10).Contains(pos))return -1;

    double a, b, c, d;
    wxRealPoint ptSrc, ptTrg;
    wxRect lsBB;

    // Get all polyline segments
    CLineSegmentArray m_arrLineSegments;
    GetLineSegments(m_arrLineSegments);

    // test whether given point lies near the line segment
    for(size_t i=0; i < m_arrLineSegments.Count(); i++)
    {
        ptSrc = m_arrLineSegments.Item(i).m_nSrc;
        ptTrg = m_arrLineSegments.Item(i).m_nTrg;

        // calculate line segment bounding box
        lsBB = wxRect(wxPoint((int)ptSrc.x, (int)ptSrc.y), wxPoint((int)ptTrg.x, (int)ptTrg.y));
        lsBB.Inflate(10);

        // convert line segment to its parametric form
        a = ptTrg.y - ptSrc.y;
        b = ptSrc.x - ptTrg.x;
        c = -a*ptSrc.x - b*ptSrc.y;

        // calculate distance of the line and give point
        d = (a*pos.x + b*pos.y + c)/sqrt(a*a + b*b);
        if((abs((int)d) <= 10) && lsBB.Contains(pos)) return (int)i;
    }
    return -1;
}

