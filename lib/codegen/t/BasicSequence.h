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
#include "base/common.h"

#include "intermediate/IntermediateGeneratorSemanticsHandler.h"
// FIXME: Tests using too much intermediate internals
#include "BlockImpl.h"
#include "NameImpl.h"

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
      static const ProgramFunction::TransferMappings& EmptyMappings()
      {
        static const ProgramFunction::TransferMappings m;
        return m;
      }
    public:
      typedef SequenceCodeGenerator Superclass;
    
      TestSequenceCodeGenerator (const Sequence& seq, ImportedNameResolver* nameRes)
       : SequenceCodeGenerator (seq, nameRes,
				EmptyMappings(), EmptyMappings(),
				std::vector<std::string> ()) {}
       
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
    TestSemanticsHandler::NamePtr varB = testScope->AddVariable (floatType, UnicodeString ("b"),
								 TestSemanticsHandler::ExpressionPtr (),
								 false);
    // Create a simple expression "a = 1"
    TestSemanticsHandler::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
    TestSemanticsHandler::ExpressionPtr expr1 = semanticsHandler.CreateConstNumericExpression (UnicodeString ("1.0"));
    TestSemanticsHandler::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprA, expr1);
    // Add to a block
    TestSemanticsHandler::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
    testBlock->AddExpressionCommand (assignExpr);
    // Create a simple expression "b = a"
    TestSemanticsHandler::ExpressionPtr exprB = semanticsHandler.CreateVariableExpression (varB);
    TestSemanticsHandler::ExpressionPtr assignExpr2 = semanticsHandler.CreateAssignExpression (exprB, exprA);
    // Add to block
    testBlock->AddExpressionCommand (assignExpr2);
    
    TestSemanticsHandler::TestBlockImpl* testBlockImpl =
      static_cast<TestSemanticsHandler::TestBlockImpl*> (testBlock.get());
    boost::shared_ptr<TestSemanticsHandler::TestNameImpl> testVarB =
      boost::static_pointer_cast<TestSemanticsHandler::TestNameImpl> (varB);
      
    TestImportedNameResolver nameRes;
    TestCodeGenerator::TestSequenceCodeGenerator seqGen (*(testBlockImpl->sequence), &nameRes);
    StringsArrayPtr generateResult (seqGen.Generate ());
    
    TS_ASSERT_EQUALS(generateResult->Size(), 1);
    if (generateResult->Size() < 1) return;
    std::string resultRegName (seqGen.GetOutputRegisterName (
      testBlockImpl->GetRegisterForName (testVarB, false)));
    StringStringMap substMap;
    substMap["R0"] = resultRegName;
    TS_ASSERT_EQUALS(generateResult->Get (0),
		     StringSubstitute ("float $R0 = 1;", substMap));
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
      boost::static_pointer_cast<TestSemanticsHandler::TestNameImpl> (varA);
    boost::shared_ptr<TestSemanticsHandler::TestNameImpl> testVarB =
      boost::static_pointer_cast<TestSemanticsHandler::TestNameImpl> (varB);
    boost::shared_ptr<TestSemanticsHandler::TestNameImpl> testVarC =
      boost::static_pointer_cast<TestSemanticsHandler::TestNameImpl> (varC);
      
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
    TS_ASSERT_EQUALS(generateResult->Size(), 3);
    unsigned int l = 0;
    if (l >= generateResult->Size()) return;
    TS_ASSERT_EQUALS(generateResult->Get (l++),
		     StringSubstitute ("float $A;", substMap));
    if (l >= generateResult->Size()) return;
    TS_ASSERT_EQUALS(generateResult->Get (l++),
		     StringSubstitute ("float $B;", substMap));
    if (l >= generateResult->Size()) return;
    TS_ASSERT_EQUALS(generateResult->Get (l++),
		     StringSubstitute ("float $C = $A + $B;", substMap));
  }
  
};