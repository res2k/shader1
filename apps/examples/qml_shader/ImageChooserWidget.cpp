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

#include "ImageChooserWidget.h"

#include "ui_ImageChooserWidget.h"

#include "CustomItemDataRole.h"
#include "ImageLocation.h"

#include "models/CombiningItemModel.h"
#include "models/CustomDataModel.h"
#include "models/FosterModel.h"
#include "models/SubindexModel.h"

#include <QDir>
#include <QFileSystemModel>
#include <QImageReader>
#include <QStandardPaths>
#include <QStringBuilder>
#include <QStringListModel>

ImageChooserWidget::ImageChooserWidget (ImageLocation* imageLocation, QWidget* parent)
  : QWidget (parent), imageLocation (imageLocation)
{
  ui.reset (new Ui::ImageChooserWidget);
  ui->setupUi (this);

  auto imagesPath = QStringLiteral (":/images/");
  connect (ui->imageTree, &QTreeView::clicked,
           this, &ImageChooserWidget::treeItemClicked);
  auto defaultImage = imageLocation->url ().fileName ();

  // Set up contents of Image selection widget
  auto imagesModel = new CombiningItemModel (this);

  {
    auto builtinImagesModel = new QStringListModel (this);
    QDir builtinImagesDir (imagesPath);
    builtinImagesModel->setStringList (builtinImagesDir.entryList (QDir::NoFilter, QDir::Name));
    auto builtinImagesModelWithURL = make_CustomDataModel (
      [=](const QModelIndex &index, int role) -> QVariant
      {
        if (role == FullUrlRole)
        {
          auto filename = builtinImagesModel->data (index, Qt::DisplayRole).toString ();
          return QUrl::fromLocalFile (imagesPath % filename);
        }
        return QVariant();
      }, builtinImagesModel, this);
    auto builtinImagesModelFoster = new FosterModel (this);
    builtinImagesModelFoster->setSourceModel (builtinImagesModelWithURL);
    QMap<int, QVariant> fosterData;
    fosterData[Qt::DisplayRole] = tr ("Builtin Images");
    builtinImagesModelFoster->setFosterData (fosterData);
    imagesModel->addModel (builtinImagesModelFoster);
  }

  {
    const auto userPicturesPaths = QStandardPaths::standardLocations (QStandardPaths::PicturesLocation);
    for (const auto& picturePath : userPicturesPaths)
    {
      auto userPicturesModel = new QFileSystemModel (this);
      auto userPicturesIndex = userPicturesModel->setRootPath (picturePath);
      auto userPicturesModelWithURL = make_CustomDataModel (
        [=](const QModelIndex &index, int role) -> QVariant
        {
          if (role == FullUrlRole)
            return QUrl::fromLocalFile (userPicturesModel->filePath (index));
          return QVariant();
        }, userPicturesModel, this);

      // Set up filesystem model to only show images supported by Qt
      const auto supportedFormats = QImageReader::supportedImageFormats ();
      QStringList supportedFormatFilters;
      for (const auto& format : supportedFormats)
      {
        QString extension = QStringLiteral ("*.") % QString::fromLatin1 (format);
        supportedFormatFilters.append (extension);
      }
      userPicturesModel->setNameFilters (supportedFormatFilters);
      userPicturesModel->setNameFilterDisables (false);

      auto userPicturesSubmodel = new SubindexModel (this);
      userPicturesSubmodel->setSourceModel (userPicturesModelWithURL);
      userPicturesSubmodel->setSourceRoot (userPicturesModelWithURL->mapFromSource (userPicturesIndex).parent());
      imagesModel->addModel (userPicturesSubmodel);
    }
  }

  ui->imageTree->setModel (imagesModel);

  auto defaultIndex = imagesModel->match (imagesModel->index (0, 0), Qt::DisplayRole, defaultImage, 1,
                                          Qt::MatchRecursive);
  if (!defaultIndex.isEmpty ())
  {
    ui->imageTree->scrollTo (defaultIndex[0]);
    ui->imageTree->setCurrentIndex (defaultIndex[0]);
  }

  // Hide all columns but the first
  for (int c = 1, num_cols = imagesModel->columnCount (); c < num_cols; c++)
  {
    ui->imageTree->hideColumn (c);
  }
}

ImageChooserWidget::~ImageChooserWidget()
{

}

void ImageChooserWidget::treeItemClicked (const QModelIndex& index)
{
  auto item_data = ui->imageTree->model ()->data (index, FullUrlRole);
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
  imageLocation->setUrl (url);
}
