/*
    Shader1
    Copyright (c) 2010-2014 Frank Richter


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
#include "parser/Diagnostics.h"
#include "parser/Parser.h"

#include "TestSemanticsHandler.h"

#include "ParserTestTraits.h"
#include "TestParser.h"

#include "../../diagnostics/t/TestDiagnosticsHandler.h"

class ParserExprTestSuite : public CxxTest::TestSuite 
{
public:
  void testIdentifier (void)
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
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "a");
  }
  
  void testIdentifierAttr (void)
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
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "a.x");
  }
  
  void testIdentifierAttr2 (void)
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
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(a + b).x");
  }
  
  void testIdentifierAttr3 (void)
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
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(a.x + b)");
  }
  
  void testIdentifierAttr4 (void)
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
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(a + b.x)");
  }
  
  void testEvaluationOrder (void)
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
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "((a - b) - c)");
  }
  
  void testPrecedence1 (void)
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
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(a + (b * c))");
  }
  
  void testPrecedence2 (void)
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
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(a = (b = c))");
  }
  
  void testPrecedence3 (void)
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
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "((a + b) == (c * d))");
  }
  
  void testPrecedence4 (void)
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
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(a = (b && c))");
  }
  
  void testPrecedence5 (void)
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
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "((a && b) || c)");
  }
  
  void testPrecedence6 (void)
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
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(a || ((b && c) && d))");
  }
  
  void testPrecedence7 (void)
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
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(!a && b)");
  }
  
  void testPrecedence8 (void)
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
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "((a + b) > (c * d))");
  }
  
  void testPrecedence9 (void)
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
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(a = (b ? c : d))");
  }
  
  void testPrecedence10 (void)
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
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "((a == b) ? c : d)");
  }
  
  void testUnary1 (void)
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
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(a + -b)");
  }
  
  void testTernary1 (void)
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
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(a ? b : c)");
  }
  
  void testTernary2 (void)
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
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(a ? (b ? 1 : 2) : c)");
  }

  void testTernaryError (void)
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
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT_EQUALS(errorHandler.parseErrors.size(), 1u);
    TS_ASSERT_EQUALS(errorHandler.parseErrors[0].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedTernaryOperator));
    TestSemanticsHandler::TestExpressionBase* testExpr =
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(a ? b : c)");
  }

  void testTernaryError2 (void)
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
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT_EQUALS(errorHandler.parseErrors.size(), 2u);
    TS_ASSERT_EQUALS(errorHandler.parseErrors[0].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedTernaryOperator));
    TS_ASSERT_EQUALS(errorHandler.parseErrors[1].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedExpression));
    TestSemanticsHandler::TestExpressionBase* testExpr =
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(a ? b : <INVALID-EXPR>)");
  }

  void testTernaryError3 (void)
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
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT_EQUALS(errorHandler.parseErrors.size(), 1u);
    TS_ASSERT_EQUALS(errorHandler.parseErrors[0].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedExpression));
    TestSemanticsHandler::TestExpressionBase* testExpr =
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(a ? <INVALID-EXPR> : c)");
  }

  void testParentheses1 (void)
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
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "a");
  }
  
  void testParentheses2 (void)
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
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "a");
  }
  
  void testParentheses3 (void)
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
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(a * (b + c))");
  }
  
  void testIdentifierArray1 (void)
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
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "a[1]");
  }
  
  void testIdentifierArray2 (void)
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
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "a[(b + c)]");
  }
  
  void testIdentifierArray3 (void)
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
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "((a + b)[1] + c)");
  }
  
  void testFunctionCall (void)
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
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(x = Foo ())");
  }
  
  void testFunctionCall2 (void)
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
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(x = Foo ((a + b), 3.0))");
  }

  void testFunctionCallError (void)
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
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT_EQUALS(errorHandler.parseErrors.size(), 1u);
    TS_ASSERT_EQUALS(errorHandler.parseErrors[0].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedSeparatorOrParenthesis));
    TestSemanticsHandler::TestExpressionBase* testExpr =
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(x = Foo (4.0))");
  }

  void testTypeCtor (void)
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
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(x = int ())");
  }
  
  void testTypeCtor2 (void)
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
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(x = int2 (1, 2))");
  }

  void testArrayCtor (void)
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
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr =
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(x = float[] (1.0, 2.0, 3.0))");
  }

  void testExprInvalid1 (void)
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
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT_EQUALS(errorHandler.parseErrors.size(), 1u);
    TS_ASSERT_EQUALS(errorHandler.parseErrors[0].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedExpression));
    TestSemanticsHandler::TestExpressionBase* testExpr =
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(a + <INVALID-EXPR>)");
  }
};
