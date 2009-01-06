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

#include "wxSFSample4App.h"
// add wxShapeFramework include file
#include "wx/wxsf/wxShapeFramework.h"

class wxSFSample4Frame: public wxFrame
{
    public:
        wxSFSample4Frame(wxFrame *frame, const wxString& title);
        ~wxSFSample4Frame();

    private:
        enum
        {
            idMenuQuit = 1000,
            idMenuAbout
        };

        int m_nControlType;

        // create wxSF diagram manager
        wxSFDiagramManager m_Manager;
        // create pointer to wxSF shape canvas
        wxSFShapeCanvas* m_pCanvas;

        wxWindow* CreateGUIControl();

        void OnClose(wxCloseEvent& event);
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);

        // declare event handlers for wxShapeCanvas
        void OnLeftClickCanvas(wxMouseEvent& event);
        void OnRightClickCanvas(wxMouseEvent& event);

        DECLARE_EVENT_TABLE()
};

class TestPanel : public wxPanel
{
    public:
        TestPanel(wxWindow *parent);

    protected:
        enum
        {
            idResize = wxID_HIGHEST + 1
        };

        wxButton *btnResize;
        bool fBig;

        void OnBtnResize(wxCommandEvent& event);
};

#endif // WXSFSAMPLE1MAIN_H
