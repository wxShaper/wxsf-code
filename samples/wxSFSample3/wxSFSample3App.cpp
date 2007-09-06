/***************************************************************
 * Name:      wxSFSample3App.cpp
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

#include "wxSFSample3App.h"
#include "wxSFSample3Main.h"

IMPLEMENT_APP(wxSFSample3App);

bool wxSFSample3App::OnInit()
{
    wxSFSample3Frame* frame = new wxSFSample3Frame(0L, _("wxShapeFramework Sample 3"));

    // To Set App Icon
    #ifdef __WXMSW__
    frame->SetIcon(wxICON(aaaa));
    #endif
    frame->Show();

    return true;
}
