/*
    Shader1
    Copyright (c) 2010-2017 Frank Richter


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
 * Lexer token definition
 */
#ifndef LEXER_TOKEN_H_
#define LEXER_TOKEN_H_

#include "TokenLocation.h"
#include "TokenType.h"

#include "base/uc/String.h"

namespace s1
{
  namespace lexer
  {
    /// Token object
    struct Token
    {
      /// Type/symbol/keyword ID of this token
      TokenType typeOrID = lexer::Invalid;
      /// Original string for this token
      uc::String tokenString;
      /// For vectors: vector dimension; For matrices: number of columns
      int dimension1 = 0;
      /// For matrices: number of rows
      int dimension2 = 0;

      /// Location of token in source
      TokenLocation location;

      Token () {}
      Token (TokenType type) : typeOrID (type) {}
      Token (TokenType type, const uc::String& tokenString) : typeOrID (type), tokenString (tokenString) {}
      Token (TokenType type, uc::Char32 tokenChar) : typeOrID (type), tokenString (tokenChar) {}
      Token (TokenType type, const char* tokenString) : typeOrID (type), tokenString (tokenString) {}
    };
  } // namespace lexer
} // namespace s1

#endif // LEXER_TOKEN_H_
