/***************************************************************
 * Name:      ShapeBase.h
 * Purpose:   Defines shape base class
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#pragma once

#include <wx/wxprec.h>

#include <wx/tokenzr.h>
#include <wx/xml/xml.h>
#include <wx/arrstr.h>
#include <wx/list.h>

#include "ShapeHandle.h"
#include "ScaledPaintDC.h"
#include "Defs.h"
#include "../wxxmlserializer/XmlSerializer.h"

#define sfRECURSIVE true
#define sfNORECURSIVE false
#define sfDIRECT true
#define sfINDIRECT false
#define sfWITHCHILDREN true
#define sfWITHOUTCHILDREN false

// default values
/// <summary> Default value of wxSFShapeObject::m_fVisible data member </summary>
#define sfdvBASESHAPE_VISIBILITY true
/// <summary> Default value of wxSFShapeObject::m_fActive data member </summary>
#define sfdvBASESHAPE_ACTIVITY true
/// <summary> Default value of wxSFShapeObject::m_nHoverColor data member </summary>
#define sfdvBASESHAPE_HOVERCOLOUR wxColor(120, 120, 255)
/// <summary> Default value of wxSFShapeObject::m_nRelativePosition data member </summary>
#define sfdvBASESHAPE_POSITION wxRealPoint(0, 0)
/// <summary> Default value of wxSFShapeObject::m_nVAlign data member </summary>
#define sfdvBASESHAPE_VALIGN valignNONE
/// <summary> Default value of wxSFShapeObject::m_nHAlign data member </summary>
#define sfdvBASESHAPE_HALIGN halignNONE
/// <summary> Default value of wxSFShapeObject::m_nVBorder data member </summary>
#define sfdvBASESHAPE_VBORDER 0
/// <summary> Default value of wxSFShapeObject::m_nHBorder data member </summary>
#define sfdvBASESHAPE_HBORDER 0
/// <summary> Default value of wxSFShapeObject::m_nStyle data member </summary>
#define sfdvBASESHAPE_DEFAULT_STYLE sfsDEFAULT_SHAPE_STYLE

class WXDLLIMPEXP_SF wxSFShapeCanvas;
class WXDLLIMPEXP_SF wxSFDiagramManager;
class WXDLLIMPEXP_SF wxSFShapeBase;

WX_DECLARE_LIST_WITH_DECL(wxSFShapeBase, ShapeList, class WXDLLIMPEXP_SF);

/// <summary>
/// Base class for all shapes providing fundamental functionality and publishing set
/// of virtual functions which must be defined by the user in derived shapes. This class
/// shouldn't be used as it is.
///
/// Shape objects derived from this class use hierarchical approach. It means that every
/// shape must have defined parent shape (can be NULL for topmost shapes). An absolute
/// shape position is then calculated as a sumation of all relative positions of all parent
/// shapes. Also the size of the parent shape can be limited be a boundind box of all
/// children shapes.
///
/// This class also declares set of virtual functions used as event handlers for various
/// events (moving, sizing, drawing, mouse events, serialization and deserialization requests, ...)
/// mostly triggered by a parent shape canvas.
/// </summary>
class WXDLLIMPEXP_SF wxSFShapeBase : public xsSerializable
{
public:

    friend class wxSFShapeCanvas;
    friend class wxSFShapeHandle;

	XS_DECLARE_CLONABLE_CLASS(wxSFShapeBase);

    /// <summary> Bit flags for wxSFShapeBase::GetCompleteBoundingBox function </summary>
    enum BBMODE
    {
		bbSELF = 1,
        bbCHILDREN = 2,
        bbCONNECTIONS = 4,
        bbALL = 7
    };

    /*! \brief Search mode flags for GetAssignedConnections function */
	enum CONNECTMODE
	{
	    /*! \brief Search for connection starting in examined shape */
	    lineSTARTING,
	    /*! \brief Search for connection ending in examined shape */
	    lineENDING,
	    /*! \brief Search for both starting and ending connections */
	    lineBOTH
	};

    /*! \brief Flags for SetVAlign function */
	enum VALIGN
	{
	    valignNONE,
	    valignTOP,
	    valignMIDDLE,
	    valignBOTTOM
	};

    /*! \brief Flags for SetHAlign function */
	enum HALIGN
	{
	    halignNONE,
	    halignLEFT,
	    halignCENTER,
	    halignRIGHT
	};

    /*! \brief Basic shape's styles used with SetStyle() function */
	enum STYLE
	{
	    /*! \brief Interactive parent change is allowed */
	    sfsPARENT_CHANGE = 1,
	    /*! \brief Interactive position change is allowed */
	    sfsPOSITION_CHANGE = 2,
	    /*! \brief Interactive size change is allowed */
	    sfsSIZE_CHANGE = 4,
	    /*! \brief Shape is highlighted at mouse hovering */
	    sfsHOVERING = 8,
	    /*! \brief Shape is highlighted at shape dragging */
	    sfsHIGHLIGHTING = 16,
	    /*! \brief Shape is always inside its parent */
	    sfsALWAYS_INSIDE = 32,
	    /*! \brief User data is destroyed at the shape deletion */
	    sfsDELETE_USER_DATA = 64,
	    /*! \brief The DEL key is processed by the shape (not by the shape canvas) */
	    sfsPROCESS_DEL = 128,
	    /*! \brief Show handles if the shape is selected */
	    sfsSHOW_HANDLES = 256,
		/*! \brief Default shape style. */
		sfsDEFAULT_SHAPE_STYLE = sfsPARENT_CHANGE | sfsPOSITION_CHANGE | sfsSIZE_CHANGE | sfsHOVERING | sfsHIGHLIGHTING | sfsSHOW_HANDLES | sfsALWAYS_INSIDE | sfsDELETE_USER_DATA
	};

    /// <summary> Default constructor </summary>
	wxSFShapeBase(void);
	/// <summary> User constructor </summary>
	/// <param name="pos"> Initial relative position </param>
	/// <param name="manager"> Pointer to parent diagram manager </param>
	wxSFShapeBase(const wxRealPoint& pos, wxSFDiagramManager* manager);
	/// <summary> Copy constructor </summary>
	/// <param name="obj"> Reference to the source object </param>
	wxSFShapeBase(const wxSFShapeBase& obj);
	/// <summary> Destructor </summary>
	virtual ~wxSFShapeBase(void);

	// public functions

    /// <summary> Refresh (redraw) the shape </summary>
	void Refresh();
	/// <summary> Draw shape. Default implementation tests basic shape visual states
	/// (normal/ready, mouse is over the shape, dragged shape can be accepted) and
	/// call appropriate virtual functions (DrawNormal, DrawHover, DrawHighlighted)
	/// for its visualisation. The function can be overrided if neccessary. </summary>
	/// <param name="dc"> Reference to a device context where the shape will be drawn to </param>
	/// <param name="children"> TRUE if the shape's children should be drawn as well </param>
	virtual void Draw(wxSFScaledPaintDC& dc, bool children = sfWITHCHILDREN);
    /// <summary> Test whether the given point is inside the shape. The function
    /// can be overrided if neccessary. </summary>
    /// <param name="pos"> Examined point </param>
    /// <returns> TRUE if the point is inside the shape area, otherwise FALSE </returns>
	virtual bool IsInside(const wxPoint& pos);
	/// <summary> Test whether the given rectangle intersects the shape. </summary>
	/// <param name="rct"> Examined rectangle </param>
	/// <returns> TRUE if the examined rectangle intersects the shape, otherwise FALSE </returns>
	virtual bool Intersects(const wxRect& rct);
	/// <summary> Get the shape's absolute position in the canvas (calculated as a sumation
	/// of all relative positions in the shapes' hierarchy. The function can be overrided if neccessary. </summary>
	/// <returns> Shape's position </returns>
	virtual wxRealPoint GetAbsolutePosition();
	/// <summary> Get intersection point of the shape border and a line leading from
	/// the shape's center to the given point.  Default implementation does nothing. The function can be overrided if neccessary. </summary>
	/// <param name="to"> Ending point of the virtual intersection line </param>
	/// <returns> Intersection point </returns>
	virtual wxRealPoint GetBorderPoint(const wxRealPoint& to);
	/// <summary> Get shape's center.  Default implementation does nothing. The function can be overrided if neccessary. </summary>
	/// <returns> Center point </returns>
	virtual wxRealPoint GetCenter();

	/// <summary> Function called by the framework responsible for creation of shape handles
    /// at the creation time. Default implementation does nothing. The function can be overrided if neccesary </summary>
	virtual void CreateHandles();
	/// <summary> Show/hide shape handles. Hidden handles are inactive. </summary>
	/// <param name="show"> TRUE for showing, FALSE for hidding </param>
	void ShowHandles(bool show);

    /*!
     * \brief Set shape's style.
	 *
	 * Default value is sfsPARENT_CHANGE | sfsPOSITION_CHANGE | sfsSIZE_CHANGE | sfsHOVERING | sfsHIGHLIGHTING | sfsSHOW_HANDLES | sfsALWAYS_INSIDE | sfsDELETE_USER_DATA
     * \param style Combination of the shape's styles
     * \sa STYLE
     */
    inline void SetStyle(long style){m_nStyle = style;}
    /*! \brief Get current shape style. */
    inline long GetStyle(){return m_nStyle;}
    inline void AddStyle(STYLE style){m_nStyle |= style;}
    inline void RemoveStyle(STYLE style){m_nStyle &= ~style;}
    inline bool ContainsStyle(STYLE style){return (m_nStyle & style) != 0;}


    /// <summary> Get child shapes associated with this (parent) shape. </summary>
    /// <param name="children"> List of child shapes </param>
    /// <param name="recursive"> Set this flag TRUE if also children of children of ... should be found
    /// (also sfRECURSIVE a sfNORECURSIVE constants can be used). </param>
	void GetChildren(ShapeList& children, bool recursive = false);
	/*!
	 * \brief Get neighbour shapes connected to this shape.
	 * \param neighbours List of neighbour shapes
	 * \param shapeInfo Line object type
	 * \param condir Connection direction
	 * \param direct Set this flag to TRUE if only closest shapes should be found,
	 * otherwise also shapes connected by forked lines will be found (also
	 * constants sfDIRECT and sfINDIRECT can be used)
	 * \sa CONNECTMODE
	 */
	void GetNeighbours(ShapeList& neighbours, wxClassInfo* shapeInfo, CONNECTMODE condir, bool direct = true);

    /// <summary> Get shapes's bounding box. The function can be overrided
    /// if neccessary. </summary>
    /// <returns> Bounding rectangle </returns>
	virtual wxRect GetBoundingBox();
	/// <summary> Get shape's bounding box which includes also associated child shapes and connections </summary>
	/// <param name="rct"> Returned bounding rectangle </param>
	/// <param name="mask"> Bit mask of object types which should be included into calculation </param>
	/// <seealso cref="BBMODE"></seealso>
	void GetCompleteBoundingBox(wxRect& rct, int mask = bbALL);

    /// <summary> Scale the shape size by in both directions. The function can be overrided if necessary
    /// (new implementation should call default one ore scale shape's children manualy if neccesary). </summary>
    /// <param name="x"> Horizontal scale factor </param>
    /// <param name="y"> Vertical scale factor </param>
    /// <param name="children"> TRUE if the shape's children shoould be scaled as well, otherwise
    /// the shape will be updated after scaling via Update() function. </param>
	virtual void Scale(double x, double y, bool children = sfWITHCHILDREN);
    /// <summary> Scale the shape size by in both directions. </summary>
    /// <param name="scale"> Scaling factor </param>
    /// <param name="children"> TRUE if the shape's children shoould be scaled as well, otherwise
    /// the shape will be updated after scaling via Update() function. </param>
	void Scale(const wxRealPoint& scale, bool children = sfWITHCHILDREN);
    /*!
     * \brief Scale shape's children
     * \param x Horizontal scale factor
     * \param y Vertical scale factor
     * \sa Scale
     */
	void ScaleChildren(double x, double y);
	/// <summary> Move the shape to the given absolute position. The function can be overrided if necessary. </summary>
	/// <param name="x"> X coordinate </param>
	/// <param name="y"> Y coordinate </param>
	virtual void MoveTo(double x, double y);
	/// <summary> Move the shape to the given absolute position. </summary>
	/// <param name="pos"> New absolute position </param>
	void MoveTo(const wxRealPoint& pos);
	/// <summary> Move the shape by the given offset. The function
    /// can be overrided if neccessary. </summary>
	/// <param name="x"> X offset </param>
	/// <param name="y"> Y offset </param>
	virtual void MoveBy(double x, double y);
	/// <summary> Move the shape by the given offset. </summary>
	/// <param name="delta"> Offset </param>
	void MoveBy(const wxRealPoint& delta);

	/*! \brief Update shape position in order to its alignment */
	void DoAlignment();
	/*! \brief Upate shape (align all child shapes an resize it to fit them) */
	virtual void Update();
	/*! \brief Resize the shape to bound all child shapes. The function can be overrided if neccessary. */
	virtual void FitToChildren();

	// public member data accessors
	/// <summary> Function returns TRUE if the shape is selected, otherwise returns FALSE. </summary>
	bool IsSelected(){return m_fSelected;}
	/// <summary> Set the shape as a selected/deselected one </summary>
	/// <param name="state"> Selection state (TRUE is selected, FALSE is deselected) </param>
	void Select(bool state){m_fSelected = state; ShowHandles(state && (m_nStyle & sfsSHOW_HANDLES));}

    /// <summary> Set shape's relative position. Absolute shape's position is then calculated
    /// as a sumation of the relative positions of this shape and all parent shapes in the shape's
    /// hierarchy. </summary>
    /// <param name="pos"> New relative position </param>
    /// <seealso cref="MoveTo"></seealso>
	void SetRelativePosition(const wxRealPoint& pos){m_nRelativePosition = pos;}
    /// <summary> Set shape's relative position. Absolute shape's position is then calculated
    /// as a sumation of the relative positions of this shape and all parent shapes in the shape's
    /// hierarchy. </summary>
    /// <param name="x"> Horizontal coordinate of new relative position </param>
    /// <param name="y"> Vertical coordinate of new relative position </param>
    /// <seealso cref="MoveTo"></seealso>
	void SetRelativePosition(double x, double y){m_nRelativePosition.x = x; m_nRelativePosition.y = y;}
    /// <summary> Get shape's relative position </summary>
    /// <returns> Current relative position </returns>
    /// <seealso cref="GetAbsolutePosition"></seealso>
	wxRealPoint GetRelativePosition() const {return m_nRelativePosition;}
	/*!
	 * \brief Set vertical alignment of this shape inside its parent
	 * \param val Alignment type
	 * \sa VALIGN
	 */
	void SetVAlign(VALIGN val){m_nVAlign = val;}
	/*!
	 * \brief Get vertical alignment of this shape inside its parent
	 * \return Alignment type
	 * \sa VALIGN
	 */
	VALIGN GetVAlign(){return m_nVAlign;}
	/*!
	 * \brief Set horizontal alignment of this shape inside its parent
	 * \param val Horizontal type
	 * \sa HALIGN
	 */
	void SetHAlign(HALIGN val){m_nHAlign = val;}
	/*!
	 * \brief Get horizontal alignment of this shape inside its parent
	 * \return Alignment type
	 * \sa VALIGN
	 */
	HALIGN GetHAlign(){return m_nHAlign;}
	/*!
	 * \brief Set vertical border between this shape and its parent (is vertical
	 * alignment is set).
	 * \param border Vertical border
	 * \sa SetVAlign
	 */
	void SetVBorder(double border){m_nVBorder = border;}
	/*!
	 * \brief Get vertical border between this shape and its parent (is vertical
	 * alignment is set).
	 * \return Vertical border
	 * \sa SetVAlign
	 */
	double GetVBorder(){return m_nVBorder;}
	/*!
	 * \brief Set horizontal border between this shape and its parent (is horizontal
	 * alignment is set).
	 * \param border Horizontal border
	 * \sa SetVAlign
	 */
	void SetHBorder(double border){m_nHBorder = border;}
	/*!
	 * \brief Get horizontal border between this shape and its parent (is horizontal
	 * alignment is set).
	 * \return Vertical border
	 * \sa SetHAlign
	 */
	double GetHBorder(){return m_nHBorder;}

    /// <summary> Get pointer to a parent shape </summary>
	wxSFShapeBase* GetParentShape();
    /// <summary> Get pointer to the topmost parent shape </summary>
	wxSFShapeBase* GetGrandParentShape();

    /*!
     * \brief Associate user data with the shape.
     *
     * If the data object is properly set then its marked properties will be serialized
     * together with the parent shape.
     * \param data Pointer to user data
     */
    void SetUserData(xsSerializable* data);//{m_pUserData = data;}
     /*!
     * \brief Get associated user data.
     *
     * \return Pointer to user data
     */
    xsSerializable* GetUserData(){return m_pUserData;}

	/*!
	 * \brief Get shape's parent diagram manager.
	 * \return Pointer to diagram manager
	 * \sa wxSFDiagramManager
	 */
	wxSFDiagramManager* GetShapeManager(){return (wxSFDiagramManager*)m_pParentManager;}
	/*!
	 * \brief Get shape's parent canvas
	 * \return Pointer to shape canvas if assigned via diagram manager, otherwise NULL
	 * \sa wxSFDiagramManager
	 */
	wxSFShapeCanvas* GetParentCanvas();
	/*!
	 * \brief Get the shape's visibility status
	 * \return TRUE if the shape is visible, otherwise FALSE
	 */
	bool IsVisible(){return m_fVisible;}
	/*!
	 * \brief Show/hide shape
	 * \param show Set the parameter to TRUE if the shape should be visible, otherwise use FALSE
	 */
	void Show(bool show){m_fVisible = show;}
	/*!
	 * \brief Set shape's hover color
	 * \param col Hover color
	 */
	void SetHoverColour(const wxColour& col){m_nHoverColor = col;}
	/*!
	 * \brief Get shape's hover color
	 * \return Current hover color
	 */
	wxColour GetHoverColour() const {return m_nHoverColor;}
	/*!
	 * \brief Function returns value of a shape's activation flag.
	 *
	 * Non-active shapes are visible, but don't receive (process) any events.
	 * \return TRUE if the shape is active, othervise FALSE
	 */
	bool IsActive() {return m_fActive;}
	/*!
	 * \brief Shape's activation/deactivation
	 *
	 * Deactivated shapes are visible, but don't receive (process) any events.
	 * \param active TRUE for activation, FALSE for deactivation
	 * \return Description
	 * \sa Show
	 */
	void Activate(bool active) {m_fActive = active;}

    /*!
     * \brief Tells whether the given shape type is accepted by this shape (it means
     * whether this shape can be its parent).
     *
     * The function is typically used by the framework for determination whether a dropped
     * shape can be assigned to an underlying shape as its child.
     * \param type Class name of examined shape object
     * \return TRUE if the shape type is accepted, otherwise FALSE.
     */
    bool IsChildAccepted(const wxString& type);
    /*!
     * \brief Function returns TRUE if all currently dragged shapes can be accepted
     * as children of this shape.
     * \sa IsShapeAccepted
     */
    bool AcceptCurrentlyDraggedShapes();
    /*!
     * \brief Add given shape type to an acceptance list. The acceptance list contains class
     * names of the shapes which can be accepted as children of this shape.
     * Note: Keyword 'All' behaves like any class name.
     * \param type Class name of accepted shape object
     * \sa IsChildAccepted
     */
	void AcceptChild(const wxString& type) {m_arrAcceptedChildren.Add(type);}
	/*!
	 * \brief Get shape types acceptance list.
	 * \return String array with class names of accepted shape types.
	 * \sa IsChildAccepted
	 */
	wxArrayString& GetAcceptedChildren() {return m_arrAcceptedChildren;}
    /*!
     * \brief Tells whether the given connection type is accepted by this shape (it means
     * whether this shape can be connected to another one by a connection of given type).
     *
     * The function is typically used by the framework during interactive connection creation.
     * \param type Class name of examined connection object
     * \return TRUE if the connection type is accepted, otherwise FALSE.
     */
	bool IsConnectionAccepted(const wxString& type);
    /*!
     * \brief Add given connection type to an acceptance list. The acceptance list contains class
     * names of the connection which can be accepted by this shape.
     * Note: Keyword 'All' behaves like any class name.
     * \param type Class name of accepted connection object
     * \sa IsConnectionAccepted
     */
	void AcceptConnection(const wxString& type) {m_arrAcceptedConnections.Add(type);}
	/*!
	 * \brief Get connection types acceptance list.
	 * \return String array with class names of accepted connection types.
	 * \sa IsConnectionAccepted
	 */
	wxArrayString& GetAcceptedConnections() {return m_arrAcceptedConnections;}
    /*!
     * \brief Tells whether the given shape type is accepted by this shape as its source neighbour(it means
     * whether this shape can be connected from another one of given type).
     *
     * The function is typically used by the framework during interactive connection creation.
     * \param type Class name of examined connection object
     * \return TRUE if the shape type is accepted, otherwise FALSE.
     */
	bool IsSrcNeighbourAccepted(const wxString& type);
    /*!
     * \brief Add given shape type to an source neighbours' acceptance list. The acceptance list contains class
     * names of the shape types which can be accepted by this shape as its source neighbour.
     * Note: Keyword 'All' behaves like any class name.
     * \param type Class name of accepted connection object
     * \sa IsSrcNeighbourAccepted
     */
	void AcceptSrcNeighbour(const wxString& type) {m_arrAcceptedSrcNeighbours.Add(type);}
	/*!
	 * \brief Get source neighbour types acceptance list.
	 * \return String array with class names of accepted source neighbours types.
	 * \sa IsSrcNeighbourAccepted
	 */
	wxArrayString& GetAcceptedSrcNeighbours() {return m_arrAcceptedSrcNeighbours;}
    /*!
     * \brief Tells whether the given shape type is accepted by this shape as its target neighbour(it means
     * whether this shape can be connected to another one of given type).
     *
     * The function is typically used by the framework during interactive connection creation.
     * \param type Class name of examined connection object
     * \return TRUE if the shape type is accepted, otherwise FALSE.
     */
	bool IsTrgNeighbourAccepted(const wxString& type);
    /*!
     * \brief Add given shape type to an target neighbours' acceptance list. The acceptance list contains class
     * names of the shape types which can be accepted by this shape as its target neighbour.
     * Note: Keyword 'All' behaves like any class name.
     * \param type Class name of accepted connection object
     * \sa IsTrgNeighbourAccepted
     */
	void AcceptTrgNeighbour(const wxString& type) {m_arrAcceptedTrgNeighbours.Add(type);}
	/*!
	 * \brief Get target neighbour types acceptance list.
	 * \return String array with class names of accepted target neighbours types.
	 * \sa IsTrgNeighbourAccepted
	 */
	wxArrayString& GetAcceptedTrgNeighbours() {return m_arrAcceptedTrgNeighbours;}
	/*!
	 * \brief Clear shape object acceptance list
	 * \sa AcceptChild
	 */
	void ClearAcceptedChilds(){m_arrAcceptedChildren.Clear();}
	/*!
	 * \brief Clear connection object acceptance list
	 * \sa AcceptConnection
	 */
	void ClearAcceptedConnections(){m_arrAcceptedConnections.Clear();}
	/*!
	 * \brief Clear source neighbour objects acceptance list
	 * \sa AcceptSrcNeighbour
	 */
	void ClearAcceptedSrcNeighbours(){m_arrAcceptedSrcNeighbours.Clear();}
	/*!
	 * \brief Clear target neighbour objects acceptance list
	 * \sa AcceptTrgNeighbour
	 */
	void ClearAcceptedTrgNeighbours(){m_arrAcceptedTrgNeighbours.Clear();}

    /*!
     * \brief Get list of currently assigned shape handles.
     * \return Reference to the handle list
     * \sa CHandleList
     */
	HandleList& GetHandles() {return m_lstHandles;}
	/*!
	 * \brief Get shape handle.
	 * \param type Handle type
	 * \param id Handle ID (usefull only for line control points)
	 * \return Pointer to the shape handle object
	 * \sa wxSFShapeHandle
	 */
	wxSFShapeHandle* GetHandle(wxSFShapeHandle::HANDLETYPE type, long id = -1);
	/*!
	 * \brief Add new handle to the shape.
	 *
	 * The function creates new instance of shape handle (if it doesn't exist yet)
	 * and inserts it into handle list.
	 * \param type Handle type
	 * \param id Handle ID (usefull only for line control points)
	 * \sa wxSFShapeHandle
	 */
	void AddHandle(wxSFShapeHandle::HANDLETYPE type, long id = -1);
	/*!
	 * \brief Remove given shape handle (if exists).
	 * \param type Handle type
	 * \param id Handle ID (usefull only for line control points)
	 * \sa wxSFShapeHandle
	 */
	void RemoveHandle(wxSFShapeHandle::HANDLETYPE type, long id = -1);

	// public event handlers
	/*!
	 * \brief Event handler called when the shape is clicked by
	 * the left mouse button. The function can be overrided if necessary.
	 *
	 * The function is called by the framework (by the shape canvas).
	 * Default implementation does nothing.
	 * \param pos Current mouse position
	 * \sa wxSFShapeCanvas
	 */
	virtual void OnLeftClick(const wxPoint& pos);
	/*!
	 * \brief Event handler called when the shape is clicked by
	 * the right mouse button. The function can be overrided if necessary.
	 *
	 * The function is called by the framework (by the shape canvas).
	 * Default implementation does nothing.
	 * \param pos Current mouse position
	 * \sa wxSFShapeCanvas
	 */
	virtual void OnRightClick(const wxPoint& pos);
	/*!
	 * \brief Event handler called when the shape is double-clicked by
	 * the left mouse button. The function can be overrided if necessary.
	 *
	 * The function is called by the framework (by the shape canvas).
	 * Default implementation does nothing.
	 * \param pos Current mouse position
	 * \sa wxSFShapeCanvas
	 */
	virtual void OnLeftDoubleClick(const wxPoint& pos);
	/*!
	 * \brief Event handler called when the shape is double-clicked by
	 * the right mouse button. The function can be overrided if necessary.
	 *
	 * The function is called by the framework (by the shape canvas).
	 * Default implementation does nothing.
	 * \param pos Current mouse position
	 * \sa wxSFShapeCanvas
	 */
	virtual void OnRightDoubleClick(const wxPoint& pos);

	/*!
	 * \brief Event handler called at the begining of the shape dragging process.
	 * The function can be overrided if necessary.
	 *
	 * The function is called by the framework (by the shape canvas).
	 * Default implementation does nothing.
	 * \sa wxSFShapeCanvas
	 */
	virtual void OnBeginDrag(const wxPoint& pos);
	/*!
	 * \brief Event handler called during the shape dragging process.
	 * The function can be overrided if necessary.
	 *
	 * The function is called by the framework (by the shape canvas).
	 * Default implementation does nothing.
	 * \param pos Current mouse position
	 * \sa wxSFShapeCanvas
	 */
	virtual void OnDragging(const wxPoint& pos);
	/*!
	 * \brief Event handler called at the end of the shape dragging process.
	 * The function can be overrided if necessary.
	 *
	 * The function is called by the framework (by the shape canvas).
	 * Default implementation does nothing.
	 * \param pos Current mouse position
	 * \sa wxSFShapeCanvas
	 */
	virtual void OnEndDrag(const wxPoint& pos);

	/*!
	 * \brief Event handler called when the user started to drag the shape handle.
	 * The function can be overrided if necessary.
	 *
	 * The function is called by the framework (by the shape canvas).
	 * Default implementation does nothing.
	 * \param handle Reference to dragged handle
	 */
	virtual void OnBeginHandle(wxSFShapeHandle& handle);
	/*!
	 * \brief Event handler called during dragging of the shape handle.
	 * The function can be overrided if necessary.
	 *
	 * The function is called by the framework (by the shape canvas).
	 * Default implementation does nothing.
	 * \param handle Reference to dragged handle
	 */
	virtual void OnHandle(wxSFShapeHandle& handle);
	/*!
	 * \brief Event handler called when the user finished dragging of the shape handle.
	 * The function can be overrided if necessary.
	 *
	 * The function is called by the framework (by the shape canvas).
	 * Default implementation does nothing.
	 * \param handle Reference to dragged handle
	 */
	virtual void OnEndHandle(wxSFShapeHandle& handle);
	/*!
	 * \brief Event handler called when a mouse pointer enters the shape area.
	 * The function can be overrided if necessary.
	 *
	 * The function is called by the framework (by the shape canvas).
	 * Default implementation does nothing.
	 * \param pos Current mouse position
	 */
	virtual void OnMouseEnter(const wxPoint& pos);
	/*!
	 * \brief Event handler called when a mouse pointer moves above the shape area.
	 * The function can be overrided if necessary.
	 *
	 * The function is called by the framework (by the shape canvas).
	 * Default implementation does nothing.
	 * \param pos Current mouse position
	 */
	virtual void OnMouseOver(const wxPoint& pos);
	/*!
	 * \brief Event handler called when a mouse pointer leaves the shape area.
	 * The function can be overrided if necessary.
	 *
	 * The function is called by the framework (by the shape canvas).
	 * Default implementation does nothing.
	 * \param pos Current mouse position
	 */
	virtual void OnMouseLeave(const wxPoint& pos);
	/*!
	 * \brief Event handler called when any key is pressed (in the shape canvas).
	 * The function can be overrided if necessary.
	 *
	 * The function is called by the framework (by the shape canvas).
	 * \param key The key code
	 * \return The function must return TRUE if the default event routine should be called
	 * as well, otherwise FALSE
	 * \sa wxSFShapeBase::_OnKey
	 */
	virtual bool OnKey(int key);

