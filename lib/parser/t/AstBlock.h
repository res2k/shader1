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
#include "parser/ast/ExprBinary.h"
#include "parser/ast/BlockStatementExpr.h"
#include "parser/ast/BlockStatementFor.h"
#include "parser/ast/BlockStatementIf.h"
#include "parser/ast/BlockStatementNestedBlock.h"
#include "parser/ast/BlockStatementWhile.h"
#include "parser/Diagnostics.h"

#include "AstMacros.h"
#include "TestAstBuilder.h"

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
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::BlockPtr block;
    TS_ASSERT_THROWS_NOTHING((block = astBuilder.ParseBlock ()));
    TS_ASSERT(errorHandler.parseErrors.empty());

    TS_ASSERT_EQUALS(block->statements.size(), 1u);
    const auto& block0Expr = dynamic_cast<const ast::BlockStatementExpr*> (block->statements[0].get())->expr;
    const auto block0ExprBinary = dynamic_cast<const ast::ExprBinary*> (block0Expr.get());
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
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::BlockPtr block;
    TS_ASSERT_THROWS_NOTHING((block = astBuilder.ParseBlock ()));
    TS_ASSERT_EQUALS(errorHandler.parseErrors.size(), 1u);
    TS_ASSERT_EQUALS(errorHandler.parseErrors[0].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedSemicolon));

    TS_ASSERT_EQUALS(block->statements.size(), 1u);
    const auto& block0Expr = dynamic_cast<const ast::BlockStatementExpr*> (block->statements[0].get())->expr;
    const auto block0ExprBinary = dynamic_cast<const ast::ExprBinary*> (block0Expr.get());
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
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::BlockPtr block;
    TS_ASSERT_THROWS_NOTHING((block = astBuilder.ParseBlock ()));
    TS_ASSERT(errorHandler.parseErrors.empty());

    TS_ASSERT_EQUALS(block->statements.size(), 1u);
    const auto block0Branch = dynamic_cast<const ast::BlockStatementIf*> (block->statements[0].get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(block0Branch->condition), "a");

    TS_ASSERT_EQUALS(block0Branch->ifBlock->statements.size(), 1u);
    const auto& block0BranchIf0Expr = dynamic_cast<const ast::BlockStatementExpr*> (block0Branch->ifBlock->statements[0].get())->expr;
    const auto block0BranchIf0ExprBinary = dynamic_cast<const ast::ExprBinary*> (block0BranchIf0Expr.get());
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
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::BlockPtr block;
    TS_ASSERT_THROWS_NOTHING((block = astBuilder.ParseBlock ()));
    TS_ASSERT(errorHandler.parseErrors.empty());

    TS_ASSERT_EQUALS(block->statements.size(), 1u);
    const auto block0Branch = dynamic_cast<const ast::BlockStatementIf*> (block->statements[0].get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(block0Branch->condition), "a");

    TS_ASSERT_EQUALS(block0Branch->ifBlock->statements.size(), 1u);
    const auto& block0BranchIf0Expr = dynamic_cast<const ast::BlockStatementExpr*> (block0Branch->ifBlock->statements[0].get())->expr;
    const auto block0BranchIf0ExprBinary = dynamic_cast<const ast::ExprBinary*> (block0BranchIf0Expr.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(block0BranchIf0ExprBinary->left), "c");
    TS_ASSERT_EQUALS(block0BranchIf0ExprBinary->op.typeOrID, s1::lexer::Assign);
    AST_TEST_EXPR_IS_IDENTIFIER(*(block0BranchIf0ExprBinary->right), "d");

    TS_ASSERT_EQUALS(block0Branch->elseBlock->statements.size(), 1u);
    const auto& block0BranchElse0Expr = dynamic_cast<const ast::BlockStatementExpr*> (block0Branch->elseBlock->statements[0].get())->expr;
    const auto block0BranchElse0ExprBinary = dynamic_cast<const ast::ExprBinary*> (block0BranchElse0Expr.get());
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
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::BlockPtr block;
    TS_ASSERT_THROWS_NOTHING((block = astBuilder.ParseBlock ()));
    TS_ASSERT(errorHandler.parseErrors.empty());

    TS_ASSERT_EQUALS(block->statements.size(), 1u);
    const auto block0While = dynamic_cast<const ast::BlockStatementWhile*> (block->statements[0].get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(block0While->condition), "a");

    TS_ASSERT_EQUALS(block0While->bodyBlock->statements.size(), 1u);
    const auto& block0Body0Expr = dynamic_cast<const ast::BlockStatementExpr*> (block0While->bodyBlock->statements[0].get())->expr;
    const auto block0Body0ExprBinary = dynamic_cast<const ast::ExprBinary*> (block0Body0Expr.get());
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
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::BlockPtr block;
    TS_ASSERT_THROWS_NOTHING((block = astBuilder.ParseBlock ()));
    TS_ASSERT(errorHandler.parseErrors.empty());

    TS_ASSERT_EQUALS(block->statements.size(), 1u);
    const auto block0For = dynamic_cast<const ast::BlockStatementFor*> (block->statements[0].get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(block0For->initExpr), "a");
    AST_TEST_EXPR_IS_IDENTIFIER(*(block0For->condition), "b");

    const auto block0ForFootExprBinary = dynamic_cast<const ast::ExprBinary*> (block0For->footExpr.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(block0ForFootExprBinary->left), "b");
    TS_ASSERT_EQUALS(block0ForFootExprBinary->op.typeOrID, s1::lexer::Assign);
    const auto block0ForFootExprBinaryRight = dynamic_cast<const ast::ExprBinary*> (block0ForFootExprBinary->right.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(block0ForFootExprBinaryRight->left), "b");
    TS_ASSERT_EQUALS(block0ForFootExprBinaryRight->op.typeOrID, s1::lexer::Plus);
    AST_TEST_EXPR_IS_NUMERIC(*(block0ForFootExprBinaryRight->right), int, 1);

    TS_ASSERT_EQUALS(block0For->bodyBlock->statements.size(), 1u);
    const auto& block0Body0Expr = dynamic_cast<const ast::BlockStatementExpr*> (block0For->bodyBlock->statements[0].get())->expr;
    const auto block0Body0ExprBinary = dynamic_cast<const ast::ExprBinary*> (block0Body0Expr.get());
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
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::BlockPtr block;
    TS_ASSERT_THROWS_NOTHING((block = astBuilder.ParseBlock ()));
    TS_ASSERT(errorHandler.parseErrors.empty());

    TS_ASSERT_EQUALS(block->statements.size(), 1u);
    const auto& block0Block = dynamic_cast<const ast::BlockStatementNestedBlock*> (block->statements[0].get())->block;

    TS_ASSERT_EQUALS(block0Block->statements.size(), 1u);
    const auto& block0BlockExpr = dynamic_cast<const ast::BlockStatementExpr*> (block0Block->statements[0].get())->expr;
    const auto block0BlockExprBinary = dynamic_cast<const ast::ExprBinary*> (block0BlockExpr.get());
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
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::BlockPtr block;
    TS_ASSERT_THROWS_NOTHING((block = astBuilder.ParseBlock ()));
    TS_ASSERT_EQUALS(errorHandler.parseErrors.size(), 1u);
    TS_ASSERT_EQUALS(errorHandler.parseErrors[0].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedSemicolon));

    TS_ASSERT_EQUALS(block->statements.size(), 3u);
    const auto& block0Expr = dynamic_cast<const ast::BlockStatementExpr*> (block->statements[0].get())->expr;
    const auto block0ExprBinary = dynamic_cast<const ast::ExprBinary*> (block0Expr.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(block0ExprBinary->left), "a");
    TS_ASSERT_EQUALS(block0ExprBinary->op.typeOrID, s1::lexer::Assign);
    AST_TEST_EXPR_IS_IDENTIFIER(*(block0ExprBinary->right), "b");

    const auto& block1Expr = dynamic_cast<const ast::BlockStatementExpr*> (block->statements[1].get())->expr;
    AST_TEST_EXPR_IS_IDENTIFIER(*block1Expr, "error");

    const auto& block2Expr = dynamic_cast<const ast::BlockStatementExpr*> (block->statements[2].get())->expr;
    const auto block2ExprBinary = dynamic_cast<const ast::ExprBinary*> (block2Expr.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(block2ExprBinary->left), "c");
    TS_ASSERT_EQUALS(block2ExprBinary->op.typeOrID, s1::lexer::Assign);
    AST_TEST_EXPR_IS_IDENTIFIER(*(block2ExprBinary->right), "d");
  }
};
