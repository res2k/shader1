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
  assert(rows.empty() || rows.root_sum() >= row);
  auto rows_it = rows.get_iterator_by_sum (row);
  if (rows_it == rows.end()) rows_it = rows.emplace_back();

  auto& cols = *rows_it;
  assert (cols.empty() || cols.root_sum() >= col);
  auto cols_it = cols.get_iterator_by_sum (col);
  if (cols_it == cols.end()) cols_it = cols.emplace_back();

  cols_it->parent = this;
  return &(*cols_it);
}

void NodeHierarchy::Node::Clear ()
{
  rows.clear ();
}

void NodeHierarchy::Node::LocateChild (const Node* child, int& row, int& col) const
{
  // Find ColumnsDataTree from Node
  auto col_it = ColumnsDataTree::s_iterator_to (*child);
  auto& cols = ColumnsDataTree::container_from_iterator (col_it);
  col = cols.get_sum_for_iterator (col_it);

  // Find RowsDataTree from cols
  auto row_it = RowsDataTree::s_iterator_to (static_cast<const RowsData&> (cols));
  auto& rows = RowsDataTree::container_from_iterator (row_it);
  row = rows.get_sum_for_iterator (row_it);
}

NodeHierarchy::Node* NodeHierarchy::Node::ChildFromIndex (int row, int col)
{
  auto rows_it = rows.get_iterator_by_sum (row);
  assert (rows_it != rows.end());

  auto& cols = *rows_it;
  auto cols_it = cols.get_iterator_by_sum (col);
  assert (cols_it != cols.end());

  return &(*cols_it);
}

void NodeHierarchy::Node::InsertColumns (int first, int last)
{
  for (auto& cols : rows)
  {
    if (!cols.empty() && (first < cols.root_sum ()))
    {
      auto insert_pos = cols.get_iterator_by_sum (first);
      for (int c = first; c <= last; c++)
      {
        auto new_node = cols.emplace (insert_pos);
        new_node->parent = this;
      }
    }
    else
    {
      for (int c = first; c <= last; c++)
      {
        auto new_node = cols.emplace_back ();
        new_node->parent = this;
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
    auto srcCol = srcRow->get_iterator_by_sum (start);
    auto srcColEnd = srcRow->get_iterator_by_sum (end + 1);

    auto dstCol = dstRow->get_iterator_by_sum (column);

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
    auto col = row->get_iterator_by_sum (first);
    auto colEnd = row->get_iterator_by_sum (last + 1);

    while (col != colEnd)
    {
      col = row->erase (col);
    }
  }
}

void NodeHierarchy::Node::InsertRows (int first, int last, int numColumns)
{
  auto create_columns =
    [=](RowsDataTree::iterator row)
    {
      auto& cols = *row;
      for (int c = 0; c < numColumns; c++)
      {
        auto new_node = cols.emplace_back();
        new_node->parent = this;
      }
    };

  if (!rows.empty() && (first < rows.root_sum ()))
  {
    auto insert_pos = rows.get_iterator_by_sum (first);
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
  auto srcRow = rows.get_iterator_by_sum (start);
  auto srcRowEnd = rows.get_iterator_by_sum (end + 1);

  auto dstRow = destination->rows.get_iterator_by_sum (row);

  while (srcRow != srcRowEnd)
  {
    srcRow = destination->rows.transfer (dstRow, srcRow);
  }
}

void NodeHierarchy::Node::RemoveRows (int first, int last)
{
  auto row = rows.get_iterator_by_sum (first);
  auto rowEnd = rows.get_iterator_by_sum (last + 1);

  while (row != rowEnd)
  {
    row = rows.erase (row);
  }
}
