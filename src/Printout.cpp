/***************************************************************
 * Name:      Printout.cpp
 * Purpose:   Implements printout class for shape canvas
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2008-05-06
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "wx/wxsf/Printout.h"
#include "wx/wxsf/ShapeCanvas.h"

wxSFPrintout::wxSFPrintout(const wxString& title, wxSFShapeCanvas *canvas)
: wxPrintout(title)
{
    m_pCanvas = canvas;
}

wxSFPrintout::~wxSFPrintout()
{
}

//----------------------------------------------------------------------------------//
// public virtual functions
//----------------------------------------------------------------------------------//

bool wxSFPrintout::HasPage(int page)
{
    return (page == 1);
}

bool wxSFPrintout::OnBeginDocument(int startPage, int endPage)
{
    if (!wxPrintout::OnBeginDocument(startPage, endPage)) return false;

    return true;
}

bool wxSFPrintout::OnPrintPage(int page)
{
    wxASSERT(m_pCanvas);

    wxDC *dc = GetDC();
    if (dc && m_pCanvas)
    {
        // get grawing size
        wxRect totalBB = m_pCanvas->GetTotalBoundingBox();
        wxCoord maxX = totalBB.GetRight();
        wxCoord maxY = totalBB.GetBottom();

        // This sets the user scale and origin of the DC so that the image fits
        // within the page margins as specified by g_PageSetupData, which you can
        // change (on some platforms, at least) in the Page Setup dialog. Note that
        // on Mac, the native Page Setup dialog doesn't let you change the margins
        // of a wxPageSetupDialogData object, so you'll have to write your own dialog or
        // use the Mac-only wxMacPageMarginsDialog, as we do in this program.
        FitThisSizeToPageMargins(wxSize(maxX, maxY), *g_pageSetupData);
        wxRect fitRect = GetLogicalPageMarginsRect(*g_pageSetupData);

        // This offsets the image so that it is centered within the reference
        // rectangle defined above.
        wxCoord xoff = (fitRect.width - maxX - totalBB.GetLeft()) / 2;
        wxCoord yoff = (fitRect.height - maxY - totalBB.GetTop()) / 2;
        OffsetLogicalOrigin(xoff, yoff);

        double prevScale = m_pCanvas->GetScale();
        long prevStyle = m_pCanvas->GetStyle();
        wxColour prevColour = m_pCanvas->GetCanvasColour();

        if( !m_pCanvas->ContainsStyle( wxSFShapeCanvas::sfsPRINT_BACKGROUND ) )
        {
            m_pCanvas->RemoveStyle( wxSFShapeCanvas::sfsGRADIENT_BACKGROUND );
            m_pCanvas->RemoveStyle( wxSFShapeCanvas::sfsGRID_SHOW );
            m_pCanvas->SetCanvasColour( *wxWHITE );
        }

        // draw the canvas content without any scale (dc is scaled by the printing framework)
        m_pCanvas->SetScale(1);
        m_pCanvas->DrawContent(*dc, sfNOT_FROM_PAINT);
        m_pCanvas->SetScale(prevScale);

        if( !m_pCanvas->ContainsStyle( wxSFShapeCanvas::sfsPRINT_BACKGROUND ) )
        {
            m_pCanvas->SetStyle( prevStyle );
            m_pCanvas->SetCanvasColour( prevColour );
        }

        return true;
    }
    else
        return false;
}

void wxSFPrintout::GetPageInfo(int *minPage, int *maxPage, int *selPageFrom, int *selPageTo)
{
    *minPage = 1;
    *maxPage = 1;
    *selPageFrom = 1;
    *selPageTo = 1;
}
