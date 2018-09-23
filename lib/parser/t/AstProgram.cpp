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

#include <boost/test/unit_test.hpp>

#include "base/common.h"

#include "base/uc/SimpleBufferStreamSource.h"
#include "parser/ast/ExprBinary.h"
#include "parser/ast/ExprFunctionCall.h"
#include "parser/ast/FunctionDecl.h"
#include "parser/ast/BlockStatementExpr.h"
#include "parser/ast/BlockStatementReturn.h"
#include "parser/ast/BlockStatementVarsDecl.h"
#include "parser/ast/Program.h"
#include "parser/ast/ProgramStatementFunctionDecl.h"
#include "parser/ast/ProgramStatementVarsDecl.h"
#include "parser/Diagnostics.h"

#include "AstMacros.h"
#include "TestAstBuilder.h"

#include "../../base/t/print_uc_String.h"
#include "../../diagnostics/t/TestDiagnosticsHandler.h"

BOOST_AUTO_TEST_SUITE(AstProgram)

BOOST_AUTO_TEST_CASE(ProgramSimple)
{
  using namespace s1::parser;

  std::string inStr ("void main() {}");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  ast::ProgramPtr program;
  BOOST_CHECK_NO_THROW(program = astBuilder.ParseProgram ());
  BOOST_CHECK(errorHandler.parseErrors.empty());

  BOOST_CHECK_EQUAL(program->statements.size(), 1u);
  const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[0].get())->functionDecl;
  BOOST_CHECK(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
  BOOST_CHECK_EQUAL(functionDecl->identifier.GetString(), "main");
  BOOST_CHECK(functionDecl->params.empty());
  BOOST_CHECK(functionDecl->body->statements.empty());
}

BOOST_AUTO_TEST_CASE(ProgramSimple2)
{
  using namespace s1::parser;

  std::string inStr ("void main (out float4 pos, out float4 color) {}");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  ast::ProgramPtr program;
  BOOST_CHECK_NO_THROW(program = astBuilder.ParseProgram ());
  BOOST_CHECK(errorHandler.parseErrors.empty());

  BOOST_CHECK_EQUAL(program->statements.size(), 1u);
  const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[0].get())->functionDecl;
  BOOST_CHECK(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
  BOOST_CHECK_EQUAL(functionDecl->identifier.GetString(), "main");

  BOOST_CHECK_EQUAL(functionDecl->params.size(), 2u);
  {
    const auto& param = functionDecl->params[0];
    BOOST_CHECK_EQUAL(param.qualifiers.size(), 1u);
    BOOST_CHECK_EQUAL(param.qualifiers[0].typeOrID, s1::lexer::kwOut);
    AST_TEST_TYPE_IS_WELL_KNOWN_VEC(*param.type, kwFloat, 4);
    BOOST_CHECK_EQUAL(param.identifier.GetString(), "pos");
    BOOST_CHECK(!param.defaultValue);
  }
  {
    const auto& param = functionDecl->params[1];
    BOOST_CHECK_EQUAL(param.qualifiers.size(), 1u);
    BOOST_CHECK_EQUAL(param.qualifiers[0].typeOrID, s1::lexer::kwOut);
    AST_TEST_TYPE_IS_WELL_KNOWN_VEC(*param.type, kwFloat, 4);
    BOOST_CHECK_EQUAL(param.identifier.GetString(), "color");
    BOOST_CHECK(!param.defaultValue);
  }

  BOOST_CHECK(functionDecl->body->statements.empty());
}

BOOST_AUTO_TEST_CASE(ProgramLessSimple)
{
  using namespace s1::parser;

  std::string inStr ("void main() { int foo = 1; }");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  ast::ProgramPtr program;
  BOOST_CHECK_NO_THROW(program = astBuilder.ParseProgram ());
  BOOST_CHECK(errorHandler.parseErrors.empty());

  BOOST_CHECK_EQUAL(program->statements.size(), 1u);
  const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[0].get())->functionDecl;
  BOOST_CHECK(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
  BOOST_CHECK_EQUAL(functionDecl->identifier.GetString(), "main");
  BOOST_CHECK(functionDecl->params.empty());

  BOOST_CHECK_EQUAL(functionDecl->body->statements.size(), 1u);
  const auto varsDecl = dynamic_cast<const ast::BlockStatementVarsDecl*> (functionDecl->body->statements[0].get());
  BOOST_CHECK(!varsDecl->isConst);
  AST_TEST_TYPE_IS_WELL_KNOWN(*varsDecl->type, kwInt);

  BOOST_CHECK_EQUAL(varsDecl->vars.size(), 1u);
  BOOST_CHECK_EQUAL(varsDecl->vars[0].identifier.GetString(), "foo");
  AST_TEST_EXPR_IS_NUMERIC(*(varsDecl->vars[0].initializer), int, 1);
}

