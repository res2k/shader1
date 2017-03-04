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
 * Helper class to mirror the model index hierarchy of some other model.
 */
#ifndef MODELHIERARCHYMIRROR_H
#define MODELHIERARCHYMIRROR_H

#include <QAbstractItemModel>

#include "NodeHierarchy.h"

#include <map>

#include <boost/container/static_vector.hpp>

template<typename Derived>
class ModelHierarchyMirror
{
private:
  typedef boost::container::static_vector<QMetaObject::Connection, 6> connections_vector;
  std::map<QAbstractItemModel*, connections_vector> connections;

  template<typename... A>
  void DoConnect (connections_vector& connections, A&&... connectArgs);
protected:
  void ConnectSignals (QAbstractItemModel* model);
  void DisconnectSignals (QAbstractItemModel* model);

  NodeHierarchy nodeData;
  NodeHierarchy::Node* ResolveNodeData (const QModelIndex& index) const;

  void sourceColumnsInserted (const QModelIndex& parent, int first, int last);
  void sourceColumnsMoved (const QModelIndex& parent, int start, int end, const QModelIndex& destination, int column);
  void sourceColumnsRemoved (const QModelIndex& parent, int first, int last);
  void sourceRowsInserted (const QModelIndex& parent, int first, int last);
  void sourceRowsMoved (const QModelIndex& parent, int start, int end, const QModelIndex& destination, int row);
  void sourceRowsRemoved (const QModelIndex& parent, int first, int last);
};

template<typename Derived> template<typename... A>
void ModelHierarchyMirror<Derived>::DoConnect (connections_vector& connections, A&&... connectArgs)
{
  connections.push_back (QObject::connect (std::forward<A> (connectArgs)...));
}

template<typename Derived>
void ModelHierarchyMirror<Derived>::ConnectSignals (QAbstractItemModel* model)
{
  auto connections_it = connections.find (model);
  bool model_connected = (connections_it != connections.end()) && !connections_it->second.empty();
  assert (!model_connected);
  if (model_connected) return;

  auto this_model = static_cast<Derived*> (this);
  connections_vector new_connections;
  new_connections.reserve (6);
  DoConnect (new_connections,
             model, &QAbstractItemModel::columnsInserted,
             this_model,
             [=](const QModelIndex& parent, int first, int last)
             {
               sourceColumnsInserted (parent, first, last);
             });
  DoConnect (new_connections,
             model, &QAbstractItemModel::columnsMoved,
             this_model,
             [=](const QModelIndex& parent, int start, int end, const QModelIndex& destination, int column)
             {
               sourceColumnsMoved (parent, start, end, destination, column);
             });
  DoConnect (new_connections,
             model, &QAbstractItemModel::columnsRemoved,
             this_model,
             [=](const QModelIndex& parent, int first, int last)
             {
               sourceColumnsRemoved (parent, first, last);
             });
  DoConnect (new_connections,
             model, &QAbstractItemModel::rowsInserted,
             this_model,
             [=](const QModelIndex& parent, int first, int last)
             {
               sourceRowsInserted (parent, first, last);
             });
  DoConnect (new_connections,
             model, &QAbstractItemModel::rowsMoved,
             this_model,
             [=](const QModelIndex& parent, int start, int end, const QModelIndex& destination, int row)
             {
               sourceRowsMoved (parent, start, end, destination, row);
             });
  DoConnect (new_connections,
             model, &QAbstractItemModel::rowsRemoved,
             this_model,
             [=](const QModelIndex& parent, int first, int last)
             {
               sourceRowsRemoved (parent, first, last);
             });

  connections[model] = std::move (new_connections);
}

template<typename Derived>
void ModelHierarchyMirror<Derived>::DisconnectSignals (QAbstractItemModel* model)
{
  auto connections_it = connections.find (model);
  assert (connections_it != connections.end());
  if (connections_it == connections.end()) return;

  for (const auto& c : connections_it->second)
  {
    QObject::disconnect (c);
  }
  connections.erase (connections_it);
}

template<typename Derived>
NodeHierarchy::Node* ModelHierarchyMirror<Derived>::ResolveNodeData (const QModelIndex& index) const
{
  if (!index.isValid()) return nodeData.GetRoot();
  auto parentNodeData = reinterpret_cast<NodeHierarchy::Node*> (index.internalPointer());
  return parentNodeData->ChildFromIndex (index.row(), index.column());
}

template<typename Derived>
void ModelHierarchyMirror<Derived>::sourceColumnsInserted (const QModelIndex& parent, int first, int last)
{
  auto this_model = static_cast<Derived*> (this);
  auto myIndex = this_model->mapFromSource (parent);

  this_model->beginInsertColumns (myIndex, first, last);
  auto nodeData = ResolveNodeData (myIndex);
  nodeData->InsertColumns (first, last);
  this_model->endInsertColumns ();
}

template<typename Derived>
void ModelHierarchyMirror<Derived>::sourceColumnsMoved (const QModelIndex& parent, int start, int end, const QModelIndex& destination, int column)
{
  auto this_model = static_cast<Derived*> (this);
  auto myFrom = this_model->mapFromSource (parent);
  auto myTo = this_model->mapFromSource (parent);

  this_model->beginMoveColumns (myFrom, start, end, myTo, column);
  auto fromData = ResolveNodeData (myFrom);
  auto toData = ResolveNodeData (myTo);
  fromData->MoveColumns (start, end, toData, column);
  this_model->endMoveColumns ();
}

template<typename Derived>
void ModelHierarchyMirror<Derived>::sourceColumnsRemoved (const QModelIndex& parent, int first, int last)
{
  auto this_model = static_cast<Derived*> (this);
  auto myIndex = this_model->mapFromSource (parent);

  this_model->beginRemoveColumns (myIndex, first, last);
  auto fromData = ResolveNodeData (this_model->mapFromSource (parent));
  fromData->RemoveColumns (first, last);
  this_model->endRemoveColumns ();
}

template<typename Derived>
void ModelHierarchyMirror<Derived>::sourceRowsInserted (const QModelIndex& parent, int first, int last)
{
  auto this_model = static_cast<Derived*> (this);
  auto myIndex = this_model->mapFromSource (parent);

  this_model->beginInsertRows (myIndex, first, last);
  auto nodeData = ResolveNodeData (myIndex);
  nodeData->InsertRows (first, last, parent.model()->columnCount (parent));
  this_model->endInsertRows ();
}

template<typename Derived>
void ModelHierarchyMirror<Derived>::sourceRowsMoved (const QModelIndex& parent, int start, int end, const QModelIndex& destination, int row)
{
  auto this_model = static_cast<Derived*> (this);
  auto myFrom = this_model->mapFromSource (parent);
  auto myTo = this_model->mapFromSource (parent);

  this_model->beginMoveRows (myFrom, start, end, myTo, row);
  auto fromData = ResolveNodeData (myFrom);
  auto toData = ResolveNodeData (myTo);
  fromData->MoveRows (start, end, toData, row);
  this_model->endMoveRows ();
}

template<typename Derived>
void ModelHierarchyMirror<Derived>::sourceRowsRemoved (const QModelIndex& parent, int first, int last)
{
  auto this_model = static_cast<Derived*> (this);
  auto myIndex = this_model->mapFromSource (parent);

  this_model->beginRemoveRows (myIndex, first, last);
  auto fromData = ResolveNodeData (myIndex);
  fromData->RemoveRows (first, last);
  this_model->endRemoveRows ();
}

#endif // MODELHIERARCHYMIRROR_H
