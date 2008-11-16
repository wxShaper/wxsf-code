/***************************************************************
 * Name:      LineShape.h
 * Purpose:   Defines line shape class
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifndef _WXSFLINESHAPE_H
#define _WXSFLINESHAPE_H

#include "ShapeBase.h"
#include "ArrowBase.h"

// default values
/*! \brief Default value of undefined ID. */
#define sfdvLINESHAPE_UNKNOWNID -1
/*! \brief Default value of wxSFLineShape::m_Pen data member. */
#define sfdvLINESHAPE_PEN *wxBLACK_PEN
/*! \brief Default value of wxSFLineShape::m_nDockPoint data member. */
#define sfdvLINESHAPE_DOCKPOINT 0
/*! \brief Default value of wxSFLineShape::m_nSrcOffset and wxSFLineShape::m_nTrgOffset data members. */
#define sfdvLINESHAPE_OFFSET wxRealPoint(-1, -1)

///*!
// * \brief Auxiliary class encapsulating the line segment.
// * \sa wxSFLineShape
// */
//class LineSegment : public wxObject
//{
//public:
//    /*!
//	 * \brief Default constructor.
//     * \param src Starting point of the line segment
//     * \param trg Ending point of the line segmetn
//     */
//    LineSegment(wxRealPoint src, wxRealPoint trg){m_nSrc = src; m_nTrg = trg;}
//    /*! \brief Starting segment position. */
//    wxRealPoint m_nSrc;
//    /*! \brief Ending segment position. */
//    wxRealPoint m_nTrg;
//};
//
//WX_DECLARE_OBJARRAY(LineSegment, LineSegmentArray);

/*!
 * \brief Basic class encapsulating the multiline consisting of several line segments.
 */
class WXDLLIMPEXP_SF wxSFLineShape : public wxSFShapeBase
{
public:

friend class wxSFShapeCanvas;

	XS_DECLARE_CLONABLE_CLASS(wxSFLineShape);

    /*! \brief Default constructor. */
	wxSFLineShape(void);
	/*!
	 * \brief User constructor.
	 * \param src ID of the source shape
	 * \param trg ID of the target shape
	 * \param path List of the line control points (can be empty)
	 * \param manager Pointer to parent shape manager
	 */
	wxSFLineShape(long src, long trg, const RealPointList& path, wxSFDiagramManager* manager);
	/*!
	 * \brief Copy constructor.
	 * \param obj Reference to the source object
	 */
	wxSFLineShape(const wxSFLineShape& obj);
	/*! \brief Destructor. */
	virtual ~wxSFLineShape(void);

	// public member data accessors
	/*!
	 * \brief Set line source.
	 * \param id ID of the source shape
	 */
	void SetSrcShapeId(long id){m_nSrcShapeId = id;}
	/*!
	 * \brief Get line source.
	 * \return ID of the source shape
	 */
	long GetSrcShapeId(){return m_nSrcShapeId;}
    /*!
	 * \brief Set line target.
	 * \param id ID of the target shape
	 */
	void SetTrgShapeId(long id){m_nTrgShapeId = id;}
	/*!
	 * \brief Get line target.
	 * \return  ID of the target shape
	 */
	long GetTrgShapeId(){return m_nTrgShapeId;}
	/*!
	 * \brief Get first line point.
	 * \return  First line point
	 */	
	wxRealPoint GetSrcPoint();
	/*!
	 * \brief Get last line point.
	 * \return  Last line point
	 */	
	wxRealPoint GetTrgPoint();
	
