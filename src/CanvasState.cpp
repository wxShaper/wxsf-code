#include <wx/mstream.h>

#include "CanvasState.h"
#include "ShapeCanvas.h"

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
	// create input stream from local memory buffer
	wxMemoryInputStream instream(m_dataBuffer.GetData(), m_dataBuffer.GetDataLen()-1);

	// deserialize canvas content
	if(instream.IsOk() && canvas)
	{
		// clear all previous canvas content
		canvas->Clear();
		canvas->DeserializeChartFromXml(instream);
		canvas->Refresh(false);
	}
}
