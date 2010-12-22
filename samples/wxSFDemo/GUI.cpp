///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov 29 2010)
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
	
	m_pAutoLayoutMenu = new wxMenu();
	m_pMenuBar->Append( m_pAutoLayoutMenu, wxT("&AutoLayout") ); 
	
	m_pHelpMenu = new wxMenu();
	m_pMenuBar->Append( m_pHelpMenu, wxT("&Help") ); 
	
	this->SetMenuBar( m_pMenuBar );
	
	m_pToolBar = this->CreateToolBar( wxTB_HORIZONTAL, wxID_ANY ); 
	m_pToolBar->Realize(); 
	
	m_pStatusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	wxFlexGridSizer* mainSizer;
	mainSizer = new wxFlexGridSizer( 2, 1, 0, 0 );
	mainSizer->AddGrowableCol( 0 );
	mainSizer->AddGrowableRow( 0 );
	mainSizer->SetFlexibleDirection( wxBOTH );
	mainSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_pCanvasPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pCanvasPanel->SetExtraStyle( wxWS_EX_BLOCK_EVENTS );
	
	m_pCanvasSizer = new wxBoxSizer( wxVERTICAL );
	
	m_pCanvasPanel->SetSizer( m_pCanvasSizer );
	m_pCanvasPanel->Layout();
	m_pCanvasSizer->Fit( m_pCanvasPanel );
	mainSizer->Add( m_pCanvasPanel, 1, wxEXPAND, 5 );
	
	m_pZoomSlider = new wxSlider( this, wxID_ZOOM_FIT, 50, 2, 99, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	m_pZoomSlider->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	m_pZoomSlider->SetToolTip( wxT("Set canvas scale") );
	
	mainSizer->Add( m_pZoomSlider, 0, wxEXPAND, 5 );
	
	this->SetSizer( mainSizer );
	this->Layout();
	
	this->Centre( wxBOTH );
}

_MainFrm::~_MainFrm()
{
}

_ThumbFrm::_ThumbFrm( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_pMainSizer = new wxBoxSizer( wxVERTICAL );
	
	this->SetSizer( m_pMainSizer );
	this->Layout();
}

_ThumbFrm::~_ThumbFrm()
{
}
