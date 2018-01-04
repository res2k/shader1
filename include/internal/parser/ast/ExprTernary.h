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
 * AST ternary expression
 */
#ifndef S1_PARSER_AST_EXPRTERNARY_H_
#define S1_PARSER_AST_EXPRTERNARY_H_

#include "Expr.h"
#include "VisitorExpr.h"

namespace s1
{
  namespace parser
  {
    namespace ast
    {
      /// AST ternary expression
      struct ExprTernary : public Expr
      {
        ExprPtr cond;
        ExprPtr trueExpr;
        ExprPtr falseExpr;

        ExprTernary (ExprPtr&& cond, ExprPtr&& trueExpr, ExprPtr&& falseExpr)
          : cond (std::move (cond)), trueExpr (std::move (trueExpr)), falseExpr (std::move (falseExpr)) {}

        void Visit (VisitorExpr& visitor) const override { visitor (*this); }
      };
    } // namespace ast
  } // namespace parser
} // namespace s1

#endif // S1_PARSER_AST_EXPRTERNARY_H_
