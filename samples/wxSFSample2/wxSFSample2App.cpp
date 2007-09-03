/***************************************************************
 * Name:      wxSFSample2App.cpp
 * Purpose:   Code for Application Class
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-08-31
 * Copyright: Michal Bližňák ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "wxSFSample2App.h"
#include "wxSFSample2Main.h"

IMPLEMENT_APP(wxSFSample2App);

bool wxSFSample2App::OnInit()
{
    wxSFSample2Frame* frame = new wxSFSample2Frame(0L, _("wxShapeFramework Sample 2"));

    // To Set App Icon
    frame->SetIcon(wxICON(aaaa));
    frame->Show();

    return true;
}
