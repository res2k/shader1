#include <cxxtest/TestSuite.h>
#include <boost/cstdint.hpp>

#include "intermediate/IntermediateGeneratorSemanticsHandler.h"
// FIXME: Tests using too much intermediate internals
#include "../../intermediate/BlockImpl.h"
#include "../../intermediate/NameImpl.h"

#include "codegen/CgGenerator.h"
#include "../SequenceCodeGenerator.h"

#include "StringSubstitute.h"

using namespace s1::intermediate;
using namespace s1::codegen;

class BasicSequenceTestSuite : public CxxTest::TestSuite 
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
    
    class TestNameImpl : public NameImpl
    {
    public:
      typedef NameImpl Superclass;
    };
  };
  
  class TestCodeGenerator : public CgGenerator
  {
  public:
    typedef CgGenerator Superclass;
    
    class TestSequenceCodeGenerator : public SequenceCodeGenerator
    {
    public:
      typedef SequenceCodeGenerator Superclass;
    
      TestSequenceCodeGenerator (const Sequence& seq)
       : SequenceCodeGenerator (seq) {}
       
      using Superclass::GetOutputRegisterName;
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
    TestSemanticsHandler::TestNameImpl* testVarA =
      static_cast<TestSemanticsHandler::TestNameImpl*> (varA.get());
      
    TestCodeGenerator::TestSequenceCodeGenerator seqGen (*(testBlockImpl->sequence));
    StringsArrayPtr generateResult (seqGen.Generate ());
    
    std::string resultRegName (seqGen.GetOutputRegisterName (testVarA->varReg, false));
    StringStringMap substMap;
    substMap["R0"] = resultRegName;
    TS_ASSERT_EQUALS(generateResult->Get (0),
		     StringSubstitute ("float $R0;", substMap));
    TS_ASSERT_EQUALS(generateResult->Get (1),
		     StringSubstitute ("$R0 = 1;", substMap));
  }
  
};
