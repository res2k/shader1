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

class ParserBlockStrictTestSuite : public CxxTest::TestSuite
{
public:
  void testBlockVarDecl (void)
  {
    using namespace s1::parser;

    std::string inStr ("int a;");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);

    s1::semantics::Handler::BlockPtr block (
      semanticsHandler.CreateBlock (s1::semantics::Handler::ScopePtr()));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestScope* testScope =
      static_cast<TestSemanticsHandler::TestScope*> (block->GetInnerScope().get());
    s1::semantics::Handler::NamePtr varRequested;
    TS_ASSERT_THROWS_NOTHING(
      varRequested =
        testScope->ResolveIdentifier (s1::uc::String ("a")).value()
    );
    TS_ASSERT_DIFFERS (varRequested, s1::semantics::Handler::NamePtr ());
    TS_ASSERT_EQUALS (varRequested->GetType(), s1::semantics::Handler::Name::Variable);
    TestSemanticsHandler::TestName* testName =
      static_cast<TestSemanticsHandler::TestName*> (varRequested.get());
    TS_ASSERT_EQUALS (testName->varValue, s1::semantics::Handler::ExpressionPtr ());
    TS_ASSERT_EQUALS (testName->varConstant, false);
    TestSemanticsHandler::TestType* testType =
      static_cast<TestSemanticsHandler::TestType*> (testName->valueType.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Base);
    TS_ASSERT_EQUALS (testType->base, s1::semantics::Handler::Int);
  }

  void testBlockVarDeclMulti (void)
  {
    using namespace s1::parser;

    std::string inStr ("int a, b;");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);

    s1::semantics::Handler::BlockPtr block (
      semanticsHandler.CreateBlock (s1::semantics::Handler::ScopePtr()));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestScope* testScope =
      static_cast<TestSemanticsHandler::TestScope*> (block->GetInnerScope().get());
    {
      s1::semantics::Handler::NamePtr varRequested;
      TS_ASSERT_THROWS_NOTHING(
        varRequested =
          testScope->ResolveIdentifier (s1::uc::String ("a")).value()
      );
      TS_ASSERT_DIFFERS (varRequested, s1::semantics::Handler::NamePtr ());
      TS_ASSERT_EQUALS (varRequested->GetType(), s1::semantics::Handler::Name::Variable);
      TestSemanticsHandler::TestName* testName =
        static_cast<TestSemanticsHandler::TestName*> (varRequested.get());
      TS_ASSERT_EQUALS (testName->varValue, s1::semantics::Handler::ExpressionPtr ());
      TS_ASSERT_EQUALS (testName->varConstant, false);
      TestSemanticsHandler::TestType* testType =
        static_cast<TestSemanticsHandler::TestType*> (testName->valueType.get());
      TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Base);
      TS_ASSERT_EQUALS (testType->base, s1::semantics::Handler::Int);
    }
    {
      s1::semantics::Handler::NamePtr varRequested;
      TS_ASSERT_THROWS_NOTHING(
        varRequested =
          testScope->ResolveIdentifier (s1::uc::String ("b")).value()
      );
      TS_ASSERT_DIFFERS (varRequested, s1::semantics::Handler::NamePtr ());
      TS_ASSERT_EQUALS (varRequested->GetType(), s1::semantics::Handler::Name::Variable);
      TestSemanticsHandler::TestName* testName =
        static_cast<TestSemanticsHandler::TestName*> (varRequested.get());
      TS_ASSERT_EQUALS (testName->varValue, s1::semantics::Handler::ExpressionPtr ());
      TS_ASSERT_EQUALS (testName->varConstant, false);
      TestSemanticsHandler::TestType* testType =
        static_cast<TestSemanticsHandler::TestType*> (testName->valueType.get());
      TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Base);
      TS_ASSERT_EQUALS (testType->base, s1::semantics::Handler::Int);
    }
  }

  void testBlockVarDeclMulti2 (void)
  {
    using namespace s1::parser;

    std::string inStr ("int a; int b;");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);

    s1::semantics::Handler::BlockPtr block (
      semanticsHandler.CreateBlock (s1::semantics::Handler::ScopePtr()));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestScope* testScope =
      static_cast<TestSemanticsHandler::TestScope*> (block->GetInnerScope().get());
    {
      s1::semantics::Handler::NamePtr varRequested;
      TS_ASSERT_THROWS_NOTHING(
        varRequested =
          testScope->ResolveIdentifier (s1::uc::String ("a")).value()
      );
      TS_ASSERT_DIFFERS (varRequested, s1::semantics::Handler::NamePtr ());
      TS_ASSERT_EQUALS (varRequested->GetType(), s1::semantics::Handler::Name::Variable);
      TestSemanticsHandler::TestName* testName =
        static_cast<TestSemanticsHandler::TestName*> (varRequested.get());
      TS_ASSERT_EQUALS (testName->varValue, s1::semantics::Handler::ExpressionPtr ());
      TS_ASSERT_EQUALS (testName->varConstant, false);
      TestSemanticsHandler::TestType* testType =
        static_cast<TestSemanticsHandler::TestType*> (testName->valueType.get());
      TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Base);
      TS_ASSERT_EQUALS (testType->base, s1::semantics::Handler::Int);
    }
    {
      s1::semantics::Handler::NamePtr varRequested;
      TS_ASSERT_THROWS_NOTHING(
        varRequested =
          testScope->ResolveIdentifier (s1::uc::String ("b")).value()
      );
      TS_ASSERT_DIFFERS (varRequested, s1::semantics::Handler::NamePtr ());
      TS_ASSERT_EQUALS (varRequested->GetType(), s1::semantics::Handler::Name::Variable);
      TestSemanticsHandler::TestName* testName =
        static_cast<TestSemanticsHandler::TestName*> (varRequested.get());
      TS_ASSERT_EQUALS (testName->varValue, s1::semantics::Handler::ExpressionPtr ());
      TS_ASSERT_EQUALS (testName->varConstant, false);
      TestSemanticsHandler::TestType* testType =
        static_cast<TestSemanticsHandler::TestType*> (testName->valueType.get());
      TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Base);
      TS_ASSERT_EQUALS (testType->base, s1::semantics::Handler::Int);
    }
  }

  void testBlockVarDeclInit (void)
  {
    using namespace s1::parser;

    std::string inStr ("int a = 1+2;");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);

    s1::semantics::Handler::BlockPtr block (
      semanticsHandler.CreateBlock (s1::semantics::Handler::ScopePtr()));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestScope* testScope =
      static_cast<TestSemanticsHandler::TestScope*> (block->GetInnerScope().get());
    s1::semantics::Handler::NamePtr varRequested;
    TS_ASSERT_THROWS_NOTHING(
      varRequested =
        testScope->ResolveIdentifier (s1::uc::String ("a")).value()
    );
    TS_ASSERT_DIFFERS (varRequested, s1::semantics::Handler::NamePtr ());
    TS_ASSERT_EQUALS (varRequested->GetType(), s1::semantics::Handler::Name::Variable);
    TestSemanticsHandler::TestName* testName =
      static_cast<TestSemanticsHandler::TestName*> (varRequested.get());
    TS_ASSERT_DIFFERS (testName->varValue, s1::semantics::Handler::ExpressionPtr ());
    TestSemanticsHandler::TestExpressionBase* testExpr =
      static_cast<TestSemanticsHandler::TestExpressionBase*> (testName->varValue.get());
    TS_ASSERT_EQUALS (testExpr->GetExprString(), "(1 + 2)");
    TS_ASSERT_EQUALS (testName->varConstant, false);
    TestSemanticsHandler::TestType* testType =
      static_cast<TestSemanticsHandler::TestType*> (testName->valueType.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Base);
    TS_ASSERT_EQUALS (testType->base, s1::semantics::Handler::Int);
  }

    void testBlockVarDeclInvalid (void)
  {
    using namespace s1::parser;

    std::string inStr ("int 0;");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);

    s1::semantics::Handler::BlockPtr block (
      semanticsHandler.CreateBlock (s1::semantics::Handler::ScopePtr()));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TS_ASSERT_EQUALS(errorHandler.parseErrors.size(), 2u);
    TS_ASSERT_EQUALS(errorHandler.parseErrors[0].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedExpression));
    TS_ASSERT_EQUALS(errorHandler.parseErrors[1].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedSemicolon));
    TestSemanticsHandler::TestBlock* testBlock =
      static_cast<TestSemanticsHandler::TestBlock*> (block.get());
    TS_ASSERT_EQUALS(testBlock->GetBlockString(),
                     "  0;\n");
  }

  void testBlockVarDeclInitMulti (void)
  {
    using namespace s1::parser;

    std::string inStr ("int a = 1+2, b;");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);

    s1::semantics::Handler::BlockPtr block (
      semanticsHandler.CreateBlock (s1::semantics::Handler::ScopePtr()));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestScope* testScope =
      static_cast<TestSemanticsHandler::TestScope*> (block->GetInnerScope().get());
    {
      s1::semantics::Handler::NamePtr varRequested;
      TS_ASSERT_THROWS_NOTHING(
        varRequested =
          testScope->ResolveIdentifier (s1::uc::String ("a")).value()
      );
      TS_ASSERT_DIFFERS (varRequested, s1::semantics::Handler::NamePtr ());
      TS_ASSERT_EQUALS (varRequested->GetType(), s1::semantics::Handler::Name::Variable);
      TestSemanticsHandler::TestName* testName =
        static_cast<TestSemanticsHandler::TestName*> (varRequested.get());
      TS_ASSERT_DIFFERS (testName->varValue, s1::semantics::Handler::ExpressionPtr ());
      TestSemanticsHandler::TestExpressionBase* testExpr =
        static_cast<TestSemanticsHandler::TestExpressionBase*> (testName->varValue.get());
      TS_ASSERT_EQUALS (testExpr->GetExprString(), "(1 + 2)");
      TS_ASSERT_EQUALS (testName->varConstant, false);
      TestSemanticsHandler::TestType* testType =
        static_cast<TestSemanticsHandler::TestType*> (testName->valueType.get());
      TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Base);
      TS_ASSERT_EQUALS (testType->base, s1::semantics::Handler::Int);
    }
    {
      s1::semantics::Handler::NamePtr varRequested;
      TS_ASSERT_THROWS_NOTHING(
        varRequested =
          testScope->ResolveIdentifier (s1::uc::String ("b")).value()
      );
      TS_ASSERT_DIFFERS (varRequested, s1::semantics::Handler::NamePtr ());
    }
  }

  void testBlockVarDeclInitMultiError (void)
  {
    using namespace s1::parser;

    std::string inStr ("int a = 1+2, ;");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);

    s1::semantics::Handler::BlockPtr block (
      semanticsHandler.CreateBlock (s1::semantics::Handler::ScopePtr()));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TS_ASSERT_EQUALS(errorHandler.parseErrors.size(), 1u);
    TS_ASSERT_EQUALS(errorHandler.parseErrors[0].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedIdentifier));
    TestSemanticsHandler::TestScope* testScope =
      static_cast<TestSemanticsHandler::TestScope*> (block->GetInnerScope().get());
    {
      s1::semantics::Handler::NamePtr varRequested;
      TS_ASSERT_THROWS_NOTHING(
        varRequested =
          testScope->ResolveIdentifier (s1::uc::String ("a")).value()
      );
      TS_ASSERT_DIFFERS (varRequested, s1::semantics::Handler::NamePtr ());
      TS_ASSERT_EQUALS (varRequested->GetType(), s1::semantics::Handler::Name::Variable);
      TestSemanticsHandler::TestName* testName =
        static_cast<TestSemanticsHandler::TestName*> (varRequested.get());
      TS_ASSERT_DIFFERS (testName->varValue, s1::semantics::Handler::ExpressionPtr ());
      TestSemanticsHandler::TestExpressionBase* testExpr =
        static_cast<TestSemanticsHandler::TestExpressionBase*> (testName->varValue.get());
      TS_ASSERT_EQUALS (testExpr->GetExprString(), "(1 + 2)");
      TS_ASSERT_EQUALS (testName->varConstant, false);
      TestSemanticsHandler::TestType* testType =
        static_cast<TestSemanticsHandler::TestType*> (testName->valueType.get());
      TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Base);
      TS_ASSERT_EQUALS (testType->base, s1::semantics::Handler::Int);
    }
  }

  void testBlockConstDecl (void)
  {
    using namespace s1::parser;

    std::string inStr ("const int a = 0;");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);

    s1::semantics::Handler::BlockPtr block (
      semanticsHandler.CreateBlock (s1::semantics::Handler::ScopePtr()));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestScope* testScope =
      static_cast<TestSemanticsHandler::TestScope*> (block->GetInnerScope().get());
    s1::semantics::Handler::NamePtr varRequested;
    TS_ASSERT_THROWS_NOTHING(
      varRequested =
        testScope->ResolveIdentifier (s1::uc::String ("a")).value()
    );
    TS_ASSERT_DIFFERS (varRequested, s1::semantics::Handler::NamePtr ());
    TS_ASSERT_EQUALS (varRequested->GetType(), s1::semantics::Handler::Name::Variable);
    TestSemanticsHandler::TestName* testName =
      static_cast<TestSemanticsHandler::TestName*> (varRequested.get());
    TS_ASSERT_DIFFERS (testName->varValue, s1::semantics::Handler::ExpressionPtr ());
    TS_ASSERT_EQUALS (testName->varConstant, true);
    TestSemanticsHandler::TestType* testType =
      static_cast<TestSemanticsHandler::TestType*> (testName->valueType.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Base);
    TS_ASSERT_EQUALS (testType->base, s1::semantics::Handler::Int);
  }

  void testBlockConstDeclIncomplete (void)
  {
    using namespace s1::parser;

    std::string inStr ("const int a;");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);

    s1::semantics::Handler::BlockPtr block (
      semanticsHandler.CreateBlock (s1::semantics::Handler::ScopePtr()));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TS_ASSERT_EQUALS(errorHandler.parseErrors.size(), 2u);
    TS_ASSERT_EQUALS(errorHandler.parseErrors[0].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedAssign));
    TS_ASSERT_EQUALS(errorHandler.parseErrors[1].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedExpression));
    TestSemanticsHandler::TestScope* testScope =
      static_cast<TestSemanticsHandler::TestScope*> (block->GetInnerScope().get());
    // Parsing will produce variable w/o initializer
    s1::semantics::Handler::NamePtr varRequested;
    TS_ASSERT_THROWS_NOTHING(
      varRequested =
        testScope->ResolveIdentifier (s1::uc::String ("a")).value()
    );
    TS_ASSERT_DIFFERS (varRequested, s1::semantics::Handler::NamePtr ());
    TS_ASSERT_EQUALS (varRequested->GetType(), s1::semantics::Handler::Name::Variable);
    TestSemanticsHandler::TestName* testName =
      static_cast<TestSemanticsHandler::TestName*> (varRequested.get());
    TS_ASSERT_EQUALS (testName->varValue, s1::semantics::Handler::ExpressionPtr ());
    TS_ASSERT_EQUALS (testName->varConstant, true);
    TestSemanticsHandler::TestType* testType =
      static_cast<TestSemanticsHandler::TestType*> (testName->valueType.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Base);
    TS_ASSERT_EQUALS (testType->base, s1::semantics::Handler::Int);
  }

  void testBlockTypedef (void)
  {
    using namespace s1::parser;

    std::string inStr ("typedef int MyInt;");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);

    s1::semantics::Handler::BlockPtr block (
      semanticsHandler.CreateBlock (s1::semantics::Handler::ScopePtr()));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TestSemanticsHandler::TestScope* testScope =
      static_cast<TestSemanticsHandler::TestScope*> (block->GetInnerScope().get());
    s1::semantics::Handler::NamePtr typeRequested;
    TS_ASSERT_THROWS_NOTHING(
      typeRequested =
        testScope->ResolveIdentifier (s1::uc::String ("MyInt")).value()
    );
    TS_ASSERT_DIFFERS (typeRequested, s1::semantics::Handler::NamePtr ());
    TS_ASSERT_EQUALS (typeRequested->GetType(), s1::semantics::Handler::Name::TypeAlias);
    TestSemanticsHandler::TestName* testName =
      static_cast<TestSemanticsHandler::TestName*> (typeRequested.get());
    TestSemanticsHandler::TestType* testType =
      static_cast<TestSemanticsHandler::TestType*> (testName->valueType.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Base);
    TS_ASSERT_EQUALS (testType->base, s1::semantics::Handler::Int);
  }

  void testBlockTypedefIndirect (void)
  {
    using namespace s1::parser;

    std::string inStr ("typedef int MyInt1; typedef MyInt1 MyInt;");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);

    s1::semantics::Handler::BlockPtr block (
      semanticsHandler.CreateBlock (s1::semantics::Handler::ScopePtr()));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TestSemanticsHandler::TestScope* testScope =
      static_cast<TestSemanticsHandler::TestScope*> (block->GetInnerScope().get());
    s1::semantics::Handler::NamePtr typeRequested;
    TS_ASSERT_THROWS_NOTHING(
      typeRequested =
        testScope->ResolveIdentifier (s1::uc::String ("MyInt")).value()
    );
    TS_ASSERT_DIFFERS (typeRequested, s1::semantics::Handler::NamePtr ());
    TS_ASSERT_EQUALS (typeRequested->GetType(), s1::semantics::Handler::Name::TypeAlias);
    TestSemanticsHandler::TestName* testName =
      static_cast<TestSemanticsHandler::TestName*> (typeRequested.get());
    TestSemanticsHandler::TestType* testType =
      static_cast<TestSemanticsHandler::TestType*> (testName->valueType.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Base);
    TS_ASSERT_EQUALS (testType->base, s1::semantics::Handler::Int);
  }

  void testBlockVarTypedefDecl (void)
  {
    using namespace s1::parser;

    std::string inStr ("typedef int MyInt; MyInt a;");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);

    s1::semantics::Handler::BlockPtr block (
      semanticsHandler.CreateBlock (s1::semantics::Handler::ScopePtr()));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TestSemanticsHandler::TestScope* testScope =
      static_cast<TestSemanticsHandler::TestScope*> (block->GetInnerScope().get());
    s1::semantics::Handler::NamePtr varRequested;
    TS_ASSERT_THROWS_NOTHING(
      varRequested =
        testScope->ResolveIdentifier (s1::uc::String ("a")).value()
    );
    TS_ASSERT_DIFFERS (varRequested, s1::semantics::Handler::NamePtr ());
    TS_ASSERT_EQUALS (varRequested->GetType(), s1::semantics::Handler::Name::Variable);
    TestSemanticsHandler::TestName* testName =
      static_cast<TestSemanticsHandler::TestName*> (varRequested.get());
    TS_ASSERT_EQUALS (testName->varValue, s1::semantics::Handler::ExpressionPtr ());
    TS_ASSERT_EQUALS (testName->varConstant, false);
    TestSemanticsHandler::TestType* testType =
      static_cast<TestSemanticsHandler::TestType*> (testName->valueType.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Base);
    TS_ASSERT_EQUALS (testType->base, s1::semantics::Handler::Int);
  }

  void testBlockTypedefInvalid1 (void)
  {
    using namespace s1::parser;

    std::string inStr ("typedef 0 MyInt;");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);

    s1::semantics::Handler::BlockPtr block (
      semanticsHandler.CreateBlock (s1::semantics::Handler::ScopePtr()));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TestSemanticsHandler::TestScope* testScope =
      static_cast<TestSemanticsHandler::TestScope*> (block->GetInnerScope().get());
    s1::semantics::Handler::NamePtr typeRequested;
    TS_ASSERT_THROWS_NOTHING(
      typeRequested =
        testScope->ResolveIdentifier (s1::uc::String ("MyInt")).value()
    );
    TS_ASSERT_DIFFERS (typeRequested, s1::semantics::Handler::NamePtr ());
    TS_ASSERT_EQUALS (typeRequested->GetType(), s1::semantics::Handler::Name::TypeAlias);
    TestSemanticsHandler::TestName* testName =
      static_cast<TestSemanticsHandler::TestName*> (typeRequested.get());
    TestSemanticsHandler::TestType* testType =
      static_cast<TestSemanticsHandler::TestType*> (testName->valueType.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Base);
    TS_ASSERT_EQUALS (testType->base, s1::semantics::Handler::Invalid);
  }

  void testBlockTypedefInvalid2 (void)
  {
    using namespace s1::parser;

    std::string inStr ("typedef int float; float a;");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);

    s1::semantics::Handler::BlockPtr block (
      semanticsHandler.CreateBlock (s1::semantics::Handler::ScopePtr()));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TestSemanticsHandler::TestScope* testScope =
      static_cast<TestSemanticsHandler::TestScope*> (block->GetInnerScope().get());

    s1::semantics::Handler::NamePtr typeRequested;
    TS_ASSERT_THROWS_NOTHING(
      typeRequested =
        testScope->ResolveIdentifier (s1::uc::String ("float")).value()
    );
    TS_ASSERT_DIFFERS (typeRequested, s1::semantics::Handler::NamePtr ());

    s1::semantics::Handler::NamePtr varRequested;
    TS_ASSERT_THROWS_NOTHING(
      varRequested =
        testScope->ResolveIdentifier (s1::uc::String ("a")).value()
    );
    TS_ASSERT_DIFFERS (varRequested, s1::semantics::Handler::NamePtr ());
    TS_ASSERT_EQUALS (varRequested->GetType(), s1::semantics::Handler::Name::Variable);
    TestSemanticsHandler::TestName* testName =
      static_cast<TestSemanticsHandler::TestName*> (varRequested.get());
    TS_ASSERT_EQUALS (testName->varValue, s1::semantics::Handler::ExpressionPtr ());
    TS_ASSERT_EQUALS (testName->varConstant, false);
    TestSemanticsHandler::TestType* testType =
      static_cast<TestSemanticsHandler::TestType*> (testName->valueType.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Base);
    TS_ASSERT_EQUALS (testType->base, s1::semantics::Handler::Float);
  }

  void testBlockTypedefInvalid3 (void)
  {
    using namespace s1::parser;

    std::string inStr ("typedef int;");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);

    s1::semantics::Handler::BlockPtr block (
      semanticsHandler.CreateBlock (s1::semantics::Handler::ScopePtr()));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
  }
};