	void GetDirectLine(wxRealPoint& src, wxRealPoint& trg);
	/*!
	 * \brief Set source arrow object.
	 * \param arrow Pointer to the arrow object which will be assigned to the begin of the line
	 */
	void SetSrcArrow(wxSFArrowBase* arrow);
    /*!
	 * \brief Set target arrow object.
	 * \param arrow Pointer to the arrow object which will be assigned to the end of the line
	 */
	void SetTrgArrow(wxSFArrowBase* arrow);
	/*!
	 * \brief Set source arrow object created from its class info.
	 * \param arrowInfo Class info of the arrow class
	 * \return New arrow object
	 */
	wxSFArrowBase* SetSrcArrow(wxClassInfo* arrowInfo);
	/*!
	 * \brief Get object of source arrow.
	 * \return Pointer to the arrow object if exists, otherwise NULL
	 */
	wxSFArrowBase* GetSrcArrow(){return m_pSrcArrow;}
	/*!
	 * \brief Set target arrow object created from its class info.
	 * \param arrowInfo Class info of the arrow class
	 * \return New arrow object
	 */
	wxSFArrowBase* SetTrgArrow(wxClassInfo* arrowInfo);
	/*!
	 * \brief Get object of target arrow.
	 * \return Pointer to the arrow object if exists, otherwise NULL
	 */
	wxSFArrowBase* GetTrgArrow(){return m_pTrgArrow;}
	/*!
	 * \brief Set line style.
	 * \param pen Reference to wxPen object
	 */
	void SetLinePen(const wxPen& pen){m_Pen = pen;}
	/*!
	 * \brief Get line style.
	 * \return wxPen class
	 */
	wxPen GetLinePen() const {return m_Pen;}
	/*!
	 * \brief Set the line dock point. It is a zerro based index of the line
	 * control point which will act as the shape position (value returned by GetRelativePosition() function).
	 * \param index Zerro based index of the line control point (-1 means UNDEFINED)
	 */
	void SetDockPoint(int index){m_nDockPoint = index;}
	/*!
	 * \brief Get the line dock point. It is a zerro based index of the line
	 * control point which will act as the shape position (value returned by GetRelativePosition() function).
	 * \return Zerro based index of the line control point (-1 means UNDEFINED)
	 */
	int GetDockPoint(){return m_nDockPoint;}
	/*!
	 * \brief Get a list of the line's contol points (their positions).
	 * \return List of control points' positions
	 */
	RealPointList& GetControlPoints() {return m_lstPoints;}
	/*!
	 * \brief Get a position of the line dock point.
	 * \return The dock point's position
	 */
	wxRealPoint GetDockPointPosition();

	// public virtual functions
    /*!
	 * \brief Get line's bounding box. The function can be overrided if neccessary.
     * \return Bounding rectangle
     */
	virtual wxRect GetBoundingBox();
	/*!
	 * \brief Get the shape's absolute position in the canvas.
	 * \return Shape's position
	 */
	virtual wxRealPoint GetAbsolutePosition();
	/*!
	 * \brief Get intersection point of the shape border and a line leading from
	 * 'start' point to 'end' point. The function can be overrided if neccessary.
	 * \param start Starting point of the virtual intersection line
     * \param end Ending point of the virtual intersection line
	 * \return Intersection point
	 */
	virtual wxRealPoint GetBorderPoint(const wxRealPoint& start, const wxRealPoint& end);
    /*!
	 * \brief Test whether the given point is inside the shape. The function
     * can be overrided if neccessary.
     * \param pos Examined point
     * \return TRUE if the point is inside the shape area, otherwise FALSE
     */
	virtual bool IsInside(const wxPoint& pos);
	/*!
	 * \brief Move the shape to the given absolute position. The function
     * can be overrided if neccessary.
	 * \param x X coordinate
	 * \param y Y coordinate
	 */
	virtual void MoveTo(double x, double y);
	/*!
	 * \brief Move the shape by the given offset. The function
     *  can be overrided if neccessary.
	 * \param x X offset
	 * \param y Y offset
	 */
	virtual void MoveBy(double x, double y);

	/*!
	 * \brief Function called by the framework responsible for creation of shape handles
     * at the creation time. The function can be overrided if neccesary.
     */
	virtual void CreateHandles();
	/*!
	 * \brief Event handler called during dragging of the shape handle.
	 * The function can be overrided if necessary.
	 *
	 * The function is called by the framework (by the shape canvas).
	 * \param handle Reference to dragged handle
	 */
	virtual void OnHandle(wxSFShapeHandle& handle);
	/*!
	 * \brief Event handler called when the user finished dragging of the shape handle.
	 * The function can be overrided if necessary.
	 *
	 * The function is called by the framework (by the shape canvas).
	 * Default implementation does nothing.
	 * \param handle Reference to dragged handle
	 */
	virtual void OnEndHandle(wxSFShapeHandle& handle);
	/*!
	 * \brief Event handler called at the begining of the shape dragging process.
	 * The function can be overrided if necessary.
	 *
	 * The function is called by the framework (by the shape canvas).
	 * \sa wxSFShapeCanvas
	 */
	virtual void OnBeginDrag(const wxPoint& pos);
	/*!
	 * \brief Event handler called when the shape is double-clicked by
	 * the left mouse button. The function can be overrided if necessary.
	 *
	 * The function is called by the framework (by the shape canvas).
	 * \param pos Current mouse position
	 * \sa wxSFShapeCanvas
	 */
	virtual void OnLeftDoubleClick(const wxPoint& pos);
    /*!
	 * \brief Scale the shape size by in both directions. The function can be overrided if necessary
     * (new implementation should call default one ore scale shape's children manualy if neccesary).
     * \param x Horizontal scale factor
     * \param y Vertical scale factor
     * \param children TRUE if the shape's children shoould be scaled as well, otherwise the shape will be updated after scaling via Update() function.
     */
	virtual void Scale(double x, double y, bool children = sfWITHCHILDREN);

//	// public functions
//	/*!
//	 * \brief Get a list of the line segments.
//	 * \param segments Reference to the list which will contain the line segments
//	 * \sa LineSegment
//	 */
//    void GetLineSegments(LineSegmentArray& segments);


protected:

