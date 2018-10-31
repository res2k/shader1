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
#include "parser/Diagnostics.h"
#include "parser/Parser.h"

#include "SimpleSemanticsDiagnosticsImpl.h"
#include "TestParser.h"
#include "TestSemanticsHandler.h"

#include "../../diagnostics/t/TestDiagnosticsHandler.h"

BOOST_AUTO_TEST_SUITE(ParserExpr)

BOOST_AUTO_TEST_CASE(Identifier)
{
  TestDiagnosticsHandler errorHandler;
  SimpleSemanticsDiagnosticsImpl semanticDiag (errorHandler);
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (semanticDiag, intType.get(), "a", nullptr, false);

  std::string inStr ("a");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  s1::Lexer lexer (ustream, errorHandler);
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::ExpressionPtr expr;
  BOOST_CHECK_NO_THROW ((expr = parser.ParseExpression (scope)));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  TestSemanticsHandler::TestExpressionBase* testExpr = 
    static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
  BOOST_CHECK_EQUAL (testExpr->GetExprString(), "a");
}

BOOST_AUTO_TEST_CASE(IdentifierAttr)
{
  TestDiagnosticsHandler errorHandler;
  SimpleSemanticsDiagnosticsImpl semanticDiag (errorHandler);
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto intVecType = semanticsHandler.CreateVectorType (intType, 4);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (semanticDiag, intVecType.get(), "a", nullptr, false);

  std::string inStr ("a.x");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  s1::Lexer lexer (ustream, errorHandler);
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::ExpressionPtr expr;
  BOOST_CHECK_NO_THROW ((expr = parser.ParseExpression (scope)));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  TestSemanticsHandler::TestExpressionBase* testExpr = 
    static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
  BOOST_CHECK_EQUAL (testExpr->GetExprString(), "a.x");
}

BOOST_AUTO_TEST_CASE(IdentifierAttr2)
{
  TestDiagnosticsHandler errorHandler;
  SimpleSemanticsDiagnosticsImpl semanticDiag (errorHandler);
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto intVecType = semanticsHandler.CreateVectorType (intType, 4);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (semanticDiag, intVecType.get(), "a", nullptr, false);
  scope->AddVariable (semanticDiag, intVecType.get(), "b", nullptr, false);

  std::string inStr ("(a+b).x");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  s1::Lexer lexer (ustream, errorHandler);
  TestParser parser (lexer, semanticsHandler, errorHandler);
  
  s1::semantics::ExpressionPtr expr;
  BOOST_CHECK_NO_THROW ((expr = parser.ParseExpression (scope)));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  TestSemanticsHandler::TestExpressionBase* testExpr = 
    static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
  BOOST_CHECK_EQUAL (testExpr->GetExprString(), "(a + b).x");
}

BOOST_AUTO_TEST_CASE(IdentifierAttr3)
{
  TestDiagnosticsHandler errorHandler;
  SimpleSemanticsDiagnosticsImpl semanticDiag (errorHandler);
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto intVecType = semanticsHandler.CreateVectorType (intType, 4);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (semanticDiag, intVecType.get(), "a", nullptr, false);
  scope->AddVariable (semanticDiag, intType.get(), "b", nullptr, false);

  std::string inStr ("a.x+b");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  s1::Lexer lexer (ustream, errorHandler);
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::ExpressionPtr expr;
  BOOST_CHECK_NO_THROW ((expr = parser.ParseExpression (scope)));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  TestSemanticsHandler::TestExpressionBase* testExpr = 
    static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
  BOOST_CHECK_EQUAL (testExpr->GetExprString(), "(a.x + b)");
}

BOOST_AUTO_TEST_CASE(IdentifierAttr4)
{
  TestDiagnosticsHandler errorHandler;
  SimpleSemanticsDiagnosticsImpl semanticDiag (errorHandler);
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto intVecType = semanticsHandler.CreateVectorType (intType, 4);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (semanticDiag, intType.get(), "a", nullptr, false);
  scope->AddVariable (semanticDiag, intVecType.get(), "b", nullptr, false);

  std::string inStr ("a+b.x");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  s1::Lexer lexer (ustream, errorHandler);
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::ExpressionPtr expr;
  BOOST_CHECK_NO_THROW ((expr = parser.ParseExpression (scope)));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  TestSemanticsHandler::TestExpressionBase* testExpr = 
    static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
  BOOST_CHECK_EQUAL (testExpr->GetExprString(), "(a + b.x)");
}

