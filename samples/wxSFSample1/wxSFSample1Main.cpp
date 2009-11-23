/***************************************************************
 * Name:      wxSFSample1Main.cpp
 * Purpose:   Code for Application Frame
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-08-31
 * Copyright: Michal Bližňák ()
 * License:
 **************************************************************/
#include "wx_pch.h"

#include "wxSFSample1Main.h"

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << wxT("-Windows");
#elif defined(__WXMAC__)
        wxbuild << wxT("-Mac");
#elif defined(__UNIX__)
        wxbuild << wxT("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << wxT("-Unicode build");
#else
        wxbuild << wxT("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

BEGIN_EVENT_TABLE(wxSFSample1Frame, wxFrame)
    EVT_CLOSE(wxSFSample1Frame::OnClose)
    EVT_MENU(idMenuQuit, wxSFSample1Frame::OnQuit)
    EVT_MENU(idMenuAbout, wxSFSample1Frame::OnAbout)
END_EVENT_TABLE()

wxSFSample1Frame::wxSFSample1Frame(wxFrame *frame, const wxString& title)
    : wxFrame(frame, -1, title)
{
    SetSize(800, 600);
	
	// initialize event types
	m_mapEventTypeInfo[ wxEVT_SF_SHAPE_LEFT_DOWN ] = wxT("Shape was clicked by LMB (wxEVT_SF_SHAPE_LEFT_DOWN )");
	m_mapEventTypeInfo[ wxEVT_SF_SHAPE_LEFT_DCLICK ] = wxT("Shape was double-clicked by LMB (wxEVT_SF_SHAPE_LEFT_DOWN )");
	m_mapEventTypeInfo[ wxEVT_SF_SHAPE_RIGHT_DOWN ] = wxT("Shape was clicked by RMB (wxEVT_SF_SHAPE_RIGHT_DOWN )");
	m_mapEventTypeInfo[ wxEVT_SF_SHAPE_RIGHT_DCLICK ] = wxT("Shape was double-clicked by RMB (wxEVT_SF_SHAPE_RIGHT_DOWN )");
	m_mapEventTypeInfo[ wxEVT_SF_SHAPE_DRAG_BEGIN ] = wxT("Shape has started to be dragged (wxEVT_SF_SHAPE_DRAG_BEGIN )");
	m_mapEventTypeInfo[ wxEVT_SF_SHAPE_DRAG ] = wxT("Shape is dragging (wxEVT_SF_SHAPE_DRAG )");
	m_mapEventTypeInfo[ wxEVT_SF_SHAPE_DRAG_END ] = wxT("Shape's dragging was finished (wxEVT_SF_SHAPE_DRAG_END )");
	m_mapEventTypeInfo[ wxEVT_SF_SHAPE_HANDLE_BEGIN ] = wxT("Shape handle has started to be dragged (wxEVT_SF_SHAPE_HANDLE_BEGIN )");
	m_mapEventTypeInfo[ wxEVT_SF_SHAPE_HANDLE ] = wxT("Shape handle is dragging (wxEVT_SF_SHAPE_HANDLE )");
	m_mapEventTypeInfo[ wxEVT_SF_SHAPE_HANDLE_END ] = wxT("Shape handle's dragging was finished (wxEVT_SF_SHAPE_HANDLE_END )");
	m_mapEventTypeInfo[ wxEVT_SF_SHAPE_KEYDOWN ] = wxT("Key was pressed (wxEVT_SF_SHAPE_KEYDOWN )");
	m_mapEventTypeInfo[ wxEVT_SF_SHAPE_MOUSE_ENTER ] = wxT("Mouse has entered shape's area (wxEVT_SF_SHAPE_MOUSE_ENTER)");
	m_mapEventTypeInfo[ wxEVT_SF_SHAPE_MOUSE_OVER] = wxT("Mouse is moving over shape's area (wxEVT_SF_SHAPE_MOUSE_OVER)");
	m_mapEventTypeInfo[ wxEVT_SF_SHAPE_MOUSE_LEAVE ] = wxT("Mouse has leaved shape's area (wxEVT_SF_SHAPE_MOUSE_LEAVE)");
	m_mapEventTypeInfo[ wxEVT_SF_SHAPE_CHILD_DROP ] = wxT("Child shape has been assigned to shape (wxEVT_SF_SHAPE_CHILD_DROP)");

#if wxUSE_MENUS
    // create a menu bar
    wxMenuBar* mbar = new wxMenuBar();
    wxMenu* fileMenu = new wxMenu(wxT(""));
    fileMenu->Append(idMenuQuit, wxT("&Quit\tAlt-F4"), wxT("Quit the application"));
    mbar->Append(fileMenu, wxT("&File"));
	
	wxMenu *logMenu = new wxMenu();
	logMenu->AppendCheckItem(idMenuLogMouseEvent, wxT("Log &mouse events"));
	logMenu->AppendCheckItem(idMenuLogHandleEvent, wxT("Log &handle events"));
	logMenu->AppendCheckItem(idMenuLogKeyEvent, wxT("Log &keyboard events"));
	logMenu->AppendCheckItem(idMenuLogChildDropEvent, wxT("Log &child drop event"));
	mbar->Append(logMenu, wxT("&Log"));

    wxMenu* helpMenu = new wxMenu(wxT(""));
    helpMenu->Append(idMenuAbout, wxT("&About\tF1"), wxT("Show info about this application"));
    mbar->Append(helpMenu, wxT("&Help"));

    SetMenuBar(mbar);
#endif // wxUSE_MENUS

	wxFlexGridSizer* mainSizer = new wxFlexGridSizer( 2, 0, 0, 0 );
	mainSizer->AddGrowableCol( 0 );
	mainSizer->AddGrowableRow( 0 );
	mainSizer->SetFlexibleDirection( wxBOTH );
	mainSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    // set some diagram manager properties if necessary...
    // set accepted shapes (accept only wxSFRectShape)
    m_Manager.AcceptShape(wxT("wxSFRectShape"));

    // create shape canvas and associate it with shape manager
    m_pCanvas = new wxSFShapeCanvas(&m_Manager, this);
    // set some shape canvas properties if necessary...
	m_pCanvas->AddStyle(wxSFShapeCanvas::sfsGRID_SHOW);
    m_pCanvas->AddStyle(wxSFShapeCanvas::sfsGRID_USE);

    // connect (some) shape canvas events
    m_pCanvas->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(wxSFSample1Frame::OnLeftClickCanvas), NULL, this);
    m_pCanvas->Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(wxSFSample1Frame::OnRightClickCanvas), NULL, this);
	
	// connect (some) shape events (for full list of available shape/shape canvas events see wxSF's reference documentation).
	m_pCanvas->Connect(wxEVT_SF_SHAPE_LEFT_DOWN, wxSFShapeMouseEventHandler(wxSFSample1Frame::OnShapeMouseEvent), NULL, this);
	m_pCanvas->Connect(wxEVT_SF_SHAPE_LEFT_DCLICK, wxSFShapeMouseEventHandler(wxSFSample1Frame::OnShapeMouseEvent), NULL, this);
	m_pCanvas->Connect(wxEVT_SF_SHAPE_RIGHT_DOWN, wxSFShapeMouseEventHandler(wxSFSample1Frame::OnShapeMouseEvent), NULL, this);
	m_pCanvas->Connect(wxEVT_SF_SHAPE_RIGHT_DCLICK, wxSFShapeMouseEventHandler(wxSFSample1Frame::OnShapeMouseEvent), NULL, this);
	m_pCanvas->Connect(wxEVT_SF_SHAPE_DRAG_BEGIN, wxSFShapeMouseEventHandler(wxSFSample1Frame::OnShapeMouseEvent), NULL, this);
	m_pCanvas->Connect(wxEVT_SF_SHAPE_DRAG, wxSFShapeMouseEventHandler(wxSFSample1Frame::OnShapeMouseEvent), NULL, this);
	m_pCanvas->Connect(wxEVT_SF_SHAPE_DRAG_END, wxSFShapeMouseEventHandler(wxSFSample1Frame::OnShapeMouseEvent), NULL, this);
	
	m_pCanvas->Connect(wxEVT_SF_SHAPE_MOUSE_ENTER, wxSFShapeMouseEventHandler(wxSFSample1Frame::OnShapeMouseEvent), NULL, this);
	m_pCanvas->Connect(wxEVT_SF_SHAPE_MOUSE_OVER, wxSFShapeMouseEventHandler(wxSFSample1Frame::OnShapeMouseEvent), NULL, this);
	m_pCanvas->Connect(wxEVT_SF_SHAPE_MOUSE_LEAVE, wxSFShapeMouseEventHandler(wxSFSample1Frame::OnShapeMouseEvent), NULL, this);
	
	m_pCanvas->Connect(wxEVT_SF_SHAPE_HANDLE_BEGIN, wxSFShapeHandleEventHandler(wxSFSample1Frame::OnShapeHandleEvent), NULL, this);
	m_pCanvas->Connect(wxEVT_SF_SHAPE_HANDLE, wxSFShapeHandleEventHandler(wxSFSample1Frame::OnShapeHandleEvent), NULL, this);
	m_pCanvas->Connect(wxEVT_SF_SHAPE_HANDLE_END, wxSFShapeHandleEventHandler(wxSFSample1Frame::OnShapeHandleEvent), NULL, this);
	
	m_pCanvas->Connect(wxEVT_SF_SHAPE_KEYDOWN, wxSFShapeKeyEventHandler(wxSFSample1Frame::OnShapeKeyEvent), NULL, this);
	
	m_pCanvas->Connect(wxEVT_SF_SHAPE_CHILD_DROP, wxSFShapeChildDropEventHandler(wxSFSample1Frame::OnShapeChildDropEvent), NULL, this);
	
	mainSizer->Add( m_pCanvas, 1, wxEXPAND, 0 );
	
	m_textLog = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,150 ), wxTE_MULTILINE );
	m_textLog->SetFont( wxFont( 8, 74, 90, 90, false, wxT("Sans") ) );
	m_textLog->SetMinSize( wxSize( -1,150 ) );
	
	mainSizer->Add( m_textLog, 0, wxEXPAND, 0 );
	
	SetSizer( mainSizer );
	Layout();

