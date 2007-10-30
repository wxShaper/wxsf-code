/***************************************************************
 * Name:      PropertyIO.cpp
 * Purpose:   Declares data types I/O and conversion functions
 * Author:    Michal Bližòák (michal.bliznak@tiscali.cz)
 * Created:   2007-10-28
 * Copyright: Michal Bližòák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifndef XSPROPERTYIO_H
#define XSPROPERTYIO_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/hashmap.h>
#include <wx/xml/xml.h>

#include "wx/wxxmlserializer/Defs.h"

class WXDLLIMPEXP_XS xsProperty;

/*!
 * \brief Base class encapsulating a property I/O handler. The class is used by
 * the xsSerializable class and is responsiblefor reading and writing of an XML node
 * containing property information. Each suppported property (data) type should have
 * its own I/O handler class.
 */
class WXDLLIMPEXP_XS xsPropertyIO : public wxObject
{
public:
    DECLARE_DYNAMIC_CLASS(xsProperty);

    /*! \brief Constructor. */
    xsPropertyIO(){;}
    /*! \brief Destructor. */
    virtual ~xsPropertyIO(){;}

    /*!
     * \brief Read content of the property XML node and store it to given property object.
     * \param property Pointer to the target property object
     * \param source Pointer to the source XML node
     */
    virtual void Read(xsProperty *property, wxXmlNode *source){;}
    /*!
     * \brief Write content of given property object to target XML node.
     * \param property Pointer to the source property object
     * \param target Pointer to the target XML node
     */
    virtual void Write(xsProperty *property, wxXmlNode *target){;}

    /*!
     * \brief Create new XML node of given name and value and assign it to the given
     * parent XML node.
     * \param parent Pointer to parent XML node
     * \param name Name of new XML node
     * \param value Content of new XML node
     */
    static wxXmlNode* AddPropertyNode(wxXmlNode* parent, const wxString& name, const wxString& value);

protected:

    /*!
     * \brief Append info about the source property to given XML node.
     * \param source Pointer to the source property
     * \param target Pointer to modified XML node
     */
    void AppendPropertyType(xsProperty *source, wxXmlNode *target);
};

/*!
 * \brief Property class encapsulating I/O functions used by 'string' properties.
 */
class WXDLLIMPEXP_XS xsStringPropIO : public xsPropertyIO
{
public:
    DECLARE_DYNAMIC_CLASS(xsLongPropIO);

    /*! \brief Constructor. */
    xsStringPropIO(){;}
    /*! \brief Destructor. */
    virtual ~xsStringPropIO(){;}

    /*! \sa xsPropertyIO::Read */
    virtual void Read(xsProperty *prop, wxXmlNode *src);
    /*! \sa xsPropertyIO::Write */
    virtual void Write(xsProperty *prop, wxXmlNode *trg);
};

/*!
 * \brief Property class encapsulating I/O functions used by 'long' properties.
 */
class WXDLLIMPEXP_XS xsLongPropIO : public xsPropertyIO
{
public:
    DECLARE_DYNAMIC_CLASS(xsLongPropIO);

    /*! \brief Constructor. */
    xsLongPropIO(){;}
    /*! \brief Destructor. */
    virtual ~xsLongPropIO(){;}

    /*! \sa xsPropertyIO::Read */
    virtual void Read(xsProperty *prop, wxXmlNode *src);
    /*! \sa xsPropertyIO::Write */
    virtual void Write(xsProperty *prop, wxXmlNode *trg);

    /*! \brief Creates a string representation of the given value */
    static wxString ToString(long value);
    /*! \brief Converts data from given string representation to its relevant value */
    static long FromString(const wxString& value);
};

/*!
 * \brief Property class encapsulating I/O functions used by 'bool' properties.
 */
class WXDLLIMPEXP_XS xsBoolPropIO : public xsPropertyIO
{
public:
    DECLARE_DYNAMIC_CLASS(xsBoolPropIO);

    /*! \brief Constructor. */
    xsBoolPropIO(){;}
    /*! \brief Destructor. */
    virtual ~xsBoolPropIO(){;}

