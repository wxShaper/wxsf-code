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

/*!
 * \brief Interpolation line shape. The class extends wxSFLineShape class and allows
 * user to create curved connection line.
 */
class WXDLLIMPEXP_SF wxSFCurveShape : public wxSFLineShape
{
public:
    XS_DECLARE_CLONABLE_CLASS(wxSFCurveShape);

    /*! \brief Default constructor. */
    wxSFCurveShape();
    /*!
     * \brief User constructor.
     * \param maxsteps Number of curve elements between two control points
     * \param src ID of the source shape
     * \param trg ID of the target shape
     * \param path Array of line's control points
     * \param manager Parent parent diagram manager
     */
	wxSFCurveShape(size_t maxsteps, long src, long trg, const RealPointList& path, wxSFDiagramManager* manager);
	/*!
     * \brief Copy constructor.
	 * \param obj Reference to the source object
	 */
	wxSFCurveShape(const wxSFCurveShape& obj);
	/*! \brief Destructor. */
    virtual ~wxSFCurveShape();

    // public virtual function
    /*!
     * \brief Get line's bounding box. The function can be overrided if neccessary.
     * \return Bounding rectangle
     */
    virtual wxRect GetBoundingBox();

    // public functions
    /*!
     * \brief Get a line point laying on the given line segment and shifted
     * from the beggining of the segment by given offset.
     * \param segment Zero-based index of the line segment
     * \param offset Real value in the range from 0 to 1 which determines
     * the linepoint offset inside the line segment
     * \return Line point
     */
    wxRealPoint GetPoint(size_t segment, double offset);

protected:

    // protected data members
    long m_nMaxSteps;

    // protected virtual functions
    /*!
     * \brief Serialize shape's properties to the given XML node. The serialization
     * routine is automatically called by the framework and should take care about serialization
     * of all specific (non-standard) shape's properties.
     * \param node Pointer to XML node where the shape's property nodes will be appended to
     * \sa xsSerializable::Serialize
     */
	virtual wxXmlNode* Serialize(wxXmlNode* node);
    /*!
     * \brief Deserialize shape's properties from the given XML node. The
     * routine is automatically called by the framework and should take care about deserialization
     * of all specific (non-standard) shape's properties.
     * \param node Pointer to a source XML node containig the shape's property nodes
     * \sa xsSerializable::Deserialize
     */
	virtual void Deserialize(wxXmlNode* node);

    /*!
     * \brief Internal function used for drawing of completed line shape.
     * \param dc Refernce of the device context where the shape will be darwn to
     */
	virtual void DrawCompleteLine(wxDC& dc);
    /*!
     * \brief Get zero-based index of the line segment laying under given point.
     * \param pos Search point
     * \return Zero-based line segment index
     */
	virtual int GetHitLinesegment(const wxPoint& pos);

private:

	// private functions

    /*! \brief Initialize serializable properties. */
	void MarkSerializableDataMembers();
	/*! \brief Auxiliary drawing function. */
	void GetUpdatedLineSegment(LineSegmentArray& segments);
	/*! \brief Auxiliary drawing function. */
	void Catmul_Rom_Kubika(const wxRealPoint& A, const wxRealPoint& B, const wxRealPoint& C, const wxRealPoint& D, wxDC& dc);
	/*! \brief Auxiliary drawing function. */
	wxRealPoint Coord_Catmul_Rom_Kubika(const wxRealPoint& p1, const wxRealPoint& p2, const wxRealPoint& p3, const wxRealPoint& p4, double t);

};

#endif //_WXSFCURVESHAPE_H
