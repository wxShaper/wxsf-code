/***************************************************************
 * Name:      ShapeCanvas.cpp
 * Purpose:   Implements shape canvas class
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

// TODO: wxSFShapeCanvas: Implement canvas thumbnail (wxSFCanvasThumbnail class)

#include <wx/wfstream.h>
#include <wx/mstream.h>
#include <wx/txtstrm.h>
#include <wx/clipbrd.h>

#include "ShapeCanvas.h"
#include "DiagramManager.h"
#include "ScaledPaintDC.h"
#include "ShapeDataObject.h"
#include "EditTextShape.h"
#include "BitmapShape.h"
#include "CommonFcn.h"

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

wxSFShapeCanvas::wxSFShapeCanvas(wxSFDiagramManager* manager, wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
: wxScrolledWindow(parent, id, pos, size, style)
{
    // initialize shape manager
    wxASSERT_MSG( manager, wxT("Shape manager has not been properly set in shape canvas."));

    m_pManager = manager;
    m_pManager->SetShapeCanvas(this);

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
	m_shpMultiEdit.SetParentManager(m_pManager);
	m_shpMultiEdit.SetId(0);
	m_shpMultiEdit.CreateHandles();
	m_shpMultiEdit.Select(true);
	m_shpMultiEdit.Show(false);
	m_shpMultiEdit.ShowHandles(true);

	m_formatShapes.SetId(dataFormatID);

	// accept all shape types
	//m_arrAcceptedShapes.Add(wxT("All"));

	m_CanvasHistory.SetParentCanvas(this);
	if( m_pManager )SaveCanvasState();
}

wxSFShapeCanvas::~wxSFShapeCanvas(void)
{
	//Clear();
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
    wxASSERT_MSG( m_pManager, wxT("Shape manager has not been properly set in shape canvas."));

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
	wxCShapeListNode *node = m_pManager->GetShapeList().GetLast();
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
	node = m_pManager->GetShapeList().GetLast();
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

	wxASSERT_MSG( m_pManager, wxT("Shape manager has not been properly set in shape canvas."));

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

					// inform selected shapes about begin of dragging...
					wxCShapeListNode *node = m_lstSelection.GetFirst();
					while(node)
					{
						node->GetData()->_OnBeginDrag(FitPositionToGrid(lpos));
						node = node->GetNext();
					}

					// call user defined actions
					pSelectedShape->OnLeftClick(FitPositionToGrid(lpos));

					m_nWorkingMode = modeSHAPEMOVE;
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
                        wxSFShapeBase* pSourceShape = m_pManager->FindShape(m_pNewLineShape->GetSrcShapeId());

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
                            wxSFShapeBase* pTrgShape = m_pManager->FindShape( pLine->GetTrgShapeId() );
                            if( pTrgShape && pParentShape->IsTrgNeighbourAccepted(pTrgShape->GetClassInfo()->GetClassName()))
                            {
                                pLine->SetSrcShapeId(pParentShape->GetId());
                            }
                        }
                        else
                        {
                            wxSFShapeBase* pSrcShape = m_pManager->FindShape( pLine->GetSrcShapeId() );
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
				m_shpMultiEdit.ShowHandles(true);
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
			wxCShapeListNode *node = m_pManager->GetShapeList().GetFirst();
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
		}
		break;

    default:
        break;
	}
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

	wxASSERT_MSG( m_pManager, wxT("Shape manager has not been properly set in shape canvas."));

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
				wxCShapeListNode* node = m_pManager->GetShapeList().GetFirst();
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
				wxCShapeListNode* node = m_pManager->GetShapeList().GetFirst();
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
			m_pManager->RemoveShapes(m_lstSelection);
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
    wxASSERT_MSG( m_pManager, wxT("Shape manager has not been properly set in shape canvas."));

	if(scale != 0)m_nScale = scale;
	else
		m_nScale = 1;

	// rescale all bitmap shapes if neccessary
	CShapeList lstBitmaps;
	m_pManager->GetShapes(CLASSINFO(wxSFBitmapShape), lstBitmaps);

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

void wxSFShapeCanvas::LoadCanvas(const wxString& file)
{
    wxASSERT_MSG( m_pManager, wxT("Shape manager has not been properly set in shape canvas."));

    bool fChartLoaded = false;
	double nScale = 1;

    wxXmlDocument xmlDoc;
    xmlDoc.Load(file);

    wxXmlNode* root = xmlDoc.GetRoot();
    if(root)
    {
        m_pManager->Clear();
        ClearCanvasHistory();

        if(root->GetName() == wxT("chart"))
        {
            // the file contains only chart without any additional info
            m_pManager->DeserializeShapes(NULL, root);
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
                else if(child->GetName() == wxT("accepted_shapes"))
                {
                    m_pManager->ClearAcceptedShapes();

                    wxXmlNode* accept = child->GetChildren();
                    while(accept)
                    {
                        if(accept->GetName() == wxT("type"))
                        {
                           m_pManager->AcceptShape(accept->GetNodeContent());
                        }
                        accept = accept->GetNext();
                    }
                }
                else if(child->GetName() == wxT("chart"))
                {
                    // only one chart can be loaded at once
                    if(!fChartLoaded)
                    {
                        m_pManager->DeserializeShapes(NULL, child);
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
    wxASSERT_MSG( m_pManager, wxT("Shape manager has not been properly set in shape canvas."));

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

    // save acceptance list
    if( !m_pManager->GetAcceptedShapes().IsEmpty() && (m_pManager->GetAcceptedShapes().Index(wxT("All"))==wxNOT_FOUND) )
    {
         child = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("accepted_shapes"));
         for(size_t i = 0; i < m_pManager->GetAcceptedShapes().GetCount(); i++)
         {
             AddPropertyNode(child, wxT("type"), m_pManager->GetAcceptedShapes().Item(i));
         }
         root->AddChild(child);
    }

    // create chart node
    wxXmlNode* chart = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("chart"));
    m_pManager->SerializeShapes(NULL, chart, false);
    root->AddChild(chart);

    // create and save XML document
    wxXmlDocument xmlDoc;
    xmlDoc.SetRoot(root);
    xmlDoc.Save(file, 2);
}

void wxSFShapeCanvas::StartInteractiveConnection(wxClassInfo* shapeInfo, const wxPoint& pos)
{
    if((m_nWorkingMode == modeREADY) && shapeInfo->IsKindOf(CLASSINFO(wxSFLineShape)))
    {
        wxASSERT_MSG( m_pManager, wxT("Shape manager has not been properly set in shape canvas."));

        m_pNewLineShape = (wxSFLineShape*)m_pManager->AddShape(shapeInfo, sfDONT_SAVE_STATE);
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
}

void wxSFShapeCanvas::AbortInteractiveConnection()
{
    wxASSERT_MSG( m_pManager, wxT("Shape manager has not been properly set in shape canvas."));

    if(m_pNewLineShape)
    {
        m_pManager->RemoveShape(m_pNewLineShape);
        m_pNewLineShape = NULL;
        OnConnectionFinished(NULL);
    }
	m_nWorkingMode = modeREADY;
	Refresh(false);
}

void wxSFShapeCanvas::OnConnectionFinished(wxSFLineShape* connection)
{
	// HINT: override to perform user-defined actions...
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

int wxSFShapeCanvas::GetSelectedShapes(CShapeList& selection)
{
    wxASSERT_MSG( m_pManager, wxT("Shape manager has not been properly set in shape canvas."));

	selection.Clear();

	wxCShapeListNode* node = m_pManager->GetShapeList().GetFirst();
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
    wxASSERT_MSG( m_pManager, wxT("Shape manager has not been properly set in shape canvas."));

	int nCounter = 1;
	CShapeList m_lstSortedShapes;
	wxSFShapeBase* pShape;

    // sort shapes list in the way that the line shapes will be at the top of the list
	wxCShapeListNode *node = m_pManager->GetShapeList().GetFirst();
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
    wxASSERT_MSG( m_pManager, wxT("Shape manager has not been properly set in shape canvas."));

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
	wxCShapeListNode *node = m_pManager->GetShapeList().GetFirst();
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
    wxASSERT_MSG( m_pManager, wxT("Shape manager has not been properly set in shape canvas."));

	shapes.Clear();
	wxSFShapeBase *pShape;

	wxCShapeListNode *node = m_pManager->GetShapeList().GetFirst();
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
    wxASSERT_MSG( m_pManager, wxT("Shape manager has not been properly set in shape canvas."));

	shapes.Clear();
	wxSFShapeBase* pShape;

	wxCShapeListNode *node = m_pManager->GetShapeList().GetFirst();
	while(node)
	{
		pShape = node->GetData();
		if(pShape->IsVisible() && pShape->IsActive() && pShape->Intersects(rct))shapes.Append(pShape);
		node = node->GetNext();
	}

	return (int)shapes.GetCount();
}

void wxSFShapeCanvas::DeselectAll()
{
    wxASSERT_MSG( m_pManager, wxT("Shape manager has not been properly set in shape canvas."));

	wxCShapeListNode *node = m_pManager->GetShapeList().GetFirst();
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
    wxASSERT_MSG( m_pManager, wxT("Shape manager has not been properly set in shape canvas."));

	if(m_pManager->GetShapeList().GetCount() > 0)
	{
		wxCShapeListNode *node = m_pManager->GetShapeList().GetFirst();
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
    wxASSERT_MSG( m_pManager, wxT("Shape manager has not been properly set in shape canvas."));

	wxCShapeListNode *node = m_pManager->GetShapeList().GetFirst();
	while(node)
	{
		node->GetData()->ShowHandles(false);
		node = node->GetNext();
	}
}

void wxSFShapeCanvas::ValidateSelection(CShapeList& selection)
{
    wxASSERT_MSG( m_pManager, wxT("Shape manager has not been properly set in shape canvas."));

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

        m_pManager->GetShapeList().DeleteObject(pShape);
        m_pManager->GetShapeList().Insert(pShape);

        MoveChildrenToTop(pShape);

		node = node->GetPrevious();
	}
}

void wxSFShapeCanvas::MoveChildrenToTop(wxSFShapeBase* parent)
{
    wxASSERT_MSG( m_pManager, wxT("Shape manager has not been properly set in shape canvas."));

	CShapeList m_lstChildren;
	m_pManager->GetChildren(parent, m_lstChildren);

	wxCShapeListNode *node = m_lstChildren.GetLast();
	while(node)
	{
		wxSFShapeBase* pShape = node->GetData();

		m_pManager->GetShapeList().DeleteObject(pShape);
		m_pManager->GetShapeList().Insert(pShape);
		MoveChildrenToTop(pShape);

		node = node->GetPrevious();
	}
}

void wxSFShapeCanvas::UpdateMultieditSize()
{
	// calculate bounding box
	wxRect unionRct;
	//bool firstRun = true;
	CShapeList m_lstSelection;
	GetSelectedShapes(m_lstSelection);
	wxCShapeListNode *node = m_lstSelection.GetFirst();
	while(node)
	{
		if(node == m_lstSelection.GetFirst())
		{
			unionRct = node->GetData()->GetBoundingBox();
			//firstRun = false;
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
    wxASSERT_MSG( m_pManager, wxT("Shape manager has not been properly set in shape canvas."));

	m_nCommonHoverColor = col;

	// update Hover color in all existing shapes
	wxCShapeListNode * node = m_pManager->GetShapeList().GetFirst();
	while(node)
	{
		node->GetData()->SetHoverColour(col);
		node = node->GetNext();
	}
}

wxRect wxSFShapeCanvas::GetTotalBoundingBox() const
{
    wxASSERT_MSG( m_pManager, wxT("Shape manager has not been properly set in shape canvas."));

    wxRect virtRct;

    // calculate total bounding box (includes all shapes)
    wxCShapeListNode* node = m_pManager->GetShapeList().GetFirst();
    while(node)
    {
        if(node == m_pManager->GetShapeList().GetFirst())
        {
            virtRct = node->GetData()->GetBoundingBox();
        }
        else
            virtRct.Union(node->GetData()->GetBoundingBox());

        node = node->GetNext();
    }

    return virtRct;
}

wxRect wxSFShapeCanvas::GetSelectionBB()
{
    wxRect bbRct;

    // get selected shapes
    CShapeList lstSelection;
    GetSelectedShapes(lstSelection);

    wxCShapeListNode *node = lstSelection.GetFirst();
    while(node)
    {
        node->GetData()->GetCompleteBoundingBox(bbRct, wxSFShapeBase::bbSELF | wxSFShapeBase::bbCHILDREN | wxSFShapeBase::bbCONNECTIONS);
        node = node->GetNext();
    }

    return bbRct;
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
    wxASSERT_MSG( m_pManager, wxT("Shape manager has not been properly set in shape canvas."));

	wxSFContentCtrl* pTextCtrl = NULL;
	CShapeList lstShapes;
	m_pManager->GetShapes(CLASSINFO(wxSFEditTextShape), lstShapes);

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
    wxASSERT_MSG( m_pManager, wxT("Shape manager has not been properly set in shape canvas."));

	wxSFShapeBase *pShape;
	wxRealPoint shapePos;
	double minx = 0, miny = 0;

	// find the maximal negative position value
	wxCShapeListNode* node = m_pManager->GetShapeList().GetFirst();
	while(node)
	{
		shapePos = node->GetData()->GetAbsolutePosition();

		if(node == m_pManager->GetShapeList().GetFirst())
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
		node = m_pManager->GetShapeList().GetFirst();
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

void wxSFShapeCanvas::AlignSelected(HALIGN halign, VALIGN valign)
{
    int nCnt = 0;

    wxRealPoint min_pos, max_pos, pos;
    wxRect shapeBB, updRct;
    wxSFShapeBase *pShape, *pParent;

    CShapeList lstSelection;
    GetSelectedShapes(lstSelection);

    updRct = GetSelectionBB();
    updRct.Inflate(MEOFFSET, MEOFFSET);

    // find most distant position
    wxCShapeListNode *node = lstSelection.GetFirst();
    while(node)
    {
        pShape = node->GetData();

        if(!pShape->IsKindOf(CLASSINFO(wxSFLineShape)))
        {
            pos = pShape->GetAbsolutePosition();
            shapeBB = pShape->GetBoundingBox();

            if( nCnt == 0 )
            {
                min_pos = pos;
                max_pos.x = pos.x + shapeBB.GetWidth();
                max_pos.y = pos.y + shapeBB.GetHeight();
            }
            else
            {
                if( pos.x < min_pos.x )min_pos.x = pos.x;
                if( pos.y < min_pos.y )min_pos.y = pos.y;
                if( (pos.x + shapeBB.GetWidth()) > max_pos.x )max_pos.x = pos.x + shapeBB.GetWidth();
                if( (pos.y + shapeBB.GetHeight()) > max_pos.y )max_pos.y = pos.y + shapeBB.GetHeight();
            }

            nCnt++;
        }

        node = node->GetNext();
    }

    // if only one non-line shape is in the selection then alignment has no sense so exit...
    if(nCnt < 2) return;

    // set new positions
    node = lstSelection.GetFirst();
    while(node)
    {
        pShape = node->GetData();

        if(!pShape->IsKindOf(CLASSINFO(wxSFLineShape)))
        {
            pos = pShape->GetAbsolutePosition();
            shapeBB = pShape->GetBoundingBox();

            switch(halign)
            {
                case halignLEFT:
                    pShape->MoveTo(min_pos.x, pos.y);
                    break;

                case halignRIGHT:
                    pShape->MoveTo(max_pos.x - shapeBB.GetWidth(), pos.y);
                    break;

                case halignCENTER:
                    pShape->MoveTo((max_pos.x + min_pos.x)/2 - shapeBB.GetWidth()/2, pos.y);
                    break;

                default:
                    break;
            }

            switch(valign)
            {
                case valignTOP:
                    pShape->MoveTo(pos.x, min_pos.y);
                    break;

                case valignBOTTOM:
                    pShape->MoveTo(pos.x, max_pos.y - shapeBB.GetHeight());
                    break;

                case valignMIDDLE:
                    pShape->MoveTo(pos.x, (max_pos.y + min_pos.y)/2 - shapeBB.GetHeight()/2);
                    break;

                default:
                    break;
            }

            // update the shape and its parent
            pShape->Update();
            pParent = pShape->GetParentShape();
            if(pParent)
            {
                pParent->Update();
            }
        }

        node = node->GetNext();
    }

    if(!updRct.IsEmpty())
    {
        UpdateMultieditSize();
        SaveCanvasState();
        RefreshCanvas(false, updRct);
    }
}

//----------------------------------------------------------------------------------//
// Clipboard functions
//----------------------------------------------------------------------------------//

void wxSFShapeCanvas::Copy()
{
    wxASSERT_MSG( m_pManager, wxT("Shape manager has not been properly set in shape canvas."));

	// copy selected shapes to the clipboard
	if(wxTheClipboard->Open())
	{
		CShapeList lstSelection;
		GetSelectedShapes(lstSelection);

		wxSFShapeDataObject* dataObj = new wxSFShapeDataObject(m_formatShapes, lstSelection, m_pManager);
		wxTheClipboard->SetData(dataObj);
		wxTheClipboard->Close();
	}
}

void wxSFShapeCanvas::Cut()
{
    wxASSERT_MSG( m_pManager, wxT("Shape manager has not been properly set in shape canvas."));

	Copy();

	// remove selected shapes
	CShapeList lstSelection;
	GetSelectedShapes(lstSelection);

	m_pManager->RemoveShapes(lstSelection);
	m_shpMultiEdit.Show(false);
	SaveCanvasState();
	Refresh(false);
}

void wxSFShapeCanvas::Paste()
{
    wxASSERT_MSG( m_pManager, wxT("Shape manager has not been properly set in shape canvas."));

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
				m_pManager->DeserializeChartFromXml(instream);

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

bool wxSFShapeCanvas::CanAlignSelected()
{
    return ( m_shpMultiEdit.IsVisible() & (m_nWorkingMode == modeREADY) );
}

void wxSFShapeCanvas::ClearCanvasHistory()
{
	m_CanvasHistory.Clear();
}
