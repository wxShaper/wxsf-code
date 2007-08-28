/***************************************************************
 * Name:      XmlSerializer.h
 * Purpose:   Defines XML serializer and related classes
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-08-28
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifndef XMLSERIALIZE_H
#define XMLSERIALIZE_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/list.h>
#include <wx/xml/xml.h>

WX_DECLARE_OBJARRAY(wxRealPoint, RealPointArray);
WX_DECLARE_LIST(wxRealPoint, RealPointList);

/*! \brief Macro creates new serialized STRING property */
#define XS_SERIALIZE_STRING(x, name) wxASSERT_MSG(wxVariant(x).GetType()==wxT("string"), wxT("Variable is not wxString"));XS_SERIALIZE_PROPERTY(x, wxT("string"), name);
/*! \brief Macro creates new serialized STRING property with defined default value */
#define XS_SERIALIZE_STRING_EX(x, name, def) wxASSERT_MSG(wxVariant(x).GetType()==wxT("string"), wxT("Variable is not wxString"));XS_SERIALIZE_PROPERTY_EX(x, wxT("string"), name, def);
/*! \brief Macro creates new serialized LONG property */
#define XS_SERIALIZE_LONG(x, name) wxASSERT_MSG(wxVariant(x).GetType()==wxT("long"), wxT("Variable is not LONG"));XS_SERIALIZE_PROPERTY(x, wxT("long"), name);
/*! \brief Macro creates new serialized LONG property with defined default value */
#define XS_SERIALIZE_LONG_EX(x, name, def) wxASSERT_MSG(wxVariant(x).GetType()==wxT("long"), wxT("Variable is not LONG"));XS_SERIALIZE_PROPERTY_EX(x, wxT("long"), name, xsSerializable::LongToString(def));
/*! \brief Macro creates new serialized DOUBLE property */
#define XS_SERIALIZE_DOUBLE(x, name) wxASSERT_MSG(wxVariant(x).GetType()==wxT("double"), wxT("Variable is not DOUBLE"));XS_SERIALIZE_PROPERTY(x, wxT("double"), name);
/*! \brief Macro creates new serialized DOUBLE property with defined default value */
#define XS_SERIALIZE_DOUBLE_EX(x, name, def) wxASSERT_MSG(wxVariant(x).GetType()==wxT("double"), wxT("Variable is not DOUBLE"));XS_SERIALIZE_PROPERTY_EX(x, wxT("double"), name, xsSerializable::DoubleToString(def));
/*! \brief Macro creates new serialized BOOL property */
#define XS_SERIALIZE_BOOL(x, name) wxASSERT_MSG(wxVariant(x).GetType()==wxT("bool"), wxT("Variable is not BOOL"));XS_SERIALIZE_PROPERTY(x, wxT("bool"), name);
/*! \brief Macro creates new serialized BOOL property with defined default value */
#define XS_SERIALIZE_BOOL_EX(x, name, def) wxASSERT_MSG(wxVariant(x).GetType()==wxT("bool"), wxT("Variable is not BOOL"));XS_SERIALIZE_PROPERTY_EX(x, wxT("bool"), name, xsSerializable::BoolToString(def));

/*! \brief Macro creates new serialized wxPoint property */
#define XS_SERIALIZE_POINT(x, name) XS_SERIALIZE_PROPERTY(x, wxT("point"), name);
/*! \brief Macro creates new serialized wxPoint property with defined default value */
#define XS_SERIALIZE_POINT_EX(x, name, def) XS_SERIALIZE_PROPERTY_EX(x, wxT("point"), name, xsSerializable::PointToString(def));
/*! \brief Macro creates new serialized wxRealPoint property */
#define XS_SERIALIZE_REALPOINT(x, name) XS_SERIALIZE_PROPERTY(x, wxT("realpoint"), name);
/*! \brief Macro creates new serialized wxRealPoint property with defined default value */
#define XS_SERIALIZE_REALPOINT_EX(x, name, def) XS_SERIALIZE_PROPERTY_EX(x, wxT("realpoint"), name, xsSerializable::RealPointToString(def));
/*! \brief Macro creates new serialized wxSize property */
#define XS_SERIALIZE_SIZE(x, name) XS_SERIALIZE_PROPERTY(x, wxT("size"), name);
/*! \brief Macro creates new serialized wxSize property with defined default value */
#define XS_SERIALIZE_SIZE_EX(x, name, def) XS_SERIALIZE_PROPERTY_EX(x, wxT("size"), name, xsSerializable::SizeToString(def));

