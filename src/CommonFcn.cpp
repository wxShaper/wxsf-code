/***************************************************************
 * Name:      CommonFcn.cpp
 * Purpose:   Implements set of global (auxiliary) functions
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include <wx/tokenzr.h>

#include "wx/wxsf/CommonFcn.h"

//----------------------------------------------------------------------------------//
// conversion functions
//----------------------------------------------------------------------------------//

//----------------------------------------------------------------------------------//
// graphical functions
//----------------------------------------------------------------------------------//

wxColour GetHybridColour(const wxColour &orig, const wxColour &modificator)
{
	int r, g, b;

	r = orig.Red() - (255 - modificator.Red())/20;
	g = orig.Green() - (255 - modificator.Green())/20;
	b = orig.Blue() - (255 - modificator.Blue())/20;

	if(r < 0) r = 0;
	if(g < 0) g = 0;
	if(b < 0) b = 0;

	return wxColour(r, g, b);
}

bool LinesIntersection(const wxRealPoint& from1, const wxRealPoint& to1, const wxRealPoint& from2, const wxRealPoint& to2, wxRealPoint& i)
{
    double a1, b1, c1, a2, b2, c2, ka, kb;

    // create line 1 info
    a1 = to1.y - from1.y;
    b1 = from1.x - to1.x;
    c1 = -a1*from1.x - b1*from1.y;

    // create line 2 info
    a2 = to2.y - from2.y;
    b2 = from2.x - to2.x;
    c2 = -a2*from2.x - b2*from2.y;

    // check, whether the lines are parallel...
    ka = a1 / a2;
    kb = b1 / b2;

    if(ka == kb)return false;

    // find intersection point
    i.x = (b1*c2 - c1 * b2) / (a1*b2 - a2*b1);
    i.y = -(a1*c2 - a2 * c1) / (a1*b2 - a2*b1);

    if(((from1.x - i.x)*(i.x - to1.x) >= 0) && ((from2.x - i.x)*(i.x - to2.x) >=0)
        && ((from1.y - i.y)*(i.y - to1.y) >= 0) && ((from2.y - i.y)*(i.y - to2.y) >= 0))
        return true;
    else
        return false;
}

double Distance(const wxRealPoint& pt1, const wxRealPoint& pt2)
{
    return sqrt((pt2.x - pt1.x)*(pt2.x - pt1.x) + (pt2.y - pt1.y)*(pt2.y - pt1.y));
}

//----------------------------------------------------------------------------------//
// xml functions
//----------------------------------------------------------------------------------//

