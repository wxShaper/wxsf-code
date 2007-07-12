#pragma once

#include "rectshape.h"

/// <summary> Auxiliary class encapsulating multiselection rectangle used
/// in the shape canvas. The class shouldn't be used directly. </summary>
/// <seealso cref="wxSFShapeCanvas"></seealso>
class wxSFMultiSelRect : public wxSFRectShape
{
public:
    /// <summary> Default constructor </summary>
	wxSFMultiSelRect(void);
	/// <summary> Destructor </summary>
	virtual ~wxSFMultiSelRect(void);

	// public virtual functions
	/// <summary> Event handler called at the begining of the shape handle dragging process.
	/// The function can be overrided if neccessary. </summary>
	/// <param name="handle"> Reference to dragged shape handle </param>
	virtual void OnBeginHandle(wxSFShapeHandle& handle);
	/// <summary> Event handler called at the end of the shape handle dragging process.
	/// The function can be overrided if neccessary. </summary>
	/// <param name="handle"> Reference to dragged shape handle </param>
	virtual void OnEndHandle(wxSFShapeHandle& handle);

protected:
	// protected virtual functions
	/// <summary> Event handler called during dragging of the right shape handle.
	/// The function can be overrided if neccessary. </summary>
	/// <param name="handle"> Reference to dragged shape handle </param>
	virtual void OnRightHandle(wxSFShapeHandle& handle);
	/// <summary> Event handler called during dragging of the left shape handle.
	/// The function can be overrided if neccessary. </summary>
	/// <param name="handle"> Reference to dragged shape handle </param>
	virtual void OnLeftHandle(wxSFShapeHandle& handle);
	/// <summary> Event handler called during dragging of the top shape handle.
	/// The function can be overrided if neccessary. </summary>
	/// <param name="handle"> Reference to dragged shape handle </param>
	virtual void OnTopHandle(wxSFShapeHandle& handle);
	/// <summary> Event handler called during dragging of the bottom shape handle.
	/// The function can be overrided if neccessary. </summary>
	/// <param name="handle"> Reference to dragged shape handle </param>
	virtual void OnBottomHandle(wxSFShapeHandle& handle);

private:

	// private functions
	/// <summary> Auxiliary function </summary>
	bool CanScale(wxSFShapeBase* shape);
	/// <summary> Auxiliary function </summary>
	bool AnyWidthExceeded(const wxPoint& delta);
	/// <summary> Auxiliary function </summary>
	bool AnyHeightExceeded(const wxPoint& delta);
	/// <summary> Auxiliary function </summary>
	void ScaleChildren(wxSFShapeBase* parent, double sx, double sy);
};