/*! \brief Macro creates new serialized wxColour property */
#define XS_SERIALIZE_COLOUR(x, name) wxASSERT_MSG(x.IsKindOf(CLASSINFO(wxColour)), wxT("Object is not wxColour"));XS_SERIALIZE_PROPERTY(x, wxT("colour"), name);
/*! \brief Macro creates new serialized wxColour property with defined default value */
#define XS_SERIALIZE_COLOUR_EX(x, name, def) wxASSERT_MSG(x.IsKindOf(CLASSINFO(wxColour)), wxT("Object is not wxColour"));XS_SERIALIZE_PROPERTY_EX(x, wxT("colour"), name, xsSerializable::ColourToString(def));
/*! \brief Macro creates new serialized wxPen property */
#define XS_SERIALIZE_PEN(x, name) wxASSERT_MSG(x.IsKindOf(CLASSINFO(wxPen)), wxT("Object is not wxPen"));XS_SERIALIZE_PROPERTY(x, wxT("pen"), name);
/*! \brief Macro creates new serialized wxPen property with defined default value */
#define XS_SERIALIZE_PEN_EX(x, name, def) wxASSERT_MSG(x.IsKindOf(CLASSINFO(wxPen)), wxT("Object is not wxPen"));XS_SERIALIZE_PROPERTY_EX(x, wxT("pen"), name, xsSerializable::PenToString(def));
/*! \brief Macro creates new serialized wxBrush property */
#define XS_SERIALIZE_BRUSH(x, name) wxASSERT_MSG(x.IsKindOf(CLASSINFO(wxBrush)), wxT("Object is not wxBrush"));XS_SERIALIZE_PROPERTY(x, wxT("brush"), name);
/*! \brief Macro creates new serialized wxBrush property with defined default value */
#define XS_SERIALIZE_BRUSH_EX(x, name, def) wxASSERT_MSG(x.IsKindOf(CLASSINFO(wxBrush)), wxT("Object is not wxBrush"));XS_SERIALIZE_PROPERTY_EX(x, wxT("brush"), name, xsSerializable::BrushToString(def));
/*! \brief Macro creates new serialized wxFont property */
#define XS_SERIALIZE_FONT(x, name) wxASSERT_MSG(x.IsKindOf(CLASSINFO(wxFont)), wxT("Object is not wxFont"));XS_SERIALIZE_PROPERTY(x, wxT("font"), name);
/*! \brief Macro creates new serialized wxFont property with defined default value */
#define XS_SERIALIZE_FONT_EX(x, name, def) wxASSERT_MSG(x.IsKindOf(CLASSINFO(wxFont)), wxT("Object is not wxFont"));XS_SERIALIZE_PROPERTY_EX(x, wxT("font"), name, xsSerializable::FontToString(def));

/*! \brief Macro creates new serialized property (type 'array of strings (wxArrayString)') */
#define XS_SERIALIZE_ARRAYSTRING(x, name) XS_SERIALIZE_PROPERTY(x, wxT("arraystring"), name);
/*! \brief Macro creates new serialized property (type 'array of wxRealPoint objects') */
#define XS_SERIALIZE_ARRAYREALPOINT(x, name) XS_SERIALIZE_PROPERTY(x, wxT("arrayrealpoint"), name);
/*! \brief Macro creates new serialized property (type 'list of wxRealPoint objects') */
#define XS_SERIALIZE_LISTREALPOINT(x, name) XS_SERIALIZE_PROPERTY(x, wxT("listrealpoint"), name);

/*! \brief Macro creates new serialized property encapsulating a dynamic serializable object */
#define XS_SERIALIZE_DYNAMIC_OBJECT(x, name) XS_SERIALIZE_PROPERTY(x, wxT("serializabledynamic"), name);
/*! \brief Macro creates new serialized property encapsulating a static serializable object */
#define XS_SERIALIZE_STATIC_OBJECT(x, name) wxASSERT_MSG(x.IsKindOf(CLASSINFO(xsSerializable)), wxT("Object is not xsSerializable"));XS_SERIALIZE_PROPERTY(x, wxT("serializablestatic"), name);

/*! \brief Macro creates new serialized property of given type */
#define XS_SERIALIZE_PROPERTY(x, type, name) AddProperty(new xsProperty(&x, type, name));
/*! \brief Macro creates new serialized property of given type with defined dafult value */
#define XS_SERIALIZE_PROPERTY_EX(x, type, name, def) AddProperty(new xsProperty(&x, type, name, def));

