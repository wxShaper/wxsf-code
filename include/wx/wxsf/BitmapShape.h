/***************************************************************
 * Name:      BitmapShape.h
 * Purpose:   Defines bitmap shape class
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#pragma once

#include "RectShape.h"

// default values
#define sfdvBITMAPSHAPE_SCALEIMAGE true

/// <summary> Class encapsulating the bitmap shape. The shape can display and control the
/// extents of BMP files loaded from a file. </summary>
class WXDLLIMPEXP_SF wxSFBitmapShape : public wxSFRectShape
{
public:

	XS_DECLARE_CLONABLE_CLASS(wxSFBitmapShape);

    /// <summary> Default constructor </summary>
	wxSFBitmapShape(void);
	/// <summary> User contructor </summary>
	/// <param name="pos"> Initial position </param>
	/// <param name="bitmapPath"> BMP file name </param>
	/// <param name="manager"> Pointer of parent manager </param>
	wxSFBitmapShape(const wxRealPoint& pos, const wxString& bitmapPath, wxSFDiagramManager* manager);
	/// <summary> Copy constructor </summary>
	/// <param name="obj"> Source shape </param>
	wxSFBitmapShape(const wxSFBitmapShape& obj);
	/// <summary> Destructor </summary>
	virtual ~wxSFBitmapShape(void);

	// public member data accessors
	/// <summary> Get full name of a source BMP file </summary>
	/// <returns> String containig full file name </returns>
	wxString GetBitmapPath(){return m_sBitmapPath;}
	/// <summary> Enable/disable scaling mode of the bitmap </summary>
	/// <param name="canscale"> Set TRUE if the bitmap shape could be scaled </param>
	void EnableScale(bool canscale){m_fCanScale = canscale;}
	/// <summary> Get information about the possibility of the shape scaling </summary>
	/// <returns> TRUE if the shape can be scaled, otherwise FALSE </returns>
	bool CanScale(){return m_fCanScale;}

	// public functions
    /// <summary> Load a bitmap from the file </summary>
    /// <param name="file"> Full file name </param>
    /// <returns> TRUE on success, otherwise FALSE </returns>
	bool CreateFromFile(const wxString& file);
    /// <summary> Load a bitmap from the XPM structure </summary>
    /// <param name="bits"> Buffer with the image bits </param>
    /// <returns> TRUE on success, otherwise FALSE </returns>
	bool CreateFromXPM(const char* const* bits);

	// public virtual function
	/// <summary> Scale the bitmap shape in both directions.
	/// The function can be overrided if neccessary. </summary>
	/// <param name="x"> Scale ratio in the horizontal direction </param>
	/// <param name="y"> Scale ratio in the vertical direction </param>
    /// <param name="children"> TRUE if the shape's children shoould be scaled as well, otherwise
    /// the shape will be updated after scaling via Update() function. </param>
	virtual void Scale(double x, double y, bool children = sfWITHCHILDREN);
	/// <summary> Event handler called when the user started to drag the shape handle.
	/// The function can be overrided if neccessary. </summary>
	/// <param name="handle"> Reference to the dragged shape handle </param>
	virtual void OnBeginHandle(wxSFShapeHandle& handle);
	/// <summary> Event handler called during dragging of the shape handle.
	/// The function can be overrided if neccessary. </summary>
	/// <param name="handle"> Reference to the dragged shape handle </param>
	virtual void OnHandle(wxSFShapeHandle& handle);
	/// <summary> Event handler called when the user finished dragging of the shape handle.
	/// The function can be overrided if neccessary. </summary>
	/// <param name="handle"> Reference to the dragged shape handle </param>
	virtual void OnEndHandle(wxSFShapeHandle& handle);

protected:

	// protected data members
	/// <summary> String containing the full bitmap file name </summary>
	wxString m_sBitmapPath;
	/// <summary> Currently processed (modified) bitmap </summary>
	wxBitmap m_Bitmap;
	/// <summary> Original archived bitmap </summary>
	wxBitmap m_OriginalBitmap;
	bool m_fCanScale;
	bool m_fRescaleInProgress;
	wxRealPoint m_nPrevPos;

	// protected functions
	/// <summary> Rescale the bitmap shape so it will fit the given extent. The
	/// shape position is not involved (the left-top bitmap corner is not moved). </summary>
	/// <param name="size"> New bitmap size </param>
	void RescaleImage(const wxRealPoint& size);

	// protected virtual functions
	/// <summary> Draw the shape in the normal way. The function can be overrided if neccessary. </summary>
	/// <param name="dc"> Reference to device context where the shape will be drawn to </param>
	virtual void DrawNormal(wxDC& dc);
	/// <summary> Draw the shape in the hower mode (the mouse cursor is above the shape). The function can be overrided if neccessary. </summary>
	/// <param name="dc"> Reference to device context where the shape will be drawn to </param>
	virtual void DrawHover(wxDC& dc);
	/// <summary> Draw the shape in the highlighted mode (another shape is dragged over this shape and this shape will accept the dragged one if it will be dropped on it). The function can be overrided if neccessary. </summary>
	/// <param name="dc"> Reference to device context where the shape will be drawn to </param>
	virtual void DrawHighlighted(wxDC& dc);

	/// <summary> Serialize shape's properties to the given XML node </summary>
	/// <param name="node"> Pointer to XML node where the shape's property nodes will be append to </param>
	/// <seealso cref="wxSFShapeBase::Serialize"></seealso>
	virtual wxXmlNode* Serialize(wxXmlNode* node);
	/// <summary> Deserialize shape's properties from the given XML node </summary>
	/// <param name="node"> Source XML node containig the shape's property nodes</param>
	/// <seealso cref="wxSFShapeBase::Deserialize"></seealso>
	virtual void Deserialize(wxXmlNode* node);

private:
	// private functions

	 /*! \brief Initialize serializable properties. */
	void MarkSerializableDataMembers();
};
