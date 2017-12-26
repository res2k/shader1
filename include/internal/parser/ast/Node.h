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
 * AST Node base class
 */
#ifndef S1_PARSER_AST_NODE_H_
#define S1_PARSER_AST_NODE_H_

#include "forwarddecl.h"

namespace s1
{
  namespace parser
  {
    namespace ast
    {
      /// AST Node base class
      struct Node
      {
        /// Start location of this particular tree node
        lexer::TokenLocation startLocation;
        /// End location of this particular tree node (inclusive!)
        lexer::TokenLocation endLocation;

        virtual ~Node() {}
      };
    } // namespace ast
  } // namespace parser
} // namespace s1

#endif // S1_PARSER_AST_NODE_H_
