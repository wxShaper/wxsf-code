/***************************************************************
 * Name:      CurveShape.cpp
 * Purpose:   Implements curve shape class
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

#include <math.h>

#include "wx/wxsf/CurveShape.h"
#include "wx/wxsf/ShapeCanvas.h"
#include "wx/wxsf/CommonFcn.h"

XS_IMPLEMENT_CLONABLE_CLASS(wxSFCurveShape, wxSFLineShape);

wxSFCurveShape::wxSFCurveShape() : wxSFLineShape()
{
    m_nMaxSteps = sfdvCURVESHAPE_MAXSTEPS;

    MarkSerializableDataMembers();
}

wxSFCurveShape::wxSFCurveShape(size_t maxsteps, long src, long trg, const RealPointList& path, wxSFDiagramManager* manager)
: wxSFLineShape(src, trg, path, manager)
{
    m_nMaxSteps = maxsteps;

    MarkSerializableDataMembers();
}

wxSFCurveShape::wxSFCurveShape(const wxSFCurveShape& obj)
: wxSFLineShape(obj)
{
    m_nMaxSteps = obj.m_nMaxSteps;

	MarkSerializableDataMembers();
}

wxSFCurveShape::~wxSFCurveShape()
{

}

void wxSFCurveShape::MarkSerializableDataMembers()
{
	XS_SERIALIZE_EX(m_nMaxSteps, wxT("max_steps"), sfdvCURVESHAPE_MAXSTEPS);
}

//----------------------------------------------------------------------------------//
// public virtual functions
//----------------------------------------------------------------------------------//

wxRect wxSFCurveShape::GetBoundingBox()
{
    return wxSFLineShape::GetBoundingBox().Inflate(20, 20);
}

//----------------------------------------------------------------------------------//
// public functions
//----------------------------------------------------------------------------------//

wxRealPoint wxSFCurveShape::GetPoint(size_t segment, double offset)
{
    LineSegmentArray arrLines;
    GetUpdatedLineSegment(arrLines);

    if((segment < arrLines.Count()-1) && (segment > 0))
    {
        return Coord_Catmul_Rom_Kubika(arrLines.Item(segment-1).m_nSrc, arrLines.Item(segment-1).m_nTrg, arrLines.Item(segment).m_nTrg, arrLines.Item(segment+1).m_nTrg, offset);
    }
    else
        return wxRealPoint();
}

//----------------------------------------------------------------------------------//
// protected virtual functions
//----------------------------------------------------------------------------------//

void wxSFCurveShape::DrawCompleteLine(wxDC& dc)
{
    int i = 0;
    LineSegmentArray arrLines;
    GetUpdatedLineSegment(arrLines);

    switch(m_nMode)
    {
    case modeREADY:
        {
            // draw basic line parts
			if( arrLines.Count() > 3 )
			{
				for(i = 0; i < (int)arrLines.Count()-2; i++)
					Catmul_Rom_Kubika(arrLines.Item(i).m_nSrc, arrLines.Item(i).m_nTrg, arrLines.Item(i+1).m_nTrg, arrLines.Item(i+2).m_nTrg, dc);
			}
			else
                dc.DrawLine(Conv2Point(arrLines[1].m_nSrc), Conv2Point(arrLines[1].m_nTrg));

            // draw source arrow
            if(m_pSrcArrow)m_pSrcArrow->Draw(arrLines[1].m_nTrg, arrLines[1].m_nSrc, dc);
            // draw target arrow
            if(m_pTrgArrow)m_pTrgArrow->Draw(arrLines[arrLines.Count()-2].m_nSrc, arrLines[arrLines.Count()-2].m_nTrg, dc);
        }
        break;

    case modeUNDERCONSTRUCTION:
        {
            // draw basic line parts
            for(i = 0; i < (int)arrLines.Count()-2; i++)
                Catmul_Rom_Kubika(arrLines.Item(i).m_nSrc, arrLines.Item(i).m_nTrg, arrLines.Item(i+1).m_nTrg, arrLines.Item(i+2).m_nTrg, dc);

            // draw unfinished line segment if any (for interactive line creation)
            dc.SetPen(wxPen(*wxBLACK, 1, wxDOT));
            if( arrLines.Count() > 1 ) dc.DrawLine(Conv2Point(arrLines[i].m_nTrg), m_nUnfinishedPoint);
            else if(m_nSrcShapeId != -1)
            {
                wxSFShapeBase* pSrcShape = GetShapeManager()->FindShape(m_nSrcShapeId);
                if( pSrcShape ) dc.DrawLine( Conv2Point(pSrcShape->GetBorderPoint(wxRealPoint(m_nUnfinishedPoint.x, m_nUnfinishedPoint.y))), m_nUnfinishedPoint);
            }
            dc.SetPen(wxNullPen);
        }
        break;

    case modeSRCCHANGE:
        {
            // draw basic line parts
            for(i = 1; i < (int)arrLines.Count()-2; i++)
                Catmul_Rom_Kubika(arrLines.Item(i).m_nSrc, arrLines.Item(i).m_nTrg, arrLines.Item(i+1).m_nTrg, arrLines.Item(i+2).m_nTrg, dc);

            // draw linesegment being updated
            dc.SetPen(wxPen(*wxBLACK, 1, wxDOT));
            dc.DrawLine(m_nUnfinishedPoint, Conv2Point(arrLines[1].m_nTrg));
            dc.SetPen(wxNullPen);
        }
        break;

    case modeTRGCHANGE:
        {
            // draw basic line parts
            for(i = 0; i < (int)arrLines.Count()-3; i++)
                Catmul_Rom_Kubika(arrLines.Item(i).m_nSrc, arrLines.Item(i).m_nTrg, arrLines.Item(i+1).m_nTrg, arrLines.Item(i+2).m_nTrg, dc);

            // draw linesegment being updated
            dc.SetPen(wxPen(*wxBLACK, 1, wxDOT));
            dc.DrawLine(m_nUnfinishedPoint, Conv2Point(arrLines[arrLines.Count()-2].m_nSrc));
            dc.SetPen(wxNullPen);
        }
        break;
    }
}

int wxSFCurveShape::GetHitLinesegment(const wxPoint& pos)
{
    if(!GetBoundingBox().Contains(pos))return -1;

    wxRect lsBB;
    wxRealPoint ptSrc, ptTrg;
    double d, a, b, c;

    // Get all polyline segments
    LineSegmentArray m_arrLineSegments;
    GetUpdatedLineSegment(m_arrLineSegments);

    // test whether given point lies near the line segment
    for(int i=1; i < (int)m_arrLineSegments.Count()-1; i++)
    {
        ptSrc = m_arrLineSegments[i].m_nSrc;
        ptTrg = m_arrLineSegments[i].m_nTrg;

        // calculate line segment bounding box
        lsBB = wxRect(Conv2Point(ptSrc), Conv2Point(ptTrg));
        if( (i > 1) && (i < (int)m_arrLineSegments.Count()-2) )lsBB.Inflate(10);

        // convert line segment to its parametric form
        a = ptTrg.y - ptSrc.y;
        b = ptSrc.x - ptTrg.x;
        c = -a*ptSrc.x - b*ptSrc.y;

        // calculate distance of the line and give point
        d = (a*pos.x + b*pos.y + c)/sqrt(a*a + b*b);
        if((abs((int)d) <= 10) && lsBB.Contains(pos)) return i-1;
    }
    return -1;
}


//----------------------------------------------------------------------------------//
// private functions
//----------------------------------------------------------------------------------//

void wxSFCurveShape::GetUpdatedLineSegment(LineSegmentArray& segments)
{
    if(m_pParentManager)
    {
        // get normal line segment
        GetLineSegments(segments);
        if(segments.Count() > 0)
        {
            wxSFShapeBase* pSrcShape = GetShapeManager()->FindShape(m_nSrcShapeId);
            wxSFShapeBase* pTrgShape = GetShapeManager()->FindShape(m_nTrgShapeId);

            // prepend and append new line segmets
            if(pSrcShape)
            {
                segments.Insert(new LineSegment(pSrcShape->GetCenter(), segments.Item(0).m_nSrc), 0);
            }

            if(m_nMode == modeUNDERCONSTRUCTION)
            {
                segments.Add(new LineSegment(segments.Item(segments.Count()-1).m_nTrg, wxRealPoint(m_nUnfinishedPoint.x, m_nUnfinishedPoint.y)));
            }
            else if(pTrgShape)
            {
                segments.Add(new LineSegment(segments.Item(segments.Count()-1).m_nTrg, pTrgShape->GetCenter()));
            }

        }
    }
}

void wxSFCurveShape::Catmul_Rom_Kubika(const wxRealPoint& A, const wxRealPoint& B, const wxRealPoint& C, const wxRealPoint& D, wxDC& dc)
{
	// the begginig of the curve is in the B point
	wxRealPoint point0=B;
	wxRealPoint point1;
	int nMaxPPI = 72;

    // optimize step count due to smooth curve drawing even on DC with high PPI and the scale value.
	wxSize ppi = dc.GetPPI();
	if( ppi.x > ppi.y )nMaxPPI = ppi.x; else nMaxPPI = ppi.y;

	long nOptimSteps = (int)((double)m_nMaxSteps * (double)(nMaxPPI/72));// * GetParentCanvas()->GetScale());
	if(nOptimSteps<2)nOptimSteps=2;

    // draw the curve
	for(double t = 0; t <= (1 + (1.0f / nOptimSteps)); t += 1.0f / (nOptimSteps-1))
	{
		point1 = Coord_Catmul_Rom_Kubika(A,B,C,D,t);
		dc.DrawLine((int)point0.x, (int)point0.y, (int)point1.x, (int)point1.y);
		point0 = point1;
	}
	point1 = Coord_Catmul_Rom_Kubika(A,B,C,D,1);
	dc.DrawLine((int)point0.x, (int)point0.y, (int)point1.x, (int)point1.y);
}

wxRealPoint wxSFCurveShape::Coord_Catmul_Rom_Kubika(const wxRealPoint& p1, const wxRealPoint& p2, const wxRealPoint& p3, const wxRealPoint& p4, double t)
{
	double pom1, pom2;
	double C1,C2,C3,C4;
    wxRealPoint point;

	// auxiliary variables
	pom1 = t - 1;
	pom2 = t * t;

	// used polynoms
    C1 = (-pom2*t + 2*pom2 - t)  / 2;
    C2 = (3*pom2*t - 5*pom2 + 2) / 2;
    C3 = (-3*pom2*t + 4*pom2 +t) / 2;
    C4 = pom1*pom2 / 2;

	// calculation of curve point for t = <0,1>
	point.x = C1*p1.x + C2*p2.x + C3*p3.x + C4*p4.x;
    point.y = C1*p1.y + C2*p2.y + C3*p3.y + C4*p4.y;
	//point.z = C1*p1.z + C2*p2.z + C3*p3.z + C4*p4.z;

    return point;
}



