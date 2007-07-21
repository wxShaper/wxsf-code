#include <wx/wfstream.h>
#include <wx/mstream.h>
#include <wx/txtstrm.h>
#include <wx/clipbrd.h>
#include <wx/listimpl.cpp>

#include "ShapeCanvas.h"
#include "ScaledPaintDC.h"
#include "ShapeDataObject.h"
#include "EditTextShape.h"
#include "BitmapShape.h"
#include "CommonFcn.h"

// TODO: wxSFShapeCanvas: Implement function DoAlignment()

WX_DEFINE_LIST(CIDList);

static const wxChar* dataFormatID = wxT("ShapeFrameWorkDataFormat1_0");

BEGIN_EVENT_TABLE(wxSFShapeCanvas, wxScrolledWindow)
	EVT_PAINT(wxSFShapeCanvas::OnPaint)
	EVT_ERASE_BACKGROUND(wxSFShapeCanvas::OnEraseBackground)
	EVT_LEFT_DOWN(wxSFShapeCanvas::_OnLeftDown)
	EVT_LEFT_UP(wxSFShapeCanvas::_OnLeftUp)
	EVT_RIGHT_DOWN(wxSFShapeCanvas::_OnRightDown)
	EVT_RIGHT_UP(wxSFShapeCanvas::_OnRightUp)
	EVT_LEFT_DCLICK(wxSFShapeCanvas::_OnLeftDoubleClick)
	EVT_RIGHT_DCLICK(wxSFShapeCanvas::_OnRightDoubleClick)
	EVT_MOTION(wxSFShapeCanvas::_OnMouseMove)
	EVT_KEY_DOWN(wxSFShapeCanvas::_OnKeyDown)
	EVT_ENTER_WINDOW(wxSFShapeCanvas::OnEnterWindow)
	EVT_LEAVE_WINDOW(wxSFShapeCanvas::OnLeaveWindow)
END_EVENT_TABLE()

wxSFShapeCanvas::wxSFShapeCanvas(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
: wxScrolledWindow(parent, id, pos, size, style)
{
    m_sVersion =  wxT("1.1.3 alpha");

	SetScrollbars(5, 5, 100, 100);
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);

	// initialize data members
	m_nScale = 1;
	m_nBackgroundColor = sfdvSHAPECANVAS_BACKGROUNDCOLOR;

	m_fCanSaveStateOnMouseUp = false;

	m_fUseGrid = sfdvSHAPECANVAS_USEGRID;
	m_fShowGrid = sfdvSHAPECANVAS_SHOWGRID;
	m_fMultiselection = sfdvSHAPECANVAS_MULTISELECTION;
	m_fMultiSizeChange = sfdvSHAPECANVAS_MULTISIZECHANGE;

	m_nGridSize = sfdvSHAPECANVAS_GRIDSIZE;
	m_nGridColor = sfdvSHAPECANVAS_GRIDCOLOR;
	m_nCommonHoverColor = sfdvSHAPECANVAS_HOVERCOLOR;

	m_nWorkingMode = modeREADY;
	m_pSelectedHandle = NULL;
	m_pNewLineShape = NULL;

	// initialize multiedit rectangle
	m_shpMultiEdit.SetParentCanvas(this);
	m_shpMultiEdit.SetId(0);
	m_shpMultiEdit.CreateHandles();
	m_shpMultiEdit.Select(true);
	m_shpMultiEdit.Show(false);
	m_shpMultiEdit.ShowHandles(true);

	m_lstIDPairs.DeleteContents(true);

	m_formatShapes.SetId(dataFormatID);

	m_CanvasHistory.SetParentCanvas(this);
	SaveCanvasState();
}

wxSFShapeCanvas::~wxSFShapeCanvas(void)
{
	Clear();
}

//----------------------------------------------------------------------------------//
// Painting functions
//----------------------------------------------------------------------------------//

void wxSFShapeCanvas::OnPaint(wxPaintEvent& event)
{
	// use double-buffered painting
	int sx, sy, x, y;

	wxPaintDC paintDC(this);
	GetClientSize(&sx, &sy);

	wxBitmap outbmp(sx, sy);
	wxSFScaledPaintDC dc(outbmp, m_nScale);

	if(dc.IsOk())
	{
        DrawContent(dc);
        dc.GetDeviceOrigin(&x, &y);

        paintDC.Blit(0, 0, sx, sy, &dc, -x, -y);
	}
}

void wxSFShapeCanvas::DrawContent(wxSFScaledPaintDC& dc)
{
    wxSFShapeBase* pShape = NULL;

	// prepare window dc
	PrepareDC(dc);

	// erase background
	dc.SetBackground(wxBrush(m_nBackgroundColor));
	dc.Clear();

	// show grid
	if(m_fShowGrid)
	{
		wxRect gridRct(wxPoint(0, 0), GetVirtualSize()+m_nGridSize);
		int maxx = int(gridRct.GetRight()/m_nScale);
		int maxy = int(gridRct.GetBottom()/m_nScale);

		dc.SetPen(wxColor(m_nGridColor));
		for(int x = gridRct.GetLeft(); x <= maxx; x += m_nGridSize.x)
		{
			dc.DrawLine(x, 0, x, maxy);
		}
		for(int y = gridRct.GetTop(); y <= maxy; y += m_nGridSize.y)
		{
			dc.DrawLine(0, y, maxx, y);
		}
	}

	// draw parent shapes (children are processed by parent objects)
	wxCShapeListNode *node = m_lstShapes.GetLast();
	while(node)
	{
		pShape = node->GetData();
        if((pShape->GetParentShapeId() == -1) && !pShape->IsKindOf(CLASSINFO(wxSFLineShape)))
		{
			pShape->Draw(dc);
		}
		node = node->GetPrevious();
	}

	// draw connections
	node = m_lstShapes.GetLast();
	while(node)
	{
		pShape = node->GetData();
        if(pShape->IsKindOf(CLASSINFO(wxSFLineShape)))
		{
			pShape->Draw(dc);
		}
		node = node->GetPrevious();
	}

	// draw line shape being created
	if(m_pNewLineShape)
	{
	    m_pNewLineShape->Draw(dc);
	}

	// draw multiselection if neccessary
	if(m_shpMultiEdit.IsVisible())
	{
		m_shpMultiEdit.Draw(dc);
	}
}

void wxSFShapeCanvas::OnEraseBackground(wxEraseEvent &event)
{
	// do nothing to suppress window flickering
}

void wxSFShapeCanvas::RefreshCanvas(bool erase, wxRect rct)
{
	wxPoint lpos = DP2LP(wxPoint(0, 0));

	rct.Inflate(int(10/m_nScale), int(10/m_nScale));
	rct.Offset(-lpos.x, -lpos.y);

	RefreshRect(wxRect(int(rct.x*m_nScale), int(rct.y*m_nScale), int(rct.width*m_nScale), int(rct.height*m_nScale)), erase);
}

//----------------------------------------------------------------------------------//
// Virtual functions
//----------------------------------------------------------------------------------//

