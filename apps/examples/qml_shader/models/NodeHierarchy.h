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

/**\file
 * Helper file to manage a hierarchy of item model nodes.
 */
#ifndef NODEHIERARCHY_H
#define NODEHIERARCHY_H

#include "OrderStatTree.h"

#include <memory>

class NodeHierarchy
{
public:
  typedef boost::intrusive::set_base_hook<boost::intrusive::link_mode<boost::intrusive::normal_link>> set_base_hook;
  class Node : public set_base_hook
  {
  protected:
    Node* parent = nullptr;
    int sum = 0;

    typedef OrderStatTree<Node,
                          orderstat_tree::intrusive::sum_member<Node, int, &Node::sum>,
                          orderstat_tree::intrusive::addend_index<int>,
                          boost::intrusive::constant_time_size<false>> ColumnsDataTree;
    struct RowsData : public set_base_hook, public ColumnsDataTree
    {
      int sum = 0;
    };
    typedef OrderStatTree<RowsData,
                          orderstat_tree::intrusive::sum_member<RowsData, int, &RowsData::sum>,
                          orderstat_tree::intrusive::addend_index<int>,
                          boost::intrusive::constant_time_size<false>> RowsDataTree;
    RowsDataTree rows;
  public:
    Node();
    ~Node();

    Node* AddChild (int row, int col);
    //void RemoveChild (Node* child);

    void Clear ();

    Node* GetParent() const { return parent; }
    void LocateChild (const Node* child, int& row, int& col) const;
    Node* ChildFromIndex (int row, int col);

    void InsertColumns (int first, int last);
    void MoveColumns (int start, int end, Node* destination, int column);
    void RemoveColumns (int first, int last);
    void InsertRows (int first, int last, int numColumns);
    void MoveRows (int start, int end, Node* destination, int row);
    void RemoveRows (int first, int last);
  };


  NodeHierarchy ();

  Node* GetRoot () const { return &rootNode; }
protected:
  mutable Node rootNode;
};

#endif // NODEHIERARCHY_H
