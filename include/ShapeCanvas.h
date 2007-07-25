/***************************************************************
 * Name:      ShapeCanvas.h
 * Purpose:   Defines shape canvas class
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#pragma once

#include "ShapeBase.h"
#include "MultiSelRect.h"
#include "CanvasHistory.h"
#include "LineShape.h"

#define MEOFFSET 5
#define serINCLUDE_PARENTS true
#define serWITHOUT_PARENTS false
#define sfSAVE_STATE true
#define sfDONT_SAVE_STATE false

// default values
/*! \brief Default value of wxSFShapeCanvas::m_fShowGrid data member */
#define sfdvSHAPECANVAS_SHOWGRID true
/*! \brief Default value of wxSFShapeCanvas::m_fUseGrid data member */
#define sfdvSHAPECANVAS_USEGRID true
/*! \brief Default value of wxSFShapeCanvas::m_fMultiselection data member */
#define sfdvSHAPECANVAS_MULTISELECTION true
/*! \brief Default value of wxSFShapeCanvas::m_fMultiSizeChange data member */
#define sfdvSHAPECANVAS_MULTISIZECHANGE true
/*! \brief Default value of wxSFShapeCanvas::m_nBackgroundColor data member */
#define sfdvSHAPECANVAS_BACKGROUNDCOLOR wxColour(240, 240, 240)
/*! \brief Default value of wxSFShapeCanvas::m_nGridSize data member */
#define sfdvSHAPECANVAS_GRIDSIZE wxSize(10, 10)
/*! \brief Default value of wxSFShapeCanvas::m_nGridColor data member */
#define sfdvSHAPECANVAS_GRIDCOLOR wxColour(200, 200, 200)
/*! \brief Default value of wxSFShapeCanvas::m_CommnonHoverColor data member */
#define sfdvSHAPECANVAS_HOVERCOLOR wxColor(120, 120, 255)

/*! \brief Auxiliary class encapsulation two variables suitable for shape IDs. It is
 * used for storing infomation about various relevant shape IDs */
class CIDPair : public wxObject
{
public:
    /*! \brief Constructor */
	CIDPair(long oldId, long newId){m_nOldID = oldId; m_nNewID = newId;}
	long m_nNewID;
	long m_nOldID;
};

WX_DECLARE_LIST(CIDPair, CIDList);

/*!
 * \brief Class encapsulating a Shape canvas. The shape canvas is window control
 * which extends the wxScrolledWindow and is responsible for management of shapes diagrams.
 * It also supports clipboard and drag&drop operations, undo/redo operations,
 * graphics exporting functions and serialization/deserialization of inserted shape objects.
 *
 * This class is a core framework class and provides many member functions suitable for adding,
 * removing, moving, resizing and drawing of shape objects. It can be used as it is or as a base class
 * if necessary. In that case, the default class functionality can be enhaced by overriding of
 * its virtual functions or by manual events handling. In both cases the user is responsible
 * for invoking of default event handlers/virtual functions otherwise the
 * built in functionality wont be available.
 * \sa wxSFShapeObject
 */
class wxSFShapeCanvas : public wxScrolledWindow
{
public:

    /*!
     * \brief Constructor
     * \param parent Parent window
     * \param id Window ID
     * \param pos Initial position
     * \param size Initial size
     * \param style Window style
     */
	wxSFShapeCanvas(wxWindow* parent, wxWindowID id = -1, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxHSCROLL | wxVSCROLL);
	/*! \brief Destructor */
	~wxSFShapeCanvas(void);

    /*! \brief Working modes */
	enum MODE
	{
	    /*! \brief The shape canvas is in ready state (no operation is pending) */
		modeREADY = 0,
        /*! \brief Some shape handle is dragged */
		modeHANDLEMOVE,
		/*! \brief Handle of multiselection tool is dragged */
		modeMULTIHANDLEMOVE,
		/*! \brief Some shape/s is/are dragged */
		modeSHAPEMOVE,
		/*! \brief Multiple shape selection is in progess */
		modeMULTISELECTION,
		/*! \brief Interactive connection creation is in progress */
		modeCREATECONNECTION
	};

