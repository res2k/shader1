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

#include "base/common.h"

#include "TernaryExpressionImpl.h"

#include "BlockImpl.h"
#include "intermediate/Exception.h"
#include "NameImpl.h"
#include "TypeImpl.h"

#include <boost/make_shared.hpp>

namespace s1
{
  namespace intermediate
  {
    IntermediateGeneratorSemanticsHandler::TernaryExpressionImpl::TernaryExpressionImpl (
      IntermediateGeneratorSemanticsHandler* handler,
      const boost::shared_ptr<ExpressionImpl>& condition,
      const boost::shared_ptr<ExpressionImpl>& ifExpr,
      const boost::shared_ptr<ExpressionImpl>& elseExpr)
       : ExpressionImpl (handler), condition (condition), ifExpr (ifExpr), elseExpr (elseExpr)
    {
    }

    IntermediateGeneratorSemanticsHandler::NameImplSet
    IntermediateGeneratorSemanticsHandler::TernaryExpressionImpl::QueryWrittenNames (bool asLvalue)
    {
      NameImplSet set;
      {
        NameImplSet op1Set (ifExpr->QueryWrittenNames (asLvalue));
        set.insert (op1Set.begin(), op1Set.end());
      }
      {
        NameImplSet op2Set (elseExpr->QueryWrittenNames (asLvalue));
        set.insert (op2Set.begin(), op2Set.end());
      }
      return set;
    }
      
    boost::shared_ptr<IntermediateGeneratorSemanticsHandler::TypeImpl>
    IntermediateGeneratorSemanticsHandler::TernaryExpressionImpl::GetValueType()
    {
      // Determine type in which to perform computation
      boost::shared_ptr<TypeImpl> valueType = IntermediateGeneratorSemanticsHandler::GetHigherPrecisionType (
        ifExpr->GetValueType(), elseExpr->GetValueType());
        
      if (!valueType)
      {
        throw Exception (TernaryExpressionTypesIncompatible);
      }
      
      return valueType;
    }
    
    RegisterPtr IntermediateGeneratorSemanticsHandler::TernaryExpressionImpl::AddToSequence (BlockImpl& block,
                                                                                             RegisterClassification classify,
                                                                                             bool asLvalue)
    {
      if (asLvalue) return RegisterPtr();
      
      /* Spec says: if condition is true, evaluate 'if' expression;
         if condition is false, evaluate 'else' expression.
         Between the lines, don't evaluate the _other_ expression in either case.
         To get that effect, synthesize a branching op. */
      
      // Get Name object for the ternary op result
      NamePtr destName (block.GetTernaryResultName (GetValueType()));

      BlockPtr ifBlock (handler->CreateBlock (block.GetInnerScope()));
      // Synthesize assignment for 'true' case
      {
        ExpressionPtr destNameExpr (handler->CreateVariableExpression (destName));
        ExpressionPtr ifAssignExpr (handler->CreateAssignExpression (destNameExpr, ifExpr));
        ifBlock->AddExpressionCommand (ifAssignExpr);
      }
      BlockPtr elseBlock (handler->CreateBlock (block.GetInnerScope()));
      // Synthesize assignment for 'false' case
      {
        ExpressionPtr destNameExpr (handler->CreateVariableExpression (destName));
        ExpressionPtr elseAssignExpr (handler->CreateAssignExpression (destNameExpr, elseExpr));
        elseBlock->AddExpressionCommand (elseAssignExpr);
      }
      // Add branch operation to sequence
      block.AddBranching (condition, ifBlock, elseBlock);
      // Assign ternary result to destination
      {
        ExpressionPtr destNameExpr (handler->CreateVariableExpression (destName));
        boost::shared_ptr<ExpressionImpl> destNameExprImpl (boost::static_pointer_cast<ExpressionImpl> (destNameExpr));
        return destNameExprImpl->AddToSequence (block, Intermediate);
      }
    }
  } // namespace intermediate
} // namespace s1
