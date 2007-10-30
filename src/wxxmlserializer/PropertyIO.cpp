/***************************************************************
 * Name:      PropertyIO.cpp
 * Purpose:   Implements data types I/O and conversion functions
 * Author:    Michal Bližòák (michal.bliznak@tiscali.cz)
 * Created:   2007-10-28
 * Copyright: Michal Bližòák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "wx/wxxmlserializer/PropertyIO.h"
#include "wx/wxxmlserializer/XmlSerializer.h"

#include <wx/tokenzr.h>

/////////////////////////////////////////////////////////////////////////////////////
// xsPropertyIO class ///////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC_CLASS(xsPropertyIO, wxObject);

wxXmlNode* xsPropertyIO::AddPropertyNode(wxXmlNode* parent, const wxString& name, const wxString& value)
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

void xsPropertyIO::AppendPropertyType(xsProperty *source, wxXmlNode *target)
{
    target->AddProperty(wxT("name"), source->m_sFieldName);
    target->AddProperty(wxT("type"), source->m_sDataType);
}

/////////////////////////////////////////////////////////////////////////////////////
// xsStringPropIO class /////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC_CLASS(xsStringPropIO, xsPropertyIO);

void xsStringPropIO::Read(xsProperty *property, wxXmlNode *source)
{
    *((wxString*)property->m_pSourceVariable) = source->GetNodeContent();
}

void xsStringPropIO::Write(xsProperty *property, wxXmlNode *target)
{
    wxString val = *((wxString*)property->m_pSourceVariable);

    if(val != property->m_sDefaultValueStr)
    {
        wxXmlNode *newNode = AddPropertyNode(target, wxT("property"), val);
        AppendPropertyType(property, newNode);
    }
}

/////////////////////////////////////////////////////////////////////////////////////
// xsLongPropIO class ///////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC_CLASS(xsLongPropIO, xsPropertyIO);

void xsLongPropIO::Read(xsProperty *property, wxXmlNode *source)
{
    *((long*)property->m_pSourceVariable) = FromString(source->GetNodeContent());
}

void xsLongPropIO::Write(xsProperty *property, wxXmlNode *target)
{
    wxString val = ToString(*((long*)property->m_pSourceVariable));

    if(val != property->m_sDefaultValueStr)
    {
        wxXmlNode *newNode = AddPropertyNode(target, wxT("property"), val);
        AppendPropertyType(property, newNode);
    }
}

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
// xsBoolPropIO class ///////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC_CLASS(xsBoolPropIO, xsPropertyIO);

void xsBoolPropIO::Read(xsProperty *property, wxXmlNode *source)
{
    *((bool*)property->m_pSourceVariable) = FromString(source->GetNodeContent());
}

void xsBoolPropIO::Write(xsProperty *property, wxXmlNode *target)
{
    wxString val = ToString(*((bool*)property->m_pSourceVariable));

    if(val != property->m_sDefaultValueStr)
    {
        wxXmlNode *newNode = AddPropertyNode(target, wxT("property"), val);
        AppendPropertyType(property, newNode);
    }
}

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

IMPLEMENT_DYNAMIC_CLASS(xsDoublePropIO, xsPropertyIO);

void xsDoublePropIO::Read(xsProperty *property, wxXmlNode *source)
{
    *((double*)property->m_pSourceVariable) = FromString(source->GetNodeContent());
}

void xsDoublePropIO::Write(xsProperty *property, wxXmlNode *target)
{
    wxString val = ToString(*((double*)property->m_pSourceVariable));

    if(val != property->m_sDefaultValueStr)
    {
        wxXmlNode *newNode = AddPropertyNode(target, wxT("property"), val);
        AppendPropertyType(property, newNode);
    }
}

wxString xsDoublePropIO::ToString(double value)
{
    return wxString::Format(wxT("%lf"), value);
}

double xsDoublePropIO::FromString(const wxString& value)
{
	double num = 0;
	if(!value.IsEmpty())
	{
		value.ToDouble(&num);
	}
	return num;
}

/////////////////////////////////////////////////////////////////////////////////////
// xsPointPropIO class //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC_CLASS(xsPointPropIO, xsPropertyIO);

void xsPointPropIO::Read(xsProperty *property, wxXmlNode *source)
{
    *((wxPoint*)property->m_pSourceVariable) = FromString(source->GetNodeContent());
}

void xsPointPropIO::Write(xsProperty *property, wxXmlNode *target)
{
    wxString val = ToString(*((wxPoint*)property->m_pSourceVariable));

    if(val != property->m_sDefaultValueStr)
    {
        wxXmlNode *newNode = AddPropertyNode(target, wxT("property"), val);
        AppendPropertyType(property, newNode);
    }
}

wxString xsPointPropIO::ToString(const wxPoint& value)
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

IMPLEMENT_DYNAMIC_CLASS(xsSizePropIO, xsPropertyIO);

void xsSizePropIO::Read(xsProperty *property, wxXmlNode *source)
{
    *((wxSize*)property->m_pSourceVariable) = FromString(source->GetNodeContent());
}

void xsSizePropIO::Write(xsProperty *property, wxXmlNode *target)
{
    wxString val = ToString(*((wxSize*)property->m_pSourceVariable));

    if(val != property->m_sDefaultValueStr)
    {
        wxXmlNode *newNode = AddPropertyNode(target, wxT("property"), val);
        AppendPropertyType(property, newNode);
    }
}

wxString xsSizePropIO::ToString(const wxSize& value)
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

IMPLEMENT_DYNAMIC_CLASS(xsRealPointPropIO, xsPropertyIO);

void xsRealPointPropIO::Read(xsProperty *property, wxXmlNode *source)
{
    *((wxRealPoint*)property->m_pSourceVariable) = FromString(source->GetNodeContent());
}

void xsRealPointPropIO::Write(xsProperty *property, wxXmlNode *target)
{
    wxString val = ToString(*((wxRealPoint*)property->m_pSourceVariable));

    if(val != property->m_sDefaultValueStr)
    {
        wxXmlNode *newNode = AddPropertyNode(target, wxT("property"), val);
        AppendPropertyType(property, newNode);
    }
}

wxString xsRealPointPropIO::ToString(const wxRealPoint& value)
{
    return wxString::Format(wxT("%lf,%lf"), value.x, value.y);
}

wxRealPoint xsRealPointPropIO::FromString(const wxString& value)
{
	wxRealPoint pt;

	if(!value.IsEmpty())
	{
		wxStringTokenizer tokens(value, wxT(","), wxTOKEN_STRTOK);

		tokens.GetNextToken().ToDouble(&pt.x);
		tokens.GetNextToken().ToDouble(&pt.y);
	}

	return pt;
}

/////////////////////////////////////////////////////////////////////////////////////
// xsColourPropIO class /////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC_CLASS(xsColourPropIO, xsPropertyIO);

void xsColourPropIO::Read(xsProperty *property, wxXmlNode *source)
{
    *((wxColour*)property->m_pSourceVariable) = FromString(source->GetNodeContent());
}

void xsColourPropIO::Write(xsProperty *property, wxXmlNode *target)
{
    wxString val = ToString(*((wxColour*)property->m_pSourceVariable));

    if(val != property->m_sDefaultValueStr)
    {
        wxXmlNode *newNode = AddPropertyNode(target, wxT("property"), val);
        AppendPropertyType(property, newNode);
    }
}

wxString xsColourPropIO::ToString(const wxColour& value)
{
    return wxString::Format(wxT("%d,%d,%d"), value.Red(), value.Green(), value.Blue());
}

wxColour xsColourPropIO::FromString(const wxString& value)
{
	long nRed = 0;
	long nGreen = 0;
	long nBlue = 0;

	if(!value.IsEmpty())
	{
		wxStringTokenizer tokens(value, wxT(","), wxTOKEN_STRTOK);

		tokens.GetNextToken().ToLong(&nRed);
		tokens.GetNextToken().ToLong(&nGreen);
		tokens.GetNextToken().ToLong(&nBlue);
	}

	return wxColour(nRed, nGreen, nBlue);
}

/////////////////////////////////////////////////////////////////////////////////////
// xsPenPropIO class ////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC_CLASS(xsPenPropIO, xsPropertyIO);

void xsPenPropIO::Read(xsProperty *property, wxXmlNode *source)
{
    *((wxPen*)property->m_pSourceVariable) = FromString(source->GetNodeContent());
}

void xsPenPropIO::Write(xsProperty *property, wxXmlNode *target)
{
    wxString val = ToString(*((wxPen*)property->m_pSourceVariable));

    if(val != property->m_sDefaultValueStr)
    {
        wxXmlNode *newNode = AddPropertyNode(target, wxT("property"), val);
        AppendPropertyType(property, newNode);
    }
}

wxString xsPenPropIO::ToString(const wxPen& value)
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

IMPLEMENT_DYNAMIC_CLASS(xsBrushPropIO, xsPropertyIO);

void xsBrushPropIO::Read(xsProperty *property, wxXmlNode *source)
{
    *((wxBrush*)property->m_pSourceVariable) = FromString(source->GetNodeContent());
}

void xsBrushPropIO::Write(xsProperty *property, wxXmlNode *target)
{
    wxString val = ToString(*((wxBrush*)property->m_pSourceVariable));

    if(val != property->m_sDefaultValueStr)
    {
        wxXmlNode *newNode = AddPropertyNode(target, wxT("property"), val);
        AppendPropertyType(property, newNode);
    }
}

wxString xsBrushPropIO::ToString(const wxBrush& value)
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

IMPLEMENT_DYNAMIC_CLASS(xsFontPropIO, xsPropertyIO);

void xsFontPropIO::Read(xsProperty *property, wxXmlNode *source)
{
    *((wxFont*)property->m_pSourceVariable) = FromString(source->GetNodeContent());
}

void xsFontPropIO::Write(xsProperty *property, wxXmlNode *target)
{
    wxString val = ToString(*((wxFont*)property->m_pSourceVariable));

    if(val != property->m_sDefaultValueStr)
    {
        wxXmlNode *newNode = AddPropertyNode(target, wxT("property"), val);
        AppendPropertyType(property, newNode);
    }
}

wxString xsFontPropIO::ToString(const wxFont& value)
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

/////////////////////////////////////////////////////////////////////////////////////
// xsListRealPointPropIO class //////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC_CLASS(xsListRealPointPropIO, xsPropertyIO);

void xsListRealPointPropIO::Read(xsProperty *property, wxXmlNode *source)
{
    ((RealPointList*)property->m_pSourceVariable)->Clear();

    wxXmlNode *listNode = source->GetChildren();
    while(listNode)
    {
        if(listNode->GetName() == wxT("item"))
        {
            ((RealPointList*)property->m_pSourceVariable)->Append(new wxRealPoint(xsRealPointPropIO::FromString(listNode->GetNodeContent())));
        }

        listNode = listNode->GetNext();
    }
}

void xsListRealPointPropIO::Write(xsProperty *property, wxXmlNode *target)
{
    RealPointList *list = (RealPointList*)property->m_pSourceVariable;

    if(list->GetCount() > 0)
    {
        wxXmlNode *newNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("property"));
        wxRealPointListNode* listNode = list->GetFirst();
        while(listNode)
        {
            AddPropertyNode(newNode, wxT("item"), xsRealPointPropIO::ToString(*(wxRealPoint*)listNode->GetData()));
            listNode = listNode->GetNext();
        }

        target->AddChild(newNode);
        AppendPropertyType(property, newNode);
    }
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

