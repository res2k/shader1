/*
    Shader1
    Copyright (c) 2018 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

/**\file
 * Default implementation for semantics::SimpleDiagnostics
 */
#ifndef SIMPLESEMANTICSDIAGNOSTICSIMPL_H_
#define SIMPLESEMANTICSDIAGNOSTICSIMPL_H_

#include "semantics/SimpleDiagnostics.h"

class SimpleSemanticsDiagnosticsImpl : public s1::semantics::SimpleDiagnostics
{
  s1::diagnostics::Handler& diagnosticsHandler;
public:
  SimpleSemanticsDiagnosticsImpl (s1::diagnostics::Handler& diagnosticsHandler) : diagnosticsHandler (diagnosticsHandler) {}

  void Error (s1::semantics::Error code) override
  {
    diagnosticsHandler.ParseError (code);
  }
};

#endif // SIMPLESEMANTICSDIAGNOSTICSIMPL_H_


