#ifndef CSTARSHAPE_H
#define CSTARSHAPE_H

// include main wxSF header file
#include "wx/wxsf/wxShapeFramework.h"


class cStarShape : public wxSFPolygonShape
{
public:
    // enable RTTI infomartion
    DECLARE_DYNAMIC_CLASS(cStarShape);

    // default constructor used by RTTI
    cStarShape();
    // user constructor
    cStarShape(const wxRealPoint& pos, wxSFDiagramManager* manager);
    // copy constructor
    cStarShape(cStarShape& obj);
    // destructor
    virtual ~cStarShape();

protected:
    // protected data members
    wxSFTextShape* m_pText;
    wxString m_sDescription;

    // protected functions;
    void Initialize();
};



#endif // CSTARSHAPE_H
