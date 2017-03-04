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
 * Item model providing a simple way to add custom data to items.
 */
#ifndef CUSTOMDATAMODEL_H
#define CUSTOMDATAMODEL_H

#include <QIdentityProxyModel>

template<typename CustomFunction>
class CustomDataModel : public QIdentityProxyModel
{
public:
  CustomDataModel (CustomFunction&& function, QObject* parent = nullptr)
    : QIdentityProxyModel (parent), function (std::move (function))
  {}

  QVariant data (const QModelIndex &index, int role) const override
  {
    QVariant customData = function (mapToSource (index), role);
    if (customData.isValid()) return customData;

    return QIdentityProxyModel::data (index, role);
  }
protected:
  CustomFunction function;
};

template<typename CustomFunction>
CustomDataModel<CustomFunction>* make_CustomDataModel (CustomFunction&& function,
                                                       QAbstractItemModel* sourceModel,
                                                       QObject* parent = nullptr)
{
  auto model = new CustomDataModel<CustomFunction> (std::move (function), parent);
  model->setSourceModel (sourceModel);
  return model;
}

#endif // CUSTOMDATAMODEL_H
