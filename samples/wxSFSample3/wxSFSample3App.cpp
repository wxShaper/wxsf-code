/***************************************************************
 * Name:      wxSFSample3App.cpp
 * Purpose:   Code for Application Class
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-08-31
 * Copyright: Michal Bližňák ()
 * License:
 **************************************************************/

#include "wx_pch.h"

#include "wxSFSample3App.h"
#include "wxSFSample3Main.h"

#include "res/wx.xpm"    

IMPLEMENT_APP(wxSFSample3App);

bool wxSFSample3App::OnInit()
{
    wxSFSample3Frame* frame = new wxSFSample3Frame(0L, _("wxShapeFramework Sample 3"));
    // To Set App Icon
    frame->SetIcon(wxIcon(wx_xpm));  
    
    frame->Show();

    return true;
}
