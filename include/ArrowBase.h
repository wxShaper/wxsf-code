/***************************************************************
 * Name:      ArrowBase.h
 * Purpose:   Defines line arrow base class
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#pragma once

#include "ShapeBase.h"

class wxSFLineShape;
/// <summary> Base class for a line arrows </summary>
class wxSFArrowBase :	public xsSerializable
{
public:
	friend class wxSFLineShape;

	DECLARE_DYNAMIC_CLASS(wxSFArrowBase);
    /// <summary> Default constructor </summary>
	wxSFArrowBase(void);
	/// <summary> Constructor </summary>
	/// <param name="parent"> Pointer to a parent shape </param>
	wxSFArrowBase(wxSFShapeBase* parent);
	/// <summary> Copy constructor </summary>
	/// <param name="obj"> Source base arrow shape </param>
	wxSFArrowBase(wxSFArrowBase& obj);
	/// <summary> Destructor </summary>
	~wxSFArrowBase(void);

	// public member data accessors
	/// <summary> Set a parent of the arrow shape </summary>
	/// <param name="parent"> Pointer to the parent shape </param>
	void SetParentShape(wxSFShapeBase* parent){m_pParentShape = parent;}
	/// <summary> Get pointer to a parent shape </summary>
	/// <returns> Pointer to a parent shape if exists, otherwise NULL </returns>
	wxSFShapeBase* GetParentShape(){return m_pParentShape;}

	// public functions
	/// <summary> Clone the arrow shape </summary>
	/// <returns> Pointer to a new instace of the arrow shape </returns>
	wxSFArrowBase* Clone(){return new wxSFArrowBase(*this);}

	// public virtual functions
	/// <summary> Draw arrow shape at the end of a virtual line </summary>
	/// <param name="from"> Start of the virtual line </param>
	/// <param name="to"> End of the virtual line </param>
	/// <param name="dc"> Device context for drawing </param>
	virtual void Draw(const wxRealPoint& from, const wxRealPoint& to, wxSFScaledPaintDC& dc);

protected:

	// protected member data
	/// <summary> Pointer to a parent shape </summary>
	wxSFShapeBase* m_pParentShape;

};