BOOST_AUTO_TEST_CASE(EvaluationOrder)
{
  TestDiagnosticsHandler errorHandler;
  SimpleSemanticsDiagnosticsImpl semanticDiag (errorHandler);
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (semanticDiag, intType.get(), "a", nullptr, false);
  scope->AddVariable (semanticDiag, intType.get(), "b", nullptr, false);
  scope->AddVariable (semanticDiag, intType.get(), "c", nullptr, false);

  std::string inStr ("a-b-c");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  s1::Lexer lexer (ustream, errorHandler);
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::ExpressionPtr expr;
  BOOST_CHECK_NO_THROW ((expr = parser.ParseExpression (scope)));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  TestSemanticsHandler::TestExpressionBase* testExpr = 
    static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
  BOOST_CHECK_EQUAL (testExpr->GetExprString(), "((a - b) - c)");
}

BOOST_AUTO_TEST_CASE(Precedence1)
{
  TestDiagnosticsHandler errorHandler;
  SimpleSemanticsDiagnosticsImpl semanticDiag (errorHandler);
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (semanticDiag, intType.get(), "a", nullptr, false);
  scope->AddVariable (semanticDiag, intType.get(), "b", nullptr, false);
  scope->AddVariable (semanticDiag, intType.get(), "c", nullptr, false);

  std::string inStr ("a+b*c");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  s1::Lexer lexer (ustream, errorHandler);
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::ExpressionPtr expr;
  BOOST_CHECK_NO_THROW ((expr = parser.ParseExpression (scope)));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  TestSemanticsHandler::TestExpressionBase* testExpr = 
    static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
  BOOST_CHECK_EQUAL (testExpr->GetExprString(), "(a + (b * c))");
}

BOOST_AUTO_TEST_CASE(Precedence2)
{
  TestDiagnosticsHandler errorHandler;
  SimpleSemanticsDiagnosticsImpl semanticDiag (errorHandler);
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (semanticDiag, intType.get(), "a", nullptr, false);
  scope->AddVariable (semanticDiag, intType.get(), "b", nullptr, false);
  scope->AddVariable (semanticDiag, intType.get(), "c", nullptr, false);

  std::string inStr ("a=b=c");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  s1::Lexer lexer (ustream, errorHandler);
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::ExpressionPtr expr;
  BOOST_CHECK_NO_THROW ((expr = parser.ParseExpression (scope)));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  TestSemanticsHandler::TestExpressionBase* testExpr = 
    static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
  BOOST_CHECK_EQUAL (testExpr->GetExprString(), "(a = (b = c))");
}

BOOST_AUTO_TEST_CASE(Precedence3)
{
  TestDiagnosticsHandler errorHandler;
  SimpleSemanticsDiagnosticsImpl semanticDiag (errorHandler);
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (semanticDiag, intType.get(), "a", nullptr, false);
  scope->AddVariable (semanticDiag, intType.get(), "b", nullptr, false);
  scope->AddVariable (semanticDiag, intType.get(), "c", nullptr, false);
  scope->AddVariable (semanticDiag, intType.get(), "d", nullptr, false);

  std::string inStr ("a+b==c*d");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  s1::Lexer lexer (ustream, errorHandler);
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::ExpressionPtr expr;
  BOOST_CHECK_NO_THROW ((expr = parser.ParseExpression (scope)));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  TestSemanticsHandler::TestExpressionBase* testExpr = 
    static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
  BOOST_CHECK_EQUAL (testExpr->GetExprString(), "((a + b) == (c * d))");
}

BOOST_AUTO_TEST_CASE(Precedence4)
{
  TestDiagnosticsHandler errorHandler;
  SimpleSemanticsDiagnosticsImpl semanticDiag (errorHandler);
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (semanticDiag, intType.get(), "a", nullptr, false);
  scope->AddVariable (semanticDiag, intType.get(), "b", nullptr, false);
  scope->AddVariable (semanticDiag, intType.get(), "c", nullptr, false);

  std::string inStr ("a=b&&c");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  s1::Lexer lexer (ustream, errorHandler);
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::ExpressionPtr expr;
  BOOST_CHECK_NO_THROW ((expr = parser.ParseExpression (scope)));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  TestSemanticsHandler::TestExpressionBase* testExpr = 
    static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
  BOOST_CHECK_EQUAL (testExpr->GetExprString(), "(a = (b && c))");
}

