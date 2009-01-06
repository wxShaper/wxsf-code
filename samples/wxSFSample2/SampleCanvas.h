#ifndef SAMPLECANVAS_H
#define SAMPLECANVAS_H

// include main wxShapeFramework header file
#include "wx/wxsf/wxShapeFramework.h"

class SampleCanvas : public wxSFShapeCanvas
{
public:
    SampleCanvas(wxSFDiagramManager*manager, wxWindow* parent);
    virtual ~SampleCanvas();

    // override required wxSF virtual functions
    virtual void OnLeftDown(wxMouseEvent& event);
    virtual void OnRightDown(wxMouseEvent& event);
};



#endif // SAMPLECANVAS_H
