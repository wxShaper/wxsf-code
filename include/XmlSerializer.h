#ifndef XMLSERIALIZE_H
#define XMLSERIALIZE_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/list.h>
#include <wx/xml/xml.h>

WX_DECLARE_OBJARRAY(wxRealPoint, RealPointArray);
WX_DECLARE_LIST(wxRealPoint, RealPointList);

#define XS_SERIALIZE_STRING(x, name) wxASSERT_MSG(wxVariant(x).GetType()==wxT("string"), wxT("Variable is not wxString"));XS_SERIALIZE_PROPERTY(x, wxT("string"), name);
#define XS_SERIALIZE_STRING_EX(x, name, def) wxASSERT_MSG(wxVariant(x).GetType()==wxT("string"), wxT("Variable is not wxString"));XS_SERIALIZE_PROPERTY_EX(x, wxT("string"), name, def);
#define XS_SERIALIZE_LONG(x, name) wxASSERT_MSG(wxVariant(x).GetType()==wxT("long"), wxT("Variable is not LONG"));XS_SERIALIZE_PROPERTY(x, wxT("long"), name);
#define XS_SERIALIZE_LONG_EX(x, name, def) wxASSERT_MSG(wxVariant(x).GetType()==wxT("long"), wxT("Variable is not LONG"));XS_SERIALIZE_PROPERTY_EX(x, wxT("long"), name, def);
#define XS_SERIALIZE_DOUBLE(x, name) wxASSERT_MSG(wxVariant(x).GetType()==wxT("double"), wxT("Variable is not DOUBLE"));XS_SERIALIZE_PROPERTY(x, wxT("double"), name);
#define XS_SERIALIZE_DOUBLE_EX(x, name, def) wxASSERT_MSG(wxVariant(x).GetType()==wxT("double"), wxT("Variable is not DOUBLE"));XS_SERIALIZE_PROPERTY_EX(x, wxT("double"), name, def);
#define XS_SERIALIZE_BOOL(x, name) wxASSERT_MSG(wxVariant(x).GetType()==wxT("bool"), wxT("Variable is not BOOL"));XS_SERIALIZE_PROPERTY(x, wxT("bool"), name);
#define XS_SERIALIZE_BOOL_EX(x, name, def) wxASSERT_MSG(wxVariant(x).GetType()==wxT("bool"), wxT("Variable is not BOOL"));XS_SERIALIZE_PROPERTY_EX(x, wxT("bool"), name, def);

#define XS_SERIALIZE_POINT(x, name) XS_SERIALIZE_PROPERTY(x, wxT("point"), name);
#define XS_SERIALIZE_POINT_EX(x, name, def) XS_SERIALIZE_PROPERTY_EX(x, wxT("point"), name, def);
#define XS_SERIALIZE_REALPOINT(x, name) XS_SERIALIZE_PROPERTY(x, wxT("realpoint"), name);
#define XS_SERIALIZE_REALPOINT_EX(x, name, def) XS_SERIALIZE_PROPERTY_EX(x, wxT("realpoint"), name, def);
#define XS_SERIALIZE_SIZE(x, name) XS_SERIALIZE_PROPERTY(x, wxT("size"), name);
#define XS_SERIALIZE_SIZE_EX(x, name, def) XS_SERIALIZE_PROPERTY_EX(x, wxT("size"), name, def);