/*! \brief Macro creates new serialized property and automaticaly determines its type (if supported) */
#define XS_SERIALIZE(x, name) AddProperty(new xsProperty(&x, name));
/*! \brief Macro creates new serialized property with defined dafult value and automaticaly determines its type (if supported)*/
#define XS_SERIALIZE_EX(x, name, def) AddProperty(new xsProperty(&x, name, def));

class xsProperty;
class xsSerializable;

WX_DECLARE_LIST(xsProperty, PropertyList);
WX_DECLARE_LIST(xsSerializable, SerializableList);

/*!
 * \brief Base class encapsulating object which can be serialized/deserialized to/from
 * XML file (disk file or any stream). This class acts as a data container for properties
 * (xsProperty class objects) encapsulating serialized class data members.
 *
 * Class data members which should be serialized must be marked by appropriate macro defined
 * in wxXmlSerializer.h header file (it is recommended to mark desired data members in the class constructor).
 *
 * Instances of this class can be arranged into a list/d-ary tree hierarchy so it can behave like
 * powerfull data container. All chained serializable class objects can be handled by class
 * member functions or by member functions of wxXmlSerializer class object which should be
 * used as their manager (recommended way).
 */
class xsSerializable : public wxObject
{
public:
    DECLARE_DYNAMIC_CLASS(xsSerializable);

    /*! \brief Constructor. */
    xsSerializable();
    /*! \brief Destructor. */
    ~xsSerializable();

    // public functions
    /*!
     * \brief Get serializable parent object.
     * \return Pointer to serializable parent object if exists, otherwise NULL
     */
    xsSerializable* GetParent(){return m_pParentItem;}
    /*!
     * \brief Get first serializable child object.
     * \return Pointer to child object if exists, otherwise NULL
     */
    xsSerializable* GetFirstChild();
    /*!
     * \brief Get last serializable child object.
     * \return Pointer to child object if exists, otherwise NULL
     */
    xsSerializable* GetLastChild();
    /*!
     * \brief Get next serializable sibbling object.
     * \return Pointer to sibbling object if exists, otherwise NULL
     */
    xsSerializable* GetSibbling();

    /*!
     * \brief Get list of all children (serializable objects) of this object.
     * \return List with child serializable objects (can be empty)
     */
    SerializableList& GetChildren(){return m_lstChildItems;}
    /*!
     * \brief Get pointer to list node containing first serializable child object.
     */
    wxSerializableListNode* GetFirstChildNode(){return m_lstChildItems.GetFirst();}
    /*!
     * \brief Get pointer to list node containing last serializable child object.
     */
    wxSerializableListNode* GetLastChildNode(){return m_lstChildItems.GetLast();}

    /*!
     * \brief Set serializable parent object.
     * \param parent Pointer to parent object
     */
    void SetParent(xsSerializable* parent){m_pParentItem = parent;}
    /*!
     * \brief Add serializable child object to this object.
     * \param child Pointer to added child object (must NOT be NULL)
     */
    void AddChild(xsSerializable* child);
    /*!
     * \brief Assign this object as a child to given parent object.
     * \param parent Pointer to new parent object (must NOT be NULL)
     */
    void Reparent(xsSerializable* parent);

    /*!
     * \brief Set ID of this object. Can be used for further objects' handling by
     * wxXmlSerializer class (default ID value is -1). This functions should NOT
     * be used directly; it is called by wxXmlSerializer object in the case that this
     * serializable object is attached to another one (or directly to root node of wxXmlSerializer) by
     * wxXmlSerializer::AddItem() member function.
     */
    void SetId(long id){m_nId = id;}
    /*!
     * \brief Get object ID.
     * \return ID value or -1 if the ID hasn't been set yet
     */
    long GetId(){return m_nId;}

    /*!
     * \brief Create new 'object' XML node and serialize all marked class data members (properties) into it.
     * \param node Pointer to parent XML node
     * \return Pointer to modified parent XML node
     */
    wxXmlNode* SerializeObject(wxXmlNode* node);
    /*!
     * \brief Deserialize marked class data members (properties) from appropriate fields of given
     * parent 'object' XML node.
     * \param node Pointer to parent 'object' XML node
     */
    void DeserializeObject(wxXmlNode* node);

