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
 * Item model mirroring all indices of another model which are children of an
 * arbitrary index.
 */
#ifndef SUBINDEXMODEL_H
#define SUBINDEXMODEL_H

#include <QAbstractProxyModel>

#include "ModelHierarchyMirror.h"

class SubindexModel : public QAbstractProxyModel,
                      public ModelHierarchyMirror<SubindexModel>
{
  Q_OBJECT
public:
  SubindexModel (QObject* parent = nullptr);
  ~SubindexModel ();

  QModelIndex index (int row, int col, const QModelIndex& parent) const override;
  QModelIndex parent (const QModelIndex& index) const override;
  int rowCount (const QModelIndex& index) const override;
  int columnCount (const QModelIndex& index) const override;

  QModelIndex mapFromSource (const QModelIndex& sourceIndex) const override;
  QModelIndex mapToSource (const QModelIndex& sourceIndex) const override;

  void setSourceModel(QAbstractItemModel* sourceModel) override;
  void setSourceRoot (const QModelIndex& sourceRootIndex);
protected:
  QModelIndex sourceRootIndex;
  friend class ModelHierarchyMirror<SubindexModel>;
};

#endif // SUBINDEXMODEL_H
