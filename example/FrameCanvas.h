#pragma once

#include "wxShapeFramework.h"

class CMainFrame;
class CFrameCanvas : public wxSFShapeCanvas
{
public:
	CFrameCanvas(wxWindow* parent, wxWindowID id);
	virtual ~CFrameCanvas(void);

	// public virtual functions
	virtual void OnLeftDown(wxMouseEvent& event);
	virtual void OnKeyDown(wxKeyEvent& event);
	virtual void OnConnectionFinished(wxSFLineShape* connection);

	// pubic member data accessors
	CMainFrame* GetParentFrame(){return m_pParentFrame;}

protected:

	// protected data members
	CMainFrame *m_pParentFrame;

};
