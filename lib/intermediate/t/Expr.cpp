/*
    Shader1
    Copyright (c) 2010-2018 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#include <boost/test/unit_test.hpp>

#include "base/common.h"

#include "intermediate/Diagnostics.h"
#include "intermediate/IntermediateGeneratorSemanticsHandler.h"
#include "intermediate/SequenceBuilder.h"
#include "semantics/Name.h"
#include "BlockImpl.h"
#include "TestSequenceVisitor.h"

#include "../../diagnostics/t/TestDiagnosticsHandler.h"

#include "print_TestSequenceVisitor_Operation.h"

#include <boost/lexical_cast.hpp>

using namespace s1::intermediate;

BOOST_AUTO_TEST_SUITE(IntermediateExpr)

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

  TestDiagnosticsHandler diagnostics;

  TestSemanticsHandler ()
  {
    SetDiagnosticsHandler (&diagnostics);
  }
};

BOOST_AUTO_TEST_CASE(ExprAssignConstFloat)
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
  // Create a simple expression "a = 1"
  s1::semantics::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
  s1::semantics::ExpressionPtr expr1 = semanticsHandler.CreateConstNumericExpression (s1::uc::String ("1.0"));
  s1::semantics::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprA, expr1);
  // Add to a block
  s1::semantics::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
  testBlock->AddExpressionCommand (assignExpr);
  testBlock->Finish();

  TestSemanticsHandler::TestBlockImpl* testBlockImpl =
    static_cast<TestSemanticsHandler::TestBlockImpl*> (testBlock.get());
  TestSequenceVisitor visitor;
  testBlockImpl->sequenceBuilder->GetSequence()->Visit (visitor);
  BOOST_CHECK_EQUAL(visitor.entries.size(), 1u);
  BOOST_CHECK_EQUAL(visitor.entries[0].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL(visitor.entries[0].floatConst, 1.0f);
}

BOOST_AUTO_TEST_CASE(ExprAssignConstInt)
{
  TestSemanticsHandler semanticsHandler;

  // Create a scope
  s1::semantics::ScopePtr testScope = semanticsHandler.CreateScope (
    s1::semantics::ScopePtr (), s1::semantics::ScopeLevel::Global);
  // Add some variables
  s1::semantics::TypePtr intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  s1::semantics::NamePtr varA = testScope->AddVariable (intType, s1::uc::String ("a"),
                                                                s1::semantics::ExpressionPtr (),
                                                                false);
  // Create a simple expression "a = 1"
  s1::semantics::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
  s1::semantics::ExpressionPtr expr1 = semanticsHandler.CreateConstNumericExpression (s1::uc::String ("1"));
  s1::semantics::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprA, expr1);
  // Add to a block
  s1::semantics::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
  testBlock->AddExpressionCommand (assignExpr);
  testBlock->Finish();

  TestSemanticsHandler::TestBlockImpl* testBlockImpl =
    static_cast<TestSemanticsHandler::TestBlockImpl*> (testBlock.get());
  TestSequenceVisitor visitor;
  testBlockImpl->sequenceBuilder->GetSequence()->Visit (visitor);
  BOOST_CHECK_EQUAL(visitor.entries.size(), 2u);
  BOOST_CHECK_EQUAL(visitor.entries[0].op, TestSequenceVisitor::opConstUInt);
  BOOST_CHECK_EQUAL(visitor.entries[0].uintConst, 1u);
  BOOST_CHECK_EQUAL(visitor.entries[1].op, TestSequenceVisitor::opCastToInt);
}

BOOST_AUTO_TEST_CASE(ExprAssignConstIntNeg)
{
  TestSemanticsHandler semanticsHandler;

  // Create a scope
  s1::semantics::ScopePtr testScope = semanticsHandler.CreateScope (
    s1::semantics::ScopePtr (), s1::semantics::ScopeLevel::Global);
  // Add some variables
  s1::semantics::TypePtr intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  s1::semantics::NamePtr varA = testScope->AddVariable (intType, s1::uc::String ("a"),
                                                                s1::semantics::ExpressionPtr (),
                                                                false);
  // Create a simple expression "a = 1"
  s1::semantics::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
  s1::semantics::ExpressionPtr expr1 = semanticsHandler.CreateConstNumericExpression (s1::uc::String ("-1"));
  s1::semantics::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprA, expr1);
  // Add to a block
  s1::semantics::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
  testBlock->AddExpressionCommand (assignExpr);

  TestSemanticsHandler::TestBlockImpl* testBlockImpl =
    static_cast<TestSemanticsHandler::TestBlockImpl*> (testBlock.get());
  TestSequenceVisitor visitor;
  testBlockImpl->sequenceBuilder->GetSequence()->Visit (visitor);
  BOOST_CHECK_EQUAL(visitor.entries.size(), 1u);
  BOOST_CHECK_EQUAL(visitor.entries[0].op, TestSequenceVisitor::opConstInt);
  BOOST_CHECK_EQUAL(visitor.entries[0].intConst, -1);
}

BOOST_AUTO_TEST_CASE(ExprAssignConstUInt)
{
  TestSemanticsHandler semanticsHandler;

  // Create a scope
  s1::semantics::ScopePtr testScope = semanticsHandler.CreateScope (
    s1::semantics::ScopePtr (), s1::semantics::ScopeLevel::Global);
  // Add some variables
  s1::semantics::TypePtr uintType = semanticsHandler.CreateType (s1::semantics::BaseType::UInt);
  s1::semantics::NamePtr varA = testScope->AddVariable (uintType, s1::uc::String ("a"),
                                                                s1::semantics::ExpressionPtr (),
                                                                false);
  // Create a simple expression "a = 1"
  s1::semantics::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
  s1::semantics::ExpressionPtr expr1 = semanticsHandler.CreateConstNumericExpression (s1::uc::String ("1"));
  s1::semantics::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprA, expr1);
  // Add to a block
  s1::semantics::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
  testBlock->AddExpressionCommand (assignExpr);

  TestSemanticsHandler::TestBlockImpl* testBlockImpl =
    static_cast<TestSemanticsHandler::TestBlockImpl*> (testBlock.get());
  TestSequenceVisitor visitor;
  testBlockImpl->sequenceBuilder->GetSequence()->Visit (visitor);
  BOOST_CHECK_EQUAL(visitor.entries.size(), 1u);
  BOOST_CHECK_EQUAL(visitor.entries[0].op, TestSequenceVisitor::opConstUInt);
  BOOST_CHECK_EQUAL(visitor.entries[0].uintConst, 1u);
}

BOOST_AUTO_TEST_CASE(ExprAssignConstIntFromHex)
{
  TestSemanticsHandler semanticsHandler;

  // Create a scope
  s1::semantics::ScopePtr testScope = semanticsHandler.CreateScope (
    s1::semantics::ScopePtr (), s1::semantics::ScopeLevel::Global);
  // Add some variables
  s1::semantics::TypePtr intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  s1::semantics::NamePtr varA = testScope->AddVariable (intType, s1::uc::String ("a"),
                                                                s1::semantics::ExpressionPtr (),
                                                                false);
  // Create a simple expression "a = 1"
  s1::semantics::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
  s1::semantics::ExpressionPtr expr1 = semanticsHandler.CreateConstNumericExpression (s1::uc::String ("0xa"));
  s1::semantics::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprA, expr1);
  // Add to a block
  s1::semantics::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
  testBlock->AddExpressionCommand (assignExpr);

  TestSemanticsHandler::TestBlockImpl* testBlockImpl =
    static_cast<TestSemanticsHandler::TestBlockImpl*> (testBlock.get());
  TestSequenceVisitor visitor;
  testBlockImpl->sequenceBuilder->GetSequence()->Visit (visitor);
  BOOST_CHECK_EQUAL(visitor.entries.size(), 2u);
  BOOST_CHECK_EQUAL(visitor.entries[0].op, TestSequenceVisitor::opConstUInt);
  BOOST_CHECK_EQUAL(visitor.entries[0].uintConst, 10u);
  BOOST_CHECK_EQUAL(visitor.entries[1].op, TestSequenceVisitor::opCastToInt);
}

BOOST_AUTO_TEST_CASE(ExprAssignConstIntFromHEX)
{
  TestSemanticsHandler semanticsHandler;

  // Create a scope
  s1::semantics::ScopePtr testScope = semanticsHandler.CreateScope (
    s1::semantics::ScopePtr (), s1::semantics::ScopeLevel::Global);
  // Add some variables
  s1::semantics::TypePtr intType = semanticsHandler.CreateType (s1::semantics::BaseType::Int);
  s1::semantics::NamePtr varA = testScope->AddVariable (intType, s1::uc::String ("a"),
                                                                s1::semantics::ExpressionPtr (),
                                                                false);
  // Create a simple expression "a = 1"
  s1::semantics::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
  s1::semantics::ExpressionPtr expr1 = semanticsHandler.CreateConstNumericExpression (s1::uc::String ("0XA"));
  s1::semantics::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprA, expr1);
  // Add to a block
  s1::semantics::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
  testBlock->AddExpressionCommand (assignExpr);

  TestSemanticsHandler::TestBlockImpl* testBlockImpl =
    static_cast<TestSemanticsHandler::TestBlockImpl*> (testBlock.get());
  TestSequenceVisitor visitor;
  testBlockImpl->sequenceBuilder->GetSequence()->Visit (visitor);
  BOOST_CHECK_EQUAL(visitor.entries.size(), 2u);
  BOOST_CHECK_EQUAL(visitor.entries[0].op, TestSequenceVisitor::opConstUInt);
  BOOST_CHECK_EQUAL(visitor.entries[0].uintConst, 10u);
  BOOST_CHECK_EQUAL(visitor.entries[1].op, TestSequenceVisitor::opCastToInt);
}

BOOST_AUTO_TEST_CASE(ExprAssignConstUIntFromHex)
{
  TestSemanticsHandler semanticsHandler;

  // Create a scope
  s1::semantics::ScopePtr testScope = semanticsHandler.CreateScope (
    s1::semantics::ScopePtr (), s1::semantics::ScopeLevel::Global);
  // Add some variables
  s1::semantics::TypePtr uintType = semanticsHandler.CreateType (s1::semantics::BaseType::UInt);
  s1::semantics::NamePtr varA = testScope->AddVariable (uintType, s1::uc::String ("a"),
                                                                s1::semantics::ExpressionPtr (),
                                                                false);
  // Create a simple expression "a = 1"
  s1::semantics::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
  s1::semantics::ExpressionPtr expr1 = semanticsHandler.CreateConstNumericExpression (s1::uc::String ("0xa"));
  s1::semantics::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprA, expr1);
  // Add to a block
  s1::semantics::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
  testBlock->AddExpressionCommand (assignExpr);

  TestSemanticsHandler::TestBlockImpl* testBlockImpl =
    static_cast<TestSemanticsHandler::TestBlockImpl*> (testBlock.get());
  TestSequenceVisitor visitor;
  testBlockImpl->sequenceBuilder->GetSequence()->Visit (visitor);
  BOOST_CHECK_EQUAL(visitor.entries.size(), 1u);
  BOOST_CHECK_EQUAL(visitor.entries[0].op, TestSequenceVisitor::opConstUInt);
  BOOST_CHECK_EQUAL(visitor.entries[0].uintConst, 10u);
}

BOOST_AUTO_TEST_CASE(ExprAssignConstUIntFromHEX)
{
  TestSemanticsHandler semanticsHandler;

  // Create a scope
  s1::semantics::ScopePtr testScope = semanticsHandler.CreateScope (
    s1::semantics::ScopePtr (), s1::semantics::ScopeLevel::Global);
  // Add some variables
  s1::semantics::TypePtr uintType = semanticsHandler.CreateType (s1::semantics::BaseType::UInt);
  s1::semantics::NamePtr varA = testScope->AddVariable (uintType, s1::uc::String ("a"),
                                                                s1::semantics::ExpressionPtr (),
                                                                false);
  // Create a simple expression "a = 1"
  s1::semantics::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
  s1::semantics::ExpressionPtr expr1 = semanticsHandler.CreateConstNumericExpression (s1::uc::String ("0XA"));
  s1::semantics::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprA, expr1);
  // Add to a block
  s1::semantics::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
  testBlock->AddExpressionCommand (assignExpr);

  TestSemanticsHandler::TestBlockImpl* testBlockImpl =
    static_cast<TestSemanticsHandler::TestBlockImpl*> (testBlock.get());
  TestSequenceVisitor visitor;
  testBlockImpl->sequenceBuilder->GetSequence()->Visit (visitor);
  BOOST_CHECK_EQUAL(visitor.entries.size(), 1u);
  BOOST_CHECK_EQUAL(visitor.entries[0].op, TestSequenceVisitor::opConstUInt);
  BOOST_CHECK_EQUAL(visitor.entries[0].uintConst, 10u);
}

BOOST_AUTO_TEST_CASE(ExprAssignWithCast)
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
  // Create a simple expression "a = 1"
  s1::semantics::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
  s1::semantics::ExpressionPtr expr1 = semanticsHandler.CreateConstNumericExpression (s1::uc::String ("1"));
  s1::semantics::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprA, expr1);
  // Add to a block
  s1::semantics::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
  testBlock->AddExpressionCommand (assignExpr);

  TestSemanticsHandler::TestBlockImpl* testBlockImpl =
    static_cast<TestSemanticsHandler::TestBlockImpl*> (testBlock.get());
  TestSequenceVisitor visitor;
  testBlockImpl->sequenceBuilder->GetSequence()->Visit (visitor);
  BOOST_CHECK_EQUAL(visitor.entries.size(), 2u);
  BOOST_CHECK_EQUAL(visitor.entries[0].op, TestSequenceVisitor::opConstUInt);
  BOOST_CHECK_EQUAL(visitor.entries[0].uintConst, 1u);
  BOOST_CHECK_EQUAL(visitor.entries[1].op, TestSequenceVisitor::opCastToFloat);
  BOOST_CHECK_EQUAL(visitor.entries[1].sourceReg[0], visitor.entries[0].destReg);
}

BOOST_AUTO_TEST_CASE(ExprAssignWithCastFromHex)
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
  // Create a simple expression "a = 1"
  s1::semantics::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
  s1::semantics::ExpressionPtr expr1 = semanticsHandler.CreateConstNumericExpression (s1::uc::String ("0x1"));
  s1::semantics::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprA, expr1);
  // Add to a block
  s1::semantics::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
  testBlock->AddExpressionCommand (assignExpr);

  TestSemanticsHandler::TestBlockImpl* testBlockImpl =
    static_cast<TestSemanticsHandler::TestBlockImpl*> (testBlock.get());
  TestSequenceVisitor visitor;
  testBlockImpl->sequenceBuilder->GetSequence()->Visit (visitor);
  BOOST_CHECK_EQUAL(visitor.entries.size(), 2u);
  BOOST_CHECK_EQUAL(visitor.entries[0].op, TestSequenceVisitor::opConstUInt);
  BOOST_CHECK_EQUAL(visitor.entries[0].uintConst, 1u);
  BOOST_CHECK_EQUAL(visitor.entries[1].op, TestSequenceVisitor::opCastToFloat);
  BOOST_CHECK_EQUAL(visitor.entries[1].sourceReg[0], visitor.entries[0].destReg);
}

BOOST_AUTO_TEST_CASE(ExprAssignMalformedEmpty)
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
  // Create a simple expression "a = 1"
  s1::semantics::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
  s1::semantics::ExpressionPtr expr1 = semanticsHandler.CreateConstNumericExpression (s1::uc::String (""));
  s1::semantics::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprA, expr1);
  // Add to a block
  s1::semantics::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
  testBlock->AddExpressionCommand (assignExpr);
  BOOST_CHECK_EQUAL(semanticsHandler.diagnostics.semanticError.code,
                    static_cast<unsigned int> (s1::intermediate::Error::NumberParseError));
}

BOOST_AUTO_TEST_CASE(ExprAssignMalformedInput1)
{
  TestSemanticsHandler semanticsHandler;

  // Create a scope
  s1::semantics::ScopePtr testScope = semanticsHandler.CreateScope (
    s1::semantics::ScopePtr (), s1::semantics::ScopeLevel::Global);
  // Add some variables
  s1::semantics::TypePtr uintType = semanticsHandler.CreateType (s1::semantics::BaseType::UInt);
  s1::semantics::NamePtr varA = testScope->AddVariable (uintType, s1::uc::String ("a"),
                                                                s1::semantics::ExpressionPtr (),
                                                                false);
  // Create a simple expression "a = 1"
  s1::semantics::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
  s1::semantics::ExpressionPtr expr1 = semanticsHandler.CreateConstNumericExpression (s1::uc::String ("0xg"));
  s1::semantics::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprA, expr1);
  // Add to a block
  s1::semantics::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
  testBlock->AddExpressionCommand (assignExpr);
  BOOST_CHECK_EQUAL(semanticsHandler.diagnostics.semanticError.code,
                    static_cast<unsigned int> (s1::intermediate::Error::NumberParseError));
}

BOOST_AUTO_TEST_CASE(ExprAssignMalformedInput2)
{
  TestSemanticsHandler semanticsHandler;

  // Create a scope
  s1::semantics::ScopePtr testScope = semanticsHandler.CreateScope (
    s1::semantics::ScopePtr (), s1::semantics::ScopeLevel::Global);
  // Add some variables
  s1::semantics::TypePtr uintType = semanticsHandler.CreateType (s1::semantics::BaseType::UInt);
  s1::semantics::NamePtr varA = testScope->AddVariable (uintType, s1::uc::String ("a"),
                                                                s1::semantics::ExpressionPtr (),
                                                                false);
  // Create a simple expression "a = 1"
  s1::semantics::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
  s1::semantics::ExpressionPtr expr1 = semanticsHandler.CreateConstNumericExpression (s1::uc::String ("-0x1"));
  s1::semantics::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprA, expr1);
  // Add to a block
  s1::semantics::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
  testBlock->AddExpressionCommand (assignExpr);
  BOOST_CHECK_EQUAL(semanticsHandler.diagnostics.semanticError.code,
                    static_cast<unsigned int> (s1::intermediate::Error::NumberParseError));
}

BOOST_AUTO_TEST_CASE(ExprAssignMalformedInput3)
{
  TestSemanticsHandler semanticsHandler;

  // Create a scope
  s1::semantics::ScopePtr testScope = semanticsHandler.CreateScope (
    s1::semantics::ScopePtr (), s1::semantics::ScopeLevel::Global);
  // Add some variables
  s1::semantics::TypePtr uintType = semanticsHandler.CreateType (s1::semantics::BaseType::UInt);
  s1::semantics::NamePtr varA = testScope->AddVariable (uintType, s1::uc::String ("a"),
                                                                s1::semantics::ExpressionPtr (),
                                                                false);
  // Create a simple expression "a = 1"
  s1::semantics::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
  s1::semantics::ExpressionPtr expr1 = semanticsHandler.CreateConstNumericExpression (s1::uc::String ("foo"));
  s1::semantics::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprA, expr1);
  // Add to a block
  s1::semantics::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
  testBlock->AddExpressionCommand (assignExpr);
  BOOST_CHECK_EQUAL(semanticsHandler.diagnostics.semanticError.code,
                    static_cast<unsigned int> (s1::intermediate::Error::NumberParseError));
}

BOOST_AUTO_TEST_CASE(ExprAssignUIntOverflow)
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
  // Create a simple expression "a = 1"
  s1::semantics::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
  std::string overflowing_num (
      boost::lexical_cast<std::string> (
          static_cast<boost::uintmax_t> (std::numeric_limits<unsigned int>::max()) + 1));
  s1::semantics::ExpressionPtr expr1 = semanticsHandler.CreateConstNumericExpression (s1::uc::String (overflowing_num.c_str()));
  s1::semantics::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprA, expr1);
  // Add to a block
  s1::semantics::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
  testBlock->AddExpressionCommand (assignExpr);
  BOOST_CHECK_EQUAL(semanticsHandler.diagnostics.semanticError.code,
                    static_cast<unsigned int> (s1::intermediate::Error::NumberParseError));
}

BOOST_AUTO_TEST_CASE(ExprAssignIntUnderflow)
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
  // Create a simple expression "a = 1"
  s1::semantics::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
  std::string underflowing_num (
      boost::lexical_cast<std::string> (
          static_cast<boost::intmax_t> (std::numeric_limits<int>::min()) - 1));
  s1::semantics::ExpressionPtr expr1 = semanticsHandler.CreateConstNumericExpression (s1::uc::String (underflowing_num.c_str()));
  s1::semantics::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprA, expr1);
  // Add to a block
  s1::semantics::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
  testBlock->AddExpressionCommand (assignExpr);
  BOOST_CHECK_EQUAL(semanticsHandler.diagnostics.semanticError.code,
                    static_cast<unsigned int> (s1::intermediate::Error::NumberParseError));
}

/* TODO: uint underflow, int overflow
  * (detect in cast op? code gen?) */