    /*! \brief Search mode flags for GetShapeAtPosition function */
	enum SEARCHMODE
	{
	    /*! \brief Search for selected shapes only*/
		searchSELECTED,
		/*! \brief Search for unselected shapes only */
		searchUNSELECTED,
		/*! \brief Search for both selected and unselected shapes */
		searchBOTH
	};

    /*! \brief Flags for AlignSelected function */
	enum VALIGN
	{
	    valignNONE,
	    valignTOP,
	    valignMIDDLE,
	    valignBOTTOM
	};

    /*! \brief Flags for AlignSelected function */
	enum HALIGN
	{
	    halignNONE,
	    halignLEFT,
	    halignCENTER,
	    halignRIGHT
	};

	// public functions
	/*! \brief Get wxShapeFramework version number */
	wxString GetVersion() {return m_sVersion;}

	/*!
	 * \brief Get the lowest free shape ID
	 */
	long GetNewId();

    /*!
     * \brief Load serialized canvas content (diagrams) from given file.
     * \param file Full file name
     */
	void LoadCanvas(const wxString& file);
    /*!
     * \brief Save  canvas content (diagrams) to given file.
     * \param file Full file name
     */
	void SaveCanvas(const wxString& file);
    /*!
     * \brief Export canvas content to BMP file.
     * \param file Full file name
     */
	void SaveCanvasToBMP(const wxString& file);

    /*!
     * \brief Create new direct connection between two shapes.
     *
     * This function creates new simple connection line (without arrows) between gived
     * shapes.
     * \param srcId ID of a source shape
     * \param trgId ID of target shape
     * \param saveState Set the parameter TRUE if you wish to save canvas state after the operation
     * \return Pointer to new connection object. The object is added to the shape canvas automaticaly.
     * \sa StartInteractiveConnection
     */
    wxSFShapeBase* CreateConnection(long srcId, long trgId, bool saveState = true);
    /*!
     * \brief Create new shape and add it to the shape canvas.
     * \param shapeInfo Shape type
     * \param saveState Set the parameter TRUE if you wish to save canvas state after the operation
     * \return Pointer to new shape object. The object is added to the shape canvas automaticaly.
     */
	wxSFShapeBase* AddShape(wxClassInfo* shapeInfo, bool saveState = true);
	/*!
	 * \brief Create new shape and add it to the shape canvas.
	 * \param shapeInfo Shape type
	 * \param pos Shape position
	 * \param saveState Set the parameter TRUE if you wish to save canvas state after the operation
	 * \return Description
	 * \sa Seealso
	 */
	wxSFShapeBase* AddShape(wxClassInfo* shapeInfo, const wxPoint& pos, bool saveState = true);
	/*!
	 * \brief Remove given shape from the shape canvas. The shape object will be deleted as well.
	 * \param shape Pointer to shape object should be deleted
	 * \param refresh Set the paramater to TRUE if you wish to repaint the canvas
	 */
	void RemoveShape(wxSFShapeBase* shape, bool refresh = true);
	/*!
	 * \brief Remove shapes from the shape canvas
	 * \param selection List of shapes which should be removed from the canvas
	 */
	void RemoveShapes(const CShapeList& selection);
	/*! \brief Remove all shapes from the canvas */
	void Clear();
    /*!
     * \brief Add given shape type to an acceptance list. The acceptance list contains class
     * names of the shapes which can be inserted into this instance of shapes canvas.
     * Note: Keyword 'All' behaves like any class name.
     * \param type Class name of accepted shape object
     * \sa IsShapeAccepted
     */
	void AcceptShape(const wxString& type) {m_arrAcceptedShapes.Add(type);}
    /*!
     * \brief Tells whether the given shape type is accepted by this canvas instance (it means
     * whether this shape can be inserted into it).
     *
     * The function is typically used by the framework for determination whether class type supplied
     * by AddShape() function can be inserted into shape canvas.
     * \param type Class name of examined shape object
     * \return TRUE if the shape type is accepted, otherwise FALSE.
     */
	bool IsShapeAccepted(const wxString& type);
	/*!
	 * \brief Clear shape object acceptance list
	 * \sa AcceptShape
	 */
	void ClearAcceptedShapes(){m_arrAcceptedShapes.Clear();}

