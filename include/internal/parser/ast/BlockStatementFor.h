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
 * AST 'for' block statement
 */
#ifndef S1_PARSER_AST_BLOCKSTATEMENTFOR_H_
#define S1_PARSER_AST_BLOCKSTATEMENTFOR_H_

#include "Expr.h"
#include "Node.h"

namespace s1
{
  namespace parser
  {
    namespace ast
    {
      /// AST 'for' block statement
      struct BlockStatementFor : public Node
      {
        ExprPtr initExpr; // may be none
        ExprPtr condition; // may be none
        ExprPtr footExpr; // may be none
        BlockPtr bodyBlock;

        BlockStatementFor (ExprPtr&& initExpr, ExprPtr&& condition, ExprPtr&& footExpr, BlockPtr&& bodyBlock)
          : initExpr (std::move (initExpr)), condition (std::move (condition)), footExpr (std::move (footExpr)),
            bodyBlock (std::move (bodyBlock)) {}
      };
    } // namespace ast
  } // namespace parser
} // namespace s1

#endif // S1_PARSER_AST_BLOCKSTATEMENTFOR_H_
