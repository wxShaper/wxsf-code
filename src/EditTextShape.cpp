/***************************************************************
 * Name:      EditTextShape.cpp
 * Purpose:   Implements editable text shape class
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "EditTextShape.h"
#include "ShapeCanvas.h"

static int textCtrlId = -1;

IMPLEMENT_DYNAMIC_CLASS(wxSFEditTextShape, wxSFTextShape);

BEGIN_EVENT_TABLE(wxSFContentCtrl, wxTextCtrl)
	EVT_KILL_FOCUS(wxSFContentCtrl::OnKillFocus)
	EVT_KEY_DOWN(wxSFContentCtrl::OnKeyDown)
END_EVENT_TABLE()

//----------------------------------------------------------------------------------//
// wxSFContentCtrl control class
//----------------------------------------------------------------------------------//

wxSFContentCtrl::wxSFContentCtrl(wxWindow* parent, wxWindowID id, wxSFEditTextShape* parentShape, const wxString& content, wxPoint pos, wxSize size, int style)
: wxTextCtrl(parent, id, content, pos, size, wxTE_PROCESS_ENTER | wxNO_BORDER | style)
{
	m_pParent = parent;
	m_pParentShape = parentShape;
	m_sPrevContent = content;

	SetInsertionPointEnd();
	if(m_pParentShape)
	{
		wxSFTextShape* pTextShape = (wxSFTextShape*)m_pParentShape;

		// update the font size in accordance to the canvas scale
		wxFont font = pTextShape->GetFont();
		font.SetPointSize(int(font.GetPointSize() * m_pParentShape->GetParentCanvas()->GetScale()));

		SetFont(font);
		SetBackgroundColour(wxColour(200, 200, 200));
		SetFocus();
	}
}

void wxSFContentCtrl::OnKillFocus(wxFocusEvent& event)
{
	Quit();
}

void wxSFContentCtrl::OnKeyDown(wxKeyEvent& event)
{
	switch(event.GetKeyCode())
	{
	case WXK_ESCAPE:
	case WXK_TAB:
		Quit();
		break;
	default:
		event.Skip();
	}
}

void wxSFContentCtrl::Quit()
{
	if(m_pParentShape)
	{
		m_pParentShape->SetText(GetValue());
		m_pParentShape->m_pTextCtrl = NULL;
		m_pParentShape->EnableSizeChange(m_pParentShape->m_fHandlesState);
		m_pParentShape->Refresh();

		// save canvas state if the textctrl content has changed...
		if(m_sPrevContent != GetValue())m_pParentShape->GetParentCanvas()->SaveCanvasState();
	}

	Destroy();
}

//----------------------------------------------------------------------------------//
// wxSFEditTextShape shape class
//----------------------------------------------------------------------------------//

wxSFEditTextShape::wxSFEditTextShape(void)
: wxSFTextShape()
{
	m_pTextCtrl = NULL;
}

wxSFEditTextShape::wxSFEditTextShape(const wxRealPoint& pos, const wxString& txt, long parentId, wxSFDiagramManager* manager)
: wxSFTextShape(pos, txt, parentId, manager)
{
	m_pTextCtrl = NULL;
}

wxSFEditTextShape::wxSFEditTextShape(wxSFEditTextShape& obj)
: wxSFTextShape(obj)
{
}

wxSFEditTextShape::~wxSFEditTextShape(void)
{
	if(m_pTextCtrl)delete m_pTextCtrl;
}

//----------------------------------------------------------------------------------//
// public virtual functions
//----------------------------------------------------------------------------------//

void wxSFEditTextShape::OnLeftDoubleClick(const wxPoint &pos)
{
	// HINT: override it if neccessary...

	if(GetParentCanvas())
	{
	    wxRealPoint shpPos = GetAbsolutePosition();
		wxRect shpBB = GetBoundingBox();
		int dx, dy;

		int style = 0;
		double scale = GetParentCanvas()->GetScale();
		GetParentCanvas()->CalcUnscrolledPosition(0, 0, &dx, &dy);

		if(m_sText.Contains(wxT("\n")))style = wxTE_MULTILINE;

		m_pTextCtrl = new wxSFContentCtrl(GetParentCanvas(), textCtrlId, this, m_sText, wxPoint(int((shpPos.x * scale) - dx), int((shpPos.y * scale) - dy)), wxSize(int(shpBB.GetWidth() * scale), int(shpBB.GetHeight() * scale)), style);
		m_fHandlesState = CanChangeSize();
		EnableSizeChange(false);
	}
}
