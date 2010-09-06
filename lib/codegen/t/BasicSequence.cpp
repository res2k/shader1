#include <cxxtest/TestSuite.h>
#include <boost/cstdint.hpp>

#include "intermediate/IntermediateGeneratorSemanticsHandler.h"
// FIXME: Tests using too much intermediate internals
#include "../../intermediate/BlockImpl.h"
#include "../../intermediate/NameImpl.h"

#include "codegen/CgGenerator.h"
#include "../cg/SequenceCodeGenerator.h"

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
  
  class TestImportedNameResolver : public ImportedNameResolver
  {
  public:
    std::string GetImportedNameIdentifier (const UnicodeString& name)
    { return std::string(); }	
    std::string GetExportedNameIdentifier (const UnicodeString& name)
    { return std::string(); }	
  };
  
  class TestCodeGenerator : public CgGenerator
  {
  public:
    typedef CgGenerator Superclass;
    
    class TestSequenceCodeGenerator : public SequenceCodeGenerator
    {
    public:
      typedef SequenceCodeGenerator Superclass;
    
      TestSequenceCodeGenerator (const Sequence& seq, ImportedNameResolver* nameRes)
       : SequenceCodeGenerator (seq, nameRes,
				ProgramFunction::TransferMappings(),
				ProgramFunction::TransferMappings()) {}
       
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
    boost::shared_ptr<TestSemanticsHandler::TestNameImpl> testVarA =
      boost::shared_static_cast<TestSemanticsHandler::TestNameImpl> (varA);
      
    TestImportedNameResolver nameRes;
    TestCodeGenerator::TestSequenceCodeGenerator seqGen (*(testBlockImpl->sequence), &nameRes);
    StringsArrayPtr generateResult (seqGen.Generate ());
    
    std::string resultRegName (seqGen.GetOutputRegisterName (
      testBlockImpl->GetRegisterForName (testVarA, false)));
    StringStringMap substMap;
    substMap["R0"] = resultRegName;
    TS_ASSERT_EQUALS(generateResult->Get (0),
		     StringSubstitute ("float $R0;", substMap));
    TS_ASSERT_EQUALS(generateResult->Get (1),
		     StringSubstitute ("$R0 = 1;", substMap));
  }
  
  void testExprArithVar (void)
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
    
    TestSemanticsHandler::TestBlockImpl* testBlockImpl =
      static_cast<TestSemanticsHandler::TestBlockImpl*> (testBlock.get());
    boost::shared_ptr<TestSemanticsHandler::TestNameImpl> testVarA =
      boost::shared_static_cast<TestSemanticsHandler::TestNameImpl> (varA);
    boost::shared_ptr<TestSemanticsHandler::TestNameImpl> testVarB =
      boost::shared_static_cast<TestSemanticsHandler::TestNameImpl> (varB);
    boost::shared_ptr<TestSemanticsHandler::TestNameImpl> testVarC =
      boost::shared_static_cast<TestSemanticsHandler::TestNameImpl> (varC);
      
    TestImportedNameResolver nameRes;
    TestCodeGenerator::TestSequenceCodeGenerator seqGen (*(testBlockImpl->sequence), &nameRes);
    StringsArrayPtr generateResult (seqGen.Generate ());
    
    std::string varARegName (seqGen.GetOutputRegisterName (
      testBlockImpl->GetRegisterForName (testVarA, false)));
    std::string varBRegName (seqGen.GetOutputRegisterName (
      testBlockImpl->GetRegisterForName (testVarB, false)));
    std::string varCRegName (seqGen.GetOutputRegisterName (
      testBlockImpl->GetRegisterForName (testVarC, false)));
    StringStringMap substMap;
    substMap["A"] = varARegName;
    substMap["B"] = varBRegName;
    substMap["C"] = varCRegName;
    int l = 0;
    TS_ASSERT_EQUALS(generateResult->Size(), 4);
    TS_ASSERT_EQUALS(generateResult->Get (l++),
		     StringSubstitute ("float $A;", substMap));
    TS_ASSERT_EQUALS(generateResult->Get (l++),
		     StringSubstitute ("float $B;", substMap));
    TS_ASSERT_EQUALS(generateResult->Get (l++),
		     StringSubstitute ("float $C;", substMap));
    TS_ASSERT_EQUALS(generateResult->Get (l++),
		     StringSubstitute ("$C = $A + $B;", substMap));
  }
  
};
