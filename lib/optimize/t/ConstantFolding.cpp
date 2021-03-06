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

#include <boost/cstdint.hpp>
#include <boost/make_shared.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

#include "base/common.h"
#include "base/intrusive_ptr.h"
#include "base/uc/SimpleBufferStreamSource.h"
#include "intermediate/IntermediateGeneratorSemanticsHandler.h"
#include "intermediate/SequenceBuilder.h"
#include "parser/Parser.h"
#include "BlockImpl.h"

#include "optimize/ConstantFolding.h"

#include "../../diagnostics/t/TestDiagnosticsHandler.h"
#include "../../intermediate/t/print_TestSequenceVisitor_Operation.h"
#include "../../intermediate/t/TestSequenceVisitor.h"
#include "../../semantics/t/TestParser.h"

using namespace s1::intermediate;
using namespace s1::optimize;

BOOST_AUTO_TEST_SUITE(ConstantFolding)

class TestSemanticsHandler : public IntermediateGeneratorSemanticsHandler
{
public:
  typedef IntermediateGeneratorSemanticsHandler Superclass;

  class TestBlockImpl : public BlockImpl
  {
  public:
    typedef BlockImpl Superclass;

    using Superclass::sequenceBuilder;
  };
};

BOOST_AUTO_TEST_CASE(SimpleCast)
{
  static const char blockSource[] =
    "float a;"
    "a = 1;"
    ;

  using namespace s1::parser;

  s1::uc::SimpleBufferStreamSource in (blockSource, strlen (blockSource));
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  // global scope is required so BlockImpl can create some unique var names
  s1::semantics::ScopePtr globalScope (semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                          s1::semantics::ScopeLevel::Global,
                                          semanticsHandler.GetVoidType()));
  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (globalScope));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK(errorHandler.parseErrors.empty());

  auto testBlockImpl = s1::get_static_ptr<TestSemanticsHandler::TestBlockImpl> (block);

  s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
  bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
  BOOST_CHECK_EQUAL (seqChanged, true);

  s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
  TestSequenceVisitor testVisitor;
  newSeq->Visit (testVisitor);
  BOOST_CHECK_EQUAL (testVisitor.entries.size(), 2u);
  if (testVisitor.entries.size() < 1) return;
  BOOST_CHECK_EQUAL (testVisitor.entries[0].op, TestSequenceVisitor::opConstUInt);
  if (testVisitor.entries.size() < 2) return;
  BOOST_CHECK_EQUAL (testVisitor.entries[1].op, TestSequenceVisitor::opConstFloat);
}

BOOST_AUTO_TEST_CASE(ArrayExtract)
{
  static const char blockSource[] =
    "float[] a;"
    "float b;"
    "a = float[] (1.0, 2.0, 3.0);"
    "b = a[1];"
    ;

  using namespace s1::parser;

  s1::uc::SimpleBufferStreamSource in (blockSource, strlen (blockSource));
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  // global scope is required so BlockImpl can create some unique var names
  s1::semantics::ScopePtr globalScope (semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                          s1::semantics::ScopeLevel::Global,
                                          semanticsHandler.GetVoidType()));
  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (globalScope));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK(errorHandler.parseErrors.empty());

  auto testBlockImpl = s1::get_static_ptr<TestSemanticsHandler::TestBlockImpl> (block);

  s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
  bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
  BOOST_CHECK_EQUAL (seqChanged, true);

  s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
  TestSequenceVisitor testVisitor;
  newSeq->Visit (testVisitor);
  BOOST_CHECK_EQUAL (testVisitor.entries.size(), 6u);
  if (testVisitor.entries.size() < 6) return;
  // Set up array
  BOOST_CHECK_EQUAL (testVisitor.entries[0].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[1].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[2].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[3].op, TestSequenceVisitor::opMakeArray);
  // Const for index (unused)
  BOOST_CHECK_EQUAL (testVisitor.entries[4].op, TestSequenceVisitor::opConstUInt);
  // Extract array member - optimized: no extract op, simple assignment instead
  BOOST_CHECK_EQUAL (testVisitor.entries[5].op, TestSequenceVisitor::opAssignment);
}

BOOST_AUTO_TEST_CASE(ArrayChange)
{
  static const char blockSource[] =
    "float[] a;"
    "float b;"
    "a = float[] (1.0, 2.0, 3.0);"
    "a[1] = 4.0;"
    "b = a[1];"
    ;

  using namespace s1::parser;

  s1::uc::SimpleBufferStreamSource in (blockSource, strlen (blockSource));
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  // global scope is required so BlockImpl can create some unique var names
  s1::semantics::ScopePtr globalScope (semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                          s1::semantics::ScopeLevel::Global,
                                          semanticsHandler.GetVoidType()));
  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (globalScope));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK(errorHandler.parseErrors.empty());

  auto testBlockImpl = s1::get_static_ptr<TestSemanticsHandler::TestBlockImpl> (block);

  s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
  bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
  BOOST_CHECK_EQUAL (seqChanged, true);

  s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
  TestSequenceVisitor testVisitor;
  newSeq->Visit (testVisitor);
  BOOST_CHECK_EQUAL (testVisitor.entries.size(), 10u);
  if (testVisitor.entries.size() < 10) return;
  // Set up array
  BOOST_CHECK_EQUAL (testVisitor.entries[0].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[1].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[2].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[3].op, TestSequenceVisitor::opMakeArray);
  // New element value
  BOOST_CHECK_EQUAL (testVisitor.entries[4].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[5].op, TestSequenceVisitor::opAssignment);
  // Const for index
  BOOST_CHECK_EQUAL (testVisitor.entries[6].op, TestSequenceVisitor::opConstUInt);
  // Change element
  BOOST_CHECK_EQUAL (testVisitor.entries[7].op, TestSequenceVisitor::opChangeArrayElement);
  // Const for index (unused)
  BOOST_CHECK_EQUAL (testVisitor.entries[8].op, TestSequenceVisitor::opConstUInt);
  // Extract array member - optimized: no extract op, simple assignment instead
  BOOST_CHECK_EQUAL (testVisitor.entries[9].op, TestSequenceVisitor::opAssignment);
}

BOOST_AUTO_TEST_CASE(ArrayLen)
{
  static const char blockSource[] =
    "float[] a;"
    "unsigned int b;"
    "a = float[] (1.0, 2.0, 3.0);"
    "b = a.length;"
    ;

  using namespace s1::parser;

  s1::uc::SimpleBufferStreamSource in (blockSource, strlen (blockSource));
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  // global scope is required so BlockImpl can create some unique var names
  s1::semantics::ScopePtr globalScope (semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                          s1::semantics::ScopeLevel::Global,
                                          semanticsHandler.GetVoidType()));
  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (globalScope));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK(errorHandler.parseErrors.empty());

  auto testBlockImpl = s1::get_static_ptr<TestSemanticsHandler::TestBlockImpl> (block);

  s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
  bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
  BOOST_CHECK_EQUAL (seqChanged, true);

  s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
  TestSequenceVisitor testVisitor;
  newSeq->Visit (testVisitor);
  BOOST_CHECK_EQUAL (testVisitor.entries.size(), 5u);
  if (testVisitor.entries.size() < 5) return;
  // Set up array
  BOOST_CHECK_EQUAL (testVisitor.entries[0].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[1].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[2].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[3].op, TestSequenceVisitor::opMakeArray);
  // Get length - optimized: no array length op, constant instead
  BOOST_CHECK_EQUAL (testVisitor.entries[4].op, TestSequenceVisitor::opConstUInt);
}

