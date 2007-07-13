// use Visual Leak Detector (for MS VC++ compiler only)
#ifdef USEVLD
#include "vld.h"
#endif

#include <wx/artprov.h>

#include "MainFrame.h"
#include "res/Rect.xpm"
#include "res/Tool.xpm"
#include "res/RoundRect.xpm"
#include "res/Grid.xpm"
#include "res/Line.xpm"
#include "res/Curve.xpm"
#include "res/Ellipse.xpm"
#include "res/Circle.xpm"
#include "res/FixedRect.xpm"
#include "res/Diamond.xpm"
#include "res/Text.xpm"
#include "res/EditText.xpm"
#include "res/Bitmap.xpm"

int gridId = wxNewId();
int toolId = wxNewId();
int rectId = wxNewId();
int lineId = wxNewId();
int curveId = wxNewId();
int rndRectId = wxNewId();
int ellipseId = wxNewId();
int circleId = wxNewId();
int fixedRectId = wxNewId();
int diamondId = wxNewId();
int textId = wxNewId();
int editTextId = wxNewId();
int bitmapId = wxNewId();
int cpickerId = wxNewId();
int savebmpId = wxNewId();

BEGIN_EVENT_TABLE(CMainFrame, wxFrame)
	EVT_MENU(wxID_EXIT, CMainFrame::OnExit)
	EVT_MENU(wxID_NEW, CMainFrame::OnNew)
	EVT_MENU(wxID_OPEN, CMainFrame::OnLoad)
	EVT_MENU(wxID_SAVE, CMainFrame::OnSave)
	EVT_MENU(wxID_UNDO, CMainFrame::OnUndo)
	EVT_MENU(wxID_REDO, CMainFrame::OnRedo)
	EVT_MENU(wxID_COPY, CMainFrame::OnCopy)
	EVT_MENU(wxID_CUT, CMainFrame::OnCut)
	EVT_MENU(wxID_PASTE, CMainFrame::OnPaste)
	EVT_MENU(wxID_ABOUT, CMainFrame::OnAbout)
	EVT_MENU(wxID_SELECTALL, CMainFrame::OnSelectAll)
	EVT_MENU(savebmpId, CMainFrame::OnExportToBMP)
	EVT_COMMAND_SCROLL(wxID_ZOOM_FIT, CMainFrame::OnSlider)
	EVT_TOOL(gridId, CMainFrame::OnShowGrid)
	EVT_TOOL(toolId, CMainFrame::OnDesignTool)
	EVT_TOOL(rectId, CMainFrame::OnRectTool)
	EVT_TOOL(rndRectId, CMainFrame::OnRoundRectTool)
	EVT_TOOL(fixedRectId, CMainFrame::OnFixedRectTool)
	EVT_TOOL(circleId, CMainFrame::OnCircleTool)
	EVT_TOOL(diamondId, CMainFrame::OnDiamondTool)
	EVT_TOOL(textId, CMainFrame::OnTextTool)
	EVT_TOOL(editTextId, CMainFrame::OnEditTextTool)
	EVT_TOOL(bitmapId, CMainFrame::OnBitmapTool)
	EVT_TOOL(ellipseId, CMainFrame::OnEllipseTool)
	EVT_TOOL(lineId, CMainFrame::OnLineTool)
	EVT_TOOL(curveId, CMainFrame::OnCurveTool)
	EVT_COLOURPICKER_CHANGED(cpickerId, CMainFrame::OnHowerColor)
	EVT_UPDATE_UI(gridId, CMainFrame::OnUpdateShowGrid)
	EVT_UPDATE_UI(toolId, CMainFrame::OnUpdateDesignTool)
	EVT_UPDATE_UI(rectId, CMainFrame::OnUpdateRectTool)
	EVT_UPDATE_UI(ellipseId, CMainFrame::OnUpdateEllipseTool)
	EVT_UPDATE_UI(rndRectId, CMainFrame::OnUpdateRoundRectTool)
	EVT_UPDATE_UI(fixedRectId, CMainFrame::OnUpdateFixedRectTool)
	EVT_UPDATE_UI(circleId, CMainFrame::OnUpdateCircleTool)
	EVT_UPDATE_UI(diamondId, CMainFrame::OnUpdateDiamondTool)
	EVT_UPDATE_UI(textId, CMainFrame::OnUpdateTextTool)
	EVT_UPDATE_UI(editTextId, CMainFrame::OnUpdateEditTextTool)
	EVT_UPDATE_UI(bitmapId, CMainFrame::OnUpdateBitmapTool)
	EVT_UPDATE_UI(lineId, CMainFrame::OnUpdateLineTool)
	EVT_UPDATE_UI(curveId, CMainFrame::OnUpdateCurveTool)
	EVT_UPDATE_UI(wxID_COPY, CMainFrame::OnUpdateCopy)
	EVT_UPDATE_UI(wxID_CUT, CMainFrame::OnUpdateCut)
	EVT_UPDATE_UI(wxID_PASTE, CMainFrame::OnUpdatePaste)
	EVT_UPDATE_UI(wxID_UNDO, CMainFrame::OnUpdateUndo)
	EVT_UPDATE_UI(wxID_REDO, CMainFrame::OnUpdateRedo)
