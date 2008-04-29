#include "wx_pch.h"

#include "FrameCanvas.h"
#include "MainFrame.h"

#include <wx/textdlg.h>

CFrameCanvas::CFrameCanvas(wxSFDiagramManager* manager, wxWindow* parent, wxWindowID id)
: wxSFShapeCanvas(manager, parent, id, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxSTATIC_BORDER)
{
	// initialize grid

	//UseGrid(true); !!! DEPRECATED !!!
	//ShowGrid(true); !!! DEPRECATED !!!

	AddStyle(sfsGRID_USE);
	AddStyle(sfsGRID_SHOW);

	// the canvas background can be filled with a solid colour ...
	//RemoveStyle(sfsGRADIENT_BACKGROUND);
	//SetBackgroundColour(sfdvSHAPECANVAS_BACKGROUNDCOLOR);
	// ... or by a gradient fill
	AddStyle(sfsGRADIENT_BACKGROUND);
	SetGradientFrom(sfdvSHAPECANVAS_GRADIENT_FROM);
	SetGradientTo(sfdvSHAPECANVAS_GRADIENT_TO);
	// also shadows style can be set here:
	//SetShadowFill(wxBrush(wxColour(100, 100, 100), wxCROSSDIAG_HATCH)); // standard values can be sfdvSHAPECANVAS_SHADOWBRUSH or sfdvSHAPECANVAS_SHADOWCOLOR
	//SetShadowOffset(wxRealPoint(7, 7));

	// now you can use also these styles...

	// RemoveStyle(sfsHOVERING);
	// RemoveStyle(sfsHIGHLIGHTING);
	// RemoveStyle(sfsUNDOREDO);
	// RemoveStyle(sfsDND);
	// RemoveStyle(sfsCLIPBOARD);
	// RemoveStyle(sfsMULTI_SIZE_CHANGE);
	// RemoveStyle(sfsMULTI_SELECTION);

	// a style flag presence can be tested like this:
	// if( ContainsStyle(sfsGRID_USE) ) DoSomething();

	// multiple styles can be set in this way:
	// SetStyle( sfsGRID_USE | sfsGRID_SHOW ) ... or ...
	// SetStyle( sfsDEFAULT_CANVAS_STYLE )

	// initialize data members
	m_pParentFrame = (CMainFrame*)parent;

	// set accepted shapes
	GetDiagramManager()->ClearAcceptedShapes();
	GetDiagramManager()->AcceptShape(wxT("All"));

	// you can set also the canvas history manager working mode:
	// FASTER, but requires implementation of xsSerializable::Clone() virtual function
	// in all classes derived from the xsSerializable class
	GetHistoryManager().SetMode(wxSFCanvasHistory::histUSE_CLONING);
	// change of the canvas history manager working mode clears the stored states
	// so we should save initial canvas state manually
	SaveCanvasState();
	// SLOWER, but no other programming overhead is required (except implementing
	// of standard serialization functionality). This working mode is default.
	// GetHistoryManager().SetMode(wxSFCanvasHistory::histUSE_SERIALIZATION);
}

CFrameCanvas::~CFrameCanvas(void)
{
}

//----------------------------------------------------------------------------------//
// public virtual functions
//----------------------------------------------------------------------------------//

