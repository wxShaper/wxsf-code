/***************************************************************
 * Name:      CommonFcn.h
 * Purpose:   Defines set of global (auxiliary) functions
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#pragma once

#include <wx/wxprec.h>
#include <wx/xml/xml.h>

// data conversion functions

/// <summary> Create wxColour value from its string representation. </summary>
/// <param name="val"> String containing the value representation </param>
/// <returns> Converted wxColour value </returns>
wxColour StringToColour(const wxString& val);
wxRealPoint StringToRealPoint(const wxString& val);
wxPoint StringToPoint(const wxString& val);
wxSize StringToSize(const wxString& val);
long StringToLong(const wxString& val);
double StringToDouble(const wxString& val);
bool StringToBool(const wxString& val);
wxBrush StringToBrush(const wxString& val);
wxPen StringToPen(const wxString& val);
wxFont StringToFont(const wxString& val);

wxString ColourToString(const wxColour& val);
wxString RealPointToString(const wxRealPoint& val);
wxString PointToString(const wxPoint& val);
wxString SizeToString(const wxSize& val);
wxString LongToString(long val);
wxString DoubleToString(double val);
wxString BoolToString(bool val);
wxString BrushToString(const wxBrush& val);
wxString PenToString(const wxPen& val);
wxString FontToString(const wxFont& val);

// XML related functions
wxXmlNode* AddPropertyNode(wxXmlNode* parent, const wxString& name, const wxString& value);

// graphics functions
wxColour GetHybridColour(const wxColour& orig, const wxColour& modificator);
bool LinesIntersection(const wxRealPoint& from1, const wxRealPoint& to1, const wxRealPoint& from2, const wxRealPoint& to2, wxRealPoint& i);
double Distance(const wxRealPoint& pt1, const wxRealPoint& pt2);
