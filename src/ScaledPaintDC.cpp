/***************************************************************
 * Name:      ScaledPaintDC.cpp
 * Purpose:   Implements scaled DC class
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "wx_pch.h"

#ifdef _DEBUG_MSVC
#define new DEBUG_NEW
#endif

#include "wx/wxsf/ScaledPaintDC.h"

bool wxSFScaledPaintDC::m_fEnableGC = false;

wxSFScaledPaintDC::wxSFScaledPaintDC(wxBitmap &outbmp, double scale)
: wxMemoryDC(outbmp)
{
	m_nScale = scale;
#if wxUSE_GRAPHICS_CONTEXT
    m_pGC = wxGraphicsContext::Create( *this );
    if( m_pGC )
    {
        m_pGC->Scale( m_nScale, m_nScale );
    }
#endif
}

wxSFScaledPaintDC::~wxSFScaledPaintDC(void)
{
    SelectObject(wxNullBitmap);
#if wxUSE_GRAPHICS_CONTEXT
    if( m_pGC ) delete m_pGC;
#endif
}

#if wxUSE_GRAPHICS_CONTEXT
void wxSFScaledPaintDC::InitGC()
{
    m_pGC->SetPen( this->GetPen() );
    m_pGC->SetBrush( this->GetBrush() );
    m_pGC->SetFont( this->GetFont(), this->GetTextForeground() );
}

void wxSFScaledPaintDC::UninitGC()
{
    m_pGC->SetPen( wxNullPen );
    m_pGC->SetBrush( wxNullBrush );
    m_pGC->SetFont( wxNullFont, *wxBLACK );
}

void wxSFScaledPaintDC::PrepareGC()
{
    int x, y;
    GetDeviceOrigin(&x, &y);
    m_pGC->Translate( x, y );
}
#endif

//----------------------------------------------------------------------------------//
// protected virtual functions
//----------------------------------------------------------------------------------//

void wxSFScaledPaintDC::DoDrawPoint(wxCoord x, wxCoord y)
{
    if( m_fEnableGC )
    {
        #if wxUSE_GRAPHICS_CONTEXT
        InitGC();
        m_pGC->StrokeLine(x, y, x+1, y);
        //UninitGC();
        #endif
    }
    else
        wxWindowDC::DoDrawPoint(Scale(x), Scale(y));
}

void wxSFScaledPaintDC::DoDrawLine(wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2)
{
    if( m_fEnableGC )
    {
        #if wxUSE_GRAPHICS_CONTEXT
        InitGC();
        m_pGC->StrokeLine(x1, y1, x2, y2);
        //UninitGC();
        #endif
    }
    else
         wxWindowDC::DoDrawLine(Scale(x1), Scale(y1), Scale(x2), Scale(y2));
}

void wxSFScaledPaintDC::DoDrawArc(wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2, wxCoord xc, wxCoord yc)
{
    wxWindowDC::DoDrawArc(Scale(x1), Scale(y1), Scale(x2), Scale(y2), Scale(xc), Scale(yc));
}

void wxSFScaledPaintDC::DoDrawCheckMark(wxCoord x, wxCoord y, wxCoord width, wxCoord height)
{
    wxWindowDC::DoDrawCheckMark(Scale(x), Scale(y), Scale(width), Scale(height));
}

void wxSFScaledPaintDC::DoDrawEllipticArc(wxCoord x, wxCoord y, wxCoord w, wxCoord h, double sa, double ea)
{
    wxWindowDC::DoDrawEllipticArc(Scale(x), Scale(y), Scale(w), Scale(h), sa, ea);
}

void wxSFScaledPaintDC::DoDrawRectangle(wxCoord x, wxCoord y, wxCoord width, wxCoord height)
{
    if( m_fEnableGC )
    {
        #if wxUSE_GRAPHICS_CONTEXT
        InitGC();
        m_pGC->DrawRectangle(x, y, width, height);
        //UninitGC();
        #endif
    }
    else
         wxWindowDC::DoDrawRectangle(Scale(x), Scale(y), Scale(width), Scale(height));
}

void wxSFScaledPaintDC::DoDrawRoundedRectangle(wxCoord x, wxCoord y, wxCoord width, wxCoord height, double radius)
{
    if( m_fEnableGC )
    {
        #if wxUSE_GRAPHICS_CONTEXT
        InitGC();
        m_pGC->DrawRoundedRectangle(x, y, width, height, radius);
        //UninitGC();
        #endif
    }
    else
        wxWindowDC::DoDrawRoundedRectangle(Scale(x), Scale(y), Scale(width), Scale(height), radius*m_nScale);
}

void wxSFScaledPaintDC::DoDrawEllipse(wxCoord x, wxCoord y, wxCoord width, wxCoord height)
{
    if( m_fEnableGC )
    {
        #if wxUSE_GRAPHICS_CONTEXT
        InitGC();
        m_pGC->DrawEllipse(x, y, width, height );
        //UninitGC();
        #endif
    }
    else
        wxWindowDC::DoDrawEllipse(Scale(x), Scale(y), Scale(width), Scale(height));
}

void wxSFScaledPaintDC::DoCrossHair(wxCoord x, wxCoord y)
{
    wxWindowDC::DoCrossHair(Scale(x), Scale(y));
}

void wxSFScaledPaintDC::DoDrawIcon(const wxIcon& icon, wxCoord x, wxCoord y)
{
    wxWindowDC::DoDrawIcon(icon, Scale(x), Scale(y));
}

void wxSFScaledPaintDC::DoDrawBitmap(const wxBitmap &bmp, wxCoord x, wxCoord y, bool useMask)
{
    if( m_fEnableGC )
    {
        #if wxUSE_GRAPHICS_CONTEXT
        InitGC();
        m_pGC->DrawBitmap( bmp, x, y, bmp.GetWidth(), bmp.GetHeight() );
        //UninitGC();
        #endif
    }
    else
        wxWindowDC::DoDrawBitmap(bmp, Scale(x), Scale(y), useMask);
}

void wxSFScaledPaintDC::DoDrawText(const wxString& text, wxCoord x, wxCoord y)
{
    if( m_fEnableGC )
    {
        #if wxUSE_GRAPHICS_CONTEXT
        InitGC();
        m_pGC->DrawText( text, x, y );
        //UninitGC();
        #endif
    }
    else
    {
        wxFont font = GetFont();
        wxFont prevfont = font;

        if(font != wxNullFont)
        {
            font.SetPointSize(int(font.GetPointSize()*m_nScale));
            SetFont(font);
        }
        wxWindowDC::DoDrawText(text, Scale(x), Scale(y));

        SetFont(prevfont);
    }
}

void wxSFScaledPaintDC::DoDrawRotatedText(const wxString& text, wxCoord x, wxCoord y, double angle)
{
    if( m_fEnableGC )
    {
        #if wxUSE_GRAPHICS_CONTEXT
        InitGC();
        m_pGC->DrawText( text, x, y, angle );
        //UninitGC();
        #endif
    }
    else
    {
        wxFont font = GetFont();
        wxFont prevfont = font;

        if(font != wxNullFont)
        {
            font.SetPointSize(int(font.GetPointSize()*m_nScale));
            SetFont(font);
        }
        wxWindowDC::DoDrawRotatedText(text, Scale(x), Scale(y), angle);

        SetFont(prevfont);
    }
}

void wxSFScaledPaintDC::DoDrawLines(int n, wxPoint points[], wxCoord xoffset, wxCoord yoffset)
{
    if( m_fEnableGC )
    {
        #if wxUSE_GRAPHICS_CONTEXT
        InitGC();

        wxPoint2DDouble *pts = new wxPoint2DDouble[n];

        for(int i = 0; i < n; i++)
        {
            pts[0].m_x = points[0].x;
            pts[0].m_y = points[0].y;
        }

        m_pGC->StrokeLines(n, pts);

        delete pts;

        //UninitGC();
        #endif
    }
    else
    {
        wxPoint *updPoints = new wxPoint[n];

        for(int i = 0; i < n; i++)
        {
            updPoints[i].x = (int)((double)points[i].x*m_nScale);
            updPoints[i].y = (int)((double)points[i].y*m_nScale);
        }

        wxWindowDC::DoDrawLines(n, updPoints, Scale(xoffset), Scale(yoffset));

        delete [] updPoints;
    }
}

void wxSFScaledPaintDC::DoDrawPolygon(int n, wxPoint points[], wxCoord xoffset, wxCoord yoffset, int fillStyle)
{
    if( m_fEnableGC )
    {
        #if wxUSE_GRAPHICS_CONTEXT
        InitGC();

        wxGraphicsPath gcPath = m_pGC->CreatePath();
        gcPath.MoveToPoint( points[0].x, points[0].y );
        for(int i = 1; i < n; i++)
        {
            gcPath.AddLineToPoint( points[i].x, points[i].y );
        }
        gcPath.CloseSubpath();

        m_pGC->DrawPath( gcPath );

        //UninitGC();
        #endif
    }
    else
    {
        wxPoint *updPoints = new wxPoint[n];

        for(int i = 0; i < n; i++)
        {
            updPoints[i].x = (int)((double)points[i].x*m_nScale);
            updPoints[i].y = (int)((double)points[i].y*m_nScale);
        }

        wxWindowDC::DoDrawPolygon(n, updPoints, Scale(xoffset), Scale(yoffset), fillStyle);

        delete [] updPoints;
    }
}

void wxSFScaledPaintDC::DoDrawPolyPolygon(int n, int count[], wxPoint points[], wxCoord xoffset, wxCoord yoffset, int fillStyle)
{
    if( m_fEnableGC )
    {
        #if wxUSE_GRAPHICS_CONTEXT
        int nIndex = 0;

        InitGC();

        wxGraphicsPath gcPath = m_pGC->CreatePath();

        for(int i = 0; i < n; i++)
        {
            gcPath.MoveToPoint( points[nIndex].x, points[nIndex].y );
            for(int j = 0; j < count[i]; j++)
            {
                nIndex++;
                gcPath.AddLineToPoint( points[nIndex].x, points[nIndex].y );
            }
            gcPath.CloseSubpath();
        }

        m_pGC->Translate( xoffset, yoffset );
        m_pGC->DrawPath( gcPath );

        //UninitGC();
        #endif
    }
    else
    {
        int nTotalPoints = 0;

        for(int i = 0; i < n; i++)nTotalPoints += count[i];

        wxPoint *updPoints = new wxPoint[nTotalPoints];

        for(int i = 0; i < nTotalPoints; i++)
        {
            updPoints[i].x = (int)((double)points[i].x*m_nScale);
            updPoints[i].y = (int)((double)points[i].y*m_nScale);
        }

        wxWindowDC::DoDrawPolyPolygon(n, count, updPoints, Scale(xoffset), Scale(yoffset), fillStyle);

        delete [] updPoints;
    }
}
