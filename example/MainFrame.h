#include <wx/clrpicker.h>

#include "FrameCanvas.h"

#ifndef MAINFRAME_H
#define MAINFRAME_H


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

	bool m_fShowGrid;
	MODE m_nToolMode;

	void OnSlider(wxScrollEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnShowGrid(wxCommandEvent& event);
	void OnDesignTool(wxCommandEvent& event);
	void OnRectTool(wxCommandEvent& event);
	void OnRoundRectTool(wxCommandEvent& event);
	void OnEllipseTool(wxCommandEvent& event);
	void OnCircleTool(wxCommandEvent& event);
	void OnFixedRectTool(wxCommandEvent& event);
	void OnDiamondTool(wxCommandEvent& event);
	void OnTextTool(wxCommandEvent& event);
	void OnEditTextTool(wxCommandEvent& event);
	void OnBitmapTool(wxCommandEvent& event);
	void OnLineTool(wxCommandEvent& event);
	void OnCurveTool(wxCommandEvent& event);
	void OnHowerColor(wxColourPickerEvent& event);
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

	void OnUpdateShowGrid(wxUpdateUIEvent& event);
	void OnUpdateDesignTool(wxUpdateUIEvent& event);
	void OnUpdateRectTool(wxUpdateUIEvent& event);
	void OnUpdateEllipseTool(wxUpdateUIEvent& event);
	void OnUpdateRoundRectTool(wxUpdateUIEvent& event);
	void OnUpdateFixedRectTool(wxUpdateUIEvent& event);
	void OnUpdateCircleTool(wxUpdateUIEvent& event);
	void OnUpdateDiamondTool(wxUpdateUIEvent& event);
	void OnUpdateTextTool(wxUpdateUIEvent& event);
	void OnUpdateEditTextTool(wxUpdateUIEvent& event);
	void OnUpdateBitmapTool(wxUpdateUIEvent& event);
	void OnUpdateLineTool(wxUpdateUIEvent& event);
	void OnUpdateCurveTool(wxUpdateUIEvent& event);
	void OnUpdateCopy(wxUpdateUIEvent& event);
	void OnUpdateCut(wxUpdateUIEvent& event);
	void OnUpdatePaste(wxUpdateUIEvent& event);
	void OnUpdateUndo(wxUpdateUIEvent& event);
	void OnUpdateRedo(wxUpdateUIEvent& event);

	DECLARE_EVENT_TABLE();

};


#endif // MAINFRAME_H
