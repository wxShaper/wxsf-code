#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/clrpicker.h>

#include "FrameCanvas.h"

class CMainFrame: public wxFrame {
public:

	enum MODE
	{
		modeDESIGN,
		modeRECT,
		modeFIXEDRECT,
		modeROUNDRECT,
		modeELLIPSE,
		modeCIRCLE,
		modeDIAMOND,
		modeTEXT,
		modeEDITTEXT,
		modeBITMAP,
		modeLINE,
		modeCURVE
	};

    CMainFrame(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_FRAME_STYLE);

	// public data accessors
	MODE GetToolMode(){return m_nToolMode;}
	void SetToolMode(MODE m){m_nToolMode = m;}

private:
    void set_properties();
    void do_layout();

protected:
    wxMenuBar* mainMenu;
    wxStatusBar* statusBar;
    wxToolBar* toolBar;
    CFrameCanvas* shapeCanvas;
	wxSlider* zoomSlider;
	wxColourPickerCtrl* cpicker;

	wxSFDiagramManager m_DiagramManager;

	bool m_fShowGrid;
	MODE m_nToolMode;

    // menu event handlers
	void OnExit(wxCommandEvent& event);
	void OnNew(wxCommandEvent& event);
	void OnSave(wxCommandEvent& event);
	void OnLoad(wxCommandEvent& event);
	void OnUndo(wxCommandEvent& event);
	void OnRedo(wxCommandEvent& event);
	void OnCopy(wxCommandEvent& event);
	void OnCut(wxCommandEvent& event);
	void OnPaste(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnSelectAll(wxCommandEvent& event);
	void OnExportToBMP(wxCommandEvent& event);

    // toolbar event handlers
	void OnSlider(wxScrollEvent& event);
	void OnTool(wxCommandEvent& event);
	void OnHowerColor(wxColourPickerEvent& event);

	void OnUpdateCopy(wxUpdateUIEvent& event);
	void OnUpdateCut(wxUpdateUIEvent& event);
	void OnUpdatePaste(wxUpdateUIEvent& event);
	void OnUpdateUndo(wxUpdateUIEvent& event);
	void OnUpdateRedo(wxUpdateUIEvent& event);
	void OnUpdateTool(wxUpdateUIEvent& event);

	DECLARE_EVENT_TABLE();

};

#endif // MAINFRAME_H