protected:

	// protected data members
	/*! \brief Selection flag */
	bool m_fSelected;
	/*! \brief Visibility flag */
	bool m_fVisible;
	/*! \brief Activation flag */
	bool m_fActive;
    /*! \brief Shape's style mask */
	long m_nStyle;

	wxColour m_nHoverColor;
	wxRealPoint m_nRelativePosition;
	/*! \brief String list with class names of accepted child shapes */
	wxArrayString m_arrAcceptedChildren;
	/*! \brief String list with class names of accepted connections */
	wxArrayString m_arrAcceptedConnections;
	/*! \brief String list with class names of accepted source neighbour shapes */
	wxArrayString m_arrAcceptedSrcNeighbours;
	/*! \brief String list with class names of accepted target neighbour shapes */
	wxArrayString m_arrAcceptedTrgNeighbours;
    /*! \brief Value of vertical border used by AlignV function */
	double m_nVBorder;
    /*! \brief Value of horizontal border used by AlignH function */
    double m_nHBorder;
    /*! \brief Vertical alignment of child shapes */
    VALIGN m_nVAlign;
    /*! \brief Horizontal alignment of child shapes */
    HALIGN m_nHAlign;

	/*! \brief Handle list */
	HandleList m_lstHandles;

	/*! \brief Container for serializable user data associated with the shape */
	xsSerializable *m_pUserData;

	// protected functions
	/*!
	 * \brief Draw the shape in the normal way. The function can be overrided if neccessary.
	 * \param dc Reference to device context where the shape will be drawn to
	 */
	virtual void DrawNormal(wxSFScaledPaintDC& dc);
	/*!
	 * \brief Draw the shape in the selected way. The function can be overrided if neccessary.
	 * \param dc Reference to device context where the shape will be drawn to
	 */
	virtual void DrawSelected(wxSFScaledPaintDC& dc);
	/*!
	 * \brief Draw the shape in the hower mode (the mouse cursor is above the shape).
	 * The function can be overrided if neccessary.
	 * \param dc Reference to device context where the shape will be drawn to
	 */
	virtual void DrawHover(wxSFScaledPaintDC& dc);
	/*!
	 * \brief Draw the shape in the highlighted mode (another shape is dragged over this
	 * shape and this shape will accept the dragged one if it will be dropped on it).
	 * The function can be overrided if neccessary.
	 * \param dc Reference to device context where the shape will be drawn to
	 */
	virtual void DrawHighlighted(wxSFScaledPaintDC& dc);

    /*!
     * \brief Serialize shape's properties to the given XML node. The serialization
     * routine is automatically called by the framework and should take care about serialization
     * of all specific (non-standard) shape's properties.
     *
     * Note, that the shape serialization is used not only for saving canvas's content to a file
     * but also during Undo/Redo and the clipboard operations so it is very important to implement this virtual
     * function otherwise all mentioned operations wont be available for this shape.
     *
     * \param node Pointer to XML node where the shape's property nodes will be appended to
     * \sa xsSerializable::Serialize
     */
	virtual wxXmlNode* Serialize(wxXmlNode* node);
    /*!
     * \brief Deserialize shape's properties from the given XML node. The
     * routine is automatically called by the framework and should take care about deserialization
     * of all specific (non-standard) shape's properties.
     *
     * Note, that the shape serialization is used not only for saving canvas's content to a file
     * but also during Undo/Redo and the clipboard operations so it is very important to implement this virtual
     * function otherwise all mentioned operations wont be available for this shape.
     *
     * \param node Pointer to a source XML node containig the shape's property nodes
     * \sa xsSerializable::Deserialize
     */
    virtual void Deserialize(wxXmlNode* node);

    /*!
     * \brief Repaint the shape
     * \param rct Canvas portion that should be updated
     */
	void Refresh(const wxRect& rct);

