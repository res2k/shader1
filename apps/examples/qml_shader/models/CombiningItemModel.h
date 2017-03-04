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
 * An implementation of QAbstractItemModel that routes item access to different
 * sub-models, giving the appearance of a single model.
 */
#ifndef COMBININGITEMMODEL_H
#define COMBININGITEMMODEL_H

#include <QAbstractItemModel>

#include "ModelHierarchyMirror.h"

#include <unordered_map>

/**
 * An implementation of QAbstractItemModel that routes item access to different
 * sub-models, giving the appearance of a single model.
 * \remarks This was implemented with models from file system(s) in mind and
 *  is thus likely not as generic as the name suggests.
 */
class CombiningItemModel : public QAbstractItemModel,
                           public ModelHierarchyMirror<CombiningItemModel>
{
  Q_OBJECT
public:
  CombiningItemModel (QObject* parent = nullptr);
  ~CombiningItemModel ();

  /// Add a model.
  void addModel (QAbstractItemModel* model);

  bool canFetchMore (const QModelIndex& parent) const override;
  int columnCount (const QModelIndex& parent = QModelIndex()) const override;
  QVariant data (const QModelIndex& index, int role = Qt::DisplayRole) const override;
  void fetchMore (const QModelIndex &parent) override;
  bool hasChildren (const QModelIndex& parent = QModelIndex()) const override;
  QModelIndex index (int row, int column, const QModelIndex& parent = QModelIndex()) const override;
  QModelIndex	parent (const QModelIndex& index) const override;
  int	rowCount (const QModelIndex& parent = QModelIndex()) const override;
protected:
  int totalColumnCount = 0;

  /// Translate a model index into a source model + it's model index
  std::pair<QAbstractItemModel*, QModelIndex> TranslateModelIndex (const QModelIndex& index) const;
  /// Translate a source model + it's index into a model index
  QModelIndex TranslateSourceIndex (QAbstractItemModel* model, const QModelIndex& index) const;

  /// Convenience access to source model, for use from slots only!
  QAbstractItemModel* sourceModel() const;
  /// Convenience wrapper for TranslateSourceIndex(), for use from slots only!
  QModelIndex mapFromSource (const QModelIndex& index) const;

  /* This combination of data structures was chosen because it can:
   * 1. Retain original order of insertion (for display purposes)
   * 2. Quickly go from a row number to the model for that row
   * 3. Quickly handle changes in row counts
   * 4. Go from model to associated data (and it's row number)
   */
  typedef sum_tree::Tree<QAbstractItemModel*, sum_tree::sum_traits::user_value<>, sum_tree::value_traits::unsorted> ModelsTree;
  ModelsTree modelsTree;
  std::unordered_map<QAbstractItemModel*, ModelsTree::node_type> modelTreeNodeMap;

  friend class ModelHierarchyMirror<CombiningItemModel>;
protected slots:
  void sourceRowsChanged (const QModelIndex& parent);
};

#endif // COMBININGITEMMODEL_H
