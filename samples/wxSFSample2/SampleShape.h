#ifndef __sampleshape__
#define __sampleshape__

// include main wxShapeFramework header file
#include "wx/wxsf/wxShapeFramework.h"

class SampleShape : public wxSFRectShape
{
public:
	XS_DECLARE_CLONABLE_CLASS(SampleShape);

	SampleShape();
	SampleShape(const SampleShape& obj);
	virtual ~SampleShape();

public:
	// override required wxSF virtual functions
	virtual void OnLeftClick(const wxPoint& pos);
	virtual void OnRightClick(const wxPoint& pos);
	virtual void OnLeftDoubleClick(const wxPoint& pos);
	virtual void OnRightDoubleClick(const wxPoint& pos);
	
	virtual void OnBeginDrag(const wxPoint& pos);
	virtual void OnDragging(const wxPoint& pos);
	virtual void OnEndDrag(const wxPoint& pos);
	
	virtual void OnBeginHandle(wxSFShapeHandle& handle);
	virtual void OnHandle(wxSFShapeHandle& handle);
	virtual void OnEndHandle(wxSFShapeHandle& handle);
	
	virtual void OnMouseEnter(const wxPoint& pos);
	virtual void OnMouseOver(const wxPoint& pos);
	virtual void OnMouseLeave(const wxPoint& pos);
	
	virtual bool OnKey(int key);
	
	virtual void OnChildDropped(const wxRealPoint& pos, wxSFShapeBase *child);
};

#endif // __sampleshape__
