#pragma once

#include <wx/dcmemory.h>

/// <summary> Class extends buffered memory DC (wxMemoryDC) and provides modified
/// drawing functions cooperating with the shape canvas able to draw scaled graphics.
/// All drawing operations performed by the shapes should be done via this class otherwise
/// the global scalling capabilities provided by the shape canvas wont be available. </summary>
/// <seealso cref="wxSFShapeCanvas"></seealso>
class wxSFScaledPaintDC :	public wxMemoryDC
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

	// public functions

    /// <summary> Draws an arc of a circle, centred on (xc, yc), with starting point (x1, y1)
    /// and ending at (x2, y2). The current pen is used for the outline and the current brush
    /// for filling the shape. </summary>
	void DrawArc(double x1, double y1, double x2, double y2, double xc, double yc);
	/// <summary> Draws an arc of a circle, centred on (center), with starting point (pt1)
    /// and ending at (pt2). The current pen is used for the outline and the current brush
    /// for filling the shape. </summary>
	void DrawArc(const wxRealPoint& pt1, const wxRealPoint& pt2, const wxRealPoint& center);

    /// <summary> Draws a circle with the given centre and radius. </summary>
    /// <param name="x"> Horizontal coordinate of the circle center </param>
    /// <param name="y"> Vertical coordinate of the circle center </param>
    /// <param name="radius"> Circle radius </param>
    void DrawCircle(double x, double y, double radius);
    /// <summary> Draws a circle with the given centre and radius. </summary>
    /// <param name="pt"> Circle center </param>
    /// <param name="radius"> Circle radius </param>
    void DrawCircle(const wxRealPoint& pt, double radius);

    /// <summary> Draws an ellipse contained in the rectangle specified either
    /// with the given top left corner and the given size or directly. The current
    /// pen is used for the outline and the current brush for filling the shape. </summary>
    /// <param name="x"> Horizontal coordinate of the left-top rectangle corner </param>
    /// <param name="y"> Vertical coordinate of the left-top rectangle corner </param>
    /// <param name="width"> Rectangle width </param>
    /// <param name="height"> Rectangle height </param>
	void DrawEllipse(double x, double y, double width, double height);
    /// <summary> Draws an ellipse contained in the rectangle specified either
    /// with the given top left corner and the given size or directly. The current
    /// pen is used for the outline and the current brush for filling the shape. </summary>
    /// <param name="pt"> Coordinate of the left-top rectangle corner </param>
    /// <param name="size"> Rectangle size </param>
	void DrawEllipse(const wxRealPoint& pt, const wxRealPoint& size);
    /// <summary> Draws an ellipse contained in the rectangle specified either
    /// with the given top left corner and the given size or directly. The current
    /// pen is used for the outline and the current brush for filling the shape. </summary>
    /// <param name="rct"> Bounding rectangle </param>
	void DrawEllipse(const wxRect& rct);

    /// <summary> Draws a rectangle with the given top left corner, and with the given size.
    /// The current pen is used for the outline and the current brush for filling the shape. </summary>
    /// <param name="x"> Horizontal coordinate of the left-top rectangle corner </param>
    /// <param name="y"> Vertical coordinate of the left-top rectangle corner </param>
    /// <param name="width"> Rectangle width </param>
    /// <param name="height"> Rectangle height </param>
	void DrawRectangle(double x, double y, double width, double height);
    /// <summary> Draws a rectangle with the given top left corner, and with the given size.
    /// The current pen is used for the outline and the current brush for filling the shape. </summary>
    /// <param name="pos"> Coordinate of the left-top rectangle corner </param>
    /// <param name="size"> Rectangle size </param>
	void DrawRectangle(const wxRealPoint& pos, const wxRealPoint& size);
    /// <summary> Draws a rectangle with the given top left corner, and with the given size.
    /// The current pen is used for the outline and the current brush for filling the shape. </summary>
    /// <param name="rct"> Rectangle </param>
	void DrawRectangle(const wxRect& rct);

    /// <summary> Draws a rectangle with the given top left corner, and with the given size.
    /// The corners are quarter-circles using the given radius. The current pen is used for
    /// the outline and the current brush for filling the shape.</summary>
    /// <param name="x"> Horizontal coordinate of the left-top rectangle corner </param>
    /// <param name="y"> Vertical coordinate of the left-top rectangle corner </param>
    /// <param name="width"> Rectangle width </param>
    /// <param name="height"> Rectangle height </param>
    /// <param name="radius"> Corner radius </param>
	void DrawRoundedRectangle(double x, double y, double width, double height, double radius);
    /// <summary> Draws a rectangle with the given top left corner, and with the given size.
    /// The corners are quarter-circles using the given radius. The current pen is used for
    /// the outline and the current brush for filling the shape.</summary>
    /// <param name="pos"> Coordinate of the left-top rectangle corner </param>
    /// <param name="size"> Rectangle size </param>
    /// <param name="radius"> Corner radius </param>
	void DrawRoundedRectangle(const wxRealPoint& pos, const wxRealPoint& size, double radius);
    /// <summary> Draws a rectangle with the given top left corner, and with the given size.
    /// The corners are quarter-circles using the given radius. The current pen is used for
    /// the outline and the current brush for filling the shape.</summary>
    /// <param name="rct"> Rectangle </param>
    /// <param name="radius"> Corner radius </param>
	void DrawRoundedRectangle(const wxRect& rct, double radius);

    /// <summary> Draws a point using the color of the current pen. Note that the other properties
    /// of the pen are not used, such as width etc.. </summary>
    /// <param name="x"> Horizontal coordinate </param>
    /// <param name="y"> Vertical coordinate </param>
	void DrawPoint(double x, double y);
    /// <summary> Draws a point using the color of the current pen. Note that the other properties
    /// of the pen are not used, such as width etc.. </summary>
    /// <param name="pt"> Point position </param>
	void DrawPoint(const wxRealPoint& pt);

    /// <summary> Draws a line from the first point to the second. The current pen is used for
    /// drawing the line. Note that the point (x2, y2) is not part of the line and is not drawn
    /// by this function (this is consistent with the behaviour of many other toolkits). </summary>
    /// <param name="x1"> Horizontal position of the starting point </param>
    /// <param name="y1"> Vertical position of the starting point </param>
    /// <param name="x2"> Horizontal position of the ending point </param>
    /// <param name="y2"> Vertical position of the ending point </param>
	void DrawLine(double x1, double y1, double x2, double y2);
    /// <summary> Draws a line from the first point to the second. The current pen is used for
    /// drawing the line. Note that the point (x2, y2) is not part of the line and is not drawn
    /// by this function (this is consistent with the behaviour of many other toolkits). </summary>
    /// <param name="pt1"> Starting point </param>
    /// <param name="pt2"> Ending point </param>
	void DrawLine(const wxRealPoint& pt1, const wxRealPoint& pt2);

    /// <summary> Draws lines using list of pointers to points, adding the optional offset coordinate.
    /// The current pen is used for drawing the lines. The programmer is responsible for deleting the
    /// list of points. </summary>
    /// <param name="points"> List of the line points </param>
    /// <param name="xoffset"> Horizontal offset </param>
    /// <param name="yoffset"> Vertical offset </param>
	void DrawLines(wxList* points, wxCoord xoffset = 0, wxCoord yoffset = 0);
	/// <summary> Draws lines using an array of points of size n, adding the optional offset coordinate.
	/// The current pen is used for drawing the lines. The programmer is responsible for deleting the list
	/// of points. </summary>
	/// <param name="n"> Number of the line points </param>
	/// <param name="points"> Array of the line points </param>
    /// <param name="xoffset"> Horizontal offset </param>
    /// <param name="yoffset"> Vertical offset </param>
	void DrawLines(int n, wxRealPoint points[], wxCoord xoffset = 0, wxCoord yoffset = 0);

    /// <summary> Draws a filled polygon using an list of pointers to points,
    /// adding the optional offset coordinate. </summary>
    /// <param name="points"> List of the polygon vertices </param>
    /// <param name="xoffset"> Horizontal offset </param>
    /// <param name="yoffset"> Vertical offset </param>
    /// <param name="fill_style"> Fill style </param>
	void DrawPolygon(wxList *points, wxCoord xoffset = 0, wxCoord yoffset = 0, int fill_style = wxODDEVEN_RULE);
	/// <summary> Draws a filled polygon using an array of points of size n,
	/// adding the optional offset coordinate. </summary>
	/// <param name="n"> Number of the polygon vertices </param>
	/// <param name="points"> Array of the polygon vertices </param>
    /// <param name="xoffset"> Horizontal offset </param>
    /// <param name="yoffset"> Vertical offset </param>
    /// <param name="fill_style"> Fill style </param>
    void DrawPolygon(size_t n, wxRealPoint points[], wxCoord xoffset = 0, wxCoord yoffset = 0,int fill_style = wxODDEVEN_RULE);

    /// <summary> Draws a text string at the specified point, using the current text font, and the current text
    /// foreground and background colours. </summary>
    /// <param name="txt"> Text </param>
    /// <param name="x"> Horizontal position of left-top corner of a text bounding box </param>
    /// <param name="y"> Vertical position of left-top corner of a text bounding box </param>
    void DrawText(const wxString& txt, double x, double y);
    /// <summary> Draws a text string at the specified point, using the current text font, and the current text
    /// foreground and background colours. </summary>
    /// <param name="txt"> Text </param>
    /// <param name="pos"> Position of left-top corner of a text bounding box </param>
	void DrawText(const wxString& txt, const wxRealPoint& pos);

    /// <summary> Draw a bitmap on the device context at the specified point. If transparent is true and the bitmap
    /// has a transparency mask, the bitmap will be drawn transparently. </summary>
    /// <param name="bitmap"> Bitmap to draw </param>
    /// <param name="x"> Horizontal position of the bitmap </param>
    /// <param name="y"> Vertical position of the bitmap </param>
    /// <param name="useMask"> Use mask </param>
	void DrawBitmap(const wxBitmap& bitmap, double x, double y, bool useMask = false);
    /// <param name="bitmap"> Bitmap to draw </param>
    /// <param name="pt"> Bitmap position </param>
    /// <param name="useMask"> Use mask </param>
	void DrawBitmap(const wxBitmap& bitmap, const wxRealPoint& pt, bool useMask = false);

protected:

	// protected data members
	/// <summary> Global graphics scale </summary>
	double m_nScale;
};