    /*!
     * \brief Add new property to the property list.
     * \param property Pointer to added property object
     * \sa xsProperty
     */
    void AddProperty(xsProperty* property);
    /*!
     * \brief Get serialized property of given name.
     * \return Pointer to the property object if exists, otherwise NULL
     * \sa xsProperty
     */
    xsProperty* GetProperty(const wxString& field);

    /*!
     * \brief Enable/disable serialization of given property.
     * \param field Property name
     * \param enab TRUE if the property should be serialized, otherwise FALSE
     */
    void EnablePropertySerialization(const wxString& field, bool enab);
    /*!
     * \brief Returns information whether the given property is serialized or not.
     * \param field Name of examined property
     */
    bool IsPropertySerialized(const wxString& field);

    /*! \brief Converts data from given string representation to its relevant value */
    static wxColour StringToColour(const wxString& val);
    /*! \brief Converts data from given string representation to its relevant value */
    static wxRealPoint StringToRealPoint(const wxString& val);
    /*! \brief Converts data from given string representation to its relevant value */
    static wxPoint StringToPoint(const wxString& val);
    /*! \brief Converts data from given string representation to its relevant value */
    static wxSize StringToSize(const wxString& val);
    /*! \brief Converts data from given string representation to its relevant value */
    static wxBrush StringToBrush(const wxString& val);
    /*! \brief Converts data from given string representation to its relevant value */
    static wxPen StringToPen(const wxString& val);
    /*! \brief Converts data from given string representation to its relevant value */
    static wxFont StringToFont(const wxString& val);
    /*! \brief Converts data from given string representation to its relevant value */
    static long StringToLong(const wxString& val);
    /*! \brief Converts data from given string representation to its relevant value */
    static double StringToDouble(const wxString& val);
    /*! \brief Converts data from given string representation to its relevant value */
    static bool StringToBool(const wxString& val);

    /*! \brief Creates a string representation of the given value */
    static wxString ColourToString(const wxColour& val);
    /*! \brief Creates a string representation of the given value */
    static wxString RealPointToString(const wxRealPoint& val);
    /*! \brief Creates a string representation of the given value */
    static wxString PointToString(const wxPoint& val);
    /*! \brief Creates a string representation of the given value */
    static wxString SizeToString(const wxSize& val);
    /*! \brief Creates a string representation of the given value */
    static wxString BrushToString(const wxBrush& val);
    /*! \brief Creates a string representation of the given value */
    static wxString PenToString(const wxPen& val);
    /*! \brief Creates a string representation of the given value */
    static wxString FontToString(const wxFont& val);
    /*! \brief Creates a string representation of the given value */
    static wxString LongToString(long val);
    /*! \brief Creates a string representation of the given value */
    static wxString DoubleToString(double val);
    /*! \brief Creates a string representation of the given value */
    static wxString BoolToString(bool val);

    /*!
     * \brief Create new XML node of given name and value and assign it to the given
     * parent XML node.
     * \param parent Pointer to parent XML node
     * \param name Name of new XML node
     * \param value Content of new XML node
     */
    static wxXmlNode* AddPropertyNode(wxXmlNode* parent, const wxString& name, const wxString& value);

protected:
    // protected data members
    /*! \brief List of serialized properties */
    PropertyList m_lstProperties;
    /*! \brief List of child objects */
    SerializableList m_lstChildItems;

    /*! \brief Pointer to parent object */
    xsSerializable* m_pParentItem;

    /*! \brief Object ID */
    long m_nId;

    // protected functions
    /*!
     * \brief Get string representation of value of given property.
     * \param property Pointer to property object
     */
    wxString ConvertToString(xsProperty* property);
    /*!
     * \brief Fill member data object encapsulated by given property with a value
     * converted from its string representation.
     * \param source String representation of property value
     * \param property Pointer to property
     */
    void FillFromString(const wxString& source, xsProperty* property);

