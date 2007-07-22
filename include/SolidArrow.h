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
class wxSFSolidArrow :	public wxSFOpenArrow
{
public:
	DECLARE_DYNAMIC_CLASS(wxSFSolidArrow);

    /// <summary> Default constructor </summary>
	wxSFSolidArrow(void);
	/// <summary> User constructor </summary>
	/// <param name="parent"> Pointer to the parent shape </param>
	wxSFSolidArrow(wxSFShapeBase* parent);
    /// <summary> Copy constructor </summary>
	/// <param name="obj"> Reference to the source object </param>
	wxSFSolidArrow(wxSFSolidArrow& obj);
	/// <summary> Destructor </summary>
	virtual ~wxSFSolidArrow(void);

	// public functions
	/// <summary> Clone the arrow shape </summary>
	/// <returns> Pointer to a new instace of the arrow shape </returns>
	wxSFSolidArrow* Clone(){return new wxSFSolidArrow(*this);}

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

	/// <summary> Serialize shape's properties to the given XML node </summary>
	/// <param name="node"> Pointer to XML node where the shape's property nodes will be append to </param>
	/// <seealso cref="wxSFShapeBase::Serialize"></seealso>
	virtual wxXmlNode* Serialize(wxXmlNode* node);
	/// <summary> Deserialize shape's properties from the given XML node </summary>
	/// <param name="node"> Source XML node containig the shape's property nodes</param>
	/// <seealso cref="wxSFShapeBase::Deserialize"></seealso>
	virtual void Deserialize(wxXmlNode* node);
};

