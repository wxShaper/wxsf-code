/***************************************************************
 * Name:      CanvasHistory.cpp
 * Purpose:   Implements manager for stored canvas states
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include <wx/mstream.h>

#include "wx/wxsf/CanvasHistory.h"
#include "wx/wxsf/ShapeCanvas.h"

wxSFCanvasHistory::wxSFCanvasHistory(void)
{
	m_pParentCanvas = NULL;
	m_pCurrentCanvasState = NULL;
	m_nHistoryDepth = 25;

	m_lstCanvasStates.DeleteContents(true);
}

wxSFCanvasHistory::~wxSFCanvasHistory(void)
{
	Clear();
}

//----------------------------------------------------------------------------------//
// Public functions
//----------------------------------------------------------------------------------//

void wxSFCanvasHistory::Clear()
{
	m_lstCanvasStates.Clear();
	m_pCurrentCanvasState = NULL;
}

void wxSFCanvasHistory::SaveCanvasState()
{
    wxASSERT(m_pParentCanvas);
    wxASSERT(m_pParentCanvas->GetDiagramManager());

	wxMemoryOutputStream outstream;

	if(outstream.IsOk() && m_pParentCanvas && m_pParentCanvas->GetDiagramManager())
	{
		// serialize canvas to memory stream
		m_pParentCanvas->GetDiagramManager()->SerializeToXml(outstream);

		// delete all states newer than the current state
		if(m_pCurrentCanvasState)
		{
			wxCStateListNode* delnode = m_lstCanvasStates.GetLast();
			while(delnode != m_pCurrentCanvasState)
			{
				m_lstCanvasStates.DeleteNode(delnode);
				delnode = m_lstCanvasStates.GetLast();
			}
		}

		// create and append new canvas state
		m_pCurrentCanvasState =  m_lstCanvasStates.Append(new wxSFCanvasState(outstream.GetOutputStreamBuffer()));

		// check the history bounds
		if(m_lstCanvasStates.GetCount() > m_nHistoryDepth)
		{
			m_lstCanvasStates.DeleteNode(m_lstCanvasStates.GetFirst());
		}
	}
}

void wxSFCanvasHistory::RestoreOlderState()
{
	// move to previous canvas state and restore it if exists
	m_pCurrentCanvasState = m_pCurrentCanvasState->GetPrevious();
	if(m_pCurrentCanvasState)
	{
		m_pCurrentCanvasState->GetData()->Restore(m_pParentCanvas);
	}
}

void wxSFCanvasHistory::RestoreNewerState()
{
	// move to next canvas state and restore it if exists
	m_pCurrentCanvasState = m_pCurrentCanvasState->GetNext();
	if(m_pCurrentCanvasState)
	{
		m_pCurrentCanvasState->GetData()->Restore(m_pParentCanvas);
	}
}

bool wxSFCanvasHistory::CanUndo()
{
	return ((m_pCurrentCanvasState != NULL) && (m_pCurrentCanvasState != m_lstCanvasStates.GetFirst()));
}

bool wxSFCanvasHistory::CanRedo()
{
	return ((m_pCurrentCanvasState != NULL) && (m_pCurrentCanvasState != m_lstCanvasStates.GetLast()));
}
