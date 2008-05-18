/***************************************************************
 * Name:      SolidArrow.h
 * Purpose:   Defines solid arrow for line shapes
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifndef _WXSFSOLIDARROW_H
#define _WXSFSOLIDARROW_H

#include "OpenArrow.h"

// default values
#define sfdvARROW_FILL *wxWHITE_BRUSH

/*!
 * \brief Class extends the wxSFArrowBase class and encapsulates
 * arrow shape consisting of a solid triangle pointing to the end of the
 * parent line shape.
 */
class WXDLLIMPEXP_SF wxSFSolidArrow :	public wxSFOpenArrow
{
public:
	XS_DECLARE_CLONABLE_CLASS(wxSFSolidArrow);

    /*! \brief Default constructor. */
	wxSFSolidArrow(void);
	/*!
     * \brief User constructor.
	 * \param parent Pointer to the parent shape
	 */
	wxSFSolidArrow(wxSFShapeBase* parent);
    /*!
     * \brief Copy constructor.
	 * \param obj Reference to the source object
	 */
	wxSFSolidArrow(const wxSFSolidArrow& obj);
	/*! \brief Destructor. */
	virtual ~wxSFSolidArrow(void);

	// public functions

	// public member data accessors
	/*!
     * \brief Set a brush filling the arrow's body.
	 * \param br Reference to the brush object
	 */
	void SetArrowFill(const wxBrush& br){m_Fill = br;}
	/*!
     * \brief Get current brush used for filling of the arrow's body.
	 * \return Used brush
	 */
	wxBrush GetArrowFill() const {return m_Fill;}

protected:
	// protected data members
	/*! \brief Arrows brush. */
	wxBrush m_Fill;

	// protected virtual functions
	/*!
     * \brief Draw arrow shape defined by array of its vertices.
	 * \param n Number of the array items
	 * \param pts Array of the arrow shape vertices
	 * \param dc Device context for drawing
	 */
	virtual void DrawArrowShape(int n, wxPoint pts[], wxDC& dc);

private:

	// private functions

	 /*! \brief Initialize serializable properties. */
	void MarkSerializableDataMembers();
};

#endif //_WXSFSOLIDARROW_H
