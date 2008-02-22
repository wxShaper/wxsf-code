/***************************************************************
 * Name:      SolidArrow.h
 * Purpose:   Defines solid arrow for line shapes
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#pragma once

#include "OpenArrow.h"

// default values
#define sfdvARROW_FILL *wxWHITE_BRUSH

/// <summary> Class extends the wxSFArrowBase class and encapsulates
/// arrow shape consisting of a solid triangle pointing to the end of the
/// parent line shape </summary>
class WXDLLIMPEXP_SF wxSFSolidArrow :	public wxSFOpenArrow
{
public:
	XS_DECLARE_CLONABLE_CLASS(wxSFSolidArrow);

    /// <summary> Default constructor </summary>
	wxSFSolidArrow(void);
	/// <summary> User constructor </summary>
	/// <param name="parent"> Pointer to the parent shape </param>
	wxSFSolidArrow(wxSFShapeBase* parent);
    /// <summary> Copy constructor </summary>
	/// <param name="obj"> Reference to the source object </param>
	wxSFSolidArrow(const wxSFSolidArrow& obj);
	/// <summary> Destructor </summary>
	virtual ~wxSFSolidArrow(void);

	// public functions

	// public member data accessors
	/// <summary> Set a brush filling the arrow's body </summary>
	/// <param name="br"> Reference to the brush object </param>
	void SetArrowFill(const wxBrush& br){m_Fill = br;}
	/// <summary> Get current brush used for filling of the arrow's body </summary>
	/// <returns> Used brush </returns>
	wxBrush GetArrowFill() const {return m_Fill;}

protected:
	// protected data members
	/// <summary> Arrows brush </summary>
	wxBrush m_Fill;

	// protected virtual functions
	/// <summary> Draw arrow shape defined by array of its vertices </summary>
	/// <param name="n"> Number of the array items </param>
	/// <param name="pts"> Array of the arrow shape vertices </param>
	/// <param name="dc"> Device context for drawing </param>
	virtual void DrawArrowShape(int n, wxRealPoint pts[], wxSFScaledPaintDC& dc);

private:

	// private functions

	 /*! \brief Initialize serializable properties. */
	void MarkSerializableDataMembers();
};

