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

#include "ArrayElementExpressionImpl.h"

#include "intermediate/Diagnostics.h"
#include "intermediate/SequenceBuilder.h"
#include "intermediate/SequenceOp/SequenceOpChangeArrayElement.h"
#include "intermediate/SequenceOp/SequenceOpExtractArrayElement.h"

#include "BlockImpl.h"
#include "TypeImpl.h"

#include <boost/make_shared.hpp>

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
    
    IntermediateGeneratorSemanticsHandler::TypeImplPtr
    IntermediateGeneratorSemanticsHandler::ArrayElementExpressionImpl::GetValueType ()
    {
      boost::shared_ptr<ExpressionImpl> exprImpl (boost::static_pointer_cast<ExpressionImpl> (arrayExpr));
      TypeImplPtr exprType (exprImpl->GetValueType());
      if (!exprType) return TypeImplPtr(); // Assume error already handled
      if (exprType->typeClass != TypeImpl::Array)
      {
        ExpressionError (Error::NotAnArray);
        return TypeImplPtr();
      }
      return boost::static_pointer_cast<TypeImpl> (exprType->avmBase);
    }
    
    RegisterPtr IntermediateGeneratorSemanticsHandler::ArrayElementExpressionImpl::AddToSequence (BlockImpl& block,
                                                                                                  RegisterClassification classify,
                                                                                                  bool asLvalue)
    {
      SequenceBuilder& seq (*(block.GetSequenceBuilder()));

      boost::shared_ptr<ExpressionImpl> arrayExprImpl (boost::static_pointer_cast<ExpressionImpl> (arrayExpr));
      boost::shared_ptr<ExpressionImpl> indexExprImpl (boost::static_pointer_cast<ExpressionImpl> (indexExpr));
      
      TypeImplPtr indexType (indexExprImpl->GetValueType());
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
          auto indexCast = handler->GenerateCast (seq, newIndexReg, handler->GetUintType(), indexReg, indexType);
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
      
      boost::shared_ptr<ExpressionImpl> arrayExprImpl (boost::static_pointer_cast<ExpressionImpl> (arrayExpr));
      boost::shared_ptr<ExpressionImpl> indexExprImpl (boost::static_pointer_cast<ExpressionImpl> (indexExpr));
      TypeImplPtr indexType (indexExprImpl->GetValueType());
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
        auto indexCast = handler->GenerateCast (seq, newIndexReg, handler->GetUintType(), indexReg, indexType);
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