void wxSFShapeCanvas::OnLeftDown(wxMouseEvent& event)
{
	// HINT: override it for custom actions...

    DeleteAllTextCtrls();
	SetFocus();

	wxPoint lpos = DP2LP(event.GetPosition());

	m_fCanSaveStateOnMouseUp = false;

	switch(m_nWorkingMode)
	{
	case modeREADY:
		{
			m_pSelectedHandle = GetTopmostHandleAtPosition(lpos);

			if(!m_pSelectedHandle)
			{
				wxSFShapeBase* pSelectedShape = GetShapeAtPosition(lpos);

				if(pSelectedShape)
				{
					// perform selection
					CShapeList m_lstSelection;
					GetSelectedShapes(m_lstSelection);

					// cancel previous selections if neccessary...
					if((pSelectedShape->GetParentShapeId() != -1) && (m_lstSelection.IndexOf(pSelectedShape->GetParentShape()) != wxNOT_FOUND))
					{
						DeselectAll();
					}
					else if(!event.ControlDown())
					{
						if(m_lstSelection.IndexOf(pSelectedShape) == wxNOT_FOUND)
						{
							DeselectAll();
						}
					}

					pSelectedShape->Select(true);
					//pSelectedShape->ShowHandles(true);

					GetSelectedShapes(m_lstSelection);
					// remove child shapes from the selection
					ValidateSelection(m_lstSelection);

					if(m_lstSelection.GetCount()>1)
					{
						HideAllHandles();
					}
					m_shpMultiEdit.Show(false);
					//m_shpMultiEdit.ShowHandles(false);
					m_nWorkingMode = modeSHAPEMOVE;

					// inform selected shapes about begin of dragging...
					wxCShapeListNode *node = m_lstSelection.GetFirst();
					while(node)
					{
						node->GetData()->_OnBeginDrag(FitPositionToGrid(lpos));
						node = node->GetNext();
					}

					// call user defined actions
					pSelectedShape->OnLeftClick(FitPositionToGrid(lpos));
				}
				else
				{
					DeselectAll();
					if(m_fMultiselection)
					{
                        m_shpMultiEdit.Show(true);
                        m_shpMultiEdit.ShowHandles(false);
                        m_shpMultiEdit.SetRelativePosition(wxRealPoint(lpos.x, lpos.y));
                        m_shpMultiEdit.SetRectSize(wxRealPoint(0, 0));
                        m_nWorkingMode = modeMULTISELECTION;
					}
					else
					{
					    m_nWorkingMode = modeREADY;
					}
				}

				// update canvas
				Refresh(false);
			}
			else
			{
				if(m_pSelectedHandle->GetParentShape() == &m_shpMultiEdit)
				{
					if(m_fMultiSizeChange)
					{
					    m_nWorkingMode = modeMULTIHANDLEMOVE;
					}
					else
                        m_nWorkingMode = modeREADY;
				}
				else
				{
					m_nWorkingMode = modeHANDLEMOVE;
					switch(m_pSelectedHandle->GetType())
					{
                    case wxSFShapeHandle::hndLINESTART:
                        {
                            wxSFLineShape* pLine = (wxSFLineShape*)m_pSelectedHandle->GetParentShape();
                            pLine->SetLineMode(wxSFLineShape::modeSRCCHANGE);
                            pLine->SetUnfinishedPoint(lpos);
                        }
                        break;

                    case wxSFShapeHandle::hndLINEEND:
                        {
                            wxSFLineShape* pLine = (wxSFLineShape*)m_pSelectedHandle->GetParentShape();
                            pLine->SetLineMode(wxSFLineShape::modeTRGCHANGE);
                            pLine->SetUnfinishedPoint(lpos);
                        }
                        break;

                    default:
                        break;
					}
				}
				m_pSelectedHandle->OnBeginDrag(FitPositionToGrid(lpos));
			}
		}
		break;

	case modeCREATECONNECTION:
		{
		    // update the line shape being created
		    if(m_pNewLineShape)
		    {
		        wxSFShapeBase* pShapeUnder = GetShapeAtPosition(lpos);
                //if(pShapeUnder && !pShapeUnder->IsKindOf(CLASSINFO(wxSFLineShape)))
				if(pShapeUnder)
                {
					if((m_pNewLineShape->GetSrcShapeId() == -1) && (pShapeUnder->IsConnectionAccepted(m_pNewLineShape->GetClassInfo()->GetClassName())))
                    {
                        m_pNewLineShape->SetSrcShapeId(pShapeUnder->GetId());

						// swith on the "under-construcion" mode
						m_pNewLineShape->SetUnfinishedPoint(lpos);
                    }
                    else if((m_pNewLineShape->GetTrgShapeId() == -1) &&
                            (pShapeUnder != m_pNewLineShape) &&
                            (pShapeUnder->IsConnectionAccepted(m_pNewLineShape->GetClassInfo()->GetClassName())))
                    {
                        // find out whether the target shape can be connected to the source shape
                        wxSFShapeBase* pSourceShape = FindShape(m_pNewLineShape->GetSrcShapeId());

                        if( pSourceShape &&
                            pShapeUnder->IsSrcNeighbourAccepted(pSourceShape->GetClassInfo()->GetClassName()) &&
                            pSourceShape->IsTrgNeighbourAccepted(pShapeUnder->GetClassInfo()->GetClassName()) )
                        {
                            m_pNewLineShape->SetTrgShapeId(pShapeUnder->GetId());
                            m_pNewLineShape->CreateHandles();

                            // swith off the "under-construcion" mode
                            m_pNewLineShape->SetLineMode(wxSFLineShape::modeREADY);

                            // inform user that the line is completed
                            OnConnectionFinished(m_pNewLineShape);

                            m_pNewLineShape->Refresh();

                            m_nWorkingMode = modeREADY;
                            m_pNewLineShape = NULL;

                            SaveCanvasState();
                        }
                    }
                }
                else
                {
                    if(m_pNewLineShape->GetSrcShapeId() != -1)
                    {
                        wxPoint flpos = FitPositionToGrid(lpos);
                        m_pNewLineShape->GetControlPoints().Append(new wxRealPoint(flpos.x, flpos.y));
                    }
                }
		    }
		}
		break;

	default:
		m_nWorkingMode = modeREADY;
	}
}

void wxSFShapeCanvas::OnLeftUp(wxMouseEvent &event)
{
    // HINT: override it for custom actions...

	wxPoint lpos = DP2LP(event.GetPosition());

	switch(m_nWorkingMode)
	{
	case modeMULTIHANDLEMOVE:
	case modeHANDLEMOVE:
		{
		    m_pSelectedHandle->OnEndDrag(lpos);

			// resize parent shape to fit all its children if neccessary
			if(m_pSelectedHandle->GetParentShape()->GetParentShapeId() != -1)
			{
				m_pSelectedHandle->GetParentShape()->GetParentShape()->FitToChildren();
			}

			// if the handle is line handle then return the line to normal state
			// and re-assign line's source/target shape
			switch(m_pSelectedHandle->GetType())
			{
            case wxSFShapeHandle::hndLINESTART:
            case wxSFShapeHandle::hndLINEEND:
                {
                    wxSFLineShape* pLine = (wxSFLineShape*)m_pSelectedHandle->GetParentShape();
                    pLine->SetLineMode(wxSFLineShape::modeREADY);

                    wxSFShapeBase* pParentShape = GetShapeAtPosition(lpos, 1, searchBOTH);
					//if(pParentShape && !pParentShape->IsKindOf(CLASSINFO(wxSFLineShape))  && (pParentShape->IsConnectionAccepted(pLine->GetClassInfo()->GetClassName())))
                    if(pParentShape && (pParentShape != pLine) && (pParentShape->IsConnectionAccepted(pLine->GetClassInfo()->GetClassName())))
                    {
                        if(m_pSelectedHandle->GetType() == wxSFShapeHandle::hndLINESTART)
                        {
                            wxSFShapeBase* pTrgShape = FindShape( pLine->GetTrgShapeId() );
                            if( pTrgShape && pParentShape->IsTrgNeighbourAccepted(pTrgShape->GetClassInfo()->GetClassName()))
                            {
                                pLine->SetSrcShapeId(pParentShape->GetId());
                            }
                        }
                        else
                        {
                            wxSFShapeBase* pSrcShape = FindShape( pLine->GetSrcShapeId() );
                            if( pSrcShape && pParentShape->IsSrcNeighbourAccepted(pSrcShape->GetClassInfo()->GetClassName()))
                            {
                                pLine->SetTrgShapeId(pParentShape->GetId());
                            }
                        }
                    }
                }
                break;

            default:
                break;
			}

            m_pSelectedHandle = NULL;
			if(m_fCanSaveStateOnMouseUp)SaveCanvasState();
		}
		break;

	case modeSHAPEMOVE:
		{
			// are shapes dropped into accepting shape?
			wxSFShapeBase* pParentShape = GetShapeAtPosition(lpos, 1, searchUNSELECTED);
			if(pParentShape && !pParentShape->AcceptCurrentlyDraggedShapes())pParentShape = NULL;

			CShapeList m_lstSelection;
			GetSelectedShapes(m_lstSelection);

			wxCShapeListNode *node = m_lstSelection.GetFirst();
			while(node)
			{
				wxSFShapeBase* pShape = node->GetData();
				pShape->_OnEndDrag(lpos);

				// set new parent
				if(pShape->CanChangeParent() && !pShape->IsKindOf(CLASSINFO(wxSFLineShape)))
				{
					if(pParentShape)
					{
						pShape->SetRelativePosition(pShape->GetAbsolutePosition() - pParentShape->GetAbsolutePosition());
						pShape->SetParentShapeId(pParentShape->GetId());
						pShape->DoAlignment();
					}
					else
					{
						if(pShape->GetParentShapeId() != -1)pShape->MoveBy(pShape->GetParentShape()->GetAbsolutePosition());
						pShape->SetParentShapeId(-1);
					}
				}
				node = node->GetNext();
			}

			// resize parent shape to fit all its children
			if(pParentShape)pParentShape->FitToChildren();

			if(m_lstSelection.GetCount()>1)
			{
				m_shpMultiEdit.Show(true);
				//m_shpMultiEdit.ShowHandles(true);
			}
			else
				m_shpMultiEdit.Show(false);

			MoveShapesFromNegatives();

			if(m_fCanSaveStateOnMouseUp)SaveCanvasState();
		}
		break;

	case modeMULTISELECTION:
		{
			CShapeList m_lstSelection;
			wxRect selRect(m_shpMultiEdit.GetBoundingBox().GetLeftTop(), m_shpMultiEdit.GetBoundingBox().GetRightBottom());
			wxCShapeListNode *node = m_lstShapes.GetFirst();
			while(node)
			{
				wxSFShapeBase* pShape = node->GetData();
				if(selRect.Contains(pShape->GetBoundingBox()))
				{
					pShape->Select(true);
					//pShape->ShowHandles(true);
					m_lstSelection.Append(pShape);
				}
				node = node->GetNext();
			}

			ValidateSelection(m_lstSelection);

			if(m_lstSelection.GetCount() > 0)
			{
				HideAllHandles();
				m_shpMultiEdit.Show(true);
				m_shpMultiEdit.ShowHandles(true);
			}
			else
				m_shpMultiEdit.Show(false);

		}
		break;

    default:
        break;

	}

    if(m_nWorkingMode != modeCREATECONNECTION)
    {
        // update canvas
        m_nWorkingMode = modeREADY;
        UpdateMultieditSize();
        UpdateVirtualSize();
        Refresh();
    }
}