    /*!
     * \brief Start interactive connection creation.
     *
     * This function switch the cavas to a mode in which a new shape connection
     * can be created interactively (by mouse operations). Every connection must
     * start and finish in some shape object or another connection. At the end of the
     * process the OnConnectionFinished event handler is invoked so the user can
     * set needed connection properties immediately.
     *
     * Function must be called from mouse event handler and the event must be passed
     * to the function.
     * \param shapeInfo Connection type
     * \param pos Position where to start
     * \sa CreateConnection
     */
    void StartInteractiveConnection(wxClassInfo* shapeInfo, const wxPoint& pos);
    /*! \brief Abort interactive connection creation process */
	void AbortInteractiveConnection();
	/*!
	 * \brief Event handler called after successfull conection creation. The function
	 * can be overrided if necessary. The default implementation does nothing.
	 * \param connection Pointer to new connection object
	 * \sa StartInteractiveConnection
	 */
	virtual void OnConnectionFinished(wxSFLineShape* connection);

    /*! \brief Select all shapes in the canvas */
	void SelectAll();
	/*! \brief Deselect all shapes */
	void DeselectAll();
	/*! \brief Hide handles of all shapes */
	void HideAllHandles();
	/*!
	 * \brief Repaint the shape canvas
	 * \param erase TRUE if the canvas should be erased before repainting
	 * \param rct Refreshed region (rectangle)
	 */
	void RefreshCanvas(bool erase, wxRect rct);

    /*! \brief Copy selected shapes to the clipboard */
	void Copy();
	/*! \brief Copy selected shapes to the clipboard and remove them from the canvas */
	void Cut();
	/*! \brief Paste shapes stored in the clipboard to the canvas */
	void Paste();
	/*! \brief Perform Undo operation (if available) */
	void Undo();
	/*! \brief Perform Redo operation (if available) */
	void Redo();
	/*! \brief Function returns TRUE if some shapes can be copied to the clipboard (it means they are selected) */
	bool CanCopy();
	/*! \brief Function returns TRUE if some shapes can be cutted to the clipboard (it means they are selected) */
	bool CanCut();
	/*! \brief Function returns TRUE if some shapes can be copied from the clipboard to the canvas
	 * (it means the clipboard contains stored shapes) */
	bool CanPaste();
	/*! \brief Function returns TRUE if Undo operation can be done */
	bool CanUndo();
	/*! \brief Function returns TRUE if Redo operation can be done */
	bool CanRedo();
	/*! \brief Function returns TRUE if AlignSelected function can be invoked (if more than
	 * one shape is selected) */
	bool CanAlignSelected();

    /*! \brief Save current canvas state (for Undo/Redo operations) */
	void SaveCanvasState();
	/*! \brief Clear all stored canvas states (no Undo/Redo operations will be available) */
	void ClearCanvasHistory();

