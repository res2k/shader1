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
#include "parser/ast/ExprArrayElement.h"
#include "parser/ast/ExprAttribute.h"
#include "parser/ast/ExprBinary.h"
#include "parser/ast/ExprFunctionCall.h"
#include "parser/ast/ExprTernary.h"
#include "parser/ast/ExprUnary.h"
#include "parser/ast/Type.h"
#include "parser/ast/TypeArray.h"
#include "parser/Diagnostics.h"

#include "AstMacros.h"
#include "TestAstBuilder.h"

#include "../../base/t/print_uc_String.h"
#include "../../diagnostics/t/TestDiagnosticsHandler.h"

BOOST_AUTO_TEST_SUITE(AstExpr)

BOOST_AUTO_TEST_CASE(Identifier)
{
  using namespace s1::parser;

  std::string inStr ("a");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::ExprPtr expr;
  BOOST_CHECK_NO_THROW((expr = astBuilder.ParseExpression ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  AST_TEST_EXPR_IS_IDENTIFIER(*expr, "a");
}

BOOST_AUTO_TEST_CASE(IdentifierAttr)
{
  using namespace s1::parser;

  std::string inStr ("a.x");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::ExprPtr expr;
  BOOST_CHECK_NO_THROW((expr = astBuilder.ParseExpression ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  const auto astExprAttr = dynamic_cast<const ast::ExprAttribute*> (expr.get());
  AST_TEST_EXPR_IS_IDENTIFIER(*(astExprAttr->value), "a");
  BOOST_CHECK_EQUAL(astExprAttr->attr.GetString(), "x");
}

BOOST_AUTO_TEST_CASE(IdentifierAttr2)
{
  using namespace s1::parser;

  std::string inStr ("(a+b).x");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::ExprPtr expr;
  BOOST_CHECK_NO_THROW((expr = astBuilder.ParseExpression ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  const auto astExprAttr = dynamic_cast<const ast::ExprAttribute*> (expr.get());
  const auto astLeftBinaryExpr = dynamic_cast<const ast::ExprBinary*> (astExprAttr->value.get());
  AST_TEST_EXPR_IS_IDENTIFIER(*(astLeftBinaryExpr->left), "a");
  BOOST_CHECK_EQUAL(astLeftBinaryExpr->op.typeOrID, s1::lexer::Plus);
  AST_TEST_EXPR_IS_IDENTIFIER(*(astLeftBinaryExpr->right), "b");
  BOOST_CHECK_EQUAL(astExprAttr->attr.GetString(), "x");
}

BOOST_AUTO_TEST_CASE(IdentifierAttr3)
{
  using namespace s1::parser;

  std::string inStr ("a.x+b");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::ExprPtr expr;
  BOOST_CHECK_NO_THROW((expr = astBuilder.ParseExpression ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
  const auto astLeftExprAttr = dynamic_cast<const ast::ExprAttribute*> (astBinaryExpr->left.get());
  AST_TEST_EXPR_IS_IDENTIFIER(*(astLeftExprAttr->value), "a");
  BOOST_CHECK_EQUAL(astLeftExprAttr->attr.GetString(), "x");
  BOOST_CHECK_EQUAL(astBinaryExpr->op.typeOrID, s1::lexer::Plus);
  AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->right), "b");
}

BOOST_AUTO_TEST_CASE(IdentifierAttr4)
{
  using namespace s1::parser;

  std::string inStr ("a+b.x");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::ExprPtr expr;
  BOOST_CHECK_NO_THROW((expr = astBuilder.ParseExpression ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
  AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "a");
  BOOST_CHECK_EQUAL(astBinaryExpr->op.typeOrID, s1::lexer::Plus);
  const auto astRightExprAttr = dynamic_cast<const ast::ExprAttribute*> (astBinaryExpr->right.get());
  AST_TEST_EXPR_IS_IDENTIFIER(*(astRightExprAttr->value), "b");
  BOOST_CHECK_EQUAL(astRightExprAttr->attr.GetString(), "x");
}

BOOST_AUTO_TEST_CASE(EvaluationOrder)
{
  using namespace s1::parser;

  std::string inStr ("a-b-c");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::ExprPtr expr;
  BOOST_CHECK_NO_THROW((expr = astBuilder.ParseExpression ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
  BOOST_CHECK_EQUAL(astBinaryExpr->op.typeOrID, s1::lexer::Minus);
  const auto astLeftBinaryExpr = dynamic_cast<const ast::ExprBinary*> (astBinaryExpr->left.get());
  AST_TEST_EXPR_IS_IDENTIFIER(*(astLeftBinaryExpr->left), "a");
  BOOST_CHECK_EQUAL(astLeftBinaryExpr->op.typeOrID, s1::lexer::Minus);
  AST_TEST_EXPR_IS_IDENTIFIER(*(astLeftBinaryExpr->right), "b");
  AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->right), "c");
}

BOOST_AUTO_TEST_CASE(Precedence1)
{
  using namespace s1::parser;

  std::string inStr ("a+b*c");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::ExprPtr expr;
  BOOST_CHECK_NO_THROW((expr = astBuilder.ParseExpression ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
  AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "a");
  BOOST_CHECK_EQUAL(astBinaryExpr->op.typeOrID, s1::lexer::Plus);
  const auto astRightBinaryExpr = dynamic_cast<const ast::ExprBinary*> (astBinaryExpr->right.get());
  AST_TEST_EXPR_IS_IDENTIFIER(*(astRightBinaryExpr->left), "b");
  BOOST_CHECK_EQUAL(astRightBinaryExpr->op.typeOrID, s1::lexer::Mult);
  AST_TEST_EXPR_IS_IDENTIFIER(*(astRightBinaryExpr->right), "c");
}

BOOST_AUTO_TEST_CASE(Precedence2)
{
  using namespace s1::parser;

  std::string inStr ("a=b=c");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::ExprPtr expr;
  BOOST_CHECK_NO_THROW((expr = astBuilder.ParseExpression ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
  AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "a");
  BOOST_CHECK_EQUAL(astBinaryExpr->op.typeOrID, s1::lexer::Assign);
  const auto astRightBinaryExpr = dynamic_cast<const ast::ExprBinary*> (astBinaryExpr->right.get());
  AST_TEST_EXPR_IS_IDENTIFIER(*(astRightBinaryExpr->left), "b");
  BOOST_CHECK_EQUAL(astRightBinaryExpr->op.typeOrID, s1::lexer::Assign);
  AST_TEST_EXPR_IS_IDENTIFIER(*(astRightBinaryExpr->right), "c");
}

BOOST_AUTO_TEST_CASE(Precedence3)
{
  using namespace s1::parser;

  std::string inStr ("a+b==c*d");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::ExprPtr expr;
  BOOST_CHECK_NO_THROW((expr = astBuilder.ParseExpression ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
  const auto astLeftBinaryExpr = dynamic_cast<const ast::ExprBinary*> (astBinaryExpr->left.get());
  AST_TEST_EXPR_IS_IDENTIFIER(*(astLeftBinaryExpr->left), "a");
  BOOST_CHECK_EQUAL(astLeftBinaryExpr->op.typeOrID, s1::lexer::Plus);
  AST_TEST_EXPR_IS_IDENTIFIER(*(astLeftBinaryExpr->right), "b");
  BOOST_CHECK_EQUAL(astBinaryExpr->op.typeOrID, s1::lexer::Equals);
  const auto astRightBinaryExpr = dynamic_cast<const ast::ExprBinary*> (astBinaryExpr->right.get());
  AST_TEST_EXPR_IS_IDENTIFIER(*(astRightBinaryExpr->left), "c");
  BOOST_CHECK_EQUAL(astRightBinaryExpr->op.typeOrID, s1::lexer::Mult);
  AST_TEST_EXPR_IS_IDENTIFIER(*(astRightBinaryExpr->right), "d");
}

BOOST_AUTO_TEST_CASE(Precedence4)
{
  using namespace s1::parser;

  std::string inStr ("a=b&&c");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::ExprPtr expr;
  BOOST_CHECK_NO_THROW((expr = astBuilder.ParseExpression ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
  AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "a");
  BOOST_CHECK_EQUAL(astBinaryExpr->op.typeOrID, s1::lexer::Assign);
  const auto astRightBinaryExpr = dynamic_cast<const ast::ExprBinary*> (astBinaryExpr->right.get());
  AST_TEST_EXPR_IS_IDENTIFIER(*(astRightBinaryExpr->left), "b");
  BOOST_CHECK_EQUAL(astRightBinaryExpr->op.typeOrID, s1::lexer::LogicAnd);
  AST_TEST_EXPR_IS_IDENTIFIER(*(astRightBinaryExpr->right), "c");
}

BOOST_AUTO_TEST_CASE(Precedence5)
{
  using namespace s1::parser;

  std::string inStr ("a&&b||c");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::ExprPtr expr;
  BOOST_CHECK_NO_THROW((expr = astBuilder.ParseExpression ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
  BOOST_CHECK_EQUAL(astBinaryExpr->op.typeOrID, s1::lexer::LogicOr);
  const auto astLeftBinaryExpr = dynamic_cast<const ast::ExprBinary*> (astBinaryExpr->left.get());
  AST_TEST_EXPR_IS_IDENTIFIER(*(astLeftBinaryExpr->left), "a");
  BOOST_CHECK_EQUAL(astLeftBinaryExpr->op.typeOrID, s1::lexer::LogicAnd);
  AST_TEST_EXPR_IS_IDENTIFIER(*(astLeftBinaryExpr->right), "b");
  AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->right), "c");
}

BOOST_AUTO_TEST_CASE(Precedence6)
{
  using namespace s1::parser;

  std::string inStr ("a||b&&c&&d");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::ExprPtr expr;
  BOOST_CHECK_NO_THROW((expr = astBuilder.ParseExpression ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
  AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "a");
  BOOST_CHECK_EQUAL(astBinaryExpr->op.typeOrID, s1::lexer::LogicOr);
  const auto astRightBinaryExpr = dynamic_cast<const ast::ExprBinary*> (astBinaryExpr->right.get());
  const auto astRightLeftBinaryExpr = dynamic_cast<const ast::ExprBinary*> (astRightBinaryExpr->left.get());
  AST_TEST_EXPR_IS_IDENTIFIER(*(astRightLeftBinaryExpr->left), "b");
  BOOST_CHECK_EQUAL(astRightLeftBinaryExpr->op.typeOrID, s1::lexer::LogicAnd);
  AST_TEST_EXPR_IS_IDENTIFIER(*(astRightLeftBinaryExpr->right), "c");
  BOOST_CHECK_EQUAL(astRightBinaryExpr->op.typeOrID, s1::lexer::LogicAnd);
  AST_TEST_EXPR_IS_IDENTIFIER(*(astRightBinaryExpr->right), "d");
}

BOOST_AUTO_TEST_CASE(Precedence7)
{
  using namespace s1::parser;

  std::string inStr ("!a&&b");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::ExprPtr expr;
  BOOST_CHECK_NO_THROW((expr = astBuilder.ParseExpression ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
  BOOST_CHECK_EQUAL(astBinaryExpr->op.typeOrID, s1::lexer::LogicAnd);
  const auto astLeftUnaryExpr = dynamic_cast<const ast::ExprUnary*> (astBinaryExpr->left.get());
  BOOST_CHECK_EQUAL(astLeftUnaryExpr->op.typeOrID, s1::lexer::LogicInvert);
  AST_TEST_EXPR_IS_IDENTIFIER(*(astLeftUnaryExpr->right), "a");
  AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->right), "b");
}

BOOST_AUTO_TEST_CASE(Precedence8)
{
  using namespace s1::parser;

  std::string inStr ("a+b>c*d");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::ExprPtr expr;
  BOOST_CHECK_NO_THROW((expr = astBuilder.ParseExpression ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
  BOOST_CHECK_EQUAL(astBinaryExpr->op.typeOrID, s1::lexer::Larger);
  const auto astLeftBinaryExpr = dynamic_cast<const ast::ExprBinary*> (astBinaryExpr->left.get());
  AST_TEST_EXPR_IS_IDENTIFIER(*(astLeftBinaryExpr->left), "a");
  BOOST_CHECK_EQUAL(astLeftBinaryExpr->op.typeOrID, s1::lexer::Plus);
  AST_TEST_EXPR_IS_IDENTIFIER(*(astLeftBinaryExpr->right), "b");
  const auto astRightBinaryExpr = dynamic_cast<const ast::ExprBinary*> (astBinaryExpr->right.get());
  AST_TEST_EXPR_IS_IDENTIFIER(*(astRightBinaryExpr->left), "c");
  BOOST_CHECK_EQUAL(astRightBinaryExpr->op.typeOrID, s1::lexer::Mult);
  AST_TEST_EXPR_IS_IDENTIFIER(*(astRightBinaryExpr->right), "d");
}

BOOST_AUTO_TEST_CASE(Precedence9)
{
  using namespace s1::parser;

  std::string inStr ("a=b?c:d");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::ExprPtr expr;
  BOOST_CHECK_NO_THROW((expr = astBuilder.ParseExpression ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
  BOOST_CHECK_EQUAL(astBinaryExpr->op.typeOrID, s1::lexer::Assign);
  AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "a");
  const auto astRightTernaryExpr = dynamic_cast<const ast::ExprTernary*> (astBinaryExpr->right.get());
  AST_TEST_EXPR_IS_IDENTIFIER(*(astRightTernaryExpr->cond), "b");
  AST_TEST_EXPR_IS_IDENTIFIER(*(astRightTernaryExpr->trueExpr), "c");
  AST_TEST_EXPR_IS_IDENTIFIER(*(astRightTernaryExpr->falseExpr), "d");
}

BOOST_AUTO_TEST_CASE(Precedence10)
{
  using namespace s1::parser;

  std::string inStr ("a==b?c:d");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::ExprPtr expr;
  BOOST_CHECK_NO_THROW((expr = astBuilder.ParseExpression ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  const auto astTernaryExpr = dynamic_cast<const ast::ExprTernary*> (expr.get());
  const auto astCondBinaryExpr = dynamic_cast<const ast::ExprBinary*> (astTernaryExpr->cond.get());
  BOOST_CHECK_EQUAL(astCondBinaryExpr->op.typeOrID, s1::lexer::Equals);
  AST_TEST_EXPR_IS_IDENTIFIER(*(astCondBinaryExpr->left), "a");
  AST_TEST_EXPR_IS_IDENTIFIER(*(astCondBinaryExpr->right), "b");
  AST_TEST_EXPR_IS_IDENTIFIER(*(astTernaryExpr->trueExpr), "c");
  AST_TEST_EXPR_IS_IDENTIFIER(*(astTernaryExpr->falseExpr), "d");
}

BOOST_AUTO_TEST_CASE(Unary1)
{
  using namespace s1::parser;

  std::string inStr ("a+-b");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::ExprPtr expr;
  BOOST_CHECK_NO_THROW((expr = astBuilder.ParseExpression ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
  BOOST_CHECK_EQUAL(astBinaryExpr->op.typeOrID, s1::lexer::Plus);
  AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "a");
  const auto astRightUnaryExpr = dynamic_cast<const ast::ExprUnary*> (astBinaryExpr->right.get());
  BOOST_CHECK_EQUAL(astRightUnaryExpr->op.typeOrID, s1::lexer::Minus);
  AST_TEST_EXPR_IS_IDENTIFIER(*(astRightUnaryExpr->right), "b");
}

BOOST_AUTO_TEST_CASE(Ternary1)
{
  using namespace s1::parser;

  std::string inStr ("a?b:c");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::ExprPtr expr;
  BOOST_CHECK_NO_THROW((expr = astBuilder.ParseExpression ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  const auto astTernaryExpr = dynamic_cast<const ast::ExprTernary*> (expr.get());
  AST_TEST_EXPR_IS_IDENTIFIER(*(astTernaryExpr->cond), "a");
  AST_TEST_EXPR_IS_IDENTIFIER(*(astTernaryExpr->trueExpr), "b");
  AST_TEST_EXPR_IS_IDENTIFIER(*(astTernaryExpr->falseExpr), "c");
}

BOOST_AUTO_TEST_CASE(Ternary2)
{
  using namespace s1::parser;

  std::string inStr ("a?b?1:2:c");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::ExprPtr expr;
  BOOST_CHECK_NO_THROW((expr = astBuilder.ParseExpression ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  const auto astTernaryExpr = dynamic_cast<const ast::ExprTernary*> (expr.get());
  AST_TEST_EXPR_IS_IDENTIFIER(*(astTernaryExpr->cond), "a");
  const auto astTrueTernaryExpr = dynamic_cast<const ast::ExprTernary*> (astTernaryExpr->trueExpr.get());
  AST_TEST_EXPR_IS_IDENTIFIER(*(astTrueTernaryExpr->cond), "b");
  AST_TEST_EXPR_IS_NUMERIC(*(astTrueTernaryExpr->trueExpr), int, 1);
  AST_TEST_EXPR_IS_NUMERIC(*(astTrueTernaryExpr->falseExpr), int, 2);
  AST_TEST_EXPR_IS_IDENTIFIER(*(astTernaryExpr->falseExpr), "c");
}

BOOST_AUTO_TEST_CASE(TernaryError)
{
  using namespace s1::parser;

  std::string inStr ("a ? b c");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::ExprPtr expr;
  BOOST_CHECK_NO_THROW((expr = astBuilder.ParseExpression ()));
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size(), 1u);
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedTernaryOperator));
  const auto astTernaryExpr = dynamic_cast<const ast::ExprTernary*> (expr.get());
  AST_TEST_EXPR_IS_IDENTIFIER(*(astTernaryExpr->cond), "a");
  AST_TEST_EXPR_IS_IDENTIFIER(*(astTernaryExpr->trueExpr), "b");
  AST_TEST_EXPR_IS_IDENTIFIER(*(astTernaryExpr->falseExpr), "c");
}

BOOST_AUTO_TEST_CASE(TernaryError2)
{
  using namespace s1::parser;

  std::string inStr ("a ? b");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::ExprPtr expr;
  BOOST_CHECK_NO_THROW((expr = astBuilder.ParseExpression ()));
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size(), 2u);
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedTernaryOperator));
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[1].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedExpression));
  const auto astTernaryExpr = dynamic_cast<const ast::ExprTernary*> (expr.get());
  AST_TEST_EXPR_IS_IDENTIFIER(*(astTernaryExpr->cond), "a");
  AST_TEST_EXPR_IS_IDENTIFIER(*(astTernaryExpr->trueExpr), "b");
  BOOST_CHECK(!astTernaryExpr->falseExpr);
}

BOOST_AUTO_TEST_CASE(TernaryError3)
{
  using namespace s1::parser;

  std::string inStr ("a ? : c");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::ExprPtr expr;
  BOOST_CHECK_NO_THROW((expr = astBuilder.ParseExpression ()));
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size(), 1u);
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedExpression));
  const auto astTernaryExpr = dynamic_cast<const ast::ExprTernary*> (expr.get());
  AST_TEST_EXPR_IS_IDENTIFIER(*(astTernaryExpr->cond), "a");
  BOOST_CHECK(!astTernaryExpr->trueExpr);
  AST_TEST_EXPR_IS_IDENTIFIER(*(astTernaryExpr->falseExpr), "c");
}

BOOST_AUTO_TEST_CASE(Parentheses1)
{
  using namespace s1::parser;

  std::string inStr ("(a)");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::ExprPtr expr;
  BOOST_CHECK_NO_THROW((expr = astBuilder.ParseExpression ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  AST_TEST_EXPR_IS_IDENTIFIER(*expr, "a");
}

BOOST_AUTO_TEST_CASE(Parentheses2)
{
  using namespace s1::parser;

  std::string inStr ("((((a))))");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::ExprPtr expr;
  BOOST_CHECK_NO_THROW((expr = astBuilder.ParseExpression ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  AST_TEST_EXPR_IS_IDENTIFIER(*expr, "a");
}

BOOST_AUTO_TEST_CASE(Parentheses3)
{
  using namespace s1::parser;

  std::string inStr ("a*(b+c)");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::ExprPtr expr;
  BOOST_CHECK_NO_THROW((expr = astBuilder.ParseExpression ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
  AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "a");
  BOOST_CHECK_EQUAL(astBinaryExpr->op.typeOrID, s1::lexer::Mult);
  const auto astRightBinaryExpr = dynamic_cast<const ast::ExprBinary*> (astBinaryExpr->right.get());
  AST_TEST_EXPR_IS_IDENTIFIER(*(astRightBinaryExpr->left), "b");
  BOOST_CHECK_EQUAL(astRightBinaryExpr->op.typeOrID, s1::lexer::Plus);
  AST_TEST_EXPR_IS_IDENTIFIER(*(astRightBinaryExpr->right), "c");
}

BOOST_AUTO_TEST_CASE(IdentifierArray1)
{
  using namespace s1::parser;

  std::string inStr ("a[1]");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::ExprPtr expr;
  BOOST_CHECK_NO_THROW((expr = astBuilder.ParseExpression ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  const auto astArrayElementExpr = dynamic_cast<const ast::ExprArrayElement*> (expr.get());
  AST_TEST_EXPR_IS_IDENTIFIER(*(astArrayElementExpr->value), "a");
  AST_TEST_EXPR_IS_NUMERIC(*(astArrayElementExpr->index), int, 1);
}

BOOST_AUTO_TEST_CASE(IdentifierArray2)
{
  using namespace s1::parser;

  std::string inStr ("a[b+c]");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::ExprPtr expr;
  BOOST_CHECK_NO_THROW((expr = astBuilder.ParseExpression ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  const auto astArrayElementExpr = dynamic_cast<const ast::ExprArrayElement*> (expr.get());
  AST_TEST_EXPR_IS_IDENTIFIER(*(astArrayElementExpr->value), "a");
  const auto astIndexBinaryExpr = dynamic_cast<const ast::ExprBinary*> (astArrayElementExpr->index.get());
  AST_TEST_EXPR_IS_IDENTIFIER(*(astIndexBinaryExpr->left), "b");
  BOOST_CHECK_EQUAL(astIndexBinaryExpr->op.typeOrID, s1::lexer::Plus);
  AST_TEST_EXPR_IS_IDENTIFIER(*(astIndexBinaryExpr->right), "c");
}

BOOST_AUTO_TEST_CASE(IdentifierArray3)
{
  using namespace s1::parser;

  std::string inStr ("(a+b)[1]+c");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::ExprPtr expr;
  BOOST_CHECK_NO_THROW((expr = astBuilder.ParseExpression ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
  const auto astLeftArrayElementExpr = dynamic_cast<const ast::ExprArrayElement*> (astBinaryExpr->left.get());
  const auto astLeftArrayElementBinaryExpr = dynamic_cast<const ast::ExprBinary*> (astLeftArrayElementExpr->value.get());
  AST_TEST_EXPR_IS_IDENTIFIER(*(astLeftArrayElementBinaryExpr->left), "a");
  BOOST_CHECK_EQUAL(astLeftArrayElementBinaryExpr->op.typeOrID, s1::lexer::Plus);
  AST_TEST_EXPR_IS_IDENTIFIER(*(astLeftArrayElementBinaryExpr->right), "b");
  AST_TEST_EXPR_IS_NUMERIC(*(astLeftArrayElementExpr->index), int, 1);
  BOOST_CHECK_EQUAL(astBinaryExpr->op.typeOrID, s1::lexer::Plus);
  AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->right), "c");
}

BOOST_AUTO_TEST_CASE(FunctionCall)
{
  using namespace s1::parser;

  std::string inStr ("x = Foo ()");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::ExprPtr expr;
  BOOST_CHECK_NO_THROW((expr = astBuilder.ParseExpression ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
  AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "x");
  BOOST_CHECK_EQUAL(astBinaryExpr->op.typeOrID, s1::lexer::Assign);
  const auto astRightFuncCallExpr = dynamic_cast<const ast::ExprFunctionCall*> (astBinaryExpr->right.get());
  const auto& funcIdent = boost::get<ast::Identifier> (astRightFuncCallExpr->identifierOrType);
  BOOST_CHECK_EQUAL(funcIdent.GetString(), "Foo");
  BOOST_CHECK(astRightFuncCallExpr->args.empty());
}

BOOST_AUTO_TEST_CASE(FunctionCall2)
{
  using namespace s1::parser;

  std::string inStr ("x = Foo (a + b, 3.0)");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::ExprPtr expr;
  BOOST_CHECK_NO_THROW((expr = astBuilder.ParseExpression ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
  AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "x");
  BOOST_CHECK_EQUAL(astBinaryExpr->op.typeOrID, s1::lexer::Assign);
  const auto astRightFuncCallExpr = dynamic_cast<const ast::ExprFunctionCall*> (astBinaryExpr->right.get());
  const auto& funcIdent = boost::get<ast::Identifier> (astRightFuncCallExpr->identifierOrType);
  BOOST_CHECK_EQUAL(funcIdent.GetString(), "Foo");
  BOOST_CHECK_EQUAL(astRightFuncCallExpr->args.size(), 2u);
  const auto astArg0BinaryExpr = dynamic_cast<const ast::ExprBinary*> (astRightFuncCallExpr->args[0].get());
  AST_TEST_EXPR_IS_IDENTIFIER(*(astArg0BinaryExpr->left), "a");
  BOOST_CHECK_EQUAL(astArg0BinaryExpr->op.typeOrID, s1::lexer::Plus);
  AST_TEST_EXPR_IS_IDENTIFIER(*(astArg0BinaryExpr->right), "b");
  AST_TEST_EXPR_IS_NUMERIC(*(astRightFuncCallExpr->args[1]), float, 3.0);
}

BOOST_AUTO_TEST_CASE(FunctionCallError)
{
  using namespace s1::parser;

  std::string inStr ("x = Foo (4.0");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::ExprPtr expr;
  BOOST_CHECK_NO_THROW((expr = astBuilder.ParseExpression ()));
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size(), 1u);
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedSeparatorOrParenthesis));
  const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
  AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "x");
  BOOST_CHECK_EQUAL(astBinaryExpr->op.typeOrID, s1::lexer::Assign);
  const auto astRightFuncCallExpr = dynamic_cast<const ast::ExprFunctionCall*> (astBinaryExpr->right.get());
  const auto& funcIdent = boost::get<ast::Identifier> (astRightFuncCallExpr->identifierOrType);
  BOOST_CHECK_EQUAL(funcIdent.GetString(), "Foo");
  BOOST_CHECK_EQUAL(astRightFuncCallExpr->args.size(), 1u);
  AST_TEST_EXPR_IS_NUMERIC(*(astRightFuncCallExpr->args[0]), float, 4.0);
}

BOOST_AUTO_TEST_CASE(TypeCtor)
{
  using namespace s1::parser;

  std::string inStr ("x = int ()");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::ExprPtr expr;
  BOOST_CHECK_NO_THROW((expr = astBuilder.ParseExpression ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
  AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "x");
  BOOST_CHECK_EQUAL(astBinaryExpr->op.typeOrID, s1::lexer::Assign);
  const auto astRightFuncCallExpr = dynamic_cast<const ast::ExprFunctionCall*> (astBinaryExpr->right.get());
  const auto& funcType = boost::get<ast::TypePtr> (astRightFuncCallExpr->identifierOrType);
  AST_TEST_TYPE_IS_WELL_KNOWN(*funcType, kwInt);
  BOOST_CHECK(astRightFuncCallExpr->args.empty());
}

BOOST_AUTO_TEST_CASE(TypeCtor2)
{
  using namespace s1::parser;

  std::string inStr ("x = int2 (1, 2)");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::ExprPtr expr;
  BOOST_CHECK_NO_THROW((expr = astBuilder.ParseExpression ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
  AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "x");
  BOOST_CHECK_EQUAL(astBinaryExpr->op.typeOrID, s1::lexer::Assign);
  const auto astRightFuncCallExpr = dynamic_cast<const ast::ExprFunctionCall*> (astBinaryExpr->right.get());
  const auto& funcType = boost::get<ast::TypePtr> (astRightFuncCallExpr->identifierOrType);
  AST_TEST_TYPE_IS_WELL_KNOWN_VEC(*funcType, kwInt, 2);
  AST_TEST_EXPR_IS_NUMERIC(*(astRightFuncCallExpr->args[0]), int, 1);
  AST_TEST_EXPR_IS_NUMERIC(*(astRightFuncCallExpr->args[1]), int, 2);
}

BOOST_AUTO_TEST_CASE(ArrayCtor)
{
  using namespace s1::parser;

  std::string inStr ("x = float[] (1.0, 2.0, 3.0)");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::ExprPtr expr;
  BOOST_CHECK_NO_THROW((expr = astBuilder.ParseExpression ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
  AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "x");
  BOOST_CHECK_EQUAL(astBinaryExpr->op.typeOrID, s1::lexer::Assign);
  const auto astRightFuncCallExpr = dynamic_cast<const ast::ExprFunctionCall*> (astBinaryExpr->right.get());
  const auto& funcType = boost::get<ast::TypePtr> (astRightFuncCallExpr->identifierOrType);
  const auto funcTypeArray = dynamic_cast<const ast::TypeArray*> (funcType.get());
  AST_TEST_TYPE_IS_WELL_KNOWN(*funcTypeArray->containedType, kwFloat);
  AST_TEST_EXPR_IS_NUMERIC(*(astRightFuncCallExpr->args[0]), float, 1.0);
  AST_TEST_EXPR_IS_NUMERIC(*(astRightFuncCallExpr->args[1]), float, 2.0);
  AST_TEST_EXPR_IS_NUMERIC(*(astRightFuncCallExpr->args[2]), float, 3.0);
}

BOOST_AUTO_TEST_CASE(ExprInvalid1)
{
  using namespace s1::parser;

  std::string inStr ("a+");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  s1::parser::ast::ExprPtr expr;
  BOOST_CHECK_NO_THROW((expr = astBuilder.ParseExpression ()));
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size(), 1u);
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedExpression));
  const auto astBinaryExpr = dynamic_cast<const ast::ExprBinary*> (expr.get());
  AST_TEST_EXPR_IS_IDENTIFIER(*(astBinaryExpr->left), "a");
  BOOST_CHECK_EQUAL(astBinaryExpr->op.typeOrID, s1::lexer::Plus);
  BOOST_CHECK(!astBinaryExpr->right);
}

BOOST_AUTO_TEST_SUITE_END()
