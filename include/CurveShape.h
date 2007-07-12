#pragma once

#include "LineShape.h"

// default values
#define sfdvCURVESHAPE_MAXSTEPS 20

/// <summary> Interpolation line shape. The class extends wxSFLineShape class and allows
/// user to create curved connection line.</summary>
class wxSFCurveShape : public wxSFLineShape
{
public:

    DECLARE_DYNAMIC_CLASS(wxSFCurveShape);

    /// <summary> Default constructor </summary>
    wxSFCurveShape();
    /// <summary> User constructor </summary>
    /// <param name="maxsteps"> Number of curve elements between two control points </param>
    /// <param name="src"> ID of the source shape </param>
    /// <param name="trg"> ID of the target shape </param>
    /// <param name="path"> Array of line's control points </param>
    /// <param name="canvas"> Parent shape canvas </param>
	wxSFCurveShape(size_t maxsteps, long src, long trg, const CPointList& path, wxSFShapeCanvas* canvas);
	/// <summary> Copy constructor </summary>
	/// <param name="obj"> Reference to the source object </param>
	wxSFCurveShape(wxSFCurveShape& obj);
	/// <summary> Destructor </summary>
    virtual ~wxSFCurveShape();

	/// <summary> Clone the object itself. </summary>
	/// <returns> Pointer to a new instace of the shape object</returns>
	wxSFCurveShape* Clone(){return new wxSFCurveShape(*this);}

    // public virtual function
    /// <summary> Get line's bounding box. The function can be overrided
    /// if neccessary. </summary>
    /// <returns> Bounding rectangle </returns>
    virtual wxRect GetBoundingBox();

    // public functions
    /// <summary> Get a line point laying on the given line segment and shifted
    /// from the beggining of the segment by given offset. </summary>
    /// <param name="segment"> Zero-based index of the line segment </param>
    /// <param name="offset"> Real value in the range from 0 to 1 which determines
    /// the linepoint offset inside the line segment </param>
    /// <returns> Line point </returns>
    wxRealPoint GetPoint(size_t segment, double offset);

protected:

    // protected data members
    size_t m_nMaxSteps;

    // protected virtual functions
	/// <summary> Serialize shape's properties to the given XML node. </summary>
	/// <param name="node"> Pointer to XML node where the shape's property nodes will be append to </param>
	/// <seealso cref="wxSFShapeBase::Serialize"></seealso>
	virtual wxXmlNode* Serialize(wxXmlNode* node);
	/// <summary> Deserialize shape's properties from the given XML node. </summary>
	/// <param name="node"> Source XML node containig the shape's property nodes</param>
	/// <seealso cref="wxSFShapeBase::Deserialize"></seealso>
	virtual void Deserialize(wxXmlNode* node);

    /// <summary> Internal function used for drawing of completed line shape. </summary>
    /// <param name="dc"> Refernce of the device context where the shape will be darwn to </param>
	virtual void DrawCompleteLine(wxSFScaledPaintDC& dc);
    /// <summary> Get zero-based index of the line segment laying under given point. </summary>
    /// <param name="pos"> Search point </param>
    /// <returns> Zero-based line segment index </returns>
	virtual int GetHitLinesegment(const wxPoint& pos);

private:

	// private functions
	/// <summary> Auxiliary drawing function </summary>
	void GetUpdatedLineSegment(CLineSegmentArray& segments);
	/// <summary> Auxiliary drawing function </summary>
	void Catmul_Rom_Kubika(const wxRealPoint& A, const wxRealPoint& B, const wxRealPoint& C, const wxRealPoint&D, wxSFScaledPaintDC& dc);
	/// <summary> Auxiliary drawing function </summary>
	wxRealPoint Coord_Catmul_Rom_Kubika(const wxRealPoint& p1, const wxRealPoint& p2, const wxRealPoint& p3, const wxRealPoint& p4, double t);

};
