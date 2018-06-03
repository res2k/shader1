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
#include <boost/cstdint.hpp>
#include <boost/make_shared.hpp>

#include "intermediate/IntermediateGeneratorSemanticsHandler.h"
#include "parser/Parser.h"
#include "BlockImpl.h"

#include "optimize/Inliner.h"
#include "t/TestSequenceVisitor.h"
#include "t/IntermediateTestTraits.h"
#include "../../parser/t/TestParser.h"

using namespace s1::intermediate;
using namespace s1::optimize;

class InlineBlockTestSuite : public CxxTest::TestSuite
{
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
public:
  void testInlineBlock (void)
  {
    static const char blockSource[] =
      "unsigned int a;"
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
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TS_ASSERT(errorHandler.parseErrors.empty());

    boost::shared_ptr<TestSemanticsHandler::TestBlockImpl> testBlockImpl (
      boost::static_pointer_cast<TestSemanticsHandler::TestBlockImpl> (block));

    s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
    bool blocksInlined = s1::optimize::Inliner::InlineAllBlocks (newSeqBuilder, testBlockImpl->GetSequence());
    TS_ASSERT_EQUALS (blocksInlined, true);

    s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
    TestSequenceVisitor testVisitor;
    newSeq->Visit (testVisitor);
    TS_ASSERT_EQUALS (testVisitor.entries.size(), 1);
    TS_ASSERT_EQUALS (testVisitor.entries[0].op, TestSequenceVisitor::opConstUInt);
  }

  void testInlineBlockNested (void)
  {
    static const char blockSource[] =
      "unsigned int a;"
      "{"
      "  {"
      "    a = 1;"
      "  }"
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
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TS_ASSERT(errorHandler.parseErrors.empty());

    boost::shared_ptr<TestSemanticsHandler::TestBlockImpl> testBlockImpl (
      boost::static_pointer_cast<TestSemanticsHandler::TestBlockImpl> (block));

    s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
    bool blocksInlined = s1::optimize::Inliner::InlineAllBlocks (newSeqBuilder, testBlockImpl->GetSequence());
    TS_ASSERT_EQUALS (blocksInlined, true);

    s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
    TestSequenceVisitor testVisitor;
    newSeq->Visit (testVisitor);
    TS_ASSERT_EQUALS (testVisitor.entries.size(), 1);
    TS_ASSERT_EQUALS (testVisitor.entries[0].op, TestSequenceVisitor::opConstUInt);
  }

  void testCondition1 (void)
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
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TS_ASSERT(errorHandler.parseErrors.empty());

    boost::shared_ptr<TestSemanticsHandler::TestBlockImpl> testBlockImpl (
      boost::static_pointer_cast<TestSemanticsHandler::TestBlockImpl> (block));

    s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
    bool blocksInlined = s1::optimize::Inliner::InlineAllBlocks (newSeqBuilder, testBlockImpl->GetSequence());
    TS_ASSERT_EQUALS (blocksInlined, false);

    s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
    TestSequenceVisitor testVisitor;
    newSeq->Visit (testVisitor);
    TS_ASSERT_EQUALS (testVisitor.entries.size(), 1);
    TS_ASSERT_EQUALS (testVisitor.entries[0].op, TestSequenceVisitor::opBranch);

    TS_ASSERT (testVisitor.entries[0].branchIfVisitor);
    TS_ASSERT_EQUALS (testVisitor.entries[0].branchIfVisitor->entries.size(), 1);
    TS_ASSERT_EQUALS (testVisitor.entries[0].branchIfVisitor->entries[0].op, TestSequenceVisitor::opBlock);
    TS_ASSERT_EQUALS (testVisitor.entries[0].branchIfVisitor->entries[0].blockVisitor->entries.size(), 1);
    TS_ASSERT_EQUALS (testVisitor.entries[0].branchIfVisitor->entries[0].blockVisitor->entries[0].op, TestSequenceVisitor::opConstUInt);