BOOST_AUTO_TEST_CASE(Precedence5)
{
  TestDiagnosticsHandler errorHandler;
  SimpleSemanticsDiagnosticsImpl semanticDiag (errorHandler);
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (semanticDiag, intType.get(), "a", nullptr, false);
  scope->AddVariable (semanticDiag, intType.get(), "b", nullptr, false);
  scope->AddVariable (semanticDiag, intType.get(), "c", nullptr, false);

  std::string inStr ("a&&b||c");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  s1::Lexer lexer (ustream, errorHandler);
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::ExpressionPtr expr;
  BOOST_CHECK_NO_THROW ((expr = parser.ParseExpression (scope)));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  TestSemanticsHandler::TestExpressionBase* testExpr = 
    static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
  BOOST_CHECK_EQUAL (testExpr->GetExprString(), "((a && b) || c)");
}

BOOST_AUTO_TEST_CASE(Precedence6)
{
  TestDiagnosticsHandler errorHandler;
  SimpleSemanticsDiagnosticsImpl semanticDiag (errorHandler);
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (semanticDiag, intType.get(), "a", nullptr, false);
  scope->AddVariable (semanticDiag, intType.get(), "b", nullptr, false);
  scope->AddVariable (semanticDiag, intType.get(), "c", nullptr, false);
  scope->AddVariable (semanticDiag, intType.get(), "d", nullptr, false);

  std::string inStr ("a||b&&c&&d");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  s1::Lexer lexer (ustream, errorHandler);
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::ExpressionPtr expr;
  BOOST_CHECK_NO_THROW ((expr = parser.ParseExpression (scope)));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  TestSemanticsHandler::TestExpressionBase* testExpr = 
    static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
  BOOST_CHECK_EQUAL (testExpr->GetExprString(), "(a || ((b && c) && d))");
}

BOOST_AUTO_TEST_CASE(Precedence7)
{
  TestDiagnosticsHandler errorHandler;
  SimpleSemanticsDiagnosticsImpl semanticDiag (errorHandler);
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (semanticDiag, intType.get(), "a", nullptr, false);
  scope->AddVariable (semanticDiag, intType.get(), "b", nullptr, false);

  std::string inStr ("!a&&b");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  s1::Lexer lexer (ustream, errorHandler);
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::ExpressionPtr expr;
  BOOST_CHECK_NO_THROW ((expr = parser.ParseExpression (scope)));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  TestSemanticsHandler::TestExpressionBase* testExpr = 
    static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
  BOOST_CHECK_EQUAL (testExpr->GetExprString(), "(!a && b)");
}

BOOST_AUTO_TEST_CASE(Precedence8)
{
  TestDiagnosticsHandler errorHandler;
  SimpleSemanticsDiagnosticsImpl semanticDiag (errorHandler);
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (semanticDiag, intType.get(), "a", nullptr, false);
  scope->AddVariable (semanticDiag, intType.get(), "b", nullptr, false);
  scope->AddVariable (semanticDiag, intType.get(), "c", nullptr, false);
  scope->AddVariable (semanticDiag, intType.get(), "d", nullptr, false);

  std::string inStr ("a+b>c*d");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  s1::Lexer lexer (ustream, errorHandler);
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::ExpressionPtr expr;
  BOOST_CHECK_NO_THROW ((expr = parser.ParseExpression (scope)));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  TestSemanticsHandler::TestExpressionBase* testExpr = 
    static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
  BOOST_CHECK_EQUAL (testExpr->GetExprString(), "((a + b) > (c * d))");
}

BOOST_AUTO_TEST_CASE(Precedence9)
{
  TestDiagnosticsHandler errorHandler;
  SimpleSemanticsDiagnosticsImpl semanticDiag (errorHandler);
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (semanticDiag, intType.get(), "a", nullptr, false);
  scope->AddVariable (semanticDiag, intType.get(), "b", nullptr, false);
  scope->AddVariable (semanticDiag, intType.get(), "c", nullptr, false);
  scope->AddVariable (semanticDiag, intType.get(), "d", nullptr, false);

  std::string inStr ("a=b?c:d");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  s1::Lexer lexer (ustream, errorHandler);
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::ExpressionPtr expr;
  BOOST_CHECK_NO_THROW ((expr = parser.ParseExpression (scope)));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  TestSemanticsHandler::TestExpressionBase* testExpr = 
    static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
  BOOST_CHECK_EQUAL (testExpr->GetExprString(), "(a = (b ? c : d))");
}

