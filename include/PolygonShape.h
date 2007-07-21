#pragma once

#include "RectShape.h"

WX_DECLARE_OBJARRAY(wxRealPoint, CPointArray);

// default values
/// <summary> Default value of wxSFPolygonShape::m_fConnextToVertex data member </summary>
#define sfdvPOLYGONSHAPE_VERTEXCONNECTIONS true

// user-defined serialization flags
/// <summary> Use sfsfPOLYGONSHAPE_VERTICES flag with the SetSerializationMask function
/// if the polygon vertices should be serialized together with other polygon's properties. </summary>
#define sfsfPOLYGONSHAPE_VERTICES 16

/// <summary> Class extends the wxSFRectShape and encapsulates general polygon shape
/// defined by a set of its vertices. The class can be used as it is or as a base class
/// for shapes with more complex form and functionality. </summary>
/// <seealso cref="wxSFDiamondShape"></seealso>
class wxSFPolygonShape : public wxSFRectShape
{
public:
	DECLARE_DYNAMIC_CLASS(wxSFPolygonShape);

    /// <summary> Default constructor </summary>
	wxSFPolygonShape(void);
	/// <summary> User constructor </summary>
	/// <param name="n"> Number of the polygon vertices </param>
	/// <param name="pts"> Array of the polygon vertices </param>
	/// <param name="pos"> Relative position of the polygon shape </param>
	/// <param name="parentId"> ID of a parent shape </param>
	/// <param name="canvas"> Pointer of a parent canvas </param>
	wxSFPolygonShape(int n, const wxRealPoint pts[], const wxRealPoint& pos, long parentId, wxSFShapeCanvas* canvas);
	/// <summary> Copy constructor </summary>
	/// <param name="obj"> Reference to a source object </param>
	wxSFPolygonShape(wxSFPolygonShape& obj);
	/// <summary> Destructor </summary>
	virtual ~wxSFPolygonShape(void);

	/// <summary> Clone the object itself. </summary>
	/// <returns> Pointer to a new instace of the shape object</returns>
	wxSFPolygonShape* Clone(){return new wxSFPolygonShape(*this);}

    // public data accessors
    /// <summary> Set connecting mode. </summary>
    /// <param name="enable"> Set this parameter to TRUE if you want to connect
    /// line shapes to the polygons's vertices, otherwise the lines will be connected
    /// to the nearest point of the shape's border. </param>
    void SetConnectToVertex(bool enable){m_fConnectToVertex = enable;}
    /// <summary> Get status of connecting mode. </summary>
    /// <returns> TRUE if the line shapes will be connected to the polygon's vertices </returns>
    bool IsConnectedToVertex(){return m_fConnectToVertex;}

	// public functions
	/// <summary> Set the poly vertices which define its form. </summary>
	/// <param name="n"> Number of the vertices </param>
	/// <param name="pts"> Array of the vertices </param>
	void SetVertices(size_t n, const wxRealPoint pts[]);

	// public virtual functions
	/// <summary> Get intersection point of the polygon's border and a line leading from
	/// the polygon's center to the given point. The function can be overrided if neccessary. </summary>
	/// <param name="to"> Ending point of the virtual intersection line </param>
	/// <returns> Intersection point </returns>
	virtual wxRealPoint GetBorderPoint(const wxRealPoint& to);
	/// <summary> Scale the shape in both directions.
	/// The function can be overrided if neccessary. </summary>
	/// <param name="x"> Scale ratio in the horizontal direction </param>
	/// <param name="y"> Scale ratio in the vertical direction </param>
    /// <param name="children"> TRUE if the shape's children shoould be scaled as well, otherwise
    /// the shape will be updated after scaling via Update() function. </param>
	virtual void Scale(double x, double y, bool children = sfWITHCHILDREN);
	/// <summary> Event handler called during dragging of the shape handle.
	/// The function can be overrided if neccessary. </summary>
	/// <param name="handle"> Reference to the dragged shape handle </param>
	virtual void OnHandle(wxSFShapeHandle &handle);

protected:
	// protected data members
	bool m_fConnectToVertex;
	CPointArray m_arrVertices;

	// protected functions
	/// <summary> Move all vertices so the polygon's relative bounding box position
	/// will be located in the origin </summary>
	void NormalizeVertices();
	/// <summary> Scale polygon's vertices to fit into the rectangle bounding the polygon </summary>
	void FitVerticesToBoundingBox();
	/// <summary> Scale the bounding rectangle to fit all polygons vertices </summary>
	void FitBoundingBoxToVertices();
	/// <summary> Get polygon extents </summary>
	/// <param name="minx"> Position of the left side of polygon's bounding box </param>
	/// <param name="miny"> Position of the top side of polygon's bounding box </param>
	/// <param name="maxx"> Position of the right side of polygon's bounding box </param>
	/// <param name="maxy"> Position of the bottom side of polygon's bounding box </param>
	void GetExtents(double *minx, double *miny, double *maxx, double *maxy);
	/// <summary> Get absolute positions of the polygon's vertices </summary>
	/// <param name="pts"> Array of translated polygon's verices</param>
	void GetTranslatedVerices(wxRealPoint pts[]);

    /// <summary> Draw the polygon shape </summary>
    /// <param name="dc"> Refernece to the device context where the shape will be drawn to </param>
	void DrawPolygonShape(wxSFScaledPaintDC& dc);

	// protected virtual functions
	/// <summary> Draw the shape in the normal way. The function can be overrided if neccessary. </summary>
	/// <param name="dc"> Reference to device context where the shape will be drawn to </param>
	virtual void DrawNormal(wxSFScaledPaintDC& dc);
	/// <summary> Draw the shape in the hower mode (the mouse cursor is above the shape). The function can be overrided if neccessary. </summary>
	/// <param name="dc"> Reference to device context where the shape will be drawn to </param>
	virtual void DrawHover(wxSFScaledPaintDC& dc);
	/// <summary> Draw the shape in the highlighted mode (another shape is dragged over this shape and this shape will accept the dragged one if it will be dropped on it). The function can be overrided if neccessary. </summary>
	/// <param name="dc"> Reference to device context where the shape will be drawn to </param>
	virtual void DrawHighlighted(wxSFScaledPaintDC& dc);

	/// <summary> Serialize shape's properties to the given XML node </summary>
	/// <param name="node"> Pointer to XML node where the shape's property nodes will be append to </param>
	/// <seealso cref="wxSFShapeBase::Serialize"></seealso>
	virtual wxXmlNode* Serialize(wxXmlNode* node);
	/// <summary> Deserialize shape's properties from the given XML node </summary>
	/// <param name="node"> Source XML node containig the shape's property nodes</param>
	/// <seealso cref="wxSFShapeBase::Deserialize"></seealso>
	virtual void Deserialize(wxXmlNode* node);
};

