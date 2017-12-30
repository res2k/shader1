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
 * AST expression
 */
#ifndef S1_PARSER_AST_EXPR_H_
#define S1_PARSER_AST_EXPR_H_

#include "ExprArrayElement.h"
#include "ExprAttribute.h"
#include "ExprBinary.h"
#include "ExprFunctionCall.h"
#include "ExprTernary.h"
#include "ExprUnary.h"
#include "ExprValue.h"
#include "Node.h"

#include <boost/variant.hpp>

namespace s1
{
  namespace parser
  {
    namespace ast
    {
      /// AST expression
      struct Expr : public Node
      {
        boost::variant<ExprValuePtr,
                       ExprBinaryPtr,
                       ExprUnaryPtr,
                       ExprTernaryPtr,
                       ExprFunctionCallPtr,
                       ExprAttributePtr,
                       ExprArrayElementPtr> value;

        template<typename U>
        Expr (U&& arg) : value (std::forward<U> (arg)) {}
      };
    } // namespace ast
  } // namespace parser
} // namespace s1

#endif // S1_PARSER_AST_EXPR_H_