BOOST_AUTO_TEST_CASE(Precedence10)
{
  TestDiagnosticsHandler errorHandler;
  SimpleSemanticsDiagnosticsImpl semanticDiag (errorHandler);
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (semanticDiag, intType.get(), "a", nullptr, false);
  scope->AddVariable (semanticDiag, intType.get(), "b", nullptr, false);
  scope->AddVariable (semanticDiag, intType.get(), "c", nullptr, false);
  scope->AddVariable (semanticDiag, intType.get(), "d", nullptr, false);

  std::string inStr ("a==b?c:d");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  s1::Lexer lexer (ustream, errorHandler);
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::ExpressionPtr expr;
  BOOST_CHECK_NO_THROW ((expr = parser.ParseExpression (scope)));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  TestSemanticsHandler::TestExpressionBase* testExpr = 
    static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
  BOOST_CHECK_EQUAL (testExpr->GetExprString(), "((a == b) ? c : d)");
}

BOOST_AUTO_TEST_CASE(Unary1)
{
  TestDiagnosticsHandler errorHandler;
  SimpleSemanticsDiagnosticsImpl semanticDiag (errorHandler);
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (semanticDiag, intType.get(), "a", nullptr, false);
  scope->AddVariable (semanticDiag, intType.get(), "b", nullptr, false);

  std::string inStr ("a+-b");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  s1::Lexer lexer (ustream, errorHandler);
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::ExpressionPtr expr;
  BOOST_CHECK_NO_THROW ((expr = parser.ParseExpression (scope)));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  TestSemanticsHandler::TestExpressionBase* testExpr = 
    static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
  BOOST_CHECK_EQUAL (testExpr->GetExprString(), "(a + -b)");
}

BOOST_AUTO_TEST_CASE(Ternary1)
{
  TestDiagnosticsHandler errorHandler;
  SimpleSemanticsDiagnosticsImpl semanticDiag (errorHandler);
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (semanticDiag, intType.get(), "a", nullptr, false);
  scope->AddVariable (semanticDiag, intType.get(), "b", nullptr, false);
  scope->AddVariable (semanticDiag, intType.get(), "c", nullptr, false);

  std::string inStr ("a?b:c");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  s1::Lexer lexer (ustream, errorHandler);
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::ExpressionPtr expr;
  BOOST_CHECK_NO_THROW ((expr = parser.ParseExpression (scope)));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  TestSemanticsHandler::TestExpressionBase* testExpr = 
    static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
  BOOST_CHECK_EQUAL (testExpr->GetExprString(), "(a ? b : c)");
}

BOOST_AUTO_TEST_CASE(Ternary2)
{
  TestDiagnosticsHandler errorHandler;
  SimpleSemanticsDiagnosticsImpl semanticDiag (errorHandler);
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (semanticDiag, intType.get(), "a", nullptr, false);
  scope->AddVariable (semanticDiag, intType.get(), "b", nullptr, false);
  scope->AddVariable (semanticDiag, intType.get(), "c", nullptr, false);

  std::string inStr ("a?b?1:2:c");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  s1::Lexer lexer (ustream, errorHandler);
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::ExpressionPtr expr;
  BOOST_CHECK_NO_THROW ((expr = parser.ParseExpression (scope)));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  TestSemanticsHandler::TestExpressionBase* testExpr = 
    static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
  BOOST_CHECK_EQUAL (testExpr->GetExprString(), "(a ? (b ? 1 : 2) : c)");
}

