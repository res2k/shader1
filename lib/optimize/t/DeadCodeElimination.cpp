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

#include "base/intrusive_ptr.h"
#include "base/uc/SimpleBufferStreamSource.h"
#include "intermediate/IntermediateGeneratorSemanticsHandler.h"
#include "intermediate/SequenceBuilder.h"
#include "parser/Parser.h"
#include "BlockImpl.h"

#include "optimize/DeadCodeElimination.h"

#include "../../base/t/print_uc_String.h"
#include "../../diagnostics/t/TestDiagnosticsHandler.h"
#include "../../intermediate/t/print_TestSequenceVisitor_Operation.h"
#include "../../intermediate/t/TestSequenceVisitor.h"
#include "../../parser/t/TestParser.h"

using namespace s1::intermediate;
using namespace s1::optimize;

BOOST_AUTO_TEST_SUITE(DeadCodeElimination)

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

BOOST_AUTO_TEST_CASE(SimpleUnused)
{
  static const char blockSource[] =
    "unsigned int a;"
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
  s1::intermediate::RegisterSet usedRegs;
  bool codeEliminated = s1::optimize::DeadCodeElimination::EliminateDeadCode (newSeqBuilder, testBlockImpl->GetSequence(), usedRegs);
  BOOST_CHECK_EQUAL (codeEliminated, true);

  s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
  TestSequenceVisitor testVisitor;
  newSeq->Visit (testVisitor);
  BOOST_CHECK_EQUAL (testVisitor.entries.size(), 0u);
}

BOOST_AUTO_TEST_CASE(SimpleUsed)
{
  static const char blockSource[] =
    "unsigned int a;"
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
  s1::intermediate::RegisterSet usedRegs;
  usedRegs.insert (testBlockImpl->GetSequenceBuilder()->GetIdentifierRegister ("a"));
  bool codeEliminated = s1::optimize::DeadCodeElimination::EliminateDeadCode (newSeqBuilder, testBlockImpl->GetSequence(), usedRegs);
  BOOST_CHECK_EQUAL (codeEliminated, false);

  s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
  TestSequenceVisitor testVisitor;
  newSeq->Visit (testVisitor);
  BOOST_CHECK_EQUAL (testVisitor.entries.size(), 1u);
  BOOST_CHECK_EQUAL (testVisitor.entries[0].op, TestSequenceVisitor::opConstUInt);
}

BOOST_AUTO_TEST_CASE(ArithUnused)
{
  static const char blockSource[] =
    "unsigned int a = 1, b = 2, c;"
    "c = a+b;"
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
  s1::intermediate::RegisterSet usedRegs;
  bool codeEliminated = s1::optimize::DeadCodeElimination::EliminateDeadCode (newSeqBuilder, testBlockImpl->GetSequence(), usedRegs);
  BOOST_CHECK_EQUAL (codeEliminated, true);

  s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
  TestSequenceVisitor testVisitor;
  newSeq->Visit (testVisitor);
  BOOST_CHECK_EQUAL (testVisitor.entries.size(), 0u);
}

BOOST_AUTO_TEST_CASE(ArithUsed)
{
  static const char blockSource[] =
    "unsigned int a = 1, b = 2, c;"
    "c = a+b;"
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
  s1::intermediate::RegisterSet usedRegs;
  usedRegs.insert (testBlockImpl->GetSequenceBuilder()->GetIdentifierRegister ("c"));
  bool codeEliminated = s1::optimize::DeadCodeElimination::EliminateDeadCode (newSeqBuilder, testBlockImpl->GetSequence(), usedRegs);
  BOOST_CHECK_EQUAL (codeEliminated, false);

  s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
  TestSequenceVisitor testVisitor;
  newSeq->Visit (testVisitor);
  BOOST_CHECK_EQUAL (testVisitor.entries.size(), 3u);
  if (testVisitor.entries.size() < 3) return;
  BOOST_CHECK_EQUAL (testVisitor.entries[0].op, TestSequenceVisitor::opConstUInt);
  BOOST_CHECK_EQUAL (testVisitor.entries[1].op, TestSequenceVisitor::opConstUInt);
  BOOST_CHECK_EQUAL (testVisitor.entries[2].op, TestSequenceVisitor::opArithAdd);
}

