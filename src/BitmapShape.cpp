/***************************************************************
 * Name:      BitmapShape.cpp
 * Purpose:   Implements bitmap shape class
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/
#include "wx_pch.h"

#include "wx/wxsf/BitmapShape.h"
#include "wx/wxsf/ShapeCanvas.h"

#include "res/NoSource.xpm"

IMPLEMENT_DYNAMIC_CLASS(wxSFBitmapShape, wxSFRectShape);

wxSFBitmapShape::wxSFBitmapShape(void)
: wxSFRectShape()
{
    m_sBitmapPath = wxT("");
	m_fRescaleInProgress = false;
	m_fCanScale = sfdvBITMAPSHAPE_SCALEIMAGE;
	CreateFromXPM(NoSource_xpm);

    XS_SERIALIZE(m_sBitmapPath, wxT("path"));
    XS_SERIALIZE_EX(m_fCanScale, wxT("scale_image"), sfdvBITMAPSHAPE_SCALEIMAGE);
}

wxSFBitmapShape::wxSFBitmapShape(const wxRealPoint& pos, const wxString& bitmapPath, wxSFDiagramManager* manager)
: wxSFRectShape(pos, wxRealPoint(1, 1), manager)
{
    m_sBitmapPath = wxT("");
	m_fRescaleInProgress = false;
	m_fCanScale = sfdvBITMAPSHAPE_SCALEIMAGE;
	CreateFromFile(bitmapPath);

    XS_SERIALIZE(m_sBitmapPath, wxT("path"));
    XS_SERIALIZE_EX(m_fCanScale, wxT("scale_image"), sfdvBITMAPSHAPE_SCALEIMAGE);
}

wxSFBitmapShape::wxSFBitmapShape(wxSFBitmapShape& obj)
{
	m_fCanScale = obj.m_fCanScale;
	m_sBitmapPath = obj.m_sBitmapPath;
	m_Bitmap = obj.m_Bitmap;
}

wxSFBitmapShape::~wxSFBitmapShape(void)
{
}

//----------------------------------------------------------------------------------//
// public functions
//----------------------------------------------------------------------------------//

bool wxSFBitmapShape::CreateFromFile(const wxString& file)
{
	bool fSuccess = true;

	// load bitmap from the file
	//if((m_sBitmapPath != file) || (!m_Bitmap.IsOk()))
	{
		m_sBitmapPath = file;
		if(wxFileExists(m_sBitmapPath))
		{
			fSuccess = m_Bitmap.LoadFile(m_sBitmapPath, wxBITMAP_TYPE_BMP);

		}
		else
			fSuccess = false;
	}

	if(!fSuccess)
	{
		m_Bitmap = wxBitmap(NoSource_xpm);
	}

    m_OriginalBitmap = m_Bitmap;

	m_nRectSize.x = m_Bitmap.GetWidth();
	m_nRectSize.y = m_Bitmap.GetHeight();

	//EnableSizeChange(m_fCanScale);
	if(m_fCanScale)
	{
	    AddStyle(sfsSIZE_CHANGE);
        //SetStyle(GetStyle() | sfsSIZE_CHANGE);
	}
	else
        RemoveStyle(sfsSIZE_CHANGE);
        //SetStyle(GetStyle() & ~sfsSIZE_CHANGE);

	return fSuccess;
}

bool wxSFBitmapShape::CreateFromXPM(const char* const* bits)
{
	bool fSuccess = false;
	m_sBitmapPath = wxT("");

	// create bitmap from XPM
	m_Bitmap = wxBitmap(bits);
	fSuccess = m_Bitmap.IsOk();

	if(!fSuccess)
	{
		m_Bitmap = wxBitmap(NoSource_xpm);
	}

	m_OriginalBitmap = m_Bitmap;

	m_nRectSize.x = m_Bitmap.GetWidth();
	m_nRectSize.y = m_Bitmap.GetHeight();

	//EnableSizeChange(m_fCanScale);
	if(m_fCanScale)
	{
        //SetStyle(GetStyle() | sfsSIZE_CHANGE);
        AddStyle(sfsSIZE_CHANGE);
	}
	else
        RemoveStyle(sfsSIZE_CHANGE);
        //SetStyle(GetStyle() & ~sfsSIZE_CHANGE);

	return fSuccess;
}

//----------------------------------------------------------------------------------//
// public virtual functions
//----------------------------------------------------------------------------------//

void wxSFBitmapShape::Scale(double x, double y, bool children)
{
	if(m_fCanScale)
	{
		m_nRectSize.x *= x;
		m_nRectSize.y *= y;

		if(!m_fRescaleInProgress)RescaleImage(m_nRectSize);

        // call default function implementation (needed for scaling of shape's children)
		wxSFShapeBase::Scale(x, y, children);
	}
}

void wxSFBitmapShape::OnBeginHandle(wxSFShapeHandle& handle)
{
	if(m_fCanScale)
	{
		m_fRescaleInProgress = true;
		m_nPrevPos = GetAbsolutePosition();
	}
}

void wxSFBitmapShape::OnHandle(wxSFShapeHandle& handle)
{
	if(m_fCanScale)
	{
		wxSFRectShape::OnHandle(handle);
	}
	else
        RemoveStyle(sfsSIZE_CHANGE);
        //SetStyle(GetStyle() & ~sfsSIZE_CHANGE);
		//EnableSizeChange(false);
}

void wxSFBitmapShape::OnEndHandle(wxSFShapeHandle& handle)
{
	if(m_fCanScale)
	{
		m_fRescaleInProgress = false;
		RescaleImage(m_nRectSize);
	}
}

//----------------------------------------------------------------------------------//
// protected functions
//----------------------------------------------------------------------------------//

void wxSFBitmapShape::RescaleImage(const wxRealPoint& size)
{
    if( GetParentCanvas() )
    {
        wxImage image = m_OriginalBitmap.ConvertToImage();
        image.Rescale(int(size.x * GetParentCanvas()->GetScale()), int(size.y * GetParentCanvas()->GetScale()), wxIMAGE_QUALITY_NORMAL);
        m_Bitmap = wxBitmap(image);
    }
}

//----------------------------------------------------------------------------------//
// protected virtual functions
//----------------------------------------------------------------------------------//

void wxSFBitmapShape::DrawNormal(wxSFScaledPaintDC &dc)
{
	// HINT: overload it for custom actions...

	if(m_fRescaleInProgress)
	{
		dc.DrawBitmap(m_Bitmap, m_nPrevPos.x, m_nPrevPos.y);

		dc.SetBrush(*wxTRANSPARENT_BRUSH);
		dc.SetPen(wxPen(wxColour(100, 100, 100), 1, wxDOT));
		dc.DrawRectangle(GetAbsolutePosition(), m_nRectSize);
		dc.SetPen(wxNullPen);
		dc.SetBrush(wxNullBrush);
	}
	else
	{
		dc.DrawBitmap(m_Bitmap, GetAbsolutePosition());
	}
}

void wxSFBitmapShape::DrawHover(wxSFScaledPaintDC &dc)
{
	// HINT: overload it for custom actions...

	wxRealPoint pos = GetAbsolutePosition();
	dc.DrawBitmap(m_Bitmap, pos.x, pos.y);

	dc.SetPen(wxPen(m_nHoverColor, 1));
	dc.SetBrush(*wxTRANSPARENT_BRUSH);
	dc.DrawRectangle(GetAbsolutePosition(), m_nRectSize);
	dc.SetBrush(wxNullBrush);
	dc.SetPen(wxNullPen);
}

void wxSFBitmapShape::DrawHighlighted(wxSFScaledPaintDC &dc)
{
	// HINT: overload it for custom actions...

	wxRealPoint pos = GetAbsolutePosition();
	dc.DrawBitmap(m_Bitmap, pos.x, pos.y);

	dc.SetPen(wxPen(m_nHoverColor, 2));
	dc.SetBrush(*wxTRANSPARENT_BRUSH);
	dc.DrawRectangle(GetAbsolutePosition(), m_nRectSize);
	dc.SetBrush(wxNullBrush);
	dc.SetPen(wxNullPen);
}