END_EVENT_TABLE()

CMainFrame::CMainFrame(wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long style):
    wxFrame(parent, id, title, pos, size, wxDEFAULT_FRAME_STYLE)
{
    // begin wxGlade: CMainFrame::CMainFrame
    mainMenu = new wxMenuBar();

    SetMenuBar(mainMenu);

    wxMenu* fileMenu = new wxMenu();
	fileMenu->Append(wxID_NEW, wxT("&New\tCtrl+N"), wxT("New chart"), wxITEM_NORMAL);
	fileMenu->Append(wxID_OPEN, wxT("&Open\tCtrl+O"), wxT("Load a chart from XML file"), wxITEM_NORMAL);
	fileMenu->Append(wxID_SAVE, wxT("&Save as...\tCtrl+Shift+S"), wxT("Save the chart to XML file"), wxITEM_NORMAL);
	fileMenu->AppendSeparator();
	fileMenu->Append(savebmpId, wxT("&Export to BMP..."), wxT("Export the chart to BMP file"), wxITEM_NORMAL);
	fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, wxT("E&xit\tAlt+X"), wxT("Close application"), wxITEM_NORMAL);
    mainMenu->Append(fileMenu, wxT("&File"));

	wxMenu* editMenu = new wxMenu();
	editMenu->Append(wxID_UNDO, wxT("&Undo\tCtrl+Z"), wxT("Discard previous action"), wxITEM_NORMAL);
	editMenu->Append(wxID_REDO, wxT("&Redo\tCtrl+Y"), wxT("Re-do previously discarded action"), wxITEM_NORMAL);
	editMenu->AppendSeparator();
	editMenu->Append(wxID_SELECTALL, wxT("Select &all\tCtrl+A"), wxT("Select all shapes"), wxITEM_NORMAL);
	editMenu->AppendSeparator();
	editMenu->Append(wxID_COPY, wxT("&Copy\tCtrl+C"), wxT("Copy shapes to the clipboard"), wxITEM_NORMAL);
	editMenu->Append(wxID_CUT, wxT("Cu&t\tCtrl+X"), wxT("Cut shapes to the clipboard"), wxITEM_NORMAL);
	editMenu->Append(wxID_PASTE, wxT("&Paste\tCtrl+V"), wxT("Paste shapes to the canvas"), wxITEM_NORMAL);
	mainMenu->Append(editMenu, wxT("&Edit"));

    wxMenu* helpMenu = new wxMenu();
    helpMenu->Append(wxID_ABOUT, wxT("&About..."), wxT("About application..."), wxITEM_NORMAL);
    mainMenu->Append(helpMenu, wxT("&Help"));

    statusBar = CreateStatusBar(1, wxST_SIZEGRIP);
    toolBar = new wxToolBar(this, -1, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL|wxTB_FLAT);
    SetToolBar(toolBar);

    shapeCanvas = new CFrameCanvas(this, -1);

    set_properties();
    do_layout();
    // end wxGlade

	// set icon
	SetIcon(wxICON(amain));

	// create zoom slider
	zoomSlider = new wxSlider(toolBar, wxID_ZOOM_FIT, 5, 2, 9, wxDefaultPosition, wxSize(200, -1));
	zoomSlider->SetToolTip(wxT("Set canvas scale"));

	// create colour picker
	cpicker = new wxColourPickerCtrl(toolBar, cpickerId, wxColor(120, 120, 255), wxDefaultPosition, wxSize(22, 22));
	cpicker->SetToolTip(wxT("Set hower color"));

	// add toolbar tools
	toolBar->AddTool(wxID_NEW, wxT("New"), wxArtProvider::GetBitmap(wxART_NEW, wxART_TOOLBAR), wxT("New"));
	toolBar->AddTool(wxID_OPEN, wxT("Load"), wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_TOOLBAR), wxT("Open file..."));
	toolBar->AddTool(wxID_SAVE, wxT("Save"), wxArtProvider::GetBitmap(wxART_FILE_SAVE, wxART_TOOLBAR), wxT("Save file..."));
	toolBar->AddSeparator();
	toolBar->AddTool(wxID_COPY, wxT("Copy"), wxArtProvider::GetBitmap(wxART_COPY, wxART_TOOLBAR), wxT("Copy to clipboard"));
	toolBar->AddTool(wxID_CUT, wxT("Cut"), wxArtProvider::GetBitmap(wxART_CUT, wxART_TOOLBAR), wxT("Cut to clipboard"));
	toolBar->AddTool(wxID_PASTE, wxT("Paste"), wxArtProvider::GetBitmap(wxART_PASTE, wxART_TOOLBAR), wxT("Paste from clipboard"));
	toolBar->AddSeparator();
	toolBar->AddTool(wxID_UNDO, wxT("Undo"), wxArtProvider::GetBitmap(wxART_UNDO, wxART_TOOLBAR), wxT("Undo"));
	toolBar->AddTool(wxID_REDO, wxT("Redo"), wxArtProvider::GetBitmap(wxART_REDO, wxART_TOOLBAR), wxT("Redo"));
	toolBar->AddSeparator();
	toolBar->AddCheckTool(gridId, wxT("Grid"), wxBitmap(Grid_xpm), wxNullBitmap, wxT("Show/hide grid"));
	toolBar->AddSeparator();
	toolBar->AddRadioTool(toolId, wxT("Tool"), wxBitmap(Tool_xpm), wxNullBitmap, wxT("Design tool"));
	toolBar->AddRadioTool(rectId, wxT("Rectangle"), wxBitmap(Rect_xpm), wxNullBitmap, wxT("Rectangle"));
	toolBar->AddRadioTool(fixedRectId, wxT("Ficed rectangle"), wxBitmap(FixedRect_xpm), wxNullBitmap, wxT("Fixed rectangle"));
	toolBar->AddRadioTool(rndRectId, wxT("RoundRect"), wxBitmap(RoundRect_xpm), wxNullBitmap, wxT("Rounded rectangle"));
	toolBar->AddRadioTool(ellipseId, wxT("Ellipse"), wxBitmap(Ellipse_xpm), wxNullBitmap, wxT("Ellipse"));
	toolBar->AddRadioTool(circleId, wxT("Circle"), wxBitmap(Circle_xpm), wxNullBitmap, wxT("Circle"));
	toolBar->AddRadioTool(diamondId, wxT("Diamond"), wxBitmap(Diamond_xpm), wxNullBitmap, wxT("Diamond"));
	toolBar->AddRadioTool(textId, wxT("Text"), wxBitmap(Text_xpm), wxNullBitmap, wxT("Text"));
	toolBar->AddRadioTool(editTextId, wxT("Editable text"), wxBitmap(EditText_xpm), wxNullBitmap, wxT("Editable text"));
	toolBar->AddRadioTool(bitmapId, wxT("Bitmap"), wxBitmap(Bitmap_xpm), wxNullBitmap, wxT("Bitmap"));
	toolBar->AddSeparator();
	toolBar->AddRadioTool(lineId, wxT("Line"), wxBitmap(Line_xpm), wxNullBitmap, wxT("Polyline connection"));
	toolBar->AddRadioTool(curveId, wxT("Curve"), wxBitmap(Curve_xpm), wxNullBitmap, wxT("Curve connection"));
	toolBar->AddSeparator();
	toolBar->AddControl(zoomSlider);
	toolBar->AddSeparator();
	toolBar->AddControl(cpicker);
	toolBar->Realize();

	// initialize data members
	m_nToolMode = modeDESIGN;
	m_fShowGrid = true;
}

