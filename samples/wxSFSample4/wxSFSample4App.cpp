/***************************************************************
 * Name:      wxSFSample1App.cpp
 * Purpose:   Code for Application Class
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-08-31
 * Copyright: Michal Bližňák ()
 * License:
 **************************************************************/

#include "wx_pch.h"

#include "wxSFSample4App.h"
#include "wxSFSample4Main.h"

#include "res/wx.xpm"

IMPLEMENT_APP(wxSFSample4App);

bool wxSFSample4App::OnInit()
{
    wxSFSample4Frame* frame = new wxSFSample4Frame(0L, _("wxShapeFramework Sample 4"));
    // To Set App Icon
    frame->SetIcon(wxIcon(wx_xpm));

    frame->Show();

    return true;
}
