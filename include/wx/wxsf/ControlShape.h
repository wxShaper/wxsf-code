/***************************************************************
 * Name:      ControlShape.h
 * Purpose:   Defines GUI control shape class
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2008-04-30
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifndef WXSFCONTROLSHAPE_H
#define WXSFCONTROLSHAPE_H

#include "wx/wxsf/RectShape.h"

#define sfFIT_SHAPE_TO_CONTROL true
#define sfFIT_CONTROL_TO_SHAPE false

#define sfdvCONTROLSHAPE_WIDGETOFFSET 0
#define sfdvCONTROLSHAPE_PROCESSEVENTS true

class WXDLLIMPEXP_SF wxSFControlShape;

class EventSink : public wxEvtHandler
{
public:
    /*! \brief Default constructor. */
    EventSink();
    /*!
     * \brief User constructor.
     * \param parent Pointer to parent control shape
     */
    EventSink(wxSFControlShape *parent);
    /*! \brief Destructor. */
    virtual ~EventSink();

    // public functions
	/*!
     * \brief Event handler used for delayed processing of a mouse button events.
	 * The handler creates new key event instance and sends it to a shape canvas for further processing.
	 * \param event Mouse event
	 */
	void _OnMouseButton(wxMouseEvent &event);
	/*!
     * \brief Event handler used for delayed processing of a mouse event (mouse movement).
	 * The handler creates new key event instance and sends it to a shape canvas for further processing.
	 * \param event Mouse event
	 */
	void _OnMouseMove(wxMouseEvent &event);
	/*!
	 * \brief Event handler used for delayed processing of a key event.
	 * The handler creates new key event instance and sends it to a shape canvas for further processing.
	 * \param event Keyboard event
	 */
	void _OnKeyDown(wxKeyEvent &event);

protected:
    // protected data members
    wxSFControlShape *m_pParentShape;

    // protected functions
    void SendEvent(wxEvent &event);
    void UpdateMouseEvent(wxMouseEvent &event);
};

class WXDLLIMPEXP_SF wxSFControlShape : public wxSFRectShape
{
public:
    XS_DECLARE_CLONABLE_CLASS(wxSFControlShape)

    /*! \brief Default constructor. */
    wxSFControlShape();
    /*!
     * \brief User constructor.
     * \param ctrl Pointer to managed GUI control
     * \param pos Initial position
     * \param size Initial size
     * \param manager Pointer to parent diagram manager
     */
    wxSFControlShape(wxWindow *ctrl, const wxRealPoint& pos, const wxRealPoint& size, wxSFDiagramManager* manager);
    /*!
     * \brief Copy constructor.
     * \param other Object to copy from
     */
    wxSFControlShape(const wxSFControlShape& other);
    /*! \brief Default destructor. */
    virtual ~wxSFControlShape();

    // member data accessors
    /*!
     * \brief Set managed GUI control.
     * \param ctrl Pointer to existing manager GUI control
     * \param fit TRUE if the control shape should be resized in accordance to the given GUI control
     */
    void SetControl(wxWindow * ctrl, bool fit = sfFIT_SHAPE_TO_CONTROL);

    /*!
     * \brief Get pointer to managed GUI control.
     * \return Pointer to the GUI control
     */
    wxWindow * GetControl() { return m_pControl; }

    void SetEventProcessing(bool enab){m_fProcessEvents = enab;}

    bool GetEventProcessing(){return m_fProcessEvents;}

    // public virtual functions

    /*!
	 * \brief Scale the shape size by in both directions. The function can be overrided if necessary
     * (new implementation should call default one ore scale shape's children manualy if neccesary).
     * \param x Horizontal scale factor
     * \param y Vertical scale factor
     * \param children TRUE if the shape's children shoould be scaled as well, otherwise the shape will be updated after scaling via Update() function.
     */
	virtual void Scale(double x, double y, bool children = sfWITHCHILDREN);
	/*!
	 * \brief Move the shape to the given absolute position. The function can be overrided if necessary.
	 * \param x X coordinate
	 * \param y Y coordinate
	 */
	virtual void MoveTo(double x, double y);
	/*!
	 * \brief Move the shape by the given offset. The function can be overrided if neccessary.
	 * \param x X offset
	 * \param y Y offset
	 */
	virtual void MoveBy(double x, double y);

	/*!
	 * \brief Event handler called at the begining of the shape dragging process.
	 * The function can be overrided if necessary.
	 *
	 * The function is called by the framework (by the shape canvas).
	 * Default implementation does nothing.
	 * \sa wxSFShapeCanvas
	 */
	virtual void OnBeginDrag(const wxPoint& pos);
	/*!
	 * \brief Event handler called during the shape dragging process.
	 * The function can be overrided if necessary.
	 *
	 * The function is called by the framework (by the shape canvas).
	 * Default implementation does nothing.
	 * \param pos Current mouse position
	 * \sa wxSFShapeCanvas
	 */
	virtual void OnDragging(const wxPoint& pos);
	/*!
	 * \brief Event handler called at the end of the shape dragging process.
	 * The function can be overrided if necessary.
	 *
	 * The function is called by the framework (by the shape canvas).
	 * Default implementation does nothing.
	 * \param pos Current mouse position
	 * \sa wxSFShapeCanvas
	 */
	virtual void OnEndDrag(const wxPoint& pos);
	/*!
	 * \brief Event handler called when the user started to drag the shape handle.
	 * The function can be overrided if necessary.
	 *
	 * The function is called by the framework (by the shape canvas).
	 * Default implementation does nothing.
	 * \param handle Reference to dragged handle
	 */
	virtual void OnBeginHandle(wxSFShapeHandle& handle);
	/*!
	 * \brief Event handler called during dragging of the shape handle.
	 * The function can be overrided if necessary.
	 *
	 * The function is called by the framework (by the shape canvas).
	 * Default implementation does nothing.
	 * \param handle Reference to dragged handle
	 */
	virtual void OnHandle(wxSFShapeHandle& handle);
	/*!
	 * \brief Event handler called when the user finished dragging of the shape handle.
	 * The function can be overrided if necessary.
	 *
	 * The function is called by the framework (by the shape canvas).
	 * Default implementation does nothing.
	 * \param handle Reference to dragged handle
	 */
	virtual void OnEndHandle(wxSFShapeHandle& handle);

protected:

    // protected data members

    /*! \brief Pointer to manager data control */
    wxWindow * m_pControl;
    bool m_fProcessEvents;

    // protected functions
    /*! \brief Update size and position of the managed control according to the parent shape. */
    void UpdateControl();

private:

    // private data members

    /*! \brief Pointer to parent window */
    wxWindow * m_pPrevParent;
    EventSink * m_pEventSink;
    long m_nPrevStyle;

};

#endif // WXSFCONTROLSHAPE_H
