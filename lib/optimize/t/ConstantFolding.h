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

#define _USE_MATH_DEFINES
#include <math.h>

#include "base/uc/SimpleBufferStreamSource.h"
#include "intermediate/IntermediateGeneratorSemanticsHandler.h"
#include "intermediate/SequenceBuilder.h"
#include "lexer/LexerErrorHandler.h"
#include "parser/Parser.h"
#include "BlockImpl.h"
#include "TypeImpl.h"

#include "optimize/ConstantFolding.h"
#include "t/TestSequenceVisitor.h"
#include "t/IntermediateTestTraits.h"

using namespace s1::intermediate;
using namespace s1::optimize;

class ConstantFoldingTestSuite : public CxxTest::TestSuite 
{
  class TestParser : public s1::Parser
  {
  public:
    TestParser (s1::Lexer& inputLexer, s1::parser::SemanticsHandler& semanticsHandler,
		s1::parser::ErrorHandler& errorHandler)
     : Parser (inputLexer, semanticsHandler, errorHandler) {}
    
    using s1::Parser::ParseBlock;
  };
  
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
  void testSimpleCast (void)
  {
    static const char blockSource[] =
      "float a;"
      "a = 1;"
      ;
    
    using namespace s1::parser;
    
    s1::uc::SimpleBufferStreamSource in (blockSource, strlen (blockSource));
    s1::uc::Stream ustream (in);
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    // global scope is required so BlockImpl can create some unique var names
    SemanticsHandler::ScopePtr globalScope (semanticsHandler.CreateScope (SemanticsHandler::ScopePtr(),
					    TestSemanticsHandler::Global,
					    semanticsHandler.GetVoidType()));
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (globalScope));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    
    boost::shared_ptr<TestSemanticsHandler::TestBlockImpl> testBlockImpl (
      boost::static_pointer_cast<TestSemanticsHandler::TestBlockImpl> (block));
      
    s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
    bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
    TS_ASSERT_EQUALS (seqChanged, true);
    
