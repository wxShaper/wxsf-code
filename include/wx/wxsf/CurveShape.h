/***************************************************************
 * Name:      CurveShape.h
 * Purpose:   Defines curve shape class
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifndef _WXSFCURVESHAPE_H
#define _WXSFCURVESHAPE_H

#include "LineShape.h"

// default values
#define sfdvCURVESHAPE_MAXSTEPS 20

/// <summary> Interpolation line shape. The class extends wxSFLineShape class and allows
/// user to create curved connection line.</summary>
class WXDLLIMPEXP_SF wxSFCurveShape : public wxSFLineShape
{
public:
    XS_DECLARE_CLONABLE_CLASS(wxSFCurveShape);

    /// <summary> Default constructor </summary>
    wxSFCurveShape();
    /// <summary> User constructor </summary>
    /// <param name="maxsteps"> Number of curve elements between two control points </param>
    /// <param name="src"> ID of the source shape </param>
    /// <param name="trg"> ID of the target shape </param>
    /// <param name="path"> Array of line's control points </param>
    /// <param name="manager"> Parent parent diagram manager </param>
	wxSFCurveShape(size_t maxsteps, long src, long trg, const RealPointList& path, wxSFDiagramManager* manager);
	/// <summary> Copy constructor </summary>
	/// <param name="obj"> Reference to the source object </param>
	wxSFCurveShape(const wxSFCurveShape& obj);
	/// <summary> Destructor </summary>
    virtual ~wxSFCurveShape();

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
    long m_nMaxSteps;

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
	virtual void DrawCompleteLine(wxDC& dc);
    /// <summary> Get zero-based index of the line segment laying under given point. </summary>
    /// <param name="pos"> Search point </param>
    /// <returns> Zero-based line segment index </returns>
	virtual int GetHitLinesegment(const wxPoint& pos);

private:

	// private functions

	 /*! \brief Initialize serializable properties. */
	void MarkSerializableDataMembers();
	/// <summary> Auxiliary drawing function </summary>
	void GetUpdatedLineSegment(LineSegmentArray& segments);
	/// <summary> Auxiliary drawing function </summary>
	void Catmul_Rom_Kubika(const wxRealPoint& A, const wxRealPoint& B, const wxRealPoint& C, const wxRealPoint&D, wxDC& dc);
	/// <summary> Auxiliary drawing function </summary>
	wxRealPoint Coord_Catmul_Rom_Kubika(const wxRealPoint& p1, const wxRealPoint& p2, const wxRealPoint& p3, const wxRealPoint& p4, double t);

};

#endif //_WXSFCURVESHAPE_H
