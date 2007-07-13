#include "FrameCanvas.h"
#include "MainFrame.h"

#include <wx/textdlg.h>

CFrameCanvas::CFrameCanvas(wxWindow* parent, wxWindowID id)
: wxSFShapeCanvas(parent, id, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxSTATIC_BORDER)
{
	// initialize grid
	UseGrid(true);
	ShowGrid(true);

	// initialize data members
	m_pParentFrame = (CMainFrame*)parent;
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
				wxSFBitmapShape* pShape = (wxSFBitmapShape*)AddShape(CLASSINFO(wxSFBitmapShape), event.GetPosition(), sfDONT_SAVE_STATE);
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
					pText = (wxSFTextShape*)AddShape(CLASSINFO(wxSFTextShape), event.GetPosition(), sfDONT_SAVE_STATE);
				else
					pText = (wxSFEditTextShape*)AddShape(CLASSINFO(wxSFEditTextShape), event.GetPosition(), sfDONT_SAVE_STATE);
                if(pText)
                {
					pText->SetText(dlg.GetValue());
					SaveCanvasState();
                    pText->Refresh();
                }
                if(!event.ControlDown())
                {
                    m_pParentFrame->SetToolMode(CMainFrame::modeDESIGN);
                }

                // set shapes policy
                pText->AcceptConnection(wxT("All"));
                pText->AcceptSrcNeighbour(wxT("All"));
                pText->AcceptTrgNeighbour(wxT("wxSFTextShape"));
                pText->AcceptTrgNeighbour(wxT("wxSFEditTextShape"));
		    }
		}
		break;

	case CMainFrame::modeDIAMOND:
		{
			wxSFShapeBase* pShape = AddShape(CLASSINFO(wxSFDiamondShape), event.GetPosition());
			if(pShape)
			{
			    // set shape policy
				pShape->AcceptChild(wxT("wxSFTextShape"));
				pShape->AcceptChild(wxT("wxSFEditTextShape"));

				pShape->AcceptConnection(wxT("All"));
                pShape->AcceptSrcNeighbour(wxT("All"));
                pShape->AcceptTrgNeighbour(wxT("All"));
			}
            if(!event.ControlDown())
            {
                m_pParentFrame->SetToolMode(CMainFrame::modeDESIGN);
            }
		}
		break;

	case CMainFrame::modeFIXEDRECT:
		{
		    wxSFShapeBase* pShape = AddShape(CLASSINFO(wxSFSquareShape), event.GetPosition());
		    if(pShape)
			{
			    // set shape policy
				pShape->AcceptChild(wxT("wxSFTextShape"));
				pShape->AcceptChild(wxT("wxSFEditTextShape"));

				pShape->AcceptConnection(wxT("All"));
                pShape->AcceptSrcNeighbour(wxT("All"));
                pShape->AcceptTrgNeighbour(wxT("All"));
			}
            if(!event.ControlDown())
            {
                m_pParentFrame->SetToolMode(CMainFrame::modeDESIGN);
            }
		}
		break;

	case CMainFrame::modeROUNDRECT:
		{
			wxSFShapeBase* pShape = AddShape(CLASSINFO(wxSFRoundRectShape), event.GetPosition());
			if(pShape)
			{
			    // set shape policy
				pShape->AcceptChild(wxT("wxSFTextShape"));
				pShape->AcceptChild(wxT("wxSFEditTextShape"));

				pShape->AcceptConnection(wxT("All"));
                pShape->AcceptSrcNeighbour(wxT("All"));
                pShape->AcceptTrgNeighbour(wxT("All"));
			}
            if(!event.ControlDown())
            {
                m_pParentFrame->SetToolMode(CMainFrame::modeDESIGN);
            }
		}
		break;

	case CMainFrame::modeRECT:
		{
			wxSFShapeBase* pShape = AddShape(CLASSINFO(wxSFRectShape), event.GetPosition());
			if(pShape)
			{
			    // set shape policy
				pShape->AcceptChild(wxT("All"));

				pShape->AcceptConnection(wxT("All"));
                pShape->AcceptSrcNeighbour(wxT("All"));
                pShape->AcceptTrgNeighbour(wxT("All"));
			}

            if(!event.ControlDown())
            {
                m_pParentFrame->SetToolMode(CMainFrame::modeDESIGN);
            }
		}
		break;

	case CMainFrame::modeELLIPSE:
		{
			wxSFShapeBase* pShape = AddShape(CLASSINFO(wxSFEllipseShape), event.GetPosition());
			if(pShape)
			{
			    // set shape policy
				pShape->AcceptChild(wxT("wxSFTextShape"));
				pShape->AcceptChild(wxT("wxSFEditTextShape"));

				pShape->AcceptConnection(wxT("All"));
                pShape->AcceptSrcNeighbour(wxT("All"));
                pShape->AcceptTrgNeighbour(wxT("All"));
			}
            if(!event.ControlDown())
            {
                m_pParentFrame->SetToolMode(CMainFrame::modeDESIGN);
            }
		}
		break;

	case CMainFrame::modeCIRCLE:
		{
			wxSFShapeBase* pShape = AddShape(CLASSINFO(wxSFCircleShape), event.GetPosition());
			if(pShape)
			{
			    // set shape policy
			    pShape->EnableSizeChange(false);
			    pShape->EnablePositionChange(false);
			    pShape->AcceptChild(wxT("wxSFTextShape"));
				pShape->AcceptChild(wxT("wxSFEditTextShape"));

				pShape->AcceptConnection(wxT("All"));
                pShape->AcceptSrcNeighbour(wxT("All"));
                pShape->AcceptTrgNeighbour(wxT("All"));
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
        CShapeList lstShapes;
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
            wxCShapeListNode* node = lstShapes.GetFirst();
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
        pShape->GetNeighbours(lstShapes, wxSFShapeBase::lineBOTH, sfINDIRECT);
        if( lstShapes.GetCount() > 0 )
        {
            msg += wxT("\nNeighbours:\n");
            wxCShapeListNode *node = lstShapes.GetFirst();
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