    /*!
     * \brief Serialize complete shape canvas to given file
     * \param file Output file
     */
	void SerializeChartToXml(const wxString& file);
    /*!
     * \brief Deserialize complete shape canvas from given file
     * \param file Input file
     */
	void DeserializeChartFromXml(const wxString& file);
    /*!
     * \brief Serialize complete shape canvas to given output stream
     * \param outstream Output stream
     */
	void SerializeChartToXml(wxOutputStream& outstream);
    /*!
     * \brief Deserialize complete shape canvas from given input stream
     * \param instream Input stream
     */
	void DeserializeChartFromXml(wxInputStream& instream);
    /*!
     * \brief Serialize child shapes of specified parent.
     *
     * The parent can be NULL (in that case all shapes in the canvas will be serialized
     * to given XML node).
     * \param parent Parent shape
     * \param node Output XML node
     * \param withparent If TRUE then parent shape will be serialized as well
     */
	void SerializeShapes(wxSFShapeBase* parent, wxXmlNode* node, bool withparent);
	/*!
	 * \brief Deserialize shapes from XML and assign them to given parent.
	 *
     * The parent can be NULL (in that case topmost shapes will have no parent assigned).
	 * \param parent Parent shapes
	 * \param node Source XML node
	 */
	void DeserializeShapes(wxSFShapeBase* parent, wxXmlNode* node);

    /*!
     * \brief Convert device position to logical position.
     *
     * The function returns unscrolled unscaled canvas position.
     * \param pos Device position (for example mouse position)
     * \return Logical position
     */
	wxPoint DP2LP(const wxPoint& pos) const;
    /*!
     * \brief Convert logical position to device position.
     *
     * The function returns scrolled scaled canvas position.
     * \param pos Logical position (for example shape position)
     * \return Device position
     */
	wxPoint LP2DP(const wxPoint& pos) const;

    /*!
     * \brief Find shape with given ID.
     * \param id Shape's ID
     * \return Pointer to shape if exists, otherwise NULL
     */
	wxSFShapeBase* FindShape(long id);
	/*!
	 * \brief Get shape at given logical position.
	 * \param pos Logical position
	 * \param zorder Z-order of searched shape (usefull if several shapes are located
	 * at the given position)
	 * \param mode Search mode
	 * \return Pointer to shape if found, otherwise NULL
	 * \sa SEARCHMODE, wxSFShapeCanvas::DP2LP
	 */
	wxSFShapeBase* GetShapeAtPosition(const wxPoint& pos, int zorder = 1, SEARCHMODE mode = searchBOTH);
    /*!
     * \brief Get topmost handle at given position
     * \param pos Logical position
     * \return Pointer to shape handle object if found, otherwise NULL
     * \sa wxSFShapeCanvas::DP2LP
     */
	wxSFShapeHandle* GetTopmostHandleAtPosition(const wxPoint& pos);
	/*!
	 * \brief Get list of all shapes located at given position
	 * \param pos Logical position
	 * \param shapes Reference to shape list where pointers to
	 * all found shapes will be stored
	 * \return Number of found shapes
	 * \sa wxSFShapeCanvas::DP2LP
	 */
	int GetShapesAtPosition(const wxPoint& pos, CShapeList& shapes);
	/*!
	 * \brief Get list of shapes located inside given rectangle
	 * \param rct Examined rectangle
	 * \param shapes Reference to shape list where pointers to
	 * all found shapes will be stored
	 * \return Number of found shapes
	 */
	int GetShapesInside(const wxRect& rct, CShapeList& shapes);
	/*!
	 * \brief Get list of all shapes in the canvas.
	 *
	 * Note that this is a main shape list and every change made on it
	 * will affect a shape diagram in the canvas.
	 * \return Reference to shape list
	 */
	CShapeList& GetShapeList(){return m_lstShapes;}
	/*!
	 * \brief Get list of selected shapes.
	 * \param selection Reference to shape list where pointers to
	 * all selected shapes will be stored
	 * \return Number of selected shapes
	 */
	int GetSelectedShapes(CShapeList& selection);
	/*!
	 * \brief Get list of connections assigned to given parent shape.
	 * \param parent Pointer to parent shape
	 * \param mode Search mode
	 * \param lines Reference to shape list where pointers to
	 * all found connections will be stored
	 * \return Number of found connections
	 * \sa wxSFShapeBase::CONNECTMODE
	 */
	int GetAssignedConnections(wxSFShapeBase* parent, wxSFShapeBase::CONNECTMODE mode, CShapeList& lines);
	/*!
	 * \brief Get list of shapes of given type.
	 * \param shapeInfo Shape object type
	 * \param shapes Reference to shape list where pointers to
	 * all found shapes will be stored
	 * \return Number of found shapes
	 */
	int GetShapes(wxClassInfo* shapeInfo, CShapeList& shapes);