BOOST_AUTO_TEST_CASE(ProgramLessSimple2)
{
  using namespace s1::parser;

  std::string inStr ("void main() { int foo = 1, bar; }");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  ast::ProgramPtr program;
  BOOST_CHECK_NO_THROW(program = astBuilder.ParseProgram ());
  BOOST_CHECK(errorHandler.parseErrors.empty());

  BOOST_CHECK_EQUAL(program->statements.size(), 1u);
  const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[0].get())->functionDecl;
  BOOST_CHECK(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
  BOOST_CHECK_EQUAL(functionDecl->identifier.GetString(), "main");
  BOOST_CHECK(functionDecl->params.empty());

  BOOST_CHECK_EQUAL(functionDecl->body->statements.size(), 1u);
  const auto varsDecl = dynamic_cast<const ast::BlockStatementVarsDecl*> (functionDecl->body->statements[0].get());
  BOOST_CHECK(!varsDecl->isConst);
  AST_TEST_TYPE_IS_WELL_KNOWN(*varsDecl->type, kwInt);

  BOOST_CHECK_EQUAL(varsDecl->vars.size(), 2u);
  BOOST_CHECK_EQUAL(varsDecl->vars[0].identifier.GetString(), "foo");
  AST_TEST_EXPR_IS_NUMERIC(*(varsDecl->vars[0].initializer), int, 1);
  BOOST_CHECK_EQUAL(varsDecl->vars[1].identifier.GetString(), "bar");
}

BOOST_AUTO_TEST_CASE(ProgramGlobalVar)
{
  using namespace s1::parser;

  std::string inStr ("int bar; void main() { int foo = bar; }");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  ast::ProgramPtr program;
  BOOST_CHECK_NO_THROW(program = astBuilder.ParseProgram ());
  BOOST_CHECK(errorHandler.parseErrors.empty());

  BOOST_CHECK_EQUAL(program->statements.size(), 2u);
  const auto globalVarsDecl = dynamic_cast<const ast::ProgramStatementVarsDecl*> (program->statements[0].get());
  BOOST_CHECK(!globalVarsDecl->isConst);
  AST_TEST_TYPE_IS_WELL_KNOWN(*globalVarsDecl->type, kwInt);

  BOOST_CHECK_EQUAL(globalVarsDecl->vars.size(), 1u);
  BOOST_CHECK_EQUAL(globalVarsDecl->vars[0].identifier.GetString(), "bar");
  BOOST_CHECK(!globalVarsDecl->vars[0].initializer);

  const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[1].get())->functionDecl;
  BOOST_CHECK(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
  BOOST_CHECK_EQUAL(functionDecl->identifier.GetString(), "main");
  BOOST_CHECK(functionDecl->params.empty());

  BOOST_CHECK_EQUAL(functionDecl->body->statements.size(), 1u);
  const auto varsDecl = dynamic_cast<const ast::BlockStatementVarsDecl*> (functionDecl->body->statements[0].get());
  BOOST_CHECK(!varsDecl->isConst);
  AST_TEST_TYPE_IS_WELL_KNOWN(*varsDecl->type, kwInt);

  BOOST_CHECK_EQUAL(varsDecl->vars.size(), 1u);
  BOOST_CHECK_EQUAL(varsDecl->vars[0].identifier.GetString(), "foo");
  AST_TEST_EXPR_IS_IDENTIFIER(*(varsDecl->vars[0].initializer), "bar");
}

