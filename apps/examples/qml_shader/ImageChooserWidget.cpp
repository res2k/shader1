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

#include "AssetBrowsingModel.h"
#include "ImageLocation.h"

#include <QImageReader>
#include <QStandardPaths>
#include <QStringBuilder>

ImageChooserWidget::ImageChooserWidget (ImageLocation* imageLocation, QWidget* parent)
  : AssetBrowsingWidget (parent), imageLocation (imageLocation)
{
  // Set up contents of Image selection widget
  assetsModel->setBuiltinsPath (QStringLiteral (":/images/"), tr ("Builtin Images"));

  // Set up model to only show images supported by Qt
  {
    const auto supportedFormats = QImageReader::supportedImageFormats ();
    QStringList supportedFormatFilters;
    for (const auto& format : supportedFormats)
    {
      QString extension = QStringLiteral ("*.") % QString::fromLatin1 (format);
      supportedFormatFilters.append (extension);
    }
    assetsModel->setFileSystemNameFilters (supportedFormatFilters);
  }

  {
    const auto userPicturesPaths = QStandardPaths::standardLocations (QStandardPaths::PicturesLocation);
    for (const auto& picturePath : userPicturesPaths)
    {
      assetsModel->addFileSystemPath (picturePath);
    }
  }

  selectFile (imageLocation->url ().fileName ());

  connect (this, &AssetBrowsingWidget::urlSelected,
           this, [=](const QUrl& url){ imageLocation->setUrl (url); });
}

ImageChooserWidget::~ImageChooserWidget()
{
}
