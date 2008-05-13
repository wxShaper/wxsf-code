#include "wx_pch.h"

#include <wx/artprov.h>

#include "MainFrame.h"
#include "Art.h"
#include "Ids.h"

// wx backward compatibility hack...
#ifndef wxSAVE
#define wxSAVE wxFD_SAVE
#endif
#ifndef wxOPEN
#define wxOPEN wxFD_OPEN
#endif

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
	EVT_MENU(IDM_SAVEASBITMAP, CMainFrame::OnExportToBMP)
	EVT_MENU(wxID_PRINT, CMainFrame::OnPrint)
	EVT_MENU(wxID_PREVIEW, CMainFrame::OnPrintPreview)
	EVT_MENU(wxID_PRINT_SETUP, CMainFrame::OnPageSetup)
	EVT_COMMAND_SCROLL(wxID_ZOOM_FIT, CMainFrame::OnSlider)
	EVT_TOOL_RANGE(IDT_FIRST_TOOLMARKER, IDT_LAST_TOOLMARKER, CMainFrame::OnTool)
	EVT_COLOURPICKER_CHANGED(IDT_COLORPICKER, CMainFrame::OnHowerColor)
	EVT_UPDATE_UI(wxID_COPY, CMainFrame::OnUpdateCopy)
	EVT_UPDATE_UI(wxID_CUT, CMainFrame::OnUpdateCut)
	EVT_UPDATE_UI(wxID_PASTE, CMainFrame::OnUpdatePaste)
	EVT_UPDATE_UI(wxID_UNDO, CMainFrame::OnUpdateUndo)
	EVT_UPDATE_UI(wxID_REDO, CMainFrame::OnUpdateRedo)
	EVT_UPDATE_UI_RANGE(IDT_FIRST_TOOLMARKER, IDT_LAST_TOOLMARKER, CMainFrame::OnUpdateTool)
END_EVENT_TABLE()