BOOST_AUTO_TEST_CASE(ProgramGlobalConst)
{
  using namespace s1::parser;

  std::string inStr ("const int bar = 1; void main() { int foo = bar; }");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  ast::ProgramPtr program;
  BOOST_CHECK_NO_THROW(program = astBuilder.ParseProgram ());
  BOOST_CHECK(errorHandler.parseErrors.empty());

  BOOST_CHECK_EQUAL(program->statements.size(), 2u);
  const auto globalVarsDecl = dynamic_cast<const ast::ProgramStatementVarsDecl*> (program->statements[0].get());
  BOOST_CHECK(globalVarsDecl->isConst);
  AST_TEST_TYPE_IS_WELL_KNOWN(*globalVarsDecl->type, kwInt);

  BOOST_CHECK_EQUAL(globalVarsDecl->vars.size(), 1u);
  BOOST_CHECK_EQUAL(globalVarsDecl->vars[0].identifier.GetString(), "bar");
  AST_TEST_EXPR_IS_NUMERIC(*(globalVarsDecl->vars[0].initializer), int, 1);

  const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[1].get())->functionDecl;
  BOOST_CHECK(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
  BOOST_CHECK_EQUAL(functionDecl->identifier.GetString(), "main");
  BOOST_CHECK(functionDecl->params.empty());

  BOOST_CHECK_EQUAL(functionDecl->body->statements.size(), 1u);
  const auto varsDecl = dynamic_cast<const ast::BlockStatementVarsDecl*> (functionDecl->body->statements[0].get());
  BOOST_CHECK(!varsDecl->isConst);
  AST_TEST_TYPE_IS_WELL_KNOWN(*varsDecl->type, kwInt);

  BOOST_CHECK_EQUAL(varsDecl->vars.size(), 1u);
  BOOST_CHECK_EQUAL(varsDecl->vars[0].identifier.GetString(), "foo");
  AST_TEST_EXPR_IS_IDENTIFIER(*(varsDecl->vars[0].initializer), "bar");
}

BOOST_AUTO_TEST_CASE(ProgramFunctionCall)
{
  using namespace s1::parser;

  std::string inStr ("void Foo() {} void main() { Foo(); }");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  ast::ProgramPtr program;
  BOOST_CHECK_NO_THROW(program = astBuilder.ParseProgram ());
  BOOST_CHECK(errorHandler.parseErrors.empty());

  BOOST_CHECK_EQUAL(program->statements.size(), 2u);
  {
    const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[0].get())->functionDecl;
    BOOST_CHECK(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
    BOOST_CHECK_EQUAL(functionDecl->identifier.GetString(), "Foo");
    BOOST_CHECK(functionDecl->params.empty());

    BOOST_CHECK(functionDecl->body->statements.empty());
  }

  {
    const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[1].get())->functionDecl;
    BOOST_CHECK(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
    BOOST_CHECK_EQUAL(functionDecl->identifier.GetString(), "main");
    BOOST_CHECK(functionDecl->params.empty());

    BOOST_CHECK_EQUAL(functionDecl->body->statements.size(), 1u);
    const auto& expr = dynamic_cast<const ast::BlockStatementExpr*> (functionDecl->body->statements[0].get())->expr;
    const auto funcCall = dynamic_cast<const ast::ExprFunctionCall*> (expr.get());
    const auto& funcCallIdent = boost::get<ast::Identifier> (funcCall->identifierOrType);
    BOOST_CHECK_EQUAL(funcCallIdent.GetString(), "Foo");
    BOOST_CHECK(funcCall->args.empty());
  }
}

BOOST_AUTO_TEST_CASE(ProgramFunctionCall2)
{
  using namespace s1::parser;

  std::string inStr ("int Foo() { return 1; } void main() { int x = Foo(); }");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  ast::ProgramPtr program;
  BOOST_CHECK_NO_THROW(program = astBuilder.ParseProgram ());
  BOOST_CHECK(errorHandler.parseErrors.empty());

  BOOST_CHECK_EQUAL(program->statements.size(), 2u);
  {
    const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[0].get())->functionDecl;
    const auto& resultType = boost::get<ast::TypePtr> (functionDecl->resultType);
    AST_TEST_TYPE_IS_WELL_KNOWN(*resultType, kwInt);
    BOOST_CHECK_EQUAL(functionDecl->identifier.GetString(), "Foo");
    BOOST_CHECK(functionDecl->params.empty());

    BOOST_CHECK_EQUAL(functionDecl->body->statements.size(), 1u);
    const auto returnStatement = dynamic_cast<const ast::BlockStatementReturn*> (functionDecl->body->statements[0].get());
    AST_TEST_EXPR_IS_NUMERIC(*(returnStatement->expr), int, 1);
  }

  {
    const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[1].get())->functionDecl;
    BOOST_CHECK(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
    BOOST_CHECK_EQUAL(functionDecl->identifier.GetString(), "main");
    BOOST_CHECK(functionDecl->params.empty());

    BOOST_CHECK_EQUAL(functionDecl->body->statements.size(), 1u);
    const auto varsDecl = dynamic_cast<const ast::BlockStatementVarsDecl*> (functionDecl->body->statements[0].get());
    BOOST_CHECK(!varsDecl->isConst);
    AST_TEST_TYPE_IS_WELL_KNOWN(*varsDecl->type, kwInt);

    BOOST_CHECK_EQUAL(varsDecl->vars.size(), 1u);
    BOOST_CHECK_EQUAL(varsDecl->vars[0].identifier.GetString(), "x");

    const auto varInitFuncCall = dynamic_cast<const ast::ExprFunctionCall*> (varsDecl->vars[0].initializer.get());
    const auto& varInitFuncCallIdent = boost::get<ast::Identifier> (varInitFuncCall->identifierOrType);
    BOOST_CHECK_EQUAL(varInitFuncCallIdent.GetString(), "Foo");
    BOOST_CHECK(varInitFuncCall->args.empty());
  }
}

