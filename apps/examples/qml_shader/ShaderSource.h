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
 * Object providing shader source to QML
 */
#ifndef SHADERSOURCE_H
#define SHADERSOURCE_H

#include <QObject>
#include <QUrl>

#include "s1/fwd.h"
#include "s1/Ptr.h"

class ShaderSource : public QObject
{
  Q_OBJECT
public:
  ShaderSource (QObject* parent = nullptr);

  /// Shader program source URL
  const QUrl& url();
  void setUrl (const QUrl& url);
  Q_PROPERTY(QUrl url READ url NOTIFY urlChanged);

  //@{
  /// Vertex program access
  QString vertexProgram();
  Q_PROPERTY(QString vertexProgram READ vertexProgram NOTIFY vertexProgramChanged);
  //@}
  //@{
  /// Fragment program access
  QString fragmentProgram();
  Q_PROPERTY(QString fragmentProgram READ fragmentProgram NOTIFY fragmentProgramChanged);
  //@}
protected:
  QUrl sourceUrl;
  /// S1 library object reference
  s1::Ptr<s1::Library> lib;
  /// S1 backend reference
  s1::Ptr<s1::Backend> compilerBackend;
  /// S1 backend options reference
  s1::Ptr<s1::BackendOptions> backendOptions;

  /// Current vertex program.
  QString currentVP;
  /// Current fragment program.
  QString currentFP;

  void initLibrary ();
  void compileShader ();
signals:
  void urlChanged ();
  void vertexProgramChanged ();
  void fragmentProgramChanged ();
};

#endif // SHADERSOURCE_H