CMainFrame::CMainFrame(wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long style):
    wxFrame(parent, id, title, pos, size, style)
{
    mainMenu = new wxMenuBar();

    SetMenuBar(mainMenu);

    wxMenu* fileMenu = new wxMenu();
	fileMenu->Append(wxID_NEW, wxT("&New\tCtrl+N"), wxT("New chart"), wxITEM_NORMAL);
	fileMenu->Append(wxID_OPEN, wxT("&Open\tCtrl+O"), wxT("Load a chart from XML file"), wxITEM_NORMAL);
	fileMenu->Append(wxID_SAVE, wxT("&Save as...\tCtrl+Shift+S"), wxT("Save the chart to XML file"), wxITEM_NORMAL);
	fileMenu->AppendSeparator();
	fileMenu->Append(IDM_SAVEASBITMAP, wxT("&Export to BMP..."), wxT("Export the chart to BMP file"), wxITEM_NORMAL);
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_PRINT, wxT("&Print...\tCtrl+P"), wxT("Open pring dialog"), wxITEM_NORMAL);
	fileMenu->Append(wxID_PREVIEW, wxT("Print pre&view...\tAlt+P"), wxT("Open print preview window"), wxITEM_NORMAL);
	fileMenu->Append(wxID_PRINT_SETUP, wxT("Pa&ge setup..."), wxT("Set print page properties"), wxITEM_NORMAL);
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

    // set shape canvas and associate it with diagram manager
    shapeCanvas = new CFrameCanvas(&m_DiagramManager, this, wxID_ANY);

    set_properties();
    do_layout();

	// set icon
	SetIcon(wxIcon(wx_xpm));

	// create zoom slider
	zoomSlider = new wxSlider(toolBar, wxID_ZOOM_FIT, 5, 2, 9, wxDefaultPosition, wxSize(200, -1));
	zoomSlider->SetToolTip(wxT("Set canvas scale"));

	// create colour picker
	#ifdef __WXMSW__
	cpicker = new wxColourPickerCtrl(toolBar, IDT_COLORPICKER, wxColor(120, 120, 255), wxDefaultPosition, wxSize(22, 22));
	#else
	cpicker = new wxColourPickerCtrl(toolBar, IDT_COLORPICKER, wxColor(120, 120, 255), wxDefaultPosition, wxSize(28, 28));
	#endif
	cpicker->SetToolTip(wxT("Set hower color"));

	// add toolbar tools
	toolBar->SetToolBitmapSize(wxSize(16, 15));
	toolBar->AddTool(wxID_NEW, wxT("New"), wxArtProvider::GetBitmap(wxART_NEW, wxART_MENU), wxT("New diagram"));
	toolBar->AddTool(wxID_OPEN, wxT("Load"), wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_MENU), wxT("Open file..."));
	toolBar->AddTool(wxID_SAVE, wxT("Save"), wxArtProvider::GetBitmap(wxART_FILE_SAVE, wxART_MENU), wxT("Save file..."));
	toolBar->AddSeparator();
	toolBar->AddTool(wxID_PRINT, wxT("Print"), wxArtProvider::GetBitmap(wxART_PRINT, wxART_MENU), wxT("Print..."));
	toolBar->AddTool(wxID_PREVIEW, wxT("Preview"), wxArtProvider::GetBitmap(wxART_FIND, wxART_MENU), wxT("Print preview..."));
	toolBar->AddSeparator();
	toolBar->AddTool(wxID_COPY, wxT("Copy"), wxArtProvider::GetBitmap(wxART_COPY, wxART_MENU), wxT("Copy to clipboard"));
	toolBar->AddTool(wxID_CUT, wxT("Cut"), wxArtProvider::GetBitmap(wxART_CUT, wxART_MENU), wxT("Cut to clipboard"));
	toolBar->AddTool(wxID_PASTE, wxT("Paste"), wxArtProvider::GetBitmap(wxART_PASTE, wxART_MENU), wxT("Paste from clipboard"));
	toolBar->AddSeparator();
	toolBar->AddTool(wxID_UNDO, wxT("Undo"), wxArtProvider::GetBitmap(wxART_UNDO, wxART_MENU), wxT("Undo"));
	toolBar->AddTool(wxID_REDO, wxT("Redo"), wxArtProvider::GetBitmap(wxART_REDO, wxART_MENU), wxT("Redo"));
	toolBar->AddSeparator();
	toolBar->AddCheckTool(IDT_GRID, wxT("Grid"), wxBitmap(Grid_xpm), wxNullBitmap, wxT("Show/hide grid"));
	toolBar->AddCheckTool(IDT_SHADOW, wxT("Shadows"), wxBitmap(Shadow_xpm), wxNullBitmap, wxT("Show/hide shadows"));
	toolBar->AddSeparator();
	toolBar->AddRadioTool(IDT_TOOL, wxT("Tool"), wxBitmap(Tool_xpm), wxNullBitmap, wxT("Design tool"));
	toolBar->AddRadioTool(IDT_RECTSHP, wxT("Rectangle"), wxBitmap(Rect_xpm), wxNullBitmap, wxT("Rectangle"));
	toolBar->AddRadioTool(IDT_SQUARESHP, wxT("Fixed rectangle"), wxBitmap(FixedRect_xpm), wxNullBitmap, wxT("Fixed rectangle"));
	toolBar->AddRadioTool(IDT_RNDRECTSHP, wxT("RoundRect"), wxBitmap(RoundRect_xpm), wxNullBitmap, wxT("Rounded rectangle"));
	toolBar->AddRadioTool(IDT_ELLIPSESHP, wxT("Ellipse"), wxBitmap(Ellipse_xpm), wxNullBitmap, wxT("Ellipse"));
	toolBar->AddRadioTool(IDT_CIRCLESHP, wxT("Circle"), wxBitmap(Circle_xpm), wxNullBitmap, wxT("Circle"));
	toolBar->AddRadioTool(IDT_DIAMONDSHP, wxT("Diamond"), wxBitmap(Diamond_xpm), wxNullBitmap, wxT("Diamond"));
	toolBar->AddRadioTool(IDT_TEXTSHP, wxT("Text"), wxBitmap(Text_xpm), wxNullBitmap, wxT("Text"));
	toolBar->AddRadioTool(IDT_EDITTEXTSHP, wxT("Editable text"), wxBitmap(EditText_xpm), wxNullBitmap, wxT("Editable text"));
	toolBar->AddRadioTool(IDT_BITMAPSHP, wxT("Bitmap"), wxBitmap(Bitmap_xpm), wxNullBitmap, wxT("Bitmap"));
	toolBar->AddSeparator();
	toolBar->AddRadioTool(IDT_LINESHP, wxT("Line"), wxBitmap(Line_xpm), wxNullBitmap, wxT("Polyline connection"));
	toolBar->AddRadioTool(IDT_CURVESHP, wxT("Curve"), wxBitmap(Curve_xpm), wxNullBitmap, wxT("Curve connection"));
	toolBar->AddSeparator();
	toolBar->AddTool(IDT_ALIGN_LEFT, wxT("Align left"), wxBitmap(AlignLeft_xpm), wxT("Align selected shapes to the left"));
	toolBar->AddTool(IDT_ALIGN_RIGHT, wxT("Align right"), wxBitmap(AlignRight_xpm), wxT("Align selected shapes to the right"));
	toolBar->AddTool(IDT_ALIGN_TOP, wxT("Align top"), wxBitmap(AlignTop_xpm), wxT("Align selected shapes to the top"));
	toolBar->AddTool(IDT_ALIGN_BOTTOM, wxT("Align bottom"), wxBitmap(AlignBottom_xpm), wxT("Align selected shapes to the bottom"));
	toolBar->AddTool(IDT_ALIGN_MIDDLE, wxT("Align middle"), wxBitmap(AlignMiddle_xpm), wxT("Align selected shapes to the middle"));
	toolBar->AddTool(IDT_ALIGN_CENTER, wxT("Align center"), wxBitmap(AlignCenter_xpm), wxT("Align selected shapes to the center"));
	toolBar->AddControl(zoomSlider);
	toolBar->AddSeparator();
	toolBar->AddControl(cpicker);
	toolBar->Realize();

	// initialize data members
	m_nToolMode = modeDESIGN;
	m_fShowGrid = true;
	m_fShowShadows = false;
}