#if wxUSE_STATUSBAR
    // create a status bar with some information about the used wxWidgets version
    CreateStatusBar(2);
    SetStatusText(wxT("Hello wxShapeFramework user!"),0);
    SetStatusText(wxbuildinfo(short_f), 1);
#endif // wxUSE_STATUSBAR

	SetSizer( mainSizer );
	Layout();
    Center();
}


wxSFSample1Frame::~wxSFSample1Frame()
{
}

void wxSFSample1Frame::OnLeftClickCanvas(wxMouseEvent& event)
{
    // HINT: perform your user actions here...

    // ... and then process standard canvas operations
    event.Skip();
}

void wxSFSample1Frame::OnRightClickCanvas(wxMouseEvent& event)
{
    // add new rectangular shape to the diagram ...
    wxSFShapeBase* pShape = m_Manager.AddShape(CLASSINFO(wxSFRectShape), event.GetPosition());
    // set some shape's properties...
    if(pShape)
    {
        // set accepted child shapes for the new shape
        pShape->AcceptChild(wxT("wxSFRectShape"));
		// enable emmiting of shape events
		pShape->AddStyle( wxSFShapeBase::sfsEMIT_EVENTS );
    }

    // ... and process standard canvas operations
    event.Skip();
}

void wxSFSample1Frame::OnClose(wxCloseEvent& WXUNUSED(event))
{
    Destroy();
}

