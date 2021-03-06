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
#include "../../semantics/t/print_semantics_BaseType.h"

BOOST_AUTO_TEST_SUITE(ParserBlockStrict)

BOOST_AUTO_TEST_CASE(BlockVarDecl)
{
  using namespace s1::parser;

  std::string inStr ("int a;");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (s1::semantics::ScopePtr()));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  TestSemanticsHandler::TestScope* testScope =
    static_cast<TestSemanticsHandler::TestScope*> (block->GetInnerScope().get());
  s1::semantics::NamePtr nameRequested;
  BOOST_CHECK_NO_THROW(
    nameRequested =
      testScope->ResolveIdentifier (s1::uc::String ("a")).value()
  );
  BOOST_CHECK_NE (nameRequested, s1::semantics::NamePtr ());
  BOOST_CHECK_EQUAL (nameRequested->GetType(), s1::semantics::Name::Variable);
  auto varRequested = s1::semantics::NameVariable::upcast (nameRequested.get());
  BOOST_CHECK_EQUAL (varRequested->GetValue(), s1::semantics::ExpressionPtr ());
  BOOST_CHECK_EQUAL (varRequested->IsConstant(), false);
  auto testType = varRequested->GetValueType();
  BOOST_CHECK_EQUAL (testType->GetTypeClass(), s1::semantics::Type::Base);
  BOOST_CHECK_EQUAL (testType->GetBaseType(), s1::semantics::BaseType::Int);
}

BOOST_AUTO_TEST_CASE(BlockVarDeclMulti)
{
  using namespace s1::parser;

  std::string inStr ("int a, b;");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (s1::semantics::ScopePtr()));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  TestSemanticsHandler::TestScope* testScope =
    static_cast<TestSemanticsHandler::TestScope*> (block->GetInnerScope().get());
  {
    s1::semantics::NamePtr nameRequested;
    BOOST_CHECK_NO_THROW(
      nameRequested =
        testScope->ResolveIdentifier (s1::uc::String ("a")).value()
    );
    BOOST_CHECK_NE (nameRequested, s1::semantics::NamePtr ());
    BOOST_CHECK_EQUAL (nameRequested->GetType(), s1::semantics::Name::Variable);
    auto varRequested = s1::semantics::NameVariable::upcast (nameRequested.get());
    BOOST_CHECK_EQUAL (varRequested->GetValue(), s1::semantics::ExpressionPtr ());
    BOOST_CHECK_EQUAL (varRequested->IsConstant(), false);
    auto testType = varRequested->GetValueType();
    BOOST_CHECK_EQUAL (testType->GetTypeClass(), s1::semantics::Type::Base);
    BOOST_CHECK_EQUAL (testType->GetBaseType(), s1::semantics::BaseType::Int);
  }
  {
    s1::semantics::NamePtr nameRequested;
    BOOST_CHECK_NO_THROW(
      nameRequested =
        testScope->ResolveIdentifier (s1::uc::String ("b")).value()
    );
    BOOST_CHECK_NE (nameRequested, s1::semantics::NamePtr ());
    BOOST_CHECK_EQUAL (nameRequested->GetType(), s1::semantics::Name::Variable);
    auto varRequested = s1::semantics::NameVariable::upcast (nameRequested.get());
    BOOST_CHECK_EQUAL (varRequested->GetValue(), s1::semantics::ExpressionPtr ());
    BOOST_CHECK_EQUAL (varRequested->IsConstant(), false);
    auto testType = varRequested->GetValueType();
    BOOST_CHECK_EQUAL (testType->GetTypeClass(), s1::semantics::Type::Base);
    BOOST_CHECK_EQUAL (testType->GetBaseType(), s1::semantics::BaseType::Int);
  }
}