BOOST_AUTO_TEST_CASE(VectorExtract)
{
  static const char blockSource[] =
    "float2 a = float2 (1.0, 2.0);"
    "float b;"
    "b = a.y;"
    ;

  using namespace s1::parser;

  s1::uc::SimpleBufferStreamSource in (blockSource, strlen (blockSource));
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  // global scope is required so BlockImpl can create some unique var names
  s1::semantics::ScopePtr globalScope (semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                          s1::semantics::ScopeLevel::Global,
                                          semanticsHandler.GetVoidType()));
  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (globalScope));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK(errorHandler.parseErrors.empty());

  auto testBlockImpl = s1::get_static_ptr<TestSemanticsHandler::TestBlockImpl> (block);

  s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
  bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
  BOOST_CHECK_EQUAL (seqChanged, true);

  s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
  TestSequenceVisitor testVisitor;
  newSeq->Visit (testVisitor);
  BOOST_CHECK_EQUAL (testVisitor.entries.size(), 4u);
  if (testVisitor.entries.size() < 4) return;
  // Set up vector
  BOOST_CHECK_EQUAL (testVisitor.entries[0].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[1].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[2].op, TestSequenceVisitor::opMakeVectorFloat);
  // Extract vector component - optimized: no extract op, const value instead
  BOOST_CHECK_EQUAL (testVisitor.entries[3].op, TestSequenceVisitor::opConstFloat);
}

BOOST_AUTO_TEST_CASE(ArithSimple)
{
  static const char blockSource[] =
    "float x = 3.0;"
    "float y = 2.0;"
    "float a; a = x+y;"
    "float b; b = x-y;"
    "float c; c = x*y;"
    "float d; d = x/y;"
    "float e; e = x%y;"
    ;

  using namespace s1::parser;

  s1::uc::SimpleBufferStreamSource in (blockSource, strlen (blockSource));
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  // global scope is required so BlockImpl can create some unique var names
  s1::semantics::ScopePtr globalScope (semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                          s1::semantics::ScopeLevel::Global,
                                          semanticsHandler.GetVoidType()));
  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (globalScope));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK(errorHandler.parseErrors.empty());

  auto testBlockImpl = s1::get_static_ptr<TestSemanticsHandler::TestBlockImpl> (block);

  s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
  bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
  BOOST_CHECK_EQUAL (seqChanged, true);

  s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
  TestSequenceVisitor testVisitor;
  newSeq->Visit (testVisitor);
  BOOST_CHECK_EQUAL (testVisitor.entries.size(), 7u);
  if (testVisitor.entries.size() < 7) return;
  // Operand assignments
  BOOST_CHECK_EQUAL (testVisitor.entries[0].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[1].op, TestSequenceVisitor::opConstFloat);
  // Addition - optimized: no arith op, const value instead
  BOOST_CHECK_EQUAL (testVisitor.entries[2].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[2].floatConst, 5.0);
  // Subtraction - optimized: no arith op, const value instead
  BOOST_CHECK_EQUAL (testVisitor.entries[3].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[3].floatConst, 1.0);
  // Multiplication - optimized: no arith op, const value instead
  BOOST_CHECK_EQUAL (testVisitor.entries[4].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[4].floatConst, 6.0);
  // Division - optimized: no arith op, const value instead
  BOOST_CHECK_EQUAL (testVisitor.entries[5].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[5].floatConst, 1.5);
  // Modulo - optimized: no arith op, const value instead
  BOOST_CHECK_EQUAL (testVisitor.entries[6].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[6].floatConst, 1.0);
}

BOOST_AUTO_TEST_CASE(ArithVector)
{
  static const char blockSource[] =
    "float2 x = float2 (3.0, 0.5);"
    "float2 y = float2 (2.0, 0.25);"
    "float2 a; a = x+y;"
    "float2 b; b = x-y;"
    "float2 c; c = x*y;"
    "float2 d; d = x/y;"
    "float2 e; e = x%y;"
    ;

  using namespace s1::parser;

  s1::uc::SimpleBufferStreamSource in (blockSource, strlen (blockSource));
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  // global scope is required so BlockImpl can create some unique var names
  s1::semantics::ScopePtr globalScope (semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                          s1::semantics::ScopeLevel::Global,
                                          semanticsHandler.GetVoidType()));
  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (globalScope));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK(errorHandler.parseErrors.empty());

  auto testBlockImpl = s1::get_static_ptr<TestSemanticsHandler::TestBlockImpl> (block);

  s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
  bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
  BOOST_CHECK_EQUAL (seqChanged, true);

  s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
  TestSequenceVisitor testVisitor;
  newSeq->Visit (testVisitor);
  BOOST_CHECK_EQUAL (testVisitor.entries.size(), 21u);
  if (testVisitor.entries.size() < 21) return;
  // Operand assignments
  BOOST_CHECK_EQUAL (testVisitor.entries[0].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[1].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[2].op, TestSequenceVisitor::opMakeVectorFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[3].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[4].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[5].op, TestSequenceVisitor::opMakeVectorFloat);
  // Addition - optimized: no arith op, const value instead
  BOOST_CHECK_EQUAL (testVisitor.entries[6].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[6].floatConst, 5.0);
  BOOST_CHECK_EQUAL (testVisitor.entries[7].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[7].floatConst, 0.75f);
  BOOST_CHECK_EQUAL (testVisitor.entries[8].op, TestSequenceVisitor::opMakeVectorFloat);
  // Subtraction - optimized: no arith op, const value instead
  BOOST_CHECK_EQUAL (testVisitor.entries[9].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[9].floatConst, 1.0);
  BOOST_CHECK_EQUAL (testVisitor.entries[10].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[10].floatConst, 0.25f);
  BOOST_CHECK_EQUAL (testVisitor.entries[11].op, TestSequenceVisitor::opMakeVectorFloat);
  // Multiplication - optimized: no arith op, const value instead
  BOOST_CHECK_EQUAL (testVisitor.entries[12].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[12].floatConst, 6.0);
  BOOST_CHECK_EQUAL (testVisitor.entries[13].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[13].floatConst, 0.125f);
  BOOST_CHECK_EQUAL (testVisitor.entries[14].op, TestSequenceVisitor::opMakeVectorFloat);
  // Division - optimized: no arith op, const value instead
  BOOST_CHECK_EQUAL (testVisitor.entries[15].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[15].floatConst, 1.5);
  BOOST_CHECK_EQUAL (testVisitor.entries[16].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[16].floatConst, 2.0);
  BOOST_CHECK_EQUAL (testVisitor.entries[17].op, TestSequenceVisitor::opMakeVectorFloat);
  // Modulo - optimized: no arith op, const value instead
  BOOST_CHECK_EQUAL (testVisitor.entries[18].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[18].floatConst, 1.0);
  BOOST_CHECK_EQUAL (testVisitor.entries[19].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[19].floatConst, 0);
  BOOST_CHECK_EQUAL (testVisitor.entries[20].op, TestSequenceVisitor::opMakeVectorFloat);
}