//----------------------------------------------------------------------------------//
// private functions
//----------------------------------------------------------------------------------//

void CMainFrame::set_properties()
{
    // begin wxGlade: CMainFrame::set_properties
    SetTitle(wxT("wxShapeFramework Demo Application"));
    SetSize(wxSize(950, 700));
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

//----------------------------------------------------------------------------------//
// menu events
//----------------------------------------------------------------------------------//

void CMainFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
	Destroy();
}

void CMainFrame::OnNew(wxCommandEvent& WXUNUSED(event))
{
	if(wxMessageBox(wxT("Current chart will be lost. Do you want to proceed?"), wxT("ShapeFramework"), wxYES_NO | wxICON_QUESTION) == wxYES)
	{
		m_DiagramManager.Clear();
		shapeCanvas->ClearCanvasHistory();

        // set accepted shapes
        m_DiagramManager.ClearAcceptedShapes();
        m_DiagramManager.AcceptShape(wxT("All"));

        shapeCanvas->SaveCanvasState();
		shapeCanvas->Refresh();
	}
}

void CMainFrame::OnLoad(wxCommandEvent& WXUNUSED(event))
{
	wxFileDialog dlg(this, wxT("Load canvas from XML..."), wxGetCwd(), wxT(""), wxT("XML Files (*.xml) | *.xml"), wxOPEN);

	if(dlg.ShowModal() == wxID_OK)
	{
		shapeCanvas->LoadCanvas(dlg.GetPath());
		zoomSlider->SetValue(int(shapeCanvas->GetScale()*5));
		cpicker->SetColour(shapeCanvas->GetHoverColour());
		// m_fShowGrid = shapeCanvas->IsGridShown(); !!! DEPRECATED !!!
	}
}

void CMainFrame::OnSave(wxCommandEvent& WXUNUSED(event))
{
	wxFileDialog dlg(this, wxT("Save canvas to XML..."), wxGetCwd(), wxT(""), wxT("XML Files (*.xml) | *.xml"), wxSAVE);

	if(dlg.ShowModal() == wxID_OK)
	{
		shapeCanvas->SaveCanvas(dlg.GetPath());

		wxMessageBox(wxString::Format(wxT("The chart has been saved to '%s'."), dlg.GetPath().GetData()), wxT("ShapeFramework"));
	}
}

void CMainFrame::OnUndo(wxCommandEvent& WXUNUSED(event))
{
	shapeCanvas->Undo();
}

void CMainFrame::OnUpdateUndo(wxUpdateUIEvent& event)
{
	event.Enable(shapeCanvas->CanUndo());
}

void CMainFrame::OnRedo(wxCommandEvent& WXUNUSED(event))
{
	shapeCanvas->Redo();
}