#define XS_SERIALIZE_COLOUR(x, name) wxASSERT_MSG(x.IsKindOf(CLASSINFO(wxColour)), wxT("Object is not wxColour"));XS_SERIALIZE_PROPERTY(x, wxT("colour"), name);
#define XS_SERIALIZE_COLOUR_EX(x, name, def) wxASSERT_MSG(x.IsKindOf(CLASSINFO(wxColour)), wxT("Object is not wxColour"));XS_SERIALIZE_PROPERTY_EX(x, wxT("colour"), name, def);
#define XS_SERIALIZE_PEN(x, name) wxASSERT_MSG(x.IsKindOf(CLASSINFO(wxPen)), wxT("Object is not wxPen"));XS_SERIALIZE_PROPERTY(x, wxT("pen"), name);
#define XS_SERIALIZE_PEN_EX(x, name, def) wxASSERT_MSG(x.IsKindOf(CLASSINFO(wxPen)), wxT("Object is not wxPen"));XS_SERIALIZE_PROPERTY_EX(x, wxT("pen"), name, def);
#define XS_SERIALIZE_BRUSH(x, name) wxASSERT_MSG(x.IsKindOf(CLASSINFO(wxBrush)), wxT("Object is not wxBrush"));XS_SERIALIZE_PROPERTY(x, wxT("brush"), name);
#define XS_SERIALIZE_BRUSH_EX(x, name, def) wxASSERT_MSG(x.IsKindOf(CLASSINFO(wxBrush)), wxT("Object is not wxBrush"));XS_SERIALIZE_PROPERTY_EX(x, wxT("brush"), name, def);
#define XS_SERIALIZE_FONT(x, name) wxASSERT_MSG(x.IsKindOf(CLASSINFO(wxFont)), wxT("Object is not wxFont"));XS_SERIALIZE_PROPERTY(x, wxT("font"), name);
#define XS_SERIALIZE_FONT_EX(x, name, def) wxASSERT_MSG(x.IsKindOf(CLASSINFO(wxFont)), wxT("Object is not wxFont"));XS_SERIALIZE_PROPERTY_EX(x, wxT("font"), name, def);

#define XS_SERIALIZE_ARRAYSTRING(x, name) XS_SERIALIZE_PROPERTY(x, wxT("arraystring"), name);
#define XS_SERIALIZE_ARRAYREALPOINT(x, name) XS_SERIALIZE_PROPERTY(x, wxT("arrayrealpoint"), name);
#define XS_SERIALIZE_LISTREALPOINT(x, name) XS_SERIALIZE_PROPERTY(x, wxT("listrealpoint"), name);

#define XS_SERIALIZE_DYNAMIC_OBJECT(x, name) XS_SERIALIZE_PROPERTY(x, wxT("serializabledynamic"), name);
#define XS_SERIALIZE_STATIC_OBJECT(x, name) wxASSERT_MSG(x.IsKindOf(CLASSINFO(xsSerializable)), wxT("Object is not xsSerializable"));XS_SERIALIZE_PROPERTY(x, wxT("serializablestatic"), name);

#define XS_SERIALIZE_PROPERTY(x, type, name) AddProperty(new xsProperty(&x, type, name));
#define XS_SERIALIZE_PROPERTY_EX(x, type, name, def) AddProperty(new xsProperty(&x, type, name, def));

class xsProperty : public wxObject
{
public:
    DECLARE_DYNAMIC_CLASS(xsProperty);

    xsProperty()
    {
        m_pSourceVariable = NULL;
        m_sDataType = wxT("Undefined");
        m_sFieldName = wxT("Undefined");
        m_sDefaultValueStr = wxT("");
        m_fSerialize = false;
    }

    xsProperty(void* src, const wxString& type, const wxString& field, const wxString& def = wxT(""))
    {
        m_pSourceVariable = src;
        m_sDataType = type;
        m_sFieldName = field;
        m_sDefaultValueStr = def;
        m_fSerialize = true;
    }
    ~xsProperty(){;}

    // public data members
    void* m_pSourceVariable;
    wxString m_sDataType;
    wxString m_sFieldName;
    wxString m_sDefaultValueStr;
    bool m_fSerialize;
};

class xsSerializable;

WX_DECLARE_LIST(xsProperty, PropertyList);
WX_DECLARE_LIST(xsSerializable, SerializableList);

class xsSerializable : public wxObject
{
public:
    DECLARE_DYNAMIC_CLASS(xsSerializable);

    xsSerializable();
    ~xsSerializable();

    // public functions
    xsSerializable* GetParent(){return m_pParentItem;}
    xsSerializable* GetFirstChild();
    xsSerializable* GetLastChild();
    xsSerializable* GetSibbling();

    SerializableList& GetChildren(){return m_lstChildItems;}
    wxSerializableListNode* GetFirstChildNode(){return m_lstChildItems.GetFirst();}
    wxSerializableListNode* GetLastChildNode(){return m_lstChildItems.GetLast();}

    void SetParent(xsSerializable* parent){m_pParentItem = parent;}
    void AddChild(xsSerializable* child);
    void Reparent(xsSerializable* parent);

    void SetId(long id){m_nId = id;}
    long GetId(){return m_nId;}

    wxXmlNode* SerializeObject(wxXmlNode* node);
    void DeserializeObject(wxXmlNode* node);