    /*!
     * \brief Function finds out whether given shape has some children.
     * \param parent Pointer to potential parent shape
     * \return TRUE if the parent shape has children, otherwise FALSE
     */
	bool HasChildren(wxSFShapeBase* parent);
	/*!
	 * \brief Get child shapes of given parent shape.
	 * \param parent Pointer to parent shape (can be NULL for all topmost shapes)
	 * \param children Reference to shape list where pointers to
	 * all found shapes will be stored
	 * \param recursive Set this parameter to TRUE if children of children should
	 * be found as well (in recursive way)
	 */
	void GetChildren(wxSFShapeBase* parent, CShapeList& children, bool recursive = false);
	/*!
	 * \brief Get neighbour shapes connected to given parent shape.
	 * \param parent Pointer to parent shape (can be NULL for all topmost shapes)
	 * \param neighbours List of neighbour shapes
	 * \param condir Connection direction
	 * \param direct Set this flag to TRUE if only closest shapes should be found,
	 * otherwise also shapes connected by forked lines will be found (also
	 * constants sfDIRECT and sfINDIRECT can be used)
	 * \sa wxSFShapeBase::CONNECTMODE
	 */
	void GetNeighbours(wxSFShapeBase* parent, CShapeList& neighbours, wxSFShapeBase::CONNECTMODE condir, bool direct = true);
    /*!
	 * \brief Get box bounding all shapes in the canvas.
	 * \return Total bounding box
	 */
	wxRect GetTotalBoundingBox() const;
    /*!
     * \brief Get bounding box of all selected shapes.
     * \return Selection bounding box
     */
	wxRect GetSelectionBB();

	/*!
	 * \brief Align selected shapes in given directions.
	 *
	 * Shapes will be aligned according to most far shape in appropriate direction.
	 * \param halign Horizontal alignment
	 * \param valign Vertical alignment
	 */
	void AlignSelected(HALIGN halign, VALIGN valign);

	// public members accessors
	/*!
	 * \brief Set canvas background color.
	 * \param col Background color
	 */
	void SetCanvasColour(const wxColour& col){m_nBackgroundColor = col;}
	/*!
	 * \brief Get canvas background color.
	 * \return Background color
	 */
	wxColour GetCanvasColour() const {return m_nBackgroundColor;}
	/*! \brief Function returns TRUE if a canvas grid is used. */
	bool IsGridUsed(){return m_fUseGrid;}
	/*!
	 * \brief Snap shapes to canvas grid On/Off.
	 * \param use TRUE if the grid should be used, otherwise FALSE.
	 */
	void UseGrid(bool use){m_fUseGrid = use;}
	/*! \brief Functions returns TRUE if the canvas grid is used. */
	bool IsGridShown(){return m_fShowGrid;}
	/*!
	 * \brief Show/hide canvas grid.
	 * \param show TRUE if the grid should be visible, otherwise FALSE.
	 */
	void ShowGrid(bool show){m_fShowGrid = show;}
	/*!
	 * \brief Get grid size.
	 * \return Grid size
	 */
	wxSize GetGrid() const {return m_nGridSize;}
	/*!
	 * \brief Set grid size.
	 * \param grid Grid size.
	 */
	void SetGrid(wxSize grid){m_nGridSize = grid;}
	/*!
	 * \brief Set grid color.
	 * \param col Grid color.
	 */
	void SetGridColour(const wxColour& col){m_nGridColor = col;}
	/*!
	 * \brief Get grid color.
	 * \return Grid color
	 */
	wxColour GetGridColour() const {return m_nGridColor;}
	/*!
	 * \brief Set canvas scale.
	 * \param scale Scale value.
	 */
	void SetScale(double scale);
	/*!
	 * \brief Get canvas scale.
	 * \return Canvas scale
	 */
	double GetScale(){return m_nScale;}
	/*!
	 * \brief Get canvas workind mode.
	 * \return Working mode
	 * \sa MODE
	 */
	MODE GetMode(){return m_nWorkingMode;}
	/*!
	 * \brief Set default hover color.
	 * \param col Hover color.
	 */
	void SetHoverColour(const wxColour& col);
	/*!
	 * \brief Get default hover color.
	 * \return Hover color
	 */
	wxColour GetHoverColour() const {return m_nCommonHoverColor;}
	/*!
	 * \brief Enable/disable shape multiselection.
	 * \param enable TRUE if the feature should be enabled, otherwise FALSE
	 */
	void EnableMultiselection(bool enable){m_fMultiselection = enable;}
	/*!
	 * \brief Get state of multiselection flag.
	 * \return TRUE if shape multiselection is enabled, otherwise FALSE
	 */
	bool IsMultiselectionEnable(){return m_fMultiselection;}
	/*!
	 * \brief Enable/disable simultaneous shape size changes (via multiselection).
	 * \param enable TRUE if the feature should be enabled, otherwise FALSE
	 */
	void EnableMultiSizeChange(bool enable){m_fMultiSizeChange = enable;}
	/*!
	 * \brief Get state of multi size change flag.
	 * \return TRUE if shape multi change is enabled, otherwise FALSE
	 */
	bool IsMultiSizeChangeEnabled(){return m_fMultiSizeChange;}