BOOST_AUTO_TEST_CASE(LogicSimple)
{
  static const char blockSource[] =
    "bool x = true;"
    "bool y = false;"
    "bool a; a = x && y;"
    "bool b; b = x || y;"
    ;

  using namespace s1::parser;

  s1::uc::SimpleBufferStreamSource in (blockSource, strlen (blockSource));
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  // global scope is required so BlockImpl can create some unique var names
  s1::semantics::ScopePtr globalScope (semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                          s1::semantics::ScopeLevel::Global,
                                          semanticsHandler.GetVoidType()));
  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (globalScope));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK(errorHandler.parseErrors.empty());

  auto testBlockImpl = s1::get_static_ptr<TestSemanticsHandler::TestBlockImpl> (block);

  s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
  bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
  BOOST_CHECK_EQUAL (seqChanged, true);

  s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
  TestSequenceVisitor testVisitor;
  newSeq->Visit (testVisitor);
  BOOST_CHECK_EQUAL (testVisitor.entries.size(), 4u);
  if (testVisitor.entries.size() < 4) return;
  // Operand assignments
  BOOST_CHECK_EQUAL (testVisitor.entries[0].op, TestSequenceVisitor::opConstBool);
  BOOST_CHECK_EQUAL (testVisitor.entries[1].op, TestSequenceVisitor::opConstBool);
  // AND - optimized: no logic op, const value instead
  BOOST_CHECK_EQUAL (testVisitor.entries[2].op, TestSequenceVisitor::opConstBool);
  BOOST_CHECK_EQUAL (testVisitor.entries[2].boolConst, false);
  // OR - optimized: no logic op, const value instead
  BOOST_CHECK_EQUAL (testVisitor.entries[3].op, TestSequenceVisitor::opConstBool);
  BOOST_CHECK_EQUAL (testVisitor.entries[3].boolConst, true);
}

BOOST_AUTO_TEST_CASE(Unary)
{
  static const char blockSource[] =
    "float x = 1.25;"
    "int y = 3;"
    "bool z = false;"
    "float a; a = -x;"
    "int b; b = -y;"
    "bool c; c = !z;"
    ;

  using namespace s1::parser;

  s1::uc::SimpleBufferStreamSource in (blockSource, strlen (blockSource));
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  // global scope is required so BlockImpl can create some unique var names
  s1::semantics::ScopePtr globalScope (semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                          s1::semantics::ScopeLevel::Global,
                                          semanticsHandler.GetVoidType()));
  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (globalScope));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK(errorHandler.parseErrors.empty());

  auto testBlockImpl = s1::get_static_ptr<TestSemanticsHandler::TestBlockImpl> (block);

  s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
  bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
  BOOST_CHECK_EQUAL (seqChanged, true);

  s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
  TestSequenceVisitor testVisitor;
  newSeq->Visit (testVisitor);
  BOOST_CHECK_EQUAL (testVisitor.entries.size(), 7u);
  if (testVisitor.entries.size() < 7) return;
  // 'x' initialization
  BOOST_CHECK_EQUAL (testVisitor.entries[0].op, TestSequenceVisitor::opConstFloat);
  // 'y' initialization
  BOOST_CHECK_EQUAL (testVisitor.entries[1].op, TestSequenceVisitor::opConstUInt);
  // Optimized: int constant instead of cast op
  BOOST_CHECK_EQUAL (testVisitor.entries[2].op, TestSequenceVisitor::opConstInt);
  // 'c' initialization
  BOOST_CHECK_EQUAL (testVisitor.entries[3].op, TestSequenceVisitor::opConstBool);
  // Float neg - optimized: constant instead of neg op
  BOOST_CHECK_EQUAL (testVisitor.entries[4].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[4].floatConst, -1.25);
  // Int neg - optimized: constant instead of neg op
  BOOST_CHECK_EQUAL (testVisitor.entries[5].op, TestSequenceVisitor::opConstInt);
  BOOST_CHECK_EQUAL (testVisitor.entries[5].intConst, -3);
  // Bool not - optimized: constant instead of not op
  BOOST_CHECK_EQUAL (testVisitor.entries[6].op, TestSequenceVisitor::opConstBool);
  BOOST_CHECK_EQUAL (testVisitor.entries[6].boolConst, true);
}

BOOST_AUTO_TEST_CASE(Compare)
{
  static const char blockSource[] =
    "unsigned int x = 2;"
    "unsigned int y = 3;"
    "unsigned int z = 2;"
    "bool a; a = x == y;"
    "bool b; b = x != y;"
    "bool c; c = x <  y;"
    "bool d; d = x <= y;"
    "bool e; e = x >  y;"
    "bool f; f = x >= y;"
    "bool g; g = x == z;"
    "bool h; h = x != z;"
    "bool i; i = x <  z;"
    "bool j; j = x <= z;"
    "bool k; k = x >  z;"
    "bool l; l = x >= z;"
    ;

  using namespace s1::parser;

  s1::uc::SimpleBufferStreamSource in (blockSource, strlen (blockSource));
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  // global scope is required so BlockImpl can create some unique var names
  s1::semantics::ScopePtr globalScope (semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                          s1::semantics::ScopeLevel::Global,
                                          semanticsHandler.GetVoidType()));
  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (globalScope));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK(errorHandler.parseErrors.empty());

  auto testBlockImpl = s1::get_static_ptr<TestSemanticsHandler::TestBlockImpl> (block);

  s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
  bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
  BOOST_CHECK_EQUAL (seqChanged, true);

  s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
  TestSequenceVisitor testVisitor;
  newSeq->Visit (testVisitor);
  BOOST_CHECK_EQUAL (testVisitor.entries.size(), 15u);
  if (testVisitor.entries.size() < 15) return;
  // 'x' initialization
  BOOST_CHECK_EQUAL (testVisitor.entries[0].op, TestSequenceVisitor::opConstUInt);
  // 'y' initialization
  BOOST_CHECK_EQUAL (testVisitor.entries[1].op, TestSequenceVisitor::opConstUInt);
  // 'z' initialization
  BOOST_CHECK_EQUAL (testVisitor.entries[2].op, TestSequenceVisitor::opConstUInt);
  // Optimized: bool constant instead of compare op
  BOOST_CHECK_EQUAL (testVisitor.entries[3].op, TestSequenceVisitor::opConstBool);
  BOOST_CHECK_EQUAL (testVisitor.entries[3].boolConst, false);
  // Optimized: bool constant instead of compare op
  BOOST_CHECK_EQUAL (testVisitor.entries[4].op, TestSequenceVisitor::opConstBool);
  BOOST_CHECK_EQUAL (testVisitor.entries[4].boolConst, true);
  // Optimized: bool constant instead of compare op
  BOOST_CHECK_EQUAL (testVisitor.entries[5].op, TestSequenceVisitor::opConstBool);
  BOOST_CHECK_EQUAL (testVisitor.entries[5].boolConst, true);
  // Optimized: bool constant instead of compare op
  BOOST_CHECK_EQUAL (testVisitor.entries[6].op, TestSequenceVisitor::opConstBool);
  BOOST_CHECK_EQUAL (testVisitor.entries[6].boolConst, true);
  // Optimized: bool constant instead of compare op
  BOOST_CHECK_EQUAL (testVisitor.entries[7].op, TestSequenceVisitor::opConstBool);
  BOOST_CHECK_EQUAL (testVisitor.entries[7].boolConst, false);
  // Optimized: bool constant instead of compare op
  BOOST_CHECK_EQUAL (testVisitor.entries[8].op, TestSequenceVisitor::opConstBool);
  BOOST_CHECK_EQUAL (testVisitor.entries[8].boolConst, false);

  // Optimized: bool constant instead of compare op
  BOOST_CHECK_EQUAL (testVisitor.entries[9].op, TestSequenceVisitor::opConstBool);
  BOOST_CHECK_EQUAL (testVisitor.entries[9].boolConst, true);
  // Optimized: bool constant instead of compare op
  BOOST_CHECK_EQUAL (testVisitor.entries[10].op, TestSequenceVisitor::opConstBool);
  BOOST_CHECK_EQUAL (testVisitor.entries[10].boolConst, false);
  // Optimized: bool constant instead of compare op
  BOOST_CHECK_EQUAL (testVisitor.entries[11].op, TestSequenceVisitor::opConstBool);
  BOOST_CHECK_EQUAL (testVisitor.entries[11].boolConst, false);
  // Optimized: bool constant instead of compare op
  BOOST_CHECK_EQUAL (testVisitor.entries[12].op, TestSequenceVisitor::opConstBool);
  BOOST_CHECK_EQUAL (testVisitor.entries[12].boolConst, true);
  // Optimized: bool constant instead of compare op
  BOOST_CHECK_EQUAL (testVisitor.entries[13].op, TestSequenceVisitor::opConstBool);
  BOOST_CHECK_EQUAL (testVisitor.entries[13].boolConst, false);
  // Optimized: bool constant instead of compare op
  BOOST_CHECK_EQUAL (testVisitor.entries[14].op, TestSequenceVisitor::opConstBool);
  BOOST_CHECK_EQUAL (testVisitor.entries[14].boolConst, true);
}