private:

	// private data members
	bool m_fMouseOver;
	bool m_fFirstMove;
	bool m_fHighlighParent;

	wxRealPoint m_nMouseOffset;

    // private functions

	 /*! \brief Initialize serializable properties. */
	void MarkSerializableDataMembers();
	/*!
	 * \brief Auxiliary function called by GetNeighbours function.
	 * \param neighbours List of neighbour shapes
	 * \param shapeInfo Line object type
	 * \param condir Connection direction
	 * \param direct Set this flag to TRUE if only closest shapes should be found,
	 * otherwise also shapes connected by forked lines will be found (also
	 * constants sfDIRECT and sfINDIRECT can be used)
	 * \sa GetNeighbours
	 */
	void _GetNeighbours(ShapeList& neighbours, wxClassInfo *shapeInfo, CONNECTMODE condir, bool direct);
	/// <summary> Auxiliary function called by GetCompleteBoundingBox function. </summary>
	/// <param name="rct"> Returned bounding rectangle </param>
	/// <param name="mask"> Bit mask of object types which should be included into calculation </param>
	/// <seealso cref="BBMODE"></seealso>
	void _GetCompleteBoundingBox(wxRect& rct, int mask = bbALL);

	// private event handlers

    /*!
     * \brief Original protected event handler called when the mouse pointer is moving around the shape canvas.
     *
	 * The function is called by the framework (by the shape canvas). After processing the event
	 * relevant overridable event handlers are called.
     * \param pos Current mouse position
     * \sa wxShapeBase::OnMouseEnter, wxShapeBase::OnMouseOver, wxShapeBase::OnMouseLeave
     */
	void _OnMouseMove(const wxPoint& pos);
    /*!
     * \brief Original protected event handler called at the begininig of dragging process.
     *
	 * The function is called by the framework (by the shape canvas). After processing the event
	 * an overridable event handler is called.
     * \param pos Current mouse position
     * \sa wxShapeBase::OnBeginDrag
     */
	void _OnBeginDrag(const wxPoint& pos);
    /*!
     * \brief Original protected event handler called during a dragging process.
     *
	 * The function is called by the framework (by the shape canvas). After processing the event
	 * an overridable event handler is called.
     * \param pos Current mouse position
     * \sa wxShapeBase::OnDragging
     */
	void _OnDragging(const wxPoint& pos);
    /*!
     * \brief Original protected event handler called at the end of dragging process.
     *
	 * The function is called by the framework (by the shape canvas). After processing the event
	 * an overridable event handler is called.
     * \param pos Current mouse position
     * \sa wxShapeBase::OnEndDrag
     */
	void _OnEndDrag(const wxPoint& pos);

	/*!
	 * \brief Original protected event handler called when any key is pressed (in the shape canvas).
	 *
	 * The function is called by the framework (by the shape canvas).
	 * Default implementation performs operations necessary for proper shape's
	 * moving and repainting.
	 * \param key The key code
	 * \sa wxSFShapeBase::OnKey
	 */
	void _OnKey(int key);

	/*!
	 * \brief Original protected event handler called during dragging of the shape handle.
	 *
	 * The function is called by the framework (by the shape canvas).
	 * Default implementation manages the child shapes' alignment (if set).
	 * \param handle Reference to dragged handle
	 */
	void _OnHandle(wxSFShapeHandle& handle);
};

extern ShapeList m_lstProcessed;