BOOST_AUTO_TEST_CASE(BlockVarDeclMulti2)
{
  using namespace s1::parser;

  std::string inStr ("int a; int b;");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (s1::semantics::ScopePtr()));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  TestSemanticsHandler::TestScope* testScope =
    static_cast<TestSemanticsHandler::TestScope*> (block->GetInnerScope().get());
  {
    s1::semantics::NamePtr nameRequested;
    BOOST_CHECK_NO_THROW(
      nameRequested =
        testScope->ResolveIdentifier (s1::uc::String ("a")).value()
    );
    BOOST_CHECK_NE (nameRequested, s1::semantics::NamePtr ());
    BOOST_CHECK_EQUAL (nameRequested->GetType(), s1::semantics::Name::Variable);
    auto varRequested = s1::semantics::NameVariable::upcast (nameRequested.get());
    BOOST_CHECK_EQUAL (varRequested->GetValue(), s1::semantics::ExpressionPtr ());
    BOOST_CHECK_EQUAL (varRequested->IsConstant(), false);
    auto testType = varRequested->GetValueType();
    BOOST_CHECK_EQUAL (testType->GetTypeClass(), s1::semantics::Type::Base);
    BOOST_CHECK_EQUAL (testType->GetBaseType(), s1::semantics::BaseType::Int);
  }
  {
    s1::semantics::NamePtr nameRequested;
    BOOST_CHECK_NO_THROW(
      nameRequested =
        testScope->ResolveIdentifier (s1::uc::String ("b")).value()
    );
    BOOST_CHECK_NE (nameRequested, s1::semantics::NamePtr ());
    BOOST_CHECK_EQUAL (nameRequested->GetType(), s1::semantics::Name::Variable);
    auto varRequested = s1::semantics::NameVariable::upcast (nameRequested.get());
    BOOST_CHECK_EQUAL (varRequested->GetValue(), s1::semantics::ExpressionPtr ());
    BOOST_CHECK_EQUAL (varRequested->IsConstant(), false);
    auto testType = varRequested->GetValueType();
    BOOST_CHECK_EQUAL (testType->GetTypeClass(), s1::semantics::Type::Base);
    BOOST_CHECK_EQUAL (testType->GetBaseType(), s1::semantics::BaseType::Int);
  }
}

BOOST_AUTO_TEST_CASE(BlockVarDeclInit)
{
  using namespace s1::parser;

  std::string inStr ("int a = 1+2;");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (s1::semantics::ScopePtr()));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  TestSemanticsHandler::TestScope* testScope =
    static_cast<TestSemanticsHandler::TestScope*> (block->GetInnerScope().get());
  s1::semantics::NamePtr nameRequested;
  BOOST_CHECK_NO_THROW(
    nameRequested =
      testScope->ResolveIdentifier (s1::uc::String ("a")).value()
  );
  BOOST_CHECK_NE (nameRequested, s1::semantics::NamePtr ());
  BOOST_CHECK_EQUAL (nameRequested->GetType(), s1::semantics::Name::Variable);
  auto varRequested = s1::semantics::NameVariable::upcast (nameRequested.get());
  BOOST_CHECK_NE (varRequested->GetValue(), s1::semantics::ExpressionPtr ());
  TestSemanticsHandler::TestExpressionBase* testExpr =
    static_cast<TestSemanticsHandler::TestExpressionBase*> (varRequested->GetValue());
  BOOST_CHECK_EQUAL (testExpr->GetExprString(), "(1 + 2)");
  BOOST_CHECK_EQUAL (varRequested->IsConstant(), false);
  auto testType = varRequested->GetValueType();
  BOOST_CHECK_EQUAL (testType->GetTypeClass(), s1::semantics::Type::Base);
  BOOST_CHECK_EQUAL (testType->GetBaseType(), s1::semantics::BaseType::Int);
}

  BOOST_AUTO_TEST_CASE(BlockVarDeclInvalid)
{
  using namespace s1::parser;

  std::string inStr ("int 0;");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (s1::semantics::ScopePtr()));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size(), 2u);
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedExpression));
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[1].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedSemicolon));
  TestSemanticsHandler::TestBlock* testBlock =
    static_cast<TestSemanticsHandler::TestBlock*> (block.get());
  BOOST_CHECK_EQUAL(testBlock->GetBlockString(),
                    "  0;\n");
}

