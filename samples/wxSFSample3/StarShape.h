#ifndef CSTARSHAPE_H
#define CSTARSHAPE_H

// include main wxSF header file
#include "wx/wxsf/wxShapeFramework.h"


class cStarShape : public wxSFPolygonShape
{
public:
    // enable RTTI information and define xsSerializable::Clone() function used by
	// wxXmlSerilizer::CopyItems() function, the data (shape) manager's
	// copy constructor and in some cases also by the canvas history manager.
    XS_DECLARE_CLONABLE_CLASS(cStarShape);

    // default constructor used by RTTI
    cStarShape();
    // user constructor
    cStarShape(const wxRealPoint& pos, wxSFDiagramManager* manager);
    // copy constructor needed by the xsSerializable::Clone() function
    cStarShape(const cStarShape& obj);
    // destructor
    virtual ~cStarShape();

protected:
    // protected data members
    wxSFEditTextShape* m_pText;
    wxString m_sDescription;

    // protected functions;
    void Initialize();
};



#endif // CSTARSHAPE_H