BOOST_AUTO_TEST_CASE(ProgramFunctionCallCast)
{
  using namespace s1::parser;

  std::string inStr ("void main() { int x; x = float (1.2); }");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  ast::ProgramPtr program;
  BOOST_CHECK_NO_THROW(program = astBuilder.ParseProgram ());
  BOOST_CHECK(errorHandler.parseErrors.empty());

  BOOST_CHECK_EQUAL(program->statements.size(), 1u);
  const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[0].get())->functionDecl;
  BOOST_CHECK(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
  BOOST_CHECK_EQUAL(functionDecl->identifier.GetString(), "main");
  BOOST_CHECK(functionDecl->params.empty());

  BOOST_CHECK_EQUAL(functionDecl->body->statements.size(), 2u);
  const auto varsDecl = dynamic_cast<const ast::BlockStatementVarsDecl*> (functionDecl->body->statements[0].get());
  BOOST_CHECK(!varsDecl->isConst);
  AST_TEST_TYPE_IS_WELL_KNOWN(*varsDecl->type, kwInt);

  BOOST_CHECK_EQUAL(varsDecl->vars.size(), 1u);
  BOOST_CHECK_EQUAL(varsDecl->vars[0].identifier.GetString(), "x");
  BOOST_CHECK(!varsDecl->vars[0].initializer);

  const auto& assignExpr = dynamic_cast<const ast::BlockStatementExpr*> (functionDecl->body->statements[1].get())->expr;
  const auto assignExprBinary = dynamic_cast<const ast::ExprBinary*> (assignExpr.get());
  AST_TEST_EXPR_IS_IDENTIFIER(*(assignExprBinary->left), "x");
  BOOST_CHECK_EQUAL(assignExprBinary->op.typeOrID, s1::lexer::Assign);
  const auto assignRightExprFunctionCall = dynamic_cast<const ast::ExprFunctionCall*> (assignExprBinary->right.get());
  const auto& assignRightExprFunctionType = boost::get<ast::TypePtr> (assignRightExprFunctionCall->identifierOrType);
  AST_TEST_TYPE_IS_WELL_KNOWN(*assignRightExprFunctionType, kwFloat);
  BOOST_CHECK_EQUAL(assignRightExprFunctionCall->args.size(), 1u);
  AST_TEST_EXPR_IS_NUMERIC(*assignRightExprFunctionCall->args[0], float, 1.2f);
}

