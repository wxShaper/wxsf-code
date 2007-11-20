/***************************************************************
 * Name:      ShapeHandle.cpp
 * Purpose:   Implements shape handle class
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "wx_pch.h"

#include "wx/wxsf/ShapeCanvas.h"
#include "wx/wxsf/ShapeHandle.h"
#include "wx/wxsf/ShapeBase.h"

#include <wx/listimpl.cpp>

IMPLEMENT_DYNAMIC_CLASS(wxSFShapeHandle, wxObject);

WX_DEFINE_LIST(CHandleList);

wxSFShapeHandle::wxSFShapeHandle(void)
{
	// initialize data members
	m_nType = hndUNDEF;
	m_pParentShape = NULL;
	m_nPrevPos = wxPoint(0, 0);
	m_nCurrPos = wxPoint(0, 0);
	m_nId = -1;

	m_fVisible = false;
	m_fMouseOver = false;
}

wxSFShapeHandle::wxSFShapeHandle(wxSFShapeBase* parent, HANDLETYPE type, long id)
{
	// initialize data members
	m_nType = type;
	m_pParentShape = parent;
	m_nPrevPos = wxPoint(0, 0);
	m_nCurrPos = wxPoint(0, 0);
	m_nId = id;

	m_fVisible = false;
	m_fMouseOver = false;
}

wxSFShapeHandle::wxSFShapeHandle(wxSFShapeHandle& obj)
{
	// initialize data members
	m_nType = obj.m_nType;
	m_pParentShape = obj.m_pParentShape;
	m_nPrevPos = wxPoint(0, 0);
	m_nCurrPos = wxPoint(0, 0);
	m_nId = obj.m_nId;

	m_fVisible = obj.m_fVisible;
	m_fMouseOver = obj.m_fMouseOver;
}

wxSFShapeHandle::~wxSFShapeHandle(void)
{
}

//----------------------------------------------------------------------------------//
// Public functions
//----------------------------------------------------------------------------------//

bool wxSFShapeHandle::IsInside(const wxPoint& pos)
{
	return GetHandleRect().Contains(pos);
}

void wxSFShapeHandle::Draw(wxSFScaledPaintDC& dc)
{
	if(m_fVisible && m_pParentShape)
	{
		if(m_fMouseOver)DrawHover(dc);
		else
			DrawNormal(dc);
	}
}

void wxSFShapeHandle::Refresh()
{
	if(m_pParentShape)m_pParentShape->Refresh();
}

void wxSFShapeHandle::OnMouseMove(const wxPoint& pos)
{
	if(m_fVisible)
	{
		if(IsInside(pos))
		{
			if(!m_fMouseOver)
			{
				m_fMouseOver = true;
				Refresh();
			}
		}
		else
		{
			if(m_fMouseOver)
			{
				m_fMouseOver = false;
				Refresh();
			}
		}
	}
}

//----------------------------------------------------------------------------------//
// Public virtual functions
//----------------------------------------------------------------------------------//

void wxSFShapeHandle::OnBeginDrag(const wxPoint& pos)
{
    // HINT: override it if neccessary...

	m_nPrevPos = m_nStartPos = pos;

	if(m_pParentShape)m_pParentShape->OnBeginHandle(*this);
}

void wxSFShapeHandle::OnDragging(const wxPoint& pos)
{
    // HINT: override it if neccessary...

	if(m_fVisible && m_pParentShape && m_pParentShape->ContainsStyle(wxSFShapeBase::sfsSIZE_CHANGE))
	{
		if(pos != m_nPrevPos)
		{
		    wxRect prevRct = m_pParentShape->GetBoundingBox();

			m_nCurrPos = pos;

			switch(m_nType)
			{
			case hndLEFTTOP:
				if((pos.x < prevRct.GetRight()) && (pos.y < prevRct.GetBottom()))
					m_pParentShape->_OnHandle(*this);
				break;

			case hndTOP:
				if(pos.y < prevRct.GetBottom())
					m_pParentShape->_OnHandle(*this);
				break;

			case hndRIGHTTOP:
				if((pos.x > prevRct.GetLeft()) && (pos.y < prevRct.GetBottom()))
					m_pParentShape->_OnHandle(*this);
				break;

			case hndRIGHT:
				if(pos.x > prevRct.GetLeft())
					m_pParentShape->_OnHandle(*this);
				break;

			case hndRIGHTBOTTOM:
				if((pos.x > prevRct.GetLeft()) && (pos.y > prevRct.GetTop()))
					m_pParentShape->_OnHandle(*this);
				break;

			case hndBOTTOM:
				if(pos.y > prevRct.GetTop())
					m_pParentShape->_OnHandle(*this);
				break;

			case hndLEFTBOTTOM:
				if((pos.x < prevRct.GetRight()) && (pos.y > prevRct.GetTop()))
					m_pParentShape->_OnHandle(*this);
				break;

			case hndLEFT:
				if(pos.x < prevRct.GetRight())
					m_pParentShape->_OnHandle(*this);
				break;

            case hndLINESTART:
            case hndLINEEND:
			case hndLINECTRL:
				m_pParentShape->_OnHandle(*this);
				break;

            default:
                break;
			}
		}

		m_nPrevPos = pos;
	}
}

void wxSFShapeHandle::OnEndDrag(const wxPoint& pos)
{
	// HINT: override it if neccessary...

	if(m_pParentShape)m_pParentShape->OnEndHandle(*this);
}

//----------------------------------------------------------------------------------//
// Protected functions functions
//----------------------------------------------------------------------------------//

void wxSFShapeHandle::DrawNormal(wxSFScaledPaintDC& dc)
{
    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(*wxBLACK_BRUSH);
    dc.SetLogicalFunction(wxINVERT);
    #ifdef __WXGTK__
    dc.DrawRectangle(GetHandleRect().Inflate(1, 1));
    #else
    dc.DrawRectangle(GetHandleRect());
    #endif
    dc.SetLogicalFunction(wxCOPY);
    dc.SetPen(wxNullPen);
    dc.SetBrush(wxNullBrush);
}

void wxSFShapeHandle::DrawHover(wxSFScaledPaintDC& dc)
{
    if(m_pParentShape->ContainsStyle(wxSFShapeBase::sfsSIZE_CHANGE))
    {
        dc.SetBrush(wxBrush(m_pParentShape->GetHoverColour()));
        dc.DrawRectangle(GetHandleRect());
        dc.SetBrush(wxNullBrush);
    }
    else
    {
        DrawNormal(dc);
    }
}

wxRect wxSFShapeHandle::GetHandleRect() const
{
	if(m_pParentShape)
	{
		wxRect hrct;
		wxRect brct = m_pParentShape->GetBoundingBox();
		switch(m_nType)
		{
		case hndLEFTTOP:
			hrct = wxRect(brct.GetLeftTop(), wxSize(7,7));
			break;
		case hndTOP:
			hrct = wxRect(wxPoint(brct.GetLeft() + brct.GetWidth()/2, brct.GetTop()), wxSize(7,7));
			break;
		case hndRIGHTTOP:
			hrct = wxRect(brct.GetRightTop(), wxSize(7,7));
			break;
		case hndRIGHT:
			hrct = wxRect(wxPoint(brct.GetRight(), brct.GetTop() + brct.GetHeight()/2), wxSize(7,7));
			break;
		case hndRIGHTBOTTOM:
			hrct = wxRect(brct.GetRightBottom(), wxSize(7,7));
			break;
		case hndBOTTOM:
			hrct = wxRect(wxPoint(brct.GetLeft() + brct.GetWidth()/2, brct.GetBottom()), wxSize(7,7));
			break;
		case hndLEFTBOTTOM:
			hrct = wxRect(brct.GetLeftBottom(), wxSize(7,7));
			break;
		case hndLEFT:
			hrct = wxRect(wxPoint(brct.GetLeft(), brct.GetTop() + brct.GetHeight()/2), wxSize(7,7));
			break;
		case hndLINECTRL:
			{
				wxRealPoint* pt = ((wxSFLineShape*)m_pParentShape)->GetControlPoints().Item(m_nId)->GetData();
				hrct = wxRect(wxPoint((int)pt->x, (int)pt->y), wxSize(7,7));
			}
			break;
        case hndLINEEND:
        case hndLINESTART:
            {
                wxSFLineShape *pLine = (wxSFLineShape*)m_pParentShape;
                // Get all polyline segments
                CLineSegmentArray m_arrLineSegments;
                pLine->GetLineSegments(m_arrLineSegments);

                wxRealPoint pt;
                if(m_nType == hndLINESTART)
                {
                    pt = m_arrLineSegments.Item(0).m_nSrc;
                }
                else
                    pt = m_arrLineSegments.Item(m_arrLineSegments.Count()-1).m_nTrg;

                hrct = wxRect(wxPoint((int)pt.x, (int)pt.y), wxSize(7,7));
            }
            break;

		default:
			hrct = wxRect();
		}

		hrct.Offset(-3, -3);
		return hrct;
	}
	else
		return wxRect();
}
