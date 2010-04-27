#include <cxxtest/TestSuite.h>

#include "intermediate/IntermediateGeneratorSemanticsHandler.h"
#include "../BlockImpl.h"
#include "TestSequenceVisitor.h"

#include "IntermediateTestTraits.h"

using namespace s1::intermediate;

class IntermediateExprTestSuite : public CxxTest::TestSuite 
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
  void testExprAssignConst (void)
  {
    TestSemanticsHandler semanticsHandler;
    
    // Create a scope
    TestSemanticsHandler::ScopePtr testScope = semanticsHandler.CreateScope (
      TestSemanticsHandler::ScopePtr (), TestSemanticsHandler::Global);
    // Add some variables
    TestSemanticsHandler::TypePtr floatType = semanticsHandler.CreateType (TestSemanticsHandler::Float);
    TestSemanticsHandler::NamePtr varA = testScope->AddVariable (floatType, UnicodeString ("a"),
								 TestSemanticsHandler::ExpressionPtr (),
								 false);
    // Create a simple expression "a = 1"
    TestSemanticsHandler::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
    TestSemanticsHandler::ExpressionPtr expr1 = semanticsHandler.CreateConstNumericExpression (UnicodeString ("1.0"));
    TestSemanticsHandler::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprA, expr1);
    // Add to a block
    TestSemanticsHandler::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
    testBlock->AddExpressionCommand (assignExpr);
    
    TestSemanticsHandler::TestBlockImpl* testBlockImpl =
      static_cast<TestSemanticsHandler::TestBlockImpl*> (testBlock.get());
    TestSequenceVisitor visitor;
    testBlockImpl->sequence->Visit (visitor);
    TS_ASSERT_EQUALS(visitor.entries.size(), 1);
    TS_ASSERT_EQUALS(visitor.entries[0].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS(visitor.entries[0].floatConst, 1.0f);
  }
  
  void testExprAssignWithCast (void)
  {
    TestSemanticsHandler semanticsHandler;
    
    // Create a scope
    TestSemanticsHandler::ScopePtr testScope = semanticsHandler.CreateScope (
      TestSemanticsHandler::ScopePtr (), TestSemanticsHandler::Global);
    // Add some variables
    TestSemanticsHandler::TypePtr floatType = semanticsHandler.CreateType (TestSemanticsHandler::Float);
    TestSemanticsHandler::NamePtr varA = testScope->AddVariable (floatType, UnicodeString ("a"),
								 TestSemanticsHandler::ExpressionPtr (),
								 false);
    // Create a simple expression "a = 1"
    TestSemanticsHandler::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
    TestSemanticsHandler::ExpressionPtr expr1 = semanticsHandler.CreateConstNumericExpression (UnicodeString ("1"));
    TestSemanticsHandler::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprA, expr1);
    // Add to a block
    TestSemanticsHandler::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
    testBlock->AddExpressionCommand (assignExpr);
    
    TestSemanticsHandler::TestBlockImpl* testBlockImpl =
      static_cast<TestSemanticsHandler::TestBlockImpl*> (testBlock.get());
    TestSequenceVisitor visitor;
    testBlockImpl->sequence->Visit (visitor);
    TS_ASSERT_EQUALS(visitor.entries.size(), 2);
    TS_ASSERT_EQUALS(visitor.entries[0].op, TestSequenceVisitor::opConstUInt);
    TS_ASSERT_EQUALS(visitor.entries[0].uintConst, 1);
    TS_ASSERT_EQUALS(visitor.entries[1].op, TestSequenceVisitor::opCastToFloat);
    TS_ASSERT_EQUALS(visitor.entries[1].sourceReg[0], visitor.entries[0].destReg);
  }
  
  void testExprAssignVar (void)
  {
    TestSemanticsHandler semanticsHandler;
    
    // Create a scope
    TestSemanticsHandler::ScopePtr testScope = semanticsHandler.CreateScope (
      TestSemanticsHandler::ScopePtr (), TestSemanticsHandler::Global);
    // Add some variables
    TestSemanticsHandler::TypePtr floatType = semanticsHandler.CreateType (TestSemanticsHandler::Float);
    TestSemanticsHandler::NamePtr varA = testScope->AddVariable (floatType, UnicodeString ("a"),
								 TestSemanticsHandler::ExpressionPtr (),
								 false);
    TestSemanticsHandler::NamePtr varB = testScope->AddVariable (floatType, UnicodeString ("b"),
								 TestSemanticsHandler::ExpressionPtr (),
								 false);
    // Create a simple expression "a = 1"
    TestSemanticsHandler::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
    TestSemanticsHandler::ExpressionPtr expr1 = semanticsHandler.CreateConstNumericExpression (UnicodeString ("1.0"));
    TestSemanticsHandler::ExpressionPtr exprB = semanticsHandler.CreateVariableExpression (varB);
    TestSemanticsHandler::ExpressionPtr assignExpr1 = semanticsHandler.CreateAssignExpression (exprA, expr1);
    TestSemanticsHandler::ExpressionPtr assignExpr2 = semanticsHandler.CreateAssignExpression (exprB, exprA);
    // Add to a block
    TestSemanticsHandler::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
    testBlock->AddExpressionCommand (assignExpr1);
    testBlock->AddExpressionCommand (assignExpr2);
    
    TestSemanticsHandler::TestBlockImpl* testBlockImpl =
      static_cast<TestSemanticsHandler::TestBlockImpl*> (testBlock.get());
    TestSequenceVisitor visitor;
    testBlockImpl->sequence->Visit (visitor);
    TS_ASSERT_EQUALS(visitor.entries.size(), 2);
    TS_ASSERT_EQUALS(visitor.entries[0].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS(visitor.entries[0].floatConst, 1.0f);
    TS_ASSERT_EQUALS(visitor.entries[1].op, TestSequenceVisitor::opAssignment);
    TS_ASSERT_EQUALS(visitor.entries[1].sourceReg[0], visitor.entries[0].destReg);
  }
  
  void testExprArith (void)
  {
    /*
    TestSemanticsHandler semanticsHandler;
    
    // Create a scope
    TestSemanticsHandler::ScopePtr testScope = semanticsHandler.CreateScope (
      TestSemanticsHandler::ScopePtr (), TestSemanticsHandler::Global);
    // Add some variables
    TestSemanticsHandler::TypePtr floatType = semanticsHandler.CreateType (TestSemanticsHandler::Float);
    TestSemanticsHandler::NamePtr varA = testScope->AddVariable (floatType, UnicodeString ("a"),
								 TestSemanticsHandler::ExpressionPtr (),
								 false);
    TestSemanticsHandler::NamePtr varB = testScope->AddVariable (floatType, UnicodeString ("b"),
								 TestSemanticsHandler::ExpressionPtr (),
								 false);
    TestSemanticsHandler::NamePtr varC = testScope->AddVariable (floatType, UnicodeString ("c"),
								 TestSemanticsHandler::ExpressionPtr (),
								 false);
    // Create a simple expression "c = a + b"
    TestSemanticsHandler::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
    TestSemanticsHandler::ExpressionPtr exprB = semanticsHandler.CreateVariableExpression (varB);
    TestSemanticsHandler::ExpressionPtr addExpr = semanticsHandler.CreateArithmeticExpression (TestSemanticsHandler::Add,
											       exprA, exprB);
    TestSemanticsHandler::ExpressionPtr exprC = semanticsHandler.CreateVariableExpression (varC);
    TestSemanticsHandler::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprC, addExpr);
    // Add to a block
    TestSemanticsHandler::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
    testBlock->AddExpressionCommand (assignExpr);
    */
  }
};
