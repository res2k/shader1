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
 * AST program statement for function declaration
 */
#ifndef S1_PARSER_AST_PROGRAMSTATEMENTFUNCTIONDECL_H_
#define S1_PARSER_AST_PROGRAMSTATEMENTFUNCTIONDECL_H_

#include "ProgramStatement.h"
#include "VisitorProgramStatement.h"

namespace s1
{
  namespace parser
  {
    namespace ast
    {
      /// AST program statement for function declaration
      struct ProgramStatementFunctionDecl : public ProgramStatement
      {
        FunctionDeclPtr functionDecl;

        ProgramStatementFunctionDecl (FunctionDeclPtr&& functionDecl) : functionDecl (std::move (functionDecl)) {}

        void Visit (VisitorProgramStatement& visitor) const override { visitor (*this); }
      };
    } // namespace ast
  } // namespace parser
} // namespace s1

#endif // S1_PARSER_AST_PROGRAMSTATEMENTFUNCTIONDECL_H_
