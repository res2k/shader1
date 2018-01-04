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
 * AST 'if' block statement
 */
#ifndef S1_PARSER_AST_BLOCKSTATEMENTIF_H_
#define S1_PARSER_AST_BLOCKSTATEMENTIF_H_

#include "Node.h"
#include "VisitorBlockStatement.h"

namespace s1
{
  namespace parser
  {
    namespace ast
    {
      /// AST 'if' block statement
      struct BlockStatementIf : public BlockStatement
      {
        ExprPtr condition;
        BlockPtr ifBlock;
        BlockPtr elseBlock; // may be none

        BlockStatementIf (ExprPtr&& condition, BlockPtr&& ifBlock, BlockPtr&& elseBlock)
          : condition (std::move (condition)), ifBlock (std::move (ifBlock)), elseBlock (std::move (elseBlock)) {}

        void Visit (VisitorBlockStatement& visitor) const override { visitor (*this); }
      };
    } // namespace ast
  } // namespace parser
} // namespace s1

#endif // S1_PARSER_AST_BLOCKSTATEMENTIF_H_
