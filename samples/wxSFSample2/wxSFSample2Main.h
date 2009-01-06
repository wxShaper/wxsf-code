/***************************************************************
 * Name:      wxSFSample2Main.h
 * Purpose:   Defines Application Frame
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-08-31
 * Copyright: Michal Bližňák ()
 * License:
 **************************************************************/

#ifndef WXSFSample2MAIN_H
#define WXSFSample2MAIN_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "wxSFSample2App.h"
#include "SampleCanvas.h"

class wxSFSample2Frame: public wxFrame
{
    public:
        wxSFSample2Frame(wxFrame *frame, const wxString& title);
        ~wxSFSample2Frame();

    private:
        enum
        {
            idMenuQuit = 1000,
            idMenuAbout
        };

        // create wxSF diagram manager
        wxSFDiagramManager m_Manager;
        // create pointer to wxSF shape canvas
        SampleCanvas* m_pCanvas;

        void OnClose(wxCloseEvent& event);
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);

        DECLARE_EVENT_TABLE()
};


#endif // WXSFSample2MAIN_H
