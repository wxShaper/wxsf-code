/***************************************************************
 * Name:      wxSFSample3Main.cpp
 * Purpose:   Code for Application Frame
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-08-31
 * Copyright: Michal Bližňák ()
 * License:
 **************************************************************/

#include "wx_pch.h"

#include "wxSFSample3Main.h"
#include "StarShape.h"

// wx backward compatibility hack...
#ifndef wxSAVE
#define wxSAVE wxFD_SAVE
#endif
#ifndef wxOPEN
#define wxOPEN wxFD_OPEN
#endif

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__WXMAC__)
        wxbuild << _T("-Mac");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

BEGIN_EVENT_TABLE(wxSFSample3Frame, wxFrame)
    EVT_CLOSE(wxSFSample3Frame::OnClose)
    EVT_MENU(idMenuQuit, wxSFSample3Frame::OnQuit)
    EVT_MENU(idMenuAbout, wxSFSample3Frame::OnAbout)
    EVT_MENU(idMenuOpen, wxSFSample3Frame::OnOpen)
    EVT_MENU(idMenuSave, wxSFSample3Frame::OnSave)
END_EVENT_TABLE()

wxSFSample3Frame::wxSFSample3Frame(wxFrame *frame, const wxString& title)
    : wxFrame(frame, -1, title)
{
    SetSize(800, 600);

#if wxUSE_MENUS
    // create a menu bar
    wxMenuBar* mbar = new wxMenuBar();
    wxMenu* fileMenu = new wxMenu(_T(""));
    fileMenu->Append(idMenuOpen, _("&Open\tCtrl-O"), _("Open diagram from XML file"));
    fileMenu->Append(idMenuSave, _("&Save\tCtrl-S"), _("Save diagram to XML file"));
    fileMenu->AppendSeparator();
    fileMenu->Append(idMenuQuit, _("&Quit\tAlt-F4"), _("Quit the application"));
    mbar->Append(fileMenu, _("&File"));

    wxMenu* helpMenu = new wxMenu(_T(""));
    helpMenu->Append(idMenuAbout, _("&About\tF1"), _("Show info about this application"));
    mbar->Append(helpMenu, _("&Help"));

    SetMenuBar(mbar);
#endif // wxUSE_MENUS

    // set some diagram manager properties if necessary...
    // set shape types accepted by the diagram manager
    m_Manager.AcceptShape(wxT("cStarShape"));
    m_Manager.AcceptShape(wxT("wxSFTextShape"));
    m_Manager.AcceptShape(wxT("wxSFLineShape"));

    // create shape canvas and associate it with the diagram manager
    m_pCanvas = new wxSFShapeCanvas(&m_Manager, this);
    // set some shape canvas' properties if necessary...
	m_pCanvas->AddStyle(wxSFShapeCanvas::sfsGRID_SHOW);
    m_pCanvas->AddStyle(wxSFShapeCanvas::sfsGRID_USE);

    // connect some event handlers...
    m_pCanvas->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(wxSFSample3Frame::OnLeftClickCanvas), NULL, this);
    m_pCanvas->Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(wxSFSample3Frame::OnRightClickCanvas), NULL, this);
    m_pCanvas->Connect(wxEVT_SF_LINE_DONE, wxSFShapeEventHandler(wxSFSample3Frame::OnLineDone), NULL, this);
	m_pCanvas->Connect(wxEVT_SF_TEXT_CHANGE, wxSFShapeTextEventHandler(wxSFSample3Frame::OnTextChanged), NULL, this);

#if wxUSE_STATUSBAR
    // create a status bar with some information about the used wxWidgets version
    CreateStatusBar(2);
    SetStatusText(_("Hello wxShapeFramework user!"),0);
    SetStatusText(wxbuildinfo(short_f), 1);
#endif // wxUSE_STATUSBAR

    Center();
}

wxSFSample3Frame::~wxSFSample3Frame()
{
}

void wxSFSample3Frame::OnLeftClickCanvas(wxMouseEvent& event)
{
    // HINT: perform your user actions here...

    // ... and then process standard canvas operations
    event.Skip();
}

