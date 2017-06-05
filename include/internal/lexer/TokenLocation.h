/*
    Shader1
    Copyright (c) 2017 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

/**
 * Lexer token location
 */
#ifndef __LEXER_TOKENLOCATION_H__
#define __LEXER_TOKENLOCATION_H__

namespace s1
{
  namespace lexer
  {
    /// Token location in the source
    struct TokenLocation
    {
      /// Line number (0-based)
      unsigned int line = 0;
      /// Columns number (0-based), in code points
      unsigned int column = 0;
    };

  } // namespace lexer
} // namespace s1

#endif // __LEXER_TOKENLOCATION_H__