    // protected virtual functions
    /*!
     * \brief Serialize stored properties to the given XML node. The serialization
     * routine is automatically called by the framework and cares about serialization
     * of all defined properties.
     *
     * Note that default implementation automatically serializes all class data members
     * marked by appropriate macros. If some non-standard class member should be serialized as well,
     * the source code of derived function implementation can be as in following example.
     *
     * \param node Pointer to XML node where the property nodes will be appended to
     *
     * Example code:
     * \code
     * wxXmlNode* DerivedFrom_xsSerializable::Serialize(wxXmlNode* node)
     * {
     *     if(node)
     *     {
     *         // call base class's serialization routine
     *         node = xsSeralizable::Serialize(node);
     *
     *         // serialize custom property
     *         AddPropertyNode(node, wxT("some_property_field_name"), wxT("string_repr_of_its_value"));
     *     }
     *     // return updated node
     *     return node;
     * }
     * \endcode
     */
    virtual wxXmlNode* Serialize(wxXmlNode* node);
    /*!
     * \brief Deserialize object properties from the given XML node. The
     * routine is automatically called by the framework and cares about deserialization
     * of all defined properties.
     *
     * Note that default implementation automatically deserializes all class data members
     * marked by appropriate macros. If some non-standard class member should be deserialized as well,
     * the source code of derived function implementation can be as in following example.
     *
     * \param node Pointer to a source XML node containig the property nodes
     *
     * Example code:
     * \code
     * void DerivedFrom_xsSerializable::Deserialize(wxXmlNode* node)
     * {
     *      // call base class's deserialization rountine (if necessary...)
     *      xsSerializable::Deserialize(node);
     *
     *      // iterate through all custom property nodes
     *      wxXmlNode *propNode = node->GetChildren();
     *      while(propNode)
     *      {
     *          if(propNode->GetName() == wxT("some_property_field_name"))
     *          {
     *              // read the node content and convert it to a proper data type
     *          }
     *          propNode = propNode->GetNext();
     *      }
     * }
     * \endcode
     */
    virtual void Deserialize(wxXmlNode* node);
};

/*!
 * \brief Class encapsulates a property stored in a list included inside a parent serializable
 * object (class xsSerializable) which is serialized/deserialized to/from XML file. The
 * property object type is defined by a string name and is processed by parent xsSerializable class object.
 *
 * Allowed property data types (keywords) are: 'long', 'double', 'bool', 'string', 'point', 'size',
 * 'realpoint', 'colour', 'brush', 'pen', 'font', 'arraystring', 'arrayrealpoint', 'listrealpoint',
 * 'serializabledynamic' and 'serializablestatic'. Only properties of these data types are recognized
 * and processed by parent serializable object.
 */
class xsProperty : public wxObject
{
public:
    DECLARE_DYNAMIC_CLASS(xsProperty);

    /*! \brief Default constructor */
    xsProperty()
    {
        m_pSourceVariable = NULL;
        m_sDataType = wxT("Undefined");
        m_sFieldName = wxT("Undefined");
        m_sDefaultValueStr = wxT("");
        m_fSerialize = false;
    }

    /*!
     * \brief Constructor
     * \param src Pointer to serialized object
     * \param type String value describing data type of serialized object
     * \param field Property name used in XML files and for property handling
     * \param def String representation of default poperty value
     */
    xsProperty(void* src, const wxString& type, const wxString& field, const wxString& def = wxT(""))
    {
        m_pSourceVariable = src;
        m_sDataType = type;
        m_sFieldName = field;
        m_sDefaultValueStr = def;
        m_fSerialize = true;
    }

    /*! \brief Constructor for BOOL property. */
    xsProperty(bool* src, const wxString& field) : m_pSourceVariable((void*)src), m_sFieldName(field), m_sDataType(wxT("bool")), m_sDefaultValueStr(wxT("")), m_fSerialize(true) {;}
    /*! \brief Constructor for BOOL property with defined default value. */
    xsProperty(bool* src, const wxString& field, bool def) : m_pSourceVariable((void*)src), m_sFieldName(field), m_sDataType(wxT("bool")), m_sDefaultValueStr(xsSerializable::BoolToString(def)), m_fSerialize(true) {;}

    /*! \brief Constructor for LONG property. */
    xsProperty(long* src, const wxString& field) : m_pSourceVariable((void*)src), m_sFieldName(field), m_sDataType(wxT("long")), m_sDefaultValueStr(wxT("")), m_fSerialize(true) {;}
    /*! \brief Constructor for LONG property with defined default value. */
    xsProperty(long* src, const wxString& field, long def) : m_pSourceVariable((void*)src), m_sFieldName(field), m_sDataType(wxT("long")), m_sDefaultValueStr(xsSerializable::LongToString(def)), m_fSerialize(true) {;}