	// public virtual event handlers
    /*!
     * \brief Event handler call when the canvas is clicked by
	 * the left mouse button. The function can be overrided if necessary.
     *
     * The function is called by the framework and provides basic functionality
     * needed for proper management of displayed shape. It is necessary to call
     * this function from overrided methods if the default canvas behaviour
     * should be preserved.
     * \param event Mouse event
     * \sa _OnLeftDown
     */
	virtual void OnLeftDown(wxMouseEvent& event);
    /*!
     * \brief Event handler call when the canvas is double-clicked by
	 * the left mouse button. The function can be overrided if necessary.
     *
     * The function is called by the framework and provides basic functionality
     * needed for proper management of displayed shape. It is necessary to call
     * this function from overrided methods if the default canvas behaviour
     * should be preserved.
     * \param event Mouse event
     * \sa _OnLeftDoubleClick
     */
	virtual void OnLeftDoubleClick(wxMouseEvent& event);
    /*!
     * \brief Event handler call when the left mouse button is released.
     * The function can be overrided if necessary.
     *
     * The function is called by the framework and provides basic functionality
     * needed for proper management of displayed shape. It is necessary to call
     * this function from overrided methods if the default canvas behaviour
     * should be preserved.
     * \param event Mouse event
     * \sa _OnLeftUp
     */
	virtual void OnLeftUp(wxMouseEvent& event);
    /*!
     * \brief Event handler call when the canvas is clicked by
	 * the right mouse button. The function can be overrided if necessary.
     *
     * The function is called by the framework and provides basic functionality
     * needed for proper management of displayed shape. It is necessary to call
     * this function from overrided methods if the default canvas behaviour
     * should be preserved.
     * \param event Mouse event
     * \sa _OnRightDown
     */
	virtual void OnRightDown(wxMouseEvent& event);
    /*!
     * \brief Event handler call when the canvas is double-clicked by
	 * the right mouse button. The function can be overrided if necessary.
     *
     * The function is called by the framework and provides basic functionality
     * needed for proper management of displayed shape. It is necessary to call
     * this function from overrided methods if the default canvas behaviour
     * should be preserved.
     * \param event Mouse event
     * \sa _OnRightDoubleClick
     */
	virtual void OnRightDoubleClick(wxMouseEvent& event);
    /*!
     * \brief Event handler call when the right mouse button is released.
     * The function can be overrided if necessary.
     *
     * The function is called by the framework and provides basic functionality
     * needed for proper management of displayed shape. It is necessary to call
     * this function from overrided methods if the default canvas behaviour
     * should be preserved.
     * \param event Mouse event
     * \sa _OnRightUp
     */
	virtual void OnRightUp(wxMouseEvent& event);
    /*!
     * \brief Event handler call when the mouse pointer is moved.
     * The function can be overrided if necessary.
     *
     * The function is called by the framework and provides basic functionality
     * needed for proper management of displayed shape. It is necessary to call
     * this function from overrided methods if the default canvas behaviour
     * should be preserved.
     * \param event Mouse event
     * \sa _OnMouseMove
     */
	virtual void OnMouseMove(wxMouseEvent& event);
    /*!
     * \brief Event handler call when any key is pressed.
     * The function can be overrided if necessary.
     *
     * The function is called by the framework and provides basic functionality
     * needed for proper management of displayed shape. It is necessary to call
     * this function from overrided methods if the default canvas behaviour
     * should be preserved.
     * \param event Keyboard event
     * \sa _OnKeyDown
     */
	virtual void OnKeyDown(wxKeyEvent& event);

protected:

