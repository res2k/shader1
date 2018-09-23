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
#include "parser/ast/BlockStatementExpr.h"
#include "parser/ast/BlockStatementTypedef.h"
#include "parser/ast/BlockStatementVarsDecl.h"
#include "parser/ast/ExprBinary.h"
#include "parser/ast/Type.h"
#include "parser/ast/Typedef.h"
#include "parser/ast/VarsDecl.h"
#include "parser/Diagnostics.h"

#include "AstMacros.h"
#include "TestAstBuilder.h"

#include "../../base/t/print_uc_String.h"
#include "../../diagnostics/t/TestDiagnosticsHandler.h"

BOOST_AUTO_TEST_SUITE(AstBlockStrict)

BOOST_AUTO_TEST_CASE(BlockVarDecl)
{
  using namespace s1::parser;

  std::string inStr ("int a;");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::BlockPtr block;
  BOOST_CHECK_NO_THROW((block = astBuilder.ParseBlock ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());

  BOOST_CHECK_EQUAL(block->statements.size(), 1u);
  const auto varsDecl = dynamic_cast<const ast::VarsDecl*> (block->statements[0].get());
  BOOST_CHECK(!varsDecl->isConst);
  AST_TEST_TYPE_IS_WELL_KNOWN(*varsDecl->type, kwInt);

  BOOST_CHECK_EQUAL(varsDecl->vars.size(), 1u);
  BOOST_CHECK_EQUAL(varsDecl->vars[0].identifier.GetString(), "a");
  BOOST_CHECK(!varsDecl->vars[0].initializer);
}

BOOST_AUTO_TEST_CASE(BlockVarDeclMulti)
{
  using namespace s1::parser;

  std::string inStr ("int a, b;");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::BlockPtr block;
  BOOST_CHECK_NO_THROW((block = astBuilder.ParseBlock ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());

  BOOST_CHECK_EQUAL(block->statements.size(), 1u);
  const auto varsDecl = dynamic_cast<const ast::BlockStatementVarsDecl*> (block->statements[0].get());
  BOOST_CHECK(!varsDecl->isConst);
  AST_TEST_TYPE_IS_WELL_KNOWN(*varsDecl->type, kwInt);

  BOOST_CHECK_EQUAL(varsDecl->vars.size(), 2u);
  BOOST_CHECK_EQUAL(varsDecl->vars[0].identifier.GetString(), "a");
  BOOST_CHECK(!varsDecl->vars[0].initializer);
  BOOST_CHECK_EQUAL(varsDecl->vars[1].identifier.GetString(), "b");
  BOOST_CHECK(!varsDecl->vars[1].initializer);
}

BOOST_AUTO_TEST_CASE(BlockVarDeclMulti2)
{
  using namespace s1::parser;

  std::string inStr ("int a; int b;");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::BlockPtr block;
  BOOST_CHECK_NO_THROW((block = astBuilder.ParseBlock ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());

  BOOST_CHECK_EQUAL(block->statements.size(), 2u);
  {
    const auto varsDecl = dynamic_cast<const ast::BlockStatementVarsDecl*> (block->statements[0].get());
    BOOST_CHECK(!varsDecl->isConst);
    AST_TEST_TYPE_IS_WELL_KNOWN(*varsDecl->type, kwInt);

    BOOST_CHECK_EQUAL(varsDecl->vars.size(), 1u);
    BOOST_CHECK_EQUAL(varsDecl->vars[0].identifier.GetString(), "a");
    BOOST_CHECK(!varsDecl->vars[0].initializer);
  }
  {
    const auto varsDecl = dynamic_cast<const ast::BlockStatementVarsDecl*> (block->statements[1].get());
    BOOST_CHECK(!varsDecl->isConst);
    AST_TEST_TYPE_IS_WELL_KNOWN(*varsDecl->type, kwInt);

    BOOST_CHECK_EQUAL(varsDecl->vars.size(), 1u);
    BOOST_CHECK_EQUAL(varsDecl->vars[0].identifier.GetString(), "b");
    BOOST_CHECK(!varsDecl->vars[0].initializer);
  }
}

BOOST_AUTO_TEST_CASE(BlockVarDeclInit)
{
  using namespace s1::parser;

  std::string inStr ("int a = 1+2;");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::BlockPtr block;
  BOOST_CHECK_NO_THROW((block = astBuilder.ParseBlock ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());

  BOOST_CHECK_EQUAL(block->statements.size(), 1u);
  const auto varsDecl = dynamic_cast<const ast::BlockStatementVarsDecl*> (block->statements[0].get());
  BOOST_CHECK(!varsDecl->isConst);
  AST_TEST_TYPE_IS_WELL_KNOWN(*varsDecl->type, kwInt);

  BOOST_CHECK_EQUAL(varsDecl->vars.size(), 1u);
  BOOST_CHECK_EQUAL(varsDecl->vars[0].identifier.GetString(), "a");

  const auto varsDeclInitExprBinary = dynamic_cast<const ast::ExprBinary*> (varsDecl->vars[0].initializer.get());
  AST_TEST_EXPR_IS_NUMERIC(*(varsDeclInitExprBinary->left), int, 1);
  BOOST_CHECK_EQUAL(varsDeclInitExprBinary->op.typeOrID, s1::lexer::Plus);
  AST_TEST_EXPR_IS_NUMERIC(*(varsDeclInitExprBinary->right), int, 2);
}

BOOST_AUTO_TEST_CASE(BlockVarDeclInvalid)
{
  using namespace s1::parser;

  std::string inStr ("int 0;");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::BlockPtr block;
  BOOST_CHECK_NO_THROW((block = astBuilder.ParseBlock ()));
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size(), 2u);
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedExpression));
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[1].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedSemicolon));

  BOOST_CHECK_EQUAL(block->statements.size(), 2u);
  const auto block0Expr = dynamic_cast<const ast::BlockStatementExpr*> (block->statements[0].get());
  BOOST_CHECK(!block0Expr->expr);
  const auto block1Expr = dynamic_cast<const ast::BlockStatementExpr*> (block->statements[1].get());
  AST_TEST_EXPR_IS_NUMERIC(*block1Expr->expr, int, 0);
}

