#include "wx_pch.h"

#include "FrameCanvas.h"
#include "MainFrame.h"
#include "MainApp.h"

#include <wx/textdlg.h>
#include <wx/filename.h>

FrameCanvas::FrameCanvas(wxSFDiagramManager* manager, wxWindow* parent, wxWindowID id)
: wxSFShapeCanvas(manager, parent, id, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxSTATIC_BORDER)
{
    // get pointer to main application frame
	m_pParentFrame = (MainFrm*)wxGetApp().GetTopWindow();

	// initialize grid

	//UseGrid(true); !!! DEPRECATED !!!
	//ShowGrid(true); !!! DEPRECATED !!!

	AddStyle(sfsGRID_USE);
	AddStyle(sfsGRID_SHOW);

	// canvas background can be printed/ommited during the canvas printing job
    //AddStyle(sfsPRINT_BACKGROUND);

    // adjust the printed drawing align and style if needed
    //SetPrintVAlign(valignTOP);
    //SetPrintHAlign(halignLEFT);
    //SetPrintMode(prnMAP_TO_MARGINS);

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

	// set accepted shapes
	GetDiagramManager()->ClearAcceptedShapes();
	GetDiagramManager()->AcceptShape(wxT("All"));

	// you can set also the canvas history manager working mode:
	// 1) FASTER, but requires implementation of xsSerializable::Clone() virtual function
	// in all classes derived from the xsSerializable class
	GetHistoryManager().SetMode(wxSFCanvasHistory::histUSE_CLONING);
	// 2) SLOWER, but no other programming overhead is required (except implementation
	// of standard serialization functionality). This working mode is default.
	// GetHistoryManager().SetMode(wxSFCanvasHistory::histUSE_SERIALIZATION);

	// change of the canvas history manager working mode clears the stored states
	// so we should save initial canvas state manually
	SaveCanvasState();
}

FrameCanvas::~FrameCanvas(void)
{
}

//----------------------------------------------------------------------------------//
// public virtual functions
//----------------------------------------------------------------------------------//

void FrameCanvas::OnLeftDown(wxMouseEvent& event)
{
    wxSFShapeBase *pShape = NULL;

	switch(m_pParentFrame->GetToolMode())
	{
	case MainFrm::modeBITMAP:
		{
			wxFileDialog dlg(this, wxT("Load bitmap image..."), wxGetCwd(), wxT(""), wxT("BMP Files (*.bmp)|*.bmp"), wxOPEN  | wxFD_FILE_MUST_EXIST);

			if(dlg.ShowModal() == wxID_OK)
			{
				pShape = GetDiagramManager()->AddShape(CLASSINFO(wxSFBitmapShape), event.GetPosition(), sfDONT_SAVE_STATE);
				if(pShape)
				{
				    // create relative path
				    wxFileName path( dlg.GetPath() );
				    path.MakeRelativeTo( wxGetCwd() );

                    // create image from BMP file
					((wxSFBitmapShape*)pShape)->CreateFromFile( path.GetFullPath(), wxBITMAP_TYPE_BMP );

					// set shape policy
					pShape->AcceptConnection(wxT("All"));
					pShape->AcceptSrcNeighbour(wxT("All"));
					pShape->AcceptTrgNeighbour(wxT("All"));
				}
			}
		}
		break;
	case MainFrm::modeTEXT:
	case MainFrm::modeEDITTEXT:
		{
		    wxTextEntryDialog dlg(this, wxT(""), wxT("Enter text"), wxT("Hello World!"));

		    if(dlg.ShowModal() == wxID_OK)
		    {
				if(m_pParentFrame->GetToolMode() == MainFrm::modeTEXT)
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
                }
		    }
		}
		break;

	case MainFrm::modeDIAMOND:
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

	case MainFrm::modeFIXEDRECT:
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

	case MainFrm::modeROUNDRECT:
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

	case MainFrm::modeRECT:
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

	case MainFrm::modeGRID:
	case MainFrm::modeFLEXGRID:
		{
		    if( m_pParentFrame->GetToolMode() == MainFrm::modeGRID )
                pShape = GetDiagramManager()->AddShape(CLASSINFO(wxSFGridShape), event.GetPosition(), sfDONT_SAVE_STATE);
		    else
                pShape = GetDiagramManager()->AddShape(CLASSINFO(wxSFFlexGridShape), event.GetPosition(), sfDONT_SAVE_STATE);

			if(pShape)
			{
			    wxSFGridShape *pGrid = (wxSFGridShape*)pShape;

                // set visual style
			    pGrid->SetFill(*wxTRANSPARENT_BRUSH);
			    pGrid->SetBorder(wxPen(*wxBLACK, 1, wxDOT));

			    // number of rows and columns cas be set here (default grid dimension is 3x3) ...
			    //pGrid->SetDimensions(2, 2);

			    // ... as well as the cell spaces (default grid cellspace is 5).
			    //pGrid->SetCellSpace(0);

			    // set shape policy
				pGrid->AcceptChild(wxT("All"));

				pGrid->AcceptConnection(wxT("All"));
                pGrid->AcceptSrcNeighbour(wxT("All"));
                pGrid->AcceptTrgNeighbour(wxT("All"));

			    // insert some shapes into the grid programmably (it can be also done interactively by drag'n'drop operations)
			    pGrid->AppendToGrid(GetDiagramManager()->AddShape(CLASSINFO(wxSFEllipseShape), sfDONT_SAVE_STATE));
			    pGrid->AppendToGrid(GetDiagramManager()->AddShape(CLASSINFO(wxSFDiamondShape), sfDONT_SAVE_STATE));
                // shapes can be also inserted before given lexicographic position (at the first position in this case) in this way ...
			    pGrid->InsertToGrid(0, GetDiagramManager()->AddShape(CLASSINFO(wxSFRoundRectShape), sfDONT_SAVE_STATE));
			    // ... or can replace previously assigned shape at the position specified by row and column indexes
			    // (note that the previous shape at the given position (if exists) will be moved to the grid's last lexicographic position).
			    pGrid->InsertToGrid(1, 0, GetDiagramManager()->AddShape(CLASSINFO(wxSFCircleShape), sfDONT_SAVE_STATE));

                // update the grid
			    pGrid->Update();
			}
		}
		break;

	case MainFrm::modeELLIPSE:
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

	case MainFrm::modeCIRCLE:
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

    case MainFrm::modeLINE:
        {
            if(GetMode() == modeREADY)
            {
                StartInteractiveConnection(CLASSINFO(wxSFLineShape), event.GetPosition());
            }
            else
                wxSFShapeCanvas::OnLeftDown(event);
        }
        break;

     case MainFrm::modeCURVE:
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
            m_pParentFrame->SetToolMode(MainFrm::modeDESIGN);
        }

        pShape->Refresh();
	}
}

void FrameCanvas::OnRightDown(wxMouseEvent& event)
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

void FrameCanvas::OnKeyDown(wxKeyEvent& event)
{
	switch(event.GetKeyCode())
	{
	case WXK_ESCAPE:
		m_pParentFrame->SetToolMode(MainFrm::modeDESIGN);
		break;
	}

	// do default actions
	wxSFShapeCanvas::OnKeyDown(event);
}

void FrameCanvas::OnConnectionFinished(wxSFLineShape* connection)
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

		m_pParentFrame->SetToolMode(MainFrm::modeDESIGN);
    }
}
