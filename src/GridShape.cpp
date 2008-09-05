/***************************************************************
 * Name:      GridShape.cpp
 * Purpose:   Implements rectangular shape class
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2008-08-02
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "wx_pch.h"

#ifdef _DEBUG_MSVC
#define new DEBUG_NEW
#endif

#include "wx/wxsf/GridShape.h"

XS_IMPLEMENT_CLONABLE_CLASS(wxSFGridShape, wxSFRectShape);

wxSFGridShape::wxSFGridShape() : wxSFRectShape()
{
    #ifdef __WXDEBUG__
    wxASSERT_MSG(NULL, wxT("wxSFGridShape is not fully implemented yet! DO NOT USE IT!!!"));
    #else
    wxMessageBox(wxT("wxSFGridShape is not fully implemented yet! DO NOT USE IT!!!"), wxT("wxShapeFramework"), wxICON_ERROR | wxOK);
    #endif

    m_nRows = sfdvGRIDSHAPE_ROWS;
    m_nCols = sfdvGRIDSHAPE_COLS;
    m_nCellSpace = sfdvGRIDSHAPE_CELLSPACE;

    MarkSerializableDataMembers();
}

wxSFGridShape::wxSFGridShape(const wxRealPoint& pos, const wxRealPoint& size, int rows, int cols, int cellspace, wxSFDiagramManager* manager)
: wxSFRectShape(pos, size, manager)
{
    #ifdef __WXDEBUG__
    wxASSERT_MSG(NULL, wxT("wxSFGridShape is not fully implemented yet! DO NOT USE IT!!!"));
    #else
    wxMessageBox(wxT("wxSFGridShape is not fully implemented yet! DO NOT USE IT!!!"), wxT("wxShapeFramework"), wxICON_ERROR | wxOK);
    #endif

    m_nRows = rows;
    m_nCols = cols;
    m_nCellSpace = cellspace;

    MarkSerializableDataMembers();
}

wxSFGridShape::wxSFGridShape(const wxSFGridShape& obj) : wxSFRectShape(obj)
{
    #ifdef __WXDEBUG__
    wxASSERT_MSG(NULL, wxT("wxSFGridShape is not fully implemented yet! DO NOT USE IT!!!"));
    #else
    wxMessageBox(wxT("wxSFGridShape is not fully implemented yet! DO NOT USE IT!!!"), wxT("wxShapeFramework"), wxICON_ERROR | wxOK);
    #endif

    m_nRows = obj.m_nRows;
    m_nCols = obj.m_nCols;
    m_nCellSpace = obj.m_nCellSpace;

    MarkSerializableDataMembers();
}

wxSFGridShape::~wxSFGridShape()
{

}

void wxSFGridShape::MarkSerializableDataMembers()
{
    XS_SERIALIZE_EX(m_nRows, wxT("rows"), sfdvGRIDSHAPE_ROWS);
	XS_SERIALIZE_EX(m_nCols, wxT("cols"), sfdvGRIDSHAPE_COLS);
	XS_SERIALIZE_EX(m_nCellSpace, wxT("cell_space"), sfdvGRIDSHAPE_CELLSPACE);
}

//----------------------------------------------------------------------------------//
// public functions
//----------------------------------------------------------------------------------//



//----------------------------------------------------------------------------------//
// public virtual functions
//----------------------------------------------------------------------------------//

void wxSFGridShape::DoChildrenLayout()
{
    wxSFShapeBase *pShape;
    int nIndex, nRow, nCol;

    wxRealPoint nAbsPos = GetAbsolutePosition();
    wxRect nParentRect = GetBoundingBox();

    wxRect currRect, maxRect = wxRect(0,0,0,0);

    // get maximum size of all managed (child) shapes
    SerializableList::compatibility_iterator node = GetFirstChildNode();
    while(node)
    {
        currRect = ((wxSFShapeBase*)node->GetData())->GetBoundingBox();

        if( currRect.GetWidth() > maxRect.GetWidth()) maxRect.SetWidth(currRect.GetWidth());
        if( currRect.GetHeight() > maxRect.GetHeight()) maxRect.SetHeight(currRect.GetHeight());

        node = node->GetNext();
    }

    if( !maxRect.IsEmpty() && m_nCols && m_nRows )
    {
        // put managed shapes to appropriate positions
        node = GetFirstChildNode();

        nIndex = nCol = 0;
        nRow = -1;

        while(node)
        {
            pShape = (wxSFShapeBase*)node->GetData();

            if( nIndex++ % m_nCols == 0 )
            {
                nCol = 0; nRow++;
            }
            else
                nCol++;

            pShape->MoveTo( nAbsPos.x + nCol*maxRect.GetWidth() + m_nCellSpace,
                            nAbsPos.y + nRow*maxRect.GetHeight() + m_nCellSpace);

            node = node->GetNext();
        }
    }
}

void wxSFGridShape::Update()
{
    // do self-alignment
    DoAlignment();

    // do alignment of shape's children
    DoChildrenLayout();

    // fit the shape to its children
    this->FitToChildren();

    // do it recursively on all parent shapes
    if( GetParentShape() )GetParentShape()->Update();
}

void  wxSFGridShape::OnChildDropped(const wxRealPoint& pos, wxSFShapeBase *child)
{

}

