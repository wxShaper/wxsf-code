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
	AddStyle(sfsGRID_USE);
	AddStyle(sfsGRID_SHOW);
	// distances between grid lines can be modified via following function:
	SetGridLineMult(10);
	// grid line style can be set as follows:
	SetGridStyle(wxSHORT_DASH);

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
	
	// process mouse wheel if needed
	AddStyle(sfsPROCESS_MOUSEWHEEL);
	// set scale boundaries aplied on mouse wheel scale change
	SetMinScale(0.1);
	SetMaxScale(2);

	// set accepted shapes
	GetDiagramManager()->ClearAcceptedShapes();
	GetDiagramManager()->AcceptShape(wxT("All"));

	// you can set also the canvas history manager working mode:
	// 1) Requires implementation of xsSerializable::Clone() virtual function
	// and copy constructor in all classes derived from the xsSerializable class
	// and shape manager.
	// GetHistoryManager().SetMode(wxSFCanvasHistory::histUSE_CLONING);
	// 2) No other programming overhead is required (except implementation
	// of standard serialization functionality). This working mode is default.
	GetHistoryManager().SetMode(wxSFCanvasHistory::histUSE_SERIALIZATION);

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
				{
					pShape = GetDiagramManager()->AddShape(CLASSINFO(wxSFEditTextShape), event.GetPosition(), sfDONT_SAVE_STATE);
					// editable text shapes can be forced to be multiline at default like this:
					// ((wxSFEditTextShape*)pShape)->ForceMultiline( true );
					
					// also edit control type can be set here like this:
					//((wxSFEditTextShape*)pShape)->SetEditType( wxSFEditTextShape::editDIALOG );
				}

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
				
				// child shapes can be locked accordingly to their parent's origin if the parent is resized
				//pShape->AddStyle( wxSFShapeBase::sfsLOCK_CHILDREN );
				
				// shapes can have fixed connection points defined in the following way:
				pShape->AddConnectionPoint( wxSFConnectionPoint::cpCENTERLEFT );
				pShape->AddConnectionPoint( wxSFConnectionPoint::cpCENTERRIGHT );
				// user can define also any number of CUSTOM connection points placed relatively to the 
				// parent shape's bounding box ("25, 50" here means 25% of width and 50% of height):
				pShape->AddConnectionPoint( wxRealPoint(25, 50) );
				pShape->AddConnectionPoint( wxRealPoint(75, 50) );
				// in this case the line connection can be assigned to the one of the defined
				// fixed connection points only.
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

			    // insert some shapes into the grid programmably (it can be also done interactively by drag'n'drop operations).
			    // shapes inserted to the grid can be aligned relatively to its grid cell region
			    wxSFShapeBase *pInnerShape = GetDiagramManager()->AddShape(CLASSINFO(wxSFEllipseShape), sfDONT_SAVE_STATE);
			    pInnerShape->SetVAlign( wxSFShapeBase::valignEXPAND );
			    pGrid->AppendToGrid( pInnerShape);
			    // add some another shapes...
			    pGrid->AppendToGrid( GetDiagramManager()->AddShape(CLASSINFO(wxSFDiamondShape), sfDONT_SAVE_STATE) );
                // shapes can be also inserted before given lexicographic position (at the first position in this case) in this way ...
			    pGrid->InsertToGrid(0, GetDiagramManager()->AddShape(CLASSINFO(wxSFRoundRectShape), sfDONT_SAVE_STATE));
			    // ... or can replace previously assigned shape at the position specified by row and column indexes
			    // (note that the previous shape at the given position (if exists) will be moved to the grid's last lexicographic position).
			    pGrid->InsertToGrid(1, 0, GetDiagramManager()->AddShape(CLASSINFO(wxSFCircleShape), sfDONT_SAVE_STATE));
				
				// also control shapes can be managed by the grid shape.
				//wxSFControlShape* pCtrl = (wxSFControlShape*)GetDiagramManager()->AddShape(CLASSINFO(wxSFControlShape), event.GetPosition(), sfDONT_SAVE_STATE);
				//if( pCtrl )
				//{
				//	pCtrl->SetVAlign( wxSFShapeBase::valignEXPAND );
				//	pCtrl->SetHAlign( wxSFShapeBase::halignEXPAND );
				//	pCtrl->SetControl( new wxButton( this, wxID_ANY, wxT("Test")) );
				//	pGrid->AppendToGrid( pCtrl );
				//}

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
				// interactive connection can be created also from existing object for example
				// if some connection properties should be modified before the connection creation
				// process is started:
                //StartInteractiveConnection(new wxSFLineShape(), event.GetPosition());
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
		
     case MainFrm::modeORTHOLINE:
        {
            if(GetMode() == modeREADY)
            {
                StartInteractiveConnection(CLASSINFO(wxSFOrthoLineShape), event.GetPosition());
            }
            else
                wxSFShapeCanvas::OnLeftDown(event);
        }
        break;
		
	case MainFrm::modeROUNDORTHOLINE:
        {
            if(GetMode() == modeREADY)
            {
                StartInteractiveConnection(CLASSINFO(wxSFRoundOrthoLineShape), event.GetPosition());
            }
            else
                wxSFShapeCanvas::OnLeftDown(event);
        }
        break;
		
	case MainFrm::modeSTANDALONELINE:
		{
			pShape = GetDiagramManager()->AddShape(CLASSINFO(wxSFLineShape), event.GetPosition(), sfDONT_SAVE_STATE);
			if( pShape )
			{
				// set the line to be stand-alone
				((wxSFLineShape*) pShape)->SetStandAlone( true );
			
				((wxSFLineShape*) pShape)->SetSrcPoint( wxSFCommonFcn::Conv2RealPoint(event.GetPosition() - wxPoint( 50, 0 )) );
				((wxSFLineShape*) pShape)->SetTrgPoint( wxSFCommonFcn::Conv2RealPoint(event.GetPosition() + wxPoint( 50, 0 )) );
				
				// line's ending style can be set as follows:
				//((wxSFLineShape*) pShape)->SetSrcArrow(CLASSINFO(wxSFCircleArrow));
				//((wxSFLineShape*) pShape)->SetTrgArrow(CLASSINFO(wxSFCircleArrow));

				pShape->AcceptChild(wxT("wxSFTextShape"));
				pShape->AcceptChild(wxT("wxSFEditTextShape"));
			}
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
        pShape->GetChildShapes(sfANY, lstShapes, sfRECURSIVE);
        if( !lstShapes.IsEmpty() )
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
        if( !lstShapes.IsEmpty() )
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
		// the line's ending style can be set like this:
        connection->SetTrgArrow(CLASSINFO(wxSFSolidArrow));
        // also wxSFOpenArrow, wxSFDiamondArrow and wxSFCircleArrow styles are available.
		connection->SetSrcArrow(CLASSINFO(wxSFCircleArrow));
		
        connection->AcceptChild(wxT("wxSFTextShape"));
        connection->AcceptChild(wxT("wxSFEditTextShape"));

		connection->AcceptConnection(wxT("All"));
        connection->AcceptSrcNeighbour(wxT("All"));
        connection->AcceptTrgNeighbour(wxT("All"));
		
		connection->SetDockPoint(sfdvLINESHAPE_DOCKPOINT_CENTER);

		m_pParentFrame->SetToolMode(MainFrm::modeDESIGN);
    }
}

void FrameCanvas::OnMouseWheel(wxMouseEvent& event)
{
	// do default actions
	wxSFShapeCanvas::OnMouseWheel(event);

	// adjust zoom slider control
	m_pParentFrame->GetZoomSlider()->SetValue(GetScale() * 50);
}
