/***************************************************************
 * Name:      PropertyIO.cpp
 * Purpose:   Implements data types I/O and conversion functions
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-10-28
 * Copyright: Michal Bliňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "wx_pch.h"

#ifdef _DEBUG_MSVC
#define new DEBUG_NEW
#endif

#include "wx/wxxmlserializer/PropertyIO.h"
#include "wx/wxxmlserializer/XmlSerializer.h"

#include <wx/listimpl.cpp>
#include <wx/arrimpl.cpp>
#include <limits>

using namespace std;

WX_DEFINE_EXPORTED_OBJARRAY(RealPointArray);
WX_DEFINE_EXPORTED_LIST(RealPointList);

/////////////////////////////////////////////////////////////////////////////////////
// xsPropertyIO class ///////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC_CLASS(xsPropertyIO, wxObject);

wxXmlNode* xsPropertyIO::AddPropertyNode(wxXmlNode* parent, const wxString& name, const wxString& value, wxXmlNodeType type)
{
	if(parent)
	{
		wxXmlNode* child = new wxXmlNode(wxXML_ELEMENT_NODE, name);
		child->AddChild(new wxXmlNode(type, wxT(""), value));
		parent->AddChild(child);
		return child;
	}
	return NULL;
}

void xsPropertyIO::AppendPropertyType(xsProperty *source, wxXmlNode *target)
{
    target->AddProperty(wxT("name"), source->m_sFieldName);
    target->AddProperty(wxT("type"), source->m_sDataType);
}

/////////////////////////////////////////////////////////////////////////////////////
// xsStringPropIO class /////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

XS_DEFINE_IO_HANDLER(wxString, xsStringPropIO);

wxString xsStringPropIO::ToString(wxString value)
{
    return value;
}

wxString xsStringPropIO::FromString(const wxString& value)
{
	return value;
}

/////////////////////////////////////////////////////////////////////////////////////
// xsCharPropIO class ///////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

XS_DEFINE_IO_HANDLER(wxChar, xsCharPropIO);

wxString xsCharPropIO::ToString(wxChar value)
{
    return wxString::Format(wxT("%c"), value);
}

wxChar xsCharPropIO::FromString(const wxString& value)
{
	return value.GetChar(0);
}


/////////////////////////////////////////////////////////////////////////////////////
// xsLongPropIO class ///////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

XS_DEFINE_IO_HANDLER(long, xsLongPropIO);

wxString xsLongPropIO::ToString(long value)
{
    return wxString::Format(wxT("%ld"), value);
}

long xsLongPropIO::FromString(const wxString& value)
{
	long num = 0;
	if(!value.IsEmpty())
	{
		value.ToLong(&num);
	}
	return num;
}

/////////////////////////////////////////////////////////////////////////////////////
// xsIntPropIO class ////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

XS_DEFINE_IO_HANDLER(int, xsIntPropIO);

wxString xsIntPropIO::ToString(int value)
{
    return wxString::Format(wxT("%d"), value);
}

int xsIntPropIO::FromString(const wxString& value)
{
	long num = 0;
	if(!value.IsEmpty())
	{
		value.ToLong(&num);
	}
	return (int)num;
}

/////////////////////////////////////////////////////////////////////////////////////
// xsBoolPropIO class ///////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

XS_DEFINE_IO_HANDLER(bool, xsBoolPropIO);

wxString xsBoolPropIO::ToString(bool value)
{
    return wxString::Format(wxT("%d"), value);
}

bool xsBoolPropIO::FromString(const wxString& value)
{
	long num = 0;
	if(!value.IsEmpty())
	{
		value.ToLong(&num);
	}
	return (num == 1);
}

/////////////////////////////////////////////////////////////////////////////////////
// xsDoublePropIO class /////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

XS_DEFINE_IO_HANDLER(double, xsDoublePropIO);

wxString xsDoublePropIO::ToString(double value)
{
    wxString sVal;

    if( wxIsNaN(value) )
    {
        sVal = wxT("NAN");
    }
    else if( wxFinite(value) == 0 )
    {
        sVal = wxT("INF");
    }
    else
    {
        // use '.' decimal point character
        sVal= wxString::Format(wxT("%lf"), value);
        sVal.Replace(wxLocale::GetInfo(wxLOCALE_DECIMAL_POINT, wxLOCALE_CAT_NUMBER), wxT("."));
    }

    return sVal;
}

double xsDoublePropIO::FromString(const wxString& value)
{
	double num = 0;

	if(!value.IsEmpty())
	{
	    if( value == wxT("NAN") )
	    {
	        num = numeric_limits<double>::quiet_NaN();
	    }
	    else if( value == wxT("INF") )
	    {
	        num = numeric_limits<double>::infinity();
	    }
	    else
	    {
	        // decimal point character used in wxXS is strictly '.'...
#ifdef __WXMSW__
			value.ToDouble(&num);
#else
	        wxString sNum = value;
	        sNum.Replace(wxT("."), wxLocale::GetInfo(wxLOCALE_DECIMAL_POINT, wxLOCALE_CAT_NUMBER) );
	        sNum.ToDouble(&num);
#endif
	    }
	}

	return num;
}

/////////////////////////////////////////////////////////////////////////////////////
// xsFloatPropIO class //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

XS_DEFINE_IO_HANDLER(float, xsFloatPropIO);

wxString xsFloatPropIO::ToString(float value)
{
    wxString sVal;
    if( wxIsNaN(value) )
    {
        sVal = wxT("NAN");
    }
    else if( wxFinite(value) == 0 )
    {
        sVal = wxT("INF");
    }
    else
    {
        sVal = wxString::Format(wxT("%f"), value);
        sVal.Replace(wxLocale::GetInfo(wxLOCALE_DECIMAL_POINT, wxLOCALE_CAT_NUMBER), wxT("."));
    }

    return sVal;
}

float xsFloatPropIO::FromString(const wxString& value)
{
	double num = 0;

	if(!value.IsEmpty())
	{
	    if( value == wxT("NAN") )
	    {
	        num = numeric_limits<float>::quiet_NaN();
	    }
	    else if( value == wxT("INF") )
	    {
	        num = numeric_limits<float>::infinity();
	    }
	    else
	    {
	        // decimal point character used in wxXS is strictly '.'...
#ifdef __WXMSW__
			value.ToDouble(&num);
#else
	        wxString sNum = value;
	        sNum.Replace(wxT("."), wxLocale::GetInfo(wxLOCALE_DECIMAL_POINT, wxLOCALE_CAT_NUMBER) );
	        sNum.ToDouble(&num);
#endif
	    }
	}

	return (float)num;
}

/////////////////////////////////////////////////////////////////////////////////////
// xsPointPropIO class //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

XS_DEFINE_IO_HANDLER(wxPoint, xsPointPropIO);

wxString xsPointPropIO::ToString(wxPoint value)
{
    return wxString::Format(wxT("%d,%d"), value.x, value.y);
}

wxPoint xsPointPropIO::FromString(const wxString& value)
{
	wxPoint pt;
	long x, y;

	if(!value.IsEmpty())
	{
		wxStringTokenizer tokens(value, wxT(","), wxTOKEN_STRTOK);

		tokens.GetNextToken().ToLong(&x);
		tokens.GetNextToken().ToLong(&y);
		pt.x = x;
		pt.y = y;
	}

	return pt;
}

/////////////////////////////////////////////////////////////////////////////////////
// xsSizePropIO class ///////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

XS_DEFINE_IO_HANDLER(wxSize, xsSizePropIO);

wxString xsSizePropIO::ToString(wxSize value)
{
    return wxString::Format(wxT("%d,%d"), value.x, value.y);
}

wxSize xsSizePropIO::FromString(const wxString& value)
{
	wxPoint pt = xsPointPropIO::FromString(value);
	return wxSize(pt.x, pt.y);
}

/////////////////////////////////////////////////////////////////////////////////////
// xsRealPointPropIO class //////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

XS_DEFINE_IO_HANDLER(wxRealPoint, xsRealPointPropIO);

wxString xsRealPointPropIO::ToString(wxRealPoint value)
{
    return wxString::Format(wxT("%s,%s"), xsDoublePropIO::ToString(value.x).c_str(), xsDoublePropIO::ToString(value.y).c_str());
}

wxRealPoint xsRealPointPropIO::FromString(const wxString& value)
{
	wxRealPoint pt;

	if(!value.IsEmpty())
	{
		wxStringTokenizer tokens(value, wxT(","), wxTOKEN_STRTOK);

		pt.x = xsDoublePropIO::FromString(tokens.GetNextToken());
		pt.y = xsDoublePropIO::FromString(tokens.GetNextToken());
	}

	return pt;
}

/////////////////////////////////////////////////////////////////////////////////////
// xsColourPropIO class /////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

XS_DEFINE_IO_HANDLER(wxColour, xsColourPropIO);

wxString xsColourPropIO::ToString(wxColour value)
{
    return wxString::Format(wxT("%d,%d,%d,%d"), value.Red(), value.Green(), value.Blue(), value.Alpha());
}

wxColour xsColourPropIO::FromString(const wxString& value)
{
	long nRed = 0;
	long nGreen = 0;
	long nBlue = 0;
	long nAlpha = 0;

	if(!value.IsEmpty())
	{
		wxStringTokenizer tokens(value, wxT(","), wxTOKEN_STRTOK);

		tokens.GetNextToken().ToLong(&nRed);
		tokens.GetNextToken().ToLong(&nGreen);
		tokens.GetNextToken().ToLong(&nBlue);

		if( tokens.HasMoreTokens() )
            tokens.GetNextToken().ToLong(&nAlpha);
        else
            nAlpha = 255;
	}

	return wxColour(nRed, nGreen, nBlue, nAlpha);
}

/////////////////////////////////////////////////////////////////////////////////////
// xsPenPropIO class ////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

XS_DEFINE_IO_HANDLER(wxPen, xsPenPropIO);

wxString xsPenPropIO::ToString(wxPen value)
{
    return wxString::Format(wxT("%s %d %d"), xsColourPropIO::ToString(value.GetColour()).c_str(), value.GetWidth(), value.GetStyle());
}

wxPen xsPenPropIO::FromString(const wxString& value)
{
	wxPen pen;

	wxStringTokenizer tokens(value, wxT(" "), wxTOKEN_STRTOK);
	pen.SetColour(xsColourPropIO::FromString(tokens.GetNextToken()));
	pen.SetWidth(xsLongPropIO::FromString(tokens.GetNextToken()));
	pen.SetStyle(xsLongPropIO::FromString(tokens.GetNextToken()));

	return pen;
}

/////////////////////////////////////////////////////////////////////////////////////
// xsBrushPropIO class //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

XS_DEFINE_IO_HANDLER(wxBrush, xsBrushPropIO);

wxString xsBrushPropIO::ToString(wxBrush value)
{
    return wxString::Format(wxT("%s %d"), xsColourPropIO::ToString(value.GetColour()).c_str(), value.GetStyle());
}

wxBrush xsBrushPropIO::FromString(const wxString& value)
{
	wxBrush brush;

	wxStringTokenizer tokens(value, wxT(" "), wxTOKEN_STRTOK);
	brush.SetColour(xsColourPropIO::FromString(tokens.GetNextToken()));
	brush.SetStyle(xsLongPropIO::FromString(tokens.GetNextToken()));

	return brush;
}

/////////////////////////////////////////////////////////////////////////////////////
// xsFontPropIO class ///////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

XS_DEFINE_IO_HANDLER(wxFont, xsFontPropIO);

wxString xsFontPropIO::ToString(wxFont value)
{
    return value.GetNativeFontInfoUserDesc();
}

wxFont xsFontPropIO::FromString(const wxString& value)
{
	wxFont font;

	font.SetNativeFontInfoUserDesc(value);

	return font;
}

/////////////////////////////////////////////////////////////////////////////////////
// xsArrayStringPropIO class ////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC_CLASS(xsArrayStringPropIO, xsPropertyIO);

void xsArrayStringPropIO::Read(xsProperty *property, wxXmlNode *source)
{
    ((wxArrayString*)property->m_pSourceVariable)->Clear();

    wxXmlNode *listNode = source->GetChildren();
    while(listNode)
    {
        if(listNode->GetName() == wxT("item"))
        {
            ((wxArrayString*)property->m_pSourceVariable)->Add(listNode->GetNodeContent());
        }

        listNode = listNode->GetNext();
    }
}

void xsArrayStringPropIO::Write(xsProperty *property, wxXmlNode *target)
{
    wxArrayString array(*((wxArrayString*)property->m_pSourceVariable));

    size_t cnt = array.GetCount();
    if(cnt > 0)
    {
        wxXmlNode *newNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("property"));
        for(size_t i = 0; i < cnt; i++)
        {
            AddPropertyNode(newNode, wxT("item"), array[i]);
        }

        target->AddChild(newNode);
        AppendPropertyType(property, newNode);
    }
}

wxString xsArrayStringPropIO::GetValueStr(xsProperty *property)
{
	return ToString(*((wxArrayString*)property->m_pSourceVariable));
}

wxString xsArrayStringPropIO::ToString(wxArrayString value)
{
 	wxString out = wxT("[ ");

	for( size_t i = 0; i < value.GetCount(); i++)
	{
		out << value[i];
		if( i < value.GetCount()-1 ) out << wxT(" | ");
	}

	out << wxT(" ]");

	return out;
}

wxArrayString xsArrayStringPropIO::FromString(const wxString& WXUNUSED(value))
{
	return wxArrayString();
}

/////////////////////////////////////////////////////////////////////////////////////
// xsArrayIntPropIO class ////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC_CLASS(xsArrayIntPropIO, xsPropertyIO);

void xsArrayIntPropIO::Read(xsProperty *property, wxXmlNode *source)
{
    ((IntArray*)property->m_pSourceVariable)->Clear();

    wxXmlNode *listNode = source->GetChildren();
    while(listNode)
    {
        if(listNode->GetName() == wxT("item"))
        {
            ((IntArray*)property->m_pSourceVariable)->Add(xsIntPropIO::FromString(listNode->GetNodeContent()));
        }

        listNode = listNode->GetNext();
    }
}

void xsArrayIntPropIO::Write(xsProperty *property, wxXmlNode *target)
{
    IntArray array(*((IntArray*)property->m_pSourceVariable));

    size_t cnt = array.GetCount();
    if(cnt > 0)
    {
        wxXmlNode *newNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("property"));
        for(size_t i = 0; i < cnt; i++)
        {
            AddPropertyNode(newNode, wxT("item"), xsIntPropIO::ToString(array[i]));
        }

        target->AddChild(newNode);
        AppendPropertyType(property, newNode);
    }
}

wxString xsArrayIntPropIO::GetValueStr(xsProperty *property)
{
	return ToString(*((IntArray*)property->m_pSourceVariable));
}

wxString xsArrayIntPropIO::ToString(IntArray value)
{
 	wxString out = wxT("[ ");

	for( size_t i = 0; i < value.GetCount(); i++)
	{
		out << xsIntPropIO::ToString(value[i]);
		if( i < value.GetCount()-1 ) out << wxT(" | ");
	}

	out << wxT(" ]");

	return out;
}

IntArray xsArrayIntPropIO::FromString(const wxString& WXUNUSED(value))
{
	return IntArray();
}

/////////////////////////////////////////////////////////////////////////////////////
// xsArrayLongPropIO class ////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC_CLASS(xsArrayLongPropIO, xsPropertyIO);

void xsArrayLongPropIO::Read(xsProperty *property, wxXmlNode *source)
{
    ((LongArray*)property->m_pSourceVariable)->Clear();

    wxXmlNode *listNode = source->GetChildren();
    while(listNode)
    {
        if(listNode->GetName() == wxT("item"))
        {
            ((LongArray*)property->m_pSourceVariable)->Add(xsLongPropIO::FromString(listNode->GetNodeContent()));
        }

        listNode = listNode->GetNext();
    }
}

void xsArrayLongPropIO::Write(xsProperty *property, wxXmlNode *target)
{
    LongArray array(*((LongArray*)property->m_pSourceVariable));

    size_t cnt = array.GetCount();
    if(cnt > 0)
    {
        wxXmlNode *newNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("property"));
        for(size_t i = 0; i < cnt; i++)
        {
            AddPropertyNode(newNode, wxT("item"), xsLongPropIO::ToString(array[i]));
        }

        target->AddChild(newNode);
        AppendPropertyType(property, newNode);
    }
}

wxString xsArrayLongPropIO::GetValueStr(xsProperty *property)
{
	return ToString(*((LongArray*)property->m_pSourceVariable));
}

wxString xsArrayLongPropIO::ToString(LongArray value)
{
 	wxString out = wxT("[ ");

	for( size_t i = 0; i < value.GetCount(); i++)
	{
		out << xsLongPropIO::ToString(value[i]);
		if( i < value.GetCount()-1 ) out << wxT(" | ");
	}

	out << wxT(" ]");

	return out;
}

LongArray xsArrayLongPropIO::FromString(const wxString& WXUNUSED(value))
{
	return LongArray();
}

/////////////////////////////////////////////////////////////////////////////////////
// xsArrayDoublePropIO class ////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC_CLASS(xsArrayDoublePropIO, xsPropertyIO);

void xsArrayDoublePropIO::Read(xsProperty *property, wxXmlNode *source)
{
    ((DoubleArray*)property->m_pSourceVariable)->Clear();

    wxXmlNode *listNode = source->GetChildren();
    while(listNode)
    {
        if(listNode->GetName() == wxT("item"))
        {
            ((DoubleArray*)property->m_pSourceVariable)->Add(xsDoublePropIO::FromString(listNode->GetNodeContent()));
        }

        listNode = listNode->GetNext();
    }
}

void xsArrayDoublePropIO::Write(xsProperty *property, wxXmlNode *target)
{
    DoubleArray array(*((DoubleArray*)property->m_pSourceVariable));

    size_t cnt = array.GetCount();
    if(cnt > 0)
    {
        wxXmlNode *newNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("property"));
        for(size_t i = 0; i < cnt; i++)
        {
            AddPropertyNode(newNode, wxT("item"), xsDoublePropIO::ToString(array[i]));
        }

        target->AddChild(newNode);
        AppendPropertyType(property, newNode);
    }
}

wxString xsArrayDoublePropIO::GetValueStr(xsProperty *property)
{
	return ToString(*((DoubleArray*)property->m_pSourceVariable));
}

wxString xsArrayDoublePropIO::ToString(DoubleArray value)
{
 	wxString out = wxT("[ ");

	for( size_t i = 0; i < value.GetCount(); i++)
	{
		out << xsDoublePropIO::ToString(value[i]);
		if( i < value.GetCount()-1 ) out << wxT(" | ");
	}

	out << wxT(" ]");

	return out;
}

DoubleArray xsArrayDoublePropIO::FromString(const wxString& WXUNUSED(value))
{
	return DoubleArray();
}

/////////////////////////////////////////////////////////////////////////////////////
// xsArrayCharPropIO class ////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC_CLASS(xsArrayCharPropIO, xsPropertyIO);

void xsArrayCharPropIO::Read(xsProperty *property, wxXmlNode *source)
{
    ((CharArray*)property->m_pSourceVariable)->Clear();

    wxXmlNode *listNode = source->GetChildren();
    while(listNode)
    {
        if(listNode->GetName() == wxT("item"))
        {
            ((CharArray*)property->m_pSourceVariable)->Add(xsCharPropIO::FromString(listNode->GetNodeContent()));
        }

        listNode = listNode->GetNext();
    }
}

void xsArrayCharPropIO::Write(xsProperty *property, wxXmlNode *target)
{
    CharArray array(*((CharArray*)property->m_pSourceVariable));

    size_t cnt = array.GetCount();
    if(cnt > 0)
    {
        wxXmlNode *newNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("property"));
        for(size_t i = 0; i < cnt; i++)
        {
            AddPropertyNode(newNode, wxT("item"), xsCharPropIO::ToString(array[i]));
        }

        target->AddChild(newNode);
        AppendPropertyType(property, newNode);
    }
}

wxString xsArrayCharPropIO::GetValueStr(xsProperty *property)
{
	return ToString(*((CharArray*)property->m_pSourceVariable));
}

wxString xsArrayCharPropIO::ToString(CharArray value)
{
 	wxString out = wxT("[ ");

	for( size_t i = 0; i < value.GetCount(); i++)
	{
		out << xsCharPropIO::ToString(value[i]);
		if( i < value.GetCount()-1 ) out << wxT(" | ");
	}

	out << wxT(" ]");

	return out;
}

CharArray xsArrayCharPropIO::FromString(const wxString& WXUNUSED(value))
{
	return CharArray();
}



/////////////////////////////////////////////////////////////////////////////////////
// xsArrayRealPointPropIO class /////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC_CLASS(xsArrayRealPointPropIO, xsPropertyIO);

void xsArrayRealPointPropIO::Read(xsProperty *property, wxXmlNode *source)
{
    ((RealPointArray*)property->m_pSourceVariable)->Clear();

    wxXmlNode *listNode = source->GetChildren();
    while(listNode)
    {
        if(listNode->GetName() == wxT("item"))
        {
            ((RealPointArray*)property->m_pSourceVariable)->Add(xsRealPointPropIO::FromString(listNode->GetNodeContent()));
        }

        listNode = listNode->GetNext();
    }
}

void xsArrayRealPointPropIO::Write(xsProperty *property, wxXmlNode *target)
{
    RealPointArray array(*((RealPointArray*)property->m_pSourceVariable));

    size_t cnt = array.GetCount();
    if(cnt > 0)
    {
        wxXmlNode *newNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("property"));
        for(size_t i = 0; i < cnt; i++)
        {
			AddPropertyNode(newNode, wxT("item"), xsRealPointPropIO::ToString(array[i]));
        }

        target->AddChild(newNode);
        AppendPropertyType(property, newNode);
    }
}

wxString xsArrayRealPointPropIO::GetValueStr(xsProperty *property)
{
	return ToString(*((RealPointArray*)property->m_pSourceVariable));
}

wxString xsArrayRealPointPropIO::ToString(RealPointArray value)
{
	wxString out = wxT("[ ");

	for( size_t i = 0; i < value.GetCount(); i++)
	{
		out << xsRealPointPropIO::ToString(value[i]);
		if( i < value.GetCount()-1 ) out << wxT(" | ");
	}

	out << wxT(" ]");

	return out;
}

RealPointArray xsArrayRealPointPropIO::FromString(const wxString& WXUNUSED(value))
{
	return RealPointArray();
}

/////////////////////////////////////////////////////////////////////////////////////
// xsListRealPointPropIO class //////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC_CLASS(xsListRealPointPropIO, xsPropertyIO);

void xsListRealPointPropIO::Read(xsProperty *property, wxXmlNode *source)
{
	RealPointList *list = (RealPointList*)property->m_pSourceVariable;

	// clear previous list content
	bool fDelState = list->GetDeleteContents();

	list->DeleteContents(true);
    list->Clear();
	list->DeleteContents(fDelState);

    wxXmlNode *listNode = source->GetChildren();
    while(listNode)
    {
        if(listNode->GetName() == wxT("item"))
        {
            list->Append(new wxRealPoint(xsRealPointPropIO::FromString(listNode->GetNodeContent())));
        }

        listNode = listNode->GetNext();
    }
}

void xsListRealPointPropIO::Write(xsProperty *property, wxXmlNode *target)
{
    RealPointList *list = (RealPointList*)property->m_pSourceVariable;

    if( !list->IsEmpty() )
    {
        wxXmlNode *newNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("property"));
		RealPointList::compatibility_iterator listNode = list->GetFirst();
        while(listNode)
        {
            AddPropertyNode(newNode, wxT("item"), xsRealPointPropIO::ToString(*(wxRealPoint*)listNode->GetData()));
            listNode = listNode->GetNext();
        }

        target->AddChild(newNode);
        AppendPropertyType(property, newNode);
    }
}

wxString xsListRealPointPropIO::GetValueStr(xsProperty *property)
{
	return ToString(*((RealPointList*)property->m_pSourceVariable));
}

wxString xsListRealPointPropIO::ToString(RealPointList value)
{
	wxString out = wxT("[ ");

	RealPointList::compatibility_iterator node = value.GetFirst();
	while( node )
	{
		out << xsRealPointPropIO::ToString(*(wxRealPoint*)node->GetData());
		if( node != value.GetLast() ) out << wxT(" | ");

		node = node->GetNext();
	}

	out << wxT(" ]");

	return out;
}

RealPointList xsListRealPointPropIO::FromString(const wxString& WXUNUSED(value))
{
	return RealPointList();
}

/////////////////////////////////////////////////////////////////////////////////////
// xsDynObjPropIO class /////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC_CLASS(xsDynObjPropIO, xsPropertyIO);

void xsDynObjPropIO::Read(xsProperty *property, wxXmlNode *source)
{
    wxXmlNode *objectNode = source->GetChildren();

    if( objectNode && (objectNode->GetName() == wxT("object")) )
    {
        *(xsSerializable**)(property->m_pSourceVariable) = (xsSerializable*)wxCreateDynamicObject(objectNode->GetPropVal(wxT("type"), wxT("")));

        xsSerializable* object = *(xsSerializable**)(property->m_pSourceVariable);
        if(object)
        {
            object->DeserializeObject(objectNode);
        }
    }
}

void xsDynObjPropIO::Write(xsProperty *property, wxXmlNode *target)
{
    xsSerializable* object = *(xsSerializable**)(property->m_pSourceVariable);

    if( object && object->IsKindOf(CLASSINFO(xsSerializable)))
    {
        wxXmlNode *newNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("property"));
        newNode->AddChild(object->SerializeObject(NULL));

        target->AddChild(newNode);
        AppendPropertyType(property, newNode);
    }
}

wxString xsDynObjPropIO::GetValueStr(xsProperty *property)
{
    return ToString(*(xsSerializable**)(property->m_pSourceVariable));
}

wxString xsDynObjPropIO::ToString(xsSerializable* value)
{
	return wxString::Format(wxT("Dynamic object at address 0x%x"), value);
}

xsSerializable* xsDynObjPropIO::FromString(const wxString& WXUNUSED(value))
{
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////////////
// xsDynNCObjPropIO class ///////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC_CLASS(xsDynNCObjPropIO, xsPropertyIO);

void xsDynNCObjPropIO::Read(xsProperty *property, wxXmlNode *source)
{
    wxXmlNode *objectNode = source->GetChildren();

    if( objectNode && (objectNode->GetName() == wxT("object")) )
    {
        xsSerializable* object = *(xsSerializable**)(property->m_pSourceVariable);
        if(object)
        {
            object->DeserializeObject(objectNode);
        }
    }
}

void xsDynNCObjPropIO::Write(xsProperty *property, wxXmlNode *target)
{
    xsSerializable* object = *(xsSerializable**)(property->m_pSourceVariable);

    if( object && object->IsKindOf(CLASSINFO(xsSerializable)))
    {
        wxXmlNode *newNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("property"));
        newNode->AddChild(object->SerializeObject(NULL));

        target->AddChild(newNode);
        AppendPropertyType(property, newNode);
    }
}

wxString xsDynNCObjPropIO::GetValueStr(xsProperty *property)
{
    return ToString(*(xsSerializable**)(property->m_pSourceVariable));
}

wxString xsDynNCObjPropIO::ToString(xsSerializable* value)
{
	return wxString::Format(wxT("Dynamic object at address 0x%x"), value);
}

xsSerializable* xsDynNCObjPropIO::FromString(const wxString& WXUNUSED(value))
{
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////////////
// xsStaticObjPropIO class //////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC_CLASS(xsStaticObjPropIO, xsPropertyIO);

void xsStaticObjPropIO::Read(xsProperty *property, wxXmlNode *source)
{
    wxXmlNode *objectNode = source->GetChildren();

    if( objectNode && (objectNode->GetName() == wxT("object")) )
    {
        (*((xsSerializable*)property->m_pSourceVariable)).DeserializeObject(objectNode);
    }
}

void xsStaticObjPropIO::Write(xsProperty *property, wxXmlNode *target)
{
    wxXmlNode *newNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("property"));
    newNode->AddChild((*((xsSerializable*)property->m_pSourceVariable)).SerializeObject(NULL));

    target->AddChild(newNode);
    AppendPropertyType(property, newNode);
}

wxString xsStaticObjPropIO::GetValueStr(xsProperty *property)
{
	return ToString(*(xsSerializable*)property->m_pSourceVariable);
}

wxString xsStaticObjPropIO::ToString(xsSerializable value)
{
	return wxString::Format(wxT("Static object at address 0x%x"), &value);
}

xsSerializable xsStaticObjPropIO::FromString(const wxString& WXUNUSED(value))
{
	xsSerializable dummy;
	return dummy;
}



