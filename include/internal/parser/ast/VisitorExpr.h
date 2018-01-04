/*
    Shader1
    Copyright (c) 2018 Frank Richter


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
 * Visitor interface for AST expressions
 */
#ifndef S1_PARSER_AST_VISITOREXPR_H_
#define S1_PARSER_AST_VISITOREXPR_H_

namespace s1
{
  namespace parser
  {
    namespace ast
    {
      struct ExprArrayElement;
      struct ExprAttribute;
      struct ExprBinary;
      struct ExprFunctionCall;
      struct ExprTernary;
      struct ExprUnary;
      struct ExprValue;

      /// Visitor interface for AST expressionsn
      struct VisitorExpr
      {
        virtual ~VisitorExpr() {}

        virtual void operator() (const ExprArrayElement& expr) = 0;
        virtual void operator() (const ExprAttribute& expr) = 0;
        virtual void operator() (const ExprBinary& expr) = 0;
        virtual void operator() (const ExprFunctionCall& expr) = 0;
        virtual void operator() (const ExprTernary& expr) = 0;
        virtual void operator() (const ExprUnary& expr) = 0;
        virtual void operator() (const ExprValue& expr) = 0;
      };
    } // namespace ast
  } // namespace parser
} // namespace s1

#endif // S1_PARSER_AST_VISITOREXPR_H_
