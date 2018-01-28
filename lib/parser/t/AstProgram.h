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

#include <cxxtest/TestSuite.h>

#include "base/common.h"

#include "parser/ast/FunctionDecl.h"
#include "parser/ast/BlockStatementReturn.h"
#include "parser/ast/Program.h"
#include "parser/ast/ProgramStatementFunctionDecl.h"
#include "parser/ast/ProgramStatementVarsDecl.h"

#include "AstMacros.h"
#include "TestAstBuilder.h"

#include "../../diagnostics/t/TestDiagnosticsHandler.h"

class AstProgramTestSuite : public CxxTest::TestSuite 
{
public:
  void testProgramSimple (void)
  {
    using namespace s1::parser;

    std::string inStr ("void main() {}");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    ast::ProgramPtr program;
    TS_ASSERT_THROWS_NOTHING(program = astBuilder.ParseProgram ());
    TS_ASSERT(errorHandler.parseErrors.empty());

    TS_ASSERT_EQUALS(program->statements.size(), 1u);
    const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[0].get())->functionDecl;
    TS_ASSERT(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
    TS_ASSERT_EQUALS(functionDecl->identifier.GetString(), "main");
    TS_ASSERT(functionDecl->params.empty());
    TS_ASSERT(functionDecl->body->statements.empty());
  }

  void testProgramSimple2 (void)
  {
    using namespace s1::parser;

    std::string inStr ("void main (out float4 pos, out float4 color) {}");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    ast::ProgramPtr program;
    TS_ASSERT_THROWS_NOTHING(program = astBuilder.ParseProgram ());
    TS_ASSERT(errorHandler.parseErrors.empty());

    TS_ASSERT_EQUALS(program->statements.size(), 1u);
    const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[0].get())->functionDecl;
    TS_ASSERT(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
    TS_ASSERT_EQUALS(functionDecl->identifier.GetString(), "main");

    TS_ASSERT_EQUALS(functionDecl->params.size(), 2u);
    {
      const auto& param = functionDecl->params[0];
      TS_ASSERT_EQUALS(param.qualifiers.size(), 1u);
      TS_ASSERT_EQUALS(param.qualifiers[0].typeOrID, s1::lexer::kwOut);
      AST_TEST_TYPE_IS_WELL_KNOWN_VEC(*param.type, kwFloat, 4);
      TS_ASSERT_EQUALS(param.identifier.GetString(), "pos");
      TS_ASSERT(!param.defaultValue);
    }
    {
      const auto& param = functionDecl->params[1];
      TS_ASSERT_EQUALS(param.qualifiers.size(), 1u);
      TS_ASSERT_EQUALS(param.qualifiers[0].typeOrID, s1::lexer::kwOut);
      AST_TEST_TYPE_IS_WELL_KNOWN_VEC(*param.type, kwFloat, 4);
      TS_ASSERT_EQUALS(param.identifier.GetString(), "color");
      TS_ASSERT(!param.defaultValue);
    }

    TS_ASSERT(functionDecl->body->statements.empty());
  }

  void testProgramLessSimple (void)
  {
    using namespace s1::parser;

    std::string inStr ("void main() { int foo = 1; }");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    ast::ProgramPtr program;
    TS_ASSERT_THROWS_NOTHING(program = astBuilder.ParseProgram ());
    TS_ASSERT(errorHandler.parseErrors.empty());

    TS_ASSERT_EQUALS(program->statements.size(), 1u);
    const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[0].get())->functionDecl;
    TS_ASSERT(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
    TS_ASSERT_EQUALS(functionDecl->identifier.GetString(), "main");
    TS_ASSERT(functionDecl->params.empty());

    TS_ASSERT_EQUALS(functionDecl->body->statements.size(), 1u);
    const auto varsDecl = dynamic_cast<const ast::BlockStatementVarsDecl*> (functionDecl->body->statements[0].get());
    TS_ASSERT(!varsDecl->isConst);
    AST_TEST_TYPE_IS_WELL_KNOWN(*varsDecl->type, kwInt);

    TS_ASSERT_EQUALS(varsDecl->vars.size(), 1u);
    TS_ASSERT_EQUALS(varsDecl->vars[0].identifier.GetString(), "foo");
    AST_TEST_EXPR_IS_NUMERIC(*(varsDecl->vars[0].initializer), int, 1);
  }