void wxSFShapeCanvas::OnLeftDoubleClick(wxMouseEvent& event)
{
	// HINT: override it for custom actions...

    DeleteAllTextCtrls();
	SetFocus();

	wxPoint lpos = DP2LP(event.GetPosition());

	switch(m_nWorkingMode)
	{
    case modeREADY:
        {
            wxSFShapeBase* pShape = GetShapeAtPosition(lpos);
            if(pShape)
            {
                pShape->OnLeftDoubleClick(lpos);
                pShape->Refresh();

                // double click onto a line shape always change its set of
                // control points so the canvas state should be saved now...
                if(pShape->IsKindOf(CLASSINFO(wxSFLineShape)))SaveCanvasState();
            }
        }
        break;

    default:
        break;
	}
}

void wxSFShapeCanvas::OnRightDown(wxMouseEvent& event)
{
	// HINT: override it for custom actions...

    DeleteAllTextCtrls();
	SetFocus();

	wxPoint lpos = DP2LP(event.GetPosition());

	switch(m_nWorkingMode)
	{
    case modeREADY:
        {
            DeselectAll();

            wxSFShapeBase* pShape = GetShapeAtPosition(lpos);
            if(pShape)
            {
                pShape->Select(true);
                pShape->OnRightClick(lpos);
                //pShape->Refresh();
            }
        }
        break;

    default:
        break;
	}

	Refresh();
}

void wxSFShapeCanvas::OnRightUp(wxMouseEvent &event)
{
	// HINT: override it for custom actions...

	wxPoint lpos = DP2LP(event.GetPosition());

	switch(m_nWorkingMode)
	{
	case modeREADY:
		{
			/*wxSFShapeBase* pSelectedShape = GetShapeAtPosition(lpos);

			if(pSelectedShape)
			{
				// perform selection
				DeselectAll();
				//HideAllHandles();
				pSelectedShape->Select(true);
				//pSelectedShape->ShowHandles(true);

				// call user defined action
				pSelectedShape->OnRightClick(lpos);
			}*/
		}
		break;

    default:
        break;
	}

	//m_shpMultiEdit.Show(false);

	//Refresh();
}

void wxSFShapeCanvas::OnRightDoubleClick(wxMouseEvent& event)
{
	// HINT: override it for custom actions...

    DeleteAllTextCtrls();
	SetFocus();

	wxPoint lpos = DP2LP(event.GetPosition());

	switch(m_nWorkingMode)
	{
    case modeREADY:
        {
            wxSFShapeBase* pShape = GetShapeAtPosition(lpos);
            if(pShape)
            {
                pShape->OnRightDoubleClick(lpos);
                pShape->Refresh();
            }
        }
        break;

    default:
        break;
	}
}

void wxSFShapeCanvas::OnMouseMove(wxMouseEvent& event)
{
	// HINT: override it for custom actions...

	wxPoint lpos = DP2LP(event.GetPosition());

	switch(m_nWorkingMode)
	{
	case modeREADY:
	case modeCREATECONNECTION:
		{
			if(!event.Dragging())
			{
				// send event to multiedit shape
				m_shpMultiEdit._OnMouseMove(lpos);

				// send event to all user shapes
				wxCShapeListNode* node = m_lstShapes.GetFirst();
				while(node)
				{
					node->GetData()->_OnMouseMove(lpos);
					node = node->GetNext();
				}

				// update unfinished line if any
				if(m_pNewLineShape)
				{
				    wxRect lineRct = m_pNewLineShape->GetBoundingBox();
				    m_pNewLineShape->SetUnfinishedPoint(lpos);
					lineRct.Union(m_pNewLineShape->GetBoundingBox());

					RefreshCanvas(false, lineRct);
				}
			}
		}
		break;

	case modeHANDLEMOVE:
	case modeMULTIHANDLEMOVE:
		{
			if(event.Dragging())
			{
				m_pSelectedHandle->OnDragging(FitPositionToGrid(lpos));
				if(m_nWorkingMode == modeMULTIHANDLEMOVE)UpdateMultieditSize();

				m_fCanSaveStateOnMouseUp = true;
			}
			else
			{
				m_pSelectedHandle = NULL;
				m_nWorkingMode = modeREADY;
			}
		}
		// do not always 'break'; allow shape's highlighting in modeHANDLEMOVE as well
		if(m_nWorkingMode == modeMULTIHANDLEMOVE)break;

	case modeSHAPEMOVE:
		{
			if(event.Dragging())
			{
				wxCShapeListNode* node = m_lstShapes.GetFirst();
				while(node)
				{
					wxSFShapeBase* pShape = node->GetData();

					if(pShape->IsSelected() && (m_nWorkingMode == modeSHAPEMOVE))
					{
						pShape->_OnDragging(FitPositionToGrid(lpos));
					}
					else
						pShape->_OnMouseMove(lpos);

					node = node->GetNext();
				}
				m_fCanSaveStateOnMouseUp = true;
			}
			else
			{
				m_nWorkingMode = modeREADY;
			}
		}
		break;

	case modeMULTISELECTION:
		{
			wxRect shpRct = m_shpMultiEdit.GetBoundingBox();
			m_shpMultiEdit.SetRectSize(wxRealPoint(lpos.x - shpRct.GetLeft(), lpos.y - shpRct.GetTop()));
			Refresh(false);
		}
		break;
	}
}