    /*! \sa xsPropertyIO::Read */
    virtual void Read(xsProperty *prop, wxXmlNode *src);
    /*! \sa xsPropertyIO::Write */
    virtual void Write(xsProperty *prop, wxXmlNode *trg);

    /*! \brief Creates a string representation of the given value */
    static wxString ToString(bool value);
    /*! \brief Converts data from given string representation to its relevant value */
    static bool FromString(const wxString& value);
};

/*!
 * \brief Property class encapsulating I/O functions used by 'double' properties.
 */
class WXDLLIMPEXP_XS xsDoublePropIO : public xsPropertyIO
{
public:
    DECLARE_DYNAMIC_CLASS(xsDoublePropIO);

    /*! \brief Constructor. */
    xsDoublePropIO(){;}
    /*! \brief Destructor. */
    virtual ~xsDoublePropIO(){;}

    /*! \sa xsPropertyIO::Read */
    virtual void Read(xsProperty *prop, wxXmlNode *src);
    /*! \sa xsPropertyIO::Write */
    virtual void Write(xsProperty *prop, wxXmlNode *trg);

    /*! \brief Creates a string representation of the given value */
    static wxString ToString(double value);
    /*! \brief Converts data from given string representation to its relevant value */
    static double FromString(const wxString& value);
};

/*!
 * \brief Property class encapsulating I/O functions used by 'wxPoint' properties.
 */
class WXDLLIMPEXP_XS xsPointPropIO : public xsPropertyIO
{
public:
    DECLARE_DYNAMIC_CLASS(xsPointPropIO);

    /*! \brief Constructor. */
    xsPointPropIO(){;}
    /*! \brief Destructor. */
    virtual ~xsPointPropIO(){;}

    /*! \sa xsPropertyIO::Read */
    virtual void Read(xsProperty *prop, wxXmlNode *src);
    /*! \sa xsPropertyIO::Write */
    virtual void Write(xsProperty *prop, wxXmlNode *trg);

    /*! \brief Creates a string representation of the given value */
    static wxString ToString(const wxPoint& value);
    /*! \brief Converts data from given string representation to its relevant value */
    static wxPoint FromString(const wxString& value);
};

/*!
 * \brief Property class encapsulating I/O functions used by 'wxSize' properties.
 */
class WXDLLIMPEXP_XS xsSizePropIO : public xsPropertyIO
{
public:
    DECLARE_DYNAMIC_CLASS(xsSizePropIO);

    /*! \brief Constructor. */
    xsSizePropIO(){;}
    /*! \brief Destructor. */
    virtual ~xsSizePropIO(){;}

    /*! \sa xsPropertyIO::Read */
    virtual void Read(xsProperty *prop, wxXmlNode *src);
    /*! \sa xsPropertyIO::Write */
    virtual void Write(xsProperty *prop, wxXmlNode *trg);

    /*! \brief Creates a string representation of the given value */
    static wxString ToString(const wxSize& value);
    /*! \brief Converts data from given string representation to its relevant value */
    static wxSize FromString(const wxString& value);
};

/*!
 * \brief Property class encapsulating I/O functions used by 'wxRealPoint' properties.
 */
class WXDLLIMPEXP_XS xsRealPointPropIO : public xsPropertyIO
{
public:
    DECLARE_DYNAMIC_CLASS(xsRealPointPropIO);

    /*! \brief Constructor. */
    xsRealPointPropIO(){;}
    /*! \brief Destructor. */
    virtual ~xsRealPointPropIO(){;}

    /*! \sa xsPropertyIO::Read */
    virtual void Read(xsProperty *prop, wxXmlNode *src);
    /*! \sa xsPropertyIO::Write */
    virtual void Write(xsProperty *prop, wxXmlNode *trg);

    /*! \brief Creates a string representation of the given value */
    static wxString ToString(const wxRealPoint& value);
    /*! \brief Converts data from given string representation to its relevant value */
    static wxRealPoint FromString(const wxString& value);
};

/*!
 * \brief Property class encapsulating I/O functions used by 'wxColour' properties.
 */
