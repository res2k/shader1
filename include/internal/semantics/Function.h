/*
    Shader1
    Copyright (c) 2010-2018 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#ifndef SEMANTICS_FUNCTION_H_
#define SEMANTICS_FUNCTION_H_

#include "forwarddecl.h"

#include "Handler.h"

namespace s1
{
  namespace semantics
  {
    /// Function
    struct Function
    {
      virtual ~Function() {}

      /// Get function block to add commands to.
      virtual Handler::BlockPtr GetBody() = 0;
      /// Finish function. Call after body was filled.
      virtual void Finish() = 0;
    };
  } // namespace semantics
} // namespace s1

#endif // SEMANTICS_FUNCTION_H_