BOOST_AUTO_TEST_CASE(BranchUnused)
{
  static const char blockSource[] =
    "unsigned int a;"
    "bool c;"
    "if (c)"
    "{"
    "  a = 1;"
    "}"
    "else"
    "{"
    "  a = 2;"
    "}"
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
  s1::intermediate::RegisterSet usedRegs;
  bool codeEliminated = s1::optimize::DeadCodeElimination::EliminateDeadCode (newSeqBuilder, testBlockImpl->GetSequence(), usedRegs);
  BOOST_CHECK_EQUAL (codeEliminated, true);

  s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
  TestSequenceVisitor testVisitor;
  newSeq->Visit (testVisitor);
  BOOST_CHECK_EQUAL (testVisitor.entries.size(), 0u);
}

BOOST_AUTO_TEST_CASE(BranchUsed)
{
  static const char blockSource[] =
    "unsigned int a;"
    "bool c;"
    "if (c)"
    "{"
    "  a = 1;"
    "}"
    "else"
    "{"
    "  a = 2;"
    "}"
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
  s1::intermediate::RegisterSet usedRegs;
  usedRegs.insert (testBlockImpl->GetSequenceBuilder()->GetIdentifierRegister ("a"));
  bool codeEliminated = s1::optimize::DeadCodeElimination::EliminateDeadCode (newSeqBuilder, testBlockImpl->GetSequence(), usedRegs);
  BOOST_CHECK_EQUAL (codeEliminated, false);

  s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
  TestSequenceVisitor testVisitor;
  newSeq->Visit (testVisitor);
  BOOST_CHECK_EQUAL (testVisitor.entries.size(), 1u);
  if (testVisitor.entries.size() < 1) return;
  BOOST_CHECK_EQUAL (testVisitor.entries[0].op, TestSequenceVisitor::opBranch);

  BOOST_CHECK (testVisitor.entries[0].branchIfVisitor);
  BOOST_CHECK_EQUAL (testVisitor.entries[0].branchIfVisitor->entries.size(), 1u);
  BOOST_CHECK_EQUAL (testVisitor.entries[0].branchIfVisitor->entries[0].op, TestSequenceVisitor::opBlock);
  BOOST_CHECK_EQUAL (testVisitor.entries[0].branchIfVisitor->entries[0].blockVisitor->entries.size(), 1u);
  if (testVisitor.entries[0].branchIfVisitor->entries[0].blockVisitor->entries.size() < 1) return;
  BOOST_CHECK_EQUAL (testVisitor.entries[0].branchIfVisitor->entries[0].blockVisitor->entries[0].op, TestSequenceVisitor::opConstUInt);

  BOOST_CHECK (testVisitor.entries[0].branchElseVisitor);
  BOOST_CHECK_EQUAL (testVisitor.entries[0].branchElseVisitor->entries.size(), 1u);
  BOOST_CHECK_EQUAL (testVisitor.entries[0].branchElseVisitor->entries[0].op, TestSequenceVisitor::opBlock);
  BOOST_CHECK_EQUAL (testVisitor.entries[0].branchElseVisitor->entries[0].blockVisitor->entries.size(), 1u);
  if (testVisitor.entries[0].branchElseVisitor->entries[0].blockVisitor->entries.size() < 1) return;
  BOOST_CHECK_EQUAL (testVisitor.entries[0].branchElseVisitor->entries[0].blockVisitor->entries[0].op, TestSequenceVisitor::opConstUInt);
}