class WXDLLIMPEXP_XS xsColourPropIO : public xsPropertyIO
{
public:
    DECLARE_DYNAMIC_CLASS(xsColourPropIO);

    /*! \brief Constructor. */
    xsColourPropIO(){;}
    /*! \brief Destructor. */
    virtual ~xsColourPropIO(){;}

    /*! \sa xsPropertyIO::Read */
    virtual void Read(xsProperty *prop, wxXmlNode *src);
    /*! \sa xsPropertyIO::Write */
    virtual void Write(xsProperty *prop, wxXmlNode *trg);

    /*! \brief Creates a string representation of the given value */
    static wxString ToString(const wxColour& value);
    /*! \brief Converts data from given string representation to its relevant value */
    static wxColour FromString(const wxString& value);
};

/*!
 * \brief Property class encapsulating I/O functions used by 'wxPen' properties.
 */
class WXDLLIMPEXP_XS xsPenPropIO : public xsPropertyIO
{
public:
    DECLARE_DYNAMIC_CLASS(xsPenPropIO);

    /*! \brief Constructor. */
    xsPenPropIO(){;}
    /*! \brief Destructor. */
    virtual ~xsPenPropIO(){;}

    /*! \sa xsPropertyIO::Read */
    virtual void Read(xsProperty *prop, wxXmlNode *src);
    /*! \sa xsPropertyIO::Write */
    virtual void Write(xsProperty *prop, wxXmlNode *trg);

    /*! \brief Creates a string representation of the given value */
    static wxString ToString(const wxPen& value);
    /*! \brief Converts data from given string representation to its relevant value */
    static wxPen FromString(const wxString& value);
};

/*!
 * \brief Property class encapsulating I/O functions used by 'wxBrush' properties.
 */
class WXDLLIMPEXP_XS xsBrushPropIO : public xsPropertyIO
{
public:
    DECLARE_DYNAMIC_CLASS(xsBrushPropIO);

    /*! \brief Constructor. */
    xsBrushPropIO(){;}
    /*! \brief Destructor. */
    virtual ~xsBrushPropIO(){;}

    /*! \sa xsPropertyIO::Read */
    virtual void Read(xsProperty *prop, wxXmlNode *src);
    /*! \sa xsPropertyIO::Write */
    virtual void Write(xsProperty *prop, wxXmlNode *trg);

    /*! \brief Creates a string representation of the given value */
    static wxString ToString(const wxBrush& value);
    /*! \brief Converts data from given string representation to its relevant value */
    static wxBrush FromString(const wxString& value);
};

/*!
 * \brief Property class encapsulating I/O functions used by 'wxFont' properties.
 */
class WXDLLIMPEXP_XS xsFontPropIO : public xsPropertyIO
{
public:
    DECLARE_DYNAMIC_CLASS(xsFontPropIO);

    /*! \brief Constructor. */
    xsFontPropIO(){;}
    /*! \brief Destructor. */
    virtual ~xsFontPropIO(){;}

    /*! \sa xsPropertyIO::Read */
    virtual void Read(xsProperty *prop, wxXmlNode *src);
    /*! \sa xsPropertyIO::Write */
    virtual void Write(xsProperty *prop, wxXmlNode *trg);

    /*! \brief Creates a string representation of the given value */
    static wxString ToString(const wxFont& value);
    /*! \brief Converts data from given string representation to its relevant value */
    static wxFont FromString(const wxString& value);
};

/*!
 * \brief Property class encapsulating I/O functions used by 'wxArrayString' properties.
 */
class WXDLLIMPEXP_XS xsArrayStringPropIO : public xsPropertyIO
{
public:
    DECLARE_DYNAMIC_CLASS(xsArrayStringPropIO);

    /*! \brief Constructor. */
    xsArrayStringPropIO(){;}
    /*! \brief Destructor. */
    virtual ~xsArrayStringPropIO(){;}

    /*! \sa xsPropertyIO::Read */
    virtual void Read(xsProperty *prop, wxXmlNode *src);
    /*! \sa xsPropertyIO::Write */
    virtual void Write(xsProperty *prop, wxXmlNode *trg);
};

