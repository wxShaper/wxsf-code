/***************************************************************
 * Name:      ControlShape.cpp
 * Purpose:   Implements GUI control shape class
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2008-04-30
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "wx/wxsf/ControlShape.h"
#include "wx/wxsf/DiagramManager.h"
#include "wx/wxsf/ShapeCanvas.h"

XS_IMPLEMENT_CLONABLE_CLASS(wxSFControlShape, wxSFRectShape);

wxSFControlShape::wxSFControlShape() : wxSFRectShape()
{
    m_pControl = NULL;
    m_fProcessEvents = sfdvCONTROLSHAPE_PROCESSEVENTS;

    m_pEventSink = new EventSink(this);

    m_Fill = *wxTRANSPARENT_BRUSH;
    //m_Border = wxPen(*wxRED, 1, wxDOT);
    m_Border = *wxTRANSPARENT_PEN;

    XS_SERIALIZE_EX(m_fProcessEvents, wxT("process_events"), sfdvCONTROLSHAPE_PROCESSEVENTS);
}

wxSFControlShape::wxSFControlShape(wxWindow *ctrl, const wxRealPoint& pos, const wxRealPoint& size, wxSFDiagramManager* manager)
: wxSFRectShape(pos, size, manager)
{
    SetControl(ctrl);
    m_fProcessEvents = sfdvCONTROLSHAPE_PROCESSEVENTS;

    m_pEventSink = new EventSink(this);

    m_Fill = *wxTRANSPARENT_BRUSH;
    //m_Border = wxPen(*wxRED, 1, wxDOT);
    m_Border = *wxTRANSPARENT_PEN;

    XS_SERIALIZE_EX(m_fProcessEvents, wxT("process_events"), sfdvCONTROLSHAPE_PROCESSEVENTS);
}

wxSFControlShape::wxSFControlShape(const wxSFControlShape& other)
: wxSFRectShape(other)
{
    m_pControl = NULL;
    m_fProcessEvents = other.m_fProcessEvents;

    m_pEventSink = new EventSink(this);

    XS_SERIALIZE_EX(m_fProcessEvents, wxT("process_events"), sfdvCONTROLSHAPE_PROCESSEVENTS);
}

wxSFControlShape::~wxSFControlShape()
{
    if( m_pControl ) m_pControl->Destroy();

    if( m_pEventSink ) delete m_pEventSink;
}

//----------------------------------------------------------------------------------//
// public functions
//----------------------------------------------------------------------------------//

void wxSFControlShape::SetControl(wxWindow *ctrl, bool fit)
{
    if( m_pControl )
    {
        m_pControl->Reparent( m_pPrevParent );
    }

    m_pControl = ctrl;

    if( m_pControl )
    {
        m_pPrevParent = ctrl->GetParent();

        if( m_pParentManager )
        {
            wxSFShapeCanvas *pCanvas = ((wxSFDiagramManager*)m_pParentManager)->GetShapeCanvas();

            // reparent GUI control if necessary
            if( pCanvas && ( (wxWindow*)pCanvas != m_pPrevParent ) )
                m_pControl->Reparent( (wxWindow*)pCanvas );

            // redirect mouse events to the event sink for their delayed processing
            m_pControl->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(EventSink::_OnLeftDown), NULL, m_pEventSink);
            m_pControl->Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(EventSink::_OnRightDown), NULL, m_pEventSink);
            m_pControl->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(EventSink::_OnKeyDown), NULL, m_pEventSink);
        }

        if( fit )
        {
            wxSize nCtrlSize = m_pControl->GetSize();

            m_nRectSize.x = nCtrlSize.x + 2*sfdvCONTROLSHAPE_WIDGETOFFSET;
            m_nRectSize.y = nCtrlSize.y + 2*sfdvCONTROLSHAPE_WIDGETOFFSET;
        }

        UpdateControl();
    }
}

//----------------------------------------------------------------------------------//
// public virtual functions
//----------------------------------------------------------------------------------//

void wxSFControlShape::Scale(double x, double y, bool children)
{
    wxSFRectShape::Scale(x, y, children);
    UpdateControl();
}

void wxSFControlShape::MoveTo(double x, double y)
{
    wxSFRectShape::MoveTo(x, y);
    UpdateControl();
}

void wxSFControlShape::MoveBy(double x, double y)
{
    wxSFRectShape::MoveBy(x, y);
    UpdateControl();
}

void wxSFControlShape::OnBeginDrag(const wxPoint& WXUNUSED(pos) )
{
    if( m_pParentManager )
    {
        wxSFShapeCanvas *pCanvas = ((wxSFDiagramManager*)m_pParentManager)->GetShapeCanvas();

        if( pCanvas )
        {
            m_nPrevStyle = pCanvas->GetStyle();
            pCanvas->RemoveStyle(wxSFShapeCanvas::sfsDND);
        }
    }

    if( m_pControl )
    {
        m_pControl->Raise();
        m_pControl->SetFocus();
    }
}

void wxSFControlShape::OnDragging(const wxPoint& WXUNUSED(pos) )
{
    if( m_pControl )
    {
        wxRealPoint absPos = GetAbsolutePosition();

        // set the control's position according to the parent control shape
        m_pControl->Move((int)absPos.x + sfdvCONTROLSHAPE_WIDGETOFFSET, (int)absPos.y + sfdvCONTROLSHAPE_WIDGETOFFSET);
    }
}

void wxSFControlShape::OnEndDrag(const wxPoint& WXUNUSED(pos) )
{
    if( m_pParentManager )
    {
        wxSFShapeCanvas *pCanvas = ((wxSFDiagramManager*)m_pParentManager)->GetShapeCanvas();

        if( pCanvas ) pCanvas->SetStyle(m_nPrevStyle);
    }
}

void wxSFControlShape::OnBeginHandle(wxSFShapeHandle& handle)
{
    //m_Border = wxPen(*wxRED, 1, wxDOT);

    // call default handler
    wxSFRectShape::OnBeginHandle(handle);

    if( m_pControl )
    {
        m_pControl->Raise();
        m_pControl->SetFocus();
    }
}

void wxSFControlShape::OnHandle(wxSFShapeHandle& handle)
{
    // call default handler
    wxSFRectShape::OnHandle(handle);

    UpdateControl();
}

/*void wxSFControlShape::OnEndHandle(wxSFShapeHandle& handle)
{
    m_Border = *wxTRANSPARENT_PEN;

    // call default handler
    wxSFRectShape::OnEndHandle(handle);
}*/