BOOST_AUTO_TEST_CASE(BlockVarDeclInitMulti)
{
  using namespace s1::parser;

  std::string inStr ("int a = 1+2, b;");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::BlockPtr block;
  BOOST_CHECK_NO_THROW((block = astBuilder.ParseBlock ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());

  BOOST_CHECK_EQUAL(block->statements.size(), 1u);
  const auto varsDecl = dynamic_cast<const ast::BlockStatementVarsDecl*> (block->statements[0].get());
  BOOST_CHECK(!varsDecl->isConst);
  AST_TEST_TYPE_IS_WELL_KNOWN(*varsDecl->type, kwInt);

  BOOST_CHECK_EQUAL(varsDecl->vars.size(), 2u);
  BOOST_CHECK_EQUAL(varsDecl->vars[0].identifier.GetString(), "a");

  const auto varsDeclInitExprBinary = dynamic_cast<const ast::ExprBinary*> (varsDecl->vars[0].initializer.get());
  AST_TEST_EXPR_IS_NUMERIC(*(varsDeclInitExprBinary->left), int, 1);
  BOOST_CHECK_EQUAL(varsDeclInitExprBinary->op.typeOrID, s1::lexer::Plus);
  AST_TEST_EXPR_IS_NUMERIC(*(varsDeclInitExprBinary->right), int, 2);

  BOOST_CHECK_EQUAL(varsDecl->vars[1].identifier.GetString(), "b");
  BOOST_CHECK(!varsDecl->vars[1].initializer);
}

BOOST_AUTO_TEST_CASE(BlockVarDeclInitMultiError)
{
  using namespace s1::parser;

  std::string inStr ("int a = 1+2, ;");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::BlockPtr block;
  BOOST_CHECK_NO_THROW((block = astBuilder.ParseBlock ()));
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size(), 1u);
  BOOST_CHECK_EQUAL (errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedIdentifier));

  BOOST_CHECK_EQUAL(block->statements.size(), 1u);
  const auto varsDecl = dynamic_cast<const ast::BlockStatementVarsDecl*> (block->statements[0].get());
  BOOST_CHECK(!varsDecl->isConst);
  AST_TEST_TYPE_IS_WELL_KNOWN(*varsDecl->type, kwInt);

  BOOST_CHECK_EQUAL(varsDecl->vars.size(), 1u);
  BOOST_CHECK_EQUAL(varsDecl->vars[0].identifier.GetString(), "a");

  const auto varsDeclInitExprBinary = dynamic_cast<const ast::ExprBinary*> (varsDecl->vars[0].initializer.get());
  AST_TEST_EXPR_IS_NUMERIC(*(varsDeclInitExprBinary->left), int, 1);
  BOOST_CHECK_EQUAL(varsDeclInitExprBinary->op.typeOrID, s1::lexer::Plus);
  AST_TEST_EXPR_IS_NUMERIC(*(varsDeclInitExprBinary->right), int, 2);
}