void wxSFSample3Frame::OnRightClickCanvas(wxMouseEvent& event)
{
    // find out whether some shape has been clicked
    if( m_pCanvas->GetShapeAtPosition(m_pCanvas->DP2LP(event.GetPosition())) )
    {
        // start interactive connection creation
        m_pCanvas->StartInteractiveConnection(CLASSINFO(wxSFLineShape), event.GetPosition());
    }
    else
    {
        // create new composite shape
        m_Manager.AddShape(new cStarShape(), NULL, event.GetPosition(), sfINITIALIZE, sfDONT_SAVE_STATE);

        // ... and process standard canvas operations
        event.Skip();
    }
}

// Event handler called when the interactive line creation process is finished.
// Alternatively you can override virtual function wxSFShapeCanvas::OnConnectionFinished().
void wxSFSample3Frame::OnLineDone(wxSFShapeEvent& event)
{
    // get new line shape (if created)
    wxSFLineShape* pLine = (wxSFLineShape*)event.GetShape();

    if( pLine )
    {
        // assign target arrow to the line shape (also source arrow can be created)
        pLine->SetTrgArrow(CLASSINFO(wxSFSolidArrow));
    }
}

// Event handler called when a text inside the star was changed.
// Alternatively you can override virtual function wxSFShapeCanvas::OnTextChange().
void wxSFSample3Frame::OnTextChanged(wxSFShapeTextEvent& event)
{
    // get changed text shape
    wxSFEditTextShape* pText = (wxSFEditTextShape*)event.GetShape();
	
    if( pText )
    {
		// update the text shape and its parent(s)
		pText->Update();
		// diplay some info...
		wxLogMessage(wxString::Format(wxT("New text of the star with ID %d is : '%s'"), pText->GetParentShape()->GetId(), event.GetText().c_str()));
    }
}

void wxSFSample3Frame::OnOpen(wxCommandEvent& WXUNUSED(event))
{
	wxFileDialog dlg(this, wxT("Load diagram from XML..."), wxGetCwd(), wxT(""), wxT("XML Files (*.xml)|*.xml"), wxOPEN);

	if(dlg.ShowModal() == wxID_OK)
	{
	    // clear previous diagram
	    m_Manager.Clear();
	    // load diagram from XML file
		m_Manager.DeserializeFromXml(dlg.GetPath());
		
		m_pCanvas->Refresh(false);
	}
}

void wxSFSample3Frame::OnSave(wxCommandEvent& WXUNUSED(event))
{
	wxFileDialog dlg(this, wxT("Save diagram to XML..."), wxGetCwd(), wxT(""), wxT("XML Files (*.xml)|*.xml"), wxSAVE);

	if(dlg.ShowModal() == wxID_OK)
	{
	    // save diagram to XML file
		m_Manager.SerializeToXml(dlg.GetPath());

		wxMessageBox(wxString::Format(wxT("The diagram has been saved to '%s'."), dlg.GetPath().GetData()), wxT("ShapeFramework"));
	}
}

void wxSFSample3Frame::OnClose(wxCloseEvent& WXUNUSED(event))
{
    Destroy();
}

void wxSFSample3Frame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Destroy();
}

void wxSFSample3Frame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxString msg = wxbuildinfo(long_f);

    msg += wxT("\nWelcome to wxSFShapeFramework Sample 3 (c) Michal Bliznak, 2007\n\n");
    msg += wxT("Sample demostrates basic wxSF functionality:\n");
    msg += wxT(" - wxSF events are redirected via 'Connect' function.\n");
    msg += wxT(" - New custom 'composite' shape is created.\n");
    msg += wxT(" - Shapes can be joined together by lines.\n\n");
    msg += wxT("Usage:\n");
    msg += wxT(" - Left mouse click operates with inserted shapes\n");
    msg += wxT(" - Right mouse click inserts a custom shape to the canvas or starts\n");
    msg += wxT("   interactive connection line's creation process\n");
	msg += wxT(" - You can modify the star's text (double click it by the left mouse button)\n");
    msg += wxT(" - DEL key removes selected shape(s)\n");

    wxMessageBox(msg, wxT("wxShapeFramework Sample 3"));
}
