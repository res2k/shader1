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
 * Helper: Define operator<< for s1::parser::Error
 */

#ifndef PRINT_PARSER_ERROR_H_
#define PRINT_PARSER_ERROR_H_

#include "parser/Diagnostics.h"

#include <iostream>

namespace s1
{
  namespace parser
  {
    static std::ostream& operator<< (std::ostream& stream, const Error& tt)
    {
      int save_flags = stream.flags();
      stream.flags ((save_flags & ~std::ios_base::basefield) | std::ios_base::hex);
      stream << static_cast<unsigned int> (tt);
      stream.flags (save_flags);
      return stream;
    }
  } // namespace lexer
} // namespace s1

#endif // PRINT_PARSER_ERROR_H_