BOOST_AUTO_TEST_CASE(Block)
{
  static const char blockSource[] =
    "unsigned int x = 2;"
    "unsigned int y = 3;"
    "unsigned int z;"
    "{"
      "z = x+y;"
    "}"
    "z = z+x;"
    ;

  using namespace s1::parser;

  s1::uc::SimpleBufferStreamSource in (blockSource, strlen (blockSource));
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  // global scope is required so BlockImpl can create some unique var names
  s1::semantics::ScopePtr globalScope (semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                          s1::semantics::ScopeLevel::Global,
                                          semanticsHandler.GetVoidType()));
  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (globalScope));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK(errorHandler.parseErrors.empty());

  auto testBlockImpl = s1::get_static_ptr<TestSemanticsHandler::TestBlockImpl> (block);

  s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
  bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
  BOOST_CHECK_EQUAL (seqChanged, true);

  s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
  TestSequenceVisitor testVisitor;
  newSeq->Visit (testVisitor);
  BOOST_CHECK_EQUAL (testVisitor.entries.size(), 4u);
  if (testVisitor.entries.size() < 4) return;
  // 'x' initialization
  BOOST_CHECK_EQUAL (testVisitor.entries[0].op, TestSequenceVisitor::opConstUInt);
  // 'y' initialization
  BOOST_CHECK_EQUAL (testVisitor.entries[1].op, TestSequenceVisitor::opConstUInt);
  // Nested block
  BOOST_CHECK_EQUAL (testVisitor.entries[2].op, TestSequenceVisitor::opBlock);
  // Optimized: constant instead of arith op
  BOOST_CHECK_EQUAL (testVisitor.entries[3].op, TestSequenceVisitor::opConstUInt);
  BOOST_CHECK_EQUAL (testVisitor.entries[3].uintConst, 7u);
}

BOOST_AUTO_TEST_CASE(BranchNonConst)
{
  static const char blockSource[] =
    "bool c;"
    "unsigned int x, y, z;"
    "if (c)"
    "{"
      "x = 1;"
    "}"
    "else"
    "{"
      "x = 2;"
    "}"
    "z = 1;"
    "y = x*z;"
    ;

  using namespace s1::parser;

  s1::uc::SimpleBufferStreamSource in (blockSource, strlen (blockSource));
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  // global scope is required so BlockImpl can create some unique var names
  s1::semantics::ScopePtr globalScope (semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                          s1::semantics::ScopeLevel::Global,
                                          semanticsHandler.GetVoidType()));
  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (globalScope));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK(errorHandler.parseErrors.empty());

  auto testBlockImpl = s1::get_static_ptr<TestSemanticsHandler::TestBlockImpl> (block);

  s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
  bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
  BOOST_CHECK_EQUAL (seqChanged, false);

  s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
  TestSequenceVisitor testVisitor;
  newSeq->Visit (testVisitor);
  BOOST_CHECK_EQUAL (testVisitor.entries.size(), 3u);
  if (testVisitor.entries.size() < 3) return;
  // Branch op
  BOOST_CHECK_EQUAL (testVisitor.entries[0].op, TestSequenceVisitor::opBranch);
  // 'z' assignment
  BOOST_CHECK_EQUAL (testVisitor.entries[1].op, TestSequenceVisitor::opConstUInt);
  // 'y' assignment
  BOOST_CHECK_EQUAL (testVisitor.entries[2].op, TestSequenceVisitor::opArithMul);
}

BOOST_AUTO_TEST_CASE(BranchConst)
{
  static const char blockSource[] =
    "bool c = true;"
    "unsigned int x, y, z;"
    "if (c)"
    "{"
      "x = 1;"
    "}"
    "else"
    "{"
      "x = 2;"
    "}"
    "z = 1;"
    "y = x*z;"
    ;

  using namespace s1::parser;

  s1::uc::SimpleBufferStreamSource in (blockSource, strlen (blockSource));
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  // global scope is required so BlockImpl can create some unique var names
  s1::semantics::ScopePtr globalScope (semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                          s1::semantics::ScopeLevel::Global,
                                          semanticsHandler.GetVoidType()));
  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (globalScope));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK(errorHandler.parseErrors.empty());

  auto testBlockImpl = s1::get_static_ptr<TestSemanticsHandler::TestBlockImpl> (block);

  s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
  bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
  BOOST_CHECK_EQUAL (seqChanged, true);

  s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
  TestSequenceVisitor testVisitor;
  newSeq->Visit (testVisitor);
  BOOST_CHECK_EQUAL (testVisitor.entries.size(), 4u);
  if (testVisitor.entries.size() < 4) return;
  // 'c' assignment
  BOOST_CHECK_EQUAL (testVisitor.entries[0].op, TestSequenceVisitor::opConstBool);
  // Optimized: nested block instead of branch op
  BOOST_CHECK_EQUAL (testVisitor.entries[1].op, TestSequenceVisitor::opBlock);
  // 'z' assignment
  BOOST_CHECK_EQUAL (testVisitor.entries[2].op, TestSequenceVisitor::opConstUInt);
  // Optimized: constant instead of multiplication
  BOOST_CHECK_EQUAL (testVisitor.entries[3].op, TestSequenceVisitor::opConstUInt);
  BOOST_CHECK_EQUAL (testVisitor.entries[3].uintConst, 1u);
}

