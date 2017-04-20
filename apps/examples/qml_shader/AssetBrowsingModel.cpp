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

#include "AssetBrowsingModel.h"

#include "CustomItemDataRole.h"

#include "models/CustomDataModel.h"
#include "models/FosterModel.h"
#include "models/SubindexModel.h"

#include <QApplication>
#include <QFileSystemModel>
#include <QDir>
#include <QStringBuilder>
#include <QStringListModel>
#include <QStyle>
#include <QUrl>

AssetBrowsingModel::AssetBrowsingModel (QObject* parent) : CombiningItemModel (parent) {}

void AssetBrowsingModel::setBuiltinsPath  (const QString& path, const QString& label)
{
  auto builtinsModel = new QStringListModel (this);
  QDir builtinsDir (path);
  builtinsModel->setStringList (builtinsDir.entryList (QDir::NoFilter, QDir::Name));
  QString path_copy = path; // Make sure lambda gets a copy
  auto builtinsModelWithURL = make_CustomDataModel (
    [=](const QModelIndex& index, int role) -> QVariant
    {
      if (role == FullUrlRole)
      {
        auto filename = builtinsModel->data (index, Qt::DisplayRole).toString ();
        return QUrl::fromLocalFile (path_copy % filename);
      }
      return QVariant();
    }, builtinsModel, this);
  auto builtinsModelModelFoster = new FosterModel (this);
  builtinsModelModelFoster->setSourceModel (builtinsModelWithURL);
  QMap<int, QVariant> fosterData;
  fosterData[Qt::DisplayRole] = label;
  fosterData[Qt::DecorationRole] = QApplication::style()->standardIcon (QStyle::SP_DirIcon);
  builtinsModelModelFoster->setFosterData (fosterData);
  
  /* TODO?: Use insertion instead to keep some sort of order?
   * (Or replacement?) */
  addModel (builtinsModelModelFoster);
}

void AssetBrowsingModel::setFileSystemNameFilters (const QStringList& nameFilters)
{
  this->nameFilters = nameFilters;
}

void AssetBrowsingModel::addFileSystemPath (const QString& path)
{
  auto fsModel = new QFileSystemModel (this);
  auto fsIndex = fsModel->setRootPath (path);
  fsModel->setNameFilters (nameFilters);
  fsModel->setNameFilterDisables (false);
  auto fsModelWithURL = make_CustomDataModel (
    [=](const QModelIndex &index, int role) -> QVariant
    {
      if (role == FullUrlRole)
        return QUrl::fromLocalFile (fsModel->filePath (index));
      return QVariant();
    }, fsModel, this);

  auto fsSubmodel = new SubindexModel (this);
  fsSubmodel->setSourceModel (fsModelWithURL);
  fsSubmodel->setSourceRoot (fsModelWithURL->mapFromSource (fsIndex).parent());

  addModel (fsSubmodel);
}
