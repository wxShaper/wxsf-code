/***************************************************************
 * Name:      ScaledPaintDC.h
 * Purpose:   Defines scaled DC class
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifndef _WXSFSCALEDPAINTDC_H
#define _WXSFSCALEDPAINTDC_H

#include <wx/dcmemory.h>

#include "Defs.h"

/*! \brief Class extends buffered memory DC (wxMemoryDC) and provides modified
 * drawing functions cooperating with the shape canvas able to draw scaled graphics.
 * All drawing operations performed by the shapes should be done via this class otherwise
 * the global scalling capabilities provided by the shape canvas wont be available.
 * \sa wxSFShapeCanvas
 */
class WXDLLIMPEXP_SF wxSFScaledPaintDC : public wxMemoryDC
{
public:
    /// <summary> Constructor </summary>
    /// <param name="outbmp"> Reference to a bitmap where all graphics is drawn to. </param>
    /// <param name="scale"> Global graphics scale </param>
	wxSFScaledPaintDC(wxBitmap& outbmp, double scale);
	/// <summary> Destructor </summary>
	virtual ~wxSFScaledPaintDC(void);

	// public member data accessors
	/// <summary> Set the global graphics scale </summary>
	/// <param name="scale"> Scale </param>
	void SetScale(double scale){m_nScale = scale;}

protected:

	// protected data members
	/// <summary> Global graphics scale </summary>
	double m_nScale;

    // protected function

    wxCoord Scale(wxCoord val){return (wxCoord)((double)val*m_nScale);}

	// protected virtual functions

    virtual void DoDrawPoint(wxCoord x, wxCoord y);

    virtual void DoDrawLine(wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2);

    virtual void DoDrawArc(wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2, wxCoord xc, wxCoord yc);

    virtual void DoDrawCheckMark(wxCoord x, wxCoord y, wxCoord width, wxCoord height);

    virtual void DoDrawEllipticArc(wxCoord x, wxCoord y, wxCoord w, wxCoord h, double sa, double ea);

    virtual void DoDrawRectangle(wxCoord x, wxCoord y, wxCoord width, wxCoord height);

    virtual void DoDrawRoundedRectangle(wxCoord x, wxCoord y, wxCoord width, wxCoord height, double radius);

    virtual void DoDrawEllipse(wxCoord x, wxCoord y, wxCoord width, wxCoord height);

    virtual void DoCrossHair(wxCoord x, wxCoord y);

    virtual void DoDrawIcon(const wxIcon& icon, wxCoord x, wxCoord y);

    virtual void DoDrawBitmap(const wxBitmap &bmp, wxCoord x, wxCoord y, bool useMask = false);

    virtual void DoDrawText(const wxString& text, wxCoord x, wxCoord y);

    virtual void DoDrawRotatedText(const wxString& text, wxCoord x, wxCoord y, double angle);

    virtual void DoDrawLines(int n, wxPoint points[], wxCoord xoffset, wxCoord yoffset);

    virtual void DoDrawPolygon(int n, wxPoint points[], wxCoord xoffset, wxCoord yoffset, int fillStyle = wxODDEVEN_RULE);

    virtual void DoDrawPolyPolygon(int n, int count[], wxPoint points[], wxCoord xoffset, wxCoord yoffset, int fillStyle = wxODDEVEN_RULE);
};

#endif //_WXSFSCALEDPAINTDC_H