    void AddProperty(xsProperty* property);
    xsProperty* GetProperty(const wxString& field);

    void EnablePropertySerialization(const wxString& field, bool enab);
    bool IsPropertySerialized(const wxString& field);

    static wxColour StringToColour(const wxString& val);
    static wxRealPoint StringToRealPoint(const wxString& val);
    static wxPoint StringToPoint(const wxString& val);
    static wxSize StringToSize(const wxString& val);
    static wxBrush StringToBrush(const wxString& val);
    static wxPen StringToPen(const wxString& val);
    static wxFont StringToFont(const wxString& val);
    static long StringToLong(const wxString& val);
    static double StringToDouble(const wxString& val);
    static bool StringToBool(const wxString& val);

    static wxString ColourToString(const wxColour& val);
    static wxString RealPointToString(const wxRealPoint& val);
    static wxString PointToString(const wxPoint& val);
    static wxString SizeToString(const wxSize& val);
    static wxString BrushToString(const wxBrush& val);
    static wxString PenToString(const wxPen& val);
    static wxString FontToString(const wxFont& val);
    static wxString LongToString(long val);
    static wxString DoubleToString(double val);
    static wxString BoolToString(bool val);

    static wxXmlNode* AddPropertyNode(wxXmlNode* parent, const wxString& name, const wxString& value);

protected:
    // protected data members
    PropertyList m_lstProperties;
    SerializableList m_lstChildItems;

    xsSerializable* m_pParentItem;

    long m_nId;

    // protected functions
    wxString ConvertToString(xsProperty* property);
    void FillFromString(const wxString& source, xsProperty* property);

    // protected virtual functions
    virtual wxXmlNode* Serialize(wxXmlNode* node);
    virtual void Deserialize(wxXmlNode* node);
};

class wxXmlSerializer : public wxObject
{
public:
    DECLARE_DYNAMIC_CLASS(wxXmlSerializer);

    wxXmlSerializer();
    wxXmlSerializer(const wxString& owner, const wxString& root, const wxString& version);
    virtual ~wxXmlSerializer();

    // public member data accessors
    void SetSerializerOwner(const wxString& name){m_sOwner = name;}
    void SetSerializerRootName(const wxString& name){m_sRootName = name;}
    void SetSerializerVersion(const wxString& name){m_sVersion = name;}
    wxString GetSerializerOwner(){return m_sOwner;}
    wxString GetSerializerRootName(){return m_sRootName;}
    wxString GetSerializerVersion(){return m_sVersion;}

    // public functions
    xsSerializable* GetRootItem(){return m_pRoot;}
    xsSerializable* GetItem(long id);
    int GetItems(wxClassInfo* type, SerializableList& list);

    void SetRootItem(xsSerializable* root);

    void AddItem(long parentId, xsSerializable* item);
    void AddItem(xsSerializable* parent, xsSerializable* item);
    void RemoveItem(long id);
    void RemoveItem(xsSerializable* item);
    void RemoveAll();

    void SerializeToXml(const wxString& file);
    void SerializeToXml(wxOutputStream& outstream);
    void DeserializeFromXml(const wxString& file);
    void DeserializeFromXml(wxInputStream& instream);

    virtual void SerializeObjects(xsSerializable* parent, wxXmlNode* node, bool withparent);
    virtual void DeserializeObjects(xsSerializable* parent, wxXmlNode* node);

	/*!
	 * \brief Get the lowest free shape ID
	 */
	long GetNewId();
	/*!
	 * \brief Find out whether given shape ID is already used.
	 * \param id Shape ID
	 * \return TRUE if the shape ID is used, otherwise FALSE
	 */
	bool IsIdUsed(long id);
	/*!
	 * \brief Get number of occurences of given ID.
	 * \param id Shape ID
	 * \return Number of ID's occurences
	 */
	int GetIDCount(long id);

protected:
    // protected data members
    wxString m_sOwner;
    wxString m_sRootName;
    wxString m_sVersion;

    xsSerializable* m_pRoot;

private:
    // private data members
    int m_nCounter;

    // private functions
    xsSerializable* _GetItem(long id, xsSerializable* parent);
    void _GetItems(wxClassInfo* type, xsSerializable* parent, SerializableList& list);

};

#endif // XMLSERIALIZE_H
