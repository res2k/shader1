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

#include "SubindexModel.h"

SubindexModel::SubindexModel (QObject* parent) : QAbstractProxyModel (parent)
{}

SubindexModel::~SubindexModel ()
{}

QModelIndex SubindexModel::index (int row, int col, const QModelIndex& parent) const
{
  auto parentNodeData = ResolveNodeData (parent);
  return createIndex (row, col, parentNodeData);
}

QModelIndex SubindexModel::parent (const QModelIndex& index) const
{
  // get parentNodeData from index user ptr
  auto parentNodeData = reinterpret_cast<NodeHierarchy::Node*> (index.internalPointer());
  if (parentNodeData == nodeData.GetRoot()) return QModelIndex();
  // create new index using parentNodeData
  int parentRow, parentCol;
  parentNodeData->GetParent()->LocateChild (parentNodeData, parentRow, parentCol);
  return createIndex (parentRow, parentCol, parentNodeData->GetParent());
}

int SubindexModel::rowCount (const QModelIndex& index) const
{
  if (!sourceModel()) return 0;
  return sourceModel()->rowCount (mapToSource (index));
}

int SubindexModel::columnCount (const QModelIndex& index) const
{
  if (!sourceModel()) return 0;
  return sourceModel()->columnCount (mapToSource (index));
}

QModelIndex SubindexModel::mapFromSource (const QModelIndex& sourceIndex) const
{
  if (sourceIndex == sourceRootIndex) return QModelIndex();
  if (!sourceModel()) return QModelIndex();
  return index (sourceIndex.row(), sourceIndex.column(), mapFromSource (sourceIndex.parent()));
}

QModelIndex SubindexModel::mapToSource (const QModelIndex& sourceIndex) const
{
  if (!sourceIndex.isValid()) return sourceRootIndex;
  if (!sourceModel()) return QModelIndex();
  return sourceModel()->index (sourceIndex.row(), sourceIndex.column(), mapToSource (sourceIndex.parent()));
}

void SubindexModel::setSourceModel(QAbstractItemModel* sourceModel)
{
  if (this->sourceModel())
  {
    DisconnectSignals (this->sourceModel());
  }

  if (sourceModel != this->sourceModel())
  {
    nodeData.GetRoot ()->Clear ();
  }
  QAbstractProxyModel::setSourceModel (sourceModel);

  if (this->sourceModel())
  {
    ConnectSignals (this->sourceModel());
  }
}

void SubindexModel::setSourceRoot (const QModelIndex& sourceRootIndex)
{
  if (this->sourceRootIndex != sourceRootIndex)
  {
    auto oldSourceModel = sourceModel();
    setSourceModel (nullptr);
    this->sourceRootIndex = sourceRootIndex;
    setSourceModel (oldSourceModel);

    if (sourceModel ())
    {
      int sourceRows = sourceModel ()->rowCount (sourceRootIndex);
      int sourceCols = sourceModel ()->columnCount (sourceRootIndex);
      for (int r = 0; r < sourceRows; r++)
      {
        for (int c = 0; c < sourceCols; c++)
        {
          nodeData.GetRoot ()->AddChild (r, c);
        }
      }
    }
  }
}
