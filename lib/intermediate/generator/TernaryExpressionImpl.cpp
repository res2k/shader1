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
#include "base/intrusive_ptr.h"

#include "TernaryExpressionImpl.h"

#include "BlockImpl.h"
#include "intermediate/Diagnostics.h"
#include "NameImpl.h"

namespace s1
{
  namespace intermediate
  {
    IntermediateGeneratorSemanticsHandler::TernaryExpressionImpl::TernaryExpressionImpl (
      IntermediateGeneratorSemanticsHandler* handler,
      ExpressionContext&& context,
      ExpressionImpl* condition,
      ExpressionImpl* ifExpr,
      ExpressionImpl* elseExpr)
       : ExpressionImpl (handler, std::move (context)), condition (condition), ifExpr (ifExpr), elseExpr (elseExpr)
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
      
    semantics::TypePtr IntermediateGeneratorSemanticsHandler::TernaryExpressionImpl::GetValueType()
    {
      // Determine type in which to perform computation
      auto valueType = semantics::Type::GetHigherPrecisionType (
        ifExpr->GetValueType().get(), elseExpr->GetValueType().get());
        
      if (!valueType)
      {
        ExpressionError (Error::TernaryExpressionTypesIncompatible);
        return nullptr;
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
      semantics::NamePtr destName (block.GetTernaryResultName (GetValueType().get()));

      auto ifBlock = handler->CreateBlock (block.GetInnerScope());
      // Synthesize assignment for 'true' case
      {
        ExpressionPtr destNameExpr (handler->CreateVariableExpression (destName));
        ExpressionPtr ifAssignExpr (handler->CreateAssignExpression (destNameExpr, ifExpr));
        ifBlock->AddExpressionCommand (ifAssignExpr);
      }
      auto elseBlock = handler->CreateBlock (block.GetInnerScope());
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
        auto destNameExprImpl = get_static_ptr<ExpressionImpl> (destNameExpr);
        return destNameExprImpl->AddToSequence (block, Intermediate);
      }
    }
  } // namespace intermediate
} // namespace s1
