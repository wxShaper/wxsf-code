#include <wx/mstream.h>

#include "CanvasHistory.h"
#include "ShapeCanvas.h"

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
	wxMemoryOutputStream outstream;

	if(outstream.IsOk() && m_pParentCanvas)
	{
		// serialize canvas to memory stream
		m_pParentCanvas->SerializeChartToXml(outstream);

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
