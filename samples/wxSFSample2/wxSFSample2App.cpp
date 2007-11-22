/***************************************************************
 * Name:      wxSFSample2App.cpp
 * Purpose:   Code for Application Class
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-08-31
 * Copyright: Michal Bližňák ()
 * License:
 **************************************************************/

#include "wx_pch.h"

#include "wxSFSample2App.h"
#include "wxSFSample2Main.h"

IMPLEMENT_APP(wxSFSample2App);

bool wxSFSample2App::OnInit()
{
    wxSFSample2Frame* frame = new wxSFSample2Frame(0L, _("wxShapeFramework Sample 2"));

    // To Set App Icon
    #ifdef __WXMSW__
    frame->SetIcon(wxICON(aaaa));
    #endif
    frame->Show();

    return true;
}