void wxSFSample1Frame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Destroy();
}

void wxSFSample1Frame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxString msg = wxbuildinfo(long_f);

    msg += wxT("\nWelcome to wxSFShapeFramework Sample 1 (c) Michal Bliznak, 2007\n\n");
    msg += wxT("Sample demostrates basic wxSF functionality.\n");
    msg += wxT("wxSF event handlers are connected via 'Connect' function.\n\n");
    msg += wxT("Usage:\n");
    msg += wxT(" - Left mouse click operates with inserted shapes\n");
    msg += wxT(" - Right mouse click inserts a rectangular shape to the canvas\n");
    msg += wxT(" - DEL key removes selected shape\n");

    wxMessageBox(msg, wxT("wxShapeFramework Sample 1"));
}

void wxSFSample1Frame::OnShapeMouseEvent(wxSFShapeMouseEvent& event)
{
	if( GetMenuBar()->GetMenu(1)->IsChecked( idMenuLogMouseEvent ) )
	{
		m_textLog->AppendText( wxString::Format( wxT("%s, ID: %d, Mouse position: %d,%d\n"),
												m_mapEventTypeInfo[event.GetEventType()].c_str(),
												event.GetId(),
												event.GetMousePosition().x,
												event.GetMousePosition().y ) );
	}
}

