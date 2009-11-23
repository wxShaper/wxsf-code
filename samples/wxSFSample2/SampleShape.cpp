#include "wx_pch.h"

#include "SampleShape.h"
#include "wxSFSample2Main.h"

XS_IMPLEMENT_CLONABLE_CLASS(SampleShape, wxSFRectShape);

// constructors and destructor ///////////////////////////////////////////////////////////////////////////

SampleShape::SampleShape() : wxSFRectShape()
{
}

SampleShape::SampleShape(const SampleShape& obj) : wxSFRectShape(obj)
{
}

SampleShape::~SampleShape()
{
}

// virtual event handlers ////////////////////////////////////////////////////////////////////////////////

void SampleShape::OnBeginDrag(const wxPoint& pos)
{
	wxSFSample2Frame::Log( wxSFSample2Frame::logMouseEvent, wxString::Format( wxT("Called handler: 'OnBeginDrag()', Shape ID: %d, Position: %d, %d\n"), GetId(), pos.x, pos.y ) );
	
	// call original handler if required
	wxSFRectShape::OnBeginDrag(pos);
}

void SampleShape::OnDragging(const wxPoint& pos)
{
	wxSFSample2Frame::Log( wxSFSample2Frame::logMouseEvent, wxString::Format( wxT("Called handler: 'OnDragging()', Shape ID: %d, Position: %d, %d\n"), GetId(), pos.x, pos.y ) );
	
	// call original handler if required
	wxSFRectShape::OnDragging(pos);
}

void SampleShape::OnEndDrag(const wxPoint& pos)
{
	wxSFSample2Frame::Log( wxSFSample2Frame::logMouseEvent, wxString::Format( wxT("Called handler: 'OnEndDrag()', Shape ID: %d, Position: %d, %d\n"), GetId(), pos.x, pos.y ) );
	
	// call original handler if required
	wxSFRectShape::OnEndDrag(pos);
}

void SampleShape::OnBeginHandle(wxSFShapeHandle& handle)
{
	wxSFSample2Frame::Log( wxSFSample2Frame::logHandleEvent, wxString::Format( wxT("Called handler: 'OnBeginHandle()', Shape ID: %d, Handle type: %d\n"), GetId(), (int)handle.GetType() ) );
	
	// call original handler if required
	wxSFRectShape::OnBeginHandle(handle);
}

void SampleShape::OnHandle(wxSFShapeHandle& handle)
{
	wxSFSample2Frame::Log( wxSFSample2Frame::logHandleEvent, wxString::Format( wxT("Called handler: 'OnHandle()', Shape ID: %d, Handle type: %d, Delta: %d, %d\n"), GetId(), (int)handle.GetType(), handle.GetDelta().x, handle.GetDelta().y ) );
	
	// call original handler if required
	wxSFRectShape::OnHandle(handle);
}

void SampleShape::OnEndHandle(wxSFShapeHandle& handle)
{
	wxSFSample2Frame::Log( wxSFSample2Frame::logHandleEvent, wxString::Format( wxT("Called handler: 'OnEndHandle()', Shape ID: %d, Handle type: %d\n"), GetId(), (int)handle.GetType() ) );
	
	// call original handler if required
	wxSFRectShape::OnEndHandle(handle);
}

void SampleShape::OnChildDropped(const wxRealPoint& pos, wxSFShapeBase* child)
{
	wxSFSample2Frame::Log( wxSFSample2Frame::logChildDropEvent, wxString::Format( wxT("Called handler: 'OnChildDropped()', Parent shape ID: %d, Child shape ID: %d\n"), GetId(), child->GetId() ) );
	
	// call original handler if required
	wxSFRectShape::OnChildDropped(pos, child);
}

bool SampleShape::OnKey(int key)
{
	wxSFSample2Frame::Log( wxSFSample2Frame::logKeyEvent, wxString::Format( wxT("Called handler: 'OnKey()', Shape ID: %d, Key code: %d\n"), GetId(), key ) );
	
	// call original handler if required
	return wxSFRectShape::OnKey(key);
}

void SampleShape::OnLeftClick(const wxPoint& pos)
{
	wxSFSample2Frame::Log( wxSFSample2Frame::logMouseEvent, wxString::Format( wxT("Called handler: 'OnLeftClick()', Shape ID: %d, Position: %d, %d\n"), GetId(), pos.x, pos.y ) );
	
	// call original handler if required
	wxSFRectShape::OnLeftClick(pos);
}

void SampleShape::OnLeftDoubleClick(const wxPoint& pos)
{
	wxSFSample2Frame::Log( wxSFSample2Frame::logMouseEvent, wxString::Format( wxT("Called handler: 'OnLeftDoubleClick()', Shape ID: %d, Position: %d, %d\n"), GetId(), pos.x, pos.y ) );
	
	// call original handler if required
	wxSFRectShape::OnLeftDoubleClick(pos);
}

void SampleShape::OnRightClick(const wxPoint& pos)
{
	wxSFSample2Frame::Log( wxSFSample2Frame::logMouseEvent, wxString::Format( wxT("Called handler: 'OnRightClick()', Shape ID: %d, Position: %d, %d\n"), GetId(), pos.x, pos.y ) );
	
	// call original handler if required
	wxSFRectShape::OnRightClick(pos);
}

void SampleShape::OnRightDoubleClick(const wxPoint& pos)
{
	wxSFSample2Frame::Log( wxSFSample2Frame::logMouseEvent, wxString::Format( wxT("Called handler: 'OnRightDoubleClick()', Shape ID: %d, Position: %d, %d\n"), GetId(), pos.x, pos.y ) );
	
	// call original handler if required
	wxSFRectShape::OnRightDoubleClick(pos);
}

void SampleShape::OnMouseEnter(const wxPoint& pos)
{
	wxSFSample2Frame::Log( wxSFSample2Frame::logMouseEvent, wxString::Format( wxT("Called handler: 'OnMouseEnter()', Shape ID: %d, Position: %d, %d\n"), GetId(), pos.x, pos.y ) );
	
	// call original handler if required
	wxSFRectShape::OnMouseEnter(pos);
}

void SampleShape::OnMouseOver(const wxPoint& pos)
{
	wxSFSample2Frame::Log( wxSFSample2Frame::logMouseEvent, wxString::Format( wxT("Called handler: 'OnMouseOver()', Shape ID: %d, Position: %d, %d\n"), GetId(), pos.x, pos.y ) );
	
	// call original handler if required
	wxSFRectShape::OnMouseOver(pos);
}

void SampleShape::OnMouseLeave(const wxPoint& pos)
{
	wxSFSample2Frame::Log( wxSFSample2Frame::logMouseEvent, wxString::Format( wxT("Called handler: 'OnMouseLeave()', Shape ID: %d, Position: %d, %d\n"), GetId(), pos.x, pos.y ) );
	
	// call original handler if required
	wxSFRectShape::OnMouseLeave(pos);
}
