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
 * AST block
 */
#ifndef S1_PARSER_AST_BLOCK_H_
#define S1_PARSER_AST_BLOCK_H_

#include "BlockStatement.h"
#include "Node.h"

#include <boost/container/deque.hpp>

namespace s1
{
  namespace parser
  {
    namespace ast
    {
      /// AST block
      struct Block : public Node
      {
        typedef boost::container::deque<BlockStatementPtr> StatementsContainer;
        StatementsContainer statements;

        Block (StatementsContainer&& statements) : statements (std::move (statements)) {}
      };
    } // namespace ast
  } // namespace parser
} // namespace s1

#endif // S1_PARSER_AST_BLOCK_H_