void wxSFShapeCanvas::OnKeyDown(wxKeyEvent &event)
{
	// HINT: override it for custom actions...

	CShapeList m_lstSelection;
	GetSelectedShapes(m_lstSelection);

	switch(event.GetKeyCode())
	{
	case WXK_DELETE:
		{
		    // send event to selected shapes
			wxCShapeListNode *node = m_lstSelection.GetFirst();
			while(node)
			{
				node->GetData()->_OnKey(event.GetKeyCode());
				node = node->GetNext();
			}

            // delete selected shapes
			RemoveShapes(m_lstSelection);
			m_shpMultiEdit.Show(false);
			SaveCanvasState();
			Refresh(false);
		}
		break;

	case WXK_ESCAPE:
		{
			switch(m_nWorkingMode)
			{
            case modeCREATECONNECTION:
                {
					AbortInteractiveConnection();
                }
                break;

            case modeHANDLEMOVE:
                {
                    if(m_pSelectedHandle && m_pSelectedHandle->GetParentShape()->IsKindOf(CLASSINFO(wxSFLineShape)))
                    {
                        wxSFLineShape* pLine = (wxSFLineShape*)m_pSelectedHandle->GetParentShape();
                        pLine->SetLineMode(wxSFLineShape::modeREADY);
                    }
                }
                break;

            default:
                // send event to selected shapes
                wxCShapeListNode *node = m_lstSelection.GetFirst();
                while(node)
                {
                    node->GetData()->_OnKey(event.GetKeyCode());
                    node = node->GetNext();
                }
                break;
			}
			m_nWorkingMode = modeREADY;
			Refresh(false);
		}
		break;

	default:
		{
			wxCShapeListNode *node = m_lstSelection.GetFirst();
			while(node)
			{
				node->GetData()->_OnKey(event.GetKeyCode());
				node = node->GetNext();
			}

			//if(m_lstSelection.GetCount() > 1)
			if(m_shpMultiEdit.IsVisible())
			{
				UpdateMultieditSize();
			}
		}
	}
}

//----------------------------------------------------------------------------------//
// Private event handlers functions
//----------------------------------------------------------------------------------//

void wxSFShapeCanvas::_OnLeftDown(wxMouseEvent& event)
{
    this->OnLeftDown(event);

    event.Skip();
}

void wxSFShapeCanvas::_OnLeftDoubleClick(wxMouseEvent& event)
{
    this->OnLeftDoubleClick(event);

    event.Skip();
}

void wxSFShapeCanvas::_OnLeftUp(wxMouseEvent& event)
{
    this->OnLeftUp(event);

    event.Skip();
}

void wxSFShapeCanvas::_OnRightDown(wxMouseEvent& event)
{
    this->OnRightDown(event);

    event.Skip();
}

void wxSFShapeCanvas::_OnRightDoubleClick(wxMouseEvent& event)
{
    this->OnRightDoubleClick(event);

    event.Skip();
}

void wxSFShapeCanvas::_OnRightUp(wxMouseEvent& event)
{
    this->OnRightUp(event);

    event.Skip();
}

void wxSFShapeCanvas::_OnMouseMove(wxMouseEvent& event)
{
    this->OnMouseMove(event);

    event.Skip();
}
void wxSFShapeCanvas::_OnKeyDown(wxKeyEvent& event)
{
    this->OnKeyDown(event);

    event.Skip();
}

void wxSFShapeCanvas::OnEnterWindow(wxMouseEvent& event)
{
	event.Skip();
}

void wxSFShapeCanvas::OnLeaveWindow(wxMouseEvent& event)
{
	wxPoint lpos = DP2LP(event.GetPosition());

	switch(m_nWorkingMode)
	{
	case modeMULTISELECTION:
		UpdateMultieditSize();
		m_shpMultiEdit.Show(false);
        m_nWorkingMode = modeREADY;
        Refresh(false);
		break;

	case modeSHAPEMOVE:
		{
			CShapeList m_lstSelection;
			GetSelectedShapes(m_lstSelection);

			MoveShapesFromNegatives();
			UpdateVirtualSize();

			if(m_lstSelection.GetCount() > 1)
			{
				UpdateMultieditSize();
				m_shpMultiEdit.Show(true);
				m_shpMultiEdit.ShowHandles(true);
			}

            m_nWorkingMode = modeREADY;
            Refresh(false);
		}
		break;

    case modeCREATECONNECTION:
        break;

    case modeHANDLEMOVE:
        {
            if(m_pSelectedHandle)
            {
				if(m_pSelectedHandle->GetParentShape()->IsKindOf(CLASSINFO(wxSFLineShape)))
				{
					wxSFLineShape* pLine = (wxSFLineShape*)m_pSelectedHandle->GetParentShape();
					pLine->SetLineMode(wxSFLineShape::modeREADY);

				}
				else if(m_pSelectedHandle->GetParentShape()->IsKindOf(CLASSINFO(wxSFBitmapShape)))
				{
					wxSFBitmapShape* pBitmap = (wxSFBitmapShape*)m_pSelectedHandle->GetParentShape();
					pBitmap->OnEndHandle(*m_pSelectedHandle);
				}

                SaveCanvasState();
				m_nWorkingMode = modeREADY;
                Refresh(false);
            }
        }
        break;

	case modeMULTIHANDLEMOVE:
		{
			if(m_pSelectedHandle)
			{
				m_pSelectedHandle->OnEndDrag(lpos);

                SaveCanvasState();
				m_nWorkingMode = modeREADY;
                Refresh(false);
			}
		}
		break;

	default:
        m_nWorkingMode = modeREADY;
        Refresh(false);
		break;
	}

	event.Skip();
}

//----------------------------------------------------------------------------------//
// Canvas positions recalculations
//----------------------------------------------------------------------------------//

wxPoint wxSFShapeCanvas::DP2LP(const wxPoint& pos) const
{
	int x,y;
	CalcUnscrolledPosition(pos.x, pos.y, &x, &y);

	return wxPoint(int(x/m_nScale), int(y/m_nScale));
}

wxPoint wxSFShapeCanvas::LP2DP(const wxPoint& pos) const
{
	int x,y;
	CalcScrolledPosition(pos.x, pos.y, &x, &y);

	return wxPoint(int(x*m_nScale), int(y*m_nScale));
}

void wxSFShapeCanvas::SetScale(double scale)
{
	if(scale != 0)m_nScale = scale;
	else
		m_nScale = 1;

	// rescale all bitmap shapes if neccessary
	CShapeList lstBitmaps;
	GetShapes(CLASSINFO(wxSFBitmapShape), lstBitmaps);
	wxCShapeListNode* node = lstBitmaps.GetFirst();
	while(node)
	{
		node->GetData()->Scale(1, 1);
		node = node->GetNext();
	}

    UpdateVirtualSize();
}

wxPoint wxSFShapeCanvas::FitPositionToGrid(const wxPoint& pos) const
{
	if(m_fUseGrid)
	{
		return wxPoint(pos.x / m_nGridSize.x * m_nGridSize.x,
			pos.y / m_nGridSize.y * m_nGridSize.y);
	}
	else
		return pos;
}

//----------------------------------------------------------------------------------//
// Shapes handling functions
//----------------------------------------------------------------------------------//

wxSFShapeBase* wxSFShapeCanvas::AddShape(wxClassInfo* shapeInfo, bool saveState)
{
	wxRect crect = GetClientRect();
	const wxPoint shapePos = wxPoint((crect.GetRight() - crect.GetLeft())/2,
			(crect.GetBottom() - crect.GetTop())/2);

	wxSFShapeBase* pShape = AddShape(shapeInfo, shapePos, saveState);

	return pShape;
}

wxSFShapeBase* wxSFShapeCanvas::AddShape(wxClassInfo* shapeInfo, const wxPoint& pos, bool saveState)
{
	// create shape object from class info
	wxSFShapeBase* pShape = (wxSFShapeBase*)shapeInfo->CreateObject();

	pShape = AddShape(pShape, pos, true, saveState);

	return pShape;
}

wxSFShapeBase* wxSFShapeCanvas::AddShape(wxSFShapeBase* shape, const wxPoint& pos, bool initialize, bool saveState)
{
	if(shape)
	{
		if(shape->IsKindOf(CLASSINFO(wxSFShapeBase)))
		{
			wxPoint newPos = FitPositionToGrid(DP2LP(pos));
			shape->SetRelativePosition(wxRealPoint(newPos.x, newPos.y));

			if(initialize)
			{
				// set shape's properties
				shape->SetParentCanvas(this);
				shape->SetParentShapeId(-1);
				shape->SetId(GetNewId());
				shape->SetHoverColour(m_nCommonHoverColor);
				shape->CreateHandles();
			}

			// insert new shape into list at the first position
            m_lstShapes.Insert(shape);

            // store canvas state
            if(saveState)SaveCanvasState();

			// repaint canvas
			shape->Refresh();
		}
		else
		{
			wxMessageBox(wxString::Format(wxT("Unable to add '%s' class object to the canvas"), shape->GetClassInfo()->GetClassName()), wxT("ShapeFramework"), wxICON_WARNING);

			delete shape;
			shape = NULL;
		}
	}
	return shape;
}

