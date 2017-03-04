/*
    Shader1
    Copyright (c) 2017 Frank Richter


    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
    distribution.
*/

#include "NodeHierarchy.h"

NodeHierarchy::NodeHierarchy () {}

//---------------------------------------------------------------------------

NodeHierarchy::Node::Node () {}

NodeHierarchy::Node::~Node ()
{
  Clear ();
}

NodeHierarchy::Node* NodeHierarchy::Node::AddChild (int row, int col)
{
  assert(rows.root_sum() >= row);
  auto rows_node = rows.get_node_by_sum (row);
  if (!rows_node) rows_node = rows.emplace_back();

  auto& cols = rows.get_node_data (rows_node);
  assert (cols.root_sum() >= col);
  auto cols_node = cols.get_node_by_sum (col);
  if (!cols_node) cols_node = cols.emplace_back();

  auto newNodePtr = &(cols.get_node_data (cols_node));
  newNodePtr->parent = this;
  return newNodePtr;
}

void NodeHierarchy::Node::Clear ()
{
  rows.clear ();
}

void NodeHierarchy::Node::LocateChild (const Node* child, int& row, int& col) const
{
  // Find ColumnsDataTree from Node
  auto col_node = ColumnsDataTree::get_data_node (*child);
  auto& cols = ColumnsDataTree::get_tree (col_node);
  col = cols.get_sum_for_node (col_node);

  // Find RowsDataTree from cols
  auto row_node = RowsDataTree::get_data_node (cols);
  auto& rows = RowsDataTree::get_tree (row_node);
  row = rows.get_sum_for_node (row_node);
}

NodeHierarchy::Node* NodeHierarchy::Node::ChildFromIndex (int row, int col)
{
  auto rows_node = rows.get_node_by_sum (row);
  assert (rows_node);

  auto& cols = rows.get_node_data (rows_node);
  auto cols_node = cols.get_node_by_sum (col);
  assert (cols_node);

  return &(ColumnsDataTree::get_node_data (cols_node));
}

void NodeHierarchy::Node::InsertColumns (int first, int last)
{
  for (auto& cols : rows)
  {
    if (first < cols.root_sum ())
    {
      auto insert_pos = cols.get_node_by_sum (first);
      for (int c = first; c <= last; c++)
      {
        auto new_node = cols.emplace (insert_pos);
        ColumnsDataTree::get_node_data (new_node).parent = this;
      }
    }
    else
    {
      for (int c = first; c <= last; c++)
      {
        auto new_node = cols.emplace_back ();
        ColumnsDataTree::get_node_data (new_node).parent = this;
      }
    }
  }
}

void NodeHierarchy::Node::MoveColumns (int start, int end, Node* destination, int column)
{
  for (auto srcRow = rows.begin(),
            srcEnd = rows.end(),
            dstRow = destination->rows.begin(),
            dstEnd = destination->rows.end();
       (srcRow != srcEnd) && (dstRow != dstEnd);
       ++srcRow, ++dstRow)
  {
    auto srcCol = srcRow->get_node_by_sum (start);
    auto srcColEnd = srcRow->get_node_by_sum (end + 1);
    if (!srcColEnd) srcColEnd = srcRow->end_node();

    auto dstCol = dstRow->get_node_by_sum (column);
    if (!dstCol) dstCol = dstRow->end_node();

    while (srcCol != srcColEnd)
    {
      srcCol = dstRow->transfer (dstCol, srcCol);
    }
  }
}

void NodeHierarchy::Node::RemoveColumns (int first, int last)
{
  for (auto row = rows.begin(),
            rowEnd = rows.end();
       row != rowEnd;
       ++row)
  {
    auto col = row->get_node_by_sum (first);
    auto colEnd = row->get_node_by_sum (last + 1);
    if (!colEnd) colEnd = row->end_node();

    while (col != colEnd)
    {
      auto next_col = row->next_node (col);
      row->erase_node (col);
      col = next_col;
    }
  }
}

void NodeHierarchy::Node::InsertRows (int first, int last, int numColumns)
{
  auto create_columns =
    [=](RowsDataTree::node_type row)
    {
      auto& cols = RowsDataTree::get_node_data (row);
      for (int c = 0; c < numColumns; c++)
      {
        auto new_node = cols.emplace_back();
        ColumnsDataTree::get_node_data (new_node).parent = this;
      }
    };

  if (first < rows.root_sum ())
  {
    auto insert_pos = rows.get_node_by_sum (first);
    for (int r = first; r <= last; r++)
    {
      auto new_row = rows.emplace (insert_pos);
      create_columns (new_row);
    }
  }
  else
  {
    for (int r = first; r <= last; r++)
    {
      auto new_row = rows.emplace_back ();
      create_columns (new_row);
    }
  }
}

void NodeHierarchy::Node::MoveRows (int start, int end, Node* destination, int row)
{
  auto srcRow = rows.get_node_by_sum (start);
  auto srcRowEnd = rows.get_node_by_sum (end + 1);
  if (!srcRowEnd) srcRowEnd = rows.end_node();

  auto dstRow = destination->rows.get_node_by_sum (row);
  if (!dstRow) dstRow = destination->rows.end_node();

  while (srcRow != srcRowEnd)
  {
    srcRow = destination->rows.transfer (dstRow, srcRow);
  }
}

void NodeHierarchy::Node::RemoveRows (int first, int last)
{
  auto row = rows.get_node_by_sum (first);
  auto rowEnd = rows.get_node_by_sum (last + 1);
  if (!rowEnd) rowEnd = rows.end_node();

  while (row != rowEnd)
  {
    auto next_row = rows.next_node (row);
    rows.erase_node (row);
    row = next_row;
  }
}
