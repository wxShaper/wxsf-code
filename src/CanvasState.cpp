/***************************************************************
 * Name:      CanvasState.cpp
 * Purpose:   Implements container for stored canvas state
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include <wx/mstream.h>

#include "wx/wxsf/CanvasState.h"
#include "wx/wxsf/ShapeCanvas.h"

#include <wx/listimpl.cpp>

WX_DEFINE_LIST(CStateList);

wxSFCanvasState::wxSFCanvasState(wxStreamBuffer *data)
{
	// copy content of stream buffer to local memory buffer
	if(data)
	{
		data->ResetBuffer();
		m_dataBuffer.AppendData(data->GetBufferStart(), data->GetDataLeft());
		m_dataBuffer.AppendByte(0);
	}
}

wxSFCanvasState::~wxSFCanvasState(void)
{
}

//----------------------------------------------------------------------------------//
// Public functions
//----------------------------------------------------------------------------------//

void wxSFCanvasState::Restore(wxSFShapeCanvas* canvas)
{
    wxASSERT(canvas);
    wxASSERT(canvas->GetDiagramManager());

	// create input stream from local memory buffer
	wxMemoryInputStream instream(m_dataBuffer.GetData(), m_dataBuffer.GetDataLen()-1);

	// deserialize canvas content
	if(instream.IsOk() && canvas && canvas->GetDiagramManager())
	{
		// clear all previous canvas content
		canvas->GetDiagramManager()->Clear();
		canvas->GetDiagramManager()->DeserializeFromXml(instream);
		canvas->Refresh(false);
	}
}