BOOST_AUTO_TEST_CASE(BranchUsed2)
{
  static const char blockSource[] =
    "unsigned int a;"
    "bool c;"
    "if (c)"
    "{"
    "  a = 1;"
    "}"
    "else"
    "{"
    "  a = 2;"
    "}"
    "unsigned int b = a;"
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
  testBlockImpl->Finish();

  s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
  s1::intermediate::RegisterSet usedRegs;
  usedRegs.insert (testBlockImpl->GetSequenceBuilder()->GetIdentifierRegister ("b"));
  bool codeEliminated = s1::optimize::DeadCodeElimination::EliminateDeadCode (newSeqBuilder, testBlockImpl->GetSequence(), usedRegs);
  BOOST_CHECK_EQUAL (codeEliminated, false);

  s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
  TestSequenceVisitor testVisitor;
  newSeq->Visit (testVisitor);
  BOOST_CHECK_EQUAL (testVisitor.entries.size(), 2u);
  if (testVisitor.entries.size() < 2) return;
  BOOST_CHECK_EQUAL (testVisitor.entries[0].op, TestSequenceVisitor::opBranch);

  BOOST_CHECK (testVisitor.entries[0].branchIfVisitor);
  BOOST_CHECK_EQUAL (testVisitor.entries[0].branchIfVisitor->entries.size(), 1u);
  BOOST_CHECK_EQUAL (testVisitor.entries[0].branchIfVisitor->entries[0].op, TestSequenceVisitor::opBlock);
  BOOST_CHECK_EQUAL (testVisitor.entries[0].branchIfVisitor->entries[0].blockVisitor->entries.size(), 1u);
  if (testVisitor.entries[0].branchIfVisitor->entries[0].blockVisitor->entries.size() < 1) return;
  BOOST_CHECK_EQUAL (testVisitor.entries[0].branchIfVisitor->entries[0].blockVisitor->entries[0].op, TestSequenceVisitor::opConstUInt);

  BOOST_CHECK (testVisitor.entries[0].branchElseVisitor);
  BOOST_CHECK_EQUAL (testVisitor.entries[0].branchElseVisitor->entries.size(), 1u);
  BOOST_CHECK_EQUAL (testVisitor.entries[0].branchElseVisitor->entries[0].op, TestSequenceVisitor::opBlock);
  BOOST_CHECK_EQUAL (testVisitor.entries[0].branchElseVisitor->entries[0].blockVisitor->entries.size(), 1u);
  if (testVisitor.entries[0].branchElseVisitor->entries[0].blockVisitor->entries.size() < 1) return;
  BOOST_CHECK_EQUAL (testVisitor.entries[0].branchElseVisitor->entries[0].blockVisitor->entries[0].op, TestSequenceVisitor::opConstUInt);
}

