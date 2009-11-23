/***************************************************************
 * Name:      wxSFSample1Main.h
 * Purpose:   Defines Application Frame
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-08-31
 * Copyright: Michal Bližňák ()
 * License:
 **************************************************************/

#ifndef WXSFSAMPLE1MAIN_H
#define WXSFSAMPLE1MAIN_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/hashmap.h>

#include "wxSFSample1App.h"
// add wxShapeFramework include file
#include "wx/wxsf/wxShapeFramework.h"

WX_DECLARE_HASH_MAP(wxEventType, wxString, wxIntegerHash, wxIntegerEqual, EventTypeMap);

class wxSFSample1Frame: public wxFrame
{
    public:
        wxSFSample1Frame(wxFrame *frame, const wxString& title);
        ~wxSFSample1Frame();

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
		
		// private data members
		EventTypeMap m_mapEventTypeInfo;

        // create wxSF diagram manager
        wxSFDiagramManager m_Manager;
        // create pointer to wxSF shape canvas
        wxSFShapeCanvas* m_pCanvas;

        void OnClose(wxCloseEvent& event);
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);

        // declare event handlers for shape canvas
        void OnLeftClickCanvas(wxMouseEvent& event);
        void OnRightClickCanvas(wxMouseEvent& event);
		// declare event handlers for shapes
		void OnShapeMouseEvent(wxSFShapeMouseEvent& event);
		void OnShapeHandleEvent(wxSFShapeHandleEvent& event);
		void OnShapeKeyEvent(wxSFShapeKeyEvent& event);
		void OnShapeChildDropEvent(wxSFShapeChildDropEvent& event);

        DECLARE_EVENT_TABLE()
};


#endif // WXSFSAMPLE1MAIN_H
