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

#include "TestSemanticsHandlerSloppyIdentifiers.h"

#include "ParserTestTraits.h"
#include "TestParser.h"

#include "../../diagnostics/t/TestDiagnosticsHandler.h"

class ParserExprTestSuite : public CxxTest::TestSuite 
{
  typedef TestSemanticsHandlerSloppyIdentifiers TestSemanticsHandler;
public:
  void testIdentifier (void)
  {
    std::string inStr ("a");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    TestSemanticsHandler::ScopePtr scope (
      semanticsHandler.CreateScope (TestSemanticsHandler::ScopePtr(),
				    TestSemanticsHandler::Global));
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "a");
  }
  
  void testIdentifierAttr (void)
  {
    std::string inStr ("a.x");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    TestSemanticsHandler::ScopePtr scope (
      semanticsHandler.CreateScope (TestSemanticsHandler::ScopePtr(),
				    TestSemanticsHandler::Global));
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "a.x");
  }
  
  void testIdentifierAttr2 (void)
  {
    std::string inStr ("(a+b).x");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    TestSemanticsHandler::ScopePtr scope (
      semanticsHandler.CreateScope (TestSemanticsHandler::ScopePtr(),
				    TestSemanticsHandler::Global));
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(a + b).x");
  }
  
  void testIdentifierAttr3 (void)
  {
    std::string inStr ("a.x+b");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    TestSemanticsHandler::ScopePtr scope (
      semanticsHandler.CreateScope (TestSemanticsHandler::ScopePtr(),
				    TestSemanticsHandler::Global));
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(a.x + b)");
  }
  
  void testIdentifierAttr4 (void)
  {
    std::string inStr ("a+b.x");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    TestSemanticsHandler::ScopePtr scope (
      semanticsHandler.CreateScope (TestSemanticsHandler::ScopePtr(),
				    TestSemanticsHandler::Global));
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(a + b.x)");
  }
  
  void testEvaluationOrder (void)
  {
    std::string inStr ("a-b-c");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    TestSemanticsHandler::ScopePtr scope (
      semanticsHandler.CreateScope (TestSemanticsHandler::ScopePtr(),
				    TestSemanticsHandler::Global));
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "((a - b) - c)");
  }
  
  void testPrecedence1 (void)
  {
    std::string inStr ("a+b*c");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    TestSemanticsHandler::ScopePtr scope (
      semanticsHandler.CreateScope (TestSemanticsHandler::ScopePtr(),
				    TestSemanticsHandler::Global));
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(a + (b * c))");
  }
  
  void testPrecedence2 (void)
  {
    std::string inStr ("a=b=c");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    TestSemanticsHandler::ScopePtr scope (
      semanticsHandler.CreateScope (TestSemanticsHandler::ScopePtr(),
				    TestSemanticsHandler::Global));
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(a = (b = c))");
  }
  
  void testPrecedence3 (void)
  {
    std::string inStr ("a+b==c*d");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    TestSemanticsHandler::ScopePtr scope (
      semanticsHandler.CreateScope (TestSemanticsHandler::ScopePtr(),
				    TestSemanticsHandler::Global));
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "((a + b) == (c * d))");
  }
  
  void testPrecedence4 (void)
  {
    std::string inStr ("a=b&&c");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    TestSemanticsHandler::ScopePtr scope (
      semanticsHandler.CreateScope (TestSemanticsHandler::ScopePtr(),
				    TestSemanticsHandler::Global));
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(a = (b && c))");
  }
  
  void testPrecedence5 (void)
  {
    std::string inStr ("a&&b||c");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    TestSemanticsHandler::ScopePtr scope (
      semanticsHandler.CreateScope (TestSemanticsHandler::ScopePtr(),
				    TestSemanticsHandler::Global));
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "((a && b) || c)");
  }
  
  void testPrecedence6 (void)
  {
    std::string inStr ("a||b&&c&&d");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    TestSemanticsHandler::ScopePtr scope (
      semanticsHandler.CreateScope (TestSemanticsHandler::ScopePtr(),
				    TestSemanticsHandler::Global));
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(a || ((b && c) && d))");
  }
  
  void testPrecedence7 (void)
  {
    std::string inStr ("!a&&b");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    TestSemanticsHandler::ScopePtr scope (
      semanticsHandler.CreateScope (TestSemanticsHandler::ScopePtr(),
				    TestSemanticsHandler::Global));
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(!a && b)");
  }
  
  void testPrecedence8 (void)
  {
    std::string inStr ("a+b>c*d");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    TestSemanticsHandler::ScopePtr scope (
      semanticsHandler.CreateScope (TestSemanticsHandler::ScopePtr(),
				    TestSemanticsHandler::Global));
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "((a + b) > (c * d))");
  }
  
  void testPrecedence9 (void)
  {
    std::string inStr ("a=b?c:d");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    TestSemanticsHandler::ScopePtr scope (
      semanticsHandler.CreateScope (TestSemanticsHandler::ScopePtr(),
				    TestSemanticsHandler::Global));
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(a = (b ? c : d))");
  }
  
  void testPrecedence10 (void)
  {
    std::string inStr ("a==b?c:d");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    TestSemanticsHandler::ScopePtr scope (
      semanticsHandler.CreateScope (TestSemanticsHandler::ScopePtr(),
				    TestSemanticsHandler::Global));
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "((a == b) ? c : d)");
  }
  
  void testUnary1 (void)
  {
    std::string inStr ("a+-b");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    TestSemanticsHandler::ScopePtr scope (
      semanticsHandler.CreateScope (TestSemanticsHandler::ScopePtr(),
				    TestSemanticsHandler::Global));
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(a + -b)");
  }
  
  void testTernary1 (void)
  {
    std::string inStr ("a?b:c");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    TestSemanticsHandler::ScopePtr scope (
      semanticsHandler.CreateScope (TestSemanticsHandler::ScopePtr(),
				    TestSemanticsHandler::Global));
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(a ? b : c)");
  }
  
  void testTernary2 (void)
  {
    std::string inStr ("a?b?1:2:c");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    TestSemanticsHandler::ScopePtr scope (
      semanticsHandler.CreateScope (TestSemanticsHandler::ScopePtr(),
				    TestSemanticsHandler::Global));
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(a ? (b ? 1 : 2) : c)");
  }

  void testTernaryError (void)
  {
    using namespace s1::parser;

    std::string inStr ("a ? b c");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    TestSemanticsHandler::ScopePtr scope (
      semanticsHandler.CreateScope (TestSemanticsHandler::ScopePtr(),
                                    TestSemanticsHandler::Global));

    TestSemanticsHandler::ExpressionPtr expr;
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

    std::string inStr ("a ? b");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    TestSemanticsHandler::ScopePtr scope (
      semanticsHandler.CreateScope (TestSemanticsHandler::ScopePtr(),
                                    TestSemanticsHandler::Global));

    TestSemanticsHandler::ExpressionPtr expr;
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

    std::string inStr ("a ? : c");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    TestSemanticsHandler::ScopePtr scope (
      semanticsHandler.CreateScope (TestSemanticsHandler::ScopePtr(),
                                    TestSemanticsHandler::Global));

    TestSemanticsHandler::ExpressionPtr expr;
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
    std::string inStr ("(a)");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    TestSemanticsHandler::ScopePtr scope (
      semanticsHandler.CreateScope (TestSemanticsHandler::ScopePtr(),
				    TestSemanticsHandler::Global));
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "a");
  }
  
  void testParentheses2 (void)
  {
    std::string inStr ("((((a))))");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    TestSemanticsHandler::ScopePtr scope (
      semanticsHandler.CreateScope (TestSemanticsHandler::ScopePtr(),
				    TestSemanticsHandler::Global));
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "a");
  }
  
  void testParentheses3 (void)
  {
    std::string inStr ("a*(b+c)");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    TestSemanticsHandler::ScopePtr scope (
      semanticsHandler.CreateScope (TestSemanticsHandler::ScopePtr(),
				    TestSemanticsHandler::Global));
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(a * (b + c))");
  }
  
  void testIdentifierArray1 (void)
  {
    std::string inStr ("a[1]");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    TestSemanticsHandler::ScopePtr scope (
      semanticsHandler.CreateScope (TestSemanticsHandler::ScopePtr(),
				    TestSemanticsHandler::Global));
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "a[1]");
  }
  
  void testIdentifierArray2 (void)
  {
    std::string inStr ("a[b+c]");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    TestSemanticsHandler::ScopePtr scope (
      semanticsHandler.CreateScope (TestSemanticsHandler::ScopePtr(),
				    TestSemanticsHandler::Global));
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "a[(b + c)]");
  }
  
  void testIdentifierArray3 (void)
  {
    std::string inStr ("(a+b)[1]+c");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    TestSemanticsHandler::ScopePtr scope (
      semanticsHandler.CreateScope (TestSemanticsHandler::ScopePtr(),
				    TestSemanticsHandler::Global));
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "((a + b)[1] + c)");
  }
  
  void testFunctionCall (void)
  {
    // Need a 'strict' semantics handler for this test
    typedef ::TestSemanticsHandler TestSemanticsHandler;
  
    std::string inStr ("x = Foo ()");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    TestSemanticsHandler::ScopePtr scope (
      semanticsHandler.CreateScope (TestSemanticsHandler::ScopePtr(),
				    TestSemanticsHandler::Global)); 
    s1::semantics::Handler::Scope::FunctionFormalParameters params;
    scope->AddVariable (TestSemanticsHandler::TypePtr (), s1::uc::String ("x"),
			TestSemanticsHandler::ExpressionPtr (), false);
    scope->AddFunction (TestSemanticsHandler::TypePtr (), s1::uc::String ("Foo"), params);
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(x = Foo ())");
  }
  
  void testFunctionCall2 (void)
  {
    // Need a 'strict' semantics handler for this test
    typedef ::TestSemanticsHandler TestSemanticsHandler;
  
    std::string inStr ("x = Foo (a + b, 3.0)");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    TestSemanticsHandler::ScopePtr scope (
      semanticsHandler.CreateScope (TestSemanticsHandler::ScopePtr(),
				    TestSemanticsHandler::Global)); 
    s1::semantics::Handler::Scope::FunctionFormalParameters params;
    scope->AddVariable (TestSemanticsHandler::TypePtr (), s1::uc::String ("a"),
			TestSemanticsHandler::ExpressionPtr (), false);
    scope->AddVariable (TestSemanticsHandler::TypePtr (), s1::uc::String ("b"),
			TestSemanticsHandler::ExpressionPtr (), false);
    scope->AddVariable (TestSemanticsHandler::TypePtr (), s1::uc::String ("x"),
			TestSemanticsHandler::ExpressionPtr (), false);
    scope->AddFunction (TestSemanticsHandler::TypePtr (), s1::uc::String ("Foo"), params);
    
    TestSemanticsHandler::ExpressionPtr expr;
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
    TestSemanticsHandler::ScopePtr scope (
      semanticsHandler.CreateScope (TestSemanticsHandler::ScopePtr(),
                                    TestSemanticsHandler::Global));
    s1::semantics::Handler::Scope::FunctionFormalParameters params;
    scope->AddVariable (TestSemanticsHandler::TypePtr (), s1::uc::String ("x"),
                        TestSemanticsHandler::ExpressionPtr (), false);
    scope->AddFunction (TestSemanticsHandler::TypePtr (), s1::uc::String ("Foo"), params);

    TestSemanticsHandler::ExpressionPtr expr;
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
    std::string inStr ("x = int ()");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    TestSemanticsHandler::ScopePtr scope (
      semanticsHandler.CreateScope (TestSemanticsHandler::ScopePtr(),
				    TestSemanticsHandler::Global)); 
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(x = int ())");
  }
  
  void testTypeCtor2 (void)
  {
    std::string inStr ("x = int2 (1, 2)");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    TestSemanticsHandler::ScopePtr scope (
      semanticsHandler.CreateScope (TestSemanticsHandler::ScopePtr(),
				    TestSemanticsHandler::Global)); 
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(x = int2 (1, 2))");
  }

  void testArrayCtor (void)
  {
    std::string inStr ("x = float[] (1.0, 2.0, 3.0)");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    TestSemanticsHandler::ScopePtr scope (
      semanticsHandler.CreateScope (TestSemanticsHandler::ScopePtr(),
                                    TestSemanticsHandler::Global));

    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestExpressionBase* testExpr =
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(x = float[] (1.0, 2.0, 3.0))");
  }

  void testExprInvalid1 (void)
  {
    using namespace s1::parser;

    std::string inStr ("a+");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    TestSemanticsHandler::ScopePtr scope (
      semanticsHandler.CreateScope (TestSemanticsHandler::ScopePtr(),
                                    TestSemanticsHandler::Global));

    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression (scope)));
    TS_ASSERT_EQUALS(errorHandler.parseErrors.size(), 1u);
    TS_ASSERT_EQUALS(errorHandler.parseErrors[0].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedExpression));
    TestSemanticsHandler::TestExpressionBase* testExpr =
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(a + <INVALID-EXPR>)");
  }
};
