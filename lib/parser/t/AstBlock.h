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

#include "base/uc/SimpleBufferStreamSource.h"
#include "parser/Exception.h"
#include "parser/Parser.h"

#include "AstMacros.h"
#include "TestParser.h"
#include "TestSemanticsHandler.h"

#include "../../diagnostics/t/TestDiagnosticsHandler.h"

class AstBlockTestSuite : public CxxTest::TestSuite
{
public:
  void testBlockExpr (void)
  {
    using namespace s1::parser;

    std::string inStr ("a = b;");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler; // FIXME: Remove
    TestParser parser (lexer, semanticsHandler, errorHandler);

    s1::parser::ast::BlockPtr block;
    TS_ASSERT_THROWS_NOTHING((block = parser.AstParseBlock ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);

    TS_ASSERT_EQUALS(block->statements.size(), 1u);
    const auto& block0Expr = boost::get<ast::ExprPtr> (block->statements[0]->value);
    const auto& block0ExprBinary = boost::get<ast::ExprBinaryPtr> (block0Expr->value);
    AST_TEST_EXPR_IS_IDENTIFIER(*(block0ExprBinary->left), "a");
    TS_ASSERT_EQUALS(block0ExprBinary->op.typeOrID, s1::lexer::Assign);
    AST_TEST_EXPR_IS_IDENTIFIER(*(block0ExprBinary->right), "b");
  }

  void testBlockStatementIncomplete (void)
  {
    using namespace s1::parser;

    std::string inStr ("a = b");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler; // FIXME: Remove
    TestParser parser (lexer, semanticsHandler, errorHandler);

    s1::parser::ast::BlockPtr block;
    TS_ASSERT_THROWS_NOTHING((block = parser.AstParseBlock ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code,
                     static_cast<unsigned int> (s1::parser::Error::UnexpectedToken));

    TS_ASSERT_EQUALS(block->statements.size(), 1u);
    const auto& block0Expr = boost::get<ast::ExprPtr> (block->statements[0]->value);
    const auto& block0ExprBinary = boost::get<ast::ExprBinaryPtr> (block0Expr->value);
    AST_TEST_EXPR_IS_IDENTIFIER(*(block0ExprBinary->left), "a");
    TS_ASSERT_EQUALS(block0ExprBinary->op.typeOrID, s1::lexer::Assign);
    AST_TEST_EXPR_IS_IDENTIFIER(*(block0ExprBinary->right), "b");
  }

  void testBlockBranch (void)
  {
    using namespace s1::parser;

    std::string inStr ("if (a) { c = d; }");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler; // FIXME: Remove
    TestParser parser (lexer, semanticsHandler, errorHandler);

    s1::parser::ast::BlockPtr block;
    TS_ASSERT_THROWS_NOTHING((block = parser.AstParseBlock ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);

    TS_ASSERT_EQUALS(block->statements.size(), 1u);
    const auto& block0Branch = boost::get<ast::BlockStatementIfPtr> (block->statements[0]->value);
    AST_TEST_EXPR_IS_IDENTIFIER(*(block0Branch->condition), "a");

    TS_ASSERT_EQUALS(block0Branch->ifBlock->statements.size(), 1u);
    const auto& block0BranchIf0Expr = boost::get<ast::ExprPtr> (block0Branch->ifBlock->statements[0]->value);
    const auto& block0BranchIf0ExprBinary = boost::get<ast::ExprBinaryPtr> (block0BranchIf0Expr->value);
    AST_TEST_EXPR_IS_IDENTIFIER(*(block0BranchIf0ExprBinary->left), "c");
    TS_ASSERT_EQUALS(block0BranchIf0ExprBinary->op.typeOrID, s1::lexer::Assign);
    AST_TEST_EXPR_IS_IDENTIFIER(*(block0BranchIf0ExprBinary->right), "d");

    TS_ASSERT(!block0Branch->elseBlock);
  }

  void testBlockBranch2 (void)
  {
    using namespace s1::parser;

    std::string inStr ("if (a) { c = d; } else { c = e; }");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler; // FIXME: Remove
    TestParser parser (lexer, semanticsHandler, errorHandler);

    s1::parser::ast::BlockPtr block;
    TS_ASSERT_THROWS_NOTHING((block = parser.AstParseBlock ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);

    TS_ASSERT_EQUALS(block->statements.size(), 1u);
    const auto& block0Branch = boost::get<ast::BlockStatementIfPtr> (block->statements[0]->value);
    AST_TEST_EXPR_IS_IDENTIFIER(*(block0Branch->condition), "a");

    TS_ASSERT_EQUALS(block0Branch->ifBlock->statements.size(), 1u);
    const auto& block0BranchIf0Expr = boost::get<ast::ExprPtr> (block0Branch->ifBlock->statements[0]->value);
    const auto& block0BranchIf0ExprBinary = boost::get<ast::ExprBinaryPtr> (block0BranchIf0Expr->value);
    AST_TEST_EXPR_IS_IDENTIFIER(*(block0BranchIf0ExprBinary->left), "c");
    TS_ASSERT_EQUALS(block0BranchIf0ExprBinary->op.typeOrID, s1::lexer::Assign);
    AST_TEST_EXPR_IS_IDENTIFIER(*(block0BranchIf0ExprBinary->right), "d");

    TS_ASSERT_EQUALS(block0Branch->elseBlock->statements.size(), 1u);
    const auto& block0BranchElse0Expr = boost::get<ast::ExprPtr> (block0Branch->elseBlock->statements[0]->value);
    const auto& block0BranchElse0ExprBinary = boost::get<ast::ExprBinaryPtr> (block0BranchElse0Expr->value);
    AST_TEST_EXPR_IS_IDENTIFIER(*(block0BranchElse0ExprBinary->left), "c");
    TS_ASSERT_EQUALS(block0BranchElse0ExprBinary->op.typeOrID, s1::lexer::Assign);
    AST_TEST_EXPR_IS_IDENTIFIER(*(block0BranchElse0ExprBinary->right), "e");
  }

  void testBlockWhile (void)
  {
    using namespace s1::parser;

    std::string inStr ("while (a) { c = d; }");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler; // FIXME: Remove
    TestParser parser (lexer, semanticsHandler, errorHandler);

    s1::parser::ast::BlockPtr block;
    TS_ASSERT_THROWS_NOTHING((block = parser.AstParseBlock ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);

    TS_ASSERT_EQUALS(block->statements.size(), 1u);
    const auto& block0While = boost::get<ast::BlockStatementWhilePtr> (block->statements[0]->value);
    AST_TEST_EXPR_IS_IDENTIFIER(*(block0While->condition), "a");

    TS_ASSERT_EQUALS(block0While->bodyBlock->statements.size(), 1u);
    const auto& block0Body0Expr = boost::get<ast::ExprPtr> (block0While->bodyBlock->statements[0]->value);
    const auto& block0Body0ExprBinary = boost::get<ast::ExprBinaryPtr> (block0Body0Expr->value);
    AST_TEST_EXPR_IS_IDENTIFIER(*(block0Body0ExprBinary->left), "c");
    TS_ASSERT_EQUALS(block0Body0ExprBinary->op.typeOrID, s1::lexer::Assign);
    AST_TEST_EXPR_IS_IDENTIFIER(*(block0Body0ExprBinary->right), "d");
  }

  void testBlockFor (void)
  {
    using namespace s1::parser;

    std::string inStr ("for (a; b; b = b+1) { c = d; }");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler; // FIXME: Remove
    TestParser parser (lexer, semanticsHandler, errorHandler);

    s1::parser::ast::BlockPtr block;
    TS_ASSERT_THROWS_NOTHING((block = parser.AstParseBlock ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);

    TS_ASSERT_EQUALS(block->statements.size(), 1u);
    const auto& block0For = boost::get<ast::BlockStatementForPtr> (block->statements[0]->value);
    AST_TEST_EXPR_IS_IDENTIFIER(*(block0For->initExpr), "a");
    AST_TEST_EXPR_IS_IDENTIFIER(*(block0For->condition), "b");

    const auto& block0ForFootExprBinary = boost::get<ast::ExprBinaryPtr> (block0For->footExpr->value);
    AST_TEST_EXPR_IS_IDENTIFIER(*(block0ForFootExprBinary->left), "b");
    TS_ASSERT_EQUALS(block0ForFootExprBinary->op.typeOrID, s1::lexer::Assign);
    const auto& block0ForFootExprBinaryRight = boost::get<ast::ExprBinaryPtr> (block0ForFootExprBinary->right->value);
    AST_TEST_EXPR_IS_IDENTIFIER(*(block0ForFootExprBinaryRight->left), "b");
    TS_ASSERT_EQUALS(block0ForFootExprBinaryRight->op.typeOrID, s1::lexer::Plus);
    AST_TEST_EXPR_IS_NUMERIC(*(block0ForFootExprBinaryRight->right), int, 1);

    TS_ASSERT_EQUALS(block0For->bodyBlock->statements.size(), 1u);
    const auto& block0Body0Expr = boost::get<ast::ExprPtr> (block0For->bodyBlock->statements[0]->value);
    const auto& block0Body0ExprBinary = boost::get<ast::ExprBinaryPtr> (block0Body0Expr->value);
    AST_TEST_EXPR_IS_IDENTIFIER(*(block0Body0ExprBinary->left), "c");
    TS_ASSERT_EQUALS(block0Body0ExprBinary->op.typeOrID, s1::lexer::Assign);
    AST_TEST_EXPR_IS_IDENTIFIER(*(block0Body0ExprBinary->right), "d");
  }

  void testBlockNested (void)
  {
    using namespace s1::parser;

    std::string inStr ("{ a = b; }");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler; // FIXME: Remove
    TestParser parser (lexer, semanticsHandler, errorHandler);

    s1::parser::ast::BlockPtr block;
    TS_ASSERT_THROWS_NOTHING((block = parser.AstParseBlock ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);

    TS_ASSERT_EQUALS(block->statements.size(), 1u);
    const auto& block0Block = boost::get<ast::BlockPtr> (block->statements[0]->value);

    TS_ASSERT_EQUALS(block0Block->statements.size(), 1u);
    const auto& block0BlockExpr = boost::get<ast::ExprPtr> (block0Block->statements[0]->value);
    const auto& block0BlockExprBinary = boost::get<ast::ExprBinaryPtr> (block0BlockExpr->value);
    AST_TEST_EXPR_IS_IDENTIFIER(*(block0BlockExprBinary->left), "a");
    TS_ASSERT_EQUALS(block0BlockExprBinary->op.typeOrID, s1::lexer::Assign);
    AST_TEST_EXPR_IS_IDENTIFIER(*(block0BlockExprBinary->right), "b");
  }

  void testBlockErrorRecovery (void)
  {
    using namespace s1::parser;

    std::string inStr ("a = b error; c=d;");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler; // FIXME: Remove
    TestParser parser (lexer, semanticsHandler, errorHandler);

    s1::parser::ast::BlockPtr block;
    TS_ASSERT_THROWS_NOTHING((block = parser.AstParseBlock ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code,
                     static_cast<unsigned int> (s1::parser::Error::UnexpectedToken));

    TS_ASSERT_EQUALS(block->statements.size(), 2u);
    const auto& block0Expr = boost::get<ast::ExprPtr> (block->statements[0]->value);
    const auto& block0ExprBinary = boost::get<ast::ExprBinaryPtr> (block0Expr->value);
    AST_TEST_EXPR_IS_IDENTIFIER(*(block0ExprBinary->left), "a");
    TS_ASSERT_EQUALS(block0ExprBinary->op.typeOrID, s1::lexer::Assign);
    AST_TEST_EXPR_IS_IDENTIFIER(*(block0ExprBinary->right), "b");

    const auto& block1Expr = boost::get<ast::ExprPtr> (block->statements[1]->value);
    const auto& block1ExprBinary = boost::get<ast::ExprBinaryPtr> (block1Expr->value);
    AST_TEST_EXPR_IS_IDENTIFIER(*(block1ExprBinary->left), "c");
    TS_ASSERT_EQUALS(block1ExprBinary->op.typeOrID, s1::lexer::Assign);
    AST_TEST_EXPR_IS_IDENTIFIER(*(block1ExprBinary->right), "d");
  }
};
