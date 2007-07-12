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
    virtual void Scale(double x, double y);
	/// <summary> Event handler called during dragging of the shape handle.
	/// The function can be overrided if neccessary. </summary>
	/// <param name="handle"> Reference to dragged shape handle </param>
    virtual void OnHandle(wxSFShapeHandle& handle);
};