  void testProgramLessSimple2 (void)
  {
    using namespace s1::parser;

    std::string inStr ("void main() { int foo = 1, bar; }");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    ast::ProgramPtr program;
    TS_ASSERT_THROWS_NOTHING(program = astBuilder.ParseProgram ());
    TS_ASSERT(errorHandler.parseErrors.empty());

    TS_ASSERT_EQUALS(program->statements.size(), 1u);
    const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[0].get())->functionDecl;
    TS_ASSERT(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
    TS_ASSERT_EQUALS(functionDecl->identifier.GetString(), "main");
    TS_ASSERT(functionDecl->params.empty());

    TS_ASSERT_EQUALS(functionDecl->body->statements.size(), 1u);
    const auto varsDecl = dynamic_cast<const ast::BlockStatementVarsDecl*> (functionDecl->body->statements[0].get());
    TS_ASSERT(!varsDecl->isConst);
    AST_TEST_TYPE_IS_WELL_KNOWN(*varsDecl->type, kwInt);

    TS_ASSERT_EQUALS(varsDecl->vars.size(), 2u);
    TS_ASSERT_EQUALS(varsDecl->vars[0].identifier.GetString(), "foo");
    AST_TEST_EXPR_IS_NUMERIC(*(varsDecl->vars[0].initializer), int, 1);
    TS_ASSERT_EQUALS(varsDecl->vars[1].identifier.GetString(), "bar");
  }

  void testProgramGlobalVar (void)
  {
    using namespace s1::parser;

    std::string inStr ("int bar; void main() { int foo = bar; }");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    ast::ProgramPtr program;
    TS_ASSERT_THROWS_NOTHING(program = astBuilder.ParseProgram ());
    TS_ASSERT(errorHandler.parseErrors.empty());

    TS_ASSERT_EQUALS(program->statements.size(), 2u);
    const auto globalVarsDecl = dynamic_cast<const ast::ProgramStatementVarsDecl*> (program->statements[0].get());
    TS_ASSERT(!globalVarsDecl->isConst);
    AST_TEST_TYPE_IS_WELL_KNOWN(*globalVarsDecl->type, kwInt);

    TS_ASSERT_EQUALS(globalVarsDecl->vars.size(), 1u);
    TS_ASSERT_EQUALS(globalVarsDecl->vars[0].identifier.GetString(), "bar");
    TS_ASSERT(!globalVarsDecl->vars[0].initializer);

    const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[1].get())->functionDecl;
    TS_ASSERT(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
    TS_ASSERT_EQUALS(functionDecl->identifier.GetString(), "main");
    TS_ASSERT(functionDecl->params.empty());

    TS_ASSERT_EQUALS(functionDecl->body->statements.size(), 1u);
    const auto varsDecl = dynamic_cast<const ast::BlockStatementVarsDecl*> (functionDecl->body->statements[0].get());
    TS_ASSERT(!varsDecl->isConst);
    AST_TEST_TYPE_IS_WELL_KNOWN(*varsDecl->type, kwInt);

    TS_ASSERT_EQUALS(varsDecl->vars.size(), 1u);
    TS_ASSERT_EQUALS(varsDecl->vars[0].identifier.GetString(), "foo");
    AST_TEST_EXPR_IS_IDENTIFIER(*(varsDecl->vars[0].initializer), "bar");
  }

