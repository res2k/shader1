#include <cxxtest/TestSuite.h>

#include "lexer/LexerErrorHandler.h"
#include "parser/Parser.h"

#include "TestSemanticsHandler.h"

#include "ParserTestTraits.h"

class ParserExprTestSuite : public CxxTest::TestSuite 
{
  class TestParser : public s1::Parser
  {
  public:
    TestParser (s1::Lexer& inputLexer, s1::parser::SemanticsHandler& semanticsHandler)
     : Parser (inputLexer, semanticsHandler) {}
    
    using s1::Parser::Expression;
    using s1::Parser::ParseExpression;
  };
  
public:
  void testIdentifier (void)
  {
    std::istringstream in ("a");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler);
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression ()));
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "a");
  }
  
  void testIdentifierAttr (void)
  {
    std::istringstream in ("a.x");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler);
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression ()));
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "a.x");
  }
  
  void testIdentifierAttr2 (void)
  {
    std::istringstream in ("(a+b).x");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler);
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression ()));
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(a + b).x");
  }
  
  void testIdentifierAttr3 (void)
  {
    std::istringstream in ("a.x+b");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler);
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression ()));
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(a.x + b)");
  }
  
  void testIdentifierAttr4 (void)
  {
    std::istringstream in ("a+b.x");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler);
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression ()));
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(a + b.x)");
  }
  
  void testPrecedence1 (void)
  {
    std::istringstream in ("a+b*c");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler);
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression ()));
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(a + (b * c))");
  }
  
  void testPrecedence2 (void)
  {
    std::istringstream in ("a=b=c");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler);
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression ()));
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(a = (b = c))");
  }
  
  void testPrecedence3 (void)
  {
    std::istringstream in ("a+b==c*d");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler);
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression ()));
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "((a + b) == (c * d))");
  }
  
  void testPrecedence4 (void)
  {
    std::istringstream in ("a=b&&c");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler);
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression ()));
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(a = (b && c))");
  }
  
  void testPrecedence5 (void)
  {
    std::istringstream in ("a&&b||c");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler);
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression ()));
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "((a && b) || c)");
  }
  
  void testPrecedence6 (void)
  {
    std::istringstream in ("a||b&&c&&d");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler);
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression ()));
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(a || (b && (c && d)))");
  }
  
  void testPrecedence7 (void)
  {
    std::istringstream in ("!a&&b");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler);
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression ()));
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(!a && b)");
  }
  
  void testPrecedence8 (void)
  {
    std::istringstream in ("a+b>c*d");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler);
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression ()));
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "((a + b) > (c * d))");
  }
  
  void testPrecedence9 (void)
  {
    std::istringstream in ("a=b?c:d");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler);
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression ()));
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(a = (b ? c : d))");
  }
  
  void testPrecedence10 (void)
  {
    std::istringstream in ("a==b?c:d");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler);
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression ()));
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "((a == b) ? c : d)");
  }
  
  void testUnary1 (void)
  {
    std::istringstream in ("a+-b");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler);
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression ()));
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(a + -b)");
  }
  
  void testTernary1 (void)
  {
    std::istringstream in ("a?b:c");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler);
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression ()));
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(a ? b : c)");
  }
  
  void testTernary2 (void)
  {
    std::istringstream in ("a?b?1:2:c");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler);
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression ()));
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(a ? (b ? 1 : 2) : c)");
  }
  
  void testParentheses1 (void)
  {
    std::istringstream in ("(a)");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler);
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression ()));
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "a");
  }
  
  void testParentheses2 (void)
  {
    std::istringstream in ("((((a))))");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler);
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression ()));
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "a");
  }
  
  void testParentheses3 (void)
  {
    std::istringstream in ("a*(b+c)");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler);
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression ()));
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(a * (b + c))");
  }
  
  void testIdentifierArray1 (void)
  {
    std::istringstream in ("a[1]");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler);
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression ()));
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "a[1]");
  }
  
  void testIdentifierArray2 (void)
  {
    std::istringstream in ("a[b+c]");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler);
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression ()));
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "a[(b + c)]");
  }
  
  void testIdentifierArray3 (void)
  {
    std::istringstream in ("(a+b)[1]+0");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler);
    
    TestSemanticsHandler::ExpressionPtr expr;
    TS_ASSERT_THROWS_NOTHING ((expr = parser.ParseExpression ()));
    TestSemanticsHandler::TestExpressionBase* testExpr = 
      static_cast<TestSemanticsHandler::TestExpressionBase*> (expr.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "((a + b)[1] + 0)");
  }
  
};
