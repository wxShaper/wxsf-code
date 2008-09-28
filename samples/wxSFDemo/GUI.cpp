///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 21 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx_pch.h"

#include "GUI.h"

///////////////////////////////////////////////////////////////////////////

_MainFrm::_MainFrm( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 1024,700 ), wxDefaultSize );
	
	m_pMenuBar = new wxMenuBar( 0 );
	m_pFileMenu = new wxMenu();
	m_pMenuBar->Append( m_pFileMenu, wxT("&File") );
	
	m_pEditMenu = new wxMenu();
	m_pMenuBar->Append( m_pEditMenu, wxT("&Edit") );
	
	m_pHelpMenu = new wxMenu();
	m_pMenuBar->Append( m_pHelpMenu, wxT("&Help") );
	
	this->SetMenuBar( m_pMenuBar );
	
	m_pToolBar = this->CreateToolBar( wxTB_HORIZONTAL, wxID_ANY ); 
	m_pToolBar->Realize();
	
	m_pStatusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	wxFlexGridSizer* m_pMainSizer;
	m_pMainSizer = new wxFlexGridSizer( 2, 1, 0, 0 );
	m_pMainSizer->AddGrowableCol( 0 );
	m_pMainSizer->AddGrowableRow( 0 );
	m_pMainSizer->SetFlexibleDirection( wxBOTH );
	m_pMainSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_pCanvasPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pMainSizer->Add( m_pCanvasPanel, 1, wxEXPAND, 5 );
	
	m_pZoomSlider = new wxSlider( this, wxID_ZOOM_FIT, 50, 2, 99, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	m_pZoomSlider->SetToolTip( wxT("Set canvas scale") );
	
	m_pMainSizer->Add( m_pZoomSlider, 0, wxALL|wxEXPAND, 5 );
	
	this->SetSizer( m_pMainSizer );
	this->Layout();
	
	this->Centre( wxBOTH );
}

_MainFrm::~_MainFrm()
{
}
