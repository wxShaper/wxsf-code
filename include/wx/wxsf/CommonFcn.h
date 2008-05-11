/***************************************************************
 * Name:      CommonFcn.h
 * Purpose:   Defines set of global (auxiliary) functions
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifndef _WXSFCOMMONFCN_H
#define _WXSFCOMMONFCN_H

#include <wx/wxprec.h>
#include <wx/xml/xml.h>

#include "Defs.h"

// data conversion functions
WXDLLIMPEXP_SF wxPoint Conv2Point(const wxRealPoint& pt);
WXDLLIMPEXP_SF wxSize Conv2Size(const wxRealPoint& pt);
WXDLLIMPEXP_SF wxRealPoint Conv2RealPoint(const wxPoint& pt);


// graphics functions
WXDLLIMPEXP_SF wxColour GetHybridColour(const wxColour& orig, const wxColour& modificator);
WXDLLIMPEXP_SF bool LinesIntersection(const wxRealPoint& from1, const wxRealPoint& to1, const wxRealPoint& from2, const wxRealPoint& to2, wxRealPoint& i);
WXDLLIMPEXP_SF double Distance(const wxRealPoint& pt1, const wxRealPoint& pt2);

#endif //_WXSFCOMMONFCN_H
