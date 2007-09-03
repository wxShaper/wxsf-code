#ifndef SF_EVENTS_H
#define SF_EVENTS_H

#include <wx/event.h>
#include "Defs.h"

class WXDLLIMPEXP_SF wxSFShapeBase;
class WXDLLIMPEXP_SF wxSFShapeEvent;

BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_SF, wxEVT_SF_LINE_DONE, 7777)
END_DECLARE_EVENT_TYPES()

typedef void (wxEvtHandler::*wxSFShapeEventFunction)(wxSFShapeEvent&);

#define wxSFShapeEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxSFShapeEventFunction, &func)

#define EVT_SF_LINE_DONE(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( \
        wxEVT_SF_LINE_DONE, id, wxID_ANY, \
        (wxObjectEventFunction)(wxEventFunction) wxStaticCastEvent( wxSFShapeEventFunction, &fn ), \
        (wxObject *) NULL \
    ),

class WXDLLIMPEXP_SF wxSFShapeEvent : public wxEvent
{
public:
    wxSFShapeEvent(wxEventType cmdType = wxEVT_NULL, int id = 0);
    wxSFShapeEvent(const wxSFShapeEvent& obj);
    virtual ~wxSFShapeEvent();

    // public member data accessors
    void SetShape(wxSFShapeBase* line){m_pShape = line;}
    wxSFShapeBase* GetShape(){return m_pShape;}

    wxEvent* Clone() const { return new wxSFShapeEvent(*this); }


private:
    // private data members
    wxSFShapeBase* m_pShape;
};

#endif // SF_EVENTS_H
