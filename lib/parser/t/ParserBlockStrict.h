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
    TestParser (s1::Lexer& inputLexer, s1::parser::SemanticsHandler& semanticsHandler,
		s1::parser::ErrorHandler& errorHandler)
     : Parser (inputLexer, semanticsHandler, errorHandler) {}
    
    using s1::Parser::ParseBlock;
  };
public:
  void testBlockVarDecl (void)
  {
    using namespace s1::parser;
    
    std::istringstream in ("int a;");
    s1::uc::Stream ustream (in);
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (SemanticsHandler::ScopePtr()));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TestSemanticsHandler::TestScope* testScope = 
      static_cast<TestSemanticsHandler::TestScope*> (block->GetInnerScope().get());
    SemanticsHandler::NamePtr varRequested;
    TS_ASSERT_THROWS_NOTHING(
      varRequested =
	testScope->ResolveIdentifier (s1::uc::String ("a"))
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
    s1::uc::Stream ustream (in);
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (SemanticsHandler::ScopePtr()));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TestSemanticsHandler::TestScope* testScope = 
      static_cast<TestSemanticsHandler::TestScope*> (block->GetInnerScope().get());
    {
      SemanticsHandler::NamePtr varRequested;
      TS_ASSERT_THROWS_NOTHING(
	varRequested =
	  testScope->ResolveIdentifier (s1::uc::String ("a"))
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
	  testScope->ResolveIdentifier (s1::uc::String ("b"))
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
  
  void testBlockVarDeclMulti2 (void)
  {
    using namespace s1::parser;
    
    std::istringstream in ("int a; int b;");
    s1::uc::Stream ustream (in);
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (SemanticsHandler::ScopePtr()));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TestSemanticsHandler::TestScope* testScope = 
      static_cast<TestSemanticsHandler::TestScope*> (block->GetInnerScope().get());
    {
      SemanticsHandler::NamePtr varRequested;
      TS_ASSERT_THROWS_NOTHING(
	varRequested =
	  testScope->ResolveIdentifier (s1::uc::String ("a"))
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
	  testScope->ResolveIdentifier (s1::uc::String ("b"))
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
    s1::uc::Stream ustream (in);
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (SemanticsHandler::ScopePtr()));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TestSemanticsHandler::TestScope* testScope = 
      static_cast<TestSemanticsHandler::TestScope*> (block->GetInnerScope().get());
    SemanticsHandler::NamePtr varRequested;
    TS_ASSERT_THROWS_NOTHING(
      varRequested =
	testScope->ResolveIdentifier (s1::uc::String ("a"))
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
    s1::uc::Stream ustream (in);
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (SemanticsHandler::ScopePtr()));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TestSemanticsHandler::TestScope* testScope = 
      static_cast<TestSemanticsHandler::TestScope*> (block->GetInnerScope().get());
    {
      SemanticsHandler::NamePtr varRequested;
      TS_ASSERT_THROWS_NOTHING(
	varRequested =
	  testScope->ResolveIdentifier (s1::uc::String ("a"))
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
	  testScope->ResolveIdentifier (s1::uc::String ("b"))
      );
      TS_ASSERT_DIFFERS (varRequested, SemanticsHandler::NamePtr ());
    }
  }
  
  void testBlockConstDecl (void)
  {
    using namespace s1::parser;
    
    std::istringstream in ("const int a = 0;");
    s1::uc::Stream ustream (in);
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (SemanticsHandler::ScopePtr()));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TestSemanticsHandler::TestScope* testScope = 
      static_cast<TestSemanticsHandler::TestScope*> (block->GetInnerScope().get());
    SemanticsHandler::NamePtr varRequested;
    TS_ASSERT_THROWS_NOTHING(
      varRequested =
	testScope->ResolveIdentifier (s1::uc::String ("a"))
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
    s1::uc::Stream ustream (in);
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (SemanticsHandler::ScopePtr()));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TestSemanticsHandler::TestScope* testScope = 
      static_cast<TestSemanticsHandler::TestScope*> (block->GetInnerScope().get());
    SemanticsHandler::NamePtr varRequested;
    TS_ASSERT_THROWS_ASSERT(
      testScope->ResolveIdentifier (s1::uc::String ("a")),
      const s1::parser::Exception& e,
      e.GetCode() == s1::parser::IdentifierUndeclared
    );
  }
  
  void testBlockTypedef (void)
  {
    using namespace s1::parser;
    
    std::istringstream in ("typedef int MyInt;");
    s1::uc::Stream ustream (in);
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (SemanticsHandler::ScopePtr()));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TestSemanticsHandler::TestScope* testScope = 
      static_cast<TestSemanticsHandler::TestScope*> (block->GetInnerScope().get());
    SemanticsHandler::NamePtr typeRequested;
    TS_ASSERT_THROWS_NOTHING(
      typeRequested =
	testScope->ResolveIdentifier (s1::uc::String ("MyInt"))
    );
    TS_ASSERT_DIFFERS (typeRequested, SemanticsHandler::NamePtr ());
    TS_ASSERT_EQUALS (typeRequested->GetType(), SemanticsHandler::Name::TypeAlias);
    TestSemanticsHandler::TestName* testName =
      static_cast<TestSemanticsHandler::TestName*> (typeRequested.get());
    TestSemanticsHandler::TestType* testType =
      static_cast<TestSemanticsHandler::TestType*> (testName->valueType.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Base);
    TS_ASSERT_EQUALS (testType->base, s1::parser::SemanticsHandler::Int);
  }
  
  void testBlockTypedefIndirect (void)
  {
    using namespace s1::parser;
    
    std::istringstream in ("typedef int MyInt1; typedef MyInt1 MyInt;");
    s1::uc::Stream ustream (in);
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (SemanticsHandler::ScopePtr()));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TestSemanticsHandler::TestScope* testScope = 
      static_cast<TestSemanticsHandler::TestScope*> (block->GetInnerScope().get());
    SemanticsHandler::NamePtr typeRequested;
    TS_ASSERT_THROWS_NOTHING(
      typeRequested =
	testScope->ResolveIdentifier (s1::uc::String ("MyInt"))
    );
    TS_ASSERT_DIFFERS (typeRequested, SemanticsHandler::NamePtr ());
    TS_ASSERT_EQUALS (typeRequested->GetType(), SemanticsHandler::Name::TypeAlias);
    TestSemanticsHandler::TestName* testName =
      static_cast<TestSemanticsHandler::TestName*> (typeRequested.get());
    TestSemanticsHandler::TestType* testType =
      static_cast<TestSemanticsHandler::TestType*> (testName->valueType.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Base);
    TS_ASSERT_EQUALS (testType->base, s1::parser::SemanticsHandler::Int);
  }
  
  void testBlockVarTypedefDecl (void)
  {
    using namespace s1::parser;
    
    std::istringstream in ("typedef int MyInt; MyInt a;");
    s1::uc::Stream ustream (in);
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (SemanticsHandler::ScopePtr()));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TestSemanticsHandler::TestScope* testScope = 
      static_cast<TestSemanticsHandler::TestScope*> (block->GetInnerScope().get());
    SemanticsHandler::NamePtr varRequested;
    TS_ASSERT_THROWS_NOTHING(
      varRequested =
	testScope->ResolveIdentifier (s1::uc::String ("a"))
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
  
};
