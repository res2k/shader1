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
 * AST type
 */
#ifndef S1_PARSER_AST_TYPE_H_
#define S1_PARSER_AST_TYPE_H_

#include "Identifier.h"
#include "Node.h"

#include "base/uc/String.h"
#include "lexer/Token.h"

#include <boost/container/static_vector.hpp>
#include <boost/variant.hpp>

namespace s1
{
  namespace parser
  {
    namespace ast
    {
      /// AST type
      struct Type : public Node
      {
        typedef boost::container::static_vector<lexer::Token, 2> WellKnownType;
        struct ArrayType
        {
          TypePtr containedType;
        };

        boost::variant<WellKnownType, Identifier, ArrayType> value;

        template<typename U>
        Type (U&& arg) : value (std::forward<U> (arg)) {}
      };
    } // namespace ast
  } // namespace parser
} // namespace s1

#endif // S1_PARSER_AST_TYPE_H_