void wxSFShapeCanvas::StartInteractiveConnection(wxClassInfo* shapeInfo, const wxPoint& pos)
{
    if((m_nWorkingMode == modeREADY) && shapeInfo->IsKindOf(CLASSINFO(wxSFLineShape)))
    {
        m_pNewLineShape = (wxSFLineShape*)AddShape(shapeInfo, sfDONT_SAVE_STATE);
        if(m_pNewLineShape)
        {
            wxPoint lpos = DP2LP(pos);

            m_nWorkingMode = modeCREATECONNECTION;
            m_pNewLineShape->SetLineMode(wxSFLineShape::modeUNDERCONSTRUCTION);

            wxSFShapeBase* pShapeUnder = GetShapeAtPosition(lpos);
            if(pShapeUnder)
            {
                if((m_pNewLineShape->GetSrcShapeId() == -1) && (pShapeUnder->IsConnectionAccepted(m_pNewLineShape->GetClassInfo()->GetClassName())))
                {
                    m_pNewLineShape->SetSrcShapeId(pShapeUnder->GetId());

                    // swith on the "under-construcion" mode
                    m_pNewLineShape->SetUnfinishedPoint(lpos);
                }
            }
        }
    }
    //_OnLeftDown(event);
}

void wxSFShapeCanvas::AbortInteractiveConnection()
{
    if(m_pNewLineShape)
    {
        RemoveShape(m_pNewLineShape);
        m_pNewLineShape = NULL;
        OnConnectionFinished(NULL);
    }
	m_nWorkingMode = modeREADY;
	Refresh(false);
}

wxSFShapeBase* wxSFShapeCanvas::CreateConnection(long srcId, long trgId, bool saveState)
{
    wxSFShapeBase* pShape = AddShape(CLASSINFO(wxSFLineShape), sfDONT_SAVE_STATE);
    if(pShape)
    {
        wxSFLineShape *pLine = (wxSFLineShape*)pShape;
        pLine->SetSrcShapeId(srcId);
        pLine->SetTrgShapeId(trgId);

		if(saveState)SaveCanvasState();

		pLine->Refresh();

    }
    return pShape;
}

void wxSFShapeCanvas::OnConnectionFinished(wxSFLineShape* connection)
{
	// HINT: override to perform user-defined actions...
}

void wxSFShapeCanvas::RemoveShape(wxSFShapeBase* shape, bool refresh)
{
	if(shape)
	{
		// delete shape from list
		if(m_lstShapes.DeleteObject(shape))
		{
            // delete the shape
            delete shape;
		}

		if(refresh)Refresh();
	}
}

void wxSFShapeCanvas::RemoveShapes(const CShapeList& selection)
{
    wxSFShapeBase* pShape;
	wxCShapeListNode *node = selection.GetFirst();
	while(node)
	{
	    pShape = node->GetData();
	    // one shape can delete also parent or conection shape so it is
	    // important whether double-linked shapes already exist before
	    // their deletion
	    if(m_lstShapes.IndexOf(pShape) != wxNOT_FOUND)RemoveShape(pShape, false);
		node = node->GetNext();
	}
}

void wxSFShapeCanvas::Clear()
{
	wxCShapeListNode *node = m_lstShapes.GetFirst();
	while(node)
	{
		wxSFShapeBase* pShape = node->GetData();
		if(m_lstShapes.DeleteObject(pShape))delete pShape;

		node = m_lstShapes.GetFirst();
	}
	m_shpMultiEdit.Show(false);
	UpdateVirtualSize();
}

void wxSFShapeCanvas::LoadCanvas(const wxString& file)
{
    bool fChartLoaded = false;
	double nScale = 1;

    wxXmlDocument xmlDoc;
    xmlDoc.Load(file);

    wxXmlNode* root = xmlDoc.GetRoot();
    if(root)
    {
        Clear();
        ClearCanvasHistory();

        if(root->GetName() == wxT("chart"))
        {
            // the file contains only chart without any additional info
            DeserializeShapes(NULL, root);
        }
        else if(root->GetName() == wxT("canvas"))
        {
            // the file contains chart and additional canvas info
            wxXmlNode* child = root->GetChildren();
            while(child)
            {
                if(child->GetName() == wxT("scale"))
                {
                    nScale = StringToDouble(child->GetNodeContent());
                }
				else if(child->GetName() == wxT("background_color"))
                {
                    m_nBackgroundColor = StringToColour(child->GetNodeContent());
                }
                else if(child->GetName() == wxT("hover_color"))
                {
                    m_nCommonHoverColor = StringToColour(child->GetNodeContent());
                }
                else if(child->GetName() == wxT("multiselection"))
                {
                    m_fMultiselection = StringToBool(child->GetNodeContent());
                }
                else if(child->GetName() == wxT("multi_size_change"))
                {
                    m_fMultiSizeChange = StringToBool(child->GetNodeContent());
                }
                else if(child->GetName() == wxT("grid"))
                {
                    wxXmlNode* grid = child->GetChildren();
                    while(grid)
                    {
                        if(grid->GetName() == wxT("use"))
                        {
                            m_fUseGrid = StringToBool(grid->GetNodeContent());
                        }
                        else if(grid->GetName() == wxT("show"))
                        {
                            m_fShowGrid = StringToBool(grid->GetNodeContent());
                        }
                        else if(grid->GetName() == wxT("size"))
                        {
                            m_nGridSize = StringToSize(grid->GetNodeContent());
                        }
                        else if(grid->GetName() == wxT("color"))
                        {
                            m_nGridColor = StringToColour(grid->GetNodeContent());
                        }
                        grid = grid->GetNext();
                    }
                }
                else if(child->GetName() == wxT("chart"))
                {
                    // only one chart can be loaded at once
                    if(!fChartLoaded)
                    {
                        DeserializeShapes(NULL, child);
                        fChartLoaded = true;
                    }
                }
                child = child->GetNext();
            }
        }
        else
            wxMessageBox(wxT("Unknown file format."), wxT("wxShapeFramework"), wxOK | wxICON_WARNING);

		SetScale(nScale);
        SaveCanvasState();
        UpdateVirtualSize();
        Refresh(false);
    }
}

void wxSFShapeCanvas::SaveCanvas(const wxString& file)
{
	// create root node and save canvas properties
	wxXmlNode *child, *root = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("canvas"));

    // save canvas scale
    AddPropertyNode(root, wxT("scale"), DoubleToString(m_nScale));

	// save background color
	if(m_nBackgroundColor != sfdvSHAPECANVAS_BACKGROUNDCOLOR)
	{
        AddPropertyNode(root, wxT("background_color"), ColourToString(m_nBackgroundColor));
	}

    // save common Hover color
    if(m_nCommonHoverColor != sfdvSHAPECANVAS_HOVERCOLOR)
    {
        AddPropertyNode(root, wxT("hover_color"), ColourToString(m_nCommonHoverColor));
    }

    // save multiselection flag
    if(m_fMultiselection != sfdvSHAPECANVAS_MULTISELECTION)
    {
        AddPropertyNode(root, wxT("multiselection"), BoolToString(m_fMultiselection));
    }

    // save multi size change
    if(m_fMultiSizeChange!= sfdvSHAPECANVAS_MULTISIZECHANGE)
    {
        AddPropertyNode(root, wxT("multi_size_change"), BoolToString(m_fMultiSizeChange));
    }

    // save grid properties
    child = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("grid"));
    if(m_fShowGrid != sfdvSHAPECANVAS_SHOWGRID)
    {
        AddPropertyNode(child, wxT("show"), BoolToString(m_fShowGrid));
    }
    if(m_fUseGrid != sfdvSHAPECANVAS_USEGRID)
    {
        AddPropertyNode(child, wxT("use"), BoolToString(m_fUseGrid));
    }
    if(m_nGridSize != sfdvSHAPECANVAS_GRIDSIZE)
    {
        AddPropertyNode(child, wxT("size"), SizeToString(m_nGridSize));
    }
    if(m_nGridColor != sfdvSHAPECANVAS_GRIDCOLOR)
    {
        AddPropertyNode(child, wxT("color"), ColourToString(m_nGridColor));
    }
    root->AddChild(child);

    // create chart node
    wxXmlNode* chart = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("chart"));
    SerializeShapes(NULL, chart, false);
    root->AddChild(chart);

    // create and save XML document
    wxXmlDocument xmlDoc;
    xmlDoc.SetRoot(root);
    xmlDoc.Save(file, 2);
}

