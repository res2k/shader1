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

#include "intermediate/Exception.h"
#include "intermediate/IntermediateGeneratorSemanticsHandler.h"
#include "intermediate/SequenceBuilder.h"
#include "BlockImpl.h"
#include "TestSequenceVisitor.h"

#include "IntermediateTestTraits.h"

#include <boost/lexical_cast.hpp>

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

      using Superclass::sequenceBuilder;
    };
  };
public:
  void testExprAssignConstFloat (void)
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
    // Create a simple expression "a = 1"
    TestSemanticsHandler::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
    TestSemanticsHandler::ExpressionPtr expr1 = semanticsHandler.CreateConstNumericExpression (s1::uc::String ("1.0"));
    TestSemanticsHandler::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprA, expr1);
    // Add to a block
    TestSemanticsHandler::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
    testBlock->AddExpressionCommand (assignExpr);

    TestSemanticsHandler::TestBlockImpl* testBlockImpl =
      static_cast<TestSemanticsHandler::TestBlockImpl*> (testBlock.get());
    TestSequenceVisitor visitor;
    testBlockImpl->sequenceBuilder->GetSequence()->Visit (visitor);
    TS_ASSERT_EQUALS(visitor.entries.size(), 1);
    TS_ASSERT_EQUALS(visitor.entries[0].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS(visitor.entries[0].floatConst, 1.0f);
  }

  void testExprAssignConstInt (void)
  {
    TestSemanticsHandler semanticsHandler;

    // Create a scope
    TestSemanticsHandler::ScopePtr testScope = semanticsHandler.CreateScope (
      TestSemanticsHandler::ScopePtr (), TestSemanticsHandler::Global);
    // Add some variables
    TestSemanticsHandler::TypePtr intType = semanticsHandler.CreateType (TestSemanticsHandler::Int);
    TestSemanticsHandler::NamePtr varA = testScope->AddVariable (intType, s1::uc::String ("a"),
                                                                 TestSemanticsHandler::ExpressionPtr (),
                                                                 false);
    // Create a simple expression "a = 1"
    TestSemanticsHandler::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
    TestSemanticsHandler::ExpressionPtr expr1 = semanticsHandler.CreateConstNumericExpression (s1::uc::String ("1"));
    TestSemanticsHandler::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprA, expr1);
    // Add to a block
    TestSemanticsHandler::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
    testBlock->AddExpressionCommand (assignExpr);

    TestSemanticsHandler::TestBlockImpl* testBlockImpl =
      static_cast<TestSemanticsHandler::TestBlockImpl*> (testBlock.get());
    TestSequenceVisitor visitor;
    testBlockImpl->sequenceBuilder->GetSequence()->Visit (visitor);
    TS_ASSERT_EQUALS(visitor.entries.size(), 2);
    TS_ASSERT_EQUALS(visitor.entries[0].op, TestSequenceVisitor::opConstUInt);
    TS_ASSERT_EQUALS(visitor.entries[0].uintConst, 1);
    TS_ASSERT_EQUALS(visitor.entries[1].op, TestSequenceVisitor::opCastToInt);
  }

  void testExprAssignConstIntNeg (void)
  {
    TestSemanticsHandler semanticsHandler;

    // Create a scope
    TestSemanticsHandler::ScopePtr testScope = semanticsHandler.CreateScope (
      TestSemanticsHandler::ScopePtr (), TestSemanticsHandler::Global);
    // Add some variables
    TestSemanticsHandler::TypePtr intType = semanticsHandler.CreateType (TestSemanticsHandler::Int);
    TestSemanticsHandler::NamePtr varA = testScope->AddVariable (intType, s1::uc::String ("a"),
                                                                 TestSemanticsHandler::ExpressionPtr (),
                                                                 false);
    // Create a simple expression "a = 1"
    TestSemanticsHandler::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
    TestSemanticsHandler::ExpressionPtr expr1 = semanticsHandler.CreateConstNumericExpression (s1::uc::String ("-1"));
    TestSemanticsHandler::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprA, expr1);
    // Add to a block
    TestSemanticsHandler::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
    testBlock->AddExpressionCommand (assignExpr);

    TestSemanticsHandler::TestBlockImpl* testBlockImpl =
      static_cast<TestSemanticsHandler::TestBlockImpl*> (testBlock.get());
    TestSequenceVisitor visitor;
    testBlockImpl->sequenceBuilder->GetSequence()->Visit (visitor);
    TS_ASSERT_EQUALS(visitor.entries.size(), 1);
    TS_ASSERT_EQUALS(visitor.entries[0].op, TestSequenceVisitor::opConstInt);
    TS_ASSERT_EQUALS(visitor.entries[0].intConst, -1);
  }

  void testExprAssignConstUInt (void)
  {
    TestSemanticsHandler semanticsHandler;

    // Create a scope
    TestSemanticsHandler::ScopePtr testScope = semanticsHandler.CreateScope (
      TestSemanticsHandler::ScopePtr (), TestSemanticsHandler::Global);
    // Add some variables
    TestSemanticsHandler::TypePtr uintType = semanticsHandler.CreateType (TestSemanticsHandler::UInt);
    TestSemanticsHandler::NamePtr varA = testScope->AddVariable (uintType, s1::uc::String ("a"),
                                                                 TestSemanticsHandler::ExpressionPtr (),
                                                                 false);
    // Create a simple expression "a = 1"
    TestSemanticsHandler::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
    TestSemanticsHandler::ExpressionPtr expr1 = semanticsHandler.CreateConstNumericExpression (s1::uc::String ("1"));
    TestSemanticsHandler::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprA, expr1);
    // Add to a block
    TestSemanticsHandler::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
    testBlock->AddExpressionCommand (assignExpr);

    TestSemanticsHandler::TestBlockImpl* testBlockImpl =
      static_cast<TestSemanticsHandler::TestBlockImpl*> (testBlock.get());
    TestSequenceVisitor visitor;
    testBlockImpl->sequenceBuilder->GetSequence()->Visit (visitor);
    TS_ASSERT_EQUALS(visitor.entries.size(), 1);
    TS_ASSERT_EQUALS(visitor.entries[0].op, TestSequenceVisitor::opConstUInt);
    TS_ASSERT_EQUALS(visitor.entries[0].uintConst, 1);
  }

  void testExprAssignConstIntFromHex (void)
  {
    TestSemanticsHandler semanticsHandler;

    // Create a scope
    TestSemanticsHandler::ScopePtr testScope = semanticsHandler.CreateScope (
      TestSemanticsHandler::ScopePtr (), TestSemanticsHandler::Global);
    // Add some variables
    TestSemanticsHandler::TypePtr intType = semanticsHandler.CreateType (TestSemanticsHandler::Int);
    TestSemanticsHandler::NamePtr varA = testScope->AddVariable (intType, s1::uc::String ("a"),
                                                                 TestSemanticsHandler::ExpressionPtr (),
                                                                 false);
    // Create a simple expression "a = 1"
    TestSemanticsHandler::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
    TestSemanticsHandler::ExpressionPtr expr1 = semanticsHandler.CreateConstNumericExpression (s1::uc::String ("0xa"));
    TestSemanticsHandler::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprA, expr1);
    // Add to a block
    TestSemanticsHandler::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
    testBlock->AddExpressionCommand (assignExpr);

    TestSemanticsHandler::TestBlockImpl* testBlockImpl =
      static_cast<TestSemanticsHandler::TestBlockImpl*> (testBlock.get());
    TestSequenceVisitor visitor;
    testBlockImpl->sequenceBuilder->GetSequence()->Visit (visitor);
    TS_ASSERT_EQUALS(visitor.entries.size(), 2);
    TS_ASSERT_EQUALS(visitor.entries[0].op, TestSequenceVisitor::opConstUInt);
    TS_ASSERT_EQUALS(visitor.entries[0].uintConst, 10);
    TS_ASSERT_EQUALS(visitor.entries[1].op, TestSequenceVisitor::opCastToInt);
  }

  void testExprAssignConstIntFromHEX (void)
  {
    TestSemanticsHandler semanticsHandler;

    // Create a scope
    TestSemanticsHandler::ScopePtr testScope = semanticsHandler.CreateScope (
      TestSemanticsHandler::ScopePtr (), TestSemanticsHandler::Global);
    // Add some variables
    TestSemanticsHandler::TypePtr intType = semanticsHandler.CreateType (TestSemanticsHandler::Int);
    TestSemanticsHandler::NamePtr varA = testScope->AddVariable (intType, s1::uc::String ("a"),
                                                                 TestSemanticsHandler::ExpressionPtr (),
                                                                 false);
    // Create a simple expression "a = 1"
    TestSemanticsHandler::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
    TestSemanticsHandler::ExpressionPtr expr1 = semanticsHandler.CreateConstNumericExpression (s1::uc::String ("0XA"));
    TestSemanticsHandler::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprA, expr1);
    // Add to a block
    TestSemanticsHandler::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
    testBlock->AddExpressionCommand (assignExpr);

    TestSemanticsHandler::TestBlockImpl* testBlockImpl =
      static_cast<TestSemanticsHandler::TestBlockImpl*> (testBlock.get());
    TestSequenceVisitor visitor;
    testBlockImpl->sequenceBuilder->GetSequence()->Visit (visitor);
    TS_ASSERT_EQUALS(visitor.entries.size(), 2);
    TS_ASSERT_EQUALS(visitor.entries[0].op, TestSequenceVisitor::opConstUInt);
    TS_ASSERT_EQUALS(visitor.entries[0].uintConst, 10);
    TS_ASSERT_EQUALS(visitor.entries[1].op, TestSequenceVisitor::opCastToInt);
  }

  void testExprAssignConstUIntFromHex (void)
  {
    TestSemanticsHandler semanticsHandler;

    // Create a scope
    TestSemanticsHandler::ScopePtr testScope = semanticsHandler.CreateScope (
      TestSemanticsHandler::ScopePtr (), TestSemanticsHandler::Global);
    // Add some variables
    TestSemanticsHandler::TypePtr uintType = semanticsHandler.CreateType (TestSemanticsHandler::UInt);
    TestSemanticsHandler::NamePtr varA = testScope->AddVariable (uintType, s1::uc::String ("a"),
                                                                 TestSemanticsHandler::ExpressionPtr (),
                                                                 false);
    // Create a simple expression "a = 1"
    TestSemanticsHandler::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
    TestSemanticsHandler::ExpressionPtr expr1 = semanticsHandler.CreateConstNumericExpression (s1::uc::String ("0xa"));
    TestSemanticsHandler::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprA, expr1);
    // Add to a block
    TestSemanticsHandler::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
    testBlock->AddExpressionCommand (assignExpr);

    TestSemanticsHandler::TestBlockImpl* testBlockImpl =
      static_cast<TestSemanticsHandler::TestBlockImpl*> (testBlock.get());
    TestSequenceVisitor visitor;
    testBlockImpl->sequenceBuilder->GetSequence()->Visit (visitor);
    TS_ASSERT_EQUALS(visitor.entries.size(), 1);
    TS_ASSERT_EQUALS(visitor.entries[0].op, TestSequenceVisitor::opConstUInt);
    TS_ASSERT_EQUALS(visitor.entries[0].uintConst, 10);
  }

  void testExprAssignConstUIntFromHEX (void)
  {
    TestSemanticsHandler semanticsHandler;

    // Create a scope
    TestSemanticsHandler::ScopePtr testScope = semanticsHandler.CreateScope (
      TestSemanticsHandler::ScopePtr (), TestSemanticsHandler::Global);
    // Add some variables
    TestSemanticsHandler::TypePtr uintType = semanticsHandler.CreateType (TestSemanticsHandler::UInt);
    TestSemanticsHandler::NamePtr varA = testScope->AddVariable (uintType, s1::uc::String ("a"),
                                                                 TestSemanticsHandler::ExpressionPtr (),
                                                                 false);
    // Create a simple expression "a = 1"
    TestSemanticsHandler::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
    TestSemanticsHandler::ExpressionPtr expr1 = semanticsHandler.CreateConstNumericExpression (s1::uc::String ("0XA"));
    TestSemanticsHandler::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprA, expr1);
    // Add to a block
    TestSemanticsHandler::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
    testBlock->AddExpressionCommand (assignExpr);

    TestSemanticsHandler::TestBlockImpl* testBlockImpl =
      static_cast<TestSemanticsHandler::TestBlockImpl*> (testBlock.get());
    TestSequenceVisitor visitor;
    testBlockImpl->sequenceBuilder->GetSequence()->Visit (visitor);
    TS_ASSERT_EQUALS(visitor.entries.size(), 1);
    TS_ASSERT_EQUALS(visitor.entries[0].op, TestSequenceVisitor::opConstUInt);
    TS_ASSERT_EQUALS(visitor.entries[0].uintConst, 10);
  }

  void testExprAssignWithCast (void)
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
    // Create a simple expression "a = 1"
    TestSemanticsHandler::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
    TestSemanticsHandler::ExpressionPtr expr1 = semanticsHandler.CreateConstNumericExpression (s1::uc::String ("1"));
    TestSemanticsHandler::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprA, expr1);
    // Add to a block
    TestSemanticsHandler::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
    testBlock->AddExpressionCommand (assignExpr);

    TestSemanticsHandler::TestBlockImpl* testBlockImpl =
      static_cast<TestSemanticsHandler::TestBlockImpl*> (testBlock.get());
    TestSequenceVisitor visitor;
    testBlockImpl->sequenceBuilder->GetSequence()->Visit (visitor);
    TS_ASSERT_EQUALS(visitor.entries.size(), 2);
    TS_ASSERT_EQUALS(visitor.entries[0].op, TestSequenceVisitor::opConstUInt);
    TS_ASSERT_EQUALS(visitor.entries[0].uintConst, 1);
    TS_ASSERT_EQUALS(visitor.entries[1].op, TestSequenceVisitor::opCastToFloat);
    TS_ASSERT_EQUALS(visitor.entries[1].sourceReg[0], visitor.entries[0].destReg);
  }

  void testExprAssignWithCastFromHex (void)
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
    // Create a simple expression "a = 1"
    TestSemanticsHandler::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
    TestSemanticsHandler::ExpressionPtr expr1 = semanticsHandler.CreateConstNumericExpression (s1::uc::String ("0x1"));
    TestSemanticsHandler::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprA, expr1);
    // Add to a block
    TestSemanticsHandler::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
    testBlock->AddExpressionCommand (assignExpr);

    TestSemanticsHandler::TestBlockImpl* testBlockImpl =
      static_cast<TestSemanticsHandler::TestBlockImpl*> (testBlock.get());
    TestSequenceVisitor visitor;
    testBlockImpl->sequenceBuilder->GetSequence()->Visit (visitor);
    TS_ASSERT_EQUALS(visitor.entries.size(), 2);
    TS_ASSERT_EQUALS(visitor.entries[0].op, TestSequenceVisitor::opConstUInt);
    TS_ASSERT_EQUALS(visitor.entries[0].uintConst, 1);
    TS_ASSERT_EQUALS(visitor.entries[1].op, TestSequenceVisitor::opCastToFloat);
    TS_ASSERT_EQUALS(visitor.entries[1].sourceReg[0], visitor.entries[0].destReg);
  }

  void testExprAssignMalformedEmpty (void)
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
    // Create a simple expression "a = 1"
    TestSemanticsHandler::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
    TestSemanticsHandler::ExpressionPtr expr1 = semanticsHandler.CreateConstNumericExpression (s1::uc::String (""));
    TestSemanticsHandler::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprA, expr1);
    // Add to a block
    TestSemanticsHandler::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
    TS_ASSERT_THROWS_ASSERT(testBlock->AddExpressionCommand (assignExpr),
                            const Exception& e,
                            TS_ASSERT_EQUALS(e.GetCode(), s1::intermediate::NumberParseError));
  }

  void testExprAssignMalformedInput1 (void)
  {
    TestSemanticsHandler semanticsHandler;

    // Create a scope
    TestSemanticsHandler::ScopePtr testScope = semanticsHandler.CreateScope (
      TestSemanticsHandler::ScopePtr (), TestSemanticsHandler::Global);
    // Add some variables
    TestSemanticsHandler::TypePtr uintType = semanticsHandler.CreateType (TestSemanticsHandler::UInt);
    TestSemanticsHandler::NamePtr varA = testScope->AddVariable (uintType, s1::uc::String ("a"),
                                                                 TestSemanticsHandler::ExpressionPtr (),
                                                                 false);
    // Create a simple expression "a = 1"
    TestSemanticsHandler::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
    TestSemanticsHandler::ExpressionPtr expr1 = semanticsHandler.CreateConstNumericExpression (s1::uc::String ("0xg"));
    TestSemanticsHandler::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprA, expr1);
    // Add to a block
    TestSemanticsHandler::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
    TS_ASSERT_THROWS_ASSERT(testBlock->AddExpressionCommand (assignExpr),
                            const Exception& e,
                            TS_ASSERT_EQUALS(e.GetCode(), s1::intermediate::NumberParseError));
  }

  void testExprAssignMalformedInput2 (void)
  {
    TestSemanticsHandler semanticsHandler;

    // Create a scope
    TestSemanticsHandler::ScopePtr testScope = semanticsHandler.CreateScope (
      TestSemanticsHandler::ScopePtr (), TestSemanticsHandler::Global);
    // Add some variables
    TestSemanticsHandler::TypePtr uintType = semanticsHandler.CreateType (TestSemanticsHandler::UInt);
    TestSemanticsHandler::NamePtr varA = testScope->AddVariable (uintType, s1::uc::String ("a"),
                                                                 TestSemanticsHandler::ExpressionPtr (),
                                                                 false);
    // Create a simple expression "a = 1"
    TestSemanticsHandler::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
    TestSemanticsHandler::ExpressionPtr expr1 = semanticsHandler.CreateConstNumericExpression (s1::uc::String ("-0x1"));
    TestSemanticsHandler::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprA, expr1);
    // Add to a block
    TestSemanticsHandler::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
    TS_ASSERT_THROWS_ASSERT(testBlock->AddExpressionCommand (assignExpr),
                            const Exception& e,
                            TS_ASSERT_EQUALS(e.GetCode(), s1::intermediate::NumberParseError));
  }

  void testExprAssignMalformedInput3 (void)
  {
    TestSemanticsHandler semanticsHandler;

    // Create a scope
    TestSemanticsHandler::ScopePtr testScope = semanticsHandler.CreateScope (
      TestSemanticsHandler::ScopePtr (), TestSemanticsHandler::Global);
    // Add some variables
    TestSemanticsHandler::TypePtr uintType = semanticsHandler.CreateType (TestSemanticsHandler::UInt);
    TestSemanticsHandler::NamePtr varA = testScope->AddVariable (uintType, s1::uc::String ("a"),
                                                                 TestSemanticsHandler::ExpressionPtr (),
                                                                 false);
    // Create a simple expression "a = 1"
    TestSemanticsHandler::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
    TestSemanticsHandler::ExpressionPtr expr1 = semanticsHandler.CreateConstNumericExpression (s1::uc::String ("foo"));
    TestSemanticsHandler::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprA, expr1);
    // Add to a block
    TestSemanticsHandler::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
    TS_ASSERT_THROWS_ASSERT(testBlock->AddExpressionCommand (assignExpr),
                            const Exception& e,
                            TS_ASSERT_EQUALS(e.GetCode(), s1::intermediate::NumberParseError));
  }

  void testExprAssignUIntOverflow (void)
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
    // Create a simple expression "a = 1"
    TestSemanticsHandler::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
    std::string overflowing_num (
        boost::lexical_cast<std::string> (
            static_cast<boost::uintmax_t> (std::numeric_limits<unsigned int>::max()) + 1));
    TestSemanticsHandler::ExpressionPtr expr1 = semanticsHandler.CreateConstNumericExpression (s1::uc::String (overflowing_num.c_str()));
    TestSemanticsHandler::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprA, expr1);
    // Add to a block
    TestSemanticsHandler::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
    TS_ASSERT_THROWS_ASSERT(testBlock->AddExpressionCommand (assignExpr),
                            const Exception& e,
                            TS_ASSERT_EQUALS(e.GetCode(), s1::intermediate::NumberParseError));
  }

  void testExprAssignIntUnderflow (void)
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
    // Create a simple expression "a = 1"
    TestSemanticsHandler::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
    std::string underflowing_num (
        boost::lexical_cast<std::string> (
            static_cast<boost::intmax_t> (std::numeric_limits<int>::min()) - 1));
    TestSemanticsHandler::ExpressionPtr expr1 = semanticsHandler.CreateConstNumericExpression (s1::uc::String (underflowing_num.c_str()));
    TestSemanticsHandler::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprA, expr1);
    // Add to a block
    TestSemanticsHandler::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
    TS_ASSERT_THROWS_ASSERT(testBlock->AddExpressionCommand (assignExpr),
                            const Exception& e,
                            TS_ASSERT_EQUALS(e.GetCode(), s1::intermediate::NumberParseError));
  }

  /* TODO: uint underflow, int overflow
   * (detect in cast op? code gen?) */

  void testExprAssignVar (void)
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
    testBlockImpl->sequenceBuilder->GetSequence()->Visit (visitor);
    TS_ASSERT_EQUALS(visitor.entries.size(), 2);
    TS_ASSERT_EQUALS(visitor.entries[0].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS(visitor.entries[0].floatConst, 1.0f);
    TS_ASSERT_EQUALS(visitor.entries[1].op, TestSequenceVisitor::opAssignment);
    TS_ASSERT_EQUALS(visitor.entries[1].sourceReg[0], visitor.entries[0].destReg);
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
    TestSequenceVisitor visitor;
    testBlockImpl->sequenceBuilder->GetSequence()->Visit (visitor);
    TS_ASSERT_EQUALS(visitor.entries.size(), 1);
    TS_ASSERT_EQUALS(visitor.entries[0].op, TestSequenceVisitor::opArithAdd);
  }

  void testExprArithConst (void)
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
    // Create a simple expression "c = a + b"
    TestSemanticsHandler::ExpressionPtr expr1 = semanticsHandler.CreateConstNumericExpression (s1::uc::String ("1.0"));
    TestSemanticsHandler::ExpressionPtr expr2 = semanticsHandler.CreateConstNumericExpression (s1::uc::String ("2.0"));
    TestSemanticsHandler::ExpressionPtr addExpr = semanticsHandler.CreateArithmeticExpression (TestSemanticsHandler::Add,
                                                                                               expr1, expr2);
    TestSemanticsHandler::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
    TestSemanticsHandler::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprA, addExpr);
    // Add to a block
    TestSemanticsHandler::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
    testBlock->AddExpressionCommand (assignExpr);

    TestSemanticsHandler::TestBlockImpl* testBlockImpl =
      static_cast<TestSemanticsHandler::TestBlockImpl*> (testBlock.get());
    TestSequenceVisitor visitor;
    testBlockImpl->sequenceBuilder->GetSequence()->Visit (visitor);
    TS_ASSERT_EQUALS(visitor.entries.size(), 3);
    TS_ASSERT_EQUALS(visitor.entries[0].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS(visitor.entries[0].floatConst, 1.0f);
    TS_ASSERT_EQUALS(visitor.entries[1].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS(visitor.entries[1].floatConst, 2.0f);
    TS_ASSERT_EQUALS(visitor.entries[2].op, TestSequenceVisitor::opArithAdd);
    TS_ASSERT_EQUALS(visitor.entries[2].sourceReg[0], visitor.entries[0].destReg);
    TS_ASSERT_EQUALS(visitor.entries[2].sourceReg[1], visitor.entries[1].destReg);
  }

  void testExprArithConst2 (void)
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
    // Create a simple expression "c = a + b"
    TestSemanticsHandler::ExpressionPtr expr1 = semanticsHandler.CreateConstNumericExpression (s1::uc::String ("1"));
    TestSemanticsHandler::ExpressionPtr expr2 = semanticsHandler.CreateConstNumericExpression (s1::uc::String ("2.0"));
    TestSemanticsHandler::ExpressionPtr addExpr = semanticsHandler.CreateArithmeticExpression (TestSemanticsHandler::Add,
                                                                                               expr1, expr2);
    TestSemanticsHandler::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
    TestSemanticsHandler::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprA, addExpr);
    // Add to a block
    TestSemanticsHandler::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
    testBlock->AddExpressionCommand (assignExpr);

    TestSemanticsHandler::TestBlockImpl* testBlockImpl =
      static_cast<TestSemanticsHandler::TestBlockImpl*> (testBlock.get());
    TestSequenceVisitor visitor;
    testBlockImpl->sequenceBuilder->GetSequence()->Visit (visitor);
    TS_ASSERT_EQUALS(visitor.entries.size(), 4);
    TS_ASSERT_EQUALS(visitor.entries[0].op, TestSequenceVisitor::opConstUInt);
    TS_ASSERT_EQUALS(visitor.entries[0].uintConst, 1);
    TS_ASSERT_EQUALS(visitor.entries[1].op, TestSequenceVisitor::opConstFloat);
    TS_ASSERT_EQUALS(visitor.entries[1].floatConst, 2.0f);
    TS_ASSERT_EQUALS(visitor.entries[2].op, TestSequenceVisitor::opCastToFloat);
    TS_ASSERT_EQUALS(visitor.entries[2].sourceReg[0], visitor.entries[0].destReg);
    TS_ASSERT_EQUALS(visitor.entries[3].op, TestSequenceVisitor::opArithAdd);
    TS_ASSERT_EQUALS(visitor.entries[3].sourceReg[0], visitor.entries[2].destReg);
    TS_ASSERT_EQUALS(visitor.entries[3].sourceReg[1], visitor.entries[1].destReg);
  }

  void testExprArithVarAssignSelf (void)
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
    // Create a simple expression "a = a + b"
    TestSemanticsHandler::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
    TestSemanticsHandler::ExpressionPtr exprB = semanticsHandler.CreateVariableExpression (varB);
    TestSemanticsHandler::ExpressionPtr addExpr = semanticsHandler.CreateArithmeticExpression (TestSemanticsHandler::Add,
                                                                                               exprA, exprB);
    TestSemanticsHandler::ExpressionPtr exprA2 = semanticsHandler.CreateVariableExpression (varA);
    TestSemanticsHandler::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprA2, addExpr);
    // Add to a block
    TestSemanticsHandler::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
    testBlock->AddExpressionCommand (assignExpr);

    TestSemanticsHandler::TestBlockImpl* testBlockImpl =
      static_cast<TestSemanticsHandler::TestBlockImpl*> (testBlock.get());
    TestSequenceVisitor visitor;
    testBlockImpl->sequenceBuilder->GetSequence()->Visit (visitor);
    TS_ASSERT_EQUALS(visitor.entries.size(), 1);
    TS_ASSERT_DIFFERS(visitor.entries[0].destReg, visitor.entries[0].sourceReg[0]);
    TS_ASSERT_DIFFERS(visitor.entries[0].destReg, visitor.entries[0].sourceReg[1]);
  }

};
