/***************************************************************
 * Name:      CanvasState.h
 * Purpose:   Defines container for stored canvas state
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#pragma once

#include <wx/wxprec.h>
#include "Defs.h"

class wxSFShapeCanvas;
/// <summary> Class which stores one saved state of the shape canvas. The instaces of this
/// class are managed by the wxSFCanvasHistory class which performs all related Undo/Redo
/// operations. </summary>
/// <seealso cref="wxSFShapeCanvas"></seealso>
class wxSFCanvasState : public wxObject
{
friend class wxSFCanvasHistory;

public:
    /// <summary> Default constructor </summary>
    /// <param name="data"> Pointer to the stream buffer containig serialized content of the shape canvas </param>
	wxSFCanvasState(wxStreamBuffer* data);
	/// <summary> Destructor </summary>
	~wxSFCanvasState(void);

protected:

	// protected functions
	/// <summary> Restore stored canvas state. </summary>
	/// <param name="canvas"> Pointer to the shape canvas which content will be replaced by the stored one </param>
	void Restore(wxSFShapeCanvas* canvas);

	// protected data members
	/// <summary> Memory buffer used during the serialization/deserialization operations. </summary>
	wxMemoryBuffer m_dataBuffer;
};

WX_DECLARE_LIST(wxSFCanvasState, CStateList);