void wxSFShapeCanvas::SaveCanvasToBMP(const wxString& file)
{
    // create memory DC a draw the canvas content into

    wxRect bmpBB = GetTotalBoundingBox();
    bmpBB.Inflate(m_nGridSize);

    wxBitmap outbmp(bmpBB.GetRight(), bmpBB.GetBottom());
    wxSFScaledPaintDC outdc(outbmp, 1);

    if(outdc.IsOk())
    {
        DrawContent(outdc);
        outbmp.SaveFile(file, wxBITMAP_TYPE_BMP);
        wxMessageBox(wxString::Format(wxT("The chart has been saved to '%s'."), file.GetData()), wxT("ShapeFramework"));
    }
    else
        wxMessageBox(wxT("Could not create output bitmap."), wxT("wxShapeFramework"), wxOK | wxICON_WARNING);
}

void wxSFShapeCanvas::SerializeChartToXml(const wxString& file)
{
	wxFileOutputStream outstream(file);

	if(outstream.IsOk())
	{
		SerializeChartToXml(outstream);
	}
	else
		wxMessageBox(wxT("Unable to initialize output file stream."), wxT("ShapeFramework"), wxICON_ERROR);
}

void wxSFShapeCanvas::SerializeChartToXml(wxOutputStream& outstream)
{
	// create root node
	wxXmlNode *root = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("chart"));

	if(root)
	{
		// serialize shapes recursively
		SerializeShapes(NULL, root, serWITHOUT_PARENTS);

		// create XML document
		try
		{
			wxXmlDocument xmlDoc;
			xmlDoc.SetRoot(root);

			xmlDoc.Save(outstream, 2);
		}
		catch (...)
		{
			wxMessageBox(wxT("Unable to save XML document."), wxT("ShapeFramework"), wxICON_ERROR);
		}
	}
}

void wxSFShapeCanvas::SerializeShapes(wxSFShapeBase* parent, wxXmlNode* node, bool withparent)
{
	wxXmlNode* shapeNode = NULL;
	wxSFShapeBase* pChild;

	CShapeList lstChildren;
	GetChildren(parent, lstChildren);

	// serialize parent shape
	if(withparent && parent)
	{
		shapeNode = parent->SerializeToXml(NULL);
		if(shapeNode)
		{
			SerializeShapes(parent, shapeNode, serWITHOUT_PARENTS);
			node->AddChild(shapeNode);
		}
	}
	else
	{
		// serialize parent's children
		wxCShapeListNode* snode = lstChildren.GetLast();
		while(snode)
		{
			pChild = snode->GetData();

			shapeNode = pChild->SerializeToXml(NULL);
			if(shapeNode)
			{
				SerializeShapes(pChild, shapeNode, serWITHOUT_PARENTS);
				node->AddChild(shapeNode);
			}

			snode = snode->GetPrevious();
		}
	}
}

void wxSFShapeCanvas::DeserializeChartFromXml(const wxString& file)
{
	wxFileInputStream instream(file);
	if(instream.IsOk())
	{
		ClearCanvasHistory();
		DeserializeChartFromXml(instream);
		SaveCanvasState();
	}
	else
		wxMessageBox(wxT("Unable to initialize input stream."), wxT("ShapeFramework"), wxICON_ERROR);
}

void wxSFShapeCanvas::DeserializeChartFromXml(wxInputStream& instream)
{
	// load an XML file
	try
	{
		wxXmlDocument xmlDoc;
		xmlDoc.Load(instream);

		wxXmlNode* root = xmlDoc.GetRoot();
		if(root && (root->GetName() == wxT("chart")))
		{
			// clear list of ID pairs
			m_lstIDPairs.Clear();
			m_lstLinesForUpdate.Clear();

			// read shape objects from XML recursively
			DeserializeShapes(NULL, root);

			// update IDs in connection lines
			UpdateConnections();

			MoveShapesFromNegatives();
			UpdateVirtualSize();
		}
		else
			wxMessageBox(wxT("Unknown file format."), wxT("ShapeFramework"), wxICON_WARNING);
	}
	catch (...)
	{
		wxMessageBox(wxT("Unable to load XML file."), wxT("ShapeFramework"), wxICON_ERROR);
	}
}

void wxSFShapeCanvas::DeserializeShapes(wxSFShapeBase* parent, wxXmlNode* node)
{
	wxSFShapeBase *pShape;

	wxXmlNode* shapeNode = node->GetChildren();
	while(shapeNode)
	{
		if(shapeNode->GetName() == wxT("object"))
		{
			pShape = AddShape((wxSFShapeBase*)wxCreateDynamicObject(shapeNode->GetPropVal(wxT("type"), wxT(""))), wxDefaultPosition, true, sfDONT_SAVE_STATE);
			if(pShape)
			{
				// store new assigned ID
				long newId = pShape->GetId();
				pShape->DeserializeFromXml(shapeNode);

				// update handle in line shapes
				if(pShape->IsKindOf(CLASSINFO(wxSFLineShape)))
				{
					pShape->CreateHandles();
					m_lstLinesForUpdate.Append(pShape);
				}

				// check whether the new ID is duplicated
				if(GetIDCount(pShape->GetId()) > 1)
				{
					// store information about ID's change and re-assign shape's id
					m_lstIDPairs.Append(new CIDPair(pShape->GetId(), newId));
					pShape->SetId(newId);
				}

				if(parent)pShape->SetParentShapeId(parent->GetId());
				else
					pShape->SetParentShapeId(-1);

				// deserialize child objects
				DeserializeShapes(pShape, shapeNode);
			}
		}
		shapeNode = shapeNode->GetNext();
	}
}

int wxSFShapeCanvas::GetSelectedShapes(CShapeList& selection)
{
	selection.Clear();

	wxCShapeListNode* node = m_lstShapes.GetFirst();
	while(node)
	{
		wxSFShapeBase* pShape = node->GetData();
		if(pShape->IsSelected())selection.Append(pShape);
		node = node->GetNext();
	}

	return (int)selection.GetCount();
}

wxSFShapeBase* wxSFShapeCanvas::GetShapeAtPosition(const wxPoint& pos, int zorder, SEARCHMODE mode)
{
	int nCounter = 1;
	CShapeList m_lstSortedShapes;
	wxSFShapeBase* pShape;

    // sort shapes list in the way that the line shapes will be at the top of the list
	wxCShapeListNode *node = m_lstShapes.GetFirst();
	while(node)
	{
	    pShape = node->GetData();
	    if(pShape->IsKindOf(CLASSINFO(wxSFLineShape)))m_lstSortedShapes.Insert(pShape);
	    else
            m_lstSortedShapes.Append(pShape);

        node = node->GetNext();
	}

    // find the topmost shape according to the given rules
	node = m_lstSortedShapes.GetFirst();
	while(node)
	{
		pShape = node->GetData();
		if(pShape->IsVisible() && pShape->IsActive() && pShape->IsInside(pos))
		{
			switch(mode)
			{
			case searchSELECTED:
				if(pShape->IsSelected())
				{
					if(nCounter == zorder)return pShape;
					else
						nCounter++;
				}
				break;

			case searchUNSELECTED:
				if(!pShape->IsSelected())
				{
					if(nCounter == zorder)return pShape;
					else
						nCounter++;
				}
				break;

			case searchBOTH:
				if(nCounter == zorder)return pShape;
				else
					nCounter++;
				break;
			}
		}
		node = node->GetNext();
	}

	return NULL;
}

