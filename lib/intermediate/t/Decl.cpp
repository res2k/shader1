#include <cxxtest/TestSuite.h>

#include "base/common.h"

#include "intermediate/IntermediateGeneratorSemanticsHandler.h"
#include "../BlockImpl.h"
#include "TestSequenceVisitor.h"

#include "IntermediateTestTraits.h"

using namespace s1::intermediate;

class IntermediateDeclTestSuite : public CxxTest::TestSuite 
{
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
  void testDeclVarInit (void)
  {
    TestSemanticsHandler semanticsHandler;
    
    // Create a scope
    TestSemanticsHandler::ScopePtr testScope = semanticsHandler.CreateScope (
      TestSemanticsHandler::ScopePtr (), TestSemanticsHandler::Global);
    // Create a block
    TestSemanticsHandler::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
    // Add some variables
    TestSemanticsHandler::TypePtr floatType = semanticsHandler.CreateType (TestSemanticsHandler::Float);
    TestSemanticsHandler::NamePtr varA = testBlock->GetInnerScope()->AddVariable (floatType, UnicodeString ("a"),
										  TestSemanticsHandler::ExpressionPtr (),
										  false);
    TestSemanticsHandler::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
    TestSemanticsHandler::NamePtr varB = testBlock->GetInnerScope()->AddVariable (floatType, UnicodeString ("b"),
										  exprA,
										  false);
    // No further ops - initializer on 'b' should be enough to emit an op ...
    
    TestSemanticsHandler::TestBlockImpl* testBlockImpl =
      static_cast<TestSemanticsHandler::TestBlockImpl*> (testBlock.get());
    testBlockImpl->FinishBlock(); // Needed for initializer emission
    
    TestSequenceVisitor visitor;
    testBlockImpl->sequence->Visit (visitor);
    TS_ASSERT_EQUALS(visitor.entries.size(), 1);
    if (visitor.entries.size() < 1) return;
    TS_ASSERT_EQUALS(visitor.entries[0].op, TestSequenceVisitor::opAssignment);
  }
  
  
};