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

/*!
 * \brief Class extends the wxSFArrowBase class and encapsulates
 * arrow shape consisting of single two lines leading from the end of the
 * parent line shape.
 */
class WXDLLIMPEXP_SF wxSFOpenArrow : public wxSFArrowBase
{
public:
	XS_DECLARE_CLONABLE_CLASS(wxSFOpenArrow);

    /*! \brief Default constructor. */
	wxSFOpenArrow(void);
	/*!
     * \brief User constructor.
	 * \param parent"> Pointer to the parent shape
	 */
	wxSFOpenArrow(wxSFShapeBase* parent);
	/*!
     * \brief Copy constructor.
	 * \param obj Reference to the source object
	 */
	wxSFOpenArrow(const wxSFOpenArrow& obj);
	/*! \brief Destructor. */
	virtual ~wxSFOpenArrow(void);

	// public virtual functions
	/*!
     * \brief Draw arrow shape at the end of a virtual line.
	 * \param from Start of the virtual line
	 * \param to End of the virtual line
	 * \param dc Device context for drawing
	 */
	virtual void Draw(const wxRealPoint& from, const wxRealPoint& to, wxDC& dc);

protected:
	// protected virtual functions
    /*!
     * \brief Draw arrow shape defined by array of its vertices.
	 * \param n Number of the array items
	 * \param pts Array of the arrow shape vertices
	 * \param dc Device context for drawing
	 */
	virtual void DrawArrowShape(int n, wxPoint pts[], wxDC& dc);
};

#endif //_WXSFOPENARROW_H
