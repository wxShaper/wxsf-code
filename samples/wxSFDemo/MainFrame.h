#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/clrpicker.h>

#include "GUI.h"
#include "FrameCanvas.h"

class MainFrm: public _MainFrm {
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
		modeCURVE,
		modeGRID,
		modeFLEXGRID
	};

    MainFrm(wxWindow* parent );

	// public data accessors
	MODE GetToolMode(){return m_nToolMode;}
	void SetToolMode(MODE m){m_nToolMode = m;}

	// public data members
	bool m_fShowShadows;

protected:
    FrameCanvas* m_pShapeCanvas;

	wxColourPickerCtrl* cpicker;

	wxSFDiagramManager m_DiagramManager;

	bool m_fShowGrid;
	MODE m_nToolMode;

	// common events
	void OnClose(wxCloseEvent& event);

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
	void OnPrint(wxCommandEvent& event);
	void OnPrintPreview(wxCommandEvent& event);
	void OnPageSetup(wxCommandEvent& event);

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
