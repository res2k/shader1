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

#include "parser/Exception.h"
#include "parser/Parser.h"

#include "parser/ast/Block.h"
#include "parser/ast/Type.h"

class TestParser : public s1::Parser
{
public:
  TestParser (s1::Lexer& inputLexer, s1::parser::SemanticsHandler& semanticsHandler,
              s1::diagnostics::Handler& diagnosticsHandler)
    : Parser (inputLexer, semanticsHandler, diagnosticsHandler) {}

  using s1::Parser::AstParseExpression;
  s1::parser::ast::TypePtr AstParseType ()
  {
    auto astType = s1::Parser::AstParseType ();
    if (astType.has_error ())
      throw s1::parser::Exception (astType.error ().error, astType.error ().token);
    return std::move (astType.value());
  }

  using s1::Parser::Expression;
  using s1::Parser::ParseProgram;

  void ParseBlock (Block block)
  {
    auto astBlock = AstParseBlock ();
    s1::Parser::ParseBlock (block, *astBlock);
  }

  Parser::Expression ParseExpression (const Scope& scope)
  {
    auto astExpr = AstParseExpression ();
    return s1::Parser::ParseExpression (scope, *astExpr);
  }
};

#endif // PARSER_T_TESTPARSER_H_