BOOST_AUTO_TEST_CASE(WhileNonConst)
{
  static const char blockSource[] =
    "unsigned int x, y, z;"
    "x = 0;"
    "while (x < 5)"
    "{"
      "x = x+1;"
    "}"
    "z = 1;"
    "y = x*z;"
    ;

  using namespace s1::parser;

  s1::uc::SimpleBufferStreamSource in (blockSource, strlen (blockSource));
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  // global scope is required so BlockImpl can create some unique var names
  s1::semantics::ScopePtr globalScope (semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                          s1::semantics::ScopeLevel::Global,
                                          semanticsHandler.GetVoidType()));
  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (globalScope));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK(errorHandler.parseErrors.empty());

  auto testBlockImpl = s1::get_static_ptr<TestSemanticsHandler::TestBlockImpl> (block);

  s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
  bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
  BOOST_CHECK_EQUAL (seqChanged, true); /* seq is still changed b/c initial 'x < 5' is optimized */

  s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
  TestSequenceVisitor testVisitor;
  newSeq->Visit (testVisitor);
  BOOST_CHECK_EQUAL (testVisitor.entries.size(), 6u);
  if (testVisitor.entries.size() < 6) return;
  // 'x' assignment
  BOOST_CHECK_EQUAL (testVisitor.entries[0].op, TestSequenceVisitor::opConstUInt);
  // 'x < 5': right side for comparison
  BOOST_CHECK_EQUAL (testVisitor.entries[1].op, TestSequenceVisitor::opConstUInt);
  // 'x < 5': optimized comparison
  BOOST_CHECK_EQUAL (testVisitor.entries[2].op, TestSequenceVisitor::opConstBool);
  // Loop
  BOOST_CHECK_EQUAL (testVisitor.entries[3].op, TestSequenceVisitor::opWhile);
  // 'z' assignment
  BOOST_CHECK_EQUAL (testVisitor.entries[4].op, TestSequenceVisitor::opConstUInt);
  // 'y' assignment
  BOOST_CHECK_EQUAL (testVisitor.entries[5].op, TestSequenceVisitor::opArithMul);
}

BOOST_AUTO_TEST_CASE(WhileConst)
{
  static const char blockSource[] =
    "unsigned int x, y, z;"
    "x = 0;"
    "while (x > 0)"
    "{"
      "x = x+1;"
    "}"
    "z = 1;"
    "y = x*z;"
    ;

  using namespace s1::parser;

  s1::uc::SimpleBufferStreamSource in (blockSource, strlen (blockSource));
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  // global scope is required so BlockImpl can create some unique var names
  s1::semantics::ScopePtr globalScope (semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                          s1::semantics::ScopeLevel::Global,
                                          semanticsHandler.GetVoidType()));
  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (globalScope));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK(errorHandler.parseErrors.empty());

  auto testBlockImpl = s1::get_static_ptr<TestSemanticsHandler::TestBlockImpl> (block);

  s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
  bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
  BOOST_CHECK_EQUAL (seqChanged, true);

  s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
  TestSequenceVisitor testVisitor;
  newSeq->Visit (testVisitor);
  BOOST_CHECK_EQUAL (testVisitor.entries.size(), 5u);
  if (testVisitor.entries.size() < 5) return;
  // 'x' assignment
  BOOST_CHECK_EQUAL (testVisitor.entries[0].op, TestSequenceVisitor::opConstUInt);
  // 'x > 0': right side for comparison
  BOOST_CHECK_EQUAL (testVisitor.entries[1].op, TestSequenceVisitor::opConstUInt);
  // 'x > 0': optimized comparison
  BOOST_CHECK_EQUAL (testVisitor.entries[2].op, TestSequenceVisitor::opConstBool);
  // Optimized: no loop
  // 'z' assignment
  BOOST_CHECK_EQUAL (testVisitor.entries[3].op, TestSequenceVisitor::opConstUInt);
  // Optimized: constant instead of multiplication
  BOOST_CHECK_EQUAL (testVisitor.entries[4].op, TestSequenceVisitor::opConstUInt);
  BOOST_CHECK_EQUAL (testVisitor.entries[4].uintConst, 0u);
}

BOOST_AUTO_TEST_CASE(BuiltinDot)
{
  static const char blockSource[] =
    "float2 x = float2 (3.0, 0.5);"
    "float2 y = float2 (2.0, 0.25);"
    "float a; a = dot (x, y);"
    ;

  using namespace s1::parser;

  s1::uc::SimpleBufferStreamSource in (blockSource, strlen (blockSource));
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  // global scope is required so BlockImpl can create some unique var names
  s1::semantics::ScopePtr builtinScope (semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                                                          s1::semantics::ScopeLevel::Builtin,
                                                                          semanticsHandler.GetVoidType()));
  s1::semantics::ScopePtr globalScope (semanticsHandler.CreateScope (builtinScope,
                                                                        s1::semantics::ScopeLevel::Global,
                                                                        semanticsHandler.GetVoidType()));
  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (globalScope));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK(errorHandler.parseErrors.empty());

  auto testBlockImpl = s1::get_static_ptr<TestSemanticsHandler::TestBlockImpl> (block);

  s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
  bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
  BOOST_CHECK_EQUAL (seqChanged, true);

  s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
  TestSequenceVisitor testVisitor;
  newSeq->Visit (testVisitor);
  BOOST_CHECK_EQUAL (testVisitor.entries.size(), 7u);
  if (testVisitor.entries.size() < 7) return;
  // Operand assignments
  BOOST_CHECK_EQUAL (testVisitor.entries[0].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[1].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[2].op, TestSequenceVisitor::opMakeVectorFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[3].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[4].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[5].op, TestSequenceVisitor::opMakeVectorFloat);
  // Dot product - optimized: no builtin call op, const value instead
  BOOST_CHECK_EQUAL (testVisitor.entries[6].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[6].floatConst, 6.125);
}

BOOST_AUTO_TEST_CASE(BuiltinCross)
{
  static const char blockSource[] =
    "float3 x = float3 (1.0, 2.0, 3.0);"
    "float3 y = float3 (4.0, 5.0, 6.0);"
    "float3 a; a = cross (x, y);"
    ;

  using namespace s1::parser;

  s1::uc::SimpleBufferStreamSource in (blockSource, strlen (blockSource));
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  // global scope is required so BlockImpl can create some unique var names
  s1::semantics::ScopePtr builtinScope (semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                                                          s1::semantics::ScopeLevel::Builtin,
                                                                          semanticsHandler.GetVoidType()));
  s1::semantics::ScopePtr globalScope (semanticsHandler.CreateScope (builtinScope,
                                                                        s1::semantics::ScopeLevel::Global,
                                                                        semanticsHandler.GetVoidType()));
  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (globalScope));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK(errorHandler.parseErrors.empty());

  auto testBlockImpl = s1::get_static_ptr<TestSemanticsHandler::TestBlockImpl> (block);

  s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
  bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
  BOOST_CHECK_EQUAL (seqChanged, true);

  s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
  TestSequenceVisitor testVisitor;
  newSeq->Visit (testVisitor);
  BOOST_CHECK_EQUAL (testVisitor.entries.size(), 12u);
  if (testVisitor.entries.size() < 12) return;
  // Operand assignments
  BOOST_CHECK_EQUAL (testVisitor.entries[0].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[1].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[2].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[3].op, TestSequenceVisitor::opMakeVectorFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[4].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[5].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[6].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[7].op, TestSequenceVisitor::opMakeVectorFloat);
  // Cross product - optimized: no builtin call op, const values instead
  BOOST_CHECK_EQUAL (testVisitor.entries[8].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[8].floatConst, -3);
  BOOST_CHECK_EQUAL (testVisitor.entries[9].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[9].floatConst, 6);
  BOOST_CHECK_EQUAL (testVisitor.entries[10].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[10].floatConst, -3);
  BOOST_CHECK_EQUAL (testVisitor.entries[11].op, TestSequenceVisitor::opMakeVectorFloat);
}

