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

#include "ShaderSource.h"

#include <QDebug>
#include <QFile>
#include <QStringBuilder>

#include "s1/Backend.h"
#include "s1/BackendOptions.h"
#include "s1/CompiledProgram.h"
#include "s1/Library.h"
#include "s1/Options.h"

static QString ResultCodeString (s1::ResultCode errorCode)
{
  const char* s = s1::GetResultCodeStr (errorCode);
  if (s) return QString::fromUtf8 (s);

  return QString::number (errorCode, 16);
}

// Get result string for last library error
static QString LastErrorString (s1::Library* lib)
{
  QString s (ResultCodeString (lib->GetLastError ()));
  const char* info = lib->GetLastErrorInfo ();
  if (info) s.append (QStringLiteral (": ") % QString::fromUtf8 (info));
  return s;
}

ShaderSource::ShaderSource (QObject* parent)
  : QObject (parent), sourceUrl (QUrl::fromLocalFile (QStringLiteral (":/shaders/grayscale.s1")))
{
  initLibrary ();
  compileShader ();
}

const QUrl& ShaderSource::url()
{
  return sourceUrl;
}

void ShaderSource::setUrl (const QUrl& url)
{
  if (url != sourceUrl)
  {
    sourceUrl = url;
    emit urlChanged ();

    compileShader();
  }
}

QString ShaderSource::vertexProgram()
{
  return currentVP;
}

QString ShaderSource::fragmentProgram()
{
  return currentFP;
}

void ShaderSource::initLibrary ()
{
  s1::ResultCode libErr (s1::Library::Create (lib));
  if (!S1_SUCCESSFUL(libErr))
  {
    qWarning() << "Failed to initialize library:" << ResultCodeString (libErr);
    return;
  }

  compilerBackend = lib->CreateBackend (L"glsl");
  if (!compilerBackend)
  {
    qWarning() << "Failed to create GLSL backend:"
      << LastErrorString (lib);
    return;
  }

  backendOptions = compilerBackend->CreateBackendOptions ();
  if (!backendOptions)
  {
    qWarning() << "Error creating backend options object:" << LastErrorString (lib);
  }
  else
  {
    if (!backendOptions->SetFromStr (s1::u8 ("avoid-qualifier-in")))
    {
      qWarning() << "Error setting 'avoid-qualifier-in':"
        << LastErrorString (lib);
    }
  }
}

void ShaderSource::compileShader ()
{
  if (!compilerBackend) return;

  if (!sourceUrl.isLocalFile())
  {
    qWarning() << "Unsupported URL:" << sourceUrl;
    return;
  }

  QByteArray shaderData;
  {
    auto shaderFilePath = sourceUrl.toLocalFile();
    QFile shaderFile (shaderFilePath);
    shaderFile.open (QFile::ReadOnly);
    if (shaderFile.error () != QFile::NoError)
    {
      qWarning () << "error opening" << shaderFilePath << ":"
        << shaderFile.errorString ();
      return;
    }
    shaderData = shaderFile.readAll ();
  }

  s1::Options::Pointer compilerOpts = lib->CreateOptions ();
  compilerOpts->SetOptLevel (3);

  s1::Program::Pointer compilerProg = lib->CreateProgramFromString (shaderData.constData (), shaderData.size ());
  if (!compilerProg)
  {
    qWarning() << "Error creating program:" << LastErrorString (lib);
    return;
  }
  compilerProg->SetOptions (compilerOpts);

  QString newVP, newFP;
  s1::CompiledProgram::Pointer compiledVP (
    compilerBackend->GenerateProgram (compilerProg, S1_TARGET_VP, backendOptions));
  if (!compiledVP)
  {
    qWarning() << "Failed to get vertex program:" << LastErrorString (lib);
  }
  else
  {
    newVP = QString::fromUtf8 (compiledVP->GetString ());
  }
  s1::CompiledProgram::Pointer compiledFP (
    compilerBackend->GenerateProgram (compilerProg, S1_TARGET_FP, backendOptions));
  if (!compiledFP)
  {
    qWarning() << "Failed to get fragment program:" << LastErrorString (lib);
  }
  else
  {
    newFP = QString::fromUtf8 (compiledFP->GetString ());
  }

  if (!newVP.isEmpty() && !newFP.isEmpty())
  {
    currentVP = newVP;
    currentFP = newFP;

    emit vertexProgramChanged ();
    emit fragmentProgramChanged ();
  }
}