    TS_ASSERT (testVisitor.entries[0].branchElseVisitor);
    TS_ASSERT_EQUALS (testVisitor.entries[0].branchElseVisitor->entries.size(), 1);
    TS_ASSERT_EQUALS (testVisitor.entries[0].branchElseVisitor->entries[0].op, TestSequenceVisitor::opBlock);
    TS_ASSERT_EQUALS (testVisitor.entries[0].branchElseVisitor->entries[0].blockVisitor->entries.size(), 1);
    TS_ASSERT_EQUALS (testVisitor.entries[0].branchElseVisitor->entries[0].blockVisitor->entries[0].op, TestSequenceVisitor::opConstUInt);
  }

  void testCondition2 (void)
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
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TS_ASSERT(errorHandler.parseErrors.empty());

    boost::shared_ptr<TestSemanticsHandler::TestBlockImpl> testBlockImpl (
      boost::static_pointer_cast<TestSemanticsHandler::TestBlockImpl> (block));

    s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
    bool blocksInlined = s1::optimize::Inliner::InlineAllBlocks (newSeqBuilder, testBlockImpl->GetSequence());
    TS_ASSERT_EQUALS (blocksInlined, false);

    s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
    TestSequenceVisitor testVisitor;
    newSeq->Visit (testVisitor);
    TS_ASSERT_EQUALS (testVisitor.entries.size(), 1);
    TS_ASSERT_EQUALS (testVisitor.entries[0].op, TestSequenceVisitor::opBranch);

    TS_ASSERT (testVisitor.entries[0].branchIfVisitor);
    TS_ASSERT_EQUALS (testVisitor.entries[0].branchIfVisitor->entries.size(), 1);
    TS_ASSERT_EQUALS (testVisitor.entries[0].branchIfVisitor->entries[0].op, TestSequenceVisitor::opBlock);
    TS_ASSERT_EQUALS (testVisitor.entries[0].branchIfVisitor->entries[0].blockVisitor->entries.size(), 2);
    TS_ASSERT_EQUALS (testVisitor.entries[0].branchIfVisitor->entries[0].blockVisitor->entries[0].op, TestSequenceVisitor::opConstUInt);
    TS_ASSERT_EQUALS (testVisitor.entries[0].branchIfVisitor->entries[0].blockVisitor->entries[1].op, TestSequenceVisitor::opConstUInt);

    TS_ASSERT (testVisitor.entries[0].branchElseVisitor);
    TS_ASSERT_EQUALS (testVisitor.entries[0].branchElseVisitor->entries.size(), 1);
    TS_ASSERT_EQUALS (testVisitor.entries[0].branchElseVisitor->entries[0].op, TestSequenceVisitor::opBlock);
    TS_ASSERT_EQUALS (testVisitor.entries[0].branchElseVisitor->entries[0].blockVisitor->entries.size(), 2);
    TS_ASSERT_EQUALS (testVisitor.entries[0].branchElseVisitor->entries[0].blockVisitor->entries[0].op, TestSequenceVisitor::opConstUInt);
    TS_ASSERT_EQUALS (testVisitor.entries[0].branchElseVisitor->entries[0].blockVisitor->entries[1].op, TestSequenceVisitor::opConstUInt);
  }

  void testConditionWithNestedBlock (void)
  {
    static const char blockSource[] =
      "unsigned int a;"
      "bool c;"
      "if (c)"
      "{"
      "  {"
      "    a = 1;"
      "  }"
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
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TS_ASSERT(errorHandler.parseErrors.empty());

    boost::shared_ptr<TestSemanticsHandler::TestBlockImpl> testBlockImpl (
      boost::static_pointer_cast<TestSemanticsHandler::TestBlockImpl> (block));

    s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
    bool blocksInlined = s1::optimize::Inliner::InlineAllBlocks (newSeqBuilder, testBlockImpl->GetSequence());
    TS_ASSERT_EQUALS (blocksInlined, true);

    s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
    TestSequenceVisitor testVisitor;
    newSeq->Visit (testVisitor);
    TS_ASSERT_EQUALS (testVisitor.entries.size(), 1);
    TS_ASSERT_EQUALS (testVisitor.entries[0].op, TestSequenceVisitor::opBranch);

    TS_ASSERT (testVisitor.entries[0].branchIfVisitor);
    TS_ASSERT_EQUALS (testVisitor.entries[0].branchIfVisitor->entries.size(), 1);
    TS_ASSERT_EQUALS (testVisitor.entries[0].branchIfVisitor->entries[0].op, TestSequenceVisitor::opBlock);
    TS_ASSERT_EQUALS (testVisitor.entries[0].branchIfVisitor->entries[0].blockVisitor->entries.size(), 1);
    TS_ASSERT_EQUALS (testVisitor.entries[0].branchIfVisitor->entries[0].blockVisitor->entries[0].op, TestSequenceVisitor::opConstUInt);

    TS_ASSERT (testVisitor.entries[0].branchElseVisitor);
    TS_ASSERT_EQUALS (testVisitor.entries[0].branchElseVisitor->entries.size(), 1);
    TS_ASSERT_EQUALS (testVisitor.entries[0].branchElseVisitor->entries[0].op, TestSequenceVisitor::opBlock);
    TS_ASSERT_EQUALS (testVisitor.entries[0].branchElseVisitor->entries[0].blockVisitor->entries.size(), 1);
    TS_ASSERT_EQUALS (testVisitor.entries[0].branchElseVisitor->entries[0].blockVisitor->entries[0].op, TestSequenceVisitor::opConstUInt);
  }

  void testNestedBlockWithCondition (void)
  {
    static const char blockSource[] =
      "unsigned int a;"
      "bool c;"
      "{"
      "  if (c)"
      "  {"
      "    a = 1;"
      "  }"
      "  else"
      "  {"
      "    a = 2;"
      "  }"
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
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TS_ASSERT(errorHandler.parseErrors.empty());

    boost::shared_ptr<TestSemanticsHandler::TestBlockImpl> testBlockImpl (
      boost::static_pointer_cast<TestSemanticsHandler::TestBlockImpl> (block));

    s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
    bool blocksInlined = s1::optimize::Inliner::InlineAllBlocks (newSeqBuilder, testBlockImpl->GetSequence());
    TS_ASSERT_EQUALS (blocksInlined, true);

    s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
    TestSequenceVisitor testVisitor;
    newSeq->Visit (testVisitor);
    TS_ASSERT_EQUALS (testVisitor.entries.size(), 1);
    TS_ASSERT_EQUALS (testVisitor.entries[0].op, TestSequenceVisitor::opBranch);

    TS_ASSERT (testVisitor.entries[0].branchIfVisitor);
    TS_ASSERT_EQUALS (testVisitor.entries[0].branchIfVisitor->entries.size(), 1);
    TS_ASSERT_EQUALS (testVisitor.entries[0].branchIfVisitor->entries[0].op, TestSequenceVisitor::opBlock);
    TS_ASSERT_EQUALS (testVisitor.entries[0].branchIfVisitor->entries[0].blockVisitor->entries.size(), 1);
    TS_ASSERT_EQUALS (testVisitor.entries[0].branchIfVisitor->entries[0].blockVisitor->entries[0].op, TestSequenceVisitor::opConstUInt);

    TS_ASSERT (testVisitor.entries[0].branchElseVisitor);
    TS_ASSERT_EQUALS (testVisitor.entries[0].branchElseVisitor->entries.size(), 1);
    TS_ASSERT_EQUALS (testVisitor.entries[0].branchElseVisitor->entries[0].op, TestSequenceVisitor::opBlock);
    TS_ASSERT_EQUALS (testVisitor.entries[0].branchElseVisitor->entries[0].blockVisitor->entries.size(), 1);
    TS_ASSERT_EQUALS (testVisitor.entries[0].branchElseVisitor->entries[0].blockVisitor->entries[0].op, TestSequenceVisitor::opConstUInt);
  }

};
