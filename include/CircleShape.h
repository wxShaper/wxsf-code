/***************************************************************
 * Name:      CircleShape.h
 * Purpose:   Defines circle shape class
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#pragma once

#include "FixedRectShape.h"

/// <summary> Class encapsulating the circle shape.</summary>
class wxSFCircleShape : public wxSFSquareShape
{
public:
    DECLARE_DYNAMIC_CLASS(wxSFCircleShape);

    /// <summary> Default constructor </summary>
	wxSFCircleShape(void);
	/// <summary> User constructor </summary>
	/// <param name="pos"> Initial shape position </param>
	/// <param name="radius"> Circle radius </param>
	/// <param name="parentId"> ID of the parent shape </param>
	/// <param name="manager"> Pointer to parent manager </param>
	wxSFCircleShape(const wxRealPoint& pos, double radius, long parentId, wxSFDiagramManager* manager);
	/// <summary> Copy constructor </summary>
	/// <param name="obj"> Reference to the source object </param>
	wxSFCircleShape(wxSFCircleShape& obj);
	/// <summary> Destructor </summary>
    virtual ~wxSFCircleShape();

	/// <summary> Clone the object itself. </summary>
	/// <returns> Pointer to a new instace of the shape object</returns>
	wxSFCircleShape* Clone(){return new wxSFCircleShape(*this);}

    // public virtual functions
    /// <summary> Test whether the given point is inside the shape. The function
    /// can be overrided if neccessary. </summary>
    /// <param name="pos"> Examined point </param>
    /// <returns> TRUE if the point is inside the shape area, otherwise FALSE </returns>
	virtual bool IsInside(const wxPoint& pos);
	/// <summary> Get intersection point of the shape border and a line leading from
	/// the shape's center to the given point. The function can be overrided if neccessary. </summary>
	/// <param name="to"> Ending point of the virtual intersection line </param>
	/// <returns> Intersection point </returns>
	virtual wxRealPoint GetBorderPoint(const wxRealPoint& to);

protected:

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
};