    s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
    TestSequenceVisitor testVisitor;
    newSeq->Visit (testVisitor);
    TS_ASSERT_EQUALS (testVisitor.entries.size(), 2);
    if (testVisitor.entries.size() < 1) return;
    TS_ASSERT_EQUALS (testVisitor.entries[0].op, TestSequenceVisitor::opConstUInt);
    if (testVisitor.entries.size() < 2) return;
    TS_ASSERT_EQUALS (testVisitor.entries[1].op, TestSequenceVisitor::opConstFloat);
  }
  
  void testArrayExtract (void)
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
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    // global scope is required so BlockImpl can create some unique var names
    SemanticsHandler::ScopePtr globalScope (semanticsHandler.CreateScope (SemanticsHandler::ScopePtr(),
					    TestSemanticsHandler::Global,
					    semanticsHandler.GetVoidType()));
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (globalScope));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    
    boost::shared_ptr<TestSemanticsHandler::TestBlockImpl> testBlockImpl (
      boost::static_pointer_cast<TestSemanticsHandler::TestBlockImpl> (block));
      
    s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
    bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
    TS_ASSERT_EQUALS (seqChanged, true);
    
    s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
    TestSequenceVisitor testVisitor;
    newSeq->Visit (testVisitor);
    TS_ASSERT_EQUALS (testVisitor.entries.size(), 6);
    if (testVisitor.entries.size() < 6) return;
    // Set up array
    TS_ASSERT_EQUALS (testVisitor.entries[0].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[1].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[2].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[3].op, TestSequenceVisitor::opMakeArray);
    // Const for index (unused)
    TS_ASSERT_EQUALS (testVisitor.entries[4].op, TestSequenceVisitor::opConstUInt);
    // Extract array member - optimized: no extract op, simple assignment instead
    TS_ASSERT_EQUALS (testVisitor.entries[5].op, TestSequenceVisitor::opAssignment);
  }
  
  void testArrayChange (void)
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
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    // global scope is required so BlockImpl can create some unique var names
    SemanticsHandler::ScopePtr globalScope (semanticsHandler.CreateScope (SemanticsHandler::ScopePtr(),
					    TestSemanticsHandler::Global,
					    semanticsHandler.GetVoidType()));
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (globalScope));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    
    boost::shared_ptr<TestSemanticsHandler::TestBlockImpl> testBlockImpl (
      boost::static_pointer_cast<TestSemanticsHandler::TestBlockImpl> (block));
      
    s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
    bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
    TS_ASSERT_EQUALS (seqChanged, true);
    
    s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
    TestSequenceVisitor testVisitor;
    newSeq->Visit (testVisitor);
    TS_ASSERT_EQUALS (testVisitor.entries.size(), 10);
    if (testVisitor.entries.size() < 10) return;
    // Set up array
    TS_ASSERT_EQUALS (testVisitor.entries[0].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[1].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[2].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[3].op, TestSequenceVisitor::opMakeArray);
    // New element value
    TS_ASSERT_EQUALS (testVisitor.entries[4].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[5].op, TestSequenceVisitor::opAssignment);
    // Const for index
    TS_ASSERT_EQUALS (testVisitor.entries[6].op, TestSequenceVisitor::opConstUInt);
    // Change element
    TS_ASSERT_EQUALS (testVisitor.entries[7].op, TestSequenceVisitor::opChangeArrayElement);
    // Const for index (unused)
    TS_ASSERT_EQUALS (testVisitor.entries[8].op, TestSequenceVisitor::opConstUInt);
    // Extract array member - optimized: no extract op, simple assignment instead
    TS_ASSERT_EQUALS (testVisitor.entries[9].op, TestSequenceVisitor::opAssignment);
  }
  
  void testArrayLen (void)
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
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    // global scope is required so BlockImpl can create some unique var names
    SemanticsHandler::ScopePtr globalScope (semanticsHandler.CreateScope (SemanticsHandler::ScopePtr(),
					    TestSemanticsHandler::Global,
					    semanticsHandler.GetVoidType()));
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (globalScope));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    
    boost::shared_ptr<TestSemanticsHandler::TestBlockImpl> testBlockImpl (
      boost::static_pointer_cast<TestSemanticsHandler::TestBlockImpl> (block));
      
    s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
    bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
    TS_ASSERT_EQUALS (seqChanged, true);
    
    s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
    TestSequenceVisitor testVisitor;
    newSeq->Visit (testVisitor);
    TS_ASSERT_EQUALS (testVisitor.entries.size(), 5);
    if (testVisitor.entries.size() < 5) return;
    // Set up array
    TS_ASSERT_EQUALS (testVisitor.entries[0].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[1].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[2].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[3].op, TestSequenceVisitor::opMakeArray);
    // Get length - optimized: no array length op, constant instead
    TS_ASSERT_EQUALS (testVisitor.entries[4].op, TestSequenceVisitor::opConstUInt);
  }
  
  void testVectorExtract (void)
  {
    static const char blockSource[] =
      "float2 a = float2 (1.0, 2.0);"
      "float b;"
      "b = a.y;"
      ;
    
    using namespace s1::parser;
    
    s1::uc::SimpleBufferStreamSource in (blockSource, strlen (blockSource));
    s1::uc::Stream ustream (in);
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    // global scope is required so BlockImpl can create some unique var names
    SemanticsHandler::ScopePtr globalScope (semanticsHandler.CreateScope (SemanticsHandler::ScopePtr(),
					    TestSemanticsHandler::Global,
					    semanticsHandler.GetVoidType()));
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (globalScope));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    
    boost::shared_ptr<TestSemanticsHandler::TestBlockImpl> testBlockImpl (
      boost::static_pointer_cast<TestSemanticsHandler::TestBlockImpl> (block));
      
    s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
    bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
    TS_ASSERT_EQUALS (seqChanged, true);
    
    s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
    TestSequenceVisitor testVisitor;
    newSeq->Visit (testVisitor);
    TS_ASSERT_EQUALS (testVisitor.entries.size(), 4);
    if (testVisitor.entries.size() < 4) return;
    // Set up vector
    TS_ASSERT_EQUALS (testVisitor.entries[0].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[1].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[2].op, TestSequenceVisitor::opMakeVectorFloat);
    // Extract vector component - optimized: no extract op, const value instead
    TS_ASSERT_EQUALS (testVisitor.entries[3].op, TestSequenceVisitor::opConstFloat);
  }
  
  void testArithSimple (void)
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
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    // global scope is required so BlockImpl can create some unique var names
    SemanticsHandler::ScopePtr globalScope (semanticsHandler.CreateScope (SemanticsHandler::ScopePtr(),
					    TestSemanticsHandler::Global,
					    semanticsHandler.GetVoidType()));
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (globalScope));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    
    boost::shared_ptr<TestSemanticsHandler::TestBlockImpl> testBlockImpl (
      boost::static_pointer_cast<TestSemanticsHandler::TestBlockImpl> (block));
      
    s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
    bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
    TS_ASSERT_EQUALS (seqChanged, true);
    
    s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
    TestSequenceVisitor testVisitor;
    newSeq->Visit (testVisitor);
    TS_ASSERT_EQUALS (testVisitor.entries.size(), 7);
    if (testVisitor.entries.size() < 7) return;
    // Operand assignments
    TS_ASSERT_EQUALS (testVisitor.entries[0].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[1].op, TestSequenceVisitor::opConstFloat);
    // Addition - optimized: no arith op, const value instead
    TS_ASSERT_EQUALS (testVisitor.entries[2].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[2].floatConst, 5.0);
    // Subtraction - optimized: no arith op, const value instead
    TS_ASSERT_EQUALS (testVisitor.entries[3].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[3].floatConst, 1.0);
    // Multiplication - optimized: no arith op, const value instead
    TS_ASSERT_EQUALS (testVisitor.entries[4].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[4].floatConst, 6.0);
    // Division - optimized: no arith op, const value instead
    TS_ASSERT_EQUALS (testVisitor.entries[5].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[5].floatConst, 1.5);
    // Modulo - optimized: no arith op, const value instead
    TS_ASSERT_EQUALS (testVisitor.entries[6].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[6].floatConst, 1.0);
  }

  void testArithVector (void)
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
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    // global scope is required so BlockImpl can create some unique var names
    SemanticsHandler::ScopePtr globalScope (semanticsHandler.CreateScope (SemanticsHandler::ScopePtr(),
					    TestSemanticsHandler::Global,
					    semanticsHandler.GetVoidType()));
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (globalScope));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    
    boost::shared_ptr<TestSemanticsHandler::TestBlockImpl> testBlockImpl (
      boost::static_pointer_cast<TestSemanticsHandler::TestBlockImpl> (block));
      
    s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
    bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
    TS_ASSERT_EQUALS (seqChanged, true);
    
    s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
    TestSequenceVisitor testVisitor;
    newSeq->Visit (testVisitor);
    TS_ASSERT_EQUALS (testVisitor.entries.size(), 21);
    if (testVisitor.entries.size() < 21) return;
    // Operand assignments
    TS_ASSERT_EQUALS (testVisitor.entries[0].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[1].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[2].op, TestSequenceVisitor::opMakeVectorFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[3].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[4].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[5].op, TestSequenceVisitor::opMakeVectorFloat);
    // Addition - optimized: no arith op, const value instead
    TS_ASSERT_EQUALS (testVisitor.entries[6].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[6].floatConst, 5.0);
    TS_ASSERT_EQUALS (testVisitor.entries[7].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[7].floatConst, 0.75f);
    TS_ASSERT_EQUALS (testVisitor.entries[8].op, TestSequenceVisitor::opMakeVectorFloat);
    // Subtraction - optimized: no arith op, const value instead
    TS_ASSERT_EQUALS (testVisitor.entries[9].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[9].floatConst, 1.0);
    TS_ASSERT_EQUALS (testVisitor.entries[10].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[10].floatConst, 0.25f);
    TS_ASSERT_EQUALS (testVisitor.entries[11].op, TestSequenceVisitor::opMakeVectorFloat);
    // Multiplication - optimized: no arith op, const value instead
    TS_ASSERT_EQUALS (testVisitor.entries[12].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[12].floatConst, 6.0);
    TS_ASSERT_EQUALS (testVisitor.entries[13].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[13].floatConst, 0.125f);
    TS_ASSERT_EQUALS (testVisitor.entries[14].op, TestSequenceVisitor::opMakeVectorFloat);
    // Division - optimized: no arith op, const value instead
    TS_ASSERT_EQUALS (testVisitor.entries[15].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[15].floatConst, 1.5);
    TS_ASSERT_EQUALS (testVisitor.entries[16].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[16].floatConst, 2.0);
    TS_ASSERT_EQUALS (testVisitor.entries[17].op, TestSequenceVisitor::opMakeVectorFloat);
    // Modulo - optimized: no arith op, const value instead
    TS_ASSERT_EQUALS (testVisitor.entries[18].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[18].floatConst, 1.0);
    TS_ASSERT_EQUALS (testVisitor.entries[19].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[19].floatConst, 0);
    TS_ASSERT_EQUALS (testVisitor.entries[20].op, TestSequenceVisitor::opMakeVectorFloat);
  }

  void testLogicSimple (void)
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
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    // global scope is required so BlockImpl can create some unique var names
    SemanticsHandler::ScopePtr globalScope (semanticsHandler.CreateScope (SemanticsHandler::ScopePtr(),
					    TestSemanticsHandler::Global,
					    semanticsHandler.GetVoidType()));
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (globalScope));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    
    boost::shared_ptr<TestSemanticsHandler::TestBlockImpl> testBlockImpl (
      boost::static_pointer_cast<TestSemanticsHandler::TestBlockImpl> (block));
      
    s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
    bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
    TS_ASSERT_EQUALS (seqChanged, true);
    
    s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
    TestSequenceVisitor testVisitor;
    newSeq->Visit (testVisitor);
    TS_ASSERT_EQUALS (testVisitor.entries.size(), 4);
    if (testVisitor.entries.size() < 4) return;
    // Operand assignments
    TS_ASSERT_EQUALS (testVisitor.entries[0].op, TestSequenceVisitor::opConstBool);
    TS_ASSERT_EQUALS (testVisitor.entries[1].op, TestSequenceVisitor::opConstBool);
    // AND - optimized: no logic op, const value instead
    TS_ASSERT_EQUALS (testVisitor.entries[2].op, TestSequenceVisitor::opConstBool);
    TS_ASSERT_EQUALS (testVisitor.entries[2].boolConst, false);
    // OR - optimized: no logic op, const value instead
    TS_ASSERT_EQUALS (testVisitor.entries[3].op, TestSequenceVisitor::opConstBool);
    TS_ASSERT_EQUALS (testVisitor.entries[3].boolConst, true);
  }

  void testUnary (void)
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
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    // global scope is required so BlockImpl can create some unique var names
    SemanticsHandler::ScopePtr globalScope (semanticsHandler.CreateScope (SemanticsHandler::ScopePtr(),
					    TestSemanticsHandler::Global,
					    semanticsHandler.GetVoidType()));
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (globalScope));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    
    boost::shared_ptr<TestSemanticsHandler::TestBlockImpl> testBlockImpl (
      boost::static_pointer_cast<TestSemanticsHandler::TestBlockImpl> (block));
      
    s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
    bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
    TS_ASSERT_EQUALS (seqChanged, true);
    
    s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
    TestSequenceVisitor testVisitor;
    newSeq->Visit (testVisitor);
    TS_ASSERT_EQUALS (testVisitor.entries.size(), 7);
    if (testVisitor.entries.size() < 7) return;
    // 'x' initialization
    TS_ASSERT_EQUALS (testVisitor.entries[0].op, TestSequenceVisitor::opConstFloat);
    // 'y' initialization
    TS_ASSERT_EQUALS (testVisitor.entries[1].op, TestSequenceVisitor::opConstUInt);
    // Optimized: int constant instead of cast op
    TS_ASSERT_EQUALS (testVisitor.entries[2].op, TestSequenceVisitor::opConstInt);
    // 'c' initialization
    TS_ASSERT_EQUALS (testVisitor.entries[3].op, TestSequenceVisitor::opConstBool);
    // Float neg - optimized: constant instead of neg op
    TS_ASSERT_EQUALS (testVisitor.entries[4].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[4].floatConst, -1.25);
    // Int neg - optimized: constant instead of neg op
    TS_ASSERT_EQUALS (testVisitor.entries[5].op, TestSequenceVisitor::opConstInt);
    TS_ASSERT_EQUALS (testVisitor.entries[5].intConst, -3);
    // Bool not - optimized: constant instead of not op
    TS_ASSERT_EQUALS (testVisitor.entries[6].op, TestSequenceVisitor::opConstBool);
    TS_ASSERT_EQUALS (testVisitor.entries[6].boolConst, true);
  }

  void testCompare (void)
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
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    // global scope is required so BlockImpl can create some unique var names
    SemanticsHandler::ScopePtr globalScope (semanticsHandler.CreateScope (SemanticsHandler::ScopePtr(),
					    TestSemanticsHandler::Global,
					    semanticsHandler.GetVoidType()));
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (globalScope));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    
    boost::shared_ptr<TestSemanticsHandler::TestBlockImpl> testBlockImpl (
      boost::static_pointer_cast<TestSemanticsHandler::TestBlockImpl> (block));
      
    s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
    bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
    TS_ASSERT_EQUALS (seqChanged, true);
    
    s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
    TestSequenceVisitor testVisitor;
    newSeq->Visit (testVisitor);
    TS_ASSERT_EQUALS (testVisitor.entries.size(), 15);
    if (testVisitor.entries.size() < 15) return;
    // 'x' initialization
    TS_ASSERT_EQUALS (testVisitor.entries[0].op, TestSequenceVisitor::opConstUInt);
    // 'y' initialization
    TS_ASSERT_EQUALS (testVisitor.entries[1].op, TestSequenceVisitor::opConstUInt);
    // 'z' initialization
    TS_ASSERT_EQUALS (testVisitor.entries[2].op, TestSequenceVisitor::opConstUInt);
    // Optimized: bool constant instead of compare op
    TS_ASSERT_EQUALS (testVisitor.entries[3].op, TestSequenceVisitor::opConstBool);
    TS_ASSERT_EQUALS (testVisitor.entries[3].boolConst, false);
    // Optimized: bool constant instead of compare op
    TS_ASSERT_EQUALS (testVisitor.entries[4].op, TestSequenceVisitor::opConstBool);
    TS_ASSERT_EQUALS (testVisitor.entries[4].boolConst, true);
    // Optimized: bool constant instead of compare op
    TS_ASSERT_EQUALS (testVisitor.entries[5].op, TestSequenceVisitor::opConstBool);
    TS_ASSERT_EQUALS (testVisitor.entries[5].boolConst, true);
    // Optimized: bool constant instead of compare op
    TS_ASSERT_EQUALS (testVisitor.entries[6].op, TestSequenceVisitor::opConstBool);
    TS_ASSERT_EQUALS (testVisitor.entries[6].boolConst, true);
    // Optimized: bool constant instead of compare op
    TS_ASSERT_EQUALS (testVisitor.entries[7].op, TestSequenceVisitor::opConstBool);
    TS_ASSERT_EQUALS (testVisitor.entries[7].boolConst, false);
    // Optimized: bool constant instead of compare op
    TS_ASSERT_EQUALS (testVisitor.entries[8].op, TestSequenceVisitor::opConstBool);
    TS_ASSERT_EQUALS (testVisitor.entries[8].boolConst, false);
    
    // Optimized: bool constant instead of compare op
    TS_ASSERT_EQUALS (testVisitor.entries[9].op, TestSequenceVisitor::opConstBool);
    TS_ASSERT_EQUALS (testVisitor.entries[9].boolConst, true);
    // Optimized: bool constant instead of compare op
    TS_ASSERT_EQUALS (testVisitor.entries[10].op, TestSequenceVisitor::opConstBool);
    TS_ASSERT_EQUALS (testVisitor.entries[10].boolConst, false);
    // Optimized: bool constant instead of compare op
    TS_ASSERT_EQUALS (testVisitor.entries[11].op, TestSequenceVisitor::opConstBool);
    TS_ASSERT_EQUALS (testVisitor.entries[11].boolConst, false);
    // Optimized: bool constant instead of compare op
    TS_ASSERT_EQUALS (testVisitor.entries[12].op, TestSequenceVisitor::opConstBool);
    TS_ASSERT_EQUALS (testVisitor.entries[12].boolConst, true);
    // Optimized: bool constant instead of compare op
    TS_ASSERT_EQUALS (testVisitor.entries[13].op, TestSequenceVisitor::opConstBool);
    TS_ASSERT_EQUALS (testVisitor.entries[13].boolConst, false);
    // Optimized: bool constant instead of compare op
    TS_ASSERT_EQUALS (testVisitor.entries[14].op, TestSequenceVisitor::opConstBool);
    TS_ASSERT_EQUALS (testVisitor.entries[14].boolConst, true);
  }

  void testBlock (void)
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
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    // global scope is required so BlockImpl can create some unique var names
    SemanticsHandler::ScopePtr globalScope (semanticsHandler.CreateScope (SemanticsHandler::ScopePtr(),
					    TestSemanticsHandler::Global,
					    semanticsHandler.GetVoidType()));
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (globalScope));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    
    boost::shared_ptr<TestSemanticsHandler::TestBlockImpl> testBlockImpl (
      boost::static_pointer_cast<TestSemanticsHandler::TestBlockImpl> (block));
      
    s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
    bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
    TS_ASSERT_EQUALS (seqChanged, true);
    
    s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
    TestSequenceVisitor testVisitor;
    newSeq->Visit (testVisitor);
    TS_ASSERT_EQUALS (testVisitor.entries.size(), 4);
    if (testVisitor.entries.size() < 4) return;
    // 'x' initialization
    TS_ASSERT_EQUALS (testVisitor.entries[0].op, TestSequenceVisitor::opConstUInt);
    // 'y' initialization
    TS_ASSERT_EQUALS (testVisitor.entries[1].op, TestSequenceVisitor::opConstUInt);
    // Nested block
    TS_ASSERT_EQUALS (testVisitor.entries[2].op, TestSequenceVisitor::opBlock);
    // Optimized: constant instead of arith op
    TS_ASSERT_EQUALS (testVisitor.entries[3].op, TestSequenceVisitor::opConstUInt);
    TS_ASSERT_EQUALS (testVisitor.entries[3].uintConst, 7);
  }

  void testBranchNonConst (void)
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
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    // global scope is required so BlockImpl can create some unique var names
    SemanticsHandler::ScopePtr globalScope (semanticsHandler.CreateScope (SemanticsHandler::ScopePtr(),
					    TestSemanticsHandler::Global,
					    semanticsHandler.GetVoidType()));
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (globalScope));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    
    boost::shared_ptr<TestSemanticsHandler::TestBlockImpl> testBlockImpl (
      boost::static_pointer_cast<TestSemanticsHandler::TestBlockImpl> (block));
      
    s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
    bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
    TS_ASSERT_EQUALS (seqChanged, false);
    
    s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
    TestSequenceVisitor testVisitor;
    newSeq->Visit (testVisitor);
    TS_ASSERT_EQUALS (testVisitor.entries.size(), 3);
    if (testVisitor.entries.size() < 3) return;
    // Branch op
    TS_ASSERT_EQUALS (testVisitor.entries[0].op, TestSequenceVisitor::opBranch);
    // 'z' assignment
    TS_ASSERT_EQUALS (testVisitor.entries[1].op, TestSequenceVisitor::opConstUInt);
    // 'y' assignment
    TS_ASSERT_EQUALS (testVisitor.entries[2].op, TestSequenceVisitor::opArithMul);
  }

  void testBranchConst (void)
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
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    // global scope is required so BlockImpl can create some unique var names
    SemanticsHandler::ScopePtr globalScope (semanticsHandler.CreateScope (SemanticsHandler::ScopePtr(),
					    TestSemanticsHandler::Global,
					    semanticsHandler.GetVoidType()));
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (globalScope));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    
    boost::shared_ptr<TestSemanticsHandler::TestBlockImpl> testBlockImpl (
      boost::static_pointer_cast<TestSemanticsHandler::TestBlockImpl> (block));
      
    s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
    bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
    TS_ASSERT_EQUALS (seqChanged, true);
    
    s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
    TestSequenceVisitor testVisitor;
    newSeq->Visit (testVisitor);
    TS_ASSERT_EQUALS (testVisitor.entries.size(), 4);
    if (testVisitor.entries.size() < 4) return;
    // 'c' assignment
    TS_ASSERT_EQUALS (testVisitor.entries[0].op, TestSequenceVisitor::opConstBool);
    // Optimized: nested block instead of branch op
    TS_ASSERT_EQUALS (testVisitor.entries[1].op, TestSequenceVisitor::opBlock);
    // 'z' assignment
    TS_ASSERT_EQUALS (testVisitor.entries[2].op, TestSequenceVisitor::opConstUInt);
    // Optimized: constant instead of multiplication
    TS_ASSERT_EQUALS (testVisitor.entries[3].op, TestSequenceVisitor::opConstUInt);
    TS_ASSERT_EQUALS (testVisitor.entries[3].uintConst, 1);
  }

  void testWhileNonConst (void)
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
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    // global scope is required so BlockImpl can create some unique var names
    SemanticsHandler::ScopePtr globalScope (semanticsHandler.CreateScope (SemanticsHandler::ScopePtr(),
					    TestSemanticsHandler::Global,
					    semanticsHandler.GetVoidType()));
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (globalScope));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    
    boost::shared_ptr<TestSemanticsHandler::TestBlockImpl> testBlockImpl (
      boost::static_pointer_cast<TestSemanticsHandler::TestBlockImpl> (block));
      
    s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
    bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
    TS_ASSERT_EQUALS (seqChanged, true); /* seq is still changed b/c initial 'x < 5' is optimized */
    
    s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
    TestSequenceVisitor testVisitor;
    newSeq->Visit (testVisitor);
    TS_ASSERT_EQUALS (testVisitor.entries.size(), 6);
    if (testVisitor.entries.size() < 6) return;
    // 'x' assignment
    TS_ASSERT_EQUALS (testVisitor.entries[0].op, TestSequenceVisitor::opConstUInt);
    // 'x < 5': right side for comparison
    TS_ASSERT_EQUALS (testVisitor.entries[1].op, TestSequenceVisitor::opConstUInt);
    // 'x < 5': optimized comparison
    TS_ASSERT_EQUALS (testVisitor.entries[2].op, TestSequenceVisitor::opConstBool);
    // Loop
    TS_ASSERT_EQUALS (testVisitor.entries[3].op, TestSequenceVisitor::opWhile);
    // 'z' assignment
    TS_ASSERT_EQUALS (testVisitor.entries[4].op, TestSequenceVisitor::opConstUInt);
    // 'y' assignment
    TS_ASSERT_EQUALS (testVisitor.entries[5].op, TestSequenceVisitor::opArithMul);
  }

  void testWhileConst (void)
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
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    // global scope is required so BlockImpl can create some unique var names
    SemanticsHandler::ScopePtr globalScope (semanticsHandler.CreateScope (SemanticsHandler::ScopePtr(),
					    TestSemanticsHandler::Global,
					    semanticsHandler.GetVoidType()));
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (globalScope));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    
    boost::shared_ptr<TestSemanticsHandler::TestBlockImpl> testBlockImpl (
      boost::static_pointer_cast<TestSemanticsHandler::TestBlockImpl> (block));
      
    s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
    bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
    TS_ASSERT_EQUALS (seqChanged, true);
    
    s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
    TestSequenceVisitor testVisitor;
    newSeq->Visit (testVisitor);
    TS_ASSERT_EQUALS (testVisitor.entries.size(), 5);
    if (testVisitor.entries.size() < 5) return;
    // 'x' assignment
    TS_ASSERT_EQUALS (testVisitor.entries[0].op, TestSequenceVisitor::opConstUInt);
    // 'x > 0': right side for comparison
    TS_ASSERT_EQUALS (testVisitor.entries[1].op, TestSequenceVisitor::opConstUInt);
    // 'x > 0': optimized comparison
    TS_ASSERT_EQUALS (testVisitor.entries[2].op, TestSequenceVisitor::opConstBool);
    // Optimized: no loop
    // 'z' assignment
    TS_ASSERT_EQUALS (testVisitor.entries[3].op, TestSequenceVisitor::opConstUInt);
    // Optimized: constant instead of multiplication
    TS_ASSERT_EQUALS (testVisitor.entries[4].op, TestSequenceVisitor::opConstUInt);
    TS_ASSERT_EQUALS (testVisitor.entries[4].uintConst, 0);
  }

  void testBuiltinDot (void)
  {
    static const char blockSource[] =
      "float2 x = float2 (3.0, 0.5);"
      "float2 y = float2 (2.0, 0.25);"
      "float a; a = dot (x, y);"
      ;
    
    using namespace s1::parser;
    
    s1::uc::SimpleBufferStreamSource in (blockSource, strlen (blockSource));
    s1::uc::Stream ustream (in);
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    // global scope is required so BlockImpl can create some unique var names
    SemanticsHandler::ScopePtr builtinScope (semanticsHandler.CreateScope (SemanticsHandler::ScopePtr(),
									   SemanticsHandler::Builtin,
									   semanticsHandler.GetVoidType()));
    SemanticsHandler::ScopePtr globalScope (semanticsHandler.CreateScope (builtinScope,
									  TestSemanticsHandler::Global,
									  semanticsHandler.GetVoidType()));
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (globalScope));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    
    boost::shared_ptr<TestSemanticsHandler::TestBlockImpl> testBlockImpl (
      boost::static_pointer_cast<TestSemanticsHandler::TestBlockImpl> (block));
      
    s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
    bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
    TS_ASSERT_EQUALS (seqChanged, true);
    
    s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
    TestSequenceVisitor testVisitor;
    newSeq->Visit (testVisitor);
    TS_ASSERT_EQUALS (testVisitor.entries.size(), 7);
    if (testVisitor.entries.size() < 7) return;
    // Operand assignments
    TS_ASSERT_EQUALS (testVisitor.entries[0].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[1].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[2].op, TestSequenceVisitor::opMakeVectorFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[3].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[4].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[5].op, TestSequenceVisitor::opMakeVectorFloat);
    // Dot product - optimized: no builtin call op, const value instead
    TS_ASSERT_EQUALS (testVisitor.entries[6].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[6].floatConst, 6.125);
  }

  void testBuiltinCross (void)
  {
    static const char blockSource[] =
      "float3 x = float3 (1.0, 2.0, 3.0);"
      "float3 y = float3 (4.0, 5.0, 6.0);"
      "float3 a; a = cross (x, y);"
      ;
    
    using namespace s1::parser;
    
    s1::uc::SimpleBufferStreamSource in (blockSource, strlen (blockSource));
    s1::uc::Stream ustream (in);
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    // global scope is required so BlockImpl can create some unique var names
    SemanticsHandler::ScopePtr builtinScope (semanticsHandler.CreateScope (SemanticsHandler::ScopePtr(),
									   SemanticsHandler::Builtin,
									   semanticsHandler.GetVoidType()));
    SemanticsHandler::ScopePtr globalScope (semanticsHandler.CreateScope (builtinScope,
									  TestSemanticsHandler::Global,
									  semanticsHandler.GetVoidType()));
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (globalScope));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    
    boost::shared_ptr<TestSemanticsHandler::TestBlockImpl> testBlockImpl (
      boost::static_pointer_cast<TestSemanticsHandler::TestBlockImpl> (block));
      
    s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
    bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
    TS_ASSERT_EQUALS (seqChanged, true);
    
    s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
    TestSequenceVisitor testVisitor;
    newSeq->Visit (testVisitor);
    TS_ASSERT_EQUALS (testVisitor.entries.size(), 12);
    if (testVisitor.entries.size() < 12) return;
    // Operand assignments
    TS_ASSERT_EQUALS (testVisitor.entries[0].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[1].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[2].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[3].op, TestSequenceVisitor::opMakeVectorFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[4].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[5].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[6].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[7].op, TestSequenceVisitor::opMakeVectorFloat);
    // Cross product - optimized: no builtin call op, const values instead
    TS_ASSERT_EQUALS (testVisitor.entries[8].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[8].floatConst, -3);
    TS_ASSERT_EQUALS (testVisitor.entries[9].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[9].floatConst, 6);
    TS_ASSERT_EQUALS (testVisitor.entries[10].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[10].floatConst, -3);
    TS_ASSERT_EQUALS (testVisitor.entries[11].op, TestSequenceVisitor::opMakeVectorFloat);
  }
  
  void testBuiltinNormalize (void)
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
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    // global scope is required so BlockImpl can create some unique var names
    SemanticsHandler::ScopePtr builtinScope (semanticsHandler.CreateScope (SemanticsHandler::ScopePtr(),
									   SemanticsHandler::Builtin,
									   semanticsHandler.GetVoidType()));
    SemanticsHandler::ScopePtr globalScope (semanticsHandler.CreateScope (builtinScope,
									  TestSemanticsHandler::Global,
									  semanticsHandler.GetVoidType()));
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (globalScope));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    
    boost::shared_ptr<TestSemanticsHandler::TestBlockImpl> testBlockImpl (
      boost::static_pointer_cast<TestSemanticsHandler::TestBlockImpl> (block));
      
    s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
    bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
    TS_ASSERT_EQUALS (seqChanged, true);
    
    s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
    TestSequenceVisitor testVisitor;
    newSeq->Visit (testVisitor);
    TS_ASSERT_EQUALS (testVisitor.entries.size(), 16);
    if (testVisitor.entries.size() < 16) return;
    // Operand assignments
    TS_ASSERT_EQUALS (testVisitor.entries[0].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[1].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[2].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[3].op, TestSequenceVisitor::opMakeVectorFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[4].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[5].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[6].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[7].op, TestSequenceVisitor::opMakeVectorFloat);
    // Normalization - optimized: no builtin call op, const values instead
    TS_ASSERT_EQUALS (testVisitor.entries[8].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[8].floatConst, 1);
    TS_ASSERT_EQUALS (testVisitor.entries[9].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[9].floatConst, 0);
    TS_ASSERT_EQUALS (testVisitor.entries[10].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[10].floatConst, 0);
    TS_ASSERT_EQUALS (testVisitor.entries[11].op, TestSequenceVisitor::opMakeVectorFloat);
    // Normalization - optimized: no builtin call op, const values instead
    TS_ASSERT_EQUALS (testVisitor.entries[12].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[12].floatConst, 0);
    TS_ASSERT_EQUALS (testVisitor.entries[13].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_DELTA  (testVisitor.entries[13].floatConst, M_SQRT1_2, 0.00001);
    TS_ASSERT_EQUALS (testVisitor.entries[14].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_DELTA  (testVisitor.entries[14].floatConst, M_SQRT1_2, 0.00001);
    TS_ASSERT_EQUALS (testVisitor.entries[15].op, TestSequenceVisitor::opMakeVectorFloat);
  }
  
  void testBuiltinLength (void)
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
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    // global scope is required so BlockImpl can create some unique var names
    SemanticsHandler::ScopePtr builtinScope (semanticsHandler.CreateScope (SemanticsHandler::ScopePtr(),
									   SemanticsHandler::Builtin,
									   semanticsHandler.GetVoidType()));
    SemanticsHandler::ScopePtr globalScope (semanticsHandler.CreateScope (builtinScope,
									  TestSemanticsHandler::Global,
									  semanticsHandler.GetVoidType()));
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (globalScope));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    
    boost::shared_ptr<TestSemanticsHandler::TestBlockImpl> testBlockImpl (
      boost::static_pointer_cast<TestSemanticsHandler::TestBlockImpl> (block));
      
    s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
    bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
    TS_ASSERT_EQUALS (seqChanged, true);
    
    s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
    TestSequenceVisitor testVisitor;
    newSeq->Visit (testVisitor);
    TS_ASSERT_EQUALS (testVisitor.entries.size(), 10);
    if (testVisitor.entries.size() < 10) return;
    // Operand assignments
    TS_ASSERT_EQUALS (testVisitor.entries[0].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[1].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[2].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[3].op, TestSequenceVisitor::opMakeVectorFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[4].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[5].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[6].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[7].op, TestSequenceVisitor::opMakeVectorFloat);
    // Length - optimized: no builtin call op, const value instead
    TS_ASSERT_EQUALS (testVisitor.entries[8].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[8].floatConst, 2);
    // Length - optimized: no builtin call op, const value instead
    TS_ASSERT_EQUALS (testVisitor.entries[9].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_DELTA  (testVisitor.entries[9].floatConst, M_SQRT2, 0.00001);
  }

  void testBuiltinMulMatrixMatrix (void)
  {
    static const char blockSource[] =
      "float2x2 x = float2x2 (2.0, 0.0, 0.0, 0.5);"
      "float2x2 y = float2x2 (0.0, 1.0, 1.0, 0.0);"
      "float2x2 a; a = mul (x, y);"
      ;
    
    using namespace s1::parser;
    
    s1::uc::SimpleBufferStreamSource in (blockSource, strlen (blockSource));
    s1::uc::Stream ustream (in);
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    // global scope is required so BlockImpl can create some unique var names
    SemanticsHandler::ScopePtr builtinScope (semanticsHandler.CreateScope (SemanticsHandler::ScopePtr(),
									   SemanticsHandler::Builtin,
									   semanticsHandler.GetVoidType()));
    SemanticsHandler::ScopePtr globalScope (semanticsHandler.CreateScope (builtinScope,
									  TestSemanticsHandler::Global,
									  semanticsHandler.GetVoidType()));
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (globalScope));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    
    boost::shared_ptr<TestSemanticsHandler::TestBlockImpl> testBlockImpl (
      boost::static_pointer_cast<TestSemanticsHandler::TestBlockImpl> (block));
      
    s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
    bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
    TS_ASSERT_EQUALS (seqChanged, true);
    
    s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
    TestSequenceVisitor testVisitor;
    newSeq->Visit (testVisitor);
    TS_ASSERT_EQUALS (testVisitor.entries.size(), 15);
    if (testVisitor.entries.size() < 15) return;
    // Operand assignments
    TS_ASSERT_EQUALS (testVisitor.entries[0].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[1].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[2].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[3].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[4].op, TestSequenceVisitor::opMakeMatrixFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[5].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[6].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[7].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[8].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[9].op, TestSequenceVisitor::opMakeMatrixFloat);
    // Matrix mul - optimized: no builtin call op, const values instead
    TS_ASSERT_EQUALS (testVisitor.entries[10].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[10].floatConst, 0);
    TS_ASSERT_EQUALS (testVisitor.entries[11].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[11].floatConst, 2);
    TS_ASSERT_EQUALS (testVisitor.entries[12].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[12].floatConst, 0.5);
    TS_ASSERT_EQUALS (testVisitor.entries[13].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[13].floatConst, 0);
    TS_ASSERT_EQUALS (testVisitor.entries[14].op, TestSequenceVisitor::opMakeMatrixFloat);
  }

  void testBuiltinMulMatrixVector (void)
  {
    static const char blockSource[] =
      "float2x2 x = float2x2 (2.0, 0.0, 0.0, 0.5);"
      "float2 y = float2 (2.0, 3.0);"
      "float2 a; a = mul (x, y);"
      ;
    
    using namespace s1::parser;
    
    s1::uc::SimpleBufferStreamSource in (blockSource, strlen (blockSource));
    s1::uc::Stream ustream (in);
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    // global scope is required so BlockImpl can create some unique var names
    SemanticsHandler::ScopePtr builtinScope (semanticsHandler.CreateScope (SemanticsHandler::ScopePtr(),
									   SemanticsHandler::Builtin,
									   semanticsHandler.GetVoidType()));
    SemanticsHandler::ScopePtr globalScope (semanticsHandler.CreateScope (builtinScope,
									  TestSemanticsHandler::Global,
									  semanticsHandler.GetVoidType()));
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (globalScope));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    
    boost::shared_ptr<TestSemanticsHandler::TestBlockImpl> testBlockImpl (
      boost::static_pointer_cast<TestSemanticsHandler::TestBlockImpl> (block));
      
    s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
    bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
    TS_ASSERT_EQUALS (seqChanged, true);
    
    s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
    TestSequenceVisitor testVisitor;
    newSeq->Visit (testVisitor);
    TS_ASSERT_EQUALS (testVisitor.entries.size(), 11);
    if (testVisitor.entries.size() < 11) return;
    // Operand assignments
    TS_ASSERT_EQUALS (testVisitor.entries[0].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[1].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[2].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[3].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[4].op, TestSequenceVisitor::opMakeMatrixFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[5].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[6].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[7].op, TestSequenceVisitor::opMakeVectorFloat);
    // Matrix/Vector mul - optimized: no builtin call op, const values instead
    TS_ASSERT_EQUALS (testVisitor.entries[8].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[8].floatConst, 4);
    TS_ASSERT_EQUALS (testVisitor.entries[9].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[9].floatConst, 1.5);
    TS_ASSERT_EQUALS (testVisitor.entries[10].op, TestSequenceVisitor::opMakeVectorFloat);
  }

  void testBuiltinMulVectorMatrix (void)
  {
    static const char blockSource[] =
      "float2 x = float2 (2.0, 3.0);"
      "float2x2 y = float2x2 (0.0, 1.0, 1.0, 0.0);"
      "float2 a; a = mul (x, y);"
      ;
    
    using namespace s1::parser;
    
    s1::uc::SimpleBufferStreamSource in (blockSource, strlen (blockSource));
    s1::uc::Stream ustream (in);
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    // global scope is required so BlockImpl can create some unique var names
    SemanticsHandler::ScopePtr builtinScope (semanticsHandler.CreateScope (SemanticsHandler::ScopePtr(),
									   SemanticsHandler::Builtin,
									   semanticsHandler.GetVoidType()));
    SemanticsHandler::ScopePtr globalScope (semanticsHandler.CreateScope (builtinScope,
									  TestSemanticsHandler::Global,
									  semanticsHandler.GetVoidType()));
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (globalScope));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    
    boost::shared_ptr<TestSemanticsHandler::TestBlockImpl> testBlockImpl (
      boost::static_pointer_cast<TestSemanticsHandler::TestBlockImpl> (block));
      
    s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
    bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
    TS_ASSERT_EQUALS (seqChanged, true);
    
    s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
    TestSequenceVisitor testVisitor;
    newSeq->Visit (testVisitor);
    TS_ASSERT_EQUALS (testVisitor.entries.size(), 11);
    if (testVisitor.entries.size() < 11) return;
    // Operand assignments
    TS_ASSERT_EQUALS (testVisitor.entries[0].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[1].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[2].op, TestSequenceVisitor::opMakeVectorFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[3].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[4].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[5].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[6].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[7].op, TestSequenceVisitor::opMakeMatrixFloat);
    // Vector/Matrix mul - optimized: no builtin call op, const values instead
    TS_ASSERT_EQUALS (testVisitor.entries[8].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[8].floatConst, 3);
    TS_ASSERT_EQUALS (testVisitor.entries[9].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[9].floatConst, 2);
    TS_ASSERT_EQUALS (testVisitor.entries[10].op, TestSequenceVisitor::opMakeVectorFloat);
  }

  void testMin (void)
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
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    // global scope is required so BlockImpl can create some unique var names
    SemanticsHandler::ScopePtr builtinScope (semanticsHandler.CreateScope (SemanticsHandler::ScopePtr(),
									   SemanticsHandler::Builtin,
									   semanticsHandler.GetVoidType()));
    SemanticsHandler::ScopePtr globalScope (semanticsHandler.CreateScope (builtinScope,
									  TestSemanticsHandler::Global,
									  semanticsHandler.GetVoidType()));
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (globalScope));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    
    boost::shared_ptr<TestSemanticsHandler::TestBlockImpl> testBlockImpl (
      boost::static_pointer_cast<TestSemanticsHandler::TestBlockImpl> (block));
      
    s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
    bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
    TS_ASSERT_EQUALS (seqChanged, true);
    
    s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
    TestSequenceVisitor testVisitor;
    newSeq->Visit (testVisitor);
    TS_ASSERT_EQUALS (testVisitor.entries.size(), 13);
    if (testVisitor.entries.size() < 13) return;
    // 'ux', 'uy' initialization
    TS_ASSERT_EQUALS (testVisitor.entries[0].op, TestSequenceVisitor::opConstUInt);
    TS_ASSERT_EQUALS (testVisitor.entries[1].op, TestSequenceVisitor::opConstUInt);
    // 'ix', 'iy' initialization (generated ops are 'ConstUInt', 'Neg', 'Cast', here opt. to consts)
    TS_ASSERT_EQUALS (testVisitor.entries[2].op, TestSequenceVisitor::opConstUInt);
    TS_ASSERT_EQUALS (testVisitor.entries[3].op, TestSequenceVisitor::opConstInt);
    TS_ASSERT_EQUALS (testVisitor.entries[4].op, TestSequenceVisitor::opConstInt);
    TS_ASSERT_EQUALS (testVisitor.entries[5].op, TestSequenceVisitor::opConstUInt);
    TS_ASSERT_EQUALS (testVisitor.entries[6].op, TestSequenceVisitor::opConstInt);
    TS_ASSERT_EQUALS (testVisitor.entries[7].op, TestSequenceVisitor::opConstInt);
    // 'fx', 'fy' initialization
    TS_ASSERT_EQUALS (testVisitor.entries[8].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[9].op, TestSequenceVisitor::opConstFloat);
    // Min - optimized: no builtin call op, const values instead
    TS_ASSERT_EQUALS (testVisitor.entries[10].op, TestSequenceVisitor::opConstUInt);
    TS_ASSERT_EQUALS (testVisitor.entries[10].uintConst, 2);
    // Min - optimized: no builtin call op, const values instead
    TS_ASSERT_EQUALS (testVisitor.entries[11].op, TestSequenceVisitor::opConstInt);
    TS_ASSERT_EQUALS (testVisitor.entries[11].intConst, -3);
    // Min - optimized: no builtin call op, const values instead
    TS_ASSERT_EQUALS (testVisitor.entries[12].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[12].floatConst, 2);
  }

  void testMax (void)
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
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    // global scope is required so BlockImpl can create some unique var names
    SemanticsHandler::ScopePtr builtinScope (semanticsHandler.CreateScope (SemanticsHandler::ScopePtr(),
									   SemanticsHandler::Builtin,
									   semanticsHandler.GetVoidType()));
    SemanticsHandler::ScopePtr globalScope (semanticsHandler.CreateScope (builtinScope,
									  TestSemanticsHandler::Global,
									  semanticsHandler.GetVoidType()));
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (globalScope));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    
    boost::shared_ptr<TestSemanticsHandler::TestBlockImpl> testBlockImpl (
      boost::static_pointer_cast<TestSemanticsHandler::TestBlockImpl> (block));
      
    s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
    bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
    TS_ASSERT_EQUALS (seqChanged, true);
    
    s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
    TestSequenceVisitor testVisitor;
    newSeq->Visit (testVisitor);
    TS_ASSERT_EQUALS (testVisitor.entries.size(), 13);
    if (testVisitor.entries.size() < 13) return;
    // 'ux', 'uy' initialization
    TS_ASSERT_EQUALS (testVisitor.entries[0].op, TestSequenceVisitor::opConstUInt);
    TS_ASSERT_EQUALS (testVisitor.entries[1].op, TestSequenceVisitor::opConstUInt);
    // 'ix', 'iy' initialization (generated ops are 'ConstUInt', 'Neg', 'Cast', here opt. to consts)
    TS_ASSERT_EQUALS (testVisitor.entries[2].op, TestSequenceVisitor::opConstUInt);
    TS_ASSERT_EQUALS (testVisitor.entries[3].op, TestSequenceVisitor::opConstInt);
    TS_ASSERT_EQUALS (testVisitor.entries[4].op, TestSequenceVisitor::opConstInt);
    TS_ASSERT_EQUALS (testVisitor.entries[5].op, TestSequenceVisitor::opConstUInt);
    TS_ASSERT_EQUALS (testVisitor.entries[6].op, TestSequenceVisitor::opConstInt);
    TS_ASSERT_EQUALS (testVisitor.entries[7].op, TestSequenceVisitor::opConstInt);
    // 'fx', 'fy' initialization
    TS_ASSERT_EQUALS (testVisitor.entries[8].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[9].op, TestSequenceVisitor::opConstFloat);
    // Max - optimized: no builtin call op, const values instead
    TS_ASSERT_EQUALS (testVisitor.entries[10].op, TestSequenceVisitor::opConstUInt);
    TS_ASSERT_EQUALS (testVisitor.entries[10].uintConst, 3);
    // Max - optimized: no builtin call op, const values instead
    TS_ASSERT_EQUALS (testVisitor.entries[11].op, TestSequenceVisitor::opConstInt);
    TS_ASSERT_EQUALS (testVisitor.entries[11].intConst, -2);
    // Max - optimized: no builtin call op, const values instead
    TS_ASSERT_EQUALS (testVisitor.entries[12].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[12].floatConst, 3);
  }

  void testPow (void)
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
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    // global scope is required so BlockImpl can create some unique var names
    SemanticsHandler::ScopePtr builtinScope (semanticsHandler.CreateScope (SemanticsHandler::ScopePtr(),
									   SemanticsHandler::Builtin,
									   semanticsHandler.GetVoidType()));
    SemanticsHandler::ScopePtr globalScope (semanticsHandler.CreateScope (builtinScope,
									  TestSemanticsHandler::Global,
									  semanticsHandler.GetVoidType()));
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (globalScope));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    
    boost::shared_ptr<TestSemanticsHandler::TestBlockImpl> testBlockImpl (
      boost::static_pointer_cast<TestSemanticsHandler::TestBlockImpl> (block));
      
    s1::intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<s1::intermediate::SequenceBuilder> ());
    bool seqChanged = s1::optimize::ConstantFolding::FoldConstants (newSeqBuilder, testBlockImpl->GetSequence());
    TS_ASSERT_EQUALS (seqChanged, true);
    
    s1::intermediate::SequencePtr newSeq (newSeqBuilder->GetSequence());
    TestSequenceVisitor testVisitor;
    newSeq->Visit (testVisitor);
    TS_ASSERT_EQUALS (testVisitor.entries.size(), 8);
    if (testVisitor.entries.size() < 8) return;
    // 'x1', 'y1' initialization
    TS_ASSERT_EQUALS (testVisitor.entries[0].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[1].op, TestSequenceVisitor::opConstFloat);
    // 'x1', 'y1' initialization (generated ops are 'ConstFloat', 'Neg' here opt. to consts)
    TS_ASSERT_EQUALS (testVisitor.entries[2].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[3].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[4].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[5].op, TestSequenceVisitor::opConstFloat);
    // Pow - optimized: no builtin call op, const values instead
    TS_ASSERT_EQUALS (testVisitor.entries[6].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[6].floatConst, 8);
    // Pow - optimized: no builtin call op, const values instead
    TS_ASSERT_EQUALS (testVisitor.entries[7].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS (testVisitor.entries[7].floatConst, -0.125);
  }

};
