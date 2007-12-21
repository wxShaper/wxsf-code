/***************************************************************
 * Name:      CanvasHistory.h
 * Purpose:   Defines manager for stored canvas states
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#pragma once

#include "CanvasState.h"

#define sfDEFAULT_MAX_CANVAS_STATES 25

class WXDLLIMPEXP_SF wxSFShapeCanvas;

/*! \brief
 * Container class that manages stored canvas states (canvas snapshots) and implements
 * basic Undo/Redo functionality.
 *
 * Two different working modes are available: 'histUSE_SERIALIZATION' mode uses basic
 * serialization functionality encapsulated by a diagram manager for storing
 * of current canvas content, but in the 'histUSE_CLONING' mode full copy of 
 * diagram manager content is done via its copy constructor. The first mode is
 * slower than the second one, but do not require implementation of xsSerializable::Clone()
 * virtual function in all classes derived from xsSerializable like the second 
 * posible working mode.
 * \sa wxSFCanvasState, wxSFCanvasHistory::MODE, xsSerializable::Clone, wxXmlSerializer::CopyItems
 */
class WXDLLIMPEXP_SF wxSFCanvasHistory : public wxObject
{
public:

	enum MODE
	{
		/*! \brief Use serialization for storing of a canvas content */
		histUSE_SERIALIZATION,
		/*! \brief Use diagram manager's copy constructor for storing of a canvas content */
		histUSE_CLONING
	};

    /// <summary> Default constructor </summary>
	wxSFCanvasHistory(MODE hnode = histUSE_SERIALIZATION);
    /// <summary> User constructor </summary>
	wxSFCanvasHistory(wxSFShapeCanvas *canvas, MODE hnode = histUSE_SERIALIZATION);
	/// <summary> Destructor </summary>
	~wxSFCanvasHistory(void);

	// public member data accessors

    /*!
     * \brief Set history working mode.
	 *
	 * For more details about available working modes see the wxSFCanvasHistory class
	 * description. Note that all stored canvas history will be cleared after
	 * usage of this function.
     * \param hmode Working mode
     * \sa MODE
     */
	void SetMode(MODE hmode);
	/// <summary> Set total number of stored canvas states. </summary>
	/// <param name="depth"> Number of stored canvas states </param>
	/// <seealso cref="GetHistoryDepth"></seealso>
	void SetHistoryDepth(size_t depth){m_nHistoryDepth = depth;}
	/// <summary> Set pointer to the parent shapes canvas. All Undo/Redo operation defined by this class
	/// will be performed on this shape canvas instance. </summary>
	/// <param name="canvas"> Pointer to parent shape canvas </param>
	void SetParentCanvas(wxSFShapeCanvas* canvas){m_pParentCanvas = canvas;}

	/*! \brief Get currently used working mode */
	MODE GetMode(){return m_nWorkingMode;}
	/// <summary> Get total number of canvas states which can be stored at the same time. </summary>
	/// <returns> Number of allowed concuretly stored canvas states </returns>
	/// <seealso cref="SetHistoryDepth"></seealso>
	size_t GetHistoryDepth(){return m_nHistoryDepth;}

	// public functions
	/// <summary> Save current canvas state </summary>
	void SaveCanvasState();
	/// <summary> Perform the 'Undo' operation </summary>
	void RestoreOlderState();
	/// <summary> Perform the 'Redo' operation </summary>
	void RestoreNewerState();
	/// <summary> Clear all canvas history. </summary>
	void Clear();

    /// <summary> The function gives information whether the 'Undo' operation is available
    /// (exists any stored canvas state older than the current one. </summary>
    /// <returns> TRUE if the 'Undo' operation can be performed, otherwise FALSE </returns>
	bool CanUndo();
	/// <summary> The function gives information whether the 'Redo' operation is available
	/// (exists any stored canvas state newer than the current one. </summary>
	/// <returns> TRUE if the 'Undo' operation can be performed, otherwise FALSE </returns>
	bool CanRedo();

protected:

	// protected data members
	/// <summary> Pointer to the parent canvas </summary>
	wxSFShapeCanvas * m_pParentCanvas;
	/// <summary> List of stored canvas state instances </summary>
	/// <seealso cref="wxSFCanvasState"></seealso>
	CStateList m_lstCanvasStates;
	/// <summary> Auxilary pointer to state list node </summary>
	wxCStateListNode* m_pCurrentCanvasState;
	/*! \brief Canvas history mode */
	MODE m_nWorkingMode;

    /// <summary> Total allowed amount of stored canvas states </summary>
	size_t m_nHistoryDepth;
};
