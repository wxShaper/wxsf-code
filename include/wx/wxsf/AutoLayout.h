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

class WXDLLIMPEXP_SF wxSFLayoutAlgorithm : public wxObject
{
public:
	virtual ~wxSFLayoutAlgorithm() {;}
	
	virtual void DoLayout(ShapeList &shapes) = 0;
	
protected:
	wxRect GetBoundingBox(const ShapeList &shapes);
	wxSize GetShapesExtent(const ShapeList &shapes);
	wxRealPoint GetShapesCenter(const ShapeList &shapes);
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
	virtual ~wxSFLayoutCircle() {;}
	
	virtual void DoLayout(ShapeList &shapes);
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
