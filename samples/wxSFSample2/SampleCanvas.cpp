#include "wx_pch.h"

#include "SampleCanvas.h"
#include "SampleShape.h"

SampleCanvas::SampleCanvas(wxSFDiagramManager *manager, wxWindow *parent)
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

    // add new custom shape to the diagram ...
    wxSFShapeBase* pShape = GetDiagramManager()->AddShape(CLASSINFO(SampleShape), event.GetPosition());
    // set some shape's properties...
    if(pShape)
    {
        // set accepted child shapes for the new shape
        pShape->AcceptChild(wxT("SampleShape"));
    }

    // perform standard operations
    wxSFShapeCanvas::OnRightDown(event);
}
