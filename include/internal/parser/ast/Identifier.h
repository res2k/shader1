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

/**\file
 * AST helper class for tokens that are guaranteed to be identifiers
 */
#ifndef S1_PARSER_AST_IDENTIFIER_H_
#define S1_PARSER_AST_IDENTIFIER_H_

#include "forwarddecl.h"
#include "lexer/Token.h"

namespace s1
{
  namespace parser
  {
    namespace ast
    {
      /// AST helper class for tokens that are guaranteed to be identifiers
      struct Identifier
      {
        lexer::Token token;

        const uc::String& GetString() const& { return token.tokenString; }
        uc::String&& GetString() && { return std::move (token.tokenString); }
      };
    } // namespace ast
  } // namespace parser
} // namespace s1

#endif // S1_PARSER_AST_IDENTIFIER_H_
