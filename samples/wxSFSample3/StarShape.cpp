#include "wx_pch.h"

#include "StarShape.h"

// implement RTTI information and xsSerializable::Clone() functions
XS_IMPLEMENT_CLONABLE_CLASS(cStarShape, wxSFPolygonShape);

// define star shape
const wxRealPoint star[10]={wxRealPoint(0,-50), wxRealPoint(15,-10),
                            wxRealPoint(50, -10), wxRealPoint(22, 10),
                            wxRealPoint(40, 50), wxRealPoint(0, 20),
                            wxRealPoint(-40, 50), wxRealPoint(-22, 10),
                            wxRealPoint(-50, -10), wxRealPoint(-15,-10)};

cStarShape::cStarShape()
{
    // disable serialization of polygon vertices, because they are always set
    // in this constructor
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

cStarShape::cStarShape(const cStarShape& obj)
: wxSFPolygonShape(obj)
{
	// clone source child text object..
    m_pText = (wxSFEditTextShape*)obj.m_pText->Clone();
	if( m_pText )
	{
		// .. and append it to this shapes as its child
		AddChild(m_pText);
		// this object is created by the parent class constructor and not
		// by the serializer (only its properties are deserialized)
		XS_SERIALIZE_DYNAMIC_OBJECT_NO_CREATE(m_pText, wxT("title"));
	}
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

    // polygon-based shapes can be connected either to the vertices or to the
    // nearest border point (default value is TRUE).
    SetConnectToVertex(false);

    // set accepted connections for the new shape
    AcceptConnection(wxT("All"));
    AcceptSrcNeighbour(wxT("cStarShape"));
    AcceptTrgNeighbour(wxT("cStarShape"));

	// create associated shape(s)
	m_pText = new wxSFEditTextShape();
    // set some properties
    if(m_pText)
    {
        // set text
        m_pText->SetText(wxT("Hello!"));

        // set alignment
        m_pText->SetVAlign(wxSFShapeBase::valignMIDDLE);
        m_pText->SetHAlign(wxSFShapeBase::halignCENTER);

        // set required shape style(s)
		m_pText->SetStyle(sfsALWAYS_INSIDE | sfsHOVERING | sfsPROCESS_DEL | sfsPROPAGATE_DRAGGING | sfsPROPAGATE_SELECTION);
		// you can also force displaying of the shapes handles even if the interactive
		// size change is not allowed:
		//m_pText->AddStyle(sfsSHOW_HANDLES);

        // components of composite shapes created at runtime in parent shape's
        // constructor cannot be fully serialized (it means created by
		// the serializer) so it is important to disable their standard serialization
        // but they can be still serialized as the parent shape's properties 
		// in the standard way by the following macro:
		SF_ADD_COMPONENT( m_pText, wxT("title") );
    }
}