	// protected data members
	bool m_fUseGrid;
	bool m_fShowGrid;
	bool m_fMultiselection;
	bool m_fMultiSizeChange;
	wxSize m_nGridSize;
	wxColour m_nGridColor;
	wxColour m_nBackgroundColor;
	wxColour m_nCommonHoverColor;
	double m_nScale;

	MODE m_nWorkingMode;

	// protected functions
	/*!
	 * \brief Function responsible for drawing of the canvas's content to given DC.
	 * \param dc Refenrence to device context where the shapes will be drawn to
	 */
	void DrawContent(wxSFScaledPaintDC& dc);

private:

	// private data members
	bool m_fCanSaveStateOnMouseUp;

	wxSFCanvasHistory m_CanvasHistory;
	CShapeList m_lstShapes;
	CShapeList m_lstLinesForUpdate;
	CIDList m_lstIDPairs;
	wxSFMultiSelRect m_shpMultiEdit;
	wxDataFormat m_formatShapes;

	wxSFShapeHandle* m_pSelectedHandle;
	wxSFLineShape* m_pNewLineShape;

	wxArrayString m_arrAcceptedShapes;

	/*! \brief wxSF version number */
	wxString m_sVersion;

	// private functions
    /*!
     * \brief Add an existing shape to the canvas.
     * \param shape Pointer to the shape
     * \param pos Position
     * \param initialize TRUE if the shape should be reinitilialized, otherwise FALSE
     * \param saveState TRUE if the canvas state should be saved
     * \return Pointer to the shape
     */
	wxSFShapeBase* AddShape(wxSFShapeBase* shape, const wxPoint& pos, bool initialize, bool saveState = true);
	/*!
	 * \brief Move child shapes of given parent shape to a top of the shape list (it
	 * means that they will be displayed in the front of all other shapes).
	 * \param parent Parent shape
	 */
	void MoveChildrenToTop(wxSFShapeBase* parent);
	/*!
	 * \brief Find out whether given shape ID is already used.
	 * \param id Shape ID
	 * \return TRUE if the shape ID is used, otherwise FALSE
	 */
	bool IsIdUsed(long id);
	/*!
	 * \brief Update given position so it will fit canvas grid (if enabled).
	 * \param pos Position which should be updated
	 * \return Updated position
	 */
	wxPoint FitPositionToGrid(const wxPoint& pos) const;
	/*! \brief Update size of multi selection rectangle */
	void UpdateMultieditSize();
    /*!
     * \brief Validate selection (remove redundantly selected shapes etc...).
     * \param selection List of selected shapes that should be validated
     */
	void ValidateSelection(CShapeList& selection);
	/*!
	 * \brief Get number of occurences of given ID.
	 * \param id Shape ID
	 * \return Number of ID's occurences
	 */
	int GetIDCount(long id);
	/*! \brief Update connection shapes after importing/dropping of new shapes */
	void UpdateConnections();
	/*! \brief Update scroll window virtual size so it can display all shape canvas */
	void UpdateVirtualSize();
	/*! \brief Close and delete all opened text editing controls actualy used by editable text shapes */
	void DeleteAllTextCtrls();
	/*! \brief Move all shapes so none of it will be located in negative position */
	void MoveShapesFromNegatives();

