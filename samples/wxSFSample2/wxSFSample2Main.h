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
		enum LOGTYPE
		{
			logMouseEvent = 1002,
			logHandleEvent,
			logKeyEvent,
			logChildDropEvent
		};
		
        wxSFSample2Frame(wxFrame *frame, const wxString& title);
        ~wxSFSample2Frame();
		
		static void Log(LOGTYPE logtype, const wxString& msg);

    private:
        enum
        {
            idMenuQuit = 1000,
            idMenuAbout,
			idMenuLogMouseEvent,
			idMenuLogHandleEvent,
			idMenuLogKeyEvent,
			idMenuLogChildDropEvent
        };
		
		// private controls
		wxTextCtrl* m_textLog;
		wxMenu* m_menuLog;

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
