/***************************************************************
 * Name:      wxSFSample1App.cpp
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

#include "wxSFSample1App.h"
#include "wxSFSample1Main.h"

IMPLEMENT_APP(wxSFSample1App);

bool wxSFSample1App::OnInit()
{
    wxSFSample1Frame* frame = new wxSFSample1Frame(0L, _("wxShapeFramework Sample 1"));

    // To Set App
    #ifdef __WXMSW__
    frame->SetIcon(wxICON(aaaa));
    #endif
    frame->Show();

    return true;
}
