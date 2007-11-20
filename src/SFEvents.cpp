/***************************************************************
 * Name:      SFEvents.cpp
 * Purpose:   Implements shape events classes
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-09-11
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "wx_pch.h"

#include "wx/wxsf/SFEvents.h"

DEFINE_EVENT_TYPE( wxEVT_SF_LINE_DONE );
DEFINE_EVENT_TYPE( wxEVT_SF_TEXT_CHANGE );

wxSFShapeEvent::wxSFShapeEvent(wxEventType cmdType, int id)
: wxEvent(id, cmdType)
{
    m_pShape = NULL;
}

wxSFShapeEvent::wxSFShapeEvent(const wxSFShapeEvent& obj)
: wxEvent(obj)
{
    m_pShape = obj.m_pShape;
}

wxSFShapeEvent::~wxSFShapeEvent()
{
}