BOOST_AUTO_TEST_CASE(ProgramExprNoResult)
{
  using namespace s1::parser;

  std::string inStr ("void main() { int x, y; x + y; }");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  ast::ProgramPtr program;
  BOOST_CHECK_NO_THROW(program = astBuilder.ParseProgram ());
  BOOST_CHECK(errorHandler.parseErrors.empty());

  BOOST_CHECK_EQUAL(program->statements.size(), 1u);
  const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[0].get())->functionDecl;
  BOOST_CHECK(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
  BOOST_CHECK_EQUAL(functionDecl->identifier.GetString(), "main");
  BOOST_CHECK(functionDecl->params.empty());

  BOOST_CHECK_EQUAL(functionDecl->body->statements.size(), 2u);
  const auto varsDecl = dynamic_cast<const ast::BlockStatementVarsDecl*> (functionDecl->body->statements[0].get());
  BOOST_CHECK(!varsDecl->isConst);
  AST_TEST_TYPE_IS_WELL_KNOWN(*varsDecl->type, kwInt);

  BOOST_CHECK_EQUAL(varsDecl->vars.size(), 2u);
  BOOST_CHECK_EQUAL(varsDecl->vars[0].identifier.GetString(), "x");
  BOOST_CHECK(!varsDecl->vars[0].initializer);
  BOOST_CHECK_EQUAL(varsDecl->vars[1].identifier.GetString(), "y");
  BOOST_CHECK(!varsDecl->vars[1].initializer);

  const auto& assignExpr = dynamic_cast<const ast::BlockStatementExpr*> (functionDecl->body->statements[1].get())->expr;
  const auto assignExprBinary = dynamic_cast<const ast::ExprBinary*> (assignExpr.get());
  AST_TEST_EXPR_IS_IDENTIFIER(*(assignExprBinary->left), "x");
  BOOST_CHECK_EQUAL(assignExprBinary->op.typeOrID, s1::lexer::Plus);
  AST_TEST_EXPR_IS_IDENTIFIER(*(assignExprBinary->right), "y");
}

BOOST_AUTO_TEST_CASE(ProgramFunctionDeclInvalid1)
{
  using namespace s1::parser;

  std::string inStr ("void main(int) {}");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  ast::ProgramPtr program;
  BOOST_CHECK_NO_THROW(program = astBuilder.ParseProgram ());
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size (), 1u);
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedIdentifier));

  BOOST_CHECK_EQUAL(program->statements.size(), 1u);
  const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[0].get())->functionDecl;
  BOOST_CHECK(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
  BOOST_CHECK_EQUAL(functionDecl->identifier.GetString(), "main");
  BOOST_CHECK_EQUAL(functionDecl->params.size(), 1u);

  {
    const auto& param = functionDecl->params[0];
    BOOST_CHECK(param.qualifiers.empty());
    AST_TEST_TYPE_IS_WELL_KNOWN(*param.type, kwInt);
    BOOST_CHECK(!param.defaultValue);
  }

  BOOST_CHECK(functionDecl->body->statements.empty());
}

BOOST_AUTO_TEST_CASE(ProgramFunctionDeclInvalid2)
{
  using namespace s1::parser;

  std::string inStr ("void main(int 12) {}");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  ast::ProgramPtr program;
  BOOST_CHECK_NO_THROW(program = astBuilder.ParseProgram ());
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size (), 2u);
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedIdentifier));
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[1].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedSeparatorOrParenthesis));

  BOOST_CHECK_EQUAL(program->statements.size(), 1u);
  const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[0].get())->functionDecl;
  BOOST_CHECK(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
  BOOST_CHECK_EQUAL(functionDecl->identifier.GetString(), "main");
  BOOST_CHECK_EQUAL(functionDecl->params.size(), 1u);

  {
    const auto& param = functionDecl->params[0];
    BOOST_CHECK(param.qualifiers.empty());
    AST_TEST_TYPE_IS_WELL_KNOWN(*param.type, kwInt);
    BOOST_CHECK(!param.defaultValue);
  }

  BOOST_CHECK(functionDecl->body->statements.empty());
}

BOOST_AUTO_TEST_CASE(ProgramFunctionDeclInvalid3)
{
  using namespace s1::parser;

  std::string inStr ("void main(int =) {}");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  ast::ProgramPtr program;
  BOOST_CHECK_NO_THROW(program = astBuilder.ParseProgram ());
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size (), 2u);
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedIdentifier));
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[1].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedExpression));

  BOOST_CHECK_EQUAL(program->statements.size(), 1u);
  const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[0].get())->functionDecl;
  BOOST_CHECK(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
  BOOST_CHECK_EQUAL(functionDecl->identifier.GetString(), "main");
  BOOST_CHECK_EQUAL(functionDecl->params.size(), 1u);

  {
    const auto& param = functionDecl->params[0];
    BOOST_CHECK(param.qualifiers.empty());
    AST_TEST_TYPE_IS_WELL_KNOWN(*param.type, kwInt);
    BOOST_CHECK(!param.defaultValue);
  }

  BOOST_CHECK(functionDecl->body->statements.empty());
}

