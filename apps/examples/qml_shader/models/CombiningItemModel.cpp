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

#include "CombiningItemModel.h"

#include <assert.h>

CombiningItemModel::CombiningItemModel (QObject* parent) : QAbstractItemModel (parent)
{}

CombiningItemModel::~CombiningItemModel ()
{}

void CombiningItemModel::addModel (QAbstractItemModel* model)
{
  int oldColumnCount = totalColumnCount;
  int newColumnCount = std::max (oldColumnCount, model->columnCount());
  if (newColumnCount > oldColumnCount)
  {
    beginInsertColumns (QModelIndex(), oldColumnCount, newColumnCount+1);
  }
  totalColumnCount = newColumnCount;

  int lastRow = static_cast<int> (rowCount());
  int modelRows = model->rowCount();
  beginInsertRows (QModelIndex(), lastRow, lastRow + modelRows);

  int sourceCols = model->columnCount (QModelIndex());
  for (int r = 0; r < modelRows; r++)
  {
    for (int c = 0; c < sourceCols; c++)
    {
      nodeData.GetRoot ()->AddChild (r + lastRow, c);
    }
  }

  ConnectSignals (model);

  auto tree_node = modelsTree.emplace_back (model);
  modelsTree.set_node_value (tree_node, modelRows);
  modelTreeNodeMap[model] = tree_node;

  /* Subscribe to signals to we can update the modelsTree if the root row count changes.
   * TODO?: Is this sufficient? What if the ModelHierarchyMirror<> signal handlers
   * trigger a slot that's called immediately and, in turn, would use methods that
   * require an accurate modelsTree? */
  connect (model, &QAbstractItemModel::rowsInserted,
           this, &CombiningItemModel::sourceRowsChanged);
  connect (model, &QAbstractItemModel::rowsMoved,
           this, &CombiningItemModel::sourceRowsChanged);
  connect (model, &QAbstractItemModel::rowsRemoved,
           this, &CombiningItemModel::sourceRowsChanged);

  endInsertRows();

  if (newColumnCount > oldColumnCount)
  {
    endInsertColumns ();
  }
}

bool CombiningItemModel::canFetchMore (const QModelIndex& parent) const
{
  if (!parent.isValid()) return false;
  auto modelAndIndex = TranslateModelIndex (parent);
  return modelAndIndex.first->canFetchMore (modelAndIndex.second);
}

int CombiningItemModel::columnCount (const QModelIndex& parent) const
{
  if (!parent.isValid())
  {
    return totalColumnCount;
  }
  else
  {
    auto modelAndIndex = TranslateModelIndex (parent);
    return modelAndIndex.first->columnCount (modelAndIndex.second);
  }
}

QVariant CombiningItemModel::data (const QModelIndex &index, int role) const
{
  auto modelAndIndex = TranslateModelIndex (index);
  assert(modelAndIndex.first);
  if (!modelAndIndex.first) return QVariant ();

  return modelAndIndex.first->data (modelAndIndex.second, role);
}

void CombiningItemModel::fetchMore (const QModelIndex &parent)
{
  if (!parent.isValid()) return;
  auto modelAndIndex = TranslateModelIndex (parent);
  modelAndIndex.first->fetchMore (modelAndIndex.second);
}

bool CombiningItemModel::hasChildren (const QModelIndex& parent) const
{
  if (!parent.isValid()) return !modelsTree.empty();
  auto modelAndIndex = TranslateModelIndex (parent);
  return modelAndIndex.first->hasChildren (modelAndIndex.second);
}

QModelIndex CombiningItemModel::index (int row, int col, const QModelIndex& parent) const
{
  auto parentNodeData = ResolveNodeData (parent);
  return createIndex (row, col, parentNodeData);
}

QModelIndex CombiningItemModel::parent (const QModelIndex& index) const
{
  // get parentNodeData from index user ptr
  auto parentNodeData = reinterpret_cast<NodeHierarchy::Node*> (index.internalPointer());
  if (parentNodeData == nodeData.GetRoot()) return QModelIndex();
  // create new index using parentNodeData
  int parentRow, parentCol;
  parentNodeData->GetParent()->LocateChild (parentNodeData, parentRow, parentCol);
  return createIndex (parentRow, parentCol, parentNodeData->GetParent());
}

int CombiningItemModel::rowCount (const QModelIndex &parent) const
{
  if (!parent.isValid())
  {
    return modelsTree.root_sum ();
  }
  else
  {
    auto modelAndIndex = TranslateModelIndex (parent);
    return modelAndIndex.first->rowCount (modelAndIndex.second);
  }
}

std::pair<QAbstractItemModel*, QModelIndex> CombiningItemModel::TranslateModelIndex (const QModelIndex& index) const
{
  // Index invalid -> return as-is
  if (!index.isValid()) return std::make_pair (nullptr, index);

  // Index has a parent -> look up parent
  if (index.parent().isValid())
  {
    auto indexInParent = TranslateModelIndex (index.parent());
    QModelIndex newIndex = indexInParent.first->index (index.row(), index.column(), indexInParent.second);
    return std::make_pair (indexInParent.first, newIndex);
  }

  // Index is top level, extract original model & row inside of it
  auto tree_node = modelsTree.get_node_by_sum (index.row ());
  auto model = modelsTree.get_node_data (tree_node);
  auto firstModelRow = modelsTree.get_sum_for_node (tree_node);
  return std::make_pair (model, model->index (index.row() - firstModelRow, index.column()));
}

QModelIndex CombiningItemModel::TranslateSourceIndex (QAbstractItemModel* model, const QModelIndex& index) const
{
  if (index.parent().isValid())
  {
    auto parentIndex = TranslateSourceIndex (model, index.parent());
    return this->index (index.row(), index.column(), parentIndex);
  }

  auto tree_node = modelTreeNodeMap.find (model)->second;
  assert (modelsTree.get_node_data (tree_node) == model);
  auto firstModelRow = modelsTree.get_sum_for_node (tree_node);
  return this->index (firstModelRow + index.row(), index.column(), QModelIndex());
}

QAbstractItemModel* CombiningItemModel::sourceModel() const
{
  auto model = qobject_cast<QAbstractItemModel*> (sender());
  assert (model);
  return model;
}

QModelIndex CombiningItemModel::mapFromSource (const QModelIndex& index) const
{
  return TranslateSourceIndex (sourceModel(), index);
}

void CombiningItemModel::sourceRowsChanged (const QModelIndex& parent)
{
  if (parent.isValid ()) return;

  // Handle a change in the top-level row count
  auto model = sourceModel ();
  auto tree_node = modelTreeNodeMap.find (model)->second;
  assert (modelsTree.get_node_data (tree_node) == model);
  modelsTree.set_node_value (tree_node, model->rowCount());
}
