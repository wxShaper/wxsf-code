/***************************************************************
 * Name:      FlexGridShape.cpp
 * Purpose:   Implements flexible grid shape class
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2008-09-27
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "wx_pch.h"

#ifdef _DEBUG_MSVC
#define new DEBUG_NEW
#endif

#include "wx/wxsf/FlexGridShape.h"

XS_IMPLEMENT_CLONABLE_CLASS(wxSFFlexGridShape, wxSFGridShape);

wxSFFlexGridShape::wxSFFlexGridShape() : wxSFGridShape()
{
}

wxSFFlexGridShape::wxSFFlexGridShape(const wxRealPoint& pos, const wxRealPoint& size, int rows, int cols, int cellspace, wxSFDiagramManager* manager)
: wxSFGridShape(pos, size, rows, cols, cellspace, manager)
{
}

wxSFFlexGridShape::wxSFFlexGridShape(const wxSFFlexGridShape& obj) : wxSFGridShape(obj)
{
}

wxSFFlexGridShape::~wxSFFlexGridShape()
{

}

//----------------------------------------------------------------------------------//
// public virtual functions
//----------------------------------------------------------------------------------//

void wxSFFlexGridShape::DoChildrenLayout()
{
    if( !m_nCols || !m_nRows ) return;

    wxSFShapeBase *pShape;
    int nIndex, nRow, nCol, nTotalX, nTotalY;
    size_t i;

    IntArray arrRowSizes;
    IntArray arrColSizes;

    wxRealPoint nAbsPos = GetAbsolutePosition();
    wxRect nCurrRect;

    arrRowSizes.SetCount( m_nRows );
    arrColSizes.SetCount( m_nCols );
    for( i = 0; i < (size_t)m_nRows; i++ ) arrRowSizes[i] = 0;
    for( i = 0; i < (size_t)m_nCols; i++ ) arrColSizes[i] = 0;

    nIndex = nCol = nTotalX = nTotalY = 0;
    nRow = -1;

    // get maximum size of all managed (child) shapes per row and collumn
    for( i = 0; i < m_arrCells.GetCount(); i++ )
    {
        pShape = (wxSFShapeBase*)GetChild(m_arrCells[i]);
        if( pShape )
        {
            if( nIndex++ % m_nCols == 0 )
            {
                nCol = 0; nRow++;
            }
            else
                nCol++;

            nCurrRect = pShape->GetBoundingBox();

            if( nCurrRect.GetWidth() > arrColSizes[nCol]) arrColSizes[nCol] = nCurrRect.GetWidth();
            if( nCurrRect.GetHeight() > arrRowSizes[nRow]) arrRowSizes[nRow] = nCurrRect.GetHeight();
        }
    }

    // put managed shapes to appropriate positions
    nIndex = nCol = 0;
    nRow = -1;

    for( i = 0; i < m_arrCells.GetCount(); i++ )
    {
        pShape = (wxSFShapeBase*)GetChild(m_arrCells[i]);
        if( pShape )
        {
            if( nIndex++ % m_nCols == 0 )
            {
                nCol = 0; nTotalX = 0; nRow++;
                if( nRow > 0 ) nTotalY += arrRowSizes[ nRow-1 ];
            }
            else
            {
                nCol++;
                if( nCol > 0 ) nTotalX += arrColSizes[ nCol-1 ];
            }

            pShape->MoveTo( nAbsPos.x + nTotalX + (nCol+1)*m_nCellSpace,
                            nAbsPos.y + nTotalY + (nRow+1)*m_nCellSpace);
        }
    }
}
