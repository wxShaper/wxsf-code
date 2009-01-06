/***************************************************************
 * Name:      wxSFSample3Main.h
 * Purpose:   Defines Application Frame
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-08-31
 * Copyright: Michal Bližňák ()
 * License:
 **************************************************************/

#ifndef WXSFSample3MAIN_H
#define WXSFSample3MAIN_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "wxSFSample3App.h"
// add wxShapeFramework include file
#include "wx/wxsf/wxShapeFramework.h"

class wxSFSample3Frame: public wxFrame
{
    public:
        wxSFSample3Frame(wxFrame *frame, const wxString& title);
        ~wxSFSample3Frame();

    private:
        enum
        {
            idMenuQuit = 1000,
            idMenuOpen,
            idMenuSave,
            idMenuAbout
        };

        // create wxSF diagram manager
        wxSFDiagramManager m_Manager;
        // create pointer to wxSF shape canvas
        wxSFShapeCanvas* m_pCanvas;

        void OnClose(wxCloseEvent& event);
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnOpen(wxCommandEvent& event);
        void OnSave(wxCommandEvent& event);

        // declare common event handlers for wxShapeCanvas
        void OnLeftClickCanvas(wxMouseEvent& event);
        void OnRightClickCanvas(wxMouseEvent& event);
        // declare wxSF specific event handler for wxShapeCanvas
        void OnLineDone(wxSFShapeEvent& event);
		void OnTextChanged(wxSFShapeTextEvent& event);

        DECLARE_EVENT_TABLE()
};


#endif // WXSFSample3MAIN_H
