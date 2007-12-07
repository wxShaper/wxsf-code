#include "wx_pch.h"

#include "SampleCanvas.h"

SampleCanvas::SampleCanvas(wxSFDiagramManager*manager, wxWindow* parent)
: wxSFShapeCanvas(manager, parent)
{
    // set some shape canvas properties if necessary...
	AddStyle(sfsGRID_SHOW);
    AddStyle(sfsGRID_USE);
}

SampleCanvas::~SampleCanvas()
{
}

void SampleCanvas::OnLeftDown(wxMouseEvent& event)
{
    // HINT: perform your user actions here...

    // perform standard operations
    wxSFShapeCanvas::OnLeftDown(event);
}

void SampleCanvas::OnRightDown(wxMouseEvent& event)
{
    // HINT: perform your user actions here...

    // add new rectangular shape to the diagram ...
    wxSFShapeBase* pShape = GetDiagramManager()->AddShape(CLASSINFO(wxSFRectShape), event.GetPosition());
    // set some shape's properties...
    if(pShape)
    {
        // set accepted child shapes for the new shape
        pShape->AcceptChild(wxT("wxSFRectShape"));
    }

    // perform standard operations
    wxSFShapeCanvas::OnRightDown(event);
}
