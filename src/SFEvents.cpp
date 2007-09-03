#include "SFEvents.h"

DEFINE_EVENT_TYPE( wxEVT_SF_LINE_DONE );

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