BOOST_AUTO_TEST_CASE(ProgramFunctionDeclInvalid4)
{
  using namespace s1::parser;

  std::string inStr ("void main(int = 12) {}");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  ast::ProgramPtr program;
  BOOST_CHECK_NO_THROW(program = astBuilder.ParseProgram ());
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size (), 1u);
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedIdentifier));

  BOOST_CHECK_EQUAL(program->statements.size(), 1u);
  const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[0].get())->functionDecl;
  BOOST_CHECK(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
  BOOST_CHECK_EQUAL(functionDecl->identifier.GetString(), "main");
  BOOST_CHECK_EQUAL(functionDecl->params.size(), 1u);

  {
    const auto& param = functionDecl->params[0];
    BOOST_CHECK(param.qualifiers.empty());
    AST_TEST_TYPE_IS_WELL_KNOWN(*param.type, kwInt);
    BOOST_CHECK(param.defaultValue);
  }

  BOOST_CHECK(functionDecl->body->statements.empty());
}

BOOST_AUTO_TEST_CASE(ProgramFunctionDeclInvalid5)
{
  using namespace s1::parser;

  std::string inStr ("void main(int x =) {}");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  ast::ProgramPtr program;
  BOOST_CHECK_NO_THROW(program = astBuilder.ParseProgram ());
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size (), 1u);
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedExpression));

  BOOST_CHECK_EQUAL(program->statements.size(), 1u);
  const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[0].get())->functionDecl;
  BOOST_CHECK(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
  BOOST_CHECK_EQUAL(functionDecl->identifier.GetString(), "main");
  BOOST_CHECK_EQUAL(functionDecl->params.size(), 1u);

  {
    const auto& param = functionDecl->params[0];
    BOOST_CHECK(param.qualifiers.empty());
    AST_TEST_TYPE_IS_WELL_KNOWN(*param.type, kwInt);
    BOOST_CHECK(!param.defaultValue);
  }

  BOOST_CHECK(functionDecl->body->statements.empty());
}

BOOST_AUTO_TEST_CASE(ProgramFunctionDeclInvalid6)
{
  using namespace s1::parser;

  std::string inStr ("void main(int x 12) {}");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  ast::ProgramPtr program;
  BOOST_CHECK_NO_THROW(program = astBuilder.ParseProgram ());
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size (), 1u);
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedSeparatorOrParenthesis));

  BOOST_CHECK_EQUAL(program->statements.size(), 1u);
  const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[0].get())->functionDecl;
  BOOST_CHECK(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
  BOOST_CHECK_EQUAL(functionDecl->identifier.GetString(), "main");
  BOOST_CHECK_EQUAL(functionDecl->params.size(), 1u);

  {
    const auto& param = functionDecl->params[0];
    BOOST_CHECK(param.qualifiers.empty());
    AST_TEST_TYPE_IS_WELL_KNOWN(*param.type, kwInt);
    BOOST_CHECK(!param.defaultValue);
  }

  BOOST_CHECK(functionDecl->body->statements.empty());
}

BOOST_AUTO_TEST_CASE(ProgramFunctionDeclInvalid7)
{
  using namespace s1::parser;

  std::string inStr ("void main(x) {}");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  ast::ProgramPtr program;
  BOOST_CHECK_NO_THROW(program = astBuilder.ParseProgram ());
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size (), 1u);
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedIdentifier));

  BOOST_CHECK_EQUAL(program->statements.size(), 1u);
  const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[0].get())->functionDecl;
  BOOST_CHECK(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
  BOOST_CHECK_EQUAL(functionDecl->identifier.GetString(), "main");
  BOOST_CHECK_EQUAL(functionDecl->params.size(), 1u);

  {
    const auto& param = functionDecl->params[0];
    BOOST_CHECK(param.qualifiers.empty());
    AST_TEST_TYPE_IS_ALIAS(*param.type, "x");
    BOOST_CHECK(!param.defaultValue);
  }

  BOOST_CHECK(functionDecl->body->statements.empty());
}