void wxSFSample1Frame::OnShapeHandleEvent(wxSFShapeHandleEvent& event)
{
	if( GetMenuBar()->GetMenu(1)->IsChecked( idMenuLogHandleEvent ) )
	{
		wxString sHndType;
		
		switch( event.GetHandle().GetType() )
		{
			case wxSFShapeHandle::hndLEFTTOP:
				sHndType = wxT("left-top");
				break;
				
			case wxSFShapeHandle::hndTOP:
				sHndType = wxT("top");
				break;
				
			case wxSFShapeHandle::hndRIGHTTOP:
				sHndType = wxT("right-top");
				break;
				
			case wxSFShapeHandle::hndLEFT:
				sHndType = wxT("left");
				break;
				
			case wxSFShapeHandle::hndRIGHT:
				sHndType = wxT("right");
				break;
				
			case wxSFShapeHandle::hndLEFTBOTTOM:
				sHndType = wxT("left-bottom");
				break;
				
			case wxSFShapeHandle::hndBOTTOM:
				sHndType = wxT("bottom");
				break;
				
			case wxSFShapeHandle::hndRIGHTBOTTOM:
				sHndType = wxT("right-bottom");
				break;
				
			default:
				break;
		}
		
		m_textLog->AppendText( wxString::Format( wxT("%s, Shape ID: %d, Handle type: %d (%s), Delta: %d,%d\n"),
												m_mapEventTypeInfo[event.GetEventType()].c_str(),
												event.GetId(),
												event.GetHandle().GetType(),
												sHndType.c_str(),
												event.GetHandle().GetDelta().x,
												event.GetHandle().GetDelta().y ) );
	}
}

void wxSFSample1Frame::OnShapeKeyEvent(wxSFShapeKeyEvent& event)
{
	if( GetMenuBar()->GetMenu(1)->IsChecked( idMenuLogKeyEvent ) )
	{
		m_textLog->AppendText( wxString::Format( wxT("%s, Shape ID: %d, Key code: %d\n"),
												m_mapEventTypeInfo[event.GetEventType()].c_str(),
												event.GetId(),
												event.GetKeyCode() ) );
	}
}

void wxSFSample1Frame::OnShapeChildDropEvent(wxSFShapeChildDropEvent& event)
{
	if( GetMenuBar()->GetMenu(1)->IsChecked( idMenuLogChildDropEvent ) )
	{
		m_textLog->AppendText( wxString::Format( wxT("%s, Shape ID: %d, Child ID: %d\n"),
												m_mapEventTypeInfo[event.GetEventType()].c_str(),
												event.GetId(),
												event.GetChildShape()->GetId() ) );
	}
}
