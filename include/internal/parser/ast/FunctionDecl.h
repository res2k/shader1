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
 * AST function declaration
 */
#ifndef S1_PARSER_AST_FUNCTIONDECL_H_
#define S1_PARSER_AST_FUNCTIONDECL_H_

#include "Identifier.h"
#include "Node.h"

#include <boost/container/deque.hpp>
#include <boost/variant.hpp>

namespace s1
{
  namespace parser
  {
    namespace ast
    {
      /// AST function declaration
      struct FunctionDecl : public Node
      {
        struct Void {};
        typedef boost::variant<Void, TypePtr> VoidOrType;
        struct Param
        {
          boost::container::deque<lexer::Token> qualifiers;
          TypePtr type;
          Identifier identifier;
          ExprPtr defaultValue; //may be none
        };
        typedef boost::container::deque<Param> ParamsContainer;

        VoidOrType resultType;
        Identifier identifier;
        ParamsContainer params;
        BlockPtr body;

        template<typename T>
        FunctionDecl (T&& resultType, Identifier&& identifier, ParamsContainer&& params, BlockPtr&& body)
          : resultType (std::forward<T> (resultType)),
            identifier (std::move (identifier)),
            params (std::move (params)),
            body (std::move (body))
        {}
      };
    } // namespace ast
  } // namespace parser
} // namespace s1

#endif // S1_PARSER_AST_FUNCTIONDECL_H_
