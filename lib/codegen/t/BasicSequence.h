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

#include "codegen/glsl/GLSLGenerator.h"
#include "codegen/glsl/GLSLOptions.h"
#include "../glsl/GLSLSequenceCodeGenerator.h"
#include "../glsl/GLSLTraits.h"

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
  
  class TestImportedNameResolver : public sl::ImportedNameResolver
  {
  public:
    s1::uc::String GetImportedNameIdentifier (const s1::uc::String& name)
    { return s1::uc::String(); }	
    s1::uc::String GetExportedNameIdentifier (const s1::uc::String& name)
    { return s1::uc::String(); }	
  };
  
  class TestCodeGenerator : public glsl::Generator
  {
  public:
    typedef glsl::Generator Superclass;
    
    class TestSequenceCodeGenerator : public glsl::SequenceCodeGenerator
    {
      static const ProgramFunction::TransferMappings& EmptyMappings()
      {
        static const ProgramFunction::TransferMappings m;
        return m;
      }
      static const glsl::Options& DefaultOptions()
      {
        static const glsl::Options o (false);
        return o;
      }
      static const glsl::Traits& DefaultTraits()
      {
        static const glsl::Traits t;
        return t;
      }
    public:
      typedef SequenceCodeGenerator Superclass;
    
      TestSequenceCodeGenerator (const Sequence& seq, sl::ImportedNameResolver* nameRes)
       : SequenceCodeGenerator (seq, nameRes,
				EmptyMappings(), EmptyMappings(),
				std::vector<s1::uc::String> (),
                                DefaultTraits (), DefaultOptions ()) {}
       
      using Superclass::GetOutputRegisterName;
    };
  };  
public:
  void testExprAssignConst (void)
  {
    TestSemanticsHandler semanticsHandler;
    
    // Create a scope
    s1::semantics::ScopePtr testScope = semanticsHandler.CreateScope (
      s1::semantics::ScopePtr (), s1::semantics::ScopeLevel::Global);
    // Add some variables
    s1::semantics::TypePtr floatType = semanticsHandler.CreateType (s1::semantics::BaseType::Float);
    s1::semantics::NamePtr varA = testScope->AddVariable (floatType, s1::uc::String ("a"),
								 s1::semantics::ExpressionPtr (),
								 false);
    s1::semantics::NamePtr varB = testScope->AddVariable (floatType, s1::uc::String ("b"),
								 s1::semantics::ExpressionPtr (),
								 false);
    // Create a simple expression "a = 1"
    s1::semantics::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
    s1::semantics::ExpressionPtr expr1 = semanticsHandler.CreateConstNumericExpression (s1::uc::String ("1.0"));
    s1::semantics::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprA, expr1);
    // Add to a block
    TestSemanticsHandler::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
    testBlock->AddExpressionCommand (assignExpr);
    // Create a simple expression "b = a"
    s1::semantics::ExpressionPtr exprB = semanticsHandler.CreateVariableExpression (varB);
    s1::semantics::ExpressionPtr assignExpr2 = semanticsHandler.CreateAssignExpression (exprB, exprA);
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
    auto testBlockReg = testBlockImpl->GetRegisterForName (testVarB, false);
    TS_ASSERT (testBlockReg);
    std::string resultRegName (seqGen.GetOutputRegisterName (testBlockReg.value()));
    StringStringMap substMap;
    substMap["R0"] = resultRegName;
    TS_ASSERT_EQUALS(generateResult->Get (0),
		     StringSubstitute ("float $R0 = 1;", substMap));
  }
  
  void testExprArithVar (void)
  {
    TestSemanticsHandler semanticsHandler;
    
    // Create a scope
    s1::semantics::ScopePtr testScope = semanticsHandler.CreateScope (
      s1::semantics::ScopePtr (), s1::semantics::ScopeLevel::Global);
    // Add some variables
    s1::semantics::TypePtr floatType = semanticsHandler.CreateType (s1::semantics::BaseType::Float);
    s1::semantics::NamePtr varA = testScope->AddVariable (floatType, s1::uc::String ("a"),
								 s1::semantics::ExpressionPtr (),
								 false);
    s1::semantics::NamePtr varB = testScope->AddVariable (floatType, s1::uc::String ("b"),
								 s1::semantics::ExpressionPtr (),
								 false);
    s1::semantics::NamePtr varC = testScope->AddVariable (floatType, s1::uc::String ("c"),
								 s1::semantics::ExpressionPtr (),
								 false);
    // Create a simple expression "c = a + b"
    s1::semantics::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
    s1::semantics::ExpressionPtr exprB = semanticsHandler.CreateVariableExpression (varB);
    s1::semantics::ExpressionPtr addExpr = semanticsHandler.CreateArithmeticExpression (TestSemanticsHandler::Add,
											       exprA, exprB);
    s1::semantics::ExpressionPtr exprC = semanticsHandler.CreateVariableExpression (varC);
    s1::semantics::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprC, addExpr);
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

    auto testBlockRegA = testBlockImpl->GetRegisterForName (testVarA, false);
    TS_ASSERT (testBlockRegA);
    auto testBlockRegB = testBlockImpl->GetRegisterForName (testVarB, false);
    TS_ASSERT (testBlockRegB);
    auto testBlockRegC = testBlockImpl->GetRegisterForName (testVarC, false);
    TS_ASSERT (testBlockRegC);
    std::string varARegName (seqGen.GetOutputRegisterName (testBlockRegA.value()));
    std::string varBRegName (seqGen.GetOutputRegisterName (testBlockRegB.value()));
    std::string varCRegName (seqGen.GetOutputRegisterName (testBlockRegC.value()));
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
