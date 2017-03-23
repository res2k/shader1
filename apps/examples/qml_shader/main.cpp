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

#include "MainWindow.h"

#include <QApplication>
#include <QScreen>

#if defined(Q_OS_WIN32)
#include <Windows.h>

#include <Uxtheme.h>
#include <vssym32.h>
#endif

int main (int argc, char* argv[])
{
  QApplication qapp (argc, argv);

#if defined(Q_OS_WIN32)
  LOGFONTW lfw;
  if (SUCCEEDED(GetThemeSysFont(NULL, TMT_MSGBOXFONT, &lfw)))
  {
    QFont font;
    font.setFamily(QString::fromWCharArray(lfw.lfFaceName));
    font.setPointSize((qAbs(lfw.lfHeight) * 72.0) / qapp.screens()[0]->logicalDotsPerInchY());
    qapp.setFont(font);
  }
#endif

  MainWindow* main_wnd = new MainWindow;
  main_wnd->show ();

  return qapp.exec();
}
