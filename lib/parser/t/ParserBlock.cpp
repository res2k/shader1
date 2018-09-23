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

BOOST_AUTO_TEST_SUITE(ParserBlock)

BOOST_AUTO_TEST_CASE(BlockExpr)
{
  using namespace s1::parser;

  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto blockScope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                                  s1::semantics::ScopeLevel::Global);
  blockScope->AddVariable (intType, "a", s1::semantics::ExpressionPtr(), false);
  blockScope->AddVariable (intType, "b", s1::semantics::ExpressionPtr(), false);

  std::string inStr ("a = b;");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (blockScope));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  TestSemanticsHandler::TestBlock* testBlock =
    static_cast<TestSemanticsHandler::TestBlock*> (block.get());
  BOOST_CHECK_EQUAL(testBlock->GetBlockString(),
                    "  (a = b);\n");
}

BOOST_AUTO_TEST_CASE(BlockStatementIncomplete)
{
  using namespace s1::parser;

  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto blockScope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                                  s1::semantics::ScopeLevel::Global);
  blockScope->AddVariable (intType, "a", s1::semantics::ExpressionPtr(), false);
  blockScope->AddVariable (intType, "b", s1::semantics::ExpressionPtr(), false);

  std::string inStr ("a = b");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (blockScope));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size(), 1u);
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedSemicolon));
  TestSemanticsHandler::TestBlock* testBlock =
    static_cast<TestSemanticsHandler::TestBlock*> (block.get());
  BOOST_CHECK_EQUAL(testBlock->GetBlockString(),
                    "  (a = b);\n");
}

BOOST_AUTO_TEST_CASE(BlockBranch)
{
  using namespace s1::parser;

  TestSemanticsHandler semanticsHandler;
  auto boolType = semanticsHandler.CreateType (s1::semantics::BaseType::Bool);
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto blockScope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                                  s1::semantics::ScopeLevel::Global);
  blockScope->AddVariable (boolType, "a", s1::semantics::ExpressionPtr(), false);
  blockScope->AddVariable (intType, "c", s1::semantics::ExpressionPtr(), false);
  blockScope->AddVariable (intType, "d", s1::semantics::ExpressionPtr(), false);

  std::string inStr ("if (a) { c = d; }");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (blockScope));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  TestSemanticsHandler::TestBlock* testBlock =
    static_cast<TestSemanticsHandler::TestBlock*> (block.get());
  BOOST_CHECK_EQUAL(testBlock->GetBlockString(),
                    "  if (a)\n"
                    "  {\n"
                    "    (c = d);\n"
                    "  }\n");
}

BOOST_AUTO_TEST_CASE(BlockBranch2)
{
  using namespace s1::parser;

  TestSemanticsHandler semanticsHandler;
  auto boolType = semanticsHandler.CreateType (s1::semantics::BaseType::Bool);
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto blockScope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                                  s1::semantics::ScopeLevel::Global);
  blockScope->AddVariable (boolType, "a", s1::semantics::ExpressionPtr(), false);
  blockScope->AddVariable (intType, "c", s1::semantics::ExpressionPtr(), false);
  blockScope->AddVariable (intType, "d", s1::semantics::ExpressionPtr(), false);
  blockScope->AddVariable (intType, "e", s1::semantics::ExpressionPtr(), false);

  std::string inStr ("if (a) { c = d; } else { c = e; }");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (blockScope));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  TestSemanticsHandler::TestBlock* testBlock =
    static_cast<TestSemanticsHandler::TestBlock*> (block.get());
  BOOST_CHECK_EQUAL(testBlock->GetBlockString(),
                    "  if (a)\n"
                    "  {\n"
                    "    (c = d);\n"
                    "  }\n"
                    "  else\n"
                    "  {\n"
                    "    (c = e);\n"
                    "  }\n");
}

