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

#include "MaterialIcons.h"

#include <QDir>
#include <QFontDatabase>

static inline void initMaterialIconsRes() { Q_INIT_RESOURCE(MaterialIcons); }
static inline void cleanupMaterialIconsRes() { Q_CLEANUP_RESOURCE(MaterialIcons); }

namespace material_icons
{
  namespace
  {
    struct ResourceInitializer
    {
      ResourceInitializer() { initMaterialIconsRes(); }
      ~ResourceInitializer() { cleanupMaterialIconsRes(); }
    };
    ResourceInitializer resInit;
  } // anonymous namespace

  void RegisterFont ()
  {
    QFontDatabase::addApplicationFont (QStringLiteral(":/material_icons/MaterialIcons-Regular.ttf"));
  }
} // namespace material_icons