BOOST_AUTO_TEST_CASE(BlockVarDeclInitMulti)
{
  using namespace s1::parser;

  std::string inStr ("int a = 1+2, b;");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (s1::semantics::ScopePtr()));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  TestSemanticsHandler::TestScope* testScope =
    static_cast<TestSemanticsHandler::TestScope*> (block->GetInnerScope().get());
  {
    s1::semantics::NamePtr nameRequested;
    BOOST_CHECK_NO_THROW(
      nameRequested =
        testScope->ResolveIdentifier (s1::uc::String ("a")).value()
    );
    BOOST_CHECK_NE (nameRequested, s1::semantics::NamePtr ());
    BOOST_CHECK_EQUAL (nameRequested->GetType(), s1::semantics::Name::Variable);
    auto varRequested = s1::semantics::NameVariable::upcast (nameRequested.get());
    BOOST_CHECK_NE (varRequested->GetValue(), s1::semantics::ExpressionPtr ());
    TestSemanticsHandler::TestExpressionBase* testExpr =
      static_cast<TestSemanticsHandler::TestExpressionBase*> (varRequested->GetValue());
    BOOST_CHECK_EQUAL (testExpr->GetExprString(), "(1 + 2)");
    BOOST_CHECK_EQUAL (varRequested->IsConstant(), false);
    auto testType = varRequested->GetValueType();
    BOOST_CHECK_EQUAL (testType->GetTypeClass(), s1::semantics::Type::Base);
    BOOST_CHECK_EQUAL (testType->GetBaseType(), s1::semantics::BaseType::Int);
  }
  {
    s1::semantics::NamePtr nameRequested;
    BOOST_CHECK_NO_THROW(
      nameRequested =
        testScope->ResolveIdentifier (s1::uc::String ("b")).value()
    );
    BOOST_CHECK_NE (nameRequested, s1::semantics::NamePtr ());
  }
}

BOOST_AUTO_TEST_CASE(BlockVarDeclInitMultiError)
{
  using namespace s1::parser;

  std::string inStr ("int a = 1+2, ;");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (s1::semantics::ScopePtr()));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size(), 1u);
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedIdentifier));
  TestSemanticsHandler::TestScope* testScope =
    static_cast<TestSemanticsHandler::TestScope*> (block->GetInnerScope().get());
  {
    s1::semantics::NamePtr nameRequested;
    BOOST_CHECK_NO_THROW(
      nameRequested =
        testScope->ResolveIdentifier (s1::uc::String ("a")).value()
    );
    BOOST_CHECK_NE (nameRequested, s1::semantics::NamePtr ());
    BOOST_CHECK_EQUAL (nameRequested->GetType(), s1::semantics::Name::Variable);
    auto varRequested = s1::semantics::NameVariable::upcast (nameRequested.get());
    BOOST_CHECK_NE (varRequested->GetValue(), s1::semantics::ExpressionPtr ());
    TestSemanticsHandler::TestExpressionBase* testExpr =
      static_cast<TestSemanticsHandler::TestExpressionBase*> (varRequested->GetValue());
    BOOST_CHECK_EQUAL (testExpr->GetExprString(), "(1 + 2)");
    BOOST_CHECK_EQUAL (varRequested->IsConstant(), false);
    auto testType = varRequested->GetValueType();
    BOOST_CHECK_EQUAL (testType->GetTypeClass(), s1::semantics::Type::Base);
    BOOST_CHECK_EQUAL (testType->GetBaseType(), s1::semantics::BaseType::Int);
  }
}

BOOST_AUTO_TEST_CASE(BlockConstDecl)
{
  using namespace s1::parser;

  std::string inStr ("const int a = 0;");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (s1::semantics::ScopePtr()));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  TestSemanticsHandler::TestScope* testScope =
    static_cast<TestSemanticsHandler::TestScope*> (block->GetInnerScope().get());
  s1::semantics::NamePtr nameRequested;
  BOOST_CHECK_NO_THROW(
    nameRequested =
      testScope->ResolveIdentifier (s1::uc::String ("a")).value()
  );
  BOOST_CHECK_NE (nameRequested, s1::semantics::NamePtr ());
  BOOST_CHECK_EQUAL (nameRequested->GetType(), s1::semantics::Name::Variable);
  auto varRequested = s1::semantics::NameVariable::upcast (nameRequested.get());
  BOOST_CHECK_NE (varRequested->GetValue(), s1::semantics::ExpressionPtr ());
  BOOST_CHECK_EQUAL (varRequested->IsConstant(), true);
  auto testType = varRequested->GetValueType();
  BOOST_CHECK_EQUAL (testType->GetTypeClass(), s1::semantics::Type::Base);
  BOOST_CHECK_EQUAL (testType->GetBaseType(), s1::semantics::BaseType::Int);
}

