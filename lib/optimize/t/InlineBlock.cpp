#include <cxxtest/TestSuite.h>
#include <boost/cstdint.hpp>
#include <boost/make_shared.hpp>

#include "intermediate/IntermediateGeneratorSemanticsHandler.h"
#include "lexer/LexerErrorHandler.h"
#include "parser/Parser.h"
#include "../../intermediate/BlockImpl.h"
#include "../../intermediate/TypeImpl.h"

#include "optimize/Inliner.h"
#include "../../intermediate/t/TestSequenceVisitor.h"
#include "../../intermediate/t/IntermediateTestTraits.h"

using namespace s1::intermediate;
using namespace s1::optimize;

class InlineBlockTestSuite : public CxxTest::TestSuite 
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
    
      using Superclass::sequence;
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
    
    std::istringstream in (blockSource);
    s1::UnicodeStream ustream (in, "utf-8");
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
      boost::shared_static_cast<TestSemanticsHandler::TestBlockImpl> (block));
      
    s1::intermediate::SequencePtr newSeq (boost::make_shared<s1::intermediate::Sequence> ());
    bool blocksInlined = s1::optimize::Inliner::InlineAllBlocks (newSeq, testBlockImpl->GetSequence());
    TS_ASSERT_EQUALS (blocksInlined, true);
    
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
    
    std::istringstream in (blockSource);
    s1::UnicodeStream ustream (in, "utf-8");
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
      boost::shared_static_cast<TestSemanticsHandler::TestBlockImpl> (block));
      
    s1::intermediate::SequencePtr newSeq (boost::make_shared<s1::intermediate::Sequence> ());
    bool blocksInlined = s1::optimize::Inliner::InlineAllBlocks (newSeq, testBlockImpl->GetSequence());
    TS_ASSERT_EQUALS (blocksInlined, true);
    
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
    
    std::istringstream in (blockSource);
    s1::UnicodeStream ustream (in, "utf-8");
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
      boost::shared_static_cast<TestSemanticsHandler::TestBlockImpl> (block));
      
    s1::intermediate::SequencePtr newSeq (boost::make_shared<s1::intermediate::Sequence> ());
    bool blocksInlined = s1::optimize::Inliner::InlineAllBlocks (newSeq, testBlockImpl->GetSequence());
    TS_ASSERT_EQUALS (blocksInlined, false);
    
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
    
    std::istringstream in (blockSource);
    s1::UnicodeStream ustream (in, "utf-8");
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
      boost::shared_static_cast<TestSemanticsHandler::TestBlockImpl> (block));
      
    s1::intermediate::SequencePtr newSeq (boost::make_shared<s1::intermediate::Sequence> ());
    bool blocksInlined = s1::optimize::Inliner::InlineAllBlocks (newSeq, testBlockImpl->GetSequence());
    TS_ASSERT_EQUALS (blocksInlined, false);
    
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
    
    std::istringstream in (blockSource);
    s1::UnicodeStream ustream (in, "utf-8");
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
      boost::shared_static_cast<TestSemanticsHandler::TestBlockImpl> (block));
      
    s1::intermediate::SequencePtr newSeq (boost::make_shared<s1::intermediate::Sequence> ());
    bool blocksInlined = s1::optimize::Inliner::InlineAllBlocks (newSeq, testBlockImpl->GetSequence());
    TS_ASSERT_EQUALS (blocksInlined, true);
    
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
    
    std::istringstream in (blockSource);
    s1::UnicodeStream ustream (in, "utf-8");
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
      boost::shared_static_cast<TestSemanticsHandler::TestBlockImpl> (block));
      
    s1::intermediate::SequencePtr newSeq (boost::make_shared<s1::intermediate::Sequence> ());
    bool blocksInlined = s1::optimize::Inliner::InlineAllBlocks (newSeq, testBlockImpl->GetSequence());
    TS_ASSERT_EQUALS (blocksInlined, true);
    
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