BOOST_AUTO_TEST_CASE(ExprAssignVar)
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
  s1::semantics::ExpressionPtr exprB = semanticsHandler.CreateVariableExpression (varB);
  s1::semantics::ExpressionPtr assignExpr1 = semanticsHandler.CreateAssignExpression (exprA, expr1);
  s1::semantics::ExpressionPtr assignExpr2 = semanticsHandler.CreateAssignExpression (exprB, exprA);
  // Add to a block
  s1::semantics::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
  testBlock->AddExpressionCommand (assignExpr1);
  testBlock->AddExpressionCommand (assignExpr2);

  TestSemanticsHandler::TestBlockImpl* testBlockImpl =
    static_cast<TestSemanticsHandler::TestBlockImpl*> (testBlock.get());
  TestSequenceVisitor visitor;
  testBlockImpl->sequenceBuilder->GetSequence()->Visit (visitor);
  BOOST_CHECK_EQUAL(visitor.entries.size(), 2u);
  BOOST_CHECK_EQUAL(visitor.entries[0].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL(visitor.entries[0].floatConst, 1.0f);
  BOOST_CHECK_EQUAL(visitor.entries[1].op, TestSequenceVisitor::opAssignment);
  BOOST_CHECK_EQUAL(visitor.entries[1].sourceReg[0], visitor.entries[0].destReg);
}

