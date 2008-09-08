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
    m_nRows = sfdvGRIDSHAPE_ROWS;
    m_nCols = sfdvGRIDSHAPE_COLS;
    m_nCellSpace = sfdvGRIDSHAPE_CELLSPACE;

    RemoveStyle(sfsSIZE_CHANGE);

    MarkSerializableDataMembers();
}

wxSFGridShape::wxSFGridShape(const wxRealPoint& pos, const wxRealPoint& size, int rows, int cols, int cellspace, wxSFDiagramManager* manager)
: wxSFRectShape(pos, size, manager)
{
    m_nRows = rows;
    m_nCols = cols;
    m_nCellSpace = cellspace;

    RemoveStyle(sfsSIZE_CHANGE);

    m_arrCells.Alloc( rows * cols );

    MarkSerializableDataMembers();
}

wxSFGridShape::wxSFGridShape(const wxSFGridShape& obj) : wxSFRectShape(obj)
{
    m_nRows = obj.m_nRows;
    m_nCols = obj.m_nCols;
    m_nCellSpace = obj.m_nCellSpace;

    RemoveStyle(sfsSIZE_CHANGE);

    m_arrCells.Clear();
    WX_APPEND_ARRAY(m_arrCells, obj.m_arrCells);

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
	XS_SERIALIZE(m_arrCells, wxT("cells"));
}

//----------------------------------------------------------------------------------//
// public functions
//----------------------------------------------------------------------------------//

void wxSFGridShape::SetDimensions(int rows, int cols)
{
    wxASSERT(rows);
    wxASSERT(cols);

    if( !(rows * cols) ) return;

    m_nRows = rows;
    m_nCols = cols;

    m_arrCells.Alloc( rows * cols );
}

void wxSFGridShape::GetDimensions(int *rows, int *cols)
{
    *rows = m_nRows;
    *cols = m_nCols;
}

void wxSFGridShape::ClearGrid()
{
    m_nRows = 0;
    m_nCols = 0;

    m_arrCells.Clear();
}

bool wxSFGridShape::AppendToGrid(wxSFShapeBase *shape)
{
    int row = m_arrCells.GetCount() / m_nCols;
    int col = m_arrCells.GetCount() - row*m_nCols;

    return InsertToGrid( row, col, shape );
}

bool wxSFGridShape::InsertToGrid(int row, int col, wxSFShapeBase *shape)
{
    wxASSERT(shape);

    if( shape && shape->IsKindOf(CLASSINFO(wxSFShapeBase)) && IsChildAccepted(shape->GetClassInfo()->GetClassName()) )
    {
        // add the shape to the children list if neccessary
        if( GetChildrenList().IndexOf(shape) == wxNOT_FOUND )
        {
            shape->Reparent(this);
        }

        // protect duplicated occurences
        if( m_arrCells.Index(shape->GetId()) != wxNOT_FOUND) return false;

        // protect unbounded horizontal index (grid can grow in a vertical direction only)
        if( row >= m_nRows ) return false;

        m_arrCells.SetCount(row * m_nCols + col + 1);
        m_arrCells[row * m_nCols + col] = shape->GetId();

        //if( m_nRows <= row ) m_nRows = row + 1;
        if( m_nCols <= col ) m_nCols = col + 1;

        return true;
    }

    return false;
}

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
        nIndex = nCol = 0;
        nRow = -1;

        for(size_t i = 0; i < m_arrCells.GetCount(); i++ )
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

                pShape->MoveTo( nAbsPos.x + nCol*maxRect.GetWidth() + (nCol+1)*m_nCellSpace,
                                nAbsPos.y + nRow*maxRect.GetHeight() + (nRow+1)*m_nCellSpace);
            }
        }
    }
}

void wxSFGridShape::Update()
{
    wxSFShapeBase *pShape;

    // check an existence of already assigned shapes
    for(size_t i = 0; i < m_arrCells.GetCount(); i++ )
    {
        if( !GetChild(m_arrCells[i])) m_arrCells.RemoveAt(i);
    }

    // check whether all child shapes' IDs are present in the cells array...
    SerializableList::compatibility_iterator node = GetFirstChildNode();
    while(node)
    {
        pShape = (wxSFShapeBase*)node->GetData();
        if( m_arrCells.Index(pShape->GetId()) == wxNOT_FOUND ) m_arrCells.Add(pShape->GetId());

        node = node->GetNext();
    }

    // do self-alignment
    DoAlignment();

    // do alignment of shape's children
    DoChildrenLayout();

    // fit the shape to its children
    this->FitToChildren();

    // do it recursively on all parent shapes
    if( GetParentShape() )GetParentShape()->Update();
}

void wxSFGridShape::FitToChildren()
{
    // HINT: overload it for custom actions...

    wxSFShapeBase* pChild;

    // get bounding box of the shape and children set be inside it
    wxRealPoint nAbsPos = GetAbsolutePosition();
    wxRect chBB = wxRect(nAbsPos.x, nAbsPos.y, 0, 0);

    SerializableList::compatibility_iterator node = GetFirstChildNode();
    while(node)
    {
        pChild = (wxSFShapeBase*)node->GetData();

        if( pChild->GetStyle() & sfsALWAYS_INSIDE )
        {
            pChild->GetCompleteBoundingBox(chBB, bbSELF | bbCHILDREN);
        }
        node = node->GetNext();
    }

    m_nRectSize = wxRealPoint(chBB.GetSize().x + 2*m_nCellSpace, chBB.GetSize().y + 2*m_nCellSpace);
}

void  wxSFGridShape::OnChildDropped(const wxRealPoint& pos, wxSFShapeBase *child)
{
    wxASSERT(child);

    if( child ) AppendToGrid( child );
}

//----------------------------------------------------------------------------------//
// public virtual functions
//----------------------------------------------------------------------------------//



