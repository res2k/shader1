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
 * Helper: Define operator<< for s1::semantics::BaseType
 */

#ifndef PRINT_SEMANTICS_BASETYPE_H_
#define PRINT_SEMANTICS_BASETYPE_H_

#include "semantics/forwarddecl.h"

#include <iostream>

namespace s1
{
  namespace semantics
  {
    static std::ostream& operator<< (std::ostream& stream, const BaseType& bt)
    {
      switch (bt)
      {
      #define PRINT_TOKEN(X)  case X: stream << #X; return stream;

      PRINT_TOKEN (BaseType::Invalid)
      PRINT_TOKEN (BaseType::Void)
      PRINT_TOKEN (BaseType::Bool)
      PRINT_TOKEN (BaseType::Int)
      PRINT_TOKEN (BaseType::UInt)
      PRINT_TOKEN (BaseType::Float)

      #undef PRINT_TOKEN
      }
      stream << static_cast<int> (bt);
      return stream;
    }
  } // namespace lexer
} // namespace s1

#endif // PRINT_SEMANTICS_BASETYPE_H_
