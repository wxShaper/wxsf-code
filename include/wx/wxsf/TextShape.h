/***************************************************************
 * Name:      TextShape.h
 * Purpose:   Defines static text shape class
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#pragma once

#include "RectShape.h"

// default values
/// <summary> Default value of wxSFTextShape::m_Font data member </summary>
#define sfdvTEXTSHAPE_FONT *wxSWISS_FONT
/// <summary> Default value of wxSFTextShape::m_TextColor data member </summary>
#define sfdvTEXTSHAPE_TEXTCOLOR *wxBLACK

/*! \brief Class encapsulates basic non-editable text shape which is suitable for
 * displaying of various text information in the canvas.
 * \sa wxSFEditTextShape
 */
class WXDLLIMPEXP_SF wxSFTextShape : public wxSFRectShape
{
public:
    DECLARE_DYNAMIC_CLASS(wxSFTextShape);

    /*! \brief Default constructor. */
    wxSFTextShape(void);
    /*!
     * \brief User constructor.
     * \param pos Initial position
     * \param txt Text content
     * \param manager Pointer to parent diagram manager
     */
	wxSFTextShape(const wxRealPoint& pos, const wxString& txt, wxSFDiagramManager* manager);
	/*!
	 * \brief Copy constructor.
	 * \param obj Source objct
	 */
	wxSFTextShape(wxSFTextShape& obj);
	/*! \brief Destructor */
    virtual ~wxSFTextShape();

	/// <summary> Clone the object itself. </summary>
	/// <returns> Pointer to a new instace of the shape object</returns>
	wxSFTextShape* Clone(){return new wxSFTextShape(*this);}

    // public member data accessors
    /*!
     * \brief Set text font.
     * \param font Font
     */
    void SetFont(const wxFont& font);
    /*!
     * \brief Get text font.
     * \return Font
     */
    wxFont& GetFont(){return m_Font;}
    /*!
     * \brief Set text.
     * \param txt Text content
     */
    void SetText(const wxString& txt);
    /*!
     * \brief Get text.
     * \return Current text content
     */
    wxString GetText() const {return m_sText;}
    /*!
     * \brief Set text color.
     * \param col Text color
     */
    void SetTextColour(const wxColour& col){m_TextColor = col;}
    /*!
     * \brief Get text color.
     * \return Current text color
     */
    wxColour GetTextColour() const {return m_TextColor;}

    // public virtual functions
    /// <summary> Scale the shape size by in both directions. The function can be overrided if necessary. </summary>
    /// <param name="x"> Horizontal scale factor </param>
    /// <param name="y"> Vertical scale factor </param>
    /// <param name="children"> TRUE if the shape's children shoould be scaled as well, otherwise
    /// the shape will be updated after scaling via Update() function. </param>
    virtual void Scale(double x, double y, bool children = sfWITHCHILDREN);
	/*!
	 * \brief Event handler called during dragging of the shape handle.
	 * The function can be overrided if necessary.
	 *
	 * The function is called by the framework (by the shape canvas).
	 * Default implementation does nothing.
	 * \param handle Reference to dragged handle
	 */
    virtual void OnHandle(wxSFShapeHandle& handle);
	/*! \brief Upate shape (align all child shapes an resize it to fit them) */
	virtual void Update();

    // public functions
    wxSize GetTextExtent();
    void UpdateRectSize();

protected:

    // protected data members
    wxFont m_Font;
    wxColour m_TextColor;
    wxString m_sText;

    // protected virtual functions
	/*!
	 * \brief Draw the shape in the normal way. The function can be overrided if neccessary.
	 * \param dc Reference to device context where the shape will be drawn to
	 */
	virtual void DrawNormal(wxSFScaledPaintDC& dc);
	/*!
	 * \brief Draw the shape in the hower mode (the mouse cursor is above the shape).
	 * The function can be overrided if neccessary.
	 * \param dc Reference to device context where the shape will be drawn to
	 */
	virtual void DrawHover(wxSFScaledPaintDC& dc);
	/*!
	 * \brief Draw the shape in the highlighted mode (another shape is dragged over this
	 * shape and this shape will accept the dragged one if it will be dropped on it).
	 * The function can be overrided if neccessary.
	 * \param dc Reference to device context where the shape will be drawn to
	 */
	virtual void DrawHighlighted(wxSFScaledPaintDC& dc);

	/// <summary> Serialize shape's properties to the given XML node </summary>
	/// <param name="node"> Pointer to XML node where the shape's property nodes will be append to </param>
	/// <seealso cref="wxSFShapeBase::Serialize"></seealso>
	virtual wxXmlNode* Serialize(wxXmlNode* node);
	/// <summary> Deserialize shape's properties from the given XML node </summary>
	/// <param name="node"> Source XML node containig the shape's property nodes</param>
	/// <seealso cref="wxSFShapeBase::Deserialize"></seealso>
	virtual void Deserialize(wxXmlNode* node);

	/// <summary> Event handler called during dragging of the left shape handle.
	/// The function can be overrided if neccessary. </summary>
	/// <param name="handle"> Reference to dragged shape handle </param>
	virtual void OnLeftHandle(wxSFShapeHandle& handle);
	/// <summary> Event handler called during dragging of the top shape handle.
	/// The function can be overrided if neccessary. </summary>
	/// <param name="handle"> Reference to dragged shape handle </param>
	virtual void OnTopHandle(wxSFShapeHandle& handle);

	// protected functions
	/*!
	 * \brief Draw text shape.
	 * \param dc Device context where the text shape will be drawn to
	 */
	void DrawTextContent(wxSFScaledPaintDC& dc);

private:

	wxCoord m_nLineHeight;
};


