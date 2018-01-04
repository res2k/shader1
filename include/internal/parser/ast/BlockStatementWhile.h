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
 * AST 'while' block statement
 */
#ifndef S1_PARSER_AST_BLOCKSTATEMENTWHILE_H_
#define S1_PARSER_AST_BLOCKSTATEMENTWHILE_H_

#include "BlockStatement.h"
#include "VisitorBlockStatement.h"

namespace s1
{
  namespace parser
  {
    namespace ast
    {
      /// AST 'while' block statement
      struct BlockStatementWhile : public BlockStatement
      {
        ExprPtr condition;
        BlockPtr bodyBlock;

        BlockStatementWhile (ExprPtr&& condition, BlockPtr&& bodyBlock)
          : condition (std::move (condition)), bodyBlock (std::move (bodyBlock)) {}

        void Visit (VisitorBlockStatement& visitor) const override { visitor (*this); }
      };
    } // namespace ast
  } // namespace parser
} // namespace s1

#endif // S1_PARSER_AST_BLOCKSTATEMENTWHILE_H_
