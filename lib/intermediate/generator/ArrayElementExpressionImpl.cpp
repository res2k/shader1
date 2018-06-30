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

#include "ArrayElementExpressionImpl.h"

#include "intermediate/Diagnostics.h"
#include "intermediate/SequenceBuilder.h"
#include "intermediate/SequenceOp/SequenceOpChangeArrayElement.h"
#include "intermediate/SequenceOp/SequenceOpExtractArrayElement.h"

#include "BlockImpl.h"

namespace s1
{
  namespace intermediate
  {
    IntermediateGeneratorSemanticsHandler::ArrayElementExpressionImpl::ArrayElementExpressionImpl (IntermediateGeneratorSemanticsHandler* handler,
                                                                                                   ExpressionContext&& context,
                                                                                                   const ExpressionPtr& arrayExpr,
                                                                                                   const ExpressionPtr& indexExpr)
     : ExpressionImpl (handler, std::move (context)), arrayExpr (arrayExpr), indexExpr (indexExpr)
    {}
    
    semantics::TypePtr IntermediateGeneratorSemanticsHandler::ArrayElementExpressionImpl::GetValueType ()
    {
      auto exprImpl = get_static_ptr<ExpressionImpl> (arrayExpr);
      auto exprType = exprImpl->GetValueType();
      if (!exprType) return nullptr; // Assume error already handled
      if (exprType->GetTypeClass() != semantics::Type::Array)
      {
        ExpressionError (Error::NotAnArray);
        return nullptr;
      }
      return exprType->GetAVMBase();
    }
    
    RegisterPtr IntermediateGeneratorSemanticsHandler::ArrayElementExpressionImpl::AddToSequence (BlockImpl& block,
                                                                                                  RegisterClassification classify,
                                                                                                  bool asLvalue)
    {
      SequenceBuilder& seq (*(block.GetSequenceBuilder()));

      auto arrayExprImpl = get_static_ptr<ExpressionImpl> (arrayExpr);
      auto indexExprImpl = get_static_ptr<ExpressionImpl> (indexExpr);
      
      auto indexType = indexExprImpl->GetValueType();
      if (!indexType->CompatibleLossless (*(handler->GetUintType())))
      {
        ExpressionError (Error::IndexNotAnInteger);
        return RegisterPtr();
      }
      
      RegisterPtr targetReg (handler->AllocateRegister (seq, GetValueType(), classify));
      
      if (asLvalue)
      {
        return targetReg;
      }
      else
      {
        RegisterPtr arrayReg (arrayExprImpl->AddToSequence (block, Intermediate));
        RegisterPtr indexReg (indexExprImpl->AddToSequence (block, Index));
        if (!arrayReg || !indexReg) return RegisterPtr(); // Assume error already handled

        RegisterPtr orgIndexReg (indexReg);
        if (!indexType->IsEqual (*(handler->GetUintType())))
        {
          RegisterPtr newIndexReg (handler->AllocateRegister (seq, handler->GetUintType(), Index));
          auto indexCast = handler->GenerateCast (seq, newIndexReg, handler->GetUintType().get(), indexReg, indexType.get());
          if (indexCast.has_error())
          {
            ExpressionError (indexCast.error());
            return RegisterPtr ();
          }
          indexReg = newIndexReg;
        }
        
        SequenceOpPtr seqOp (new SequenceOpExtractArrayElement (targetReg, arrayReg, indexReg));
        seq.AddOp (seqOp);
        
        arrayExprImpl->AddToSequencePostAction (block, arrayReg, false);
        indexExprImpl->AddToSequencePostAction (block, orgIndexReg, false);
        
        return targetReg;
      }
      
    }

    void IntermediateGeneratorSemanticsHandler::ArrayElementExpressionImpl::AddToSequencePostAction (BlockImpl& block,
                                                                                                     const RegisterPtr& target,
                                                                                                     bool wasLvalue)
    {
      if (!wasLvalue) return;
      
      SequenceBuilder& seq (*(block.GetSequenceBuilder()));
      
      auto arrayExprImpl = get_static_ptr<ExpressionImpl> (arrayExpr);
      auto indexExprImpl = get_static_ptr<ExpressionImpl> (indexExpr);
      auto indexType = indexExprImpl->GetValueType();
      if (!indexType) return; // Assume error already handled
      
      RegisterPtr arrayRegSrc (arrayExprImpl->AddToSequence (block, Intermediate, false));
      RegisterPtr arrayRegDst (arrayExprImpl->AddToSequence (block, Intermediate, true));
      RegisterPtr indexReg (indexExprImpl->AddToSequence (block, Index));
      if (!arrayRegDst)
      {
        ExpressionError (Error::ArrayNotAnLvalue);
        return;
      }
      if (!arrayRegSrc || !indexReg) return; // Assume error already handled

      RegisterPtr orgIndexReg (indexReg);
      if (!indexType->IsEqual (*(handler->GetUintType())))
      {
        RegisterPtr newIndexReg (handler->AllocateRegister (seq, handler->GetUintType(), Index));
        auto indexCast = handler->GenerateCast (seq, newIndexReg, handler->GetUintType().get(), indexReg, indexType.get());
        if (indexCast.has_error())
        {
          ExpressionError (indexCast.error());
          return;
        }
        indexReg = newIndexReg;
      }
      
      SequenceOpPtr seqOp (new SequenceOpChangeArrayElement (arrayRegDst, arrayRegSrc, indexReg, target));
      seq.AddOp (seqOp);
      
      arrayExprImpl->AddToSequencePostAction (block, arrayRegSrc, false);
      arrayExprImpl->AddToSequencePostAction (block, arrayRegDst, true);
      indexExprImpl->AddToSequencePostAction (block, orgIndexReg, false);
    }

  } // namespace intermediate
} // namespace s1