void CFrameCanvas::OnLeftDown(wxMouseEvent& event)
{
    wxSFShapeBase *pShape = NULL;

	switch(m_pParentFrame->GetToolMode())
	{
	case CMainFrame::modeBITMAP:
		{
			wxFileDialog dlg(this, wxT("Load bitmap image..."), wxGetCwd(), wxT(""), wxT("BMP Files (*.bmp) | *.bmp"), wxOPEN);

			if(dlg.ShowModal() == wxID_OK)
			{
				pShape = GetDiagramManager()->AddShape(CLASSINFO(wxSFBitmapShape), event.GetPosition(), sfDONT_SAVE_STATE);
				if(pShape)
				{
					((wxSFBitmapShape*)pShape)->CreateFromFile(dlg.GetPath());

					// set shape policy
					pShape->AcceptConnection(wxT("All"));
					pShape->AcceptSrcNeighbour(wxT("All"));
					pShape->AcceptTrgNeighbour(wxT("All"));
				}
			}
		}
		break;
	case CMainFrame::modeTEXT:
	case CMainFrame::modeEDITTEXT:
		{
		    wxTextEntryDialog dlg(this, wxT(""), wxT("Enter text"), wxT("Hello World!"));

		    if(dlg.ShowModal() == wxID_OK)
		    {
				if(m_pParentFrame->GetToolMode() == CMainFrame::modeTEXT)
					pShape = GetDiagramManager()->AddShape(CLASSINFO(wxSFTextShape), event.GetPosition(), sfDONT_SAVE_STATE);
				else
					pShape = GetDiagramManager()->AddShape(CLASSINFO(wxSFEditTextShape), event.GetPosition(), sfDONT_SAVE_STATE);

                if(pShape)
                {
					((wxSFTextShape*)pShape)->SetText(dlg.GetValue());

                    // set alignment
                    pShape->SetVAlign(wxSFShapeBase::valignTOP);
                    pShape->SetHAlign(wxSFShapeBase::halignCENTER);
                    pShape->SetVBorder(10);

                    // set shapes policy
                    pShape->AcceptConnection(wxT("All"));
                    pShape->AcceptSrcNeighbour(wxT("All"));
                    pShape->AcceptTrgNeighbour(wxT("wxSFTextShape"));
                    pShape->AcceptTrgNeighbour(wxT("wxSFEditTextShape"));

                    pShape->Refresh();
                }
		    }
		}
		break;

	case CMainFrame::modeDIAMOND:
		{
			pShape = GetDiagramManager()->AddShape(CLASSINFO(wxSFDiamondShape), event.GetPosition(), sfDONT_SAVE_STATE);
			if(pShape)
			{
			    // set shape policy
				pShape->AcceptChild(wxT("wxSFTextShape"));
				pShape->AcceptChild(wxT("wxSFEditTextShape"));

				pShape->AcceptConnection(wxT("All"));
                pShape->AcceptSrcNeighbour(wxT("All"));
                pShape->AcceptTrgNeighbour(wxT("All"));
			}
		}
		break;

	case CMainFrame::modeFIXEDRECT:
		{
		    pShape = GetDiagramManager()->AddShape(CLASSINFO(wxSFSquareShape), event.GetPosition(), sfDONT_SAVE_STATE);
		    if(pShape)
			{
			    // set shape policy
				pShape->AcceptChild(wxT("wxSFTextShape"));
				pShape->AcceptChild(wxT("wxSFEditTextShape"));

				pShape->AcceptConnection(wxT("All"));
                pShape->AcceptSrcNeighbour(wxT("All"));
                pShape->AcceptTrgNeighbour(wxT("All"));
			}
		}
		break;

	case CMainFrame::modeROUNDRECT:
		{
			pShape = GetDiagramManager()->AddShape(CLASSINFO(wxSFRoundRectShape), event.GetPosition(), sfDONT_SAVE_STATE);
			if(pShape)
			{
			    // set shape policy
				pShape->AcceptChild(wxT("wxSFTextShape"));
				pShape->AcceptChild(wxT("wxSFEditTextShape"));

				pShape->AcceptConnection(wxT("All"));
                pShape->AcceptSrcNeighbour(wxT("All"));
                pShape->AcceptTrgNeighbour(wxT("All"));
			}
		}
		break;

	case CMainFrame::modeRECT:
		{
			pShape = GetDiagramManager()->AddShape(CLASSINFO(wxSFRectShape), event.GetPosition(), sfDONT_SAVE_STATE);
			if(pShape)
			{
			    // set shape policy
				pShape->AcceptChild(wxT("All"));

				pShape->AcceptConnection(wxT("All"));
                pShape->AcceptSrcNeighbour(wxT("All"));
                pShape->AcceptTrgNeighbour(wxT("All"));
			}
		}
		break;

	case CMainFrame::modeELLIPSE:
		{
			pShape = GetDiagramManager()->AddShape(CLASSINFO(wxSFEllipseShape), event.GetPosition(), sfDONT_SAVE_STATE);
			if(pShape)
			{
			    // set shape policy
				pShape->AcceptChild(wxT("wxSFTextShape"));
				pShape->AcceptChild(wxT("wxSFEditTextShape"));

				pShape->AcceptConnection(wxT("All"));
                pShape->AcceptSrcNeighbour(wxT("All"));
                pShape->AcceptTrgNeighbour(wxT("All"));
			}
		}
		break;

	case CMainFrame::modeCIRCLE:
		{
			pShape = GetDiagramManager()->AddShape(CLASSINFO(wxSFCircleShape), event.GetPosition(), sfDONT_SAVE_STATE);
			if(pShape)
			{
			    // set shape policy
			    pShape->RemoveStyle(wxSFShapeBase::sfsSIZE_CHANGE);

			    pShape->AcceptChild(wxT("wxSFTextShape"));
				pShape->AcceptChild(wxT("wxSFEditTextShape"));

				pShape->AcceptConnection(wxT("All"));
                pShape->AcceptSrcNeighbour(wxT("All"));
                pShape->AcceptTrgNeighbour(wxT("All"));
			}
		}
		break;

    case CMainFrame::modeLINE:
        {
            if(GetMode() == modeREADY)
            {
                StartInteractiveConnection(CLASSINFO(wxSFLineShape), event.GetPosition());
            }
            else
                wxSFShapeCanvas::OnLeftDown(event);
        }
        break;

     case CMainFrame::modeCURVE:
        {
            if(GetMode() == modeREADY)
            {
                StartInteractiveConnection(CLASSINFO(wxSFCurveShape), event.GetPosition());
            }
            else
                wxSFShapeCanvas::OnLeftDown(event);
        }
        break;

	default:
		// do default actions
		wxSFShapeCanvas::OnLeftDown(event);
	}

	if( pShape )
	{
	    ShowShadows(m_pParentFrame->m_fShowShadows, shadowALL);

	    SaveCanvasState();

        if(!event.ControlDown())
        {
            m_pParentFrame->SetToolMode(CMainFrame::modeDESIGN);
        }
	}
}