BOOST_AUTO_TEST_CASE(BlockConstDecl)
{
  using namespace s1::parser;

  std::string inStr ("const int a = 0;");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::BlockPtr block;
  BOOST_CHECK_NO_THROW((block = astBuilder.ParseBlock ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());

  BOOST_CHECK_EQUAL(block->statements.size(), 1u);
  const auto varsDecl = dynamic_cast<const ast::BlockStatementVarsDecl*> (block->statements[0].get());
  BOOST_CHECK(varsDecl->isConst);
  AST_TEST_TYPE_IS_WELL_KNOWN(*varsDecl->type, kwInt);

  BOOST_CHECK_EQUAL(varsDecl->vars.size(), 1u);
  BOOST_CHECK_EQUAL(varsDecl->vars[0].identifier.GetString(), "a");

  AST_TEST_EXPR_IS_NUMERIC(*(varsDecl->vars[0].initializer), int, 0);
}

BOOST_AUTO_TEST_CASE(BlockConstDeclIncomplete)
{
  using namespace s1::parser;

  std::string inStr ("const int a;");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::BlockPtr block;
  BOOST_CHECK_NO_THROW((block = astBuilder.ParseBlock ()));
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size(), 2u);
  BOOST_CHECK_EQUAL (errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedAssign));
  BOOST_CHECK_EQUAL (errorHandler.parseErrors[1].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedExpression));

  BOOST_CHECK_EQUAL(block->statements.size(), 1u);
  const auto varsDecl = dynamic_cast<const ast::BlockStatementVarsDecl*> (block->statements[0].get());
  BOOST_CHECK(varsDecl->isConst);
  AST_TEST_TYPE_IS_WELL_KNOWN(*varsDecl->type, kwInt);

  // Parsing will produce variable w/o initializer
  BOOST_CHECK_EQUAL(varsDecl->vars.size(), 1u);
  BOOST_CHECK_EQUAL(varsDecl->vars[0].identifier.GetString(), "a");
  BOOST_CHECK(!varsDecl->vars[0].initializer);
}

