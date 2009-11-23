/***************************************************************
 * Name:      wxSFSample2App.h
 * Purpose:   Defines Application Class
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-08-31
 * Copyright: Michal Bližňák ()
 * License:
 **************************************************************/

#ifndef WXSFSample2APP_H
#define WXSFSample2APP_H

#include <wx/app.h>

class wxSFSample2App : public wxApp
{
    public:
        virtual bool OnInit();
};

DECLARE_APP(wxSFSample2App);

#endif // WXSFSample2APP_H
