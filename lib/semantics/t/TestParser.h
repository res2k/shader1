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

#ifndef PARSER_T_TESTPARSER_H_
#define PARSER_T_TESTPARSER_H_

#include "../../parser/t/TestAstBuilder.h"

#include "parser/Parser.h"

#include "parser/ast/Block.h"
#include "parser/ast/Expr.h"
#include "parser/ast/Type.h"

class TestParser : public s1::Parser
{
public:
  TestParser (s1::Lexer& inputLexer, s1::semantics::Handler& semanticsHandler,
              s1::diagnostics::Handler& diagnosticsHandler)
    : Parser (inputLexer, semanticsHandler, diagnosticsHandler) {}

  using s1::Parser::Expression;
  using s1::Parser::ParseProgram;

  void ParseBlock (Block block)
  {
    auto astBlock = TestAstBuilder (inputLexer, diagnosticsHandler).ParseBlock ();
    s1::Parser::ParseBlock (block, *astBlock);
  }

  Parser::Expression ParseExpression (const Scope& scope)
  {
    auto astExpr = TestAstBuilder (inputLexer, diagnosticsHandler).ParseExpression ();
    return s1::Parser::ParseExpression (scope, astExpr.get());
  }

  Parser::Type ParseType (const Scope& scope)
  {
    auto astType = TestAstBuilder (inputLexer, diagnosticsHandler).ParseType ();
    return Parser::ParseType (astType.get(), scope);
  }
};

#endif // PARSER_T_TESTPARSER_H_
