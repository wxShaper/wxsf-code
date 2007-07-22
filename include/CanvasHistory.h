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

class wxSFShapeCanvas;
/// <summary> Container class that manages stored canvas states (canvas snapshots) and implements
/// basic Undo/Redo functionality. </summary>
/// <seealso cref="wxSFCanvasState"></seealso>
class wxSFCanvasHistory : public wxObject
{
public:
    /// <summary> Default constructor </summary>
	wxSFCanvasHistory(void);
	/// <summary> Destructor </summary>
	~wxSFCanvasHistory(void);

	// public member data accessors
	/// <summary> Set total number of stored canvas states. </summary>
	/// <param name="depth"> Number of stored canvas states </param>
	/// <seealso cref="GetHistoryDepth"></seealso>
	void SetHistoryDepth(size_t depth){m_nHistoryDepth = depth;}
	/// <summary> Get total number of canvas states which can be stored at the same time. </summary>
	/// <returns> Number of allowed concuretly stored canvas states </returns>
	/// <seealso cref="SetHistoryDepth"></seealso>
	size_t GetHistoryDepth(){return m_nHistoryDepth;}
	/// <summary> Set pointer to the parent shapes canvas. All Undo/Redo operation defined by this class
	/// will be performed on this shape canvas instance. </summary>
	/// <param name="canvas"> Pointer to parent shape canvas </param>
	void SetParentCanvas(wxSFShapeCanvas* canvas){m_pParentCanvas = canvas;}

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

    /// <summary> Total allowed amount of stored canvas states </summary>
	size_t m_nHistoryDepth;
};
