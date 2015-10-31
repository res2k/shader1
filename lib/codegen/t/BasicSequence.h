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
#include "intermediate/SequenceBuilder.h"
// FIXME: Tests using too much intermediate internals
#include "BlockImpl.h"
#include "NameImpl.h"

#include "codegen/CgGenerator.h"
#include "codegen/CgOptions.h"
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
    
      using Superclass::sequenceBuilder;
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
    std::string GetImportedNameIdentifier (const s1::uc::String& name)
    { return std::string(); }	
    std::string GetExportedNameIdentifier (const s1::uc::String& name)
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
      static const CgOptions& DefaultOptions()
      {
        static const CgOptions o;
        return o;
      }
    public:
      typedef SequenceCodeGenerator Superclass;
    
      TestSequenceCodeGenerator (const Sequence& seq, ImportedNameResolver* nameRes)
       : SequenceCodeGenerator (seq, nameRes,
				EmptyMappings(), EmptyMappings(),
				std::vector<std::string> (),
                                DefaultOptions ()) {}
       
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
    TestSemanticsHandler::NamePtr varA = testScope->AddVariable (floatType, s1::uc::String ("a"),
								 TestSemanticsHandler::ExpressionPtr (),
								 false);
    TestSemanticsHandler::NamePtr varB = testScope->AddVariable (floatType, s1::uc::String ("b"),
								 TestSemanticsHandler::ExpressionPtr (),
								 false);
    // Create a simple expression "a = 1"
    TestSemanticsHandler::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
    TestSemanticsHandler::ExpressionPtr expr1 = semanticsHandler.CreateConstNumericExpression (s1::uc::String ("1.0"));
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
    TestCodeGenerator::TestSequenceCodeGenerator seqGen (*(testBlockImpl->sequenceBuilder->GetSequence()), &nameRes);
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
    TestSemanticsHandler::NamePtr varA = testScope->AddVariable (floatType, s1::uc::String ("a"),
								 TestSemanticsHandler::ExpressionPtr (),
								 false);
    TestSemanticsHandler::NamePtr varB = testScope->AddVariable (floatType, s1::uc::String ("b"),
								 TestSemanticsHandler::ExpressionPtr (),
								 false);
    TestSemanticsHandler::NamePtr varC = testScope->AddVariable (floatType, s1::uc::String ("c"),
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
    TestCodeGenerator::TestSequenceCodeGenerator seqGen (*(testBlockImpl->sequenceBuilder->GetSequence()), &nameRes);
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