BOOST_AUTO_TEST_CASE(TernaryError)
{
  using namespace s1::parser;

  TestDiagnosticsHandler errorHandler;
  SimpleSemanticsDiagnosticsImpl semanticDiag (errorHandler);
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (semanticDiag, intType.get(), "a", nullptr, false);
  scope->AddVariable (semanticDiag, intType.get(), "b", nullptr, false);
  scope->AddVariable (semanticDiag, intType.get(), "c", nullptr, false);

  std::string inStr ("a ? b c");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  s1::Lexer lexer (ustream, errorHandler);
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::ExpressionPtr expr;
  BOOST_CHECK_NO_THROW ((expr = parser.ParseExpression (scope)));
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size(), 1u);
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedTernaryOperator));
  TestSemanticsHandler::TestExpressionBase* testExpr =
    static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
  BOOST_CHECK_EQUAL (testExpr->GetExprString(), "(a ? b : c)");
}

BOOST_AUTO_TEST_CASE(TernaryError2)
{
  using namespace s1::parser;

  TestDiagnosticsHandler errorHandler;
  SimpleSemanticsDiagnosticsImpl semanticDiag (errorHandler);
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (semanticDiag, intType.get(), "a", nullptr, false);
  scope->AddVariable (semanticDiag, intType.get(), "b", nullptr, false);

  std::string inStr ("a ? b");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  s1::Lexer lexer (ustream, errorHandler);
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::ExpressionPtr expr;
  BOOST_CHECK_NO_THROW ((expr = parser.ParseExpression (scope)));
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size(), 2u);
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedTernaryOperator));
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[1].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedExpression));
  TestSemanticsHandler::TestExpressionBase* testExpr =
    static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
  BOOST_CHECK_EQUAL (testExpr->GetExprString(), "(a ? b : <INVALID-EXPR>)");
}

BOOST_AUTO_TEST_CASE(TernaryError3)
{
  using namespace s1::parser;

  TestDiagnosticsHandler errorHandler;
  SimpleSemanticsDiagnosticsImpl semanticDiag (errorHandler);
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (semanticDiag, intType.get(), "a", nullptr, false);
  scope->AddVariable (semanticDiag, intType.get(), "c", nullptr, false);

  std::string inStr ("a ? : c");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  s1::Lexer lexer (ustream, errorHandler);
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::ExpressionPtr expr;
  BOOST_CHECK_NO_THROW ((expr = parser.ParseExpression (scope)));
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size(), 1u);
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedExpression));
  TestSemanticsHandler::TestExpressionBase* testExpr =
    static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
  BOOST_CHECK_EQUAL (testExpr->GetExprString(), "(a ? <INVALID-EXPR> : c)");
}

BOOST_AUTO_TEST_CASE(Parentheses1)
{
  TestDiagnosticsHandler errorHandler;
  SimpleSemanticsDiagnosticsImpl semanticDiag (errorHandler);
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (semanticDiag, intType.get(), "a", nullptr, false);

  std::string inStr ("(a)");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  s1::Lexer lexer (ustream, errorHandler);
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::ExpressionPtr expr;
  BOOST_CHECK_NO_THROW ((expr = parser.ParseExpression (scope)));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  TestSemanticsHandler::TestExpressionBase* testExpr = 
    static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
  BOOST_CHECK_EQUAL (testExpr->GetExprString(), "a");
}

BOOST_AUTO_TEST_CASE(Parentheses2)
{
  TestDiagnosticsHandler errorHandler;
  SimpleSemanticsDiagnosticsImpl semanticDiag (errorHandler);
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (semanticDiag, intType.get(), "a", nullptr, false);

  std::string inStr ("((((a))))");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  s1::Lexer lexer (ustream, errorHandler);
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::ExpressionPtr expr;
  BOOST_CHECK_NO_THROW ((expr = parser.ParseExpression (scope)));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  TestSemanticsHandler::TestExpressionBase* testExpr = 
    static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
  BOOST_CHECK_EQUAL (testExpr->GetExprString(), "a");
}

BOOST_AUTO_TEST_CASE(Parentheses3)
{
  TestDiagnosticsHandler errorHandler;
  SimpleSemanticsDiagnosticsImpl semanticDiag (errorHandler);
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (semanticDiag, intType.get(), "a", nullptr, false);
  scope->AddVariable (semanticDiag, intType.get(), "b", nullptr, false);
  scope->AddVariable (semanticDiag, intType.get(), "c", nullptr, false);

  std::string inStr ("a*(b+c)");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  s1::Lexer lexer (ustream, errorHandler);
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::ExpressionPtr expr;
  BOOST_CHECK_NO_THROW ((expr = parser.ParseExpression (scope)));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  TestSemanticsHandler::TestExpressionBase* testExpr = 
    static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
  BOOST_CHECK_EQUAL (testExpr->GetExprString(), "(a * (b + c))");
}