BOOST_AUTO_TEST_CASE(ExprArithVar)
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
  s1::semantics::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
  testBlock->AddExpressionCommand (assignExpr);

  TestSemanticsHandler::TestBlockImpl* testBlockImpl =
    static_cast<TestSemanticsHandler::TestBlockImpl*> (testBlock.get());
  TestSequenceVisitor visitor;
  testBlockImpl->sequenceBuilder->GetSequence()->Visit (visitor);
  BOOST_CHECK_EQUAL(visitor.entries.size(), 1u);
  BOOST_CHECK_EQUAL(visitor.entries[0].op, TestSequenceVisitor::opArithAdd);
}

BOOST_AUTO_TEST_CASE(ExprArithConst)
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
  // Create a simple expression "c = a + b"
  s1::semantics::ExpressionPtr expr1 = semanticsHandler.CreateConstNumericExpression (s1::uc::String ("1.0"));
  s1::semantics::ExpressionPtr expr2 = semanticsHandler.CreateConstNumericExpression (s1::uc::String ("2.0"));
  s1::semantics::ExpressionPtr addExpr = semanticsHandler.CreateArithmeticExpression (TestSemanticsHandler::Add,
                                                                                              expr1, expr2);
  s1::semantics::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
  s1::semantics::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprA, addExpr);
  // Add to a block
  s1::semantics::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
  testBlock->AddExpressionCommand (assignExpr);

  TestSemanticsHandler::TestBlockImpl* testBlockImpl =
    static_cast<TestSemanticsHandler::TestBlockImpl*> (testBlock.get());
  TestSequenceVisitor visitor;
  testBlockImpl->sequenceBuilder->GetSequence()->Visit (visitor);
  BOOST_CHECK_EQUAL(visitor.entries.size(), 3u);
  BOOST_CHECK_EQUAL(visitor.entries[0].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL(visitor.entries[0].floatConst, 1.0f);
  BOOST_CHECK_EQUAL(visitor.entries[1].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL(visitor.entries[1].floatConst, 2.0f);
  BOOST_CHECK_EQUAL(visitor.entries[2].op, TestSequenceVisitor::opArithAdd);
  BOOST_CHECK_EQUAL(visitor.entries[2].sourceReg[0], visitor.entries[0].destReg);
  BOOST_CHECK_EQUAL(visitor.entries[2].sourceReg[1], visitor.entries[1].destReg);
}

BOOST_AUTO_TEST_CASE(ExprArithConst2)
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
  // Create a simple expression "c = a + b"
  s1::semantics::ExpressionPtr expr1 = semanticsHandler.CreateConstNumericExpression (s1::uc::String ("1"));
  s1::semantics::ExpressionPtr expr2 = semanticsHandler.CreateConstNumericExpression (s1::uc::String ("2.0"));
  s1::semantics::ExpressionPtr addExpr = semanticsHandler.CreateArithmeticExpression (TestSemanticsHandler::Add,
                                                                                              expr1, expr2);
  s1::semantics::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
  s1::semantics::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprA, addExpr);
  // Add to a block
  s1::semantics::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
  testBlock->AddExpressionCommand (assignExpr);

  TestSemanticsHandler::TestBlockImpl* testBlockImpl =
    static_cast<TestSemanticsHandler::TestBlockImpl*> (testBlock.get());
  TestSequenceVisitor visitor;
  testBlockImpl->sequenceBuilder->GetSequence()->Visit (visitor);
  BOOST_CHECK_EQUAL(visitor.entries.size(), 4u);
  BOOST_CHECK_EQUAL(visitor.entries[0].op, TestSequenceVisitor::opConstUInt);
  BOOST_CHECK_EQUAL(visitor.entries[0].uintConst, 1u);
  BOOST_CHECK_EQUAL(visitor.entries[1].op, TestSequenceVisitor::opConstFloat);
  BOOST_CHECK_EQUAL(visitor.entries[1].floatConst, 2.0f);
  BOOST_CHECK_EQUAL(visitor.entries[2].op, TestSequenceVisitor::opCastToFloat);
  BOOST_CHECK_EQUAL(visitor.entries[2].sourceReg[0], visitor.entries[0].destReg);
  BOOST_CHECK_EQUAL(visitor.entries[3].op, TestSequenceVisitor::opArithAdd);
  BOOST_CHECK_EQUAL(visitor.entries[3].sourceReg[0], visitor.entries[2].destReg);
  BOOST_CHECK_EQUAL(visitor.entries[3].sourceReg[1], visitor.entries[1].destReg);
}

