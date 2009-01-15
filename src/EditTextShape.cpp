/***************************************************************
 * Name:      EditTextShape.cpp
 * Purpose:   Implements editable text shape class
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

#include <wx/textctrl.h>
#include "wx/wxsf/EditTextShape.h"
#include "wx/wxsf/ShapeCanvas.h"

static int textCtrlId = wxNewId();

XS_IMPLEMENT_CLONABLE_CLASS(wxSFEditTextShape, wxSFTextShape);

BEGIN_EVENT_TABLE(wxSFContentCtrl, wxTextCtrl)
	EVT_KILL_FOCUS(wxSFContentCtrl::OnKillFocus)
	EVT_KEY_DOWN(wxSFContentCtrl::OnKeyDown)
	EVT_TEXT_ENTER(textCtrlId, wxSFContentCtrl::OnEnterDown)
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

void wxSFContentCtrl::OnKillFocus(wxFocusEvent& WXUNUSED(event))
{
	//Quit();
}

void wxSFContentCtrl::OnKeyDown(wxKeyEvent& event)
{
	switch(event.GetKeyCode())
	{
	case WXK_ESCAPE:
		Quit( sfCANCEL_TEXT_CHANGES );
		break;
	case WXK_TAB:
		Quit( sfAPPLY_TEXT_CHANGES );
		break;
	default:
		event.Skip();
	}
}

void wxSFContentCtrl::OnEnterDown(wxCommandEvent& event)
{
	// enter new line if SHIFT key was pressed together with the ENTER key
	if( wxGetKeyState( WXK_SHIFT ) )
	{
		event.Skip();
	}
	else
		Quit( sfAPPLY_TEXT_CHANGES );
}

void wxSFContentCtrl::Quit(bool apply)
{
	if(m_pParentShape)
	{
		m_pParentShape->m_pTextCtrl = NULL;
		m_pParentShape->SetStyle(m_pParentShape->m_nCurrentState);

		// save canvas state if the textctrl content has changed...
		if( apply && ( m_sPrevContent != GetValue() ) )
		{
			m_pParentShape->SetText(GetValue());
			m_sPrevContent = GetValue();
			
		    // inform parent shape canvas about text change...
            m_pParentShape->GetParentCanvas()->OnTextChange(m_pParentShape);
		    m_pParentShape->GetParentCanvas()->SaveCanvasState();
		}

		m_pParentShape->Update();
		m_pParentShape->GetParentCanvas()->Refresh();
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
	m_fForceMultiline = sfdvEDITTEXTSHAPE_FORCEMULTILINE;

	XS_SERIALIZE_EX(m_fForceMultiline, wxT("multiline"), sfdvEDITTEXTSHAPE_FORCEMULTILINE);
}

wxSFEditTextShape::wxSFEditTextShape(const wxRealPoint& pos, const wxString& txt, wxSFDiagramManager* manager)
: wxSFTextShape(pos, txt, manager)
{
	m_pTextCtrl = NULL;
	m_fForceMultiline = sfdvEDITTEXTSHAPE_FORCEMULTILINE;

	XS_SERIALIZE_EX(m_fForceMultiline, wxT("multiline"), sfdvEDITTEXTSHAPE_FORCEMULTILINE);
}

wxSFEditTextShape::wxSFEditTextShape(const wxSFEditTextShape& obj)
: wxSFTextShape(obj)
{
	m_pTextCtrl = NULL;
	m_fForceMultiline = obj.m_fForceMultiline;

	XS_SERIALIZE_EX(m_fForceMultiline, wxT("multiline"), sfdvEDITTEXTSHAPE_FORCEMULTILINE);
}

wxSFEditTextShape::~wxSFEditTextShape(void)
{
	if(m_pTextCtrl)delete m_pTextCtrl;
}
//----------------------------------------------------------------------------------//
// public functions
//----------------------------------------------------------------------------------//

void wxSFEditTextShape::EditLabel()
{
	if(GetParentCanvas())
	{
	    wxRealPoint shpPos = GetAbsolutePosition();
		wxRect shpBB = GetBoundingBox();
		int dx, dy;

		int style = 0;
		double scale = GetParentCanvas()->GetScale();
		GetParentCanvas()->CalcUnscrolledPosition(0, 0, &dx, &dy);

		if( m_fForceMultiline || m_sText.Contains(wxT("\n")) )
		{
			style = wxTE_MULTILINE;
			// set minimal control size
		}

		if( (m_sText == wxEmptyString) || ((style == wxTE_MULTILINE) && (shpBB.GetWidth() < 50)) )shpBB.SetWidth(50);

		m_pTextCtrl = new wxSFContentCtrl(GetParentCanvas(), textCtrlId, this, m_sText, wxPoint(int((shpPos.x * scale) - dx), int((shpPos.y * scale) - dy)), wxSize(int(shpBB.GetWidth() * scale), int(shpBB.GetHeight() * scale)), style);

		m_nCurrentState = GetStyle();
		RemoveStyle(sfsSIZE_CHANGE);
	}
}

//----------------------------------------------------------------------------------//
// public virtual functions
//----------------------------------------------------------------------------------//

void wxSFEditTextShape::OnLeftDoubleClick(const wxPoint& WXUNUSED(pos))
{
	// HINT: override it if neccessary...

    EditLabel();
}

bool wxSFEditTextShape::OnKey(int key)
{
    // HINT: override it if neccessary...

    switch(key)
    {
        case WXK_F2:
            if(IsActive() && IsVisible())
            {
                EditLabel();
            }
            break;

        default:
            break;
    }

    return wxSFShapeBase::OnKey(key);
}