  void testProgramGlobalConst (void)
  {
    using namespace s1::parser;

    std::string inStr ("const int bar = 1; void main() { int foo = bar; }");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    ast::ProgramPtr program;
    TS_ASSERT_THROWS_NOTHING(program = astBuilder.ParseProgram ());
    TS_ASSERT(errorHandler.parseErrors.empty());

    TS_ASSERT_EQUALS(program->statements.size(), 2u);
    const auto globalVarsDecl = dynamic_cast<const ast::ProgramStatementVarsDecl*> (program->statements[0].get());
    TS_ASSERT(globalVarsDecl->isConst);
    AST_TEST_TYPE_IS_WELL_KNOWN(*globalVarsDecl->type, kwInt);

    TS_ASSERT_EQUALS(globalVarsDecl->vars.size(), 1u);
    TS_ASSERT_EQUALS(globalVarsDecl->vars[0].identifier.GetString(), "bar");
    AST_TEST_EXPR_IS_NUMERIC(*(globalVarsDecl->vars[0].initializer), int, 1);

    const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[1].get())->functionDecl;
    TS_ASSERT(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
    TS_ASSERT_EQUALS(functionDecl->identifier.GetString(), "main");
    TS_ASSERT(functionDecl->params.empty());

    TS_ASSERT_EQUALS(functionDecl->body->statements.size(), 1u);
    const auto varsDecl = dynamic_cast<const ast::BlockStatementVarsDecl*> (functionDecl->body->statements[0].get());
    TS_ASSERT(!varsDecl->isConst);
    AST_TEST_TYPE_IS_WELL_KNOWN(*varsDecl->type, kwInt);

    TS_ASSERT_EQUALS(varsDecl->vars.size(), 1u);
    TS_ASSERT_EQUALS(varsDecl->vars[0].identifier.GetString(), "foo");
    AST_TEST_EXPR_IS_IDENTIFIER(*(varsDecl->vars[0].initializer), "bar");
  }

  void testProgramFunctionCall (void)
  {
    using namespace s1::parser;

    std::string inStr ("void Foo() {} void main() { Foo(); }");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    ast::ProgramPtr program;
    TS_ASSERT_THROWS_NOTHING(program = astBuilder.ParseProgram ());
    TS_ASSERT(errorHandler.parseErrors.empty());

    TS_ASSERT_EQUALS(program->statements.size(), 2u);
    {
      const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[0].get())->functionDecl;
      TS_ASSERT(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
      TS_ASSERT_EQUALS(functionDecl->identifier.GetString(), "Foo");
      TS_ASSERT(functionDecl->params.empty());

      TS_ASSERT(functionDecl->body->statements.empty());
    }

    {
      const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[1].get())->functionDecl;
      TS_ASSERT(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
      TS_ASSERT_EQUALS(functionDecl->identifier.GetString(), "main");
      TS_ASSERT(functionDecl->params.empty());

      TS_ASSERT_EQUALS(functionDecl->body->statements.size(), 1u);
      const auto& expr = dynamic_cast<const ast::BlockStatementExpr*> (functionDecl->body->statements[0].get())->expr;
      const auto funcCall = dynamic_cast<const ast::ExprFunctionCall*> (expr.get());
      const auto& funcCallIdent = boost::get<ast::Identifier> (funcCall->identifierOrType);
      TS_ASSERT_EQUALS(funcCallIdent.GetString(), "Foo");
      TS_ASSERT(funcCall->args.empty());
    }
  }

