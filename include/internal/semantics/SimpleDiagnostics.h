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
 * Simple diagnostics handler for semantic issues
 */
#ifndef SEMANTICS_SIMPLEDIAGNOSTICS_H_
#define SEMANTICS_SIMPLEDIAGNOSTICS_H_

#include "forwarddecl.h"

#include "semantics/Diagnostics_fwd.h"

namespace s1
{
  namespace semantics
  {
    struct SimpleDiagnostics
    {
      virtual ~SimpleDiagnostics() {}

      /// Report a semantic error
      virtual void Error (Error code) = 0;
    };
  } // namespace semantics
} // namespace s1

#endif // SEMANTICS_SIMPLEDIAGNOSTICS_H_
