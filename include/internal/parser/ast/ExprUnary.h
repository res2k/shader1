/*
    Shader1
    Copyright (c) 2017-2018 Frank Richter


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
 * AST unary expression
 */
#ifndef S1_PARSER_AST_EXPRUNARY_H_
#define S1_PARSER_AST_EXPRUNARY_H_

#include "Expr.h"
#include "VisitorExpr.h"

namespace s1
{
  namespace parser
  {
    namespace ast
    {
      /// AST unary expression
      struct ExprUnary : public Expr
      {
        lexer::Token op;
        ExprPtr right;

        ExprUnary (lexer::Token op, ExprPtr&& right) : op (op), right (std::move (right)) {}

        void Visit (VisitorExpr& visitor) const override { visitor (*this); }
      };
    } // namespace ast
  } // namespace parser
} // namespace s1

#endif // S1_PARSER_AST_EXPRUNARY_H_