void CMainFrame::OnUpdateRedo(wxUpdateUIEvent& event)
{
	event.Enable(shapeCanvas->CanRedo());
}

void CMainFrame::OnCopy(wxCommandEvent& WXUNUSED(event))
{
	shapeCanvas->Copy();
}

void CMainFrame::OnUpdateCopy(wxUpdateUIEvent& event)
{
	event.Enable(shapeCanvas->CanCopy());
}

void CMainFrame::OnCut(wxCommandEvent& WXUNUSED(event))
{
	shapeCanvas->Cut();
}

void CMainFrame::OnUpdateCut(wxUpdateUIEvent& event)
{
	event.Enable(shapeCanvas->CanCut());
}

void CMainFrame::OnPaste(wxCommandEvent& WXUNUSED(event))
{
	shapeCanvas->Paste();
}

void CMainFrame::OnUpdatePaste(wxUpdateUIEvent& event)
{
	event.Enable(shapeCanvas->CanPaste());
}

void CMainFrame::OnSelectAll(wxCommandEvent& WXUNUSED(event))
{
	shapeCanvas->SelectAll();
}

void CMainFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(wxString::Format(wxT("ShapeFramework Demonstration Application v1.5.4 \nwxShapeFramework version number: %s\nMichal Bliznak (c) 2007"), m_DiagramManager.GetVersion().c_str()), wxT("ShapeFranework"));
}

void CMainFrame::OnExportToBMP(wxCommandEvent& WXUNUSED(event))
{
	wxFileDialog dlg(this, wxT("Export canvas to BMP..."), wxGetCwd(), wxT(""), wxT("BMP Files (*.bmp) | *.bmp"), wxSAVE);

	if(dlg.ShowModal() == wxID_OK)
	{
        shapeCanvas->SaveCanvasToBMP(dlg.GetPath());
	}
}

//----------------------------------------------------------------------------------//
// tool events
//----------------------------------------------------------------------------------//

void CMainFrame::OnTool(wxCommandEvent& event)
{
    if(shapeCanvas->GetMode() == CFrameCanvas::modeCREATECONNECTION)shapeCanvas->AbortInteractiveConnection();

    switch(event.GetId())
    {
        case IDT_GRID:
        	m_fShowGrid = !m_fShowGrid;
			if( m_fShowGrid )
			{
				shapeCanvas->AddStyle(wxSFShapeCanvas::sfsGRID_SHOW);
				shapeCanvas->AddStyle(wxSFShapeCanvas::sfsGRID_USE);
			}
			else
			{
				shapeCanvas->RemoveStyle(wxSFShapeCanvas::sfsGRID_SHOW);
				shapeCanvas->RemoveStyle(wxSFShapeCanvas::sfsGRID_USE);
			}
            //shapeCanvas->UseGrid(m_fShowGrid); !!! DEPRECATED !!!
            //shapeCanvas->ShowGrid(m_fShowGrid); !!! DEPRECATED !!!
            shapeCanvas->Refresh(false);
            break;

        case IDT_SHADOW:
        	m_fShowShadows = !m_fShowShadows;

        	shapeCanvas->ShowShadows(m_fShowShadows, wxSFShapeCanvas::shadowALL);
        	// also shadows for topmost shapes only are allowed:
        	//shapeCanvas->ShowShadows(m_fShowShadows, wxSFShapeCanvas::shadowTOPMOST);

            shapeCanvas->Refresh(false);
            break;

        case IDT_BITMAPSHP:
        	m_nToolMode = modeBITMAP;
        	break;

        case IDT_CIRCLESHP:
            m_nToolMode = modeCIRCLE;
            break;

        case IDT_CURVESHP:
            m_nToolMode = modeCURVE;
            break;

        case IDT_DIAMONDSHP:
            m_nToolMode = modeDIAMOND;
            break;

        case IDT_EDITTEXTSHP:
            m_nToolMode = modeEDITTEXT;
            break;

        case IDT_ELLIPSESHP:
            m_nToolMode = modeELLIPSE;
            break;

        case IDT_LINESHP:
            m_nToolMode = modeLINE;
            break;

        case IDT_RECTSHP:
            m_nToolMode = modeRECT;
            break;

        case IDT_RNDRECTSHP:
            m_nToolMode = modeROUNDRECT;
            break;

        case IDT_SQUARESHP:
            m_nToolMode = modeFIXEDRECT;
            break;

        case IDT_TEXTSHP:
            m_nToolMode = modeTEXT;
            break;

        case IDT_TOOL:
            m_nToolMode = modeDESIGN;
            break;

        case IDT_ALIGN_LEFT:
            shapeCanvas->AlignSelected(wxSFShapeCanvas::halignLEFT, wxSFShapeCanvas::valignNONE);
            break;

        case IDT_ALIGN_RIGHT:
            shapeCanvas->AlignSelected(wxSFShapeCanvas::halignRIGHT, wxSFShapeCanvas::valignNONE);
            break;

        case IDT_ALIGN_CENTER:
            shapeCanvas->AlignSelected(wxSFShapeCanvas::halignCENTER, wxSFShapeCanvas::valignNONE);
            break;

        case IDT_ALIGN_TOP:
            shapeCanvas->AlignSelected(wxSFShapeCanvas::halignNONE, wxSFShapeCanvas::valignTOP);
            break;

        case IDT_ALIGN_BOTTOM:
            shapeCanvas->AlignSelected(wxSFShapeCanvas::halignNONE, wxSFShapeCanvas::valignBOTTOM);
            break;

        case IDT_ALIGN_MIDDLE:
            shapeCanvas->AlignSelected(wxSFShapeCanvas::halignNONE, wxSFShapeCanvas::valignMIDDLE);
            break;

        default:
            event.Skip();
            break;
    }
}