    /*! \brief Constructor for DOUBLE property. */
    xsProperty(double* src, const wxString& field) : m_pSourceVariable((void*)src), m_sFieldName(field), m_sDataType(wxT("double")), m_sDefaultValueStr(wxT("")), m_fSerialize(true) {;}
    /*! \brief Constructor for DOUBLE property with defined default value. */
    xsProperty(double* src, const wxString& field, double def) : m_pSourceVariable((void*)src), m_sFieldName(field), m_sDataType(wxT("double")), m_sDefaultValueStr(xsSerializable::DoubleToString(def)), m_fSerialize(true) {;}

    /*! \brief Constructor for wxString property. */
    xsProperty(wxString* src, const wxString& field) : m_pSourceVariable((void*)src), m_sFieldName(field), m_sDataType(wxT("string")), m_sDefaultValueStr(wxT("")), m_fSerialize(true) {;}
    /*! \brief Constructor for wxString property with defined default value. */
    xsProperty(wxString* src, const wxString& field, const wxString& def) : m_pSourceVariable((void*)src), m_sFieldName(field), m_sDataType(wxT("string")), m_sDefaultValueStr(def), m_fSerialize(true) {;}

    /*! \brief Constructor for wxPoint property. */
    xsProperty(wxPoint* src, const wxString& field) : m_pSourceVariable((void*)src), m_sFieldName(field), m_sDataType(wxT("point")), m_sDefaultValueStr(wxT("")), m_fSerialize(true) {;}
    /*! \brief Constructor for wxPoint property with defined default value. */
    xsProperty(wxPoint* src, const wxString& field, const wxPoint& def) : m_pSourceVariable((void*)src), m_sFieldName(field), m_sDataType(wxT("point")), m_sDefaultValueStr(xsSerializable::PointToString(def)), m_fSerialize(true) {;}

    /*! \brief Constructor for wxRealPoint property. */
    xsProperty(wxRealPoint* src, const wxString& field) : m_pSourceVariable((void*)src), m_sFieldName(field), m_sDataType(wxT("realpoint")), m_sDefaultValueStr(wxT("")), m_fSerialize(true) {;}
    /*! \brief Constructor for wxRealPoint property with defined default value. */
    xsProperty(wxRealPoint* src, const wxString& field, const wxRealPoint& def) : m_pSourceVariable((void*)src), m_sFieldName(field), m_sDataType(wxT("realpoint")), m_sDefaultValueStr(xsSerializable::RealPointToString(def)), m_fSerialize(true) {;}

    /*! \brief Constructor for wxSize property. */
    xsProperty(wxSize* src, const wxString& field) : m_pSourceVariable((void*)src), m_sFieldName(field), m_sDataType(wxT("size")), m_sDefaultValueStr(wxT("")), m_fSerialize(true) {;}
    /*! \brief Constructor for wxSize property with defined default value. */
    xsProperty(wxSize* src, const wxString& field, const wxSize& def) : m_pSourceVariable((void*)src), m_sFieldName(field), m_sDataType(wxT("size")), m_sDefaultValueStr(xsSerializable::SizeToString(def)), m_fSerialize(true) {;}

    /*! \brief Constructor for wxBrush property. */
    xsProperty(wxBrush* src, const wxString& field) : m_pSourceVariable((void*)src), m_sFieldName(field), m_sDataType(wxT("brush")), m_sDefaultValueStr(wxT("")), m_fSerialize(true) {;}
    /*! \brief Constructor for wxBrush property with defined default value. */
    xsProperty(wxBrush* src, const wxString& field, const wxBrush& def) : m_pSourceVariable((void*)src), m_sFieldName(field), m_sDataType(wxT("brush")), m_sDefaultValueStr(xsSerializable::BrushToString(def)), m_fSerialize(true) {;}

    /*! \brief Constructor for wxPen property. */
    xsProperty(wxPen* src, const wxString& field) : m_pSourceVariable((void*)src), m_sFieldName(field), m_sDataType(wxT("pen")), m_sDefaultValueStr(wxT("")), m_fSerialize(true) {;}
    /*! \brief Constructor for wxPen property with defined default value. */
    xsProperty(wxPen* src, const wxString& field, const wxPen& def) : m_pSourceVariable((void*)src), m_sFieldName(field), m_sDataType(wxT("pen")), m_sDefaultValueStr(xsSerializable::PenToString(def)), m_fSerialize(true) {;}