BOOST_AUTO_TEST_CASE(BuiltinNormalize)
{
  static const char blockSource[] =
    "float3 x = float3 (2.0, 0.0, 0.0);"
    "float3 y = float3 (0.0, 1.0, 1.0);"
    "float3 a; a = normalize (x);"
    "float3 b; b = normalize (y);"
    ;

  using namespace s1::parser;

  s1::uc::SimpleBufferStreamSource in (blockSource, strlen (blockSource));
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  // global scope is required so BlockImpl can create some unique var names
  s1::semantics::ScopePtr builtinScope (semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                                                          s1::semantics::ScopeLevel::Builtin,
                                                                          semanticsHandler.GetVoidType()));
  s1::semantics::ScopePtr globalScope (semanticsHandler.CreateScope (builtinScope,
                                                                        s1::semantics::ScopeLevel::Global,
                                                                        semanticsHandler.GetVoidType()));
  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (globalScope));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK(errorHandler.parseErrors.empty());

  auto testBlockImpl = s1::get_static_ptr<TestSemanticsHandler::TestBlockImpl> (block);

  s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
  bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
  BOOST_CHECK_EQUAL (seqChanged, true);

  s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
  TestSequenceVisitor testVisitor;
  newSeq->Visit (testVisitor);
  BOOST_CHECK_EQUAL (testVisitor.entries.size(), 16u);
  if (testVisitor.entries.size() < 16) return;
  // Operand assignments
  BOOST_CHECK_EQUAL (testVisitor.entries[0].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[1].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[2].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[3].op, TestSequenceVisitor::opMakeVectorFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[4].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[5].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[6].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[7].op, TestSequenceVisitor::opMakeVectorFloat);
  // Normalization - optimized: no builtin call op, const values instead
  BOOST_CHECK_EQUAL (testVisitor.entries[8].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[8].floatConst, 1);
  BOOST_CHECK_EQUAL (testVisitor.entries[9].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[9].floatConst, 0);
  BOOST_CHECK_EQUAL (testVisitor.entries[10].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[10].floatConst, 0);
  BOOST_CHECK_EQUAL (testVisitor.entries[11].op, TestSequenceVisitor::opMakeVectorFloat);
  // Normalization - optimized: no builtin call op, const values instead
  BOOST_CHECK_EQUAL (testVisitor.entries[12].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[12].floatConst, 0);
  BOOST_CHECK_EQUAL (testVisitor.entries[13].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_CLOSE  (testVisitor.entries[13].floatConst, M_SQRT1_2, 0.00001);
  BOOST_CHECK_EQUAL (testVisitor.entries[14].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_CLOSE  (testVisitor.entries[14].floatConst, M_SQRT1_2, 0.00001);
  BOOST_CHECK_EQUAL (testVisitor.entries[15].op, TestSequenceVisitor::opMakeVectorFloat);
}

BOOST_AUTO_TEST_CASE(BuiltinLength)
{
  static const char blockSource[] =
    "float3 x = float3 (2.0, 0.0, 0.0);"
    "float3 y = float3 (0.0, 1.0, 1.0);"
    "float a; a = length (x);"
    "float b; b = length (y);"
    ;

  using namespace s1::parser;

  s1::uc::SimpleBufferStreamSource in (blockSource, strlen (blockSource));
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  // global scope is required so BlockImpl can create some unique var names
  s1::semantics::ScopePtr builtinScope (semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                                                          s1::semantics::ScopeLevel::Builtin,
                                                                          semanticsHandler.GetVoidType()));
  s1::semantics::ScopePtr globalScope (semanticsHandler.CreateScope (builtinScope,
                                                                        s1::semantics::ScopeLevel::Global,
                                                                        semanticsHandler.GetVoidType()));
  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (globalScope));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK(errorHandler.parseErrors.empty());

  auto testBlockImpl = s1::get_static_ptr<TestSemanticsHandler::TestBlockImpl> (block);

  s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
  bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
  BOOST_CHECK_EQUAL (seqChanged, true);

  s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
  TestSequenceVisitor testVisitor;
  newSeq->Visit (testVisitor);
  BOOST_CHECK_EQUAL (testVisitor.entries.size(), 10u);
  if (testVisitor.entries.size() < 10) return;
  // Operand assignments
  BOOST_CHECK_EQUAL (testVisitor.entries[0].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[1].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[2].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[3].op, TestSequenceVisitor::opMakeVectorFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[4].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[5].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[6].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[7].op, TestSequenceVisitor::opMakeVectorFloat);
  // Length - optimized: no builtin call op, const value instead
  BOOST_CHECK_EQUAL (testVisitor.entries[8].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[8].floatConst, 2);
  // Length - optimized: no builtin call op, const value instead
  BOOST_CHECK_EQUAL (testVisitor.entries[9].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_CLOSE  (testVisitor.entries[9].floatConst, M_SQRT2, 0.00001);
}

BOOST_AUTO_TEST_CASE(BuiltinMulMatrixMatrix)
{
  static const char blockSource[] =
    "float2x2 x = float2x2 (2.0, 0.0, 0.0, 0.5);"
    "float2x2 y = float2x2 (0.0, 1.0, 1.0, 0.0);"
    "float2x2 a; a = mul (x, y);"
    ;

  using namespace s1::parser;

  s1::uc::SimpleBufferStreamSource in (blockSource, strlen (blockSource));
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  // global scope is required so BlockImpl can create some unique var names
  s1::semantics::ScopePtr builtinScope (semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                                                          s1::semantics::ScopeLevel::Builtin,
                                                                          semanticsHandler.GetVoidType()));
  s1::semantics::ScopePtr globalScope (semanticsHandler.CreateScope (builtinScope,
                                                                        s1::semantics::ScopeLevel::Global,
                                                                        semanticsHandler.GetVoidType()));
  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (globalScope));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK(errorHandler.parseErrors.empty());

  auto testBlockImpl = s1::get_static_ptr<TestSemanticsHandler::TestBlockImpl> (block);

  s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
  bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
  BOOST_CHECK_EQUAL (seqChanged, true);

  s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
  TestSequenceVisitor testVisitor;
  newSeq->Visit (testVisitor);
  BOOST_CHECK_EQUAL (testVisitor.entries.size(), 15u);
  if (testVisitor.entries.size() < 15) return;
  // Operand assignments
  BOOST_CHECK_EQUAL (testVisitor.entries[0].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[1].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[2].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[3].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[4].op, TestSequenceVisitor::opMakeMatrixFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[5].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[6].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[7].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[8].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[9].op, TestSequenceVisitor::opMakeMatrixFloat);
  // Matrix mul - optimized: no builtin call op, const values instead
  BOOST_CHECK_EQUAL (testVisitor.entries[10].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[10].floatConst, 0);
  BOOST_CHECK_EQUAL (testVisitor.entries[11].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[11].floatConst, 2);
  BOOST_CHECK_EQUAL (testVisitor.entries[12].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[12].floatConst, 0.5);
  BOOST_CHECK_EQUAL (testVisitor.entries[13].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[13].floatConst, 0);
  BOOST_CHECK_EQUAL (testVisitor.entries[14].op, TestSequenceVisitor::opMakeMatrixFloat);
}