//----------------------------------------------------------------------------------//
// private functions
//----------------------------------------------------------------------------------//

void CMainFrame::set_properties()
{
    // begin wxGlade: CMainFrame::set_properties
    SetTitle(wxT("ShapeFramework v1.0"));
    SetSize(wxSize(800, 600));
    int statusBar_widths[] = { -1 };
    statusBar->SetStatusWidths(1, statusBar_widths);
    const wxString statusBar_fields[] = {
        wxT("Ready...")
    };
    for(int i = 0; i < statusBar->GetFieldsCount(); ++i) {
        statusBar->SetStatusText(statusBar_fields[i], i);
    }
    // end wxGlade
}

void CMainFrame::do_layout()
{
    // begin wxGlade: CMainFrame::do_layout
    wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(shapeCanvas, 1, wxEXPAND, 0);
    SetSizer(mainSizer);
    Layout();
    Centre();
    // end wxGlade
}

//----------------------------------------------------------------------------------//
// protected event handlers
//----------------------------------------------------------------------------------//

void CMainFrame::OnExit(wxCommandEvent& event)
{
	Destroy();
}

void CMainFrame::OnSlider(wxScrollEvent& event)
{
	shapeCanvas->SetScale(double(zoomSlider->GetValue())/5);
	shapeCanvas->Refresh(false);
}