BOOST_AUTO_TEST_CASE(BranchMixed)
{
  static const char blockSource[] =
    "unsigned int a, b;"
    "bool c;"
    "if (c)"
    "{"
    "  a = 1;"
    "  b = 3;"
    "}"
    "else"
    "{"
    "  a = 2;"
    "  b = 4;"
    "}"
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
  s1::intermediate::RegisterSet usedRegs;
  usedRegs.insert (testBlockImpl->GetSequenceBuilder()->GetIdentifierRegister ("a"));
  bool codeEliminated = s1::optimize::DeadCodeElimination::EliminateDeadCode (newSeqBuilder, testBlockImpl->GetSequence(), usedRegs);
  BOOST_CHECK_EQUAL (codeEliminated, true);

  s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
  TestSequenceVisitor testVisitor;
  newSeq->Visit (testVisitor);
  BOOST_CHECK_EQUAL (testVisitor.entries.size(), 1u);
  if (testVisitor.entries.size() < 1) return;
  BOOST_CHECK_EQUAL (testVisitor.entries[0].op, TestSequenceVisitor::opBranch);

  BOOST_CHECK (testVisitor.entries[0].branchIfVisitor);
  BOOST_CHECK_EQUAL (testVisitor.entries[0].branchIfVisitor->entries.size(), 1u);
  BOOST_CHECK_EQUAL (testVisitor.entries[0].branchIfVisitor->entries[0].op, TestSequenceVisitor::opBlock);
  BOOST_CHECK_EQUAL (testVisitor.entries[0].branchIfVisitor->entries[0].blockVisitor->entries.size(), 1u);
  if (testVisitor.entries[0].branchIfVisitor->entries[0].blockVisitor->entries.size() < 1) return;
  BOOST_CHECK_EQUAL (testVisitor.entries[0].branchIfVisitor->entries[0].blockVisitor->entries[0].op, TestSequenceVisitor::opConstUInt);
  // @@@ Slightly fragile, relies on imported regs naming
  BOOST_CHECK_EQUAL (testVisitor.entries[0].branchIfVisitor->entries[0].blockVisitor->entries[0].destReg->GetName(), s1::uc::String ("m_a_B0"));

  BOOST_CHECK (testVisitor.entries[0].branchElseVisitor);
  BOOST_CHECK_EQUAL (testVisitor.entries[0].branchElseVisitor->entries.size(), 1u);
  BOOST_CHECK_EQUAL (testVisitor.entries[0].branchElseVisitor->entries[0].op, TestSequenceVisitor::opBlock);
  BOOST_CHECK_EQUAL (testVisitor.entries[0].branchElseVisitor->entries[0].blockVisitor->entries.size(), 1u);
  if (testVisitor.entries[0].branchElseVisitor->entries[0].blockVisitor->entries.size() < 1) return;
  BOOST_CHECK_EQUAL (testVisitor.entries[0].branchElseVisitor->entries[0].blockVisitor->entries[0].op, TestSequenceVisitor::opConstUInt);
  // @@@ Slightly fragile, relies on imported regs naming
  BOOST_CHECK_EQUAL (testVisitor.entries[0].branchElseVisitor->entries[0].blockVisitor->entries[0].destReg->GetName(), s1::uc::String ("m_a_B0"));
}

BOOST_AUTO_TEST_CASE(LoopUnused)
{
  static const char blockSource[] =
    "unsigned int a;"
    "bool c;"
    "while (c)"
    "{"
    "  a = 1;"
    "}"
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
  s1::intermediate::RegisterSet usedRegs;
  bool codeEliminated = s1::optimize::DeadCodeElimination::EliminateDeadCode (newSeqBuilder, testBlockImpl->GetSequence(), usedRegs);
  BOOST_CHECK_EQUAL (codeEliminated, true);

  s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
  TestSequenceVisitor testVisitor;
  newSeq->Visit (testVisitor);
  BOOST_CHECK_EQUAL (testVisitor.entries.size(), 0u);
}

BOOST_AUTO_TEST_CASE(LoopUsed)
{
  static const char blockSource[] =
    "unsigned int a;"
    "bool c;"
    "while (c)"
    "{"
    "  a = 1;"
    "}"
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
  s1::intermediate::RegisterSet usedRegs;
  usedRegs.insert (testBlockImpl->GetSequenceBuilder()->GetIdentifierRegister ("a"));
  bool codeEliminated = s1::optimize::DeadCodeElimination::EliminateDeadCode (newSeqBuilder, testBlockImpl->GetSequence(), usedRegs);
  BOOST_CHECK_EQUAL (codeEliminated, false);

  s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
  TestSequenceVisitor testVisitor;
  newSeq->Visit (testVisitor);
  BOOST_CHECK_EQUAL (testVisitor.entries.size(), 2u);
  if (testVisitor.entries.size() < 2) return;
  // Op1: condition variable assignment
  BOOST_CHECK_EQUAL (testVisitor.entries[0].op, TestSequenceVisitor::opAssignment);
  // Op2: actual while loop
  BOOST_CHECK_EQUAL (testVisitor.entries[1].op, TestSequenceVisitor::opWhile);
}

BOOST_AUTO_TEST_SUITE_END()
