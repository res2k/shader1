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

#include "AssetBrowsingWidget.h"

#include "ui_AssetBrowsingWidget.h"

#include "AssetBrowsingModel.h"
#include "CustomItemDataRole.h"

#include "models/CustomDataModel.h"

#include <QFileInfo>
#include <QUrl>

AssetBrowsingWidget::AssetBrowsingWidget (QWidget* parent)
  : QWidget (parent)
{
  ui.reset (new Ui::AssetBrowsingWidget);
  ui->setupUi (this);

  assetsModel = new AssetBrowsingModel (this);

  // Only show icons for folders
  auto assetsModelFolderIconsOnly = make_CustomDataModel (
    [=](const QModelIndex &index, int role) -> QVariant
    {
      if (role == Qt::DecorationRole)
      {
        if (!assetsModel->hasChildren (index)) return QIcon();
      }
      return QVariant();
    }, assetsModel, this);

  ui->assetTree->setModel (assetsModelFolderIconsOnly);

  connect (assetsModelFolderIconsOnly, &QAbstractItemModel::columnsInserted,
           this, [=](){ hideExtraColumns(); });
  connect (assetsModelFolderIconsOnly, &QAbstractItemModel::columnsRemoved,
           this, [=](){ hideExtraColumns(); });

  connect (ui->assetTree->selectionModel(), &QItemSelectionModel::currentChanged,
           this, &AssetBrowsingWidget::treeItemSelected);
}

AssetBrowsingWidget::~AssetBrowsingWidget()
{
}

void AssetBrowsingWidget::hideExtraColumns ()
{
  // Hide all columns but the first
  for (int c = 0, num_cols = assetsModel->columnCount (); c < num_cols; c++)
  {
    ui->assetTree->setColumnHidden (c, c != 0);
  }
}

void AssetBrowsingWidget::selectFile (const QString& name)
{
  auto index =
    ui->assetTree->model()->match (ui->assetTree->model()->index (0, 0),
                                   Qt::DisplayRole, name, 1,
                                   Qt::MatchRecursive);
  if (!index.isEmpty ())
  {
    ui->assetTree->scrollTo (index[0]);
    ui->assetTree->setCurrentIndex (index[0]);
  }
}

void AssetBrowsingWidget::treeItemSelected (const QModelIndex& index)
{
  auto item_data = ui->assetTree->model ()->data (index, FullUrlRole);
  // Ignore items with empty URL
  if (!item_data.isValid()) return;
  auto url = item_data.toUrl ();
  if (url.isEmpty ()) return;
  // Ignore directories
  if (url.isLocalFile ())
  {
    auto localPath = url.toLocalFile ();
    QFileInfo info (localPath);
    if (!info.isFile ()) return;
  }
  emit urlSelected (url);
}