    /*! \brief Constructor for wxFont property. */
    xsProperty(wxFont* src, const wxString& field) : m_pSourceVariable((void*)src), m_sFieldName(field), m_sDataType(wxT("font")), m_sDefaultValueStr(wxT("")), m_fSerialize(true) {;}
    /*! \brief Constructor for wxFont property with defined default value. */
    xsProperty(wxFont* src, const wxString& field, const wxFont& def) : m_pSourceVariable((void*)src), m_sFieldName(field), m_sDataType(wxT("font")), m_sDefaultValueStr(xsSerializable::FontToString(def)), m_fSerialize(true) {;}

    /*! \brief Constructor for wxColour property. */
    xsProperty(wxColour* src, const wxString& field) : m_pSourceVariable((void*)src), m_sFieldName(field), m_sDataType(wxT("colour")), m_sDefaultValueStr(wxT("")), m_fSerialize(true) {;}
    /*! \brief Constructor for wxColour property with defined default value. */
    xsProperty(wxColour* src, const wxString& field, const wxColour& def) : m_pSourceVariable((void*)src), m_sFieldName(field), m_sDataType(wxT("colour")), m_sDefaultValueStr(xsSerializable::ColourToString(def)), m_fSerialize(true) {;}

    /*! \brief Constructor for wxArrayString property. */
    xsProperty(wxArrayString* src, const wxString& field) : m_pSourceVariable((void*)src), m_sFieldName(field), m_sDataType(wxT("arraystring")), m_sDefaultValueStr(wxT("")), m_fSerialize(true) {;}

    /*! \brief Constructor for RealPointArray property. */
    xsProperty(RealPointArray* src, const wxString& field) : m_pSourceVariable((void*)src), m_sFieldName(field), m_sDataType(wxT("arrayrealpoint")), m_sDefaultValueStr(wxT("")), m_fSerialize(true) {;}

    /*! \brief Constructor for RealPointList property. */
    xsProperty(RealPointList* src, const wxString& field) : m_pSourceVariable((void*)src), m_sFieldName(field), m_sDataType(wxT("listrealpoint")), m_sDefaultValueStr(wxT("")), m_fSerialize(true) {;}

    /*! \brief Constructor for static serializable property. */
    xsProperty(xsSerializable* src, const wxString& field) : m_pSourceVariable((void*)src), m_sFieldName(field), m_sDataType(wxT("serializablestatic")), m_sDefaultValueStr(wxT("")), m_fSerialize(true) {;}

    /*! \brief Constructor for dynamic serializable property. */
    xsProperty(xsSerializable** src, const wxString& field) : m_pSourceVariable((void**)src), m_sFieldName(field), m_sDataType(wxT("serializabledynamic")), m_sDefaultValueStr(wxT("")), m_fSerialize(true) {;}


    ~xsProperty(){;}

    // public data members
    /*! \brief General (void) pointer to serialized object encapsulated by the property */
    void* m_pSourceVariable;
    /*! \brief Field (property) name */
    wxString m_sFieldName;
    /*! \brief Data type */
    wxString m_sDataType;
    /*! \brief String representation of property's default value */
    wxString m_sDefaultValueStr;
    /*! \brief Flag used for enabling/disabling of property serialization */
    bool m_fSerialize;
};

/*!
 * \brief Class encapsulates a serializable objects' manager which is responsible
 * for handling stored serializable objects and their serialization/deserialization
 * from/to XML files or streams.
 *
 * Stored objects can be arranged into a list or d-ary tree structure so this class
 * can be used as a container for various application data. Also created XML files
 * (root node) can be marked with given version and owner name, so it is possible to
 * control a version of saved document.
 *
 * wxXmlSerializer class contains one instance of xsSerializable object created in the
 * class constructor (can be set later via member functions as well). This serializable
 * object called 'root object' holds all other inserted serializable objects (in case of
 * tree structure it is a topmost tree node, in case of list structure all list items are
 * its children). These child object can be handled via xsSerializable and wxXmlSerializer
 * classes' member functions.
 */
class wxXmlSerializer : public wxObject
{
public:
    DECLARE_DYNAMIC_CLASS(wxXmlSerializer);

    /*! \brief Constructor. */
    wxXmlSerializer();
    /*!
     * \brief User constructor.
     * \param owner Owner name
     * \param root Name of root node
     * \param version File version
     */
    wxXmlSerializer(const wxString& owner, const wxString& root, const wxString& version);
    /*! \brief Destructor. */
    virtual ~wxXmlSerializer();

