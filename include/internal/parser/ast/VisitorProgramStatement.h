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
 * Visitor interface for AST program statement
 */
#ifndef S1_PARSER_AST_VISITORPROGRAMSTATEMENT_H_
#define S1_PARSER_AST_VISITORPROGRAMSTATEMENT_H_

namespace s1
{
  namespace parser
  {
    namespace ast
    {
      struct ProgramStatementFunctionDecl;
      struct ProgramStatementTypedef;
      struct ProgramStatementVarsDecl;

      /// Visitor interface for AST program statement
      struct VisitorProgramStatement
      {
        virtual ~VisitorProgramStatement() {}

        virtual void operator() (const ProgramStatementFunctionDecl& statement) = 0;
        virtual void operator() (const ProgramStatementTypedef& statement) = 0;
        virtual void operator() (const ProgramStatementVarsDecl& statement) = 0;
      };
    } // namespace ast
  } // namespace parser
} // namespace s1

#endif // S1_PARSER_AST_VISITORPROGRAMSTATEMENT_H_
