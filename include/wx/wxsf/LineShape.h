/***************************************************************
 * Name:      LineShape.h
 * Purpose:   Defines line shape class
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#pragma once

#include "ShapeBase.h"
#include "ArrowBase.h"

// default values
/// <summary> Default value of undefined ID </summary>
#define sfdvLINESHAPE_UNKNOWNID -1
/// <summary> Default value of wxSFLineShape::m_Pen data member </summary>
#define sfdvLINESHAPE_PEN *wxBLACK_PEN
/// <summary> Default value of wxSFLineShape::m_nDockPoint data member </summary>
#define sfdvLINESHAPE_DOCKPOINT 0

//WX_DECLARE_LIST(wxRealPoint, CPointList);

/// <summary> Auxiliary class encapsulating the line segment. </summary>
/// <seealso cref="wxSFLineShape"></seealso>
class LineSegment : public wxObject
{
public:
    /// <summary> Default constructor </summary>
    /// <param name="src"> Starting point of the line segment </param>
    /// <param name="trg"> Ending point of the line segmetn </param>
    LineSegment(wxRealPoint src, wxRealPoint trg){m_nSrc = src; m_nTrg = trg;}
    /// <summary> Starting segment position </summary>
    wxRealPoint m_nSrc;
    /// <summary> Ending segment position </summary>
    wxRealPoint m_nTrg;
};

WX_DECLARE_OBJARRAY(LineSegment, LineSegmentArray);

/// <summary> Basic class encapsulating the multiline consisting of several line segments. </summary>
class WXDLLIMPEXP_SF wxSFLineShape : public wxSFShapeBase
{
public:

friend class wxSFShapeCanvas;

	XS_DECLARE_CLONABLE_CLASS(wxSFLineShape);

    /// <summary> Default constructor </summary>
	wxSFLineShape(void);
	/// <summary> User constructor </summary>
	/// <param name="src"> ID of the source shape </param>
	/// <param name="trg"> ID of the target shape </param>
	/// <param name="path"> List of the line control points (can be empty) </param>
	/// <param name="manager"> Pointer to parent shape manager </param>
	wxSFLineShape(long src, long trg, const RealPointList& path, wxSFDiagramManager* manager);
	/// <summary> Copy constructor </summary>
	/// <param name="obj"> Reference to the source object</param>
	wxSFLineShape(const wxSFLineShape& obj);
	/// <summary> Destructor </summary>
	virtual ~wxSFLineShape(void);

	// public member data accessors
	/// <summary> Set line source </summary>
	/// <param name="id"> ID of the source shape </param>
	void SetSrcShapeId(long id){m_nSrcShapeId = id;}
	/// <summary> Get line source </summary>
	/// <returns> ID of the source shape </returns>
	long GetSrcShapeId(){return m_nSrcShapeId;}
	/// <summary> Set line target </summary>
	/// <param name="id"> ID of the target shape </param>
	void SetTrgShapeId(long id){m_nTrgShapeId = id;}
	/// <summary> Get line target </summary>
	/// <returns> ID of the target shape </returns>
	long GetTrgShapeId(){return m_nTrgShapeId;}
	/// <summary> Set source arrow object </summary>
	/// <param name="arrow"> Pointer to the arrow object which will be
	/// assigned to the begin of the line </param>
	void SetSrcArrow(wxSFArrowBase* arrow);
    /// <summary> Set target arrow object </summary>
	/// <param name="arrow"> Pointer to the arrow object which will be
	/// assigned to the end of the line </param>
	void SetTrgArrow(wxSFArrowBase* arrow);
	/// <summary> Set source arrow object created from its class info </summary>
	/// <param name="arrowInfo"> Class info of the arrow class </param>
	/// <returns> New arrow object </returns>
	wxSFArrowBase* SetSrcArrow(wxClassInfo* arrowInfo);
	/// <summary> Get object of source arrow </summary>
	/// <returns> Pointer to the arrow object if exists, otherwise NULL </returns>
	wxSFArrowBase* GetSrcArrow(){return m_pSrcArrow;}
	/// <summary> Set target arrow object created from its class info </summary>
	/// <param name="arrowInfo"> Class info of the arrow class </param>
	/// <returns> New arrow object </returns>
	wxSFArrowBase* SetTrgArrow(wxClassInfo* arrowInfo);
	/// <summary> Get object of target arrow </summary>
	/// <returns> Pointer to the arrow object if exists, otherwise NULL </returns>
	wxSFArrowBase* GetTrgArrow(){return m_pTrgArrow;}
	/// <summary> Set line style </summary>
	/// <param name="pen"> Reference to wxPen object </param>
	void SetLinePen(const wxPen& pen){m_Pen = pen;}
	/// <summary> Get line style </summary>
	/// <returns> wxPen class </returns>
	wxPen GetLinePen() const {return m_Pen;}
	/// <summary> Set the line dock point. It is a zerro based index of the line
	/// control point which will act as the shape position (value returned by GetRelativePosition() function). </summary>
	/// <param name="index"> Zerro based index of the line control point (-1 means UNDEFINED)</param>
	void SetDockPoint(int index){m_nDockPoint = index;}
	/// <summary> Get the line dock point. It is a zerro based index of the line
	/// control point which will act as the shape position (value returned by GetRelativePosition() function). </summary>
	/// <returns> Zerro based index of the line control point (-1 means UNDEFINED) </returns>
	int GetDockPoint(){return m_nDockPoint;}
	/// <summary> Get a list of the line's contol points (their positions). </summary>
	/// <returns> List of control points' positions </returns>
	RealPointList& GetControlPoints() {return m_lstPoints;}
	/// <summary> Get a position of the line dock point. </summary>
	/// <returns> The dock point's position </returns>
	wxRealPoint GetDockPointPosition();

	// public virtual functions
    /// <summary> Get line's bounding box. The function can be overrided
    /// if neccessary. </summary>
    /// <returns> Bounding rectangle </returns>
	virtual wxRect GetBoundingBox();
	/// <summary> Get the shape's absolute position in the canvas </summary>
	/// <returns> Shape's position </returns>
	virtual wxRealPoint GetAbsolutePosition();
	/// <summary> Get intersection point of the shape border and a line leading from
	/// the shape's center to the given point. The function can be overrided if neccessary. </summary>
	/// <param name="to"> Ending point of the virtual intersection line </param>
	/// <returns> Intersection point </returns>
	virtual wxRealPoint GetBorderPoint(const wxRealPoint& to);
    /// <summary> Test whether the given point is inside the shape. The function
    /// can be overrided if neccessary. </summary>
    /// <param name="pos"> Examined point </param>
    /// <returns> TRUE if the point is inside the shape area, otherwise FALSE </returns>
	virtual bool IsInside(const wxPoint& pos);
	/// <summary> Move the shape to the given absolute position. The function
    /// can be overrided if neccessary. </summary>
	/// <param name="x"> X coordinate </param>
	/// <param name="y"> Y coordinate </param>
	virtual void MoveTo(double x, double y);
	/// <summary> Move the shape by the given offset. The function
    /// can be overrided if neccessary. </summary>
	/// <param name="x"> X offset </param>
	/// <param name="y"> Y offset </param>
	virtual void MoveBy(double x, double y);

    /// <summary> Function called by the framework responsible for creation
    /// of the shape's handles. The function can be overrided if neccessary. </summary>
	virtual void CreateHandles();
	/// <summary> Event 	/// <summary> Move the shape by the given offset. The function
    /// can be overrided if neccessary. </summary>
	/// <param name="x"> X offset </param>
	/// <param name="y"> Y offset </param>handler called during dragging of the shape handle.
	/// The function can be overrided if necessary. </summary>
	/// <param name="handle"> Reference to the dragged shape handle </param>
	virtual void OnHandle(wxSFShapeHandle& handle);
	/// <summary> Event handler called at the begining of the shape dragging process.
	/// The function can be overrided if necessary </summary>
	/// <param name="pos"> Current mouse position </param>
	virtual void OnBeginDrag(const wxPoint& pos);
	/// <summary> Event handler called when the shape is double-clicked by
	/// the left mouse button. The function can be overrided if necessary </summary>
	/// <param name="pos"> Current mouse position </param>
	virtual void OnLeftDoubleClick(const wxPoint& pos);
    /// <summary> Scale the shape size by in both directions. The function can be overrided if necessary </summary>
    /// <param name="x"> Horizontal scale factor </param>
    /// <param name="y"> Vertical scale factor </param>
    /// <param name="children"> TRUE if the shape's children shoould be scaled as well, otherwise
    /// the shape will be updated after scaling via Update() function. </param>
	virtual void Scale(double x, double y, bool children = sfWITHCHILDREN);

	// public functions
	/// <summary> Get a list of the line segments</summary>
	/// <param name="segments"> Reference to the list which will contain the line segments </param>
	/// <seealso cref="LineSegment"></seealso>
    void GetLineSegments(LineSegmentArray& segments);

protected:

    /// <summary> The modes in which the line shape can stay </summary>
    enum LINEMODE
    {
        modeREADY,
        modeUNDERCONSTRUCTION,
        modeSRCCHANGE,
        modeTRGCHANGE
    };

	// protected data members
	/// <summary> List of the line's control points </summary>
	RealPointList m_lstPoints;
	wxRealPoint m_nPrevPosition;
	wxPoint m_nUnfinishedPoint;
	LINEMODE m_nMode;
	/// <summary> Index of the line dock point </summary>
	int m_nDockPoint;

	long m_nSrcShapeId;
	long m_nTrgShapeId;
	wxSFArrowBase* m_pSrcArrow;
	wxSFArrowBase* m_pTrgArrow;

	wxPen m_Pen;

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

    /// <summary> Draw completed line </summary>
	virtual void DrawCompleteLine(wxSFScaledPaintDC& dc);

	// protected functions
	/// <summary> Get index of the line segment intersecting the given point </summary>
	/// <param name="pos"> Examined point </param>
	/// <returns> Zero-based index of line segment located under the given point </returns>
	virtual int GetHitLinesegment(const wxPoint& pos);

    /// <summary> Set line shape's working mode </summary>
    /// <param name="mode"> Working mode </param>
    /// <seealso cref="LINEMODE"></seealso>
	void SetLineMode(LINEMODE mode){m_nMode = mode;}
	/// <summary> Get current working mode </summary>
	/// <returns> Current working mode </returns>
	/// <seealso cref="LINEMODE"></seealso>
	LINEMODE GetLineMpode(){return m_nMode;}
	/// <summary> Set next potential control point position (usefull in modeUNDERCONSTRUCTION
	/// working mode) </summary>
	/// <param name="pos"> New potential control point position </param>
	/// <seealso cref="LINEMODE"></seealso>
	void SetUnfinishedPoint(const wxPoint& pos){m_nUnfinishedPoint = pos;}

private:

	// private functions

	 /*! \brief Initialize serializable properties. */
	void MarkSerializableDataMembers();
};
