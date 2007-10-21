/***************************************************************
 * Name:      EditTextShape.h
 * Purpose:   Defines editable text shape class
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-07-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#pragma once

#include "TextShape.h"

class WXDLLIMPEXP_SF wxSFEditTextShape;

/// <summary> Auxiliary class providing neccessary functionality needed for in-place
/// modification of a content of the text shape. </summary>
/// <seealso cref="wxSFEditTextShape"></seealso>
class wxSFContentCtrl : public wxTextCtrl
{
public:
    /// <summary> Constructor </summary>
    /// <param name="parent"> Pointer to the parent window </param>
    /// <param name="id"> ID of the text control window </param>
    /// <param name="parentShape"> Pointer to the parent editable text shape </param>
    /// <param name="content"> Initial content of the text control </param>
    /// <param name="pos"> Initial position </param>
    /// <param name="size"> Initial size </param>
    /// <param name="style"> Window style </param>
	wxSFContentCtrl(wxWindow* parent, wxWindowID id, wxSFEditTextShape* parentShape, const wxString& content, wxPoint pos, wxSize size, int style);

    /// <summary> Abort the editing process </summary>
	void Quit();

protected:

	wxWindow* m_pParent;
	wxSFEditTextShape* m_pParentShape;
	wxString m_sPrevContent;

    /// <summary> Event handler called if the text control lost the focus </summary>
    /// <param name="event"> Reference to the event class instance </param>
	void OnKillFocus(wxFocusEvent& event);
	/// <summary> Event handler called if the key was pressed in the text control </summary>
	/// <param name="event"> Reference to the event class instance </param>
	void OnKeyDown(wxKeyEvent& event);

	DECLARE_EVENT_TABLE();
};

/// <summary> Class encapsulating the editable text shape. It extends the basic text shape. </summary>
/// <seealso cref="wxSFTextShape"></seealso>
class WXDLLIMPEXP_SF wxSFEditTextShape :	public wxSFTextShape
{
public:
	friend class wxSFContentCtrl;

	DECLARE_DYNAMIC_CLASS(wxSFEditTextShape);

    /// <summary> Default constructor </summary>
	wxSFEditTextShape(void);
	/// <summary> User constructor </summary>
	/// <param name="pos"> Initial position </param>
	/// <param name="txt"> Text content </param>
	/// <param name="manager"> Pointer to the parent canvas </param>
	wxSFEditTextShape(const wxRealPoint& pos, const wxString& txt, wxSFDiagramManager* manager);
	/// <summary> Copy constructor </summary>
	/// <param name="obj"> Reference to the source object </param>
	wxSFEditTextShape(wxSFEditTextShape& obj);
	/// <summary> Destructor </summary>
	virtual ~wxSFEditTextShape(void);

	/// <summary> Clone the object itself. </summary>
	/// <returns> Pointer to a new instace of the shape object</returns>
	wxSFEditTextShape* Clone(){return new wxSFEditTextShape(*this);}

	// public member data accessors
	/// <summary> Get pointer to assigned text control allowing user to change the
	/// shape's content directly in the canvas </summary>
	/// <returns> Pointer to instance of wxSFContentCtrl class </returns>
	wxSFContentCtrl* GetTextCtrl() {return m_pTextCtrl;}

	// public functions
	/// <summary> Switch the shape to a label editation mode. </summary>
	void EditLabel();

	// public virtual functions
	/// <summary> Event handler called when the shape was double-clicked.
	/// The function can be overrided if neccessary. </summary>
	/// <param name="pos"> Mouse position </param>
	virtual void OnLeftDoubleClick(const wxPoint& pos);
	/*!
	 * \brief Event handler called when any key is pressed (in the shape canvas).
	 * The function can be overrided if necessary.
	 *
	 * The function is called by the framework (by the shape canvas).
	 * \param key The key code
	 * \return The function must return TRUE if the default event routine should be called
	 * as well, otherwise FALSE
	 * \sa wxSFShapeBase::OnKey
	 */
	virtual bool OnKey(int key);

protected:
	wxSFContentCtrl* m_pTextCtrl;
	long m_nCurrentState;
};

