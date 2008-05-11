/***************************************************************
 * Name:      OpenArrow.h
 * Purpose:   Defines open arrow for line shapes
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifndef _WXSFOPENARROW_H
#define _WXSFOPENARROW_H

#include "ArrowBase.h"

/// <summary> Class extends the wxSFArrowBase class and encapsulates
/// arrow shape consisting of single two lines leading from the end of the
/// parent line shape </summary>
class WXDLLIMPEXP_SF wxSFOpenArrow : public wxSFArrowBase
{
public:
	XS_DECLARE_CLONABLE_CLASS(wxSFOpenArrow);

    /// <summary> Default constructor </summary>
	wxSFOpenArrow(void);
	/// <summary> User constructor </summary>
	/// <param name="parent"> Pointer to the parent shape </param>
	wxSFOpenArrow(wxSFShapeBase* parent);
	/// <summary> Copy constructor </summary>
	/// <param name="obj"> Reference to the source object </param>
	wxSFOpenArrow(const wxSFOpenArrow& obj);
	/// <summary> Destructor </summary>
	virtual ~wxSFOpenArrow(void);

	// public virtual functions
	/// <summary> Draw arrow shape at the end of a virtual line </summary>
	/// <param name="from"> Start of the virtual line </param>
	/// <param name="to"> End of the virtual line </param>
	/// <param name="dc"> Device context for drawing </param>
	virtual void Draw(const wxRealPoint& from, const wxRealPoint& to, wxDC& dc);

protected:
	// protected virtual functions
	/// <summary> Draw arrow shape defined by array of its vertices </summary>
	/// <param name="n"> Number of the array items </param>
	/// <param name="pts"> Array of the arrow shape vertices </param>
	/// <param name="dc"> Device context for drawing </param>
	virtual void DrawArrowShape(int n, wxPoint pts[], wxDC& dc);
};

#endif //_WXSFOPENARROW_H
