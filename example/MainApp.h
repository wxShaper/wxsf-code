#ifndef __MAINAPP_H__
#define __MAINAPP_H__

#ifndef WX_PRECOMP
#include <wx/defs.h>
#include <wx/app.h>
#endif

class CMainApp : public wxApp
{
    // class info
    DECLARE_DYNAMIC_CLASS( CMainApp )

public:
    virtual bool OnInit();
};

// wxwindows app
DECLARE_APP( CMainApp )

#endif // __MAINAPP_H__
