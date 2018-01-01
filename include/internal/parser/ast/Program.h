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
 * AST program
 */
#ifndef S1_PARSER_AST_PROGRAM_H_
#define S1_PARSER_AST_PROGRAM_H_

#include "FunctionDecl.h"
#include "Node.h"
#include "Typedef.h"
#include "VarsDecl.h"

#include <boost/container/deque.hpp>
#include <boost/variant.hpp>

namespace s1
{
  namespace parser
  {
    namespace ast
    {
      /// AST program
      struct Program : public Node
      {
        typedef boost::variant<TypedefPtr, VarsDeclPtr, FunctionDeclPtr> Statement;
        typedef boost::container::deque<Statement> StatementsContainer;
        StatementsContainer statements;

        Program (StatementsContainer&& statements) : statements (std::move (statements)) {}
      };
    } // namespace ast
  } // namespace parser
} // namespace s1

#endif // S1_PARSER_AST_PROGRAM_H_
