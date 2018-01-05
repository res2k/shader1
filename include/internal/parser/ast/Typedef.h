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
 * AST type definition
 */
#ifndef S1_PARSER_AST_TYPEDEF_H_
#define S1_PARSER_AST_TYPEDEF_H_

#include "forwarddecl.h"
#include "Identifier.h"

namespace s1
{
  namespace parser
  {
    namespace ast
    {
      /// AST type definition
      // Note: intended for use in composition
      struct Typedef
      {
        TypePtr type;
        Identifier alias;

        Typedef (TypePtr&& type, Identifier&& alias)
          : type (std::move (type)), alias (std::move (alias)) {}
      };
    } // namespace ast
  } // namespace parser
} // namespace s1

#endif // S1_PARSER_AST_TYPEDEF_H_