void CMainFrame::OnUpdateTool(wxUpdateUIEvent& event)
{
    switch(event.GetId())
    {
        case IDT_GRID:
        	event.Check(m_fShowGrid);
            break;

        case IDT_BITMAPSHP:
        	event.Check(m_nToolMode == modeBITMAP);
        	break;

        case IDT_CIRCLESHP:
            event.Check(m_nToolMode == modeCIRCLE);
            break;

        case IDT_CURVESHP:
            event.Check(m_nToolMode == modeCURVE);
            break;

        case IDT_DIAMONDSHP:
            event.Check(m_nToolMode == modeDIAMOND);
            break;

        case IDT_EDITTEXTSHP:
            event.Check(m_nToolMode == modeEDITTEXT);
            break;

        case IDT_ELLIPSESHP:
            event.Check(m_nToolMode == modeELLIPSE);
            break;

        case IDT_LINESHP:
            event.Check(m_nToolMode == modeLINE);
            break;

        case IDT_RECTSHP:
            event.Check(m_nToolMode == modeRECT);
            break;

        case IDT_RNDRECTSHP:
            event.Check(m_nToolMode == modeROUNDRECT);
            break;

        case IDT_SQUARESHP:
            event.Check(m_nToolMode == modeFIXEDRECT);
            break;

        case IDT_TEXTSHP:
            event.Check(m_nToolMode == modeTEXT);
            break;

        case IDT_TOOL:
            event.Check(m_nToolMode == modeDESIGN);
            break;

        case IDT_ALIGN_RIGHT:
        case IDT_ALIGN_LEFT:
        case IDT_ALIGN_TOP:
        case IDT_ALIGN_BOTTOM:
        case IDT_ALIGN_MIDDLE:
        case IDT_ALIGN_CENTER:
            event.Enable(shapeCanvas->CanAlignSelected());
            break;

        default:
            event.Skip();
            break;
    }
}


//----------------------------------------------------------------------------------//
// printing functions
//----------------------------------------------------------------------------------//

void CMainFrame::OnPrint(wxCommandEvent& WXUNUSED(event))
{
    shapeCanvas->Print();
}

void CMainFrame::OnPrintPreview(wxCommandEvent& WXUNUSED(event))
{
    shapeCanvas->PrintPreview();
}

void CMainFrame::OnPageSetup(wxCommandEvent& WXUNUSED(event))
{
    shapeCanvas->PageSetup();
}

//----------------------------------------------------------------------------------//
// other events
//----------------------------------------------------------------------------------//

void CMainFrame::OnSlider(wxScrollEvent& WXUNUSED(event))
{
	shapeCanvas->SetScale(double(zoomSlider->GetValue())/5);
	shapeCanvas->Refresh(false);
}

void CMainFrame::OnHowerColor(wxColourPickerEvent& event)
{
	shapeCanvas->SetHoverColour(event.GetColour());
}
