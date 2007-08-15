#include "XmlSerializer.h"

#include <wx/tokenzr.h>

wxString xsSerializable::ConvertToString(xsProperty* property)
{
    wxASSERT(property);

    if(property->m_sDataType == wxT("string"))
    {
        return *((wxString*)property->m_pSourceVariable);
    }
    else if(property->m_sDataType == wxT("long"))
    {
        return LongToString(*((long*)property->m_pSourceVariable));
    }
    else if(property->m_sDataType == wxT("bool"))
    {
        return BoolToString(*((bool*)property->m_pSourceVariable));
    }
    else if(property->m_sDataType == wxT("double"))
    {
        return DoubleToString(*((double*)property->m_pSourceVariable));
    }
    else if(property->m_sDataType == wxT("point"))
    {
        return PointToString(*((wxPoint*)property->m_pSourceVariable));
    }
    else if(property->m_sDataType == wxT("size"))
    {
        return SizeToString(*((wxSize*)property->m_pSourceVariable));
    }
    else if(property->m_sDataType == wxT("realpoint"))
    {
        return RealPointToString(*((wxRealPoint*)property->m_pSourceVariable));
    }
    else if(property->m_sDataType == wxT("colour"))
    {
        return ColourToString(*((wxColour*)property->m_pSourceVariable));
    }
    else if(property->m_sDataType == wxT("brush"))
    {
        return BrushToString(*((wxBrush*)property->m_pSourceVariable));
    }
    else if(property->m_sDataType == wxT("pen"))
    {
        return PenToString(*((wxPen*)property->m_pSourceVariable));
    }
    else if(property->m_sDataType == wxT("font"))
    {
        return FontToString(*((wxFont*)property->m_pSourceVariable));
    }
    else
        return wxT("!!! UNSUPPORTED DATA TYPE !!!");
}

void xsSerializable::FillFromString(const wxString& source, xsProperty* property)
{
    wxASSERT(property);

    if(property->m_sDataType == wxT("string"))
    {
        *((wxString*)property->m_pSourceVariable) = source;
    }
    else if(property->m_sDataType == wxT("long"))
    {
        *((long*)property->m_pSourceVariable) = StringToLong(source);
    }
    else if(property->m_sDataType == wxT("bool"))
    {
        *((bool*)property->m_pSourceVariable) = StringToBool(source);
    }
    else if(property->m_sDataType == wxT("double"))
    {
        *((double*)property->m_pSourceVariable) = StringToDouble(source);
    }
    else if(property->m_sDataType == wxT("point"))
    {
        *((wxPoint*)property->m_pSourceVariable) = StringToPoint(source);
    }
    else if(property->m_sDataType == wxT("size"))
    {
        *((wxSize*)property->m_pSourceVariable) = StringToSize(source);
    }
    else if(property->m_sDataType == wxT("realpoint"))
    {
        *((wxRealPoint*)property->m_pSourceVariable) = StringToRealPoint(source);
    }
    else if(property->m_sDataType == wxT("colour"))
    {
        *((wxColour*)property->m_pSourceVariable) = StringToColour(source);
    }
    else if(property->m_sDataType == wxT("pen"))
    {
        *((wxPen*)property->m_pSourceVariable) = StringToPen(source);
    }
    else if(property->m_sDataType == wxT("brush"))
    {
        *((wxBrush*)property->m_pSourceVariable) = StringToBrush(source);
    }
    else if(property->m_sDataType == wxT("font"))
    {
        *((wxFont*)property->m_pSourceVariable) = StringToFont(source);
    }
}

wxColour xsSerializable::StringToColour(const wxString& val)
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

wxRealPoint xsSerializable::StringToRealPoint(const wxString& val)
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

wxPoint xsSerializable::StringToPoint(const wxString& val)
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

wxSize xsSerializable::StringToSize(const wxString& val)
{
	wxPoint pt = StringToPoint(val);
	return wxSize(pt.x, pt.y);
}

wxBrush xsSerializable::StringToBrush(const wxString& val)
{
	wxBrush brush;

	wxStringTokenizer tokens(val, wxT(" "), wxTOKEN_STRTOK);
	brush.SetColour(StringToColour(tokens.GetNextToken()));
	brush.SetStyle(StringToLong(tokens.GetNextToken()));

	return brush;
}

wxPen xsSerializable::StringToPen(const wxString& val)
{
	wxPen pen;

	wxStringTokenizer tokens(val, wxT(" "), wxTOKEN_STRTOK);
	pen.SetColour(StringToColour(tokens.GetNextToken()));
	pen.SetWidth(StringToLong(tokens.GetNextToken()));
	pen.SetStyle(StringToLong(tokens.GetNextToken()));

	return pen;
}

wxFont xsSerializable::StringToFont(const wxString& val)
{
	wxFont font;

	font.SetNativeFontInfoUserDesc(val);

	return font;
}

long xsSerializable::StringToLong(const wxString& val)
{
	long num;

	if(!val.IsEmpty())
	{
		val.ToLong(&num);
	}

	return num;
}

double xsSerializable::StringToDouble(const wxString& val)
{
	double num;

	if(!val.IsEmpty())
	{
		val.ToDouble(&num);
	}

	return num;
}

bool xsSerializable::StringToBool(const wxString& val)
{
	long num = 0;

	if(!val.IsEmpty())
	{
		val.ToLong(&num);
	}

	return (num == 1);
}

wxString xsSerializable::ColourToString(const wxColour& val)
{
	return wxString::Format(wxT("%d,%d,%d"), val.Red(), val.Green(), val.Blue());
}

wxString xsSerializable::RealPointToString(const wxRealPoint& val)
{
	return wxString::Format(wxT("%lf,%lf"), val.x, val.y);
}

wxString xsSerializable::PointToString(const wxPoint& val)
{
	return wxString::Format(wxT("%d,%d"), val.x, val.y);
}

wxString xsSerializable::SizeToString(const wxSize& val)
{
	return wxString::Format(wxT("%d,%d"), val.x, val.y);
}

wxString xsSerializable::BrushToString(const wxBrush& val)
{
	return wxString::Format(wxT("%s %d"), ColourToString(val.GetColour()).c_str(), val.GetStyle());
}

wxString xsSerializable::PenToString(const wxPen& val)
{
	return wxString::Format(wxT("%s %d %d"), ColourToString(val.GetColour()).c_str(), val.GetWidth(), val.GetStyle());
}

wxString xsSerializable::FontToString(const wxFont& val)
{
	return val.GetNativeFontInfoUserDesc();
}

wxString xsSerializable::LongToString(long val)
{
	return wxString::Format(wxT("%ld"), val);
}

wxString xsSerializable::DoubleToString(double val)
{
	return wxString::Format(wxT("%lf"), val);
}

wxString xsSerializable::BoolToString(bool val)
{
	return wxString::Format(wxT("%d"), val);
}