    /*! \brief The modes in which the line shape can stay. */
    enum LINEMODE
    {
        modeREADY,
        modeUNDERCONSTRUCTION,
        modeSRCCHANGE,
        modeTRGCHANGE
    };

	// protected data members
	/*! \brief List of the line's control points. */
	RealPointList m_lstPoints;
	wxRealPoint m_nPrevPosition;
	wxPoint m_nUnfinishedPoint;
	LINEMODE m_nMode;
	/*! \brief Index of the line dock point. */
	int m_nDockPoint;

	long m_nSrcShapeId;
	long m_nTrgShapeId;
	wxSFArrowBase* m_pSrcArrow;
	wxSFArrowBase* m_pTrgArrow;

	wxPen m_Pen;

	// protected virtual functions
	/*!
	 * \brief Draw the shape in the normal way. The function can be overrided if neccessary.
	 * \param dc Reference to device context where the shape will be drawn to
	 */
	virtual void DrawNormal(wxDC& dc);
	/*!
	 * \brief Draw the shape in the hower mode (the mouse cursor is above the shape).
	 * The function can be overrided if neccessary.
	 * \param dc Reference to device context where the shape will be drawn to
	 */
	virtual void DrawHover(wxDC& dc);
	/*!
	 * \brief Draw the shape in the highlighted mode (another shape is dragged over this
	 * shape and this shape will accept the dragged one if it will be dropped on it).
	 * The function can be overrided if neccessary.
	 * \param dc Reference to device context where the shape will be drawn to
	 */
	virtual void DrawHighlighted(wxDC& dc);

    /*! \brief Draw completed line. */
	virtual void DrawCompleteLine(wxDC& dc);

	// protected functions
	/*!
     * \brief Get starting and ending point of line segment defined by its index.
	 * \param index Index of desired line segment
	 * \param src Reference to variable where starting point will be stored
	 * \param trg Reference to variable where ending point will be stored 
	 * \return TRUE if a line segment of given index exists, otherwise FALSE
	 */
	bool GetLineSegment(size_t index, wxRealPoint& src, wxRealPoint& trg);
	/*!
     * \brief Get index of the line segment intersecting the given point.
	 * \param pos Examined point
	 * \return Zero-based index of line segment located under the given point
	 */
	virtual int GetHitLinesegment(const wxPoint& pos);

    /*!
     * \brief Set line shape's working mode.
     * \param mode Working mode
     * \sa LINEMODE
     */
	void SetLineMode(LINEMODE mode){m_nMode = mode;}
	/*!
     * \brief Get current working mode.
	 * \return Current working mode
	 * \sa LINEMODE
	 */
	LINEMODE GetLineMode(){return m_nMode;}
	/*!
     * \brief Set next potential control point position (usefull in modeUNDERCONSTRUCTION working mode).
	 * \param pos New potential control point position
	 * \sa LINEMODE
	 */
	void SetUnfinishedPoint(const wxPoint& pos){m_nUnfinishedPoint = pos;}

	/*!
     * \brief Get modified starting line point .
	 * \return Modified starting line point
	 */
	wxRealPoint GetModSrcPoint();
	/*!
     * \brief Get modified ending line point .
	 * \return Modified ending line point
	 */
	wxRealPoint GetModTrgPoint();

private:
    // private data members
    /*! \brief Modification offset for starting line point. */
    wxRealPoint m_nSrcOffset;
    /*! \brief Modification offset for ending line point. */
	wxRealPoint m_nTrgOffset;

	// private functions
    /*! \brief Initialize serializable properties. */
	void MarkSerializableDataMembers();

};

#endif //_WXSFLINESHAPE_H
