/***************************************************************
 * Name:      SFEvents.h
 * Purpose:   Defines shape events classes
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-09-11
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifndef SF_EVENTS_H
#define SF_EVENTS_H

#include <wx/event.h>
#include "Defs.h"

class WXDLLIMPEXP_SF wxSFShapeBase;
class WXDLLIMPEXP_SF wxSFShapeEvent;

BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_SF, wxEVT_SF_LINE_DONE, 7770)
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_SF, wxEVT_SF_TEXT_CHANGE, 7771)
END_DECLARE_EVENT_TYPES()

typedef void (wxEvtHandler::*wxSFShapeEventFunction)(wxSFShapeEvent&);

#define wxSFShapeEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxSFShapeEventFunction, &func)

/*! \brief Event table macro mapping event wxEVT_SF_LINE_DONE. This event occures
 * when the interactive connection creation process is finished. The generated event
 * object holds a pointer to the new line shape. */
#define EVT_SF_LINE_DONE(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( \
        wxEVT_SF_LINE_DONE, id, wxID_ANY, \
        (wxObjectEventFunction)(wxEventFunction) wxStaticCastEvent( wxSFShapeEventFunction, &fn ), \
        (wxObject *) NULL \
    ),

/*! \brief Event table macro mapping event wxEVT_SF_TEXT_CHANGE. This event occures
 * when the editable text shape's content is changed. */
#define EVT_SF_TEXT_CHANGE(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( \
        wxEVT_TEXT_CHANGE, id, wxID_ANY, \
        (wxObjectEventFunction)(wxEventFunction) wxStaticCastEvent( wxSFShapeEventFunction, &fn ), \
        (wxObject *) NULL \
    ),

/*!
 * \brief Class encapsulating generic wxSF shapes' event.
 */
class WXDLLIMPEXP_SF wxSFShapeEvent : public wxEvent
{
public:
    /*! \brief Constructor */
    wxSFShapeEvent(wxEventType cmdType = wxEVT_NULL, int id = 0);
    /*! \brief Copy constructor */
    wxSFShapeEvent(const wxSFShapeEvent& obj);
    /*! \brief Destructor */
    virtual ~wxSFShapeEvent();

    // public member data accessors
    /*!
     * \brief Insert a shape object to the event object.
     * \param shape Pointer to the shape object
     */
    void SetShape(wxSFShapeBase* shape){m_pShape = shape;}
    /*!
     * \brief Get a shape object from the event object.
     * \return Pointer to the shape object.
     */
    wxSFShapeBase* GetShape(){return m_pShape;}

    /*! \brief Clone this event object and return pointer to the new instance. */
    wxEvent* Clone() const { return new wxSFShapeEvent(*this); }


private:
    // private data members
    /*! \brief Pointer to stored shape object. */
    wxSFShapeBase* m_pShape;
};

#endif // SF_EVENTS_H