    // public member data accessors
    /*!
     * \brief Set owner name.
     * \param name Owner name
     */
    void SetSerializerOwner(const wxString& name){m_sOwner = name;}
    /*!
     * \brief Set root name.
     * \param name Root name
     */
    void SetSerializerRootName(const wxString& name){m_sRootName = name;}
    /*!
     * \brief Set file versio.
     * \param name File version
     */
    void SetSerializerVersion(const wxString& name){m_sVersion = name;}
    /*! \brief Get owner name. */
    wxString GetSerializerOwner(){return m_sOwner;}
    /*! \brief Get name of root node. */
    wxString GetSerializerRootName(){return m_sRootName;}
    /*! \brief Get file version. */
    wxString GetSerializerVersion(){return m_sVersion;}

    // public functions
    /*! \brief Get pointer to root serializable object. */
    xsSerializable* GetRootItem(){return m_pRoot;}
    /*!
     * \brief Get serialzable object with given ID.
     * \param id Object ID
     * \return Pointer to serializable object if exists, otherwise NULL
     */
    xsSerializable* GetItem(long id);
    /*!
     * \brief Get items of given class type.
     * \param type Class type
     * \param list List with matching serializable objects
     */
    int GetItems(wxClassInfo* type, SerializableList& list);

    /*!
     * \brief Set root item.
     * \param root Pointer to root item
     */
    void SetRootItem(xsSerializable* root);

    /*!
     * \brief Add serializable object to the serializer.
     * \param parentId ID of parent serializable object
     * \param item Added serializable object
     */
    void AddItem(long parentId, xsSerializable* item);
    /*!
     * \brief Add serializable object to the serializer.
     * \param parent Pointer to parent serializable object (if NULL then the object
	 * is added directly to the root item)
     * \param item Added serializable object
     */
    void AddItem(xsSerializable* parent, xsSerializable* item);
    /*!
     * \brief Remove serializable object from the serializer (object will be destroyed).
     * \param id Object ID
     */
    void RemoveItem(long id);
    /*!
     * \brief Remove serializable object from the serializer (object will be destroyed).
     * \param item Pointer to removed object
     */
    void RemoveItem(xsSerializable* item);
    /*! \brief Remove and destroy all stored serializable objects*/
    void RemoveAll();

    /*!
     * \brief Serialize stored objects to given file.
     * \param file Full path to output file.
     */
    void SerializeToXml(const wxString& file);
    /*!
     * \brief Serialize strored objects to given stream.
     * \param outstream Output stream
     */
    void SerializeToXml(wxOutputStream& outstream);
    /*!
     * \brief Deserialize objects from given file.
     * \param file Full path to input file
     */
    void DeserializeFromXml(const wxString& file);
    /*!
     * \brief Deserialize objects from given stream.
     * \param instream Input stream
     */
    void DeserializeFromXml(wxInputStream& instream);

    /*!
     * \brief Serialize child objects of given parent object (parent object can be optionaly
     * serialized as well) to given XML node. The function can be overriden if necessary.
     * \param parent Pointer to parent serializable object
     * \param node Pointer to output XML node
     * \param withparent TRUE if the parent object should be serialized as well
     */
    virtual void SerializeObjects(xsSerializable* parent, wxXmlNode* node, bool withparent);
    /*!
     * \brief Deserialize child objects of given parent object from given XML node.
     * The function can be overriden if necessary.
     * \param parent Pointer to parent serializable object
     * \param node Pointer to input XML node
     */
    virtual void DeserializeObjects(xsSerializable* parent, wxXmlNode* node);

	/*!
	 * \brief Get the lowest free object ID
	 */
	long GetNewId();
	/*!
	 * \brief Find out whether given object ID is already used.
	 * \param id Object ID
	 * \return TRUE if the object ID is used, otherwise FALSE
	 */
	bool IsIdUsed(long id);
	/*!
	 * \brief Get number of occurences of given ID.
	 * \param id Object ID
	 * \return Number of ID's occurences
	 */
	int GetIDCount(long id);

protected:
    // protected data members
    /*! \brief Owner name */
    wxString m_sOwner;
    /*! \brief Root node name */
    wxString m_sRootName;
    /*! \brief File version */
    wxString m_sVersion;

    /*! \brief Pointer to root object */
    xsSerializable* m_pRoot;

private:
    // private data members
    int m_nCounter;

    // private functions
    /*! \brief Auxiliary function */
    xsSerializable* _GetItem(long id, xsSerializable* parent);
    /*! \brief Auxiliary function */
    void _GetItems(wxClassInfo* type, xsSerializable* parent, SerializableList& list);

};

#endif // XMLSERIALIZE_H