BOOST_AUTO_TEST_CASE(ExprArithVarAssignSelf)
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
  // Create a simple expression "a = a + b"
  s1::semantics::ExpressionPtr exprA = semanticsHandler.CreateVariableExpression (varA);
  s1::semantics::ExpressionPtr exprB = semanticsHandler.CreateVariableExpression (varB);
  s1::semantics::ExpressionPtr addExpr = semanticsHandler.CreateArithmeticExpression (TestSemanticsHandler::Add,
                                                                                              exprA, exprB);
  s1::semantics::ExpressionPtr exprA2 = semanticsHandler.CreateVariableExpression (varA);
  s1::semantics::ExpressionPtr assignExpr = semanticsHandler.CreateAssignExpression (exprA2, addExpr);
  // Add to a block
  s1::semantics::BlockPtr testBlock = semanticsHandler.CreateBlock (testScope);
  testBlock->AddExpressionCommand (assignExpr);

  TestSemanticsHandler::TestBlockImpl* testBlockImpl =
    static_cast<TestSemanticsHandler::TestBlockImpl*> (testBlock.get());
  TestSequenceVisitor visitor;
  testBlockImpl->sequenceBuilder->GetSequence()->Visit (visitor);
  BOOST_CHECK_EQUAL(visitor.entries.size(), 1u);
  BOOST_CHECK_NE(visitor.entries[0].destReg, visitor.entries[0].sourceReg[0]);
  BOOST_CHECK_NE(visitor.entries[0].destReg, visitor.entries[0].sourceReg[1]);
}

BOOST_AUTO_TEST_SUITE_END()
