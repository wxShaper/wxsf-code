/***************************************************************
 * Name:      FixedRectShape.h
 * Purpose:   Defines square shape class
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#pragma once

#include "RectShape.h"

/// <summary> Class encapsulating the square shape. It extends the basic rectangular shape. </summary>
/// <seealso cref="wxSFRectShape"></seealso>
class wxSFSquareShape : public wxSFRectShape
{
public:
    DECLARE_DYNAMIC_CLASS(wxSFSquareShape);

    /// <summary> Default constructor </summary>
	wxSFSquareShape(void);
	/// <summary> User constructor </summary>
	/// <param name="pos"> Initial position </param>
	/// <param name="size"> Initial size </param>
	/// <param name="parentId"> ID of the parent shape </param>
	/// <param name="canvas"> Pointer to the parent canvas </param>
	wxSFSquareShape(const wxRealPoint& pos, double size, long parentId, wxSFShapeCanvas* canvas);
	/// <summary> Copy constructor </summary>
	/// <param name="obj"> Reference to the source object </param>
	wxSFSquareShape(wxSFSquareShape& obj);
	/// <summary> Destructor </summary>
    virtual ~wxSFSquareShape();

	/// <summary> Clone the object itself. </summary>
	/// <returns> Pointer to a new instace of the shape object</returns>
	wxSFSquareShape* Clone(){return new wxSFSquareShape(*this);}

    // public virtual functions
	/// <summary> Scale the text shape in both directions.
	/// The function can be overrided if neccessary. </summary>
	/// <param name="x"> Scale ratio in the horizontal direction </param>
	/// <param name="y"> Scale ratio in the vertical direction </param>
    /// <param name="children"> TRUE if the shape's children shoould be scaled as well, otherwise
    /// the shape will be updated after scaling via Update() function. </param>
    virtual void Scale(double x, double y, bool children = sfWITHCHILDREN);
	/// <summary> Event handler called during dragging of the shape handle.
	/// The function can be overrided if neccessary. </summary>
	/// <param name="handle"> Reference to dragged shape handle </param>
    virtual void OnHandle(wxSFShapeHandle& handle);
};