void CFrameCanvas::OnRightDown(wxMouseEvent& event)
{
    // try to find shape under cursor
	wxSFShapeBase* pShape = GetShapeUnderCursor();
	// eventualy you can use:
    //wxSFShapeBase *pShape = GetShapeAtPosition(DP2LP(event.GetPosition()), 1, searchBOTH);

    // print out information about the shape (if found)
    if(pShape)
    {
        ShapeList lstShapes;
        wxString msg;
        wxSFShapeBase *pChild;
        int counter;

        // show basic info
        msg.Printf(wxT("Class name: %s, ID: %d\n"), pShape->GetClassInfo()->GetClassName(), pShape->GetId());

        // show info about shape's children
        counter = 1;
        pShape->GetChildShapes(lstShapes, sfRECURSIVE);
        if( lstShapes.GetCount() > 0 )
        {
            msg += wxT("\nChildren:\n");
            ShapeList::compatibility_iterator node = lstShapes.GetFirst();
            while(node)
            {
                pChild = node->GetData();

                msg += wxString::Format(wxT("%d. Class name: %s, ID: %d\n"), counter, pChild->GetClassInfo()->GetClassName(), pChild->GetId());
                counter++;

                node = node->GetNext();
            }
        }

        // show info about shape's neighbours
        counter = 1;
        lstShapes.Clear();
        pShape->GetNeighbours(lstShapes, CLASSINFO(wxSFLineShape), wxSFShapeBase::lineBOTH, sfINDIRECT);
        if( lstShapes.GetCount() > 0 )
        {
            msg += wxT("\nNeighbours:\n");
            ShapeList::compatibility_iterator node = lstShapes.GetFirst();
            while(node)
            {
                pChild = node->GetData();

                msg += wxString::Format(wxT("%d. Class name: %s, ID: %d\n"), counter, pChild->GetClassInfo()->GetClassName(), pChild->GetId());
                counter++;

                node = node->GetNext();
            }
        }

        // show message
        wxMessageBox(msg, wxT("wxShapeFramework"), wxOK | wxICON_INFORMATION);
    }
    else
        wxMessageBox(wxT("No shape found on this position."), wxT("wxShapeFramework"), wxOK | wxICON_INFORMATION);

    // call default handler
    wxSFShapeCanvas::OnRightDown(event);
}

void CFrameCanvas::OnKeyDown(wxKeyEvent& event)
{
	switch(event.GetKeyCode())
	{
	case WXK_ESCAPE:
		m_pParentFrame->SetToolMode(CMainFrame::modeDESIGN);
		break;
	}

	// do default actions
	wxSFShapeCanvas::OnKeyDown(event);
}

void CFrameCanvas::OnConnectionFinished(wxSFLineShape* connection)
{
    if(connection)
    {
        //connection->SetSrcArrow(CLASSINFO(wxSFOpenArrow));
        connection->SetTrgArrow(CLASSINFO(wxSFSolidArrow));

        connection->AcceptChild(wxT("wxSFTextShape"));
        connection->AcceptChild(wxT("wxSFEditTextShape"));

		connection->AcceptConnection(wxT("All"));
        connection->AcceptSrcNeighbour(wxT("All"));
        connection->AcceptTrgNeighbour(wxT("All"));

		m_pParentFrame->SetToolMode(CMainFrame::modeDESIGN);
    }
}