BOOST_AUTO_TEST_CASE(IdentifierArray1)
{
  TestDiagnosticsHandler errorHandler;
  SimpleSemanticsDiagnosticsImpl semanticDiag (errorHandler);
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (semanticDiag, intType.get(), "a", nullptr, false);

  std::string inStr ("a[1]");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  s1::Lexer lexer (ustream, errorHandler);
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::ExpressionPtr expr;
  BOOST_CHECK_NO_THROW ((expr = parser.ParseExpression (scope)));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  TestSemanticsHandler::TestExpressionBase* testExpr = 
    static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
  BOOST_CHECK_EQUAL (testExpr->GetExprString(), "a[1]");
}

BOOST_AUTO_TEST_CASE(IdentifierArray2)
{
  TestDiagnosticsHandler errorHandler;
  SimpleSemanticsDiagnosticsImpl semanticDiag (errorHandler);
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (semanticDiag, intType.get(), "a", nullptr, false);
  scope->AddVariable (semanticDiag, intType.get(), "b", nullptr, false);
  scope->AddVariable (semanticDiag, intType.get(), "c", nullptr, false);

  std::string inStr ("a[b+c]");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  s1::Lexer lexer (ustream, errorHandler);
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::ExpressionPtr expr;
  BOOST_CHECK_NO_THROW ((expr = parser.ParseExpression (scope)));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  TestSemanticsHandler::TestExpressionBase* testExpr = 
    static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
  BOOST_CHECK_EQUAL (testExpr->GetExprString(), "a[(b + c)]");
}

BOOST_AUTO_TEST_CASE(IdentifierArray3)
{
  TestDiagnosticsHandler errorHandler;
  SimpleSemanticsDiagnosticsImpl semanticDiag (errorHandler);
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (semanticDiag, intType.get(), "a", nullptr, false);
  scope->AddVariable (semanticDiag, intType.get(), "b", nullptr, false);
  scope->AddVariable (semanticDiag, intType.get(), "c", nullptr, false);

  std::string inStr ("(a+b)[1]+c");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  s1::Lexer lexer (ustream, errorHandler);
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::ExpressionPtr expr;
  BOOST_CHECK_NO_THROW ((expr = parser.ParseExpression (scope)));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  TestSemanticsHandler::TestExpressionBase* testExpr = 
    static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
  BOOST_CHECK_EQUAL (testExpr->GetExprString(), "((a + b)[1] + c)");
}

BOOST_AUTO_TEST_CASE(FunctionCall)
{
  std::string inStr ("x = Foo ()");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  SimpleSemanticsDiagnosticsImpl semanticDiag (errorHandler);
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);
  s1::semantics::ScopePtr scope (
    semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
          s1::semantics::ScopeLevel::Global)); 
  s1::semantics::FunctionFormalParameters params;
  scope->AddVariable (semanticDiag, nullptr, s1::uc::String ("x"), nullptr, false);
  scope->AddFunction (s1::semantics::TypePtr (), s1::uc::String ("Foo"), params);

  s1::semantics::ExpressionPtr expr;
  BOOST_CHECK_NO_THROW ((expr = parser.ParseExpression (scope)));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  TestSemanticsHandler::TestExpressionBase* testExpr = 
    static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
  BOOST_CHECK_EQUAL (testExpr->GetExprString(), "(x = Foo ())");
}

BOOST_AUTO_TEST_CASE(FunctionCall2)
{
  std::string inStr ("x = Foo (a + b, 3.0)");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  SimpleSemanticsDiagnosticsImpl semanticDiag (errorHandler);
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);
  s1::semantics::ScopePtr scope (
    semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
          s1::semantics::ScopeLevel::Global)); 
  s1::semantics::FunctionFormalParameters params;
  scope->AddVariable (semanticDiag, nullptr, s1::uc::String ("a"), nullptr, false);
  scope->AddVariable (semanticDiag, nullptr, s1::uc::String ("b"), nullptr, false);
  scope->AddVariable (semanticDiag, nullptr, s1::uc::String ("x"), nullptr, false);
  scope->AddFunction (s1::semantics::TypePtr (), s1::uc::String ("Foo"), params);

  s1::semantics::ExpressionPtr expr;
  BOOST_CHECK_NO_THROW ((expr = parser.ParseExpression (scope)));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  TestSemanticsHandler::TestExpressionBase* testExpr = 
    static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
  BOOST_CHECK_EQUAL (testExpr->GetExprString(), "(x = Foo ((a + b), 3.0))");
}

