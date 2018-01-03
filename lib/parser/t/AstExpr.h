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
#include "parser/ast/Type.h"

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
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
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
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    const auto& astExprAttr = boost::get<ast::ExprAttributePtr> ((*expr).value);
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
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    const auto& astExprAttr = boost::get<ast::ExprAttributePtr> ((*expr).value);
    const auto& astLeftBinaryExpr = boost::get<ast::ExprBinaryPtr> ((*astExprAttr->value).value);
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
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    const auto& astBinaryExpr = boost::get<ast::ExprBinaryPtr> ((*expr).value);
    const auto& astLeftExprAttr = boost::get<ast::ExprAttributePtr> ((*(astBinaryExpr->left)).value);
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
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    const auto& astBinaryExpr = boost::get<ast::ExprBinaryPtr> ((*expr).value);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "a");
    TS_ASSERT_EQUALS(astBinaryExpr->op.typeOrID, s1::lexer::Plus);
    const auto& astRightExprAttr = boost::get<ast::ExprAttributePtr> ((*(astBinaryExpr->right)).value);
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
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    const auto& astBinaryExpr = boost::get<ast::ExprBinaryPtr> ((*expr).value);
    TS_ASSERT_EQUALS(astBinaryExpr->op.typeOrID, s1::lexer::Minus);
    const auto& astLeftBinaryExpr = boost::get<ast::ExprBinaryPtr> ((*(astBinaryExpr->left)).value);
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
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    const auto& astBinaryExpr = boost::get<ast::ExprBinaryPtr> ((*expr).value);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "a");
    TS_ASSERT_EQUALS(astBinaryExpr->op.typeOrID, s1::lexer::Plus);
    const auto& astRightBinaryExpr = boost::get<ast::ExprBinaryPtr> ((*(astBinaryExpr->right)).value);
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
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    const auto& astBinaryExpr = boost::get<ast::ExprBinaryPtr> ((*expr).value);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "a");
    TS_ASSERT_EQUALS(astBinaryExpr->op.typeOrID, s1::lexer::Assign);
    const auto& astRightBinaryExpr = boost::get<ast::ExprBinaryPtr> ((*(astBinaryExpr->right)).value);
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
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    const auto& astBinaryExpr = boost::get<ast::ExprBinaryPtr> ((*expr).value);
    const auto& astLeftBinaryExpr = boost::get<ast::ExprBinaryPtr> ((*(astBinaryExpr->left)).value);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astLeftBinaryExpr->left), "a");
    TS_ASSERT_EQUALS(astLeftBinaryExpr->op.typeOrID, s1::lexer::Plus);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astLeftBinaryExpr->right), "b");
    TS_ASSERT_EQUALS(astBinaryExpr->op.typeOrID, s1::lexer::Equals);
    const auto& astRightBinaryExpr = boost::get<ast::ExprBinaryPtr> ((*(astBinaryExpr->right)).value);
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
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    const auto& astBinaryExpr = boost::get<ast::ExprBinaryPtr> ((*expr).value);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "a");
    TS_ASSERT_EQUALS(astBinaryExpr->op.typeOrID, s1::lexer::Assign);
    const auto& astRightBinaryExpr = boost::get<ast::ExprBinaryPtr> ((*(astBinaryExpr->right)).value);
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
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    const auto& astBinaryExpr = boost::get<ast::ExprBinaryPtr> ((*expr).value);
    TS_ASSERT_EQUALS(astBinaryExpr->op.typeOrID, s1::lexer::LogicOr);
    const auto& astLeftBinaryExpr = boost::get<ast::ExprBinaryPtr> (astBinaryExpr->left->value);
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
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    const auto& astBinaryExpr = boost::get<ast::ExprBinaryPtr> ((*expr).value);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "a");
    TS_ASSERT_EQUALS(astBinaryExpr->op.typeOrID, s1::lexer::LogicOr);
    const auto& astRightBinaryExpr = boost::get<ast::ExprBinaryPtr> (astBinaryExpr->right->value);
    const auto& astRightLeftBinaryExpr = boost::get<ast::ExprBinaryPtr> (astRightBinaryExpr->left->value);
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
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    const auto& astBinaryExpr = boost::get<ast::ExprBinaryPtr> ((*expr).value);
    TS_ASSERT_EQUALS(astBinaryExpr->op.typeOrID, s1::lexer::LogicAnd);
    const auto& astLeftUnaryExpr = boost::get<ast::ExprUnaryPtr> (astBinaryExpr->left->value);
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
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    const auto& astBinaryExpr = boost::get<ast::ExprBinaryPtr> ((*expr).value);
    TS_ASSERT_EQUALS(astBinaryExpr->op.typeOrID, s1::lexer::Larger);
    const auto& astLeftBinaryExpr = boost::get<ast::ExprBinaryPtr> (astBinaryExpr->left->value);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astLeftBinaryExpr->left), "a");
    TS_ASSERT_EQUALS(astLeftBinaryExpr->op.typeOrID, s1::lexer::Plus);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astLeftBinaryExpr->right), "b");
    const auto& astRightBinaryExpr = boost::get<ast::ExprBinaryPtr> (astBinaryExpr->right->value);
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
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    const auto& astBinaryExpr = boost::get<ast::ExprBinaryPtr> ((*expr).value);
    TS_ASSERT_EQUALS(astBinaryExpr->op.typeOrID, s1::lexer::Assign);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "a");
    const auto& astRightTernaryExpr = boost::get<ast::ExprTernaryPtr> (astBinaryExpr->right->value);
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
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    const auto& astTernaryExpr = boost::get<ast::ExprTernaryPtr> ((*expr).value);
    const auto& astCondBinaryExpr = boost::get<ast::ExprBinaryPtr> (astTernaryExpr->cond->value);
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
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    const auto& astBinaryExpr = boost::get<ast::ExprBinaryPtr> ((*expr).value);
    TS_ASSERT_EQUALS(astBinaryExpr->op.typeOrID, s1::lexer::Plus);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "a");
    const auto& astRightUnaryExpr = boost::get<ast::ExprUnaryPtr> (astBinaryExpr->right->value);
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
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    const auto& astTernaryExpr = boost::get<ast::ExprTernaryPtr> ((*expr).value);
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
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    const auto& astTernaryExpr = boost::get<ast::ExprTernaryPtr> ((*expr).value);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astTernaryExpr->cond), "a");
    const auto& astTrueTernaryExpr = boost::get<ast::ExprTernaryPtr> (astTernaryExpr->trueExpr->value);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astTrueTernaryExpr->cond), "b");
    AST_TEST_EXPR_IS_NUMERIC(*(astTrueTernaryExpr->trueExpr), int, 1);
    AST_TEST_EXPR_IS_NUMERIC(*(astTrueTernaryExpr->falseExpr), int, 2);
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
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
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
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
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
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    const auto& astBinaryExpr = boost::get<ast::ExprBinaryPtr> ((*expr).value);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "a");
    TS_ASSERT_EQUALS(astBinaryExpr->op.typeOrID, s1::lexer::Mult);
    const auto& astRightBinaryExpr = boost::get<ast::ExprBinaryPtr> ((*(astBinaryExpr->right)).value);
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
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    const auto& astArrayElementExpr = boost::get<ast::ExprArrayElementPtr> ((*expr).value);
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
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    const auto& astArrayElementExpr = boost::get<ast::ExprArrayElementPtr> ((*expr).value);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astArrayElementExpr->value), "a");
    const auto& astIndexBinaryExpr = boost::get<ast::ExprBinaryPtr> (astArrayElementExpr->index->value);
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
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    const auto& astBinaryExpr = boost::get<ast::ExprBinaryPtr> ((*expr).value);
    const auto& astLeftArrayElementExpr = boost::get<ast::ExprArrayElementPtr> (astBinaryExpr->left->value);
    const auto& astLeftArrayElementBinaryExpr = boost::get<ast::ExprBinaryPtr> (astLeftArrayElementExpr->value->value);
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
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    const auto& astBinaryExpr = boost::get<ast::ExprBinaryPtr> ((*expr).value);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "x");
    TS_ASSERT_EQUALS(astBinaryExpr->op.typeOrID, s1::lexer::Assign);
    const auto& astRightFuncCallExpr = boost::get<ast::ExprFunctionCallPtr> (astBinaryExpr->right->value);
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
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    const auto& astBinaryExpr = boost::get<ast::ExprBinaryPtr> ((*expr).value);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "x");
    TS_ASSERT_EQUALS(astBinaryExpr->op.typeOrID, s1::lexer::Assign);
    const auto& astRightFuncCallExpr = boost::get<ast::ExprFunctionCallPtr> (astBinaryExpr->right->value);
    const auto& funcIdent = boost::get<ast::Identifier> (astRightFuncCallExpr->identifierOrType);
    TS_ASSERT_EQUALS(funcIdent.GetString(), "Foo");
    TS_ASSERT_EQUALS(astRightFuncCallExpr->args.size(), 2u);
    const auto& astArg0BinaryExpr = boost::get<ast::ExprBinaryPtr> (astRightFuncCallExpr->args[0]->value);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astArg0BinaryExpr->left), "a");
    TS_ASSERT_EQUALS(astArg0BinaryExpr->op.typeOrID, s1::lexer::Plus);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astArg0BinaryExpr->right), "b");
    AST_TEST_EXPR_IS_NUMERIC(*(astRightFuncCallExpr->args[1]), float, 3.0);
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
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    const auto& astBinaryExpr = boost::get<ast::ExprBinaryPtr> ((*expr).value);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "x");
    TS_ASSERT_EQUALS(astBinaryExpr->op.typeOrID, s1::lexer::Assign);
    const auto& astRightFuncCallExpr = boost::get<ast::ExprFunctionCallPtr> (astBinaryExpr->right->value);
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
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    const auto& astBinaryExpr = boost::get<ast::ExprBinaryPtr> ((*expr).value);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "x");
    TS_ASSERT_EQUALS(astBinaryExpr->op.typeOrID, s1::lexer::Assign);
    const auto& astRightFuncCallExpr = boost::get<ast::ExprFunctionCallPtr> (astBinaryExpr->right->value);
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
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    const auto& astBinaryExpr = boost::get<ast::ExprBinaryPtr> ((*expr).value);
    AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "x");
    TS_ASSERT_EQUALS(astBinaryExpr->op.typeOrID, s1::lexer::Assign);
    const auto& astRightFuncCallExpr = boost::get<ast::ExprFunctionCallPtr> (astBinaryExpr->right->value);
    const auto& funcType = boost::get<ast::TypePtr> (astRightFuncCallExpr->identifierOrType);
    const auto& funcTypeArray = boost::get<ast::Type::ArrayType> (funcType->value);
    AST_TEST_TYPE_IS_WELL_KNOWN(*funcTypeArray.containedType, kwFloat);
    AST_TEST_EXPR_IS_NUMERIC(*(astRightFuncCallExpr->args[0]), float, 1.0);
    AST_TEST_EXPR_IS_NUMERIC(*(astRightFuncCallExpr->args[1]), float, 2.0);
    AST_TEST_EXPR_IS_NUMERIC(*(astRightFuncCallExpr->args[2]), float, 3.0);
  }
};
