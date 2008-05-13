#ifndef _FRAMECANVAS_H
#define _FRAMECANVAS_H

#include "wx/wxsf/wxShapeFramework.h"

class CMainFrame;
class CFrameCanvas : public wxSFShapeCanvas
{
public:
    CFrameCanvas(){;}
	CFrameCanvas(wxSFDiagramManager* manager, wxWindow* parent, wxWindowID id);
	virtual ~CFrameCanvas(void);

	// public virtual functions
	virtual void OnLeftDown(wxMouseEvent& event);
	virtual void OnRightDown(wxMouseEvent& event);
	virtual void OnKeyDown(wxKeyEvent& event);
	virtual void OnConnectionFinished(wxSFLineShape* connection);

	// pubic member data accessors
	CMainFrame* GetParentFrame(){return m_pParentFrame;}

protected:

	// protected data members
	CMainFrame *m_pParentFrame;

};

#endif //_FRAMECANVAS_H
