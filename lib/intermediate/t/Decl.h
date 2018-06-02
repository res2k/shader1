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
#include "BlockImpl.h"
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

      using Superclass::sequenceBuilder;
    };
  };
public:
  void testDeclVarInit (void)
  {
    TestSemanticsHandler semanticsHandler;

    // Create a scope
    s1::semantics::ScopePtr testScope = semanticsHandler.CreateScope (
      s1::semantics::ScopePtr (), s1::semantics::ScopeLevel::Global);
    // Create a block
    s1::semantics::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
    // Add some variables
    s1::semantics::TypePtr floatType = semanticsHandler.CreateType (s1::semantics::BaseType::Float);
    s1::semantics::NamePtr varA = testBlock->GetInnerScope()->AddVariable (floatType, s1::uc::String ("a"),
                                                                           s1::semantics::ExpressionPtr (),
                                                                           false);
    s1::semantics::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
    s1::semantics::NamePtr varB = testBlock->GetInnerScope()->AddVariable (floatType, s1::uc::String ("b"),
                                                                           exprA,
                                                                           false);
    // No further ops - initializer on 'b' should be enough to emit an op ...

    TestSemanticsHandler::TestBlockImpl* testBlockImpl =
      static_cast<TestSemanticsHandler::TestBlockImpl*> (testBlock.get());
    testBlockImpl->FinishBlock(); // Needed for initializer emission

    TestSequenceVisitor visitor;
    testBlockImpl->sequenceBuilder->GetSequence()->Visit (visitor);
    TS_ASSERT_EQUALS(visitor.entries.size(), 1);
    if (visitor.entries.size() < 1) return;
    TS_ASSERT_EQUALS(visitor.entries[0].op, TestSequenceVisitor::opAssignment);
  }


};
