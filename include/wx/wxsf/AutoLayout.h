/***************************************************************
 * Name:      AutoLayout.h
 * Purpose:   Declares class for autolayouting algoriths' provider
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2010-12-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifndef _WXSFAUTOLAYOUT_H_
#define _WXSFAUTOLAYOUT_H_

#include <wx/wxsf/DiagramManager.h>

#include <wx/hashmap.h>

/*!
 * \class wxSFLayoutAlgorithm
 * \author Michal Bližňák
 * \date 23.12.2010
 * \file AutoLayout.h
 * \brief Base class for all layouting algorithms. The class containts one abstract function modifying
 * shapes' layout and several helper functions.
 */
class WXDLLIMPEXP_SF wxSFLayoutAlgorithm : public wxObject
{
public:
	/*!
	 * \brief Destructor.
	 */
	virtual ~wxSFLayoutAlgorithm() {;}
	/*!
	 * \brief Functions perfomrning the layout change. All derived classes must implement it.
	 * \param shapes List of shapes which should be layouted
	 */
	virtual void DoLayout(ShapeList &shapes) = 0;
	
protected:
	/*!
	 * \brief Calculate bounding box surroundig given shapes.
	 * \param shapes List of shapes
	 * \return Bounding box
	 */
	wxRect GetBoundingBox(const ShapeList &shapes);
	/*!
	 * \brief Get overall extent of all given shapes calculated as a sum of their width and height.
	 * \param shapes List of shapes
	 * \return Overall shapes extent
	 */
	wxSize GetShapesExtent(const ShapeList &shapes);
	/*!
	 * \brief Get center point of given shapes.
	 * \param shapes List of shapes
	 * \return Center point
	 */
	wxRealPoint GetShapesCenter(const ShapeList &shapes);
	/*!
	 * \brief Get top-left point of bounding box surrounding given shapes.
	 * \param shapes List of shapes
	 * \return Top-left point of bounding box surrounding given shapes
	 */
	wxRealPoint GetTopLeft(const ShapeList &shapes);
};

WX_DECLARE_HASH_MAP( wxString, wxSFLayoutAlgorithm*, wxStringHash, wxStringEqual, LayoutAlgoritmMap );
	
class WXDLLIMPEXP_SF wxSFAutoLayout
{
public:
	wxSFAutoLayout();
	~wxSFAutoLayout();

	void Layout(ShapeList &shapes, const wxString& algname);
	void Layout(wxSFDiagramManager &manager, const wxString& algname);
	void Layout(wxSFShapeCanvas *canvas, const wxString& algname);
	
	static bool RegisterLayoutAlgorithm(const wxString& algname, wxSFLayoutAlgorithm *alg);
	static void CleanUp();
	
	wxArrayString GetRegisteredAlgorithms();
	wxSFLayoutAlgorithm *GetAlgorithm(const wxString &algname) { return m_mapAlgorithms[algname]; }
	
protected:
	void InitializeAllAlgorithms();
	
	static LayoutAlgoritmMap m_mapAlgorithms;
	
};

// pre-defined algorithms //////////////////////////////////////////////////////

class wxSFLayoutCircle : public wxSFLayoutAlgorithm
{
public:
	wxSFLayoutCircle() : m_DistanceRatio(1) {;}
	virtual ~wxSFLayoutCircle() {;}
	
	virtual void DoLayout(ShapeList &shapes);
	
	void SetDistanceRatio(double DistanceRatio) {this->m_DistanceRatio = DistanceRatio;}
	double GetDistanceRatio() const {return m_DistanceRatio;}
	
protected:
	double m_DistanceRatio;
};

class wxSFLayoutVerticalTree : public wxSFLayoutAlgorithm
{
public:
	wxSFLayoutVerticalTree() : m_HSpace(30), m_VSpace(30) {;}
	virtual ~wxSFLayoutVerticalTree() {;}
	
	virtual void DoLayout(ShapeList &shapes);
	
	void SetHSpace(double HSpace) {this->m_HSpace = HSpace;}
	void SetVSpace(double VSpace) {this->m_VSpace = VSpace;}
	double GetHSpace() const {return m_HSpace;}
	double GetVSpace() const {return m_VSpace;}
	
protected:
	double m_nMinX;
	int m_nCurrMaxWidth;
	
	double m_HSpace;
	double m_VSpace;

	void ProcessNode(wxSFShapeBase *node, double y);
};

class wxSFLayoutHorizontalTree : public wxSFLayoutAlgorithm
{
public:
	wxSFLayoutHorizontalTree() : m_HSpace(30), m_VSpace(30) {;}
	virtual ~wxSFLayoutHorizontalTree() {;}
	
	virtual void DoLayout(ShapeList &shapes);
	
	void SetHSpace(double HSpace) {this->m_HSpace = HSpace;}
	void SetVSpace(double VSpace) {this->m_VSpace = VSpace;}
	double GetHSpace() const {return m_HSpace;}
	double GetVSpace() const {return m_VSpace;}
	
protected:
	double m_nMinY;
	int m_nCurrMaxHeight;
	
	double m_HSpace;
	double m_VSpace;

	void ProcessNode(wxSFShapeBase *node, double x);
};

class wxSFLayoutMesh : public wxSFLayoutAlgorithm
{
public:
	wxSFLayoutMesh() : m_HSpace(30), m_VSpace(30) {;}
	virtual ~wxSFLayoutMesh() {;}
	
	virtual void DoLayout(ShapeList &shapes);
	
	void SetHSpace(double HSpace) {this->m_HSpace = HSpace;}
	void SetVSpace(double VSpace) {this->m_VSpace = VSpace;}
	double GetHSpace() const {return m_HSpace;}
	double GetVSpace() const {return m_VSpace;}
	
protected:
	double m_HSpace;
	double m_VSpace;
};

#endif //_WXSFAUTOLAYOUT_H_
