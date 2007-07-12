#include "CircleShape.h"
#include "CommonFcn.h"

IMPLEMENT_DYNAMIC_CLASS(wxSFCircleShape, wxSFSquareShape);

wxSFCircleShape::wxSFCircleShape(void)
: wxSFSquareShape()
{
    SetRectSize(50, 50);
}

wxSFCircleShape::wxSFCircleShape(const wxRealPoint& pos, double radius, long parentId, wxSFShapeCanvas* canvas)
: wxSFSquareShape(pos, radius, parentId, canvas)
{

}

wxSFCircleShape::wxSFCircleShape(wxSFCircleShape& obj)
: wxSFSquareShape(obj)
{

}

wxSFCircleShape::~wxSFCircleShape()
{

}

//----------------------------------------------------------------------------------//
// public virtual functions
//----------------------------------------------------------------------------------//

wxRealPoint wxSFCircleShape::GetBorderPoint(const wxRealPoint& to)
{
    wxRealPoint center = GetAbsolutePosition() + wxRealPoint(m_nRectSize.x/2, m_nRectSize.y/2);
	double dist = Distance(center, to);

	if(dist)
	{
		double srcDx = m_nRectSize.x/2*(to.x-center.x)/dist;
		double srcDy = m_nRectSize.y/2*(to.y-center.y)/dist;
		return wxRealPoint(center.x + srcDx, center.y + srcDy);
	}
	else
		return center;

}

bool wxSFCircleShape::IsInside(const wxPoint& pos)
{
    wxRealPoint center = GetAbsolutePosition() + wxRealPoint(m_nRectSize.x/2, m_nRectSize.y/2);

    if(Distance(center, wxRealPoint(pos.x, pos.y)) <= m_nRectSize.x/2) return true;
    else
        return false;

}

//----------------------------------------------------------------------------------//
// protected virtual functions
//----------------------------------------------------------------------------------//

void wxSFCircleShape::DrawNormal(wxSFScaledPaintDC& dc)
{
	// HINT: overload it for custom actions...

	wxRealPoint pos = GetAbsolutePosition();

	dc.SetPen(m_Border);
	dc.SetBrush(m_Fill);
	dc.DrawCircle(pos.x + m_nRectSize.x/2, pos.y + m_nRectSize.y/2, m_nRectSize.x/2);
	dc.SetBrush(wxNullBrush);
	dc.SetPen(wxNullPen);
}

void wxSFCircleShape::DrawHover(wxSFScaledPaintDC& dc)
{
	// HINT: overload it for custom actions...

	wxRealPoint pos = GetAbsolutePosition();

	dc.SetPen(wxPen(m_nHoverColor, 1));
	dc.SetBrush(m_Fill);
	dc.DrawCircle(pos.x + m_nRectSize.x/2, pos.y + m_nRectSize.y/2, m_nRectSize.x/2);
	dc.SetBrush(wxNullBrush);
	dc.SetPen(wxNullPen);
}

void wxSFCircleShape::DrawHighlighted(wxSFScaledPaintDC& dc)
{
	// HINT: overload it for custom actions...

    wxRealPoint pos = GetAbsolutePosition();

	dc.SetPen(wxPen(m_nHoverColor, 2));
	dc.SetBrush(m_Fill);
	dc.DrawCircle(pos.x + m_nRectSize.x/2, pos.y + m_nRectSize.y/2, m_nRectSize.x/2);
	dc.SetBrush(wxNullBrush);
	dc.SetPen(wxNullPen);
}



