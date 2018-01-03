/*
    Shader1
    Copyright (c) 2010-2018 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#ifndef PARSER_T_TESTASTBUILDER_H_
#define PARSER_T_TESTASTBUILDER_H_

#include "parser/AstBuilder.h"
#include "parser/Exception.h"

#include "parser/ast/Block.h"
#include "parser/ast/Program.h"
#include "parser/ast/Type.h"

class TestAstBuilder : public s1::parser::AstBuilder
{
public:
  TestAstBuilder (s1::Lexer& inputLexer, s1::diagnostics::Handler& diagnosticsHandler)
    : AstBuilder (inputLexer, diagnosticsHandler) {}

  using AstBuilder::ParseBlock;
  using AstBuilder::ParseExpression;
  using AstBuilder::ParseProgram;

  s1::parser::ast::TypePtr ParseType ()
  {
    auto astType = AstBuilder::ParseType ();
    if (astType.has_error ())
      throw s1::parser::Exception (astType.error ().error, astType.error ().token);
    return std::move (astType.value());
  }
};

#endif // PARSER_T_TESTASTBUILDER_H_
