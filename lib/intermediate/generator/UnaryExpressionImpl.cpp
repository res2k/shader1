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

#include "UnaryExpressionImpl.h"

#include "BlockImpl.h"
#include "intermediate/Diagnostics.h"
#include "intermediate/SequenceBuilder.h"
#include "intermediate/SequenceOp/SequenceOpUnaryOp.h"

#include <boost/make_shared.hpp>

namespace s1
{
  namespace intermediate
  {
    IntermediateGeneratorSemanticsHandler::UnaryExpressionImpl::UnaryExpressionImpl (
                           IntermediateGeneratorSemanticsHandler* handler,
                           ExpressionContext&& context,
                           UnaryOp op,
                           const boost::shared_ptr<ExpressionImpl>& operand)
       : ExpressionImpl (handler, std::move (context)), op (op), operand (operand)
    {
    }

    IntermediateGeneratorSemanticsHandler::NameImplSet
    IntermediateGeneratorSemanticsHandler::UnaryExpressionImpl::QueryWrittenNames (bool asLvalue)
    {
      return operand->QueryWrittenNames (asLvalue);
    }
      
    boost::shared_ptr<IntermediateGeneratorSemanticsHandler::TypeImpl>
    IntermediateGeneratorSemanticsHandler::UnaryExpressionImpl::GetValueType()
    {
      boost::shared_ptr<TypeImpl> operandType = operand->GetValueType();
      if (!operandType) return TypeImplPtr(); // Assume error already handled
      boost::shared_ptr<TypeImpl> valueType;
      
      switch (op)
      {
      case Neg:
        if (operandType->IsEqual (*(handler->GetFloatType()))
            || operandType->IsEqual (*(handler->GetIntType())))
          valueType = operandType;
        else if (operandType->IsEqual (*(handler->GetUintType())))
          valueType = handler->GetIntType();
        break;
      case Inv:
        if (operandType->IsEqual (*(handler->GetUintType()))
            || operandType->IsEqual (*(handler->GetIntType())))
          valueType = operandType;
        break;
      case Not:
        if (operandType->IsEqual (*(handler->GetBoolType())))
          valueType = operandType;
        break;
      }

      if (!valueType)
      {
        ExpressionError (Error::OperandTypesInvalid);
        return TypeImplPtr();
      }
      
      return valueType;
    }
    
    RegisterPtr IntermediateGeneratorSemanticsHandler::UnaryExpressionImpl::AddToSequence (BlockImpl& block,
                                                                                           RegisterClassification classify,
                                                                                           bool asLvalue)
    {
      if (asLvalue) return RegisterPtr();
      
      SequenceBuilder& seq (*(block.GetSequenceBuilder()));
      boost::shared_ptr<TypeImpl> valueType = GetValueType ();
      if (!valueType) return RegisterPtr(); // Assume error already handled
      boost::shared_ptr<TypeImpl> operandType = operand->GetValueType();
      if (!operandType) return RegisterPtr(); // Assume error already handled

      // Set up register for operand value
      RegisterPtr orgReg, reg;
      orgReg = reg = operand->AddToSequence (block, Intermediate);
      if (!reg) return RegisterPtr(); // Assume error already handled
      if (!valueType->IsEqual (*(operandType.get())))
      {
        // Insert cast op
        RegisterPtr newReg (handler->AllocateRegister (seq, valueType, Intermediate));
        auto srcCast = handler->GenerateCast (seq, newReg, valueType,
                                              reg, operandType);
        if (srcCast.has_error ())
        {
          ExpressionError (srcCast.error ());
          return RegisterPtr ();
        }
        reg = newReg;
      }
      
      RegisterPtr destination (handler->AllocateRegister (*(block.GetSequenceBuilder()), GetValueType(), classify));
      
      // Create actual sequence operation
      SequenceOpPtr seqOp;
      switch (op)
      {
      case Neg:
        seqOp = SequenceOpPtr (new SequenceOpUnaryOp (destination, SequenceVisitor::Neg, reg));
        break;
      case Inv:
        seqOp = SequenceOpPtr (new SequenceOpUnaryOp (destination, SequenceVisitor::Inv, reg));
        break;
      case Not:
        seqOp = SequenceOpPtr (new SequenceOpUnaryOp (destination, SequenceVisitor::Not, reg));
        break;
      }
      assert (seqOp);
      seq.AddOp (seqOp);
      
      operand->AddToSequencePostAction (block, orgReg, false);
      
      return destination;
    }
  } // namespace intermediate
} // namespace s1
