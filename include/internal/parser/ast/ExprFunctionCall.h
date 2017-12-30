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
 * AST function call expression
 */
#ifndef S1_PARSER_AST_EXPRFUNCTIONCALL_H_
#define S1_PARSER_AST_EXPRFUNCTIONCALL_H_

#include "forwarddecl.h"
#include "Node.h"

#include <vector>

#include <boost/variant.hpp>

namespace s1
{
  namespace parser
  {
    namespace ast
    {
      /// AST function call expression
      struct ExprFunctionCall : public Node
      {
        boost::variant<Identifier, TypePtr> identifierOrType;
        std::vector<ExprPtr> args;

        template<typename U, typename V>
        ExprFunctionCall (U&& id, V&& args)
          : identifierOrType (std::forward<U> (id)), args (std::forward<V> (args)) {}
      };
    } // namespace ast
  } // namespace parser
} // namespace s1

#endif // S1_PARSER_AST_EXPRFUNCTIONCALL_H_
