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
 * Item model implementation for browsing for some 'asset' (eg images, shader
 * programs), from built-in as well as filesystem sources.
 */
#ifndef ASSETBROWSINGMODEL_H
#define ASSETBROWSINGMODEL_H

#include "models/CombiningItemModel.h"

/**
 * Item model implementation for browsing for some 'asset' (eg images, shader
 * programs), from built-in as well as filesystem sources.
 */
class AssetBrowsingModel : public CombiningItemModel
{
  Q_OBJECT
public:
  AssetBrowsingModel (QObject* parent = nullptr);

  /// Set path to 'builtin' resources.
  void setBuiltinsPath  (const QString& path, const QString& label);
  /// Set name filters for filesystem sources
  void setFileSystemNameFilters (const QStringList& nameFilters);
  /// Add a filesystem path
  void addFileSystemPath (const QString& path);
protected:
  QStringList nameFilters;
};

#endif // ASSETBROWSINGMODEL_H
