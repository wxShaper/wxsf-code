#pragma once

#include "rectshape.h"

// default values

/// <summary> Default value of wxSFRoundRectShape::m_nRadius data member </summary>
#define sfdvROUNDRECTSHAPE_RADIUS 20

/// <summary> Class ecapsulating rounded rectangle. It extends the basic rectangular shape.</summary>
class wxSFRoundRectShape :	public wxSFRectShape
{
public:
	DECLARE_DYNAMIC_CLASS(wxSFRoundRectShape);

    /// <summary> Default constructor </summary>
	wxSFRoundRectShape(void);
	/// <summary> User constructor </summary>
	/// <param name="pos"> Initial position </param>
	/// <param name="size"> Initial size </param>
	/// <param name="radius"> Corner radius </param>
	/// <param name="parentId"> ID of a parent shape </param>
	/// <param name="canvas"> Pointer of a parent canvas </param>
	wxSFRoundRectShape(const wxRealPoint& pos, const wxRealPoint &size, double radius, long parentId, wxSFShapeCanvas* canvas);
	/// <summary> Copy constructor </summary>
	/// <param name="obj"> Refernce to the source object </param>
	wxSFRoundRectShape(wxSFRoundRectShape& obj);
	/// <summary> Destructor </summary>
	virtual ~wxSFRoundRectShape(void);

	/// <summary> Clone the object itself. </summary>
	/// <returns> Pointer to a new instace of the shape object</returns>
	wxSFRoundRectShape* Clone(){return new wxSFRoundRectShape(*this);}

	// public virtual functions
    /// <summary> Test whether the given point is inside the shape. The function
    /// can be overrided if neccessary. </summary>
    /// <param name="pos"> Examined point </param>
    /// <returns> TRUE if the point is inside the shape area, otherwise FALSE </returns>
	virtual bool IsInside(const wxPoint &pos);

	// public member data accessors
	/// <summary> Set corner radius </summary>
	/// <param name="radius"> New corner radius </param>
	void SetRadius(double radius){m_nRadius = radius;}
	/// <summary> Get current corner radius </summary>
	/// <returns>  </returns>
	double GetRadius(){return m_nRadius;}

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

	/// <summary> Serialize shape's properties to the given XML node </summary>
	/// <param name="node"> Pointer to XML node where the shape's property nodes will be append to </param>
	/// <seealso cref="wxSFShapeBase::Serialize"></seealso>
	virtual wxXmlNode* Serialize(wxXmlNode* node);
	/// <summary> Deserialize shape's properties from the given XML node </summary>
	/// <param name="node"> Source XML node containig the shape's property nodes</param>
	/// <seealso cref="wxSFShapeBase::Deserialize"></seealso>
	virtual void Deserialize(wxXmlNode* node);

	// protected functions
	/// <summary> Auxiliary function. Checks whether the point is inside a circle with given center. The circle's radius
	/// is the roundrect corner radius. </summary>
	/// <param name="pos"> Examined point </param>
	/// <param name="center"> Circle center </param>
	bool IsInCircle(const wxPoint& pos, const wxPoint& center);

	// protected data members
	/// <summary> Corner radius </summary>
	double m_nRadius;
};
