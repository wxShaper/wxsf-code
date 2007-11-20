#include "wx_pch.h"

#include "FrameCanvas.h"
#include "MainFrame.h"

#include <wx/textdlg.h>

CFrameCanvas::CFrameCanvas(wxSFDiagramManager* manager, wxWindow* parent, wxWindowID id)
: wxSFShapeCanvas(manager, parent, id, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxSTATIC_BORDER)
{
	// initialize grid
	UseGrid(true);
	ShowGrid(true);

	// initialize data members
	m_pParentFrame = (CMainFrame*)parent;

	// set accepted shapes
	GetDiagramManager()->ClearAcceptedShapes();
	GetDiagramManager()->AcceptShape(wxT("All"));
}

CFrameCanvas::~CFrameCanvas(void)
{
}

//----------------------------------------------------------------------------------//
// public virtual functions
//----------------------------------------------------------------------------------//

void CFrameCanvas::OnLeftDown(wxMouseEvent& event)
{
	switch(m_pParentFrame->GetToolMode())
	{
	case CMainFrame::modeBITMAP:
		{
			wxFileDialog dlg(this, wxT("Load bitmap image..."), wxGetCwd(), wxT(""), wxT("BMP Files (*.bmp) | *.bmp"), wxOPEN);

			if(dlg.ShowModal() == wxID_OK)
			{
				wxSFBitmapShape* pShape = (wxSFBitmapShape*)GetDiagramManager()->AddShape(CLASSINFO(wxSFBitmapShape), event.GetPosition(), sfDONT_SAVE_STATE);
				if(pShape)
				{
					pShape->CreateFromFile(dlg.GetPath());

					// set shape policy
					pShape->AcceptConnection(wxT("All"));
					pShape->AcceptSrcNeighbour(wxT("All"));
					pShape->AcceptTrgNeighbour(wxT("All"));

					SaveCanvasState();
				}
				if(!event.ControlDown())
				{
					m_pParentFrame->SetToolMode(CMainFrame::modeDESIGN);
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
                wxSFTextShape* pText = NULL;

				if(m_pParentFrame->GetToolMode() == CMainFrame::modeTEXT)
					pText = (wxSFTextShape*)GetDiagramManager()->AddShape(CLASSINFO(wxSFTextShape), event.GetPosition(), sfDONT_SAVE_STATE);
				else
					pText = (wxSFEditTextShape*)GetDiagramManager()->AddShape(CLASSINFO(wxSFEditTextShape), event.GetPosition(), sfDONT_SAVE_STATE);

                if(pText)
                {
					pText->SetText(dlg.GetValue());

                    // set alignment
                    pText->SetVAlign(wxSFShapeBase::valignTOP);
                    pText->SetHAlign(wxSFShapeBase::halignCENTER);
                    pText->SetVBorder(10);

                    // set shapes policy
                    pText->AcceptConnection(wxT("All"));
                    pText->AcceptSrcNeighbour(wxT("All"));
                    pText->AcceptTrgNeighbour(wxT("wxSFTextShape"));
                    pText->AcceptTrgNeighbour(wxT("wxSFEditTextShape"));

					SaveCanvasState();

                    pText->Refresh();
                }
                if(!event.ControlDown())
                {
                    m_pParentFrame->SetToolMode(CMainFrame::modeDESIGN);
                }
		    }
		}
		break;

	case CMainFrame::modeDIAMOND:
		{
			wxSFShapeBase* pShape = GetDiagramManager()->AddShape(CLASSINFO(wxSFDiamondShape), event.GetPosition(), sfDONT_SAVE_STATE);
			if(pShape)
			{
			    // set shape policy
				pShape->AcceptChild(wxT("wxSFTextShape"));
				pShape->AcceptChild(wxT("wxSFEditTextShape"));

				pShape->AcceptConnection(wxT("All"));
                pShape->AcceptSrcNeighbour(wxT("All"));
                pShape->AcceptTrgNeighbour(wxT("All"));

                SaveCanvasState();
			}
            if(!event.ControlDown())
            {
                m_pParentFrame->SetToolMode(CMainFrame::modeDESIGN);
            }
		}
		break;

	case CMainFrame::modeFIXEDRECT:
		{
		    wxSFShapeBase* pShape = GetDiagramManager()->AddShape(CLASSINFO(wxSFSquareShape), event.GetPosition(), sfDONT_SAVE_STATE);
		    if(pShape)
			{
			    // set shape policy
				pShape->AcceptChild(wxT("wxSFTextShape"));
				pShape->AcceptChild(wxT("wxSFEditTextShape"));

				pShape->AcceptConnection(wxT("All"));
                pShape->AcceptSrcNeighbour(wxT("All"));
                pShape->AcceptTrgNeighbour(wxT("All"));

                SaveCanvasState();
			}
            if(!event.ControlDown())
            {
                m_pParentFrame->SetToolMode(CMainFrame::modeDESIGN);
            }
		}
		break;

	case CMainFrame::modeROUNDRECT:
		{
			wxSFShapeBase* pShape = GetDiagramManager()->AddShape(CLASSINFO(wxSFRoundRectShape), event.GetPosition(), sfDONT_SAVE_STATE);
			if(pShape)
			{
			    // set shape policy
				pShape->AcceptChild(wxT("wxSFTextShape"));
				pShape->AcceptChild(wxT("wxSFEditTextShape"));

				pShape->AcceptConnection(wxT("All"));
                pShape->AcceptSrcNeighbour(wxT("All"));
                pShape->AcceptTrgNeighbour(wxT("All"));

                SaveCanvasState();
			}
            if(!event.ControlDown())
            {
                m_pParentFrame->SetToolMode(CMainFrame::modeDESIGN);
            }
		}
		break;

	case CMainFrame::modeRECT:
		{
			wxSFShapeBase* pShape = GetDiagramManager()->AddShape(CLASSINFO(wxSFRectShape), event.GetPosition(), sfDONT_SAVE_STATE);
			if(pShape)
			{
			    // set shape policy
				pShape->AcceptChild(wxT("All"));

				pShape->AcceptConnection(wxT("All"));
                pShape->AcceptSrcNeighbour(wxT("All"));
                pShape->AcceptTrgNeighbour(wxT("All"));

                SaveCanvasState();
			}

            if(!event.ControlDown())
            {
                m_pParentFrame->SetToolMode(CMainFrame::modeDESIGN);
            }
		}
		break;

	case CMainFrame::modeELLIPSE:
		{
			wxSFShapeBase* pShape = GetDiagramManager()->AddShape(CLASSINFO(wxSFEllipseShape), event.GetPosition(), sfDONT_SAVE_STATE);
			if(pShape)
			{
			    // set shape policy
				pShape->AcceptChild(wxT("wxSFTextShape"));
				pShape->AcceptChild(wxT("wxSFEditTextShape"));

				pShape->AcceptConnection(wxT("All"));
                pShape->AcceptSrcNeighbour(wxT("All"));
                pShape->AcceptTrgNeighbour(wxT("All"));

                SaveCanvasState();
			}
            if(!event.ControlDown())
            {
                m_pParentFrame->SetToolMode(CMainFrame::modeDESIGN);
            }
		}
		break;

	case CMainFrame::modeCIRCLE:
		{
			wxSFShapeBase* pShape = GetDiagramManager()->AddShape(CLASSINFO(wxSFCircleShape), event.GetPosition(), sfDONT_SAVE_STATE);
			if(pShape)
			{
			    // set shape policy
			    pShape->RemoveStyle(wxSFShapeBase::sfsSIZE_CHANGE);

			    pShape->AcceptChild(wxT("wxSFTextShape"));
				pShape->AcceptChild(wxT("wxSFEditTextShape"));

				pShape->AcceptConnection(wxT("All"));
                pShape->AcceptSrcNeighbour(wxT("All"));
                pShape->AcceptTrgNeighbour(wxT("All"));

                SaveCanvasState();
			}
            if(!event.ControlDown())
            {
                m_pParentFrame->SetToolMode(CMainFrame::modeDESIGN);
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
}

void CFrameCanvas::OnRightDown(wxMouseEvent& event)
{
    // try to find shape under cursor
    wxSFShapeBase *pShape = GetShapeAtPosition(DP2LP(event.GetPosition()), 1, searchBOTH);

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
        pShape->GetChildren(lstShapes, sfRECURSIVE);
        if( lstShapes.GetCount() > 0 )
        {
            msg += wxT("\nChildren:\n");
            wxShapeListNode* node = lstShapes.GetFirst();
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
            wxShapeListNode *node = lstShapes.GetFirst();
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
