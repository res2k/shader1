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

#include "TestSemanticsHandler.h"

#include "TestParser.h"

#include "../../diagnostics/t/TestDiagnosticsHandler.h"

BOOST_AUTO_TEST_SUITE(ParserExpr)

BOOST_AUTO_TEST_CASE(Identifier)
{
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (intType, "a", s1::semantics::ExpressionPtr(), false);

  std::string inStr ("a");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
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
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto intVecType = semanticsHandler.CreateVectorType (intType, 4);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (intVecType, "a", s1::semantics::ExpressionPtr(), false);

  std::string inStr ("a.x");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
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
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto intVecType = semanticsHandler.CreateVectorType (intType, 4);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (intVecType, "a", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intVecType, "b", s1::semantics::ExpressionPtr(), false);

  std::string inStr ("(a+b).x");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
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
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto intVecType = semanticsHandler.CreateVectorType (intType, 4);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (intVecType, "a", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intType, "b", s1::semantics::ExpressionPtr(), false);

  std::string inStr ("a.x+b");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
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
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto intVecType = semanticsHandler.CreateVectorType (intType, 4);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (intType, "a", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intVecType, "b", s1::semantics::ExpressionPtr(), false);

  std::string inStr ("a+b.x");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
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
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (intType, "a", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intType, "b", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intType, "c", s1::semantics::ExpressionPtr(), false);

  std::string inStr ("a-b-c");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
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
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (intType, "a", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intType, "b", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intType, "c", s1::semantics::ExpressionPtr(), false);

  std::string inStr ("a+b*c");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
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
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (intType, "a", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intType, "b", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intType, "c", s1::semantics::ExpressionPtr(), false);

  std::string inStr ("a=b=c");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
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
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (intType, "a", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intType, "b", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intType, "c", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intType, "d", s1::semantics::ExpressionPtr(), false);

  std::string inStr ("a+b==c*d");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
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
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (intType, "a", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intType, "b", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intType, "c", s1::semantics::ExpressionPtr(), false);

  std::string inStr ("a=b&&c");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
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
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (intType, "a", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intType, "b", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intType, "c", s1::semantics::ExpressionPtr(), false);

  std::string inStr ("a&&b||c");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
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
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (intType, "a", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intType, "b", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intType, "c", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intType, "d", s1::semantics::ExpressionPtr(), false);

  std::string inStr ("a||b&&c&&d");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
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
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (intType, "a", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intType, "b", s1::semantics::ExpressionPtr(), false);

  std::string inStr ("!a&&b");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
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
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (intType, "a", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intType, "b", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intType, "c", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intType, "d", s1::semantics::ExpressionPtr(), false);

  std::string inStr ("a+b>c*d");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
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
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (intType, "a", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intType, "b", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intType, "c", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intType, "d", s1::semantics::ExpressionPtr(), false);

  std::string inStr ("a=b?c:d");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
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
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (intType, "a", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intType, "b", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intType, "c", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intType, "d", s1::semantics::ExpressionPtr(), false);

  std::string inStr ("a==b?c:d");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
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
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (intType, "a", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intType, "b", s1::semantics::ExpressionPtr(), false);

  std::string inStr ("a+-b");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
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
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (intType, "a", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intType, "b", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intType, "c", s1::semantics::ExpressionPtr(), false);

  std::string inStr ("a?b:c");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
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
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (intType, "a", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intType, "b", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intType, "c", s1::semantics::ExpressionPtr(), false);

  std::string inStr ("a?b?1:2:c");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
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

  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (intType, "a", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intType, "b", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intType, "c", s1::semantics::ExpressionPtr(), false);

  std::string inStr ("a ? b c");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
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

  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (intType, "a", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intType, "b", s1::semantics::ExpressionPtr(), false);

  std::string inStr ("a ? b");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
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

  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (intType, "a", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intType, "c", s1::semantics::ExpressionPtr(), false);

  std::string inStr ("a ? : c");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
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
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (intType, "a", s1::semantics::ExpressionPtr(), false);

  std::string inStr ("(a)");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
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
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (intType, "a", s1::semantics::ExpressionPtr(), false);

  std::string inStr ("((((a))))");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
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
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (intType, "a", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intType, "b", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intType, "c", s1::semantics::ExpressionPtr(), false);

  std::string inStr ("a*(b+c)");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
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
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (intType, "a", s1::semantics::ExpressionPtr(), false);

  std::string inStr ("a[1]");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
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
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (intType, "a", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intType, "b", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intType, "c", s1::semantics::ExpressionPtr(), false);

  std::string inStr ("a[b+c]");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
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
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (intType, "a", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intType, "b", s1::semantics::ExpressionPtr(), false);
  scope->AddVariable (intType, "c", s1::semantics::ExpressionPtr(), false);

  std::string inStr ("(a+b)[1]+c");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
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
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);
  s1::semantics::ScopePtr scope (
    semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
          s1::semantics::ScopeLevel::Global)); 
  s1::semantics::Scope::FunctionFormalParameters params;
  scope->AddVariable (s1::semantics::TypePtr (), s1::uc::String ("x"),
    s1::semantics::ExpressionPtr (), false);
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
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);
  s1::semantics::ScopePtr scope (
    semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
          s1::semantics::ScopeLevel::Global)); 
  s1::semantics::Scope::FunctionFormalParameters params;
  scope->AddVariable (s1::semantics::TypePtr (), s1::uc::String ("a"),
    s1::semantics::ExpressionPtr (), false);
  scope->AddVariable (s1::semantics::TypePtr (), s1::uc::String ("b"),
    s1::semantics::ExpressionPtr (), false);
  scope->AddVariable (s1::semantics::TypePtr (), s1::uc::String ("x"),
    s1::semantics::ExpressionPtr (), false);
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
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);
  s1::semantics::ScopePtr scope (
    semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                  s1::semantics::ScopeLevel::Global));
  s1::semantics::Scope::FunctionFormalParameters params;
  scope->AddVariable (s1::semantics::TypePtr (), s1::uc::String ("x"),
                      s1::semantics::ExpressionPtr (), false);
  scope->AddFunction (s1::semantics::TypePtr (), s1::uc::String ("Foo"), params);

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
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (intType, "x", s1::semantics::ExpressionPtr(), false);

  std::string inStr ("x = int ()");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
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
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (intType, "x", s1::semantics::ExpressionPtr(), false);

  std::string inStr ("x = int2 (1, 2)");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
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
  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (intType, "x", s1::semantics::ExpressionPtr(), false);

  std::string inStr ("x = float[] (1.0, 2.0, 3.0)");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
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

  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto scope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                              s1::semantics::ScopeLevel::Global);
  scope->AddVariable (intType, "a", s1::semantics::ExpressionPtr(), false);

  std::string inStr ("a+");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
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
