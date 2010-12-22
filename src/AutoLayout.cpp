/***************************************************************
 * Name:      AutoLayout.cpp
 * Purpose:   Implements class for autolayouting algoriths' provider
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2010-12-22
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "wx_pch.h"

#ifdef _DEBUG_MSVC
#define new DEBUG_NEW
#endif

#include <wx/wxsf/AutoLayout.h>
#include <wx/wxsf/LineShape.h>
#include <wx/wxsf/ShapeCanvas.h>

#include <math.h>

#define PI 3.14159265

// constructor and destructor //////////////////////////////////////////////////

wxSFAutoLayout::wxSFAutoLayout()
{
	InitializeAllAlgorithms();
}

wxSFAutoLayout::~wxSFAutoLayout()
{
}

// static stuff ////////////////////////////////////////////////////////////////

LayoutAlgoritmMap wxSFAutoLayout::m_mapAlgorithms;

bool wxSFAutoLayout::RegisterLayoutAlgorithm(const wxString& algname, wxSFLayoutAlgorithm* alg)
{
	if( alg && m_mapAlgorithms.find( algname ) == m_mapAlgorithms.end() )
	{
		m_mapAlgorithms[ algname ] = alg;
		return true;
	}
	else
		return false;
}

void wxSFAutoLayout::CleanUp()
{
	LayoutAlgoritmMap::iterator it = m_mapAlgorithms.begin();
    while( it != m_mapAlgorithms.end() )
    {
		if( it->second ) delete it->second;
        it++;
    }
    m_mapAlgorithms.clear();
}

// member functions ////////////////////////////////////////////////////////////

wxArrayString wxSFAutoLayout::GetRegisteredAlgorithms()
{
	wxArrayString arrAlgorithms;
	
	for( LayoutAlgoritmMap::iterator it = m_mapAlgorithms.begin(); it != m_mapAlgorithms.end(); ++ it )
	{
		arrAlgorithms.Add( it->first );
	}
	
	return arrAlgorithms;
}

void wxSFAutoLayout::InitializeAllAlgorithms()
{
	// register predefined algorithms
	RegisterLayoutAlgorithm( wxT("Circle"), new wxSFLayoutCircle() );
	//RegisterLayoutAlgorithm( wxT("Tree"), new wxSFLayoutTree() );
	RegisterLayoutAlgorithm( wxT("Mesh"), new wxSFLayoutMesh() );
}

void wxSFAutoLayout::Layout(ShapeList& shapes, const wxString& algname)
{
	wxSFLayoutAlgorithm *pAlg = m_mapAlgorithms[ algname ];
	if( pAlg )
	{
		pAlg->DoLayout( shapes );
	}
}

void wxSFAutoLayout::Layout(wxSFDiagramManager& manager, const wxString& algname)
{
	wxSFLayoutAlgorithm *pAlg = m_mapAlgorithms[ algname ];
	if( pAlg )
	{
		ShapeList lstShapes;
		manager.GetShapes( CLASSINFO(wxSFShapeBase), lstShapes );
		
		// remove all child shapes
		ShapeList::compatibility_iterator it = lstShapes.GetFirst();
		while( it )
		{
			wxSFShapeBase *pShape = it->GetData();
			if( pShape->GetParentShape() || pShape->IsKindOf(CLASSINFO(wxSFLineShape)) )
			{
				lstShapes.DeleteNode( it );
				it = lstShapes.GetFirst();
			}
			else
				it = it->GetNext();
		}
		
		pAlg->DoLayout( lstShapes );
		
		manager.MoveShapesFromNegatives();
	}
}

void wxSFAutoLayout::Layout(wxSFShapeCanvas* canvas, const wxString& algname)
{
	if( canvas )
	{
		Layout( *canvas->GetDiagramManager(), algname );
		canvas->UpdateVirtualSize();
		canvas->Refresh(false);
	}
}

// pre-defined layout algorithms ///////////////////////////////////////////////

void wxSFLayoutCircle::DoLayout(ShapeList& shapes)
{
	// first calculate diagram center and total width and height
	wxRealPoint nCenter;
	int nTotalWidth = 0;
	int nTotalHeight = 0;
	int nCount = shapes.GetCount();
	
	for( ShapeList::iterator it = shapes.begin(); it != shapes.end(); ++ it )
	{
		wxSFShapeBase *pShape = *it;
		
		wxRect rctBB = pShape->GetBoundingBox();
		
		nCenter = nCenter + pShape->GetAbsolutePosition();
		nTotalWidth += rctBB.GetWidth();
		nTotalHeight += rctBB.GetHeight();
	}
	
	nCenter.x /= nCount;
	nCenter.y /= nCount;
	
	double x, y;
	double step = 360.0 / nCount;
	double degree = 0;
	double rx = nTotalWidth / 2;
	double ry = nTotalHeight / 2;
	
	// move shapes
	for( ShapeList::iterator it = shapes.begin(); it != shapes.end(); ++ it )
	{
		wxSFShapeBase *pShape = *it;
		
		x = nCenter.x + cos( degree * PI / 180 ) * rx;
		y = nCenter.y + sin( degree * PI / 180 ) * ry;
		degree += step;
		
		pShape->MoveTo( x, y );
	}
}

void wxSFLayoutTree::DoLayout(ShapeList& shapes)
{
	// TODO: implement it...
}

void wxSFLayoutMesh::DoLayout(ShapeList& shapes)
{
	int i, cols = floor( sqrt( shapes.GetCount() ) );
	
	double roffset, coffset, startx, starty, maxh;
	startx = starty = INT_MAX;
	roffset = coffset = 0;
	
	// find starting position
	for( ShapeList::iterator it = shapes.begin(); it != shapes.end(); ++ it )
	{
		wxSFShapeBase *pShape = *it;
		
		wxRealPoint nPos = pShape->GetAbsolutePosition();
		if( nPos.x < startx ) startx = nPos.x;
		if( nPos.y < starty ) starty = nPos.y;
	}
	
	// move shapes
	i = 0;
	maxh = 0;
	for( ShapeList::iterator it = shapes.begin(); it != shapes.end(); ++ it )
	{
		wxSFShapeBase *pShape = *it;
		
		if( i++ % cols == 0 )
		{
			coffset = 0;
			roffset += maxh + 10;
			maxh = 0;
		}
		
		pShape->MoveTo( startx + coffset, starty + roffset );
		
		wxRect rctBB = pShape->GetBoundingBox();
		coffset += rctBB.GetWidth() + 10;
		
		if( rctBB.GetHeight() > maxh ) maxh = rctBB.GetHeight();
	}
}