/*void wxSFControlShape::OnMouseEnter(const wxPoint& pos)
{
    if( m_pControl ) m_pControl->Lower();
}*/

//----------------------------------------------------------------------------------//
// protected functions
//----------------------------------------------------------------------------------//

void wxSFControlShape::UpdateControl()
{
    if( m_pControl )
    {
        wxRect rctBB = GetBoundingBox().Deflate(sfdvCONTROLSHAPE_WIDGETOFFSET, sfdvCONTROLSHAPE_WIDGETOFFSET);

        // set the control's dimensions and position according to the parent control shape
        m_pControl->SetSize(rctBB.GetWidth(), rctBB.GetHeight());
        m_pControl->Move(rctBB.GetLeft(), rctBB.GetTop());
    }
}


//----------------------------------------------------------------------------------//
// private functions
//----------------------------------------------------------------------------------//


//----------------------------------------------------------------------------------//
// EventSink class
//----------------------------------------------------------------------------------//

EventSink::EventSink()
{
    m_pParentShape = NULL;
}

EventSink::EventSink(wxSFControlShape *parent)
{
    m_pParentShape = parent;
}

EventSink::~EventSink()
{
}

//----------------------------------------------------------------------------------//
// public functions
//----------------------------------------------------------------------------------//

void EventSink::_OnLeftDown(wxMouseEvent &event)
{
    SendEvent(event);
}

void EventSink::_OnRightDown(wxMouseEvent &event)
{
    SendEvent(event);
}

void EventSink::_OnKeyDown(wxKeyEvent &event)
{
    SendEvent(event);
}

//----------------------------------------------------------------------------------//
// protected functions
//----------------------------------------------------------------------------------//

void EventSink::SendEvent(wxEvent &event)
{
    if( m_pParentShape && m_pParentShape->GetParentManager() )
    {
        wxSFShapeCanvas *pCanvas = ((wxSFDiagramManager*)m_pParentShape->GetParentManager())->GetShapeCanvas();

        // send new copy of the event to the shape canvas
        if( pCanvas ) pCanvas->AddPendingEvent(event);

        // process the event also by an original handler if requested
        if( m_pParentShape->GetEventProcessing() ) event.Skip();
    }
}
