#ifndef WX_PRECOMP
#endif

#include "MainApp.h"
#include "MainFrame.h"

// wxwindows app
IMPLEMENT_APP( CMainApp )

// class info
IMPLEMENT_DYNAMIC_CLASS( CMainApp, wxApp )

bool CMainApp::OnInit()
{
    // main frame
    CMainFrame *frame = new CMainFrame(NULL, -1, wxT(""));
    SetTopWindow( frame );
    frame->Show( true );

    return true;
}