void CMainFrame::OnShowGrid(wxCommandEvent& event)
{
	m_fShowGrid = !m_fShowGrid;
	shapeCanvas->UseGrid(m_fShowGrid);
	shapeCanvas->ShowGrid(m_fShowGrid);
	shapeCanvas->Refresh(false);
}

void CMainFrame::OnUpdateShowGrid(wxUpdateUIEvent& event)
{
	event.Check(m_fShowGrid);
}

void CMainFrame::OnDesignTool(wxCommandEvent& event)
{
	if(shapeCanvas->GetMode() == CFrameCanvas::modeCREATECONNECTION)shapeCanvas->AbortInteractiveConnection();
	m_nToolMode = modeDESIGN;
}

void CMainFrame::OnUpdateDesignTool(wxUpdateUIEvent& event)
{
	event.Check(m_nToolMode == modeDESIGN);
}

void CMainFrame::OnRectTool(wxCommandEvent& event)
{
	if(shapeCanvas->GetMode() == CFrameCanvas::modeCREATECONNECTION)shapeCanvas->AbortInteractiveConnection();
	m_nToolMode = modeRECT;
}

void CMainFrame::OnUpdateRectTool(wxUpdateUIEvent& event)
{
	event.Check(m_nToolMode == modeRECT);
}

void CMainFrame::OnRoundRectTool(wxCommandEvent& event)
{
	if(shapeCanvas->GetMode() == CFrameCanvas::modeCREATECONNECTION)shapeCanvas->AbortInteractiveConnection();
	m_nToolMode = modeROUNDRECT;
}

void CMainFrame::OnUpdateRoundRectTool(wxUpdateUIEvent& event)
{
	event.Check(m_nToolMode == modeROUNDRECT);
}

void CMainFrame::OnFixedRectTool(wxCommandEvent& event)
{
	if(shapeCanvas->GetMode() == CFrameCanvas::modeCREATECONNECTION)shapeCanvas->AbortInteractiveConnection();
	m_nToolMode = modeFIXEDRECT;
}

void CMainFrame::OnUpdateFixedRectTool(wxUpdateUIEvent& event)
{
	event.Check(m_nToolMode == modeFIXEDRECT);
}

void CMainFrame::OnCircleTool(wxCommandEvent& event)
{
	if(shapeCanvas->GetMode() == CFrameCanvas::modeCREATECONNECTION)shapeCanvas->AbortInteractiveConnection();
	m_nToolMode = modeCIRCLE;
}

void CMainFrame::OnUpdateCircleTool(wxUpdateUIEvent& event)
{
	event.Check(m_nToolMode == modeCIRCLE);
}

void CMainFrame::OnEllipseTool(wxCommandEvent& event)
{
	if(shapeCanvas->GetMode() == CFrameCanvas::modeCREATECONNECTION)shapeCanvas->AbortInteractiveConnection();
	m_nToolMode = modeELLIPSE;
}

void CMainFrame::OnUpdateEllipseTool(wxUpdateUIEvent& event)
{
	event.Check(m_nToolMode == modeELLIPSE);
}

void CMainFrame::OnDiamondTool(wxCommandEvent& event)
{
	if(shapeCanvas->GetMode() == CFrameCanvas::modeCREATECONNECTION)shapeCanvas->AbortInteractiveConnection();
	m_nToolMode = modeDIAMOND;
}

void CMainFrame::OnUpdateDiamondTool(wxUpdateUIEvent& event)
{
	event.Check(m_nToolMode == modeDIAMOND);
}

void CMainFrame::OnTextTool(wxCommandEvent& event)
{
	if(shapeCanvas->GetMode() == CFrameCanvas::modeCREATECONNECTION)shapeCanvas->AbortInteractiveConnection();
	m_nToolMode = modeTEXT;
}

void CMainFrame::OnUpdateTextTool(wxUpdateUIEvent& event)
{
	event.Check(m_nToolMode == modeTEXT);
}

void CMainFrame::OnEditTextTool(wxCommandEvent& event)
{
	if(shapeCanvas->GetMode() == CFrameCanvas::modeCREATECONNECTION)shapeCanvas->AbortInteractiveConnection();
	m_nToolMode = modeEDITTEXT;
}

void CMainFrame::OnUpdateEditTextTool(wxUpdateUIEvent& event)
{
	event.Check(m_nToolMode == modeEDITTEXT);
}

