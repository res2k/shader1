#include <cxxtest/TestSuite.h>

#include "base/hash_UnicodeString.h"

#include "lexer/LexerErrorHandler.h"
#include "parser/Exception.h"
#include "parser/Parser.h"

#include "TestSemanticsHandler.h"

#include "ParserTestTraits.h"

class ParserBlockStrictTestSuite : public CxxTest::TestSuite 
{
  class TestParser : public s1::Parser
  {
  public:
    TestParser (s1::Lexer& inputLexer, s1::parser::SemanticsHandler& semanticsHandler)
     : Parser (inputLexer, semanticsHandler) {}
    
    using s1::Parser::ParseBlock;
  };
  
  typedef TestSemanticsHandlerTemplated<0> TestSemanticsHandler;
public:
  void testBlockVarDecl (void)
  {
    using namespace s1::parser;
    
    std::istringstream in ("int a;");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler);
    
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (SemanticsHandler::ScopePtr()));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TestSemanticsHandler::TestScope* testScope = 
      static_cast<TestSemanticsHandler::TestScope*> (block->GetInnerScope().get());
    SemanticsHandler::NamePtr varRequested;
    TS_ASSERT_THROWS_NOTHING(
      varRequested =
	testScope->ResolveIdentifier (UnicodeString ("a"))
    );
    TS_ASSERT_DIFFERS (varRequested, SemanticsHandler::NamePtr ());
    TS_ASSERT_EQUALS (varRequested->GetType(), SemanticsHandler::Name::Variable);
    TestSemanticsHandler::TestName* testName =
      static_cast<TestSemanticsHandler::TestName*> (varRequested.get());
    TS_ASSERT_EQUALS (testName->varValue, SemanticsHandler::ExpressionPtr ());
    TS_ASSERT_EQUALS (testName->varConstant, false);
    TestSemanticsHandler::TestType* testType =
      static_cast<TestSemanticsHandler::TestType*> (testName->valueType.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Base);
    TS_ASSERT_EQUALS (testType->base, s1::parser::SemanticsHandler::Int);
  }
  
  void testBlockVarDeclMulti (void)
  {
    using namespace s1::parser;
    
    std::istringstream in ("int a, b;");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler);
    
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (SemanticsHandler::ScopePtr()));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TestSemanticsHandler::TestScope* testScope = 
      static_cast<TestSemanticsHandler::TestScope*> (block->GetInnerScope().get());
    {
      SemanticsHandler::NamePtr varRequested;
      TS_ASSERT_THROWS_NOTHING(
	varRequested =
	  testScope->ResolveIdentifier (UnicodeString ("a"))
      );
      TS_ASSERT_DIFFERS (varRequested, SemanticsHandler::NamePtr ());
      TS_ASSERT_EQUALS (varRequested->GetType(), SemanticsHandler::Name::Variable);
      TestSemanticsHandler::TestName* testName =
	static_cast<TestSemanticsHandler::TestName*> (varRequested.get());
      TS_ASSERT_EQUALS (testName->varValue, SemanticsHandler::ExpressionPtr ());
      TS_ASSERT_EQUALS (testName->varConstant, false);
      TestSemanticsHandler::TestType* testType =
	static_cast<TestSemanticsHandler::TestType*> (testName->valueType.get());
      TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Base);
      TS_ASSERT_EQUALS (testType->base, s1::parser::SemanticsHandler::Int);
    }
    {
      SemanticsHandler::NamePtr varRequested;
      TS_ASSERT_THROWS_NOTHING(
	varRequested =
	  testScope->ResolveIdentifier (UnicodeString ("b"))
      );
      TS_ASSERT_DIFFERS (varRequested, SemanticsHandler::NamePtr ());
      TS_ASSERT_EQUALS (varRequested->GetType(), SemanticsHandler::Name::Variable);
      TestSemanticsHandler::TestName* testName =
	static_cast<TestSemanticsHandler::TestName*> (varRequested.get());
      TS_ASSERT_EQUALS (testName->varValue, SemanticsHandler::ExpressionPtr ());
      TS_ASSERT_EQUALS (testName->varConstant, false);
      TestSemanticsHandler::TestType* testType =
	static_cast<TestSemanticsHandler::TestType*> (testName->valueType.get());
      TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Base);
      TS_ASSERT_EQUALS (testType->base, s1::parser::SemanticsHandler::Int);
    }
  }
  
  void testBlockVarDeclInit (void)
  {
    using namespace s1::parser;
    
    std::istringstream in ("int a = 1+2;");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler);
    
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (SemanticsHandler::ScopePtr()));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TestSemanticsHandler::TestScope* testScope = 
      static_cast<TestSemanticsHandler::TestScope*> (block->GetInnerScope().get());
    SemanticsHandler::NamePtr varRequested;
    TS_ASSERT_THROWS_NOTHING(
      varRequested =
	testScope->ResolveIdentifier (UnicodeString ("a"))
    );
    TS_ASSERT_DIFFERS (varRequested, SemanticsHandler::NamePtr ());
    TS_ASSERT_EQUALS (varRequested->GetType(), SemanticsHandler::Name::Variable);
    TestSemanticsHandler::TestName* testName =
      static_cast<TestSemanticsHandler::TestName*> (varRequested.get());
    TS_ASSERT_DIFFERS (testName->varValue, SemanticsHandler::ExpressionPtr ());
    TestSemanticsHandler::TestExpressionBase* testExpr =
      static_cast<TestSemanticsHandler::TestExpressionBase*> (testName->varValue.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(1 + 2)");
    TS_ASSERT_EQUALS (testName->varConstant, false);
    TestSemanticsHandler::TestType* testType =
      static_cast<TestSemanticsHandler::TestType*> (testName->valueType.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Base);
    TS_ASSERT_EQUALS (testType->base, s1::parser::SemanticsHandler::Int);
  }
  
  void testBlockVarDeclInitMulti (void)
  {
    using namespace s1::parser;
    
    std::istringstream in ("int a = 1+2, b;");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler);
    
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (SemanticsHandler::ScopePtr()));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TestSemanticsHandler::TestScope* testScope = 
      static_cast<TestSemanticsHandler::TestScope*> (block->GetInnerScope().get());
    {
      SemanticsHandler::NamePtr varRequested;
      TS_ASSERT_THROWS_NOTHING(
	varRequested =
	  testScope->ResolveIdentifier (UnicodeString ("a"))
      );
      TS_ASSERT_DIFFERS (varRequested, SemanticsHandler::NamePtr ());
      TS_ASSERT_EQUALS (varRequested->GetType(), SemanticsHandler::Name::Variable);
      TestSemanticsHandler::TestName* testName =
	static_cast<TestSemanticsHandler::TestName*> (varRequested.get());
      TS_ASSERT_DIFFERS (testName->varValue, SemanticsHandler::ExpressionPtr ());
      TestSemanticsHandler::TestExpressionBase* testExpr =
	static_cast<TestSemanticsHandler::TestExpressionBase*> (testName->varValue.get());
      TS_ASSERT_EQUALS (testExpr->GetExprString(), "(1 + 2)");
      TS_ASSERT_EQUALS (testName->varConstant, false);
      TestSemanticsHandler::TestType* testType =
	static_cast<TestSemanticsHandler::TestType*> (testName->valueType.get());
      TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Base);
      TS_ASSERT_EQUALS (testType->base, s1::parser::SemanticsHandler::Int);
    }
    {
      SemanticsHandler::NamePtr varRequested;
      TS_ASSERT_THROWS_NOTHING(
	varRequested =
	  testScope->ResolveIdentifier (UnicodeString ("b"))
      );
      TS_ASSERT_DIFFERS (varRequested, SemanticsHandler::NamePtr ());
    }
  }
  
  void testBlockConstDecl (void)
  {
    using namespace s1::parser;
    
    std::istringstream in ("const int a = 0;");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler);
    
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (SemanticsHandler::ScopePtr()));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TestSemanticsHandler::TestScope* testScope = 
      static_cast<TestSemanticsHandler::TestScope*> (block->GetInnerScope().get());
    SemanticsHandler::NamePtr varRequested;
    TS_ASSERT_THROWS_NOTHING(
      varRequested =
	testScope->ResolveIdentifier (UnicodeString ("a"))
    );
    TS_ASSERT_DIFFERS (varRequested, SemanticsHandler::NamePtr ());
    TS_ASSERT_EQUALS (varRequested->GetType(), SemanticsHandler::Name::Variable);
    TestSemanticsHandler::TestName* testName =
      static_cast<TestSemanticsHandler::TestName*> (varRequested.get());
    TS_ASSERT_DIFFERS (testName->varValue, SemanticsHandler::ExpressionPtr ());
    TS_ASSERT_EQUALS (testName->varConstant, true);
    TestSemanticsHandler::TestType* testType =
      static_cast<TestSemanticsHandler::TestType*> (testName->valueType.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Base);
    TS_ASSERT_EQUALS (testType->base, s1::parser::SemanticsHandler::Int);
  }
  
  void testBlockConstDeclIncomplete (void)
  {
    using namespace s1::parser;
    
    std::istringstream in ("const int a;");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler);
    
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (SemanticsHandler::ScopePtr()));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TestSemanticsHandler::TestScope* testScope = 
      static_cast<TestSemanticsHandler::TestScope*> (block->GetInnerScope().get());
    SemanticsHandler::NamePtr varRequested;
    TS_ASSERT_THROWS_ASSERT(
      varRequested =
	testScope->ResolveIdentifier (UnicodeString ("a")),
      const s1::parser::Exception& e,
      e.GetCode() == s1::parser::IdentifierUndeclared
    );
  }
  
};