BOOST_AUTO_TEST_CASE(FunctionCallError)
{
  using namespace s1::parser;

  std::string inStr ("x = Foo (4.0");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  SimpleSemanticsDiagnosticsImpl semanticDiag (errorHandler);
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);
  s1::semantics::ScopePtr scope (
    semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                  s1::semantics::ScopeLevel::Global));
  s1::semantics::FunctionFormalParameters params;
  scope->AddVariable (semanticDiag, nullptr, s1::uc::String ("x"), nullptr, false);
  scope->AddFunction (nullptr, s1::uc::String ("Foo"), params);

  s1::semantics::ExpressionPtr expr;
  BOOST_CHECK_NO_THROW ((expr = parser.ParseExpression (scope)));
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size(), 1u);
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedSeparatorOrParenthesis));
  TestSemanticsHandler::TestExpressionBase* testExpr =
    static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
  BOOST_CHECK_EQUAL (testExpr->GetExprString(), "(x = Foo (4.0))");
}

BOOST_AUTO_TEST_CASE(TypeCtor)
{
  TestDiagnosticsHandler errorHandler;
  SimpleSemanticsDiagnosticsImpl semanticDiag (errorHandler);
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (semanticDiag, intType.get(), "x", nullptr, false);

  std::string inStr ("x = int ()");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  s1::Lexer lexer (ustream, errorHandler);
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::ExpressionPtr expr;
  BOOST_CHECK_NO_THROW ((expr = parser.ParseExpression (scope)));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  TestSemanticsHandler::TestExpressionBase* testExpr = 
    static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
  BOOST_CHECK_EQUAL (testExpr->GetExprString(), "(x = int ())");
}

BOOST_AUTO_TEST_CASE(TypeCtor2)
{
  TestDiagnosticsHandler errorHandler;
  SimpleSemanticsDiagnosticsImpl semanticDiag (errorHandler);
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (semanticDiag, intType.get(), "x", nullptr, false);

  std::string inStr ("x = int2 (1, 2)");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  s1::Lexer lexer (ustream, errorHandler);
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::ExpressionPtr expr;
  BOOST_CHECK_NO_THROW ((expr = parser.ParseExpression (scope)));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  TestSemanticsHandler::TestExpressionBase* testExpr = 
    static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
  BOOST_CHECK_EQUAL (testExpr->GetExprString(), "(x = int2 (1, 2))");
}

BOOST_AUTO_TEST_CASE(ArrayCtor)
{
  TestDiagnosticsHandler errorHandler;
  SimpleSemanticsDiagnosticsImpl semanticDiag (errorHandler);
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (semanticDiag, intType.get(), "x", nullptr, false);

  std::string inStr ("x = float[] (1.0, 2.0, 3.0)");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  s1::Lexer lexer (ustream, errorHandler);
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::ExpressionPtr expr;
  BOOST_CHECK_NO_THROW ((expr = parser.ParseExpression (scope)));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  TestSemanticsHandler::TestExpressionBase* testExpr =
    static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
  BOOST_CHECK_EQUAL (testExpr->GetExprString(), "(x = float[] (1.0, 2.0, 3.0))");
}

BOOST_AUTO_TEST_CASE(ExprInvalid1)
{
  using namespace s1::parser;

  TestDiagnosticsHandler errorHandler;
  SimpleSemanticsDiagnosticsImpl semanticDiag (errorHandler);
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (semanticDiag, intType.get(), "a", nullptr, false);

  std::string inStr ("a+");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  s1::Lexer lexer (ustream, errorHandler);
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::ExpressionPtr expr;
  BOOST_CHECK_NO_THROW ((expr = parser.ParseExpression (scope)));
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size(), 1u);
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedExpression));
  TestSemanticsHandler::TestExpressionBase* testExpr =
    static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
  BOOST_CHECK_EQUAL (testExpr->GetExprString(), "(a + <INVALID-EXPR>)");
}

BOOST_AUTO_TEST_SUITE_END()