wxSFShapeHandle* wxSFShapeCanvas::GetTopmostHandleAtPosition(const wxPoint& pos)
{
    wxSFShapeHandle* pHandle;

	// first test multiedit handles...
	wxCHandleListNode* hnode = m_shpMultiEdit.GetHandles().GetFirst();
	while(hnode)
	{
		pHandle = hnode->GetData();
		if(pHandle->IsVisible() && pHandle->IsInside(pos))return pHandle;
		hnode = hnode->GetNext();
	}

	// ... then test normal handles
	wxCShapeListNode *node = m_lstShapes.GetFirst();
	while(node)
	{
		// iterate through all shape's handles
		wxCHandleListNode* hnode = node->GetData()->GetHandles().GetFirst();
		while(hnode)
		{
			pHandle = hnode->GetData();
			if(pHandle->IsVisible() && pHandle->IsInside(pos))return pHandle;
			hnode = hnode->GetNext();
		}
		node = node->GetNext();
	}

	return NULL;
}

int wxSFShapeCanvas::GetShapesAtPosition(const wxPoint& pos, CShapeList& shapes)
{
	shapes.Clear();
	wxSFShapeBase *pShape;

	wxCShapeListNode *node = m_lstShapes.GetFirst();
	while(node)
	{
		pShape = node->GetData();
		if(pShape->IsVisible() && pShape->IsActive() && pShape->IsInside(pos))shapes.Append(pShape);
		node = node->GetNext();
	}

	return (int)shapes.GetCount();
}

int wxSFShapeCanvas::GetShapesInside(const wxRect& rct, CShapeList& shapes)
{
	shapes.Clear();
	wxSFShapeBase* pShape;

	wxCShapeListNode *node = m_lstShapes.GetFirst();
	while(node)
	{
		pShape = node->GetData();
		if(pShape->IsVisible() && pShape->IsActive() && pShape->Intersects(rct))shapes.Append(pShape);
		node = node->GetNext();
	}

	return (int)shapes.GetCount();
}

int wxSFShapeCanvas::GetAssignedConnections(wxSFShapeBase* parent, wxSFShapeBase::CONNECTMODE mode, CShapeList& lines)
{
	wxSFLineShape* pLine;

    CShapeList m_lstLines;
    if(GetShapes(CLASSINFO(wxSFLineShape), m_lstLines))
    {
        wxCShapeListNode *node = m_lstLines.GetFirst();
        while(node)
        {
            pLine = (wxSFLineShape*)node->GetData();
            switch(mode)
            {
                case wxSFShapeBase::lineSTARTING:
                    if(pLine->GetSrcShapeId() == parent->GetId())lines.Append(pLine);
                    break;

                case wxSFShapeBase::lineENDING:
                    if(pLine->GetTrgShapeId() == parent->GetId())lines.Append(pLine);
                    break;

                case wxSFShapeBase::lineBOTH:
                    if((pLine->GetSrcShapeId() == parent->GetId())
                    || (pLine->GetTrgShapeId() == parent->GetId()))lines.Append(pLine);
                    break;
            }
            node = node->GetNext();
        }
    }

    return (int)lines.GetCount();
}

int wxSFShapeCanvas::GetShapes(wxClassInfo* shapeInfo, CShapeList& shapes)
{
	shapes.Clear();
	wxSFShapeBase* pShape;

	wxCShapeListNode *node = m_lstShapes.GetFirst();
	while(node)
	{
	    pShape = node->GetData();
	    if(pShape->IsKindOf(shapeInfo))shapes.Append(pShape);
	    node = node->GetNext();
	}

	return (int)shapes.GetCount();
}

wxSFShapeBase* wxSFShapeCanvas::FindShape(long id)
{
	wxCShapeListNode *node = m_lstShapes.GetFirst();
	while(node)
	{
		if(node->GetData()->GetId() == id)return node->GetData();
		node = node->GetNext();
	}
	return NULL;
}

void wxSFShapeCanvas::DeselectAll()
{
	wxCShapeListNode *node = m_lstShapes.GetFirst();
	while(node)
	{
		node->GetData()->Select(false);
		//node->GetData()->ShowHandles(false);
		node = node->GetNext();
	}

    m_shpMultiEdit.Show(false);
    //m_shpMultiEdit.ShowHandles(false);
}

void wxSFShapeCanvas::SelectAll()
{
	if(m_lstShapes.GetCount() > 0)
	{
		wxCShapeListNode *node = m_lstShapes.GetFirst();
		while(node)
		{
			node->GetData()->Select(true);
			//node->GetData()->ShowHandles(true);
			node = node->GetNext();
		}

		CShapeList lstSelection;
		GetSelectedShapes(lstSelection);
		ValidateSelection(lstSelection);

		HideAllHandles();
		UpdateMultieditSize();
		m_shpMultiEdit.Show(true);
		m_shpMultiEdit.ShowHandles(true);

		Refresh(false);
	}
}

void wxSFShapeCanvas::HideAllHandles()
{
	wxCShapeListNode *node = m_lstShapes.GetFirst();
	while(node)
	{
		node->GetData()->ShowHandles(false);
		node = node->GetNext();
	}
}

void wxSFShapeCanvas::GetChildren(wxSFShapeBase* parent, CShapeList& children, bool recursive)
{
	if(parent)
	{
		parent->GetChildren(children, recursive);
	}
	else
	{
		wxSFShapeBase* pShape;

		wxCShapeListNode *node = m_lstShapes.GetFirst();
		while(node)
		{
			pShape = node->GetData();
			if(pShape->GetParentShapeId() == -1)
			{
				children.Append(pShape);
				if(recursive)pShape->GetChildren(children, recursive);
			}
			node = node->GetNext();
		}
	}
}

void wxSFShapeCanvas::GetNeighbours(wxSFShapeBase* parent, CShapeList& neighbours, wxSFShapeBase::CONNECTMODE condir, bool direct)
{
    if(parent)
    {
        parent->GetNeighbours(neighbours, condir, direct);
    }
    else
    {
		wxSFShapeBase* pShape;

		wxCShapeListNode *node = m_lstShapes.GetFirst();
		while(node)
		{
			pShape = node->GetData();
			if(pShape->GetParentShapeId() == -1)
			{
				pShape->GetNeighbours(neighbours, condir, direct);
			}
			node = node->GetNext();
		}
    }
}

bool wxSFShapeCanvas::HasChildren(wxSFShapeBase* parent)
{
	CShapeList children;
	GetChildren(parent, children);
	return !children.IsEmpty();
}

void wxSFShapeCanvas::ValidateSelection(CShapeList& selection)
{
	CShapeList m_lstShapesToRemove;

	// find child shapes that have parents in the list
	wxCShapeListNode *node = selection.GetFirst();
	while(node)
	{
		wxSFShapeBase *pShape = node->GetData();
		if((pShape->GetParentShapeId()!= -1) && (selection.IndexOf(pShape->GetParentShape()) != wxNOT_FOUND))
		{
			m_lstShapesToRemove.Append(pShape);
		}
		node = node->GetNext();
	}

	// remove child shapes with parents from the list
	node = m_lstShapesToRemove.GetFirst();
	while(node)
	{
		wxSFShapeBase* pShape = node->GetData();

		pShape->Select(false);
		//pShape->ShowHandles(false);
		selection.DeleteObject(pShape);

		node = node->GetNext();
	}

	// move selected shapes to the top of the global list
	node = selection.GetLast();
	while(node)
	{
		wxSFShapeBase* pShape = node->GetData();

        m_lstShapes.DeleteObject(pShape);
        m_lstShapes.Insert(pShape);

        MoveChildrenToTop(pShape);

		node = node->GetPrevious();
	}
}

void wxSFShapeCanvas::MoveChildrenToTop(wxSFShapeBase* parent)
{
	CShapeList m_lstChildren;
	GetChildren(parent, m_lstChildren);

	wxCShapeListNode *node = m_lstChildren.GetLast();
	while(node)
	{
		wxSFShapeBase* pShape = node->GetData();

		m_lstShapes.DeleteObject(pShape);
		m_lstShapes.Insert(pShape);
		MoveChildrenToTop(pShape);

		node = node->GetPrevious();
	}
}

