/***************************************************************
 * Name:      RectShape.h
 * Purpose:   Defines rectangular shape class
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#pragma once

#include "ShapeBase.h"

// default values
/// <summary> Default value of wxSFRectShape::m_nRectSize data member </summary>
#define sfdvRECTSHAPE_SIZE wxRealPoint(100, 50)
/// <summary> Default value of wxSFRectShape::m_Fill data member </summary>
#define sfdvRECTSHAPE_FILL *wxWHITE_BRUSH
/// <summary> Default value of wxSFRectShape::m_Border data member </summary>
#define sfdvRECTSHAPE_BORDER *wxBLACK_PEN

/// <summary> Class encapsulates basic rectangle shape which is used as a base class
/// for many other shapes that can be bounded by a simple rectangle. The class
/// provides all functionality needed for manipulating the rectangle's (bounding box)
/// size and position. </summary>
class WXDLLIMPEXP_SF wxSFRectShape : public wxSFShapeBase
{
public:
	XS_DECLARE_CLONABLE_CLASS(wxSFRectShape);

    /// <summary> Default constructor </summary>
	wxSFRectShape(void);
	/// <summary> User constructor </summary>
	/// <param name="pos"> Initial position </param>
	/// <param name="size"> Initial size </param>
	/// <param name="manager"> Pointer to parent diagram manager </param>
	wxSFRectShape(const wxRealPoint& pos, const wxRealPoint& size, wxSFDiagramManager* manager);
	/// <summary> Copy constructor </summary>
	/// <param name="obj"> Refernce to the source object </param>
	wxSFRectShape(const wxSFRectShape& obj);
	/// <summary> Destructor </summary>
	virtual ~wxSFRectShape(void);

	// public virtual functions
    /// <summary> Get shapes's bounding box. The function can be overrided
    /// if neccessary. </summary>
    /// <returns> Bounding rectangle </returns>
	virtual wxRect GetBoundingBox();
	/// <summary> Get intersection point of the shape's border and a line leading from
	/// the shape's center to the given point. The function can be overrided if neccessary. </summary>
	/// <param name="to"> Ending point of the virtual intersection line </param>
	/// <returns> Intersection point </returns>
	virtual wxRealPoint GetBorderPoint(const wxRealPoint& to);

    /// <summary> Function called by the framework responsible for creation
    /// of the shape's handles. The function can be overrided if neccessary. </summary>
	virtual void CreateHandles();
	/// <summary> Event handler called during dragging of the shape handle.
	/// The function can be overrided if necessary. </summary>
	/// <param name="handle"> Reference to the dragged shape handle </param>
	virtual void OnHandle(wxSFShapeHandle& handle);
	/// <summary> Event handler called when the user started to drag the shape handle.
	/// The function can be overrided if neccessary. </summary>
	/// <param name="handle"> Reference to the dragged shape handle </param>
	virtual void OnBeginHandle(wxSFShapeHandle& handle);

    /// <summary> Resize the rectangle to bound all child shapes.
    /// The function can be overrided if neccessary. </summary>
	virtual void FitToChildren();
	/// <summary> Scale the shape in both directions.
	/// The function can be overrided if neccessary. </summary>
	/// <param name="x"> Scale ratio in the horizontal direction </param>
	/// <param name="y"> Scale ratio in the vertical direction </param>
    /// <param name="children"> TRUE if the shape's children shoould be scaled as well, otherwise
    /// the shape will be updated after scaling via Update() function. </param>
	virtual void Scale(double x, double y, bool children = sfWITHCHILDREN);

	// public data accessors
	/// <summary> Set rectangle's fill style </summary>
	/// <param name="brush"> Refernce to a brush object </param>
	void SetFill(const wxBrush& brush){m_Fill = brush;}
	/// <summary> Get current fill style </summary>
	/// <returns> Current brush </returns>
	wxBrush GetFill() const {return m_Fill;}
	/// <summary> Set rectangle's border style </summary>
	/// <param name="pen"> Refernce to a pen object </param>
	void SetBorder(const wxPen& pen){m_Border = pen;}
	/// <summary> Get current border style </summary>
	/// <returns> Current pen </returns>
	wxPen GetBorder() const {return m_Border;}
	/// <summary> Set the rectangle size </summary>
	/// <param name="size"> New size </param>
	void SetRectSize(const wxRealPoint& size){m_nRectSize = size;}
	/// <summary> Set the rectangle size </summary>
	/// <param name="x"> Horizontal size </param>
	/// <param name="y"> Verical size </param>
	void SetRectSize(double x, double y){m_nRectSize.x = x; m_nRectSize.y = y;}
	/// <summary> Get the rectangle size </summary>
	/// <returns> Current size </returns>
	wxRealPoint GetRectSize() const {return m_nRectSize;}


protected:

	// protected data members
	/// <summary> Pen object used for drawing of the rectangle border </summary>
	wxPen m_Border;
	/// <summary> Brush object used for drawing of the rectangle body </summary>
	wxBrush m_Fill;
	/// <summary> The rectangle size </summary>
	wxRealPoint m_nRectSize;

	// protected virtual functions
	/// <summary> Draw the shape in the normal way. The function can be overrided if neccessary. </summary>
	/// <param name="dc"> Reference to device context where the shape will be drawn to </param>
	virtual void DrawNormal(wxDC& dc);
	/// <summary> Draw the shape in the hower mode (the mouse cursor is above the shape). The function can be overrided if neccessary. </summary>
	/// <param name="dc"> Reference to device context where the shape will be drawn to </param>
	virtual void DrawHover(wxDC& dc);
	/// <summary> Draw the shape in the highlighted mode (another shape is dragged over this shape and this shape will accept the dragged one if it will be dropped on it). The function can be overrided if neccessary. </summary>
	/// <param name="dc"> Reference to device context where the shape will be drawn to </param>
	virtual void DrawHighlighted(wxDC& dc);
	/*!
	 * \brief Draw shadow under the shape. The function can be overrided if neccessary.
	 * \param dc Reference to device context where the shadow will be drawn to
	 */
	virtual void DrawShadow(wxDC& dc);

	/// <summary> Serialize shape's properties to the given XML node </summary>
	/// <param name="node"> Pointer to XML node where the shape's property nodes will be append to </param>
	/// <seealso cref="wxSFShapeBase::Serialize"></seealso>
	virtual wxXmlNode* Serialize(wxXmlNode* node);
	/// <summary> Deserialize shape's properties from the given XML node </summary>
	/// <param name="node"> Source XML node containig the shape's property nodes</param>
	/// <seealso cref="wxSFShapeBase::Deserialize"></seealso>
	virtual void Deserialize(wxXmlNode* node);

	/// <summary> Event handler called during dragging of the right shape handle.
	/// The function can be overrided if neccessary. </summary>
	/// <param name="handle"> Reference to dragged shape handle </param>
	virtual void OnRightHandle(wxSFShapeHandle& handle);
	/// <summary> Event handler called during dragging of the left shape handle.
	/// The function can be overrided if neccessary. </summary>
	/// <param name="handle"> Reference to dragged shape handle </param>
	virtual void OnLeftHandle(wxSFShapeHandle& handle);
	/// <summary> Event handler called during dragging of the top shape handle.
	/// The function can be overrided if neccessary. </summary>
	/// <param name="handle"> Reference to dragged shape handle </param>
	virtual void OnTopHandle(wxSFShapeHandle& handle);
	/// <summary> Event handler called during dragging of the bottom shape handle.
	/// The function can be overrided if neccessary. </summary>
	/// <param name="handle"> Reference to dragged shape handle </param>
	virtual void OnBottomHandle(wxSFShapeHandle& handle);

private:

	// private functions

	 /*! \brief Initialize serializable properties. */
	void MarkSerializableDataMembers();

    /// <summary> Auxiliary data member </summary>
    wxRealPoint m_nPrevSize;
    /// <summary> Auxiliary data member </summary>
    wxRealPoint m_nPrevPosition;
};