	// private event handlers
	/*!
	 * \brief Event handler called when the canvas should be repainted.
	 * \param event Paint event
	 */
	void OnPaint(wxPaintEvent& event);
	/*!
	 * \brief Event handler called when the canvas should be erased.
	 * \param event Erase event
	 */
	void OnEraseBackground(wxEraseEvent& event);
	/*!
	 * \brief Event handler called when the mouse pointer leaves the canvas window.
	 * \param event Mouse event
	 */
	void OnLeaveWindow(wxMouseEvent& event);
	/*!
	 * \brief Event handler called when the mouse pointer enters the canvas window.
	 * \param event Mouse event
	 */
	void OnEnterWindow(wxMouseEvent& event);

	// original private event handlers
	/*!
	 * \brief Original private event handler called when the canvas is clicked by
	 * left mouse button. The handler calls user-overridable event handler function
	 * and skips the event for next possible processing.
	 * \param event Mouse event
	 * \sa wxSFShapeCanvas::OnLeftDown
	 */
	void _OnLeftDown(wxMouseEvent& event);
	/*!
	 * \brief Original private event handler called when the canvas is double-clicked by
	 * left mouse button. The handler calls user-overridable event handler function
	 * and skips the event for next possible processing.
	 * \param event Mouse event
	 * \sa wxSFShapeCanvas::OnLeftDoubleClick
	 */
	void _OnLeftDoubleClick(wxMouseEvent& event);
	/*!
	 * \brief Original private event handler called when the left mouse button
	 * is release above the canvas. The handler calls user-overridable event handler function
	 * and skips the event for next possible processing.
	 * \param event Mouse event
	 * \sa wxSFShapeCanvas::OnLeftUp
	 */
	void _OnLeftUp(wxMouseEvent& event);
	/*!
	 * \brief Original private event handler called when the canvas is clicked by
	 * right mouse button. The handler calls user-overridable event handler function
	 * and skips the event for next possible processing.
	 * \param event Mouse event
	 * \sa wxSFShapeCanvas::OnRightDown
	 */
	void _OnRightDown(wxMouseEvent& event);
	/*!
	 * \brief Original private event handler called when the canvas is double-clicked by
	 * right mouse button. The handler calls user-overridable event handler function
	 * and skips the event for next possible processing.
	 * \param event Mouse event
	 * \sa wxSFShapeCanvas::OnRightDoubleClick
	 */
	void _OnRightDoubleClick(wxMouseEvent& event);
	/*!
	 * \brief Original private event handler called when the right mouse button
	 * is release above the canvas. The handler calls user-overridable event handler function
	 * and skips the event for next possible processing.
	 * \param event Mouse event
	 * \sa wxSFShapeCanvas::OnRightUp
	 */
	void _OnRightUp(wxMouseEvent& event);
	/*!
	 * \brief Original private event handler called when the mouse pointer is moving above
	 * the canvas. The handler calls user-overridable event handler function
	 * and skips the event for next possible processing.
	 * \param event Mouse event
	 * \sa wxSFShapeCanvas::OnMouseMove
	 */
	void _OnMouseMove(wxMouseEvent& event);
	/*!
	 * \brief Original private event handler called when any key is pressed.
	 * The handler calls user-overridable event handler function
	 * and skips the event for next possible processing.
	 * \param event Keyboard event
	 * \sa wxSFShapeCanvas::OnKeyDown
	 */
	void _OnKeyDown(wxKeyEvent& event);

	DECLARE_EVENT_TABLE();
};