BOOST_AUTO_TEST_CASE(BlockConstDeclIncomplete)
{
  using namespace s1::parser;

  std::string inStr ("const int a;");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (s1::semantics::ScopePtr()));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size(), 2u);
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedAssign));
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[1].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedExpression));
  TestSemanticsHandler::TestScope* testScope =
    static_cast<TestSemanticsHandler::TestScope*> (block->GetInnerScope().get());
  // Parsing will produce variable w/o initializer
  s1::semantics::NamePtr nameRequested;
  BOOST_CHECK_NO_THROW(
    nameRequested =
      testScope->ResolveIdentifier (s1::uc::String ("a")).value()
  );
  BOOST_CHECK_NE (nameRequested, s1::semantics::NamePtr ());
  BOOST_CHECK_EQUAL (nameRequested->GetType(), s1::semantics::Name::Variable);
  auto varRequested = s1::semantics::NameVariable::upcast (nameRequested.get());
  BOOST_CHECK_EQUAL (varRequested->GetValue(), s1::semantics::ExpressionPtr ());
  BOOST_CHECK_EQUAL (varRequested->IsConstant(), true);
  auto testType = varRequested->GetValueType();
  BOOST_CHECK_EQUAL (testType->GetTypeClass(), s1::semantics::Type::Base);
  BOOST_CHECK_EQUAL (testType->GetBaseType(), s1::semantics::BaseType::Int);
}

BOOST_AUTO_TEST_CASE(BlockTypedef)
{
  using namespace s1::parser;

  std::string inStr ("typedef int MyInt;");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (s1::semantics::ScopePtr()));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  TestSemanticsHandler::TestScope* testScope =
    static_cast<TestSemanticsHandler::TestScope*> (block->GetInnerScope().get());
  s1::semantics::NamePtr nameRequested;
  BOOST_CHECK_NO_THROW(
    nameRequested =
      testScope->ResolveIdentifier (s1::uc::String ("MyInt")).value()
  );
  BOOST_CHECK_NE (nameRequested, s1::semantics::NamePtr ());
  BOOST_CHECK_EQUAL (nameRequested->GetType(), s1::semantics::Name::TypeAlias);
  auto typeRequested = s1::semantics::NameTypeAlias::upcast (nameRequested.get());
  auto testType = typeRequested->GetAliasedType();
  BOOST_CHECK_EQUAL (testType->GetTypeClass(), s1::semantics::Type::Base);
  BOOST_CHECK_EQUAL (testType->GetBaseType(), s1::semantics::BaseType::Int);
}

BOOST_AUTO_TEST_CASE(BlockTypedefIndirect)
{
  using namespace s1::parser;

  std::string inStr ("typedef int MyInt1; typedef MyInt1 MyInt;");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (s1::semantics::ScopePtr()));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  TestSemanticsHandler::TestScope* testScope =
    static_cast<TestSemanticsHandler::TestScope*> (block->GetInnerScope().get());
  s1::semantics::NamePtr nameRequested;
  BOOST_CHECK_NO_THROW(
    nameRequested =
      testScope->ResolveIdentifier (s1::uc::String ("MyInt")).value()
  );
  BOOST_CHECK_NE (nameRequested, s1::semantics::NamePtr ());
  BOOST_CHECK_EQUAL (nameRequested->GetType(), s1::semantics::Name::TypeAlias);
  auto typeRequested = s1::semantics::NameTypeAlias::upcast (nameRequested.get());
  auto testType = typeRequested->GetAliasedType();
  BOOST_CHECK_EQUAL (testType->GetTypeClass(), s1::semantics::Type::Base);
  BOOST_CHECK_EQUAL (testType->GetBaseType(), s1::semantics::BaseType::Int);
}

