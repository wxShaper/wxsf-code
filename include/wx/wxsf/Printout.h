/***************************************************************
 * Name:      Printout.h
 * Purpose:   Defines printout class for shape canvas
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2008-05-06
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifndef WXSFPRINTOUT_H
#define WXSFPRINTOUT_H

#include <wx/print.h>
#include <wx/printdlg.h>

#include "wx/wxsf/Defs.h"

class WXDLLIMPEXP_SF wxSFShapeCanvas;

/*!
 * \brief Auxiliary class encapsulating wxSF printout class providing all necessary functions needed for canvas printing.
 * This class is used directly by the wxSFShapeCanvas and wxWidgets printing framework.
 */
class WXDLLIMPEXP_SF wxSFPrintout : public wxPrintout
{
public:
    /*! \brief Default constructor */
    wxSFPrintout(const wxString& title, wxSFShapeCanvas *canvas);
    /*! \brief Default destructor */
    virtual ~wxSFPrintout();

    // public virtual functions

    /*! \brief Called by printing framework. Functions TRUE if a page of given index already exists in printed document. */
    virtual bool HasPage(int page);
    /*! \brief Called by printing framework. Initialize print job. */
    virtual bool OnBeginDocument(int startPage, int endPage);
    /*! \brief Called by printing framework. Do print job. */
    virtual bool OnPrintPage(int page);
    /*! \brief Called by printing framework. Supply information about printed pages. */
    virtual void GetPageInfo(int *minPage, int *maxPage, int *selPageFrom, int *selPageTo);

protected:
    // protected data members

    /*! \brief Pointer to parent shape canvas. */
    wxSFShapeCanvas *m_pCanvas;
};

#endif // WXSFPRINTOUT_H