BOOST_AUTO_TEST_CASE(BuiltinMulMatrixVector)
{
  static const char blockSource[] =
    "float2x2 x = float2x2 (2.0, 0.0, 0.0, 0.5);"
    "float2 y = float2 (2.0, 3.0);"
    "float2 a; a = mul (x, y);"
    ;

  using namespace s1::parser;

  s1::uc::SimpleBufferStreamSource in (blockSource, strlen (blockSource));
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  // global scope is required so BlockImpl can create some unique var names
  s1::semantics::ScopePtr builtinScope (semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                                                          s1::semantics::ScopeLevel::Builtin,
                                                                          semanticsHandler.GetVoidType()));
  s1::semantics::ScopePtr globalScope (semanticsHandler.CreateScope (builtinScope,
                                                                        s1::semantics::ScopeLevel::Global,
                                                                        semanticsHandler.GetVoidType()));
  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (globalScope));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK(errorHandler.parseErrors.empty());

  auto testBlockImpl = s1::get_static_ptr<TestSemanticsHandler::TestBlockImpl> (block);

  s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
  bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
  BOOST_CHECK_EQUAL (seqChanged, true);

  s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
  TestSequenceVisitor testVisitor;
  newSeq->Visit (testVisitor);
  BOOST_CHECK_EQUAL (testVisitor.entries.size(), 11u);
  if (testVisitor.entries.size() < 11) return;
  // Operand assignments
  BOOST_CHECK_EQUAL (testVisitor.entries[0].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[1].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[2].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[3].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[4].op, TestSequenceVisitor::opMakeMatrixFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[5].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[6].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[7].op, TestSequenceVisitor::opMakeVectorFloat);
  // Matrix/Vector mul - optimized: no builtin call op, const values instead
  BOOST_CHECK_EQUAL (testVisitor.entries[8].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[8].floatConst, 4);
  BOOST_CHECK_EQUAL (testVisitor.entries[9].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[9].floatConst, 1.5);
  BOOST_CHECK_EQUAL (testVisitor.entries[10].op, TestSequenceVisitor::opMakeVectorFloat);
}

BOOST_AUTO_TEST_CASE(BuiltinMulVectorMatrix)
{
  static const char blockSource[] =
    "float2 x = float2 (2.0, 3.0);"
    "float2x2 y = float2x2 (0.0, 1.0, 1.0, 0.0);"
    "float2 a; a = mul (x, y);"
    ;

  using namespace s1::parser;

  s1::uc::SimpleBufferStreamSource in (blockSource, strlen (blockSource));
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  // global scope is required so BlockImpl can create some unique var names
  s1::semantics::ScopePtr builtinScope (semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                                                          s1::semantics::ScopeLevel::Builtin,
                                                                          semanticsHandler.GetVoidType()));
  s1::semantics::ScopePtr globalScope (semanticsHandler.CreateScope (builtinScope,
                                                                        s1::semantics::ScopeLevel::Global,
                                                                        semanticsHandler.GetVoidType()));
  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (globalScope));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK(errorHandler.parseErrors.empty());

  auto testBlockImpl = s1::get_static_ptr<TestSemanticsHandler::TestBlockImpl> (block);

  s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
  bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
  BOOST_CHECK_EQUAL (seqChanged, true);

  s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
  TestSequenceVisitor testVisitor;
  newSeq->Visit (testVisitor);
  BOOST_CHECK_EQUAL (testVisitor.entries.size(), 11u);
  if (testVisitor.entries.size() < 11) return;
  // Operand assignments
  BOOST_CHECK_EQUAL (testVisitor.entries[0].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[1].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[2].op, TestSequenceVisitor::opMakeVectorFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[3].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[4].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[5].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[6].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[7].op, TestSequenceVisitor::opMakeMatrixFloat);
  // Vector/Matrix mul - optimized: no builtin call op, const values instead
  BOOST_CHECK_EQUAL (testVisitor.entries[8].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[8].floatConst, 3);
  BOOST_CHECK_EQUAL (testVisitor.entries[9].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[9].floatConst, 2);
  BOOST_CHECK_EQUAL (testVisitor.entries[10].op, TestSequenceVisitor::opMakeVectorFloat);
}

BOOST_AUTO_TEST_CASE(Min)
{
  static const char blockSource[] =
    "unsigned int ux = 2;"
    "unsigned int uy = 3;"
    "int ix = -2;"
    "int iy = -3;"
    "float fx = 2.;"
    "float fy = 3.;"
    "unsigned int a; a = min (ux, uy);"
    "int b; b = min (ix, iy);"
    "float c; c = min (fx, fy);"
    ;

  using namespace s1::parser;

  s1::uc::SimpleBufferStreamSource in (blockSource, strlen (blockSource));
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  // global scope is required so BlockImpl can create some unique var names
  s1::semantics::ScopePtr builtinScope (semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                                                          s1::semantics::ScopeLevel::Builtin,
                                                                          semanticsHandler.GetVoidType()));
  s1::semantics::ScopePtr globalScope (semanticsHandler.CreateScope (builtinScope,
                                                                        s1::semantics::ScopeLevel::Global,
                                                                        semanticsHandler.GetVoidType()));
  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (globalScope));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK(errorHandler.parseErrors.empty());

  auto testBlockImpl = s1::get_static_ptr<TestSemanticsHandler::TestBlockImpl> (block);

  s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
  bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
  BOOST_CHECK_EQUAL (seqChanged, true);

  s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
  TestSequenceVisitor testVisitor;
  newSeq->Visit (testVisitor);
  BOOST_CHECK_EQUAL (testVisitor.entries.size(), 13u);
  if (testVisitor.entries.size() < 13) return;
  // 'ux', 'uy' initialization
  BOOST_CHECK_EQUAL (testVisitor.entries[0].op, TestSequenceVisitor::opConstUInt);
  BOOST_CHECK_EQUAL (testVisitor.entries[1].op, TestSequenceVisitor::opConstUInt);
  // 'ix', 'iy' initialization (generated ops are 'ConstUInt', 'Neg', 'Cast', here opt. to consts)
  BOOST_CHECK_EQUAL (testVisitor.entries[2].op, TestSequenceVisitor::opConstUInt);
  BOOST_CHECK_EQUAL (testVisitor.entries[3].op, TestSequenceVisitor::opConstInt);
  BOOST_CHECK_EQUAL (testVisitor.entries[4].op, TestSequenceVisitor::opConstInt);
  BOOST_CHECK_EQUAL (testVisitor.entries[5].op, TestSequenceVisitor::opConstUInt);
  BOOST_CHECK_EQUAL (testVisitor.entries[6].op, TestSequenceVisitor::opConstInt);
  BOOST_CHECK_EQUAL (testVisitor.entries[7].op, TestSequenceVisitor::opConstInt);
  // 'fx', 'fy' initialization
  BOOST_CHECK_EQUAL (testVisitor.entries[8].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[9].op, TestSequenceVisitor::opConstFloat);
  // Min - optimized: no builtin call op, const values instead
  BOOST_CHECK_EQUAL (testVisitor.entries[10].op, TestSequenceVisitor::opConstUInt);
  BOOST_CHECK_EQUAL (testVisitor.entries[10].uintConst, 2u);
  // Min - optimized: no builtin call op, const values instead
  BOOST_CHECK_EQUAL (testVisitor.entries[11].op, TestSequenceVisitor::opConstInt);
  BOOST_CHECK_EQUAL (testVisitor.entries[11].intConst, -3);
  // Min - optimized: no builtin call op, const values instead
  BOOST_CHECK_EQUAL (testVisitor.entries[12].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[12].floatConst, 2);
}