BOOST_AUTO_TEST_CASE(ProgramFunctionDeclInvalid8)
{
  using namespace s1::parser;

  std::string inStr ("void main(int x float y) {}");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  ast::ProgramPtr program;
  BOOST_CHECK_NO_THROW(program = astBuilder.ParseProgram ());
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size (), 1u);
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedSeparatorOrParenthesis));

  BOOST_CHECK_EQUAL(program->statements.size(), 1u);
  const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[0].get())->functionDecl;
  BOOST_CHECK(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
  BOOST_CHECK_EQUAL(functionDecl->identifier.GetString(), "main");
  BOOST_CHECK_EQUAL(functionDecl->params.size(), 2u);

  {
    const auto& param = functionDecl->params[0];
    BOOST_CHECK(param.qualifiers.empty());
    AST_TEST_TYPE_IS_WELL_KNOWN(*param.type, kwInt);
    BOOST_CHECK_EQUAL(param.identifier.GetString(), "x");
    BOOST_CHECK(!param.defaultValue);
  }
  {
    const auto& param = functionDecl->params[1];
    BOOST_CHECK(param.qualifiers.empty());
    AST_TEST_TYPE_IS_WELL_KNOWN(*param.type, kwFloat);
    BOOST_CHECK_EQUAL(param.identifier.GetString(), "y");
    BOOST_CHECK(!param.defaultValue);
  }

  BOOST_CHECK(functionDecl->body->statements.empty());
}

BOOST_AUTO_TEST_CASE(ProgramFunctionDeclInvalid9)
{
  using namespace s1::parser;

  std::string inStr ("void main(int x, ) {}");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  ast::ProgramPtr program;
  BOOST_CHECK_NO_THROW(program = astBuilder.ParseProgram ());
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size (), 1u);
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedTypeOrIdentifier));

  BOOST_CHECK_EQUAL(program->statements.size(), 1u);
  const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[0].get())->functionDecl;
  BOOST_CHECK(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
  BOOST_CHECK_EQUAL(functionDecl->identifier.GetString(), "main");
  BOOST_CHECK_EQUAL(functionDecl->params.size(), 1u);

  {
    const auto& param = functionDecl->params[0];
    BOOST_CHECK(param.qualifiers.empty());
    AST_TEST_TYPE_IS_WELL_KNOWN(*param.type, kwInt);
    BOOST_CHECK_EQUAL(param.identifier.GetString(), "x");
    BOOST_CHECK(!param.defaultValue);
  }

  BOOST_CHECK(functionDecl->body->statements.empty());
}

BOOST_AUTO_TEST_CASE(ProgramFunctionDeclInvalid10)
{
  using namespace s1::parser;

  std::string inStr ("void main(int x,, ) {}");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  ast::ProgramPtr program;
  BOOST_CHECK_NO_THROW(program = astBuilder.ParseProgram ());
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size (), 2u);
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedTypeOrIdentifier));
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[1].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedTypeOrIdentifier));

  BOOST_CHECK_EQUAL(program->statements.size(), 1u);
  const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[0].get())->functionDecl;
  BOOST_CHECK(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
  BOOST_CHECK_EQUAL(functionDecl->identifier.GetString(), "main");
  BOOST_CHECK_EQUAL(functionDecl->params.size(), 1u);

  {
    const auto& param = functionDecl->params[0];
    BOOST_CHECK(param.qualifiers.empty());
    AST_TEST_TYPE_IS_WELL_KNOWN(*param.type, kwInt);
    BOOST_CHECK_EQUAL(param.identifier.GetString(), "x");
    BOOST_CHECK(!param.defaultValue);
  }

  BOOST_CHECK(functionDecl->body->statements.empty());
}

BOOST_AUTO_TEST_CASE(ProgramFunctionDeclInvalid11)
{
  using namespace s1::parser;

  std::string inStr ("void main(int x {}");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  ast::ProgramPtr program;
  BOOST_CHECK_NO_THROW(program = astBuilder.ParseProgram ());
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size (), 1u);
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedSeparatorOrParenthesis));

  BOOST_CHECK_EQUAL(program->statements.size(), 1u);
  const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[0].get())->functionDecl;
  BOOST_CHECK(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
  BOOST_CHECK_EQUAL(functionDecl->identifier.GetString(), "main");
  BOOST_CHECK_EQUAL(functionDecl->params.size(), 1u);

  {
    const auto& param = functionDecl->params[0];
    BOOST_CHECK(param.qualifiers.empty());
    AST_TEST_TYPE_IS_WELL_KNOWN(*param.type, kwInt);
    BOOST_CHECK_EQUAL(param.identifier.GetString(), "x");
    BOOST_CHECK(!param.defaultValue);
  }

  BOOST_CHECK(functionDecl->body->statements.empty());
}

BOOST_AUTO_TEST_SUITE_END()
