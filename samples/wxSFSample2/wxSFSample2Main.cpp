/***************************************************************
 * Name:      wxSFSample2Main.cpp
 * Purpose:   Code for Application Frame
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-08-31
 * Copyright: Michal Bližňák ()
 * License:
 **************************************************************/

#include "wx_pch.h"

#include "wxSFSample2Main.h"

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << wxT("-Windows");
#elif defined(__WXMAC__)
        wxbuild << wxT("-Mac");
#elif defined(__UNIX__)
        wxbuild << wxT("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << wxT("-Unicode build");
#else
        wxbuild << wxT("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

BEGIN_EVENT_TABLE(wxSFSample2Frame, wxFrame)
    EVT_CLOSE(wxSFSample2Frame::OnClose)
    EVT_MENU(idMenuQuit, wxSFSample2Frame::OnQuit)
    EVT_MENU(idMenuAbout, wxSFSample2Frame::OnAbout)
END_EVENT_TABLE()

wxSFSample2Frame::wxSFSample2Frame(wxFrame *frame, const wxString& title)
    : wxFrame(frame, -1, title)
{
    SetSize(800, 600);

#if wxUSE_MENUS
    // create a menu bar
    wxMenuBar* mbar = new wxMenuBar();
    wxMenu* fileMenu = new wxMenu(wxT(""));
    fileMenu->Append(idMenuQuit, wxT("&Quit\tAlt-F4"), wxT("Quit the application"));
    mbar->Append(fileMenu, wxT("&File"));
	
	m_menuLog = new wxMenu();
	m_menuLog->AppendCheckItem(idMenuLogMouseEvent, wxT("Log &mouse handlers"));
	m_menuLog->AppendCheckItem(idMenuLogHandleEvent, wxT("Log &handle handlers"));
	m_menuLog->AppendCheckItem(idMenuLogKeyEvent, wxT("Log &keyboard handler"));
	m_menuLog->AppendCheckItem(idMenuLogChildDropEvent, wxT("Log &child drop handler"));
	mbar->Append(m_menuLog, wxT("&Log"));

    wxMenu* helpMenu = new wxMenu(wxT(""));
    helpMenu->Append(idMenuAbout, wxT("&About\tF1"), wxT("Show info about this application"));
    mbar->Append(helpMenu, wxT("&Help"));

    SetMenuBar(mbar);
#endif // wxUSE_MENUS

	wxFlexGridSizer* mainSizer = new wxFlexGridSizer( 2, 0, 0, 0 );
	mainSizer->AddGrowableCol( 0 );
	mainSizer->AddGrowableRow( 0 );
	mainSizer->SetFlexibleDirection( wxBOTH );
	mainSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    // set some diagram manager properties if necessary...
    // set accepted shapes (accept only 'SampleShape')
    m_Manager.AcceptShape(wxT("SampleShape"));

    // create shape canvas and associate it with shape manager
    m_pCanvas = new SampleCanvas(&m_Manager, this);

	mainSizer->Add( m_pCanvas, 1, wxEXPAND, 0 );
	
	m_textLog = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,150 ), wxTE_MULTILINE );
	m_textLog->SetFont( wxFont( 8, 74, 90, 90, false, wxT("Sans") ) );
	m_textLog->SetMinSize( wxSize( -1,150 ) );
	
	mainSizer->Add( m_textLog, 0, wxEXPAND, 0 );
	
	SetSizer( mainSizer );
	Layout();

#if wxUSE_STATUSBAR
    // create a status bar with some information about the used wxWidgets version
    CreateStatusBar(2);
    SetStatusText(wxT("Hello wxShapeFramework user!"),0);
    SetStatusText(wxbuildinfo(short_f), 1);
#endif // wxUSE_STATUSBAR

	SetSizer( mainSizer );
	Layout();
    Center();
}


wxSFSample2Frame::~wxSFSample2Frame()
{
}

void wxSFSample2Frame::OnClose(wxCloseEvent& WXUNUSED(event))
{
    Destroy();
}

void wxSFSample2Frame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Destroy();
}

void wxSFSample2Frame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxString msg = wxbuildinfo(long_f);

    msg += wxT("\nWelcome to wxSFShapeFramework Sample 2 (c) Michal Bliznak, 2007\n\n");
    msg += wxT("Sample demostrates basic wxSF functionality.\n");
    msg += wxT("wxSF event handlers are overrided in user-defined canvas and shape class.\n\n");
    msg += wxT("Usage:\n");
    msg += wxT(" - Left mouse click operates with inserted shapes\n");
    msg += wxT(" - Right mouse click inserts a rectangular shape to the canvas\n");
    msg += wxT(" - DEL key removes selected shape\n");

    wxMessageBox(msg, wxT("wxShapeFramework Sample 2"));
}

void wxSFSample2Frame::Log(LOGTYPE logtype, const wxString& msg)
{
	wxSFSample2Frame *mainFrame = (wxSFSample2Frame*)wxGetApp().GetTopWindow();
	
	if( mainFrame->m_menuLog->IsChecked( (int)logtype ) )
	{
		mainFrame->m_textLog->AppendText( msg );
	}
}
