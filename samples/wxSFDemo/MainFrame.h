#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/clrpicker.h>

#include "GUI.h"
#include "FrameCanvas.h"

// wx backward compatibility hack...
#ifndef wxSAVE
#define wxSAVE wxFD_SAVE
#endif
#ifndef wxOPEN
#define wxOPEN wxFD_OPEN
#endif

// thumbnail frame
class ThumbFrm : public _ThumbFrm
{
public:
	ThumbFrm(wxWindow *parent);
	virtual ~ThumbFrm();
	
	// public member data accessors
	wxSFThumbnail* GetThumbnail() {	return m_pThumbnail; }
	
protected:
	// protected data members
	wxSFThumbnail *m_pThumbnail;
};

// main application frame
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
		modeORTHOLINE,
		modeROUNDORTHOLINE,
		modeGRID,
		modeFLEXGRID,
		modeSTANDALONELINE
	};

    MainFrm(wxWindow* parent );

	// public data accessors
	MODE GetToolMode(){return m_nToolMode;}
	void SetToolMode(MODE m){m_nToolMode = m;}
	
	wxSlider* GetZoomSlider(){return m_pZoomSlider;}

	// public data members
	bool m_fShowShadows;

protected:
    FrameCanvas* m_pShapeCanvas;
	ThumbFrm *m_pThumbFrm;

	wxColourPickerCtrl* cpicker;

	wxSFDiagramManager m_DiagramManager;
	wxSFAutoLayout m_AutoLayout;

	bool m_fShowGrid;
	MODE m_nToolMode;
	
	// protected function
	void CleanUp();

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
	void OnAutoLayout(wxCommandEvent& event);

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
	void OnUpdateAutoLayout(wxUpdateUIEvent& event);

	DECLARE_EVENT_TABLE();

};

#endif // MAINFRAME_H
