///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __GUI__
#define __GUI__

#include <wx/string.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/toolbar.h>
#include <wx/statusbr.h>
#include <wx/panel.h>
#include <wx/slider.h>
#include <wx/sizer.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class _MainFrm
///////////////////////////////////////////////////////////////////////////////
class _MainFrm : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* m_pMenuBar;
		wxMenu* m_pFileMenu;
		wxMenu* m_pEditMenu;
		wxMenu* m_pHelpMenu;
		wxToolBar* m_pToolBar;
		wxStatusBar* m_pStatusBar;
		wxPanel* m_pCanvasPanel;
		wxSlider* m_pZoomSlider;
	
	public:
		_MainFrm( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("wxShapeFramework Demo Application"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1024,700 ), long style = wxCLOSE_BOX|wxDEFAULT_FRAME_STYLE|wxRESIZE_BORDER|wxTAB_TRAVERSAL );
		~_MainFrm();
	
};

#endif //__GUI__
