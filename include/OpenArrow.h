#pragma once

#include "ArrowBase.h"

/// <summary> Class extends the wxSFArrowBase class and encapsulates
/// arrow shape consisting of single two lines leading from the end of the
/// parent line shape </summary>
class wxSFOpenArrow : public wxSFArrowBase
{
public:
	DECLARE_DYNAMIC_CLASS(wxSFSolidArrow);

    /// <summary> Default constructor </summary>
	wxSFOpenArrow(void);
	/// <summary> User constructor </summary>
	/// <param name="parent"> Pointer to the parent shape </param>
	wxSFOpenArrow(wxSFShapeBase* parent);
	/// <summary> Copy constructor </summary>
	/// <param name="obj"> Reference to the source object </param>
	wxSFOpenArrow(wxSFOpenArrow& obj);
	/// <summary> Destructor </summary>
	virtual ~wxSFOpenArrow(void);

	// public functions
	/// <summary> Clone the arrow shape </summary>
	/// <returns> Pointer to a new instace of the arrow shape </returns>
	wxSFOpenArrow* Clone(){return new wxSFOpenArrow(*this);}

	// public virtual functions
	/// <summary> Draw arrow shape at the end of a virtual line </summary>
	/// <param name="from"> Start of the virtual line </param>
	/// <param name="to"> End of the virtual line </param>
	/// <param name="dc"> Device context for drawing </param>
	virtual void Draw(const wxRealPoint& from, const wxRealPoint& to, wxSFScaledPaintDC& dc);

protected:
	// protected virtual functions
	/// <summary> Draw arrow shape defined by array of its vertices </summary>
	/// <param name="n"> Number of the array items </param>
	/// <param name="pts"> Array of the arrow shape vertices </param>
	/// <param name="dc"> Device context for drawing </param>
	virtual void DrawArrowShape(int n, wxRealPoint pts[], wxSFScaledPaintDC& dc);
};