BOOST_AUTO_TEST_CASE(Max)
{
  static const char blockSource[] =
    "unsigned int ux = 2;"
    "unsigned int uy = 3;"
    "int ix = -2;"
    "int iy = -3;"
    "float fx = 2.;"
    "float fy = 3.;"
    "unsigned int a; a = max (ux, uy);"
    "int b; b = max (ix, iy);"
    "float c; c = max (fx, fy);"
    ;

  using namespace s1::parser;

  s1::uc::SimpleBufferStreamSource in (blockSource, strlen (blockSource));
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  // global scope is required so BlockImpl can create some unique var names
  s1::semantics::ScopePtr builtinScope (semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                                                          s1::semantics::ScopeLevel::Builtin,
                                                                          semanticsHandler.GetVoidType()));
  s1::semantics::ScopePtr globalScope (semanticsHandler.CreateScope (builtinScope,
                                                                        s1::semantics::ScopeLevel::Global,
                                                                        semanticsHandler.GetVoidType()));
  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (globalScope));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK(errorHandler.parseErrors.empty());

  auto testBlockImpl = s1::get_static_ptr<TestSemanticsHandler::TestBlockImpl> (block);

  s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
  bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
  BOOST_CHECK_EQUAL (seqChanged, true);

  s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
  TestSequenceVisitor testVisitor;
  newSeq->Visit (testVisitor);
  BOOST_CHECK_EQUAL (testVisitor.entries.size(), 13u);
  if (testVisitor.entries.size() < 13) return;
  // 'ux', 'uy' initialization
  BOOST_CHECK_EQUAL (testVisitor.entries[0].op, TestSequenceVisitor::opConstUInt);
  BOOST_CHECK_EQUAL (testVisitor.entries[1].op, TestSequenceVisitor::opConstUInt);
  // 'ix', 'iy' initialization (generated ops are 'ConstUInt', 'Neg', 'Cast', here opt. to consts)
  BOOST_CHECK_EQUAL (testVisitor.entries[2].op, TestSequenceVisitor::opConstUInt);
  BOOST_CHECK_EQUAL (testVisitor.entries[3].op, TestSequenceVisitor::opConstInt);
  BOOST_CHECK_EQUAL (testVisitor.entries[4].op, TestSequenceVisitor::opConstInt);
  BOOST_CHECK_EQUAL (testVisitor.entries[5].op, TestSequenceVisitor::opConstUInt);
  BOOST_CHECK_EQUAL (testVisitor.entries[6].op, TestSequenceVisitor::opConstInt);
  BOOST_CHECK_EQUAL (testVisitor.entries[7].op, TestSequenceVisitor::opConstInt);
  // 'fx', 'fy' initialization
  BOOST_CHECK_EQUAL (testVisitor.entries[8].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[9].op, TestSequenceVisitor::opConstFloat);
  // Max - optimized: no builtin call op, const values instead
  BOOST_CHECK_EQUAL (testVisitor.entries[10].op, TestSequenceVisitor::opConstUInt);
  BOOST_CHECK_EQUAL (testVisitor.entries[10].uintConst, 3u);
  // Max - optimized: no builtin call op, const values instead
  BOOST_CHECK_EQUAL (testVisitor.entries[11].op, TestSequenceVisitor::opConstInt);
  BOOST_CHECK_EQUAL (testVisitor.entries[11].intConst, -2);
  // Max - optimized: no builtin call op, const values instead
  BOOST_CHECK_EQUAL (testVisitor.entries[12].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[12].floatConst, 3);
}

BOOST_AUTO_TEST_CASE(Pow)
{
  static const char blockSource[] =
    "float x1 = 2.;"
    "float y1 = 3.;"
    "float x2 = -2.;"
    "float y2 = -3.;"
    "float a; a = pow (x1, y1);"
    "float b; b = pow (x2, y2);"
    ;

  using namespace s1::parser;

  s1::uc::SimpleBufferStreamSource in (blockSource, strlen (blockSource));
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  // global scope is required so BlockImpl can create some unique var names
  s1::semantics::ScopePtr builtinScope (semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                                                          s1::semantics::ScopeLevel::Builtin,
                                                                          semanticsHandler.GetVoidType()));
  s1::semantics::ScopePtr globalScope (semanticsHandler.CreateScope (builtinScope,
                                                                        s1::semantics::ScopeLevel::Global,
                                                                        semanticsHandler.GetVoidType()));
  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (globalScope));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK(errorHandler.parseErrors.empty());

  auto testBlockImpl = s1::get_static_ptr<TestSemanticsHandler::TestBlockImpl> (block);

  s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
  bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
  BOOST_CHECK_EQUAL (seqChanged, true);

  s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
  TestSequenceVisitor testVisitor;
  newSeq->Visit (testVisitor);
  BOOST_CHECK_EQUAL (testVisitor.entries.size(), 8u);
  if (testVisitor.entries.size() < 8) return;
  // 'x1', 'y1' initialization
  BOOST_CHECK_EQUAL (testVisitor.entries[0].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[1].op, TestSequenceVisitor::opConstFloat);
  // 'x1', 'y1' initialization (generated ops are 'ConstFloat', 'Neg' here opt. to consts)
  BOOST_CHECK_EQUAL (testVisitor.entries[2].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[3].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[4].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[5].op, TestSequenceVisitor::opConstFloat);
  // Pow - optimized: no builtin call op, const values instead
  BOOST_CHECK_EQUAL (testVisitor.entries[6].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[6].floatConst, 8);
  // Pow - optimized: no builtin call op, const values instead
  BOOST_CHECK_EQUAL (testVisitor.entries[7].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL (testVisitor.entries[7].floatConst, -0.125);
}

BOOST_AUTO_TEST_SUITE_END()
