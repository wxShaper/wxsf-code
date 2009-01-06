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

#ifdef _DEBUG_MSVC
#define new DEBUG_NEW
#endif

#include "wx/wxsf/SFEvents.h"

DEFINE_EVENT_TYPE( wxEVT_SF_LINE_DONE );
DEFINE_EVENT_TYPE( wxEVT_SF_TEXT_CHANGE );
DEFINE_EVENT_TYPE( wxEVT_SF_ON_DROP );
DEFINE_EVENT_TYPE( wxEVT_SF_ON_PASTE );

//----------------------------------------------------------------------------------//
// wxSFShapeEvent class
//----------------------------------------------------------------------------------//

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

//----------------------------------------------------------------------------------//
// wxSFShapeEvent class
//----------------------------------------------------------------------------------//

wxSFShapeTextEvent::wxSFShapeTextEvent(wxEventType cmdType, int id)
: wxEvent(id, cmdType)
{
    m_pShape = NULL;
	m_sText = wxT("");
}

wxSFShapeTextEvent::wxSFShapeTextEvent(const wxSFShapeTextEvent& obj)
: wxEvent(obj)
{
    m_pShape = obj.m_pShape;
	m_sText = obj.m_sText;
}

wxSFShapeTextEvent::~wxSFShapeTextEvent()
{
}

//----------------------------------------------------------------------------------//
// wxSFShapeDropEvent class
//----------------------------------------------------------------------------------//

wxSFShapeDropEvent::wxSFShapeDropEvent(wxEventType cmdType, wxCoord x, wxCoord y, wxDragResult def, int id)
: wxEvent(id, cmdType)
{
	m_nDropPosition = wxPoint(x, y);
	m_nDragResult = def;
}

wxSFShapeDropEvent::wxSFShapeDropEvent(const wxSFShapeDropEvent& obj)
: wxEvent(obj)
{
	SetDroppedShapes(obj.m_lstDroppedShapes);
}

wxSFShapeDropEvent::~wxSFShapeDropEvent()
{
	m_lstDroppedShapes.Clear();
}

void wxSFShapeDropEvent::SetDroppedShapes(const ShapeList &list)
{
	ShapeList::compatibility_iterator node = list.GetFirst();
	while(node)
	{
		m_lstDroppedShapes.Append(node->GetData());
		node = node->GetNext();
	}
}

//----------------------------------------------------------------------------------//
// wxSFShapeDropEvent class
//----------------------------------------------------------------------------------//

wxSFShapePasteEvent::wxSFShapePasteEvent(wxEventType cmdType, int id)
: wxEvent(id, cmdType)
{
}

wxSFShapePasteEvent::wxSFShapePasteEvent(const wxSFShapePasteEvent& obj)
: wxEvent(obj)
{
	SetPastedShapes(obj.m_lstPastedShapes);
}

wxSFShapePasteEvent::~wxSFShapePasteEvent()
{
	m_lstPastedShapes.Clear();
}

void wxSFShapePasteEvent::SetPastedShapes(const ShapeList &list)
{
	ShapeList::compatibility_iterator node = list.GetFirst();
	while(node)
	{
		m_lstPastedShapes.Append(node->GetData());
		node = node->GetNext();
	}
}