void CMainFrame::OnBitmapTool(wxCommandEvent& event)
{
	if(shapeCanvas->GetMode() == CFrameCanvas::modeCREATECONNECTION)shapeCanvas->AbortInteractiveConnection();
	m_nToolMode = modeBITMAP;
}

void CMainFrame::OnUpdateBitmapTool(wxUpdateUIEvent& event)
{
	event.Check(m_nToolMode == modeBITMAP);
}

void CMainFrame::OnLineTool(wxCommandEvent& event)
{
	m_nToolMode = modeLINE;
}

void CMainFrame::OnUpdateLineTool(wxUpdateUIEvent& event)
{
	event.Check(m_nToolMode == modeLINE);
}

void CMainFrame::OnCurveTool(wxCommandEvent& event)
{
	m_nToolMode = modeCURVE;
}

void CMainFrame::OnUpdateCurveTool(wxUpdateUIEvent& event)
{
	event.Check(m_nToolMode == modeCURVE);
}

void CMainFrame::OnHowerColor(wxColourPickerEvent& event)
{
	shapeCanvas->SetHoverColour(event.GetColour());
}

void CMainFrame::OnNew(wxCommandEvent& event)
{
	if(wxMessageBox(wxT("Current chart will be lost. Do you want to proceed?"), wxT("ShapeFramework"), wxYES_NO | wxICON_QUESTION) == wxYES)
	{
		shapeCanvas->Clear();
		shapeCanvas->ClearCanvasHistory();
		shapeCanvas->Refresh();
	}
}

void CMainFrame::OnLoad(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("Load canvas from XML..."), wxGetCwd(), wxT(""), wxT("XML Files (*.xml) | *.xml"), wxOPEN);

	if(dlg.ShowModal() == wxID_OK)
	{
		shapeCanvas->LoadCanvas(dlg.GetPath());
		zoomSlider->SetValue(int(shapeCanvas->GetScale()*5));
		cpicker->SetColour(shapeCanvas->GetHoverColour());
		m_fShowGrid = shapeCanvas->IsGridShown();
	}
}

void CMainFrame::OnSave(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("Save canvas to XML..."), wxGetCwd(), wxT(""), wxT("XML Files (*.xml) | *.xml"), wxSAVE);

	if(dlg.ShowModal() == wxID_OK)
	{
		shapeCanvas->SaveCanvas(dlg.GetPath());

		wxMessageBox(wxString::Format(wxT("The chart has been saved to '%s'."), dlg.GetPath().GetData()), wxT("ShapeFramework"));
	}
}

void CMainFrame::OnUndo(wxCommandEvent& event)
{
	shapeCanvas->Undo();
}

void CMainFrame::OnUpdateUndo(wxUpdateUIEvent& event)
{
	event.Enable(shapeCanvas->CanUndo());
}

void CMainFrame::OnRedo(wxCommandEvent& event)
{
	shapeCanvas->Redo();
}

void CMainFrame::OnUpdateRedo(wxUpdateUIEvent& event)
{
	event.Enable(shapeCanvas->CanRedo());
}

void CMainFrame::OnCopy(wxCommandEvent& event)
{
	shapeCanvas->Copy();
}

void CMainFrame::OnUpdateCopy(wxUpdateUIEvent& event)
{
	event.Enable(shapeCanvas->CanCopy());
}

void CMainFrame::OnCut(wxCommandEvent& event)
{
	shapeCanvas->Cut();
}

void CMainFrame::OnUpdateCut(wxUpdateUIEvent& event)
{
	event.Enable(shapeCanvas->CanCut());
}

void CMainFrame::OnPaste(wxCommandEvent& event)
{
	shapeCanvas->Paste();
}

void CMainFrame::OnUpdatePaste(wxUpdateUIEvent& event)
{
	event.Enable(shapeCanvas->CanPaste());
}

void CMainFrame::OnSelectAll(wxCommandEvent& event)
{
	shapeCanvas->SelectAll();
}

void CMainFrame::OnAbout(wxCommandEvent& event)
{
	wxMessageBox(wxString::Format(wxT("ShapeFramework Demonstration Application v1.2\nwxShapeFramework version number: %s\nMichal Bliznak (c) 2007"), shapeCanvas->GetVersion().c_str()), wxT("ShapeFranework"));
}

void CMainFrame::OnExportToBMP(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("Export canvas to BMP..."), wxGetCwd(), wxT(""), wxT("BMP Files (*.bmp) | *.bmp"), wxSAVE);

	if(dlg.ShowModal() == wxID_OK)
	{
        shapeCanvas->SaveCanvasToBMP(dlg.GetPath());
	}
}