/*!
 * \brief Property class encapsulating I/O functions used by 'ArrayRealPoint' (array of
 * wxRealPoint objects) properties.
 */
class WXDLLIMPEXP_XS xsArrayRealPointPropIO : public xsPropertyIO
{
public:
    DECLARE_DYNAMIC_CLASS(xsArrayRealPointPropIO);

    /*! \brief Constructor. */
    xsArrayRealPointPropIO(){;}
    /*! \brief Destructor. */
    virtual ~xsArrayRealPointPropIO(){;}

    /*! \sa xsPropertyIO::Read */
    virtual void Read(xsProperty *prop, wxXmlNode *src);
    /*! \sa xsPropertyIO::Write */
    virtual void Write(xsProperty *prop, wxXmlNode *trg);
};

/*!
 * \brief Property class encapsulating I/O functions used by 'ListRealPoint' (list of
 * wxRealPoint objects) properties.
 */
class WXDLLIMPEXP_XS xsListRealPointPropIO : public xsPropertyIO
{
public:
    DECLARE_DYNAMIC_CLASS(xsListRealPointPropIO);

    /*! \brief Constructor. */
    xsListRealPointPropIO(){;}
    /*! \brief Destructor. */
    virtual ~xsListRealPointPropIO(){;}

    /*! \sa xsPropertyIO::Read */
    virtual void Read(xsProperty *prop, wxXmlNode *src);
    /*! \sa xsPropertyIO::Write */
    virtual void Write(xsProperty *prop, wxXmlNode *trg);
};

/*!
 * \brief Property class encapsulating I/O functions used by 'serializabledynamic' (xsSerializable
 * dynamic class objects which are created during the deserialization process) properties.
 */
class WXDLLIMPEXP_XS xsDynObjPropIO : public xsPropertyIO
{
public:
    DECLARE_DYNAMIC_CLASS(xsDynObjPropIO);

    /*! \brief Constructor. */
    xsDynObjPropIO(){;}
    /*! \brief Destructor. */
    virtual ~xsDynObjPropIO(){;}

    /*! \sa xsPropertyIO::Read */
    virtual void Read(xsProperty *prop, wxXmlNode *src);
    /*! \sa xsPropertyIO::Write */
    virtual void Write(xsProperty *prop, wxXmlNode *trg);
};

/*!
 * \brief Property class encapsulating I/O functions used by 'serializabledynamicnocreate' (already
 * existing xsSerializable dynamic class objects) properties.
 */
class WXDLLIMPEXP_XS xsDynNCObjPropIO : public xsPropertyIO
{
public:
    DECLARE_DYNAMIC_CLASS(xsDynNCObjPropIO);

    /*! \brief Constructor. */
    xsDynNCObjPropIO(){;}
    /*! \brief Destructor. */
    virtual ~xsDynNCObjPropIO(){;}

    /*! \sa xsPropertyIO::Read */
    virtual void Read(xsProperty *prop, wxXmlNode *src);
    /*! \sa xsPropertyIO::Write */
    virtual void Write(xsProperty *prop, wxXmlNode *trg);
};

/*!
 * \brief Property class encapsulating I/O functions used by 'serializablestatic' (static
 * xsSerializable class objects) properties.
 */
class WXDLLIMPEXP_XS xsStaticObjPropIO : public xsPropertyIO
{
public:
    DECLARE_DYNAMIC_CLASS(xsStaticObjPropIO);

    /*! \brief Constructor. */
    xsStaticObjPropIO(){;}
    /*! \brief Destructor. */
    virtual ~xsStaticObjPropIO(){;}

    /*! \sa xsPropertyIO::Read */
    virtual void Read(xsProperty *prop, wxXmlNode *src);
    /*! \sa xsPropertyIO::Write */
    virtual void Write(xsProperty *prop, wxXmlNode *trg);
};

WX_DECLARE_HASH_MAP( wxString, xsPropertyIO*, wxStringHash, wxStringEqual, PropertyIOMap );

#endif // XSPROPERTYIO_H