  void testProgramFunctionCall2 (void)
  {
    using namespace s1::parser;

    std::string inStr ("int Foo() { return 1; } void main() { int x = Foo(); }");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    ast::ProgramPtr program;
    TS_ASSERT_THROWS_NOTHING(program = astBuilder.ParseProgram ());
    TS_ASSERT(errorHandler.parseErrors.empty());

    TS_ASSERT_EQUALS(program->statements.size(), 2u);
    {
      const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[0].get())->functionDecl;
      const auto& resultType = boost::get<ast::TypePtr> (functionDecl->resultType);
      AST_TEST_TYPE_IS_WELL_KNOWN(*resultType, kwInt);
      TS_ASSERT_EQUALS(functionDecl->identifier.GetString(), "Foo");
      TS_ASSERT(functionDecl->params.empty());

      TS_ASSERT_EQUALS(functionDecl->body->statements.size(), 1u);
      const auto returnStatement = dynamic_cast<const ast::BlockStatementReturn*> (functionDecl->body->statements[0].get());
      AST_TEST_EXPR_IS_NUMERIC(*(returnStatement->expr), int, 1);
    }

    {
      const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[1].get())->functionDecl;
      TS_ASSERT(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
      TS_ASSERT_EQUALS(functionDecl->identifier.GetString(), "main");
      TS_ASSERT(functionDecl->params.empty());

      TS_ASSERT_EQUALS(functionDecl->body->statements.size(), 1u);
      const auto varsDecl = dynamic_cast<const ast::BlockStatementVarsDecl*> (functionDecl->body->statements[0].get());
      TS_ASSERT(!varsDecl->isConst);
      AST_TEST_TYPE_IS_WELL_KNOWN(*varsDecl->type, kwInt);

      TS_ASSERT_EQUALS(varsDecl->vars.size(), 1u);
      TS_ASSERT_EQUALS(varsDecl->vars[0].identifier.GetString(), "x");

      const auto varInitFuncCall = dynamic_cast<const ast::ExprFunctionCall*> (varsDecl->vars[0].initializer.get());
      const auto& varInitFuncCallIdent = boost::get<ast::Identifier> (varInitFuncCall->identifierOrType);
      TS_ASSERT_EQUALS(varInitFuncCallIdent.GetString(), "Foo");
      TS_ASSERT(varInitFuncCall->args.empty());
    }
  }

  void testProgramFunctionCallCast (void)
  {
    using namespace s1::parser;

    std::string inStr ("void main() { int x; x = float (1.2); }");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    ast::ProgramPtr program;
    TS_ASSERT_THROWS_NOTHING(program = astBuilder.ParseProgram ());
    TS_ASSERT(errorHandler.parseErrors.empty());

    TS_ASSERT_EQUALS(program->statements.size(), 1u);
    const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[0].get())->functionDecl;
    TS_ASSERT(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
    TS_ASSERT_EQUALS(functionDecl->identifier.GetString(), "main");
    TS_ASSERT(functionDecl->params.empty());

    TS_ASSERT_EQUALS(functionDecl->body->statements.size(), 2u);
    const auto varsDecl = dynamic_cast<const ast::BlockStatementVarsDecl*> (functionDecl->body->statements[0].get());
    TS_ASSERT(!varsDecl->isConst);
    AST_TEST_TYPE_IS_WELL_KNOWN(*varsDecl->type, kwInt);

    TS_ASSERT_EQUALS(varsDecl->vars.size(), 1u);
    TS_ASSERT_EQUALS(varsDecl->vars[0].identifier.GetString(), "x");
    TS_ASSERT(!varsDecl->vars[0].initializer);

    const auto& assignExpr = dynamic_cast<const ast::BlockStatementExpr*> (functionDecl->body->statements[1].get())->expr;
    const auto assignExprBinary = dynamic_cast<const ast::ExprBinary*> (assignExpr.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(assignExprBinary->left), "x");
    TS_ASSERT_EQUALS(assignExprBinary->op.typeOrID, s1::lexer::Assign);
    const auto assignRightExprFunctionCall = dynamic_cast<const ast::ExprFunctionCall*> (assignExprBinary->right.get());
    const auto& assignRightExprFunctionType = boost::get<ast::TypePtr> (assignRightExprFunctionCall->identifierOrType);
    AST_TEST_TYPE_IS_WELL_KNOWN(*assignRightExprFunctionType, kwFloat);
    TS_ASSERT_EQUALS(assignRightExprFunctionCall->args.size(), 1u);
    AST_TEST_EXPR_IS_NUMERIC(*assignRightExprFunctionCall->args[0], float, 1.2f);
  }

  void testProgramExprNoResult (void)
  {
    using namespace s1::parser;

    std::string inStr ("void main() { int x, y; x + y; }");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    ast::ProgramPtr program;
    TS_ASSERT_THROWS_NOTHING(program = astBuilder.ParseProgram ());
    TS_ASSERT(errorHandler.parseErrors.empty());

    TS_ASSERT_EQUALS(program->statements.size(), 1u);
    const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[0].get())->functionDecl;
    TS_ASSERT(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
    TS_ASSERT_EQUALS(functionDecl->identifier.GetString(), "main");
    TS_ASSERT(functionDecl->params.empty());

    TS_ASSERT_EQUALS(functionDecl->body->statements.size(), 2u);
    const auto varsDecl = dynamic_cast<const ast::BlockStatementVarsDecl*> (functionDecl->body->statements[0].get());
    TS_ASSERT(!varsDecl->isConst);
    AST_TEST_TYPE_IS_WELL_KNOWN(*varsDecl->type, kwInt);

    TS_ASSERT_EQUALS(varsDecl->vars.size(), 2u);
    TS_ASSERT_EQUALS(varsDecl->vars[0].identifier.GetString(), "x");
    TS_ASSERT(!varsDecl->vars[0].initializer);
    TS_ASSERT_EQUALS(varsDecl->vars[1].identifier.GetString(), "y");
    TS_ASSERT(!varsDecl->vars[1].initializer);

    const auto& assignExpr = dynamic_cast<const ast::BlockStatementExpr*> (functionDecl->body->statements[1].get())->expr;
    const auto assignExprBinary = dynamic_cast<const ast::ExprBinary*> (assignExpr.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(assignExprBinary->left), "x");
    TS_ASSERT_EQUALS(assignExprBinary->op.typeOrID, s1::lexer::Plus);
    AST_TEST_EXPR_IS_IDENTIFIER(*(assignExprBinary->right), "y");
  }

  void testProgramFunctionDeclInvalid1 (void)
  {
    using namespace s1::parser;

    std::string inStr ("void main(int) {}");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    ast::ProgramPtr program;
    TS_ASSERT_THROWS_NOTHING(program = astBuilder.ParseProgram ());
    TS_ASSERT_EQUALS(errorHandler.parseErrors.size (), 1u);
    TS_ASSERT_EQUALS(errorHandler.parseErrors[0].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedIdentifier));

    TS_ASSERT_EQUALS(program->statements.size(), 1u);
    const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[0].get())->functionDecl;
    TS_ASSERT(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
    TS_ASSERT_EQUALS(functionDecl->identifier.GetString(), "main");
    TS_ASSERT_EQUALS(functionDecl->params.size(), 1u);

    {
      const auto& param = functionDecl->params[0];
      TS_ASSERT(param.qualifiers.empty());
      AST_TEST_TYPE_IS_WELL_KNOWN(*param.type, kwInt);
      TS_ASSERT(!param.defaultValue);
    }

    TS_ASSERT(functionDecl->body->statements.empty());
  }

  void testProgramFunctionDeclInvalid2 (void)
  {
    using namespace s1::parser;

    std::string inStr ("void main(int 12) {}");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    ast::ProgramPtr program;
    TS_ASSERT_THROWS_NOTHING(program = astBuilder.ParseProgram ());
    TS_ASSERT_EQUALS(errorHandler.parseErrors.size (), 2u);
    TS_ASSERT_EQUALS(errorHandler.parseErrors[0].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedIdentifier));
    TS_ASSERT_EQUALS(errorHandler.parseErrors[1].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedSeparatorOrParenthesis));

    TS_ASSERT_EQUALS(program->statements.size(), 1u);
    const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[0].get())->functionDecl;
    TS_ASSERT(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
    TS_ASSERT_EQUALS(functionDecl->identifier.GetString(), "main");
    TS_ASSERT_EQUALS(functionDecl->params.size(), 1u);

    {
      const auto& param = functionDecl->params[0];
      TS_ASSERT(param.qualifiers.empty());
      AST_TEST_TYPE_IS_WELL_KNOWN(*param.type, kwInt);
      TS_ASSERT(!param.defaultValue);
    }

    TS_ASSERT(functionDecl->body->statements.empty());
  }

  void testProgramFunctionDeclInvalid3 (void)
  {
    using namespace s1::parser;

    std::string inStr ("void main(int =) {}");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    ast::ProgramPtr program;
    TS_ASSERT_THROWS_NOTHING(program = astBuilder.ParseProgram ());
    TS_ASSERT_EQUALS(errorHandler.parseErrors.size (), 2u);
    TS_ASSERT_EQUALS(errorHandler.parseErrors[0].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedIdentifier));
    TS_ASSERT_EQUALS(errorHandler.parseErrors[1].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedExpression));

    TS_ASSERT_EQUALS(program->statements.size(), 1u);
    const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[0].get())->functionDecl;
    TS_ASSERT(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
    TS_ASSERT_EQUALS(functionDecl->identifier.GetString(), "main");
    TS_ASSERT_EQUALS(functionDecl->params.size(), 1u);

    {
      const auto& param = functionDecl->params[0];
      TS_ASSERT(param.qualifiers.empty());
      AST_TEST_TYPE_IS_WELL_KNOWN(*param.type, kwInt);
      TS_ASSERT(!param.defaultValue);
    }

    TS_ASSERT(functionDecl->body->statements.empty());
  }

  void testProgramFunctionDeclInvalid4 (void)
  {
    using namespace s1::parser;

    std::string inStr ("void main(int = 12) {}");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    ast::ProgramPtr program;
    TS_ASSERT_THROWS_NOTHING(program = astBuilder.ParseProgram ());
    TS_ASSERT_EQUALS(errorHandler.parseErrors.size (), 1u);
    TS_ASSERT_EQUALS(errorHandler.parseErrors[0].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedIdentifier));

    TS_ASSERT_EQUALS(program->statements.size(), 1u);
    const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[0].get())->functionDecl;
    TS_ASSERT(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
    TS_ASSERT_EQUALS(functionDecl->identifier.GetString(), "main");
    TS_ASSERT_EQUALS(functionDecl->params.size(), 1u);

    {
      const auto& param = functionDecl->params[0];
      TS_ASSERT(param.qualifiers.empty());
      AST_TEST_TYPE_IS_WELL_KNOWN(*param.type, kwInt);
      TS_ASSERT(param.defaultValue);
    }

    TS_ASSERT(functionDecl->body->statements.empty());
  }

  void testProgramFunctionDeclInvalid5 (void)
  {
    using namespace s1::parser;

    std::string inStr ("void main(int x =) {}");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    ast::ProgramPtr program;
    TS_ASSERT_THROWS_NOTHING(program = astBuilder.ParseProgram ());
    TS_ASSERT_EQUALS(errorHandler.parseErrors.size (), 1u);
    TS_ASSERT_EQUALS(errorHandler.parseErrors[0].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedExpression));

    TS_ASSERT_EQUALS(program->statements.size(), 1u);
    const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[0].get())->functionDecl;
    TS_ASSERT(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
    TS_ASSERT_EQUALS(functionDecl->identifier.GetString(), "main");
    TS_ASSERT_EQUALS(functionDecl->params.size(), 1u);

    {
      const auto& param = functionDecl->params[0];
      TS_ASSERT(param.qualifiers.empty());
      AST_TEST_TYPE_IS_WELL_KNOWN(*param.type, kwInt);
      TS_ASSERT(!param.defaultValue);
    }

    TS_ASSERT(functionDecl->body->statements.empty());
  }

  void testProgramFunctionDeclInvalid6 (void)
  {
    using namespace s1::parser;

    std::string inStr ("void main(int x 12) {}");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    ast::ProgramPtr program;
    TS_ASSERT_THROWS_NOTHING(program = astBuilder.ParseProgram ());
    TS_ASSERT_EQUALS(errorHandler.parseErrors.size (), 1u);
    TS_ASSERT_EQUALS(errorHandler.parseErrors[0].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedSeparatorOrParenthesis));

    TS_ASSERT_EQUALS(program->statements.size(), 1u);
    const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[0].get())->functionDecl;
    TS_ASSERT(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
    TS_ASSERT_EQUALS(functionDecl->identifier.GetString(), "main");
    TS_ASSERT_EQUALS(functionDecl->params.size(), 1u);

    {
      const auto& param = functionDecl->params[0];
      TS_ASSERT(param.qualifiers.empty());
      AST_TEST_TYPE_IS_WELL_KNOWN(*param.type, kwInt);
      TS_ASSERT(!param.defaultValue);
    }

    TS_ASSERT(functionDecl->body->statements.empty());
  }

  void testProgramFunctionDeclInvalid7 (void)
  {
    using namespace s1::parser;

    std::string inStr ("void main(x) {}");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    ast::ProgramPtr program;
    TS_ASSERT_THROWS_NOTHING(program = astBuilder.ParseProgram ());
    TS_ASSERT_EQUALS(errorHandler.parseErrors.size (), 1u);
    TS_ASSERT_EQUALS(errorHandler.parseErrors[0].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedIdentifier));

    TS_ASSERT_EQUALS(program->statements.size(), 1u);
    const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[0].get())->functionDecl;
    TS_ASSERT(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
    TS_ASSERT_EQUALS(functionDecl->identifier.GetString(), "main");
    TS_ASSERT_EQUALS(functionDecl->params.size(), 1u);

    {
      const auto& param = functionDecl->params[0];
      TS_ASSERT(param.qualifiers.empty());
      AST_TEST_TYPE_IS_ALIAS(*param.type, "x");
      TS_ASSERT(!param.defaultValue);
    }

    TS_ASSERT(functionDecl->body->statements.empty());
  }

  void testProgramFunctionDeclInvalid8 (void)
  {
    using namespace s1::parser;

    std::string inStr ("void main(int x float y) {}");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    ast::ProgramPtr program;
    TS_ASSERT_THROWS_NOTHING(program = astBuilder.ParseProgram ());
    TS_ASSERT_EQUALS(errorHandler.parseErrors.size (), 1u);
    TS_ASSERT_EQUALS(errorHandler.parseErrors[0].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedSeparatorOrParenthesis));

    TS_ASSERT_EQUALS(program->statements.size(), 1u);
    const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[0].get())->functionDecl;
    TS_ASSERT(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
    TS_ASSERT_EQUALS(functionDecl->identifier.GetString(), "main");
    TS_ASSERT_EQUALS(functionDecl->params.size(), 2u);

    {
      const auto& param = functionDecl->params[0];
      TS_ASSERT(param.qualifiers.empty());
      AST_TEST_TYPE_IS_WELL_KNOWN(*param.type, kwInt);
      TS_ASSERT_EQUALS(param.identifier.GetString(), "x");
      TS_ASSERT(!param.defaultValue);
    }
    {
      const auto& param = functionDecl->params[1];
      TS_ASSERT(param.qualifiers.empty());
      AST_TEST_TYPE_IS_WELL_KNOWN(*param.type, kwFloat);
      TS_ASSERT_EQUALS(param.identifier.GetString(), "y");
      TS_ASSERT(!param.defaultValue);
    }

    TS_ASSERT(functionDecl->body->statements.empty());
  }

  void testProgramFunctionDeclInvalid9 (void)
  {
    using namespace s1::parser;

    std::string inStr ("void main(int x, ) {}");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    ast::ProgramPtr program;
    TS_ASSERT_THROWS_NOTHING(program = astBuilder.ParseProgram ());
    TS_ASSERT_EQUALS(errorHandler.parseErrors.size (), 1u);
    TS_ASSERT_EQUALS(errorHandler.parseErrors[0].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedTypeOrIdentifier));

    TS_ASSERT_EQUALS(program->statements.size(), 1u);
    const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[0].get())->functionDecl;
    TS_ASSERT(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
    TS_ASSERT_EQUALS(functionDecl->identifier.GetString(), "main");
    TS_ASSERT_EQUALS(functionDecl->params.size(), 1u);

    {
      const auto& param = functionDecl->params[0];
      TS_ASSERT(param.qualifiers.empty());
      AST_TEST_TYPE_IS_WELL_KNOWN(*param.type, kwInt);
      TS_ASSERT_EQUALS(param.identifier.GetString(), "x");
      TS_ASSERT(!param.defaultValue);
    }

    TS_ASSERT(functionDecl->body->statements.empty());
  }

  void testProgramFunctionDeclInvalid10 (void)
  {
    using namespace s1::parser;

    std::string inStr ("void main(int x,, ) {}");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    ast::ProgramPtr program;
    TS_ASSERT_THROWS_NOTHING(program = astBuilder.ParseProgram ());
    TS_ASSERT_EQUALS(errorHandler.parseErrors.size (), 2u);
    TS_ASSERT_EQUALS(errorHandler.parseErrors[0].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedTypeOrIdentifier));
    TS_ASSERT_EQUALS(errorHandler.parseErrors[1].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedTypeOrIdentifier));

    TS_ASSERT_EQUALS(program->statements.size(), 1u);
    const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[0].get())->functionDecl;
    TS_ASSERT(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
    TS_ASSERT_EQUALS(functionDecl->identifier.GetString(), "main");
    TS_ASSERT_EQUALS(functionDecl->params.size(), 1u);

    {
      const auto& param = functionDecl->params[0];
      TS_ASSERT(param.qualifiers.empty());
      AST_TEST_TYPE_IS_WELL_KNOWN(*param.type, kwInt);
      TS_ASSERT_EQUALS(param.identifier.GetString(), "x");
      TS_ASSERT(!param.defaultValue);
    }

    TS_ASSERT(functionDecl->body->statements.empty());
  }

  void testProgramFunctionDeclInvalid11 (void)
  {
    using namespace s1::parser;

    std::string inStr ("void main(int x {}");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    ast::ProgramPtr program;
    TS_ASSERT_THROWS_NOTHING(program = astBuilder.ParseProgram ());
    TS_ASSERT_EQUALS(errorHandler.parseErrors.size (), 1u);
    TS_ASSERT_EQUALS(errorHandler.parseErrors[0].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedSeparatorOrParenthesis));

    TS_ASSERT_EQUALS(program->statements.size(), 1u);
    const auto& functionDecl = dynamic_cast<const ast::ProgramStatementFunctionDecl*> (program->statements[0].get())->functionDecl;
    TS_ASSERT(boost::get<ast::FunctionDecl::Void> (&functionDecl->resultType));
    TS_ASSERT_EQUALS(functionDecl->identifier.GetString(), "main");
    TS_ASSERT_EQUALS(functionDecl->params.size(), 1u);

    {
      const auto& param = functionDecl->params[0];
      TS_ASSERT(param.qualifiers.empty());
      AST_TEST_TYPE_IS_WELL_KNOWN(*param.type, kwInt);
      TS_ASSERT_EQUALS(param.identifier.GetString(), "x");
      TS_ASSERT(!param.defaultValue);
    }

    TS_ASSERT(functionDecl->body->statements.empty());
  }
};
