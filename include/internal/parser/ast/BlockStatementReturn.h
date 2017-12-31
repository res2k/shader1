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
 * AST 'return' block statement
 */
#ifndef S1_PARSER_AST_BLOCKSTATEMENTRETURN_H_
#define S1_PARSER_AST_BLOCKSTATEMENTRETURN_H_

#include "Node.h"

namespace s1
{
  namespace parser
  {
    namespace ast
    {
      /// AST 'return' block statement
      struct BlockStatementReturn : public Node
      {
        ExprPtr expr; // may be none

        template<typename U>
        BlockStatementReturn (U&& arg) : expr (std::forward<U> (arg)) {}
      };
    } // namespace ast
  } // namespace parser
} // namespace s1

#endif // S1_PARSER_AST_BLOCKSTATEMENTRETURN_H_
