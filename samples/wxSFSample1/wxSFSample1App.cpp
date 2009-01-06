/***************************************************************
 * Name:      wxSFSample1App.cpp
 * Purpose:   Code for Application Class
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-08-31
 * Copyright: Michal Bližňák ()
 * License:
 **************************************************************/

#include "wx_pch.h"

#include "wxSFSample1App.h"
#include "wxSFSample1Main.h"

#include "res/wx.xpm"

IMPLEMENT_APP(wxSFSample1App);

bool wxSFSample1App::OnInit()
{
    wxSFSample1Frame* frame = new wxSFSample1Frame(0L, _("wxShapeFramework Sample 1"));
    // To Set App Icon
    frame->SetIcon(wxIcon(wx_xpm)); 

    frame->Show();

    return true;
}
