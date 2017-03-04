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

#include "FosterModel.h"

FosterModel::FosterModel (QObject* parent) : QAbstractProxyModel (parent)
{
  nodeData.GetRoot ()->AddChild (0, 0);
  fosterIndex = createIndex (0, 0, nodeData.GetRoot());
}

FosterModel::~FosterModel ()
{}

QModelIndex FosterModel::index (int row, int col, const QModelIndex& parent) const
{
  auto parentNodeData = ResolveNodeData (parent);
  return createIndex (row, col, parentNodeData);
}

QModelIndex FosterModel::parent (const QModelIndex& index) const
{
  // get parentNodeData from index user ptr
  auto parentNodeData = reinterpret_cast<NodeHierarchy::Node*> (index.internalPointer());
  if (parentNodeData == nodeData.GetRoot()) return QModelIndex();
  // create new index using parentNodeData
  int parentRow, parentCol;
  parentNodeData->GetParent()->LocateChild (parentNodeData, parentRow, parentCol);
  return createIndex (parentRow, parentCol, parentNodeData->GetParent());
}

int FosterModel::rowCount (const QModelIndex& index) const
{
  if (!index.isValid())
    return 1;

  if (!sourceModel()) return 0;
  return sourceModel()->rowCount (mapToSource (index));
}

int FosterModel::columnCount (const QModelIndex& index) const
{
  if (!index.isValid())
    return 1;

  if (!sourceModel()) return 0;
  return sourceModel()->columnCount (mapToSource (index));
}

QVariant FosterModel::data (const QModelIndex& index, int role) const
{
  if (index == fosterIndex)
  {
    return fosterData.value (role, QVariant());
  }
  return QAbstractProxyModel::data (index, role);
}

QMap<int, QVariant> FosterModel::itemData (const QModelIndex& index) const
{
  if (index == fosterIndex)
  {
    return fosterData;
  }
  return QAbstractProxyModel::itemData (index);
}

Qt::ItemFlags FosterModel::flags (const QModelIndex& index) const
{
  if (index == fosterIndex)
  {
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
  }
  return QAbstractProxyModel::flags (index);
}

QModelIndex FosterModel::mapFromSource (const QModelIndex& sourceIndex) const
{
  if (!sourceIndex.isValid()) return fosterIndex;
  if (!sourceModel()) return QModelIndex();
  return index (sourceIndex.row(), sourceIndex.column(), mapFromSource (sourceIndex.parent()));
}

QModelIndex FosterModel::mapToSource (const QModelIndex& sourceIndex) const
{
  if (!sourceIndex.isValid() || (sourceIndex == fosterIndex)) return QModelIndex();
  if (!sourceModel()) return QModelIndex();
  return sourceModel()->index (sourceIndex.row(), sourceIndex.column(), mapToSource (sourceIndex.parent()));
}

void FosterModel::setSourceModel(QAbstractItemModel* sourceModel)
{
  if (this->sourceModel())
  {
    DisconnectSignals (this->sourceModel());
  }

  if (sourceModel != this->sourceModel())
  {
    auto fosterNodeData = ResolveNodeData (fosterIndex);
    fosterNodeData->Clear ();

    if (sourceModel)
    {
      int sourceRows = sourceModel->rowCount (QModelIndex());
      int sourceCols = sourceModel->columnCount (QModelIndex());
      for (int r = 0; r < sourceRows; r++)
      {
        for (int c = 0; c < sourceCols; c++)
        {
          fosterNodeData->AddChild (r, c);
        }
      }
    }
  }
  QAbstractProxyModel::setSourceModel (sourceModel);

  if (this->sourceModel())
  {
    ConnectSignals (this->sourceModel());
  }
}

void FosterModel::setFosterData (const QMap<int, QVariant>& data)
{
  fosterData = data;
  emit dataChanged (fosterIndex, fosterIndex, data.keys().toVector());
}
