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
    wxMenu* fileMenu = new wxMenu(_T(""));
    fileMenu->Append(idMenuQuit, _("&Quit\tAlt-F4"), _("Quit the application"));
    mbar->Append(fileMenu, _("&File"));

    wxMenu* helpMenu = new wxMenu(_T(""));
    helpMenu->Append(idMenuAbout, _("&About\tF1"), _("Show info about this application"));
    mbar->Append(helpMenu, _("&Help"));

    SetMenuBar(mbar);
#endif // wxUSE_MENUS

    // set some diagram manager properties if necessary...
    // set accepted shapes (accept only wxSFRectShape)
    m_Manager.AcceptShape(wxT("wxSFRectShape"));

    // create shape canvas and associate it with shape manager
    m_pCanvas = new SampleCanvas(&m_Manager, this);

#if wxUSE_STATUSBAR
    // create a status bar with some information about the used wxWidgets version
    CreateStatusBar(2);
    SetStatusText(_("Hello Code::Blocks user!"),0);
    SetStatusText(wxbuildinfo(short_f), 1);
#endif // wxUSE_STATUSBAR

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
    msg += wxT("wxSF event handlers are overrided in user-defined canvas class.\n\n");
    msg += wxT("Usage:\n");
    msg += wxT(" - Left mouse click operates with inserted shapes\n");
    msg += wxT(" - Right mouse click inserts a rectangular shape to the canvas\n");
    msg += wxT(" - DEL key removes selected shape\n");

    wxMessageBox(msg, wxT("wxShapeFramework Sample 2"));
}