BOOST_AUTO_TEST_CASE(BlockVarTypedefDecl)
{
  using namespace s1::parser;

  std::string inStr ("typedef int MyInt; MyInt a;");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (s1::semantics::ScopePtr()));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  TestSemanticsHandler::TestScope* testScope =
    static_cast<TestSemanticsHandler::TestScope*> (block->GetInnerScope().get());
  s1::semantics::NamePtr nameRequested;
  BOOST_CHECK_NO_THROW(
    nameRequested =
      testScope->ResolveIdentifier (s1::uc::String ("a")).value()
  );
  BOOST_CHECK_NE (nameRequested, s1::semantics::NamePtr ());
  BOOST_CHECK_EQUAL (nameRequested->GetType(), s1::semantics::Name::Variable);
  auto varRequested = s1::semantics::NameVariable::upcast (nameRequested.get());
  BOOST_CHECK_EQUAL (varRequested->GetValue(), s1::semantics::ExpressionPtr ());
  BOOST_CHECK_EQUAL (varRequested->IsConstant(), false);
  auto testType = varRequested->GetValueType();
  BOOST_CHECK_EQUAL (testType->GetTypeClass(), s1::semantics::Type::Base);
  BOOST_CHECK_EQUAL (testType->GetBaseType(), s1::semantics::BaseType::Int);
}

BOOST_AUTO_TEST_CASE(BlockTypedefInvalid1)
{
  using namespace s1::parser;

  std::string inStr ("typedef 0 MyInt;");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (s1::semantics::ScopePtr()));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  TestSemanticsHandler::TestScope* testScope =
    static_cast<TestSemanticsHandler::TestScope*> (block->GetInnerScope().get());
  s1::semantics::NamePtr nameRequested;
  BOOST_CHECK_NO_THROW(
    nameRequested =
      testScope->ResolveIdentifier (s1::uc::String ("MyInt")).value()
  );
  BOOST_CHECK_NE (nameRequested, s1::semantics::NamePtr ());
  BOOST_CHECK_EQUAL (nameRequested->GetType(), s1::semantics::Name::TypeAlias);
  auto typeRequested = s1::semantics::NameTypeAlias::upcast (nameRequested.get());
  auto testType = typeRequested->GetAliasedType();
  BOOST_CHECK_EQUAL (testType->GetTypeClass(), s1::semantics::Type::Base);
  BOOST_CHECK_EQUAL (testType->GetBaseType(), s1::semantics::BaseType::Invalid);
}

BOOST_AUTO_TEST_CASE(BlockTypedefInvalid2)
{
  using namespace s1::parser;

  std::string inStr ("typedef int float; float a;");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (s1::semantics::ScopePtr()));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  TestSemanticsHandler::TestScope* testScope =
    static_cast<TestSemanticsHandler::TestScope*> (block->GetInnerScope().get());

  s1::semantics::NamePtr nameRequested;
  BOOST_CHECK_NO_THROW(
    nameRequested =
      testScope->ResolveIdentifier (s1::uc::String ("float")).value()
  );
  BOOST_CHECK_NE (nameRequested, s1::semantics::NamePtr ());

  BOOST_CHECK_NO_THROW(
    nameRequested =
      testScope->ResolveIdentifier (s1::uc::String ("a")).value()
  );
  BOOST_CHECK_NE (nameRequested, s1::semantics::NamePtr ());
  BOOST_CHECK_EQUAL (nameRequested->GetType(), s1::semantics::Name::Variable);
  auto varRequested = s1::semantics::NameVariable::upcast (nameRequested.get());
  BOOST_CHECK_EQUAL (varRequested->GetValue(), s1::semantics::ExpressionPtr ());
  BOOST_CHECK_EQUAL (varRequested->IsConstant(), false);
  auto testType = varRequested->GetValueType();
  BOOST_CHECK_EQUAL (testType->GetTypeClass(), s1::semantics::Type::Base);
  BOOST_CHECK_EQUAL (testType->GetBaseType(), s1::semantics::BaseType::Float);
}

BOOST_AUTO_TEST_CASE(BlockTypedefInvalid3)
{
  using namespace s1::parser;

  std::string inStr ("typedef int;");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (s1::semantics::ScopePtr()));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
}

BOOST_AUTO_TEST_SUITE_END()