BOOST_AUTO_TEST_CASE(BlockWhile)
{
  using namespace s1::parser;

  TestSemanticsHandler semanticsHandler;
  auto boolType = semanticsHandler.CreateType (s1::semantics::BaseType::Bool);
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto blockScope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                                  s1::semantics::ScopeLevel::Global);
  blockScope->AddVariable (boolType, "a", s1::semantics::ExpressionPtr(), false);
  blockScope->AddVariable (intType, "c", s1::semantics::ExpressionPtr(), false);
  blockScope->AddVariable (intType, "d", s1::semantics::ExpressionPtr(), false);

  std::string inStr ("while (a) { c = d; }");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (blockScope));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  TestSemanticsHandler::TestBlock* testBlock =
    static_cast<TestSemanticsHandler::TestBlock*> (block.get());
  BOOST_CHECK_EQUAL(testBlock->GetBlockString(),
                    "  while (a)\n"
                    "  {\n"
                    "    (c = d);\n"
                    "  }\n");
}

BOOST_AUTO_TEST_CASE(BlockFor)
{
  using namespace s1::parser;

  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto blockScope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                                  s1::semantics::ScopeLevel::Global);
  blockScope->AddVariable (intType, "a", s1::semantics::ExpressionPtr(), false);
  blockScope->AddVariable (intType, "b", s1::semantics::ExpressionPtr(), false);
  blockScope->AddVariable (intType, "c", s1::semantics::ExpressionPtr(), false);
  blockScope->AddVariable (intType, "d", s1::semantics::ExpressionPtr(), false);

  std::string inStr ("for (a; b; b = b+1) { c = d; }");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (blockScope));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  TestSemanticsHandler::TestBlock* testBlock =
    static_cast<TestSemanticsHandler::TestBlock*> (block.get());
  BOOST_CHECK_EQUAL(testBlock->GetBlockString(),
                    "  for (a; b; (b = (b + 1)))\n"
                    "  {\n"
                    "    (c = d);\n"
                    "  }\n");
}

BOOST_AUTO_TEST_CASE(BlockNested)
{
  using namespace s1::parser;

  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto blockScope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                                  s1::semantics::ScopeLevel::Global);
  blockScope->AddVariable (intType, "a", s1::semantics::ExpressionPtr(), false);
  blockScope->AddVariable (intType, "b", s1::semantics::ExpressionPtr(), false);

  std::string inStr ("{ a = b; }");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (blockScope));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  TestSemanticsHandler::TestBlock* testBlock =
    static_cast<TestSemanticsHandler::TestBlock*> (block.get());
  BOOST_CHECK_EQUAL(testBlock->GetBlockString(),
                    "  {\n"
                    "    (a = b);\n"
                    "  }\n");
}

BOOST_AUTO_TEST_CASE(BlockErrorRecovery)
{
  using namespace s1::parser;

  TestSemanticsHandler semanticsHandler;
  auto intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  auto blockScope = semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                                  s1::semantics::ScopeLevel::Global);
  blockScope->AddVariable (intType, "a", s1::semantics::ExpressionPtr(), false);
  blockScope->AddVariable (intType, "b", s1::semantics::ExpressionPtr(), false);
  blockScope->AddVariable (intType, "error", s1::semantics::ExpressionPtr(), false);
  blockScope->AddVariable (intType, "c", s1::semantics::ExpressionPtr(), false);
  blockScope->AddVariable (intType, "d", s1::semantics::ExpressionPtr(), false);

  std::string inStr ("a = b error; c=d;");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (blockScope));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size(), 1u);
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedSemicolon));
  TestSemanticsHandler::TestBlock* testBlock =
    static_cast<TestSemanticsHandler::TestBlock*> (block.get());
  BOOST_CHECK_EQUAL(testBlock->GetBlockString(),
                    "  (a = b);\n"
                    "  error;\n"
                    "  (c = d);\n");
}

BOOST_AUTO_TEST_SUITE_END()
