/***************************************************************
 * Name:      GridShape.h
 * Purpose:   Defines grid shape class
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2008-08-02
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifndef _WXSFGRIDSHAPE_H
#define _WXSFGRIDSHAPE_H

#include "RectShape.h"

// default values
/*! \brief Default value of wxSFGridShape::m_nRows data member. */
#define sfdvGRIDSHAPE_ROWS 3
/*! \brief Default value of wxSFGridShape::m_nCols data member. */
#define sfdvGRIDSHAPE_COLS 3
/*! \brief Default value of wxSFGridShape::m_nCellSpace data member. */
#define sfdvGRIDSHAPE_CELLSPACE 5

/*!
 * \brief Class encapsulates a rectangular shape derived from wxSFRectShape class which acts as a grid-based
 * container able to manage other assigned child shapes (it can control their position and size). The managed
 * shapes are aligned into defined grid with a behaviour similar to classic wxWidget's wxGridSizer class.
 */
class WXDLLIMPEXP_SF wxSFGridShape : public wxSFRectShape
{
    public:
        XS_DECLARE_CLONABLE_CLASS(wxSFRectShape);

        /*! \brief Default constructor. */
        wxSFGridShape();
        /*!
         * \brief User constructor.
         * \param pos Initial position
         * \param size Initial size
         * \param rows Number of grid rows
         * \param cols Number of grid columns
         * \param cols Additional space between managed shapes
         * \param manager Pointer to parent diagram manager
         */
        wxSFGridShape(const wxRealPoint& pos, const wxRealPoint& size, int rows, int cols, int cellspace, wxSFDiagramManager* manager);
        /*!
        * \brief Copy constructor.
        * \param obj Reference to the source object
        */
        wxSFGridShape(const wxSFGridShape& obj);
        /*! \brief Destructor. */
        virtual ~wxSFGridShape();

        // public member data accessors

        // public functions

        // public virtual functions
        /*! \brief Upate shape (align all child shapes an resize it to fit them) */
        virtual void Update();

        /*! \brief Do layout of assigned child shapes */
        virtual void DoChildrenLayout();

        /*!
         * \brief Event handler called when any shape is dropped above this shape (and the dropped
         * shape is accepted as a child of this shape). The function can be overrided if necessary.
         *
         * The function is called by the framework (by the shape canvas).
         * \param pos Relative position of dropped shape
         * \param child Pointer to dropped shape
         */
        virtual void OnChildDropped(const wxRealPoint& pos, wxSFShapeBase *child);

    protected:
        // protected data members
        /*! \brief Number of grid rows. */
        int m_nRows;
        /*! \brief Number of grid columns. */
        int m_nCols;
        /*! \brief Space additional space between managed shapes. */
        int m_nCellSpace;



    private:

        // private functions

         /*! \brief Initialize serializable properties. */
        void MarkSerializableDataMembers();
};

#endif // WXSFGRIDSHAPE_H
