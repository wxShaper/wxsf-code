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
    m_Border = *wxTRANSPARENT_PEN;
    /*m_Fill = wxBrush(wxColour(200, 200, 200), wxSOLID);
    m_Border = wxPen(wxColour(200, 200, 200), 1, wxSOLID);*/

    XS_SERIALIZE_EX(m_fProcessEvents, wxT("forward_events"), sfdvCONTROLSHAPE_PROCESSEVENTS);
}

wxSFControlShape::wxSFControlShape(wxWindow *ctrl, const wxRealPoint& pos, const wxRealPoint& size, wxSFDiagramManager* manager)
: wxSFRectShape(pos, size, manager)
{
    SetControl(ctrl);
    m_fProcessEvents = sfdvCONTROLSHAPE_PROCESSEVENTS;

    m_pEventSink = new EventSink(this);

    m_Fill = *wxTRANSPARENT_BRUSH;
    m_Border = *wxTRANSPARENT_PEN;
    /*m_Fill = wxBrush(wxColour(200, 200, 200), wxSOLID);
    m_Border = wxPen(wxColour(200, 200, 200), 1, wxSOLID);*/

    XS_SERIALIZE_EX(m_fProcessEvents, wxT("forward_events"), sfdvCONTROLSHAPE_PROCESSEVENTS);
}

wxSFControlShape::wxSFControlShape(const wxSFControlShape& other)
: wxSFRectShape(other)
{
    m_pControl = NULL;
    m_fProcessEvents = other.m_fProcessEvents;

    m_pEventSink = new EventSink(this);

    XS_SERIALIZE_EX(m_fProcessEvents, wxT("forward_events"), sfdvCONTROLSHAPE_PROCESSEVENTS);
}

wxSFControlShape::~wxSFControlShape()
{
    if( m_pControl )
    {
        /*m_pControl->Disconnect(wxEVT_LEFT_DOWN, wxMouseEventHandler(wxSFShapeCanvas::_OnLeftDown), NULL, pCanvas);
        m_pControl->Disconnect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(wxSFShapeCanvas::_OnRightDown), NULL, pCanvas);
        m_pControl->Disconnect(wxEVT_KEY_DOWN, wxKeyEventHandler(wxSFShapeCanvas::_OnKeyDown), NULL, pCanvas);*/

        m_pControl->Destroy();
    }

    if( m_pEventSink ) delete m_pEventSink;
}

//----------------------------------------------------------------------------------//
// public functions
//----------------------------------------------------------------------------------//

void wxSFControlShape::SetControl(wxWindow *ctrl)
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
            //m_pControl->Connect(wxEVT_MOTION, wxMouseEventHandler(EventSink::_OnMouseMove), NULL, m_pEventSink);
            m_pControl->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(EventSink::_OnKeyDown), NULL, m_pEventSink);
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

void wxSFControlShape::OnDragging(const wxPoint& WXUNUSED(pos) )
{
    if( m_pControl )
    {
        wxRealPoint absPos = GetAbsolutePosition();

        // set the control's position according to the parent control shape
        m_pControl->Move((int)absPos.x + sfdvCONTROLSHAPE_WIDGETOFFSET, (int)absPos.y + sfdvCONTROLSHAPE_WIDGETOFFSET);
    }
}

void wxSFControlShape::OnHandle(wxSFShapeHandle& handle)
{
    // call default handler
    wxSFRectShape::OnHandle(handle);

    UpdateControl();
}

/*bool wxSFControlShape::OnKey(int key)
{
    if( key == WXK_DELETE)
    {
        return false;
    }
    else
        return true;
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
        m_pControl->Move(rctBB.GetLeft(), rctBB.GetTop());
        m_pControl->SetSize(rctBB.GetWidth(), rctBB.GetHeight());
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

void EventSink::_OnMouseMove(wxMouseEvent &event)
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
