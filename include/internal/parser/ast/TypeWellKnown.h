/*
    Shader1
    Copyright (c) 2017-2018 Frank Richter


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
 * AST type for well-known types
 */
#ifndef S1_PARSER_AST_TYPEWELLKNOWN_H_
#define S1_PARSER_AST_TYPEWELLKNOWN_H_

#include "Type.h"
#include "VisitorType.h"

#include <boost/container/static_vector.hpp>

namespace s1
{
  namespace parser
  {
    namespace ast
    {
      /// AST type for well-known types
      struct TypeWellKnown : public Type
      {
        typedef boost::container::static_vector<lexer::Token, 2> Tokens;
        Tokens tokens;

        TypeWellKnown (Tokens&& tokens) : tokens (std::move (tokens)) {}

        void Visit (VisitorType& visitor) const override { visitor (*this); }
      };
    } // namespace ast
  } // namespace parser
} // namespace s1

#endif // S1_PARSER_AST_TYPEWELLKNOWN_H_
