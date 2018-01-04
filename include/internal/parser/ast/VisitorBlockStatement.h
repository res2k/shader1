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
 * Visitor interface for AST block statement
 */
#ifndef S1_PARSER_AST_VISITORBLOCKSTATEMENT_H_
#define S1_PARSER_AST_VISITORBLOCKSTATEMENT_H_

namespace s1
{
  namespace parser
  {
    namespace ast
    {
      struct BlockStatementExpr;
      struct BlockStatementFor;
      struct BlockStatementIf;
      struct BlockStatementNestedBlock;
      struct BlockStatementReturn;
      struct BlockStatementTypedef;
      struct BlockStatementVarsDecl;
      struct BlockStatementWhile;

      /// Visitor interface for AST block statement
      struct VisitorBlockStatement
      {
        virtual ~VisitorBlockStatement() {}

        virtual void operator() (const BlockStatementExpr& statement) = 0;
        virtual void operator() (const BlockStatementFor& statement) = 0;
        virtual void operator() (const BlockStatementIf& statement) = 0;
        virtual void operator() (const BlockStatementNestedBlock& statement) = 0;
        virtual void operator() (const BlockStatementReturn& statement) = 0;
        virtual void operator() (const BlockStatementTypedef& statement) = 0;
        virtual void operator() (const BlockStatementVarsDecl& statement) = 0;
        virtual void operator() (const BlockStatementWhile& statement) = 0;
      };
    } // namespace ast
  } // namespace parser
} // namespace s1

#endif // S1_PARSER_AST_VISITORBLOCKSTATEMENT_H_
