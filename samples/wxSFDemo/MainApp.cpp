#include "wx_pch.h"

#include "MainApp.h"
#include "MainFrame.h"

// wxwindows app
IMPLEMENT_APP( MainApp )

// class info
IMPLEMENT_DYNAMIC_CLASS( MainApp, wxApp )

bool MainApp::OnInit()
{
    // main frame
    MainFrm *frame = new MainFrm( NULL );
    SetTopWindow( frame );
    frame->Show( true );

    return true;
}
