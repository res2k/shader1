#include <cxxtest/TestSuite.h>
#include <boost/cstdint.hpp>
#include <boost/make_shared.hpp>

#include "intermediate/IntermediateGeneratorSemanticsHandler.h"
#include "lexer/LexerErrorHandler.h"
#include "parser/Parser.h"
#include "../../intermediate/BlockImpl.h"
#include "../../intermediate/TypeImpl.h"

#include "optimize/DeadCodeElimination.h"
#include "../../intermediate/t/TestSequenceVisitor.h"
#include "../../intermediate/t/IntermediateTestTraits.h"

using namespace s1::intermediate;
using namespace s1::optimize;

class DeadCodeEliminationTestSuite : public CxxTest::TestSuite 
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
  void testSimpleUnused (void)
  {
    static const char blockSource[] =
      "unsigned int a;"
      "a = 1;"
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
    s1::intermediate::RegisterSet usedRegs;
    bool codeEliminated = s1::optimize::DeadCodeElimination::EliminateDeadCode (newSeq, testBlockImpl->GetSequence(), usedRegs);
    TS_ASSERT_EQUALS (codeEliminated, true);
    
    TestSequenceVisitor testVisitor;
    newSeq->Visit (testVisitor);
    TS_ASSERT_EQUALS (testVisitor.entries.size(), 0);
  }
  
  void testSimpleUsed (void)
  {
    static const char blockSource[] =
      "unsigned int a;"
      "a = 1;"
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
    s1::intermediate::RegisterSet usedRegs;
    usedRegs.insert (testBlockImpl->GetSequence()->GetIdentifierRegister ("a"));
    bool codeEliminated = s1::optimize::DeadCodeElimination::EliminateDeadCode (newSeq, testBlockImpl->GetSequence(), usedRegs);
    TS_ASSERT_EQUALS (codeEliminated, true);
    
    TestSequenceVisitor testVisitor;
    newSeq->Visit (testVisitor);
    TS_ASSERT_EQUALS (testVisitor.entries.size(), 1);
    TS_ASSERT_EQUALS (testVisitor.entries[0].op, TestSequenceVisitor::opConstUInt);
  }
  
};