void wxSFShapeCanvas::UpdateMultieditSize()
{
	// calculate bounding box
	wxRect unionRct;
	bool firstRun = true;
	CShapeList m_lstSelection;
	GetSelectedShapes(m_lstSelection);
	wxCShapeListNode *node = m_lstSelection.GetFirst();
	while(node)
	{
		if(firstRun)
		{
			unionRct = node->GetData()->GetBoundingBox();
			firstRun = false;
		}
		else
			unionRct.Union(node->GetData()->GetBoundingBox());
		node = node->GetNext();
	}
	unionRct.Inflate(MEOFFSET, MEOFFSET);

	// draw rectangle
	m_shpMultiEdit.SetRelativePosition(wxRealPoint(unionRct.GetPosition().x, unionRct.GetPosition().y));
	m_shpMultiEdit.SetRectSize(wxRealPoint(unionRct.GetSize().x, unionRct.GetSize().y));
}

void wxSFShapeCanvas::SetHoverColour(const wxColour& col)
{
	m_nCommonHoverColor = col;

	// update Hover color in all existing shapes
	wxCShapeListNode * node = m_lstShapes.GetFirst();
	while(node)
	{
		node->GetData()->SetHoverColour(col);
		node = node->GetNext();
	}
}

void wxSFShapeCanvas::UpdateConnections()
{
	wxSFLineShape* pLine;
	CIDPair* pIDPair;

	if(m_lstLinesForUpdate.GetCount() > 0)
	{
	    wxCIDListNode* idnode = m_lstIDPairs.GetFirst();
	    while(idnode)
	    {
	        pIDPair = idnode->GetData();
	        if(pIDPair->m_nNewID != pIDPair->m_nOldID)
	        {
                wxCShapeListNode* node = m_lstLinesForUpdate.GetFirst();
                while(node)
                {
                    pLine = (wxSFLineShape*)node->GetData();
                    if(pLine->GetSrcShapeId() == pIDPair->m_nOldID)pLine->SetSrcShapeId(pIDPair->m_nNewID);
                    if(pLine->GetTrgShapeId() == pIDPair->m_nOldID)pLine->SetTrgShapeId(pIDPair->m_nNewID);
                    node = node->GetNext();
                }
	        }
	        idnode = idnode->GetNext();
	    }
    }

	m_lstIDPairs.Clear();
	m_lstLinesForUpdate.Clear();
}

bool wxSFShapeCanvas::IsIdUsed(long id)
{
	return (GetIDCount(id) > 0);
}

int wxSFShapeCanvas::GetIDCount(long id)
{
	int nCount = 0;

	wxCShapeListNode *node = m_lstShapes.GetFirst();
	while(node)
	{
		if(node->GetData()->GetId() == id)nCount++;
		node = node->GetNext();
	}

	return nCount;
}

long wxSFShapeCanvas::GetNewId()
{
	long nId = 1;

	while(IsIdUsed(nId))nId++;

	return nId;
}

wxRect wxSFShapeCanvas::GetTotalBoundingBox() const
{
    wxRect virtRct;

    // calculate total bounding box (includes all shapes)
    wxCShapeListNode* node = m_lstShapes.GetFirst();
    while(node)
    {
        if(node == m_lstShapes.GetFirst())
        {
            virtRct = node->GetData()->GetBoundingBox();
        }
        else
            virtRct.Union(node->GetData()->GetBoundingBox());

        node = node->GetNext();
    }

    return virtRct;
}

void wxSFShapeCanvas::UpdateVirtualSize()
{
    wxRect virtRct = GetTotalBoundingBox();

    // update virtual area of the scrolled window if neccessary
    if(!virtRct.IsEmpty())
    {
        SetVirtualSize(int(virtRct.GetRight()*m_nScale), int(virtRct.GetBottom()*m_nScale));
    }
    else
        SetVirtualSize(500, 500);
}

void wxSFShapeCanvas::DeleteAllTextCtrls()
{
	wxSFContentCtrl* pTextCtrl = NULL;
	CShapeList lstShapes;
	GetShapes(CLASSINFO(wxSFEditTextShape), lstShapes);

	wxCShapeListNode* node = lstShapes.GetFirst();
	while(node)
	{
		pTextCtrl = ((wxSFEditTextShape*)node->GetData())->GetTextCtrl();
		if(pTextCtrl)pTextCtrl->Quit();
		node = node->GetNext();
	}
}

void wxSFShapeCanvas::MoveShapesFromNegatives()
{
	wxSFShapeBase *pShape;
	wxRealPoint shapePos;
	double minx = 0, miny = 0;

	// find the maximal negative position value
	wxCShapeListNode* node = m_lstShapes.GetFirst();
	while(node)
	{
		shapePos = node->GetData()->GetAbsolutePosition();

		if(node == m_lstShapes.GetFirst())
		{
			minx = shapePos.x;
			miny = shapePos.y;
		}
		else
		{
			if(shapePos.x < minx)minx = shapePos.x;
			if(shapePos.y < miny)miny = shapePos.y;
		}

		node = node->GetNext();
	}

	// move all parents shape so they (and their children) will be located in the positive values only
	if((minx < 0) || (miny < 0))
	{
		node = m_lstShapes.GetFirst();
		while(node)
		{
			pShape = node->GetData();

			if(pShape->GetParentShapeId() == -1)
			{
				if(minx < 0)pShape->MoveBy(abs((int)minx), 0);
				if(miny < 0)pShape->MoveBy(0, abs((int)miny));
			}
			node = node->GetNext();
		}
	}
}

//----------------------------------------------------------------------------------//
// Clipboard functions
//----------------------------------------------------------------------------------//

void wxSFShapeCanvas::Copy()
{
	// copy selected shapes to the clipboard
	if(wxTheClipboard->Open())
	{
		CShapeList lstSelection;
		GetSelectedShapes(lstSelection);

		wxSFShapeDataObject* dataObj = new wxSFShapeDataObject(m_formatShapes, lstSelection, this);
		wxTheClipboard->SetData(dataObj);
		wxTheClipboard->Close();
	}
}

void wxSFShapeCanvas::Cut()
{
	Copy();

	// remove selected shapes
	CShapeList lstSelection;
	GetSelectedShapes(lstSelection);

	RemoveShapes(lstSelection);
	m_shpMultiEdit.Show(false);
	SaveCanvasState();
	Refresh(false);
}

void wxSFShapeCanvas::Paste()
{
	if(wxTheClipboard->Open())
	{
		// read data object from the clipboars
		wxSFShapeDataObject dataObj(m_formatShapes);
		if(wxTheClipboard->GetData(dataObj))
		{
			// create memory stream with copied data
			wxMemoryBuffer dataBuffer;
			wxMemoryOutputStream outstream;
			wxTextOutputStream tout(outstream);

			tout.WriteString(dataObj.m_Data.GetText());

			wxStreamBuffer* strbuff = outstream.GetOutputStreamBuffer();
			strbuff->ResetBuffer();
			dataBuffer.AppendData(strbuff->GetBufferStart(), strbuff->GetDataLeft());
			dataBuffer.AppendByte(0);

			wxMemoryInputStream instream(dataBuffer.GetData(), dataBuffer.GetDataLen()-1);

			if(instream.IsOk())
			{
				// deserialize XML data
				DeserializeChartFromXml(instream);

				SaveCanvasState();
				Refresh();
			}
		}
	}
}

void wxSFShapeCanvas::Undo()
{
	m_CanvasHistory.RestoreOlderState();
}

void wxSFShapeCanvas::Redo()
{
	m_CanvasHistory.RestoreNewerState();
}

void wxSFShapeCanvas::SaveCanvasState()
{
	m_CanvasHistory.SaveCanvasState();
}

bool wxSFShapeCanvas::CanCopy()
{
	CShapeList lstSelection;
	GetSelectedShapes(lstSelection);
	return !lstSelection.IsEmpty();
}

bool wxSFShapeCanvas::CanCut()
{
	return CanCopy();
}

bool wxSFShapeCanvas::CanPaste()
{
	bool result = false;

	if(wxTheClipboard->Open())
	{
		result = wxTheClipboard->IsSupported(m_formatShapes);
		wxTheClipboard->Close();
	}

	return result;
}

bool wxSFShapeCanvas::CanUndo()
{
	return m_CanvasHistory.CanUndo();
}

bool wxSFShapeCanvas::CanRedo()
{
	return m_CanvasHistory.CanRedo();
}

void wxSFShapeCanvas::ClearCanvasHistory()
{
	m_CanvasHistory.Clear();
}
