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

#include "CommonFcn.h"

//----------------------------------------------------------------------------------//
// conversion functions
//----------------------------------------------------------------------------------//

wxColour StringToColour(const wxString& val)
{
	long nRed = 0;
	long nGreen = 0;
	long nBlue = 0;

	if(!val.IsEmpty())
	{
		wxStringTokenizer tokens(val, wxT(","), wxTOKEN_STRTOK);

		tokens.GetNextToken().ToLong(&nRed);
		tokens.GetNextToken().ToLong(&nGreen);
		tokens.GetNextToken().ToLong(&nBlue);
	}

	return wxColour(nRed, nGreen, nBlue);
}

wxRealPoint StringToRealPoint(const wxString& val)
{
	wxRealPoint pt;

	if(!val.IsEmpty())
	{
		wxStringTokenizer tokens(val, wxT(","), wxTOKEN_STRTOK);

		tokens.GetNextToken().ToDouble(&pt.x);
		tokens.GetNextToken().ToDouble(&pt.y);
	}

	return pt;
}

wxPoint StringToPoint(const wxString& val)
{
	wxPoint pt;
	long x, y;

	if(!val.IsEmpty())
	{
		wxStringTokenizer tokens(val, wxT(","), wxTOKEN_STRTOK);

		tokens.GetNextToken().ToLong(&x);
		tokens.GetNextToken().ToLong(&y);
		pt.x = x;
		pt.y = y;
	}

	return pt;
}

wxSize StringToSize(const wxString& val)
{
	wxPoint pt = StringToPoint(val);
	return wxSize(pt.x, pt.y);
}

long StringToLong(const wxString& val)
{
	long num;

	if(!val.IsEmpty())
	{
		val.ToLong(&num);
	}

	return num;
}

double StringToDouble(const wxString& val)
{
	double num;

	if(!val.IsEmpty())
	{
		val.ToDouble(&num);
	}

	return num;
}

bool StringToBool(const wxString& val)
{
	long num = 0;

	if(!val.IsEmpty())
	{
		val.ToLong(&num);
	}

	return (num == 1);
}

wxBrush StringToBrush(const wxString& val)
{
	wxBrush brush;

	wxStringTokenizer tokens(val, wxT(" "), wxTOKEN_STRTOK);
	brush.SetColour(StringToColour(tokens.GetNextToken()));
	brush.SetStyle(StringToLong(tokens.GetNextToken()));

	return brush;
}

wxPen StringToPen(const wxString& val)
{
	wxPen pen;

	wxStringTokenizer tokens(val, wxT(" "), wxTOKEN_STRTOK);
	pen.SetColour(StringToColour(tokens.GetNextToken()));
	pen.SetWidth(StringToLong(tokens.GetNextToken()));
	pen.SetStyle(StringToLong(tokens.GetNextToken()));

	return pen;
}

wxFont StringToFont(const wxString& val)
{
	wxFont font;

	font.SetNativeFontInfoUserDesc(val);

	return font;
}

wxString ColourToString(const wxColour& val)
{
	return wxString::Format(wxT("%d,%d,%d"), val.Red(), val.Green(), val.Blue());
}

wxString RealPointToString(const wxRealPoint& val)
{
	return wxString::Format(wxT("%lf,%lf"), val.x, val.y);
}

wxString PointToString(const wxPoint& val)
{
	return wxString::Format(wxT("%d,%d"), val.x, val.y);
}

wxString SizeToString(const wxSize& val)
{
	return wxString::Format(wxT("%d,%d"), val.x, val.y);
}

wxString LongToString(long val)
{
	return wxString::Format(wxT("%ld"), val);
}
wxString DoubleToString(double val)
{
	return wxString::Format(wxT("%lf"), val);
}
wxString BoolToString(bool val)
{
	return wxString::Format(wxT("%d"), val);
}

wxString BrushToString(const wxBrush& val)
{
	return wxString::Format(wxT("%s %d"), ColourToString(val.GetColour()).c_str(), val.GetStyle());
}

wxString PenToString(const wxPen& val)
{
	return wxString::Format(wxT("%s %d %d"), ColourToString(val.GetColour()).c_str(), val.GetWidth(), val.GetStyle());
}

wxString FontToString(const wxFont& val)
{
	return val.GetNativeFontInfoUserDesc();
}

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

wxXmlNode* AddPropertyNode(wxXmlNode* parent, const wxString& name, const wxString& value)
{
	if(parent)
	{
		wxXmlNode* child = new wxXmlNode(wxXML_ELEMENT_NODE, name);
		child->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxT(""), value));
		parent->AddChild(child);
		return child;
	}
	return NULL;
}
