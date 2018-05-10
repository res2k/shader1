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
#include "parser/ast/ExprArrayElement.h"
#include "parser/ast/ExprAttribute.h"
#include "parser/ast/ExprFunctionCall.h"
#include "parser/ast/ExprTernary.h"
#include "parser/ast/ExprUnary.h"
#include "parser/ast/Type.h"
#include "parser/ast/TypeArray.h"
#include "parser/Diagnostics.h"

#include "AstMacros.h"
#include "TestAstBuilder.h"

#include "../../diagnostics/t/TestDiagnosticsHandler.h"

class AstExprTestSuite : public CxxTest::TestSuite
{
public:
  void testIdentifier (void)
  {
    using namespace s1::parser;

    std::string inStr ("a");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::ExprPtr expr;
    TS_ASSERT_THROWS_NOTHING((expr = astBuilder.ParseExpression ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    AST_TEST_EXPR_IS_IDENTIFIER(*expr, "a");
  }

  void testIdentifierAttr (void)
  {
    using namespace s1::parser;

    std::string inStr ("a.x");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::ExprPtr expr;
    TS_ASSERT_THROWS_NOTHING((expr = astBuilder.ParseExpression ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    const auto astExprAttr = dynamic_cast<const ast::ExprAttribute*> (expr.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(astExprAttr->value), "a");
    TS_ASSERT_EQUALS(astExprAttr->attr.GetString(), "x");
  }

  void testIdentifierAttr2 (void)
  {
    using namespace s1::parser;

    std::string inStr ("(a+b).x");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::ExprPtr expr;
    TS_ASSERT_THROWS_NOTHING((expr = astBuilder.ParseExpression ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    const auto astExprAttr = dynamic_cast<const ast::ExprAttribute*> (expr.get());
    const auto astLeftBinaryExpr = dynamic_cast<const ast::ExprBinary*> (astExprAttr->value.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(astLeftBinaryExpr->left), "a");
    TS_ASSERT_EQUALS(astLeftBinaryExpr->op.typeOrID, s1::lexer::Plus);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astLeftBinaryExpr->right), "b");
    TS_ASSERT_EQUALS(astExprAttr->attr.GetString(), "x");
  }

  void testIdentifierAttr3 (void)
  {
    using namespace s1::parser;

    std::string inStr ("a.x+b");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::ExprPtr expr;
    TS_ASSERT_THROWS_NOTHING((expr = astBuilder.ParseExpression ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
    const auto astLeftExprAttr = dynamic_cast<const ast::ExprAttribute*> (astBinaryExpr->left.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(astLeftExprAttr->value), "a");
    TS_ASSERT_EQUALS(astLeftExprAttr->attr.GetString(), "x");
    TS_ASSERT_EQUALS(astBinaryExpr->op.typeOrID, s1::lexer::Plus);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->right), "b");
  }

  void testIdentifierAttr4 (void)
  {
    using namespace s1::parser;

    std::string inStr ("a+b.x");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::ExprPtr expr;
    TS_ASSERT_THROWS_NOTHING((expr = astBuilder.ParseExpression ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "a");
    TS_ASSERT_EQUALS(astBinaryExpr->op.typeOrID, s1::lexer::Plus);
    const auto astRightExprAttr = dynamic_cast<const ast::ExprAttribute*> (astBinaryExpr->right.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(astRightExprAttr->value), "b");
    TS_ASSERT_EQUALS(astRightExprAttr->attr.GetString(), "x");
  }

  void testEvaluationOrder (void)
  {
    using namespace s1::parser;

    std::string inStr ("a-b-c");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::ExprPtr expr;
    TS_ASSERT_THROWS_NOTHING((expr = astBuilder.ParseExpression ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
    TS_ASSERT_EQUALS(astBinaryExpr->op.typeOrID, s1::lexer::Minus);
    const auto astLeftBinaryExpr = dynamic_cast<const ast::ExprBinary*> (astBinaryExpr->left.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(astLeftBinaryExpr->left), "a");
    TS_ASSERT_EQUALS(astLeftBinaryExpr->op.typeOrID, s1::lexer::Minus);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astLeftBinaryExpr->right), "b");
    AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->right), "c");
  }

  void testPrecedence1 (void)
  {
    using namespace s1::parser;

    std::string inStr ("a+b*c");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::ExprPtr expr;
    TS_ASSERT_THROWS_NOTHING((expr = astBuilder.ParseExpression ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "a");
    TS_ASSERT_EQUALS(astBinaryExpr->op.typeOrID, s1::lexer::Plus);
    const auto astRightBinaryExpr = dynamic_cast<const ast::ExprBinary*> (astBinaryExpr->right.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(astRightBinaryExpr->left), "b");
    TS_ASSERT_EQUALS(astRightBinaryExpr->op.typeOrID, s1::lexer::Mult);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astRightBinaryExpr->right), "c");
  }

  void testPrecedence2 (void)
  {
    using namespace s1::parser;

    std::string inStr ("a=b=c");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::ExprPtr expr;
    TS_ASSERT_THROWS_NOTHING((expr = astBuilder.ParseExpression ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "a");
    TS_ASSERT_EQUALS(astBinaryExpr->op.typeOrID, s1::lexer::Assign);
    const auto astRightBinaryExpr = dynamic_cast<const ast::ExprBinary*> (astBinaryExpr->right.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(astRightBinaryExpr->left), "b");
    TS_ASSERT_EQUALS(astRightBinaryExpr->op.typeOrID, s1::lexer::Assign);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astRightBinaryExpr->right), "c");
  }

  void testPrecedence3 (void)
  {
    using namespace s1::parser;

    std::string inStr ("a+b==c*d");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::ExprPtr expr;
    TS_ASSERT_THROWS_NOTHING((expr = astBuilder.ParseExpression ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
    const auto astLeftBinaryExpr = dynamic_cast<const ast::ExprBinary*> (astBinaryExpr->left.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(astLeftBinaryExpr->left), "a");
    TS_ASSERT_EQUALS(astLeftBinaryExpr->op.typeOrID, s1::lexer::Plus);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astLeftBinaryExpr->right), "b");
    TS_ASSERT_EQUALS(astBinaryExpr->op.typeOrID, s1::lexer::Equals);
    const auto astRightBinaryExpr = dynamic_cast<const ast::ExprBinary*> (astBinaryExpr->right.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(astRightBinaryExpr->left), "c");
    TS_ASSERT_EQUALS(astRightBinaryExpr->op.typeOrID, s1::lexer::Mult);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astRightBinaryExpr->right), "d");
  }

  void testPrecedence4 (void)
  {
    using namespace s1::parser;

    std::string inStr ("a=b&&c");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::ExprPtr expr;
    TS_ASSERT_THROWS_NOTHING((expr = astBuilder.ParseExpression ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "a");
    TS_ASSERT_EQUALS(astBinaryExpr->op.typeOrID, s1::lexer::Assign);
    const auto astRightBinaryExpr = dynamic_cast<const ast::ExprBinary*> (astBinaryExpr->right.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(astRightBinaryExpr->left), "b");
    TS_ASSERT_EQUALS(astRightBinaryExpr->op.typeOrID, s1::lexer::LogicAnd);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astRightBinaryExpr->right), "c");
  }

  void testPrecedence5 (void)
  {
    using namespace s1::parser;

    std::string inStr ("a&&b||c");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::ExprPtr expr;
    TS_ASSERT_THROWS_NOTHING((expr = astBuilder.ParseExpression ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
    TS_ASSERT_EQUALS(astBinaryExpr->op.typeOrID, s1::lexer::LogicOr);
    const auto astLeftBinaryExpr = dynamic_cast<const ast::ExprBinary*> (astBinaryExpr->left.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(astLeftBinaryExpr->left), "a");
    TS_ASSERT_EQUALS(astLeftBinaryExpr->op.typeOrID, s1::lexer::LogicAnd);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astLeftBinaryExpr->right), "b");
    AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->right), "c");
  }

  void testPrecedence6 (void)
  {
    using namespace s1::parser;

    std::string inStr ("a||b&&c&&d");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::ExprPtr expr;
    TS_ASSERT_THROWS_NOTHING((expr = astBuilder.ParseExpression ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "a");
    TS_ASSERT_EQUALS(astBinaryExpr->op.typeOrID, s1::lexer::LogicOr);
    const auto astRightBinaryExpr = dynamic_cast<const ast::ExprBinary*> (astBinaryExpr->right.get());
    const auto astRightLeftBinaryExpr = dynamic_cast<const ast::ExprBinary*> (astRightBinaryExpr->left.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(astRightLeftBinaryExpr->left), "b");
    TS_ASSERT_EQUALS(astRightLeftBinaryExpr->op.typeOrID, s1::lexer::LogicAnd);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astRightLeftBinaryExpr->right), "c");
    TS_ASSERT_EQUALS(astRightBinaryExpr->op.typeOrID, s1::lexer::LogicAnd);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astRightBinaryExpr->right), "d");
  }

  void testPrecedence7 (void)
  {
    using namespace s1::parser;

    std::string inStr ("!a&&b");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::ExprPtr expr;
    TS_ASSERT_THROWS_NOTHING((expr = astBuilder.ParseExpression ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
    TS_ASSERT_EQUALS(astBinaryExpr->op.typeOrID, s1::lexer::LogicAnd);
    const auto astLeftUnaryExpr = dynamic_cast<const ast::ExprUnary*> (astBinaryExpr->left.get());
    TS_ASSERT_EQUALS(astLeftUnaryExpr->op.typeOrID, s1::lexer::LogicInvert);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astLeftUnaryExpr->right), "a");
    AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->right), "b");
  }

  void testPrecedence8 (void)
  {
    using namespace s1::parser;

    std::string inStr ("a+b>c*d");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::ExprPtr expr;
    TS_ASSERT_THROWS_NOTHING((expr = astBuilder.ParseExpression ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
    TS_ASSERT_EQUALS(astBinaryExpr->op.typeOrID, s1::lexer::Larger);
    const auto astLeftBinaryExpr = dynamic_cast<const ast::ExprBinary*> (astBinaryExpr->left.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(astLeftBinaryExpr->left), "a");
    TS_ASSERT_EQUALS(astLeftBinaryExpr->op.typeOrID, s1::lexer::Plus);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astLeftBinaryExpr->right), "b");
    const auto astRightBinaryExpr = dynamic_cast<const ast::ExprBinary*> (astBinaryExpr->right.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(astRightBinaryExpr->left), "c");
    TS_ASSERT_EQUALS(astRightBinaryExpr->op.typeOrID, s1::lexer::Mult);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astRightBinaryExpr->right), "d");
  }

  void testPrecedence9 (void)
  {
    using namespace s1::parser;

    std::string inStr ("a=b?c:d");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::ExprPtr expr;
    TS_ASSERT_THROWS_NOTHING((expr = astBuilder.ParseExpression ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
    TS_ASSERT_EQUALS(astBinaryExpr->op.typeOrID, s1::lexer::Assign);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "a");
    const auto astRightTernaryExpr = dynamic_cast<const ast::ExprTernary*> (astBinaryExpr->right.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(astRightTernaryExpr->cond), "b");
    AST_TEST_EXPR_IS_IDENTIFIER(*(astRightTernaryExpr->trueExpr), "c");
    AST_TEST_EXPR_IS_IDENTIFIER(*(astRightTernaryExpr->falseExpr), "d");
  }

  void testPrecedence10 (void)
  {
    using namespace s1::parser;

    std::string inStr ("a==b?c:d");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::ExprPtr expr;
    TS_ASSERT_THROWS_NOTHING((expr = astBuilder.ParseExpression ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    const auto astTernaryExpr = dynamic_cast<const ast::ExprTernary*> (expr.get());
    const auto astCondBinaryExpr = dynamic_cast<const ast::ExprBinary*> (astTernaryExpr->cond.get());
    TS_ASSERT_EQUALS(astCondBinaryExpr->op.typeOrID, s1::lexer::Equals);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astCondBinaryExpr->left), "a");
    AST_TEST_EXPR_IS_IDENTIFIER(*(astCondBinaryExpr->right), "b");
    AST_TEST_EXPR_IS_IDENTIFIER(*(astTernaryExpr->trueExpr), "c");
    AST_TEST_EXPR_IS_IDENTIFIER(*(astTernaryExpr->falseExpr), "d");
  }

  void testUnary1 (void)
  {
    using namespace s1::parser;

    std::string inStr ("a+-b");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::ExprPtr expr;
    TS_ASSERT_THROWS_NOTHING((expr = astBuilder.ParseExpression ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
    TS_ASSERT_EQUALS(astBinaryExpr->op.typeOrID, s1::lexer::Plus);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "a");
    const auto astRightUnaryExpr = dynamic_cast<const ast::ExprUnary*> (astBinaryExpr->right.get());
    TS_ASSERT_EQUALS(astRightUnaryExpr->op.typeOrID, s1::lexer::Minus);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astRightUnaryExpr->right), "b");
  }

  void testTernary1 (void)
  {
    using namespace s1::parser;

    std::string inStr ("a?b:c");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::ExprPtr expr;
    TS_ASSERT_THROWS_NOTHING((expr = astBuilder.ParseExpression ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    const auto astTernaryExpr = dynamic_cast<const ast::ExprTernary*> (expr.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(astTernaryExpr->cond), "a");
    AST_TEST_EXPR_IS_IDENTIFIER(*(astTernaryExpr->trueExpr), "b");
    AST_TEST_EXPR_IS_IDENTIFIER(*(astTernaryExpr->falseExpr), "c");
  }

  void testTernary2 (void)
  {
    using namespace s1::parser;

    std::string inStr ("a?b?1:2:c");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::ExprPtr expr;
    TS_ASSERT_THROWS_NOTHING((expr = astBuilder.ParseExpression ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    const auto astTernaryExpr = dynamic_cast<const ast::ExprTernary*> (expr.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(astTernaryExpr->cond), "a");
    const auto astTrueTernaryExpr = dynamic_cast<const ast::ExprTernary*> (astTernaryExpr->trueExpr.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(astTrueTernaryExpr->cond), "b");
    AST_TEST_EXPR_IS_NUMERIC(*(astTrueTernaryExpr->trueExpr), int, 1);
    AST_TEST_EXPR_IS_NUMERIC(*(astTrueTernaryExpr->falseExpr), int, 2);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astTernaryExpr->falseExpr), "c");
  }

  void testTernaryError (void)
  {
    using namespace s1::parser;

    std::string inStr ("a ? b c");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::ExprPtr expr;
    TS_ASSERT_THROWS_NOTHING((expr = astBuilder.ParseExpression ()));
    TS_ASSERT_EQUALS(errorHandler.parseErrors.size(), 1u);
    TS_ASSERT_EQUALS(errorHandler.parseErrors[0].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedTernaryOperator));
    const auto astTernaryExpr = dynamic_cast<const ast::ExprTernary*> (expr.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(astTernaryExpr->cond), "a");
    AST_TEST_EXPR_IS_IDENTIFIER(*(astTernaryExpr->trueExpr), "b");
    AST_TEST_EXPR_IS_IDENTIFIER(*(astTernaryExpr->falseExpr), "c");
  }

  void testTernaryError2 (void)
  {
    using namespace s1::parser;

    std::string inStr ("a ? b");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::ExprPtr expr;
    TS_ASSERT_THROWS_NOTHING((expr = astBuilder.ParseExpression ()));
    TS_ASSERT_EQUALS(errorHandler.parseErrors.size(), 2u);
    TS_ASSERT_EQUALS(errorHandler.parseErrors[0].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedTernaryOperator));
    TS_ASSERT_EQUALS(errorHandler.parseErrors[1].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedExpression));
    const auto astTernaryExpr = dynamic_cast<const ast::ExprTernary*> (expr.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(astTernaryExpr->cond), "a");
    AST_TEST_EXPR_IS_IDENTIFIER(*(astTernaryExpr->trueExpr), "b");
    TS_ASSERT(!astTernaryExpr->falseExpr);
  }

  void testTernaryError3 (void)
  {
    using namespace s1::parser;

    std::string inStr ("a ? : c");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::ExprPtr expr;
    TS_ASSERT_THROWS_NOTHING((expr = astBuilder.ParseExpression ()));
    TS_ASSERT_EQUALS(errorHandler.parseErrors.size(), 1u);
    TS_ASSERT_EQUALS(errorHandler.parseErrors[0].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedExpression));
    const auto astTernaryExpr = dynamic_cast<const ast::ExprTernary*> (expr.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(astTernaryExpr->cond), "a");
    TS_ASSERT(!astTernaryExpr->trueExpr);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astTernaryExpr->falseExpr), "c");
  }

  void testParentheses1 (void)
  {
    using namespace s1::parser;

    std::string inStr ("(a)");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::ExprPtr expr;
    TS_ASSERT_THROWS_NOTHING((expr = astBuilder.ParseExpression ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    AST_TEST_EXPR_IS_IDENTIFIER(*expr, "a");
  }

  void testParentheses2 (void)
  {
    using namespace s1::parser;

    std::string inStr ("((((a))))");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::ExprPtr expr;
    TS_ASSERT_THROWS_NOTHING((expr = astBuilder.ParseExpression ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    AST_TEST_EXPR_IS_IDENTIFIER(*expr, "a");
  }

  void testParentheses3 (void)
  {
    using namespace s1::parser;

    std::string inStr ("a*(b+c)");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::ExprPtr expr;
    TS_ASSERT_THROWS_NOTHING((expr = astBuilder.ParseExpression ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "a");
    TS_ASSERT_EQUALS(astBinaryExpr->op.typeOrID, s1::lexer::Mult);
    const auto astRightBinaryExpr = dynamic_cast<const ast::ExprBinary*> (astBinaryExpr->right.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(astRightBinaryExpr->left), "b");
    TS_ASSERT_EQUALS(astRightBinaryExpr->op.typeOrID, s1::lexer::Plus);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astRightBinaryExpr->right), "c");
  }

  void testIdentifierArray1 (void)
  {
    using namespace s1::parser;

    std::string inStr ("a[1]");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::ExprPtr expr;
    TS_ASSERT_THROWS_NOTHING((expr = astBuilder.ParseExpression ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    const auto astArrayElementExpr = dynamic_cast<const ast::ExprArrayElement*> (expr.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(astArrayElementExpr->value), "a");
    AST_TEST_EXPR_IS_NUMERIC(*(astArrayElementExpr->index), int, 1);
  }

  void testIdentifierArray2 (void)
  {
    using namespace s1::parser;

    std::string inStr ("a[b+c]");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::ExprPtr expr;
    TS_ASSERT_THROWS_NOTHING((expr = astBuilder.ParseExpression ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    const auto astArrayElementExpr = dynamic_cast<const ast::ExprArrayElement*> (expr.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(astArrayElementExpr->value), "a");
    const auto astIndexBinaryExpr = dynamic_cast<const ast::ExprBinary*> (astArrayElementExpr->index.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(astIndexBinaryExpr->left), "b");
    TS_ASSERT_EQUALS(astIndexBinaryExpr->op.typeOrID, s1::lexer::Plus);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astIndexBinaryExpr->right), "c");
  }

  void testIdentifierArray3 (void)
  {
    using namespace s1::parser;

    std::string inStr ("(a+b)[1]+c");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::ExprPtr expr;
    TS_ASSERT_THROWS_NOTHING((expr = astBuilder.ParseExpression ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
    const auto astLeftArrayElementExpr = dynamic_cast<const ast::ExprArrayElement*> (astBinaryExpr->left.get());
    const auto astLeftArrayElementBinaryExpr = dynamic_cast<const ast::ExprBinary*> (astLeftArrayElementExpr->value.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(astLeftArrayElementBinaryExpr->left), "a");
    TS_ASSERT_EQUALS(astLeftArrayElementBinaryExpr->op.typeOrID, s1::lexer::Plus);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astLeftArrayElementBinaryExpr->right), "b");
    AST_TEST_EXPR_IS_NUMERIC(*(astLeftArrayElementExpr->index), int, 1);
    TS_ASSERT_EQUALS(astBinaryExpr->op.typeOrID, s1::lexer::Plus);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->right), "c");
  }

  void testFunctionCall (void)
  {
    using namespace s1::parser;

    std::string inStr ("x = Foo ()");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::ExprPtr expr;
    TS_ASSERT_THROWS_NOTHING((expr = astBuilder.ParseExpression ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "x");
    TS_ASSERT_EQUALS(astBinaryExpr->op.typeOrID, s1::lexer::Assign);
    const auto astRightFuncCallExpr = dynamic_cast<const ast::ExprFunctionCall*> (astBinaryExpr->right.get());
    const auto& funcIdent = boost::get<ast::Identifier> (astRightFuncCallExpr->identifierOrType);
    TS_ASSERT_EQUALS(funcIdent.GetString(), "Foo");
    TS_ASSERT(astRightFuncCallExpr->args.empty());
  }

  void testFunctionCall2 (void)
  {
    using namespace s1::parser;

    std::string inStr ("x = Foo (a + b, 3.0)");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::ExprPtr expr;
    TS_ASSERT_THROWS_NOTHING((expr = astBuilder.ParseExpression ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "x");
    TS_ASSERT_EQUALS(astBinaryExpr->op.typeOrID, s1::lexer::Assign);
    const auto astRightFuncCallExpr = dynamic_cast<const ast::ExprFunctionCall*> (astBinaryExpr->right.get());
    const auto& funcIdent = boost::get<ast::Identifier> (astRightFuncCallExpr->identifierOrType);
    TS_ASSERT_EQUALS(funcIdent.GetString(), "Foo");
    TS_ASSERT_EQUALS(astRightFuncCallExpr->args.size(), 2u);
    const auto astArg0BinaryExpr = dynamic_cast<const ast::ExprBinary*> (astRightFuncCallExpr->args[0].get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(astArg0BinaryExpr->left), "a");
    TS_ASSERT_EQUALS(astArg0BinaryExpr->op.typeOrID, s1::lexer::Plus);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astArg0BinaryExpr->right), "b");
    AST_TEST_EXPR_IS_NUMERIC(*(astRightFuncCallExpr->args[1]), float, 3.0);
  }

  void testFunctionCallError (void)
  {
    using namespace s1::parser;

    std::string inStr ("x = Foo (4.0");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::ExprPtr expr;
    TS_ASSERT_THROWS_NOTHING((expr = astBuilder.ParseExpression ()));
    TS_ASSERT_EQUALS(errorHandler.parseErrors.size(), 1u);
    TS_ASSERT_EQUALS(errorHandler.parseErrors[0].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedSeparatorOrParenthesis));
    const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "x");
    TS_ASSERT_EQUALS(astBinaryExpr->op.typeOrID, s1::lexer::Assign);
    const auto astRightFuncCallExpr = dynamic_cast<const ast::ExprFunctionCall*> (astBinaryExpr->right.get());
    const auto& funcIdent = boost::get<ast::Identifier> (astRightFuncCallExpr->identifierOrType);
    TS_ASSERT_EQUALS(funcIdent.GetString(), "Foo");
    TS_ASSERT_EQUALS(astRightFuncCallExpr->args.size(), 1u);
    AST_TEST_EXPR_IS_NUMERIC(*(astRightFuncCallExpr->args[0]), float, 4.0);
  }

  void testTypeCtor (void)
  {
    using namespace s1::parser;

    std::string inStr ("x = int ()");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::ExprPtr expr;
    TS_ASSERT_THROWS_NOTHING((expr = astBuilder.ParseExpression ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "x");
    TS_ASSERT_EQUALS(astBinaryExpr->op.typeOrID, s1::lexer::Assign);
    const auto astRightFuncCallExpr = dynamic_cast<const ast::ExprFunctionCall*> (astBinaryExpr->right.get());
    const auto& funcType = boost::get<ast::TypePtr> (astRightFuncCallExpr->identifierOrType);
    AST_TEST_TYPE_IS_WELL_KNOWN(*funcType, kwInt);
    TS_ASSERT(astRightFuncCallExpr->args.empty());
  }

  void testTypeCtor2 (void)
  {
    using namespace s1::parser;

    std::string inStr ("x = int2 (1, 2)");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::ExprPtr expr;
    TS_ASSERT_THROWS_NOTHING((expr = astBuilder.ParseExpression ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "x");
    TS_ASSERT_EQUALS(astBinaryExpr->op.typeOrID, s1::lexer::Assign);
    const auto astRightFuncCallExpr = dynamic_cast<const ast::ExprFunctionCall*> (astBinaryExpr->right.get());
    const auto& funcType = boost::get<ast::TypePtr> (astRightFuncCallExpr->identifierOrType);
    AST_TEST_TYPE_IS_WELL_KNOWN_VEC(*funcType, kwInt, 2);
    AST_TEST_EXPR_IS_NUMERIC(*(astRightFuncCallExpr->args[0]), int, 1);
    AST_TEST_EXPR_IS_NUMERIC(*(astRightFuncCallExpr->args[1]), int, 2);
  }

  void testArrayCtor (void)
  {
    using namespace s1::parser;

    std::string inStr ("x = float[] (1.0, 2.0, 3.0)");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::ExprPtr expr;
    TS_ASSERT_THROWS_NOTHING((expr = astBuilder.ParseExpression ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "x");
    TS_ASSERT_EQUALS(astBinaryExpr->op.typeOrID, s1::lexer::Assign);
    const auto astRightFuncCallExpr = dynamic_cast<const ast::ExprFunctionCall*> (astBinaryExpr->right.get());
    const auto& funcType = boost::get<ast::TypePtr> (astRightFuncCallExpr->identifierOrType);
    const auto funcTypeArray = dynamic_cast<const ast::TypeArray*> (funcType.get());
    AST_TEST_TYPE_IS_WELL_KNOWN(*funcTypeArray->containedType, kwFloat);
    AST_TEST_EXPR_IS_NUMERIC(*(astRightFuncCallExpr->args[0]), float, 1.0);
    AST_TEST_EXPR_IS_NUMERIC(*(astRightFuncCallExpr->args[1]), float, 2.0);
    AST_TEST_EXPR_IS_NUMERIC(*(astRightFuncCallExpr->args[2]), float, 3.0);
  }

  void testExprInvalid1 (void)
  {
    using namespace s1::parser;

    std::string inStr ("a+");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::ExprPtr expr;
    TS_ASSERT_THROWS_NOTHING((expr = astBuilder.ParseExpression ()));
    TS_ASSERT_EQUALS(errorHandler.parseErrors.size(), 1u);
    TS_ASSERT_EQUALS(errorHandler.parseErrors[0].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedExpression));
    const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
    AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "a");
    TS_ASSERT_EQUALS(astBinaryExpr->op.typeOrID, s1::lexer::Plus);
    TS_ASSERT(!astBinaryExpr->right);
  }
};
