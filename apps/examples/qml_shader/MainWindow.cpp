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

#include "ImageChooserWidget.h"
#include "ImageLocation.h"
#include "ShaderChooserWidget.h"
#include "ShaderSource.h"

#include <QDockWidget>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickWidget>
#include <QToolBar>

MainWindow::MainWindow (QWidget* parent) : QMainWindow (parent)
{
  resize (800, 600);

  shaderSrc = new ShaderSource (this);
  imageLocation = new ImageLocation (this);

  auto qml_widget = new QQuickWidget (this);
  qml_widget->engine()->rootContext()->setContextProperty (QStringLiteral ("shaderSource"), shaderSrc);
  qml_widget->engine()->rootContext()->setContextProperty (QStringLiteral ("imageLocation"), imageLocation);
  qml_widget->setResizeMode (QQuickWidget::SizeRootObjectToView);
  qml_widget->setSource (QStringLiteral ("qrc:/ImageView.qml"));
  setCentralWidget (qml_widget);

  auto shaderDock = new QDockWidget (tr ("Shader Program"), this);
  auto shaderWidget = new ShaderChooserWidget (shaderSrc, shaderDock);
  shaderDock->setWidget (shaderWidget);
  shaderDock->setFeatures (QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetClosable);
  addDockWidget (Qt::LeftDockWidgetArea, shaderDock);

  auto optionsDock = new QDockWidget (tr ("Image"), this);
  auto optionsWidget = new ImageChooserWidget (imageLocation, optionsDock);
  optionsDock->setWidget (optionsWidget);
  optionsDock->setFeatures (QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetClosable);
  addDockWidget (Qt::LeftDockWidgetArea, optionsDock);

  QFont materialIconsFont (QStringLiteral ("Material Icons"), 18);

  auto toolbar = new QToolBar (this);
  toolbar->setFont (materialIconsFont);
  toolbar->setFloatable (false);
  toolbar->setMovable (false);
  toolbar->setOrientation (Qt::Vertical);
  toolbar->setToolButtonStyle (Qt::ToolButtonTextOnly);
  addToolBar (Qt::LeftToolBarArea, toolbar);

  auto shaderBrowserAction = shaderDock->toggleViewAction ();
  shaderBrowserAction->setToolTip (shaderDock->windowTitle ());
  shaderBrowserAction->setText (QStringLiteral ("gradient"));
  toolbar->addAction (shaderBrowserAction);

  auto imageBrowserAction = optionsDock->toggleViewAction ();
  imageBrowserAction->setToolTip (optionsDock->windowTitle ());
  imageBrowserAction->setText (QStringLiteral ("image"));
  toolbar->addAction (imageBrowserAction);
}