BOOST_AUTO_TEST_CASE(BlockTypedef)
{
  using namespace s1::parser;

  std::string inStr ("typedef int MyInt;");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::BlockPtr block;
  BOOST_CHECK_NO_THROW((block = astBuilder.ParseBlock ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());

  BOOST_CHECK_EQUAL(block->statements.size(), 1u);
  const auto typeDef = dynamic_cast<const ast::BlockStatementTypedef*> (block->statements[0].get());
  AST_TEST_TYPE_IS_WELL_KNOWN(*typeDef->type, kwInt);
  BOOST_CHECK_EQUAL(typeDef->alias.GetString(), "MyInt");
}

BOOST_AUTO_TEST_CASE(BlockTypedefIndirect)
{
  using namespace s1::parser;

  std::string inStr ("typedef int MyInt1; typedef MyInt1 MyInt;");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::BlockPtr block;
  BOOST_CHECK_NO_THROW((block = astBuilder.ParseBlock ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());

  BOOST_CHECK_EQUAL(block->statements.size(), 2u);
  {
    const auto typeDef = dynamic_cast<const ast::BlockStatementTypedef*> (block->statements[0].get());
    AST_TEST_TYPE_IS_WELL_KNOWN(*typeDef->type, kwInt);
    BOOST_CHECK_EQUAL(typeDef->alias.GetString(), "MyInt1");
  }
  {
    const auto typeDef = dynamic_cast<const ast::BlockStatementTypedef*> (block->statements[1].get());
    AST_TEST_TYPE_IS_ALIAS(*typeDef->type, "MyInt1");
    BOOST_CHECK_EQUAL(typeDef->alias.GetString(), "MyInt");
  }
}

BOOST_AUTO_TEST_CASE(BlockVarTypedefDecl)
{
  using namespace s1::parser;

  std::string inStr ("typedef int MyInt; MyInt a;");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::BlockPtr block;
  BOOST_CHECK_NO_THROW((block = astBuilder.ParseBlock ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());

  BOOST_CHECK_EQUAL(block->statements.size(), 2u);
  {
    const auto typeDef = dynamic_cast<const ast::BlockStatementTypedef*> (block->statements[0].get());
    AST_TEST_TYPE_IS_WELL_KNOWN(*typeDef->type, kwInt);
    BOOST_CHECK_EQUAL(typeDef->alias.GetString(), "MyInt");
  }
  {
    const auto varsDecl = dynamic_cast<const ast::BlockStatementVarsDecl*> (block->statements[1].get());
    BOOST_CHECK(!varsDecl->isConst);
    AST_TEST_TYPE_IS_ALIAS(*varsDecl->type, "MyInt");
    BOOST_CHECK_EQUAL(varsDecl->vars.size(), 1u);
    BOOST_CHECK_EQUAL(varsDecl->vars[0].identifier.GetString(), "a");
    BOOST_CHECK(!varsDecl->vars[0].initializer);
  }
}

BOOST_AUTO_TEST_CASE(BlockTypedefInvalid1)
{
  using namespace s1::parser;

  std::string inStr ("typedef 0 MyInt;");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::BlockPtr block;
  BOOST_CHECK_NO_THROW((block = astBuilder.ParseBlock ()));
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size(), 1u);
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedTypeOrIdentifier));

  BOOST_CHECK_EQUAL(block->statements.size(), 1u);
  const auto typeDef = dynamic_cast<const ast::BlockStatementTypedef*> (block->statements[0].get());
  BOOST_CHECK(!typeDef->type);
  BOOST_CHECK_EQUAL(typeDef->alias.GetString(), "MyInt");
}

BOOST_AUTO_TEST_CASE(BlockTypedefInvalid2)
{
  using namespace s1::parser;

  std::string inStr ("typedef int float;");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::BlockPtr block;
  BOOST_CHECK_NO_THROW((block = astBuilder.ParseBlock ()));
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size(), 1u);
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedIdentifier));

  BOOST_CHECK_EQUAL(block->statements.size(), 1u);
  const auto typeDef = dynamic_cast<const ast::BlockStatementTypedef*> (block->statements[0].get());
  AST_TEST_TYPE_IS_WELL_KNOWN(*typeDef->type, kwInt);
  BOOST_CHECK_EQUAL(typeDef->alias.GetString(), "float");
}

BOOST_AUTO_TEST_CASE(BlockTypedefInvalid3)
{
  using namespace s1::parser;

  std::string inStr ("typedef int;");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::BlockPtr block;
  BOOST_CHECK_NO_THROW((block = astBuilder.ParseBlock ()));
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size(), 1u);
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedIdentifier));

  BOOST_CHECK_EQUAL(block->statements.size(), 1u);
  const auto typeDef = dynamic_cast<const ast::BlockStatementTypedef*> (block->statements[0].get());
  AST_TEST_TYPE_IS_WELL_KNOWN(*typeDef->type, kwInt);
  BOOST_CHECK_EQUAL(typeDef->alias.GetString(), ";");
}

BOOST_AUTO_TEST_SUITE_END()
