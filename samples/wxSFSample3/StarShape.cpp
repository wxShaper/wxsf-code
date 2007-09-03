#include "StarShape.h"

// implement RTTI information
IMPLEMENT_DYNAMIC_CLASS(cStarShape, wxSFPolygonShape);

// define star shape
const wxRealPoint star[10]={wxRealPoint(0,-50), wxRealPoint(15,-10),
                            wxRealPoint(50, -10), wxRealPoint(22, 10),
                            wxRealPoint(40, 50), wxRealPoint(0, 20),
                            wxRealPoint(-40, 50), wxRealPoint(-22, 10),
                            wxRealPoint(-50, -10), wxRealPoint(-15,-10)};

cStarShape::cStarShape()
{
    // disable serialization of polygon vertices, because they are always set
    // in constructor
	EnablePropertySerialization(wxT("vertices"), false);
    // set polygon vertices
	SetVertices(10, star);

    // initialize shape
    Initialize();
}

cStarShape::cStarShape(const wxRealPoint& pos, wxSFDiagramManager* manager)
: wxSFPolygonShape(10, star, pos, manager)
{
    // disable serialization of polygon vertices, because they are always set
    // in constructor
	EnablePropertySerialization(wxT("vertices"), false);

    // initialize shape
    Initialize();
}

cStarShape::cStarShape(cStarShape& obj)
: wxSFPolygonShape(obj)
{
    m_pText = obj.m_pText->Clone();
    AddChild(m_pText);
}

cStarShape::~cStarShape()
{
}

void cStarShape::Initialize()
{
    // initialize custom data members...
    m_sDescription = wxT("Insert some shape's description text here...");
    // now tell the serializer that this data member should be serialized
    // (see library documentation to get more info about available serialization
    // macros (supported data types))
    XS_SERIALIZE(m_sDescription, wxT("description"));

    // polygon-based shapes can be connected both to the vertices and to the
    // nearest border point (default value is TRUE).
    SetConnectToVertex(false);

    // set accepted connections for the new shape
    AcceptConnection(wxT("All"));
    AcceptSrcNeighbour(wxT("cStarShape"));
    AcceptTrgNeighbour(wxT("cStarShape"));

	// create associated shape(s)
    m_pText = new wxSFTextShape();
    // set some properties
    if(m_pText)
    {
        // set text
        m_pText->SetText(wxT("Hello!"));

        // set alignment
        m_pText->SetVAlign(wxSFShapeBase::valignMIDDLE);
        m_pText->SetHAlign(wxSFShapeBase::halignCENTER);

        // disable interactive features if required
        m_pText->EnableParentChange(false);
        m_pText->EnableHighlighting(false);
        m_pText->EnableHovering(false);
        m_pText->EnablePositionChange(false);
        m_pText->EnableSizeChange(false);

        // components of composite shapes created at runtime in parent shape's
        // constructor cannot be serialized in standard way so it is important
        // to disable their serialization...
        m_pText->EnableSerialization(false);
        // ... but they can be serialized like other supported data types (class members)
        XS_SERIALIZE_DYNAMIC_OBJECT_NO_CREATE(m_pText, wxT("title"));

        // assign the text shape to the parent shape
        AddChild(m_pText);
    }
}


