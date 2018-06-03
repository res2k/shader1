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

#include "ArithmeticExpressionImpl.h"

#include "BlockImpl.h"
#include "intermediate/Diagnostics.h"
#include "intermediate/SequenceBuilder.h"
#include "intermediate/SequenceOp/SequenceOpArith.h"

#include <boost/make_shared.hpp>

namespace s1
{
  namespace intermediate
  {
    IntermediateGeneratorSemanticsHandler::ArithmeticExpressionImpl::ArithmeticExpressionImpl (
      IntermediateGeneratorSemanticsHandler* handler,
      ExpressionContext&& context,
      ArithmeticOp op,
      const boost::shared_ptr<ExpressionImpl>& operand1,
      const boost::shared_ptr<ExpressionImpl>& operand2)
       : BinaryExpressionImpl (handler, std::move (context), operand1, operand2), op (op)
    {
    }

    IntermediateGeneratorSemanticsHandler::TypeImplPtr
    IntermediateGeneratorSemanticsHandler::ArithmeticExpressionImpl::GetValueType()
    {
      TypeImplPtr type1 = operand1->GetValueType();
      TypeImplPtr type2 = operand2->GetValueType();
      if (!type1 || !type2) return TypeImplPtr(); // Assume error already handled
      
      TypeImplPtr baseType1, baseType2;
      unsigned int vectorDim1 = 0, vectorDim2 = 0;
      
      if (type1->typeClass == TypeImpl::Vector)
      {
        baseType1 = boost::static_pointer_cast<TypeImpl> (type1->avmBase);
        vectorDim1 = type1->vectorDim;
      }
      else
        baseType1 = type1;
      if (type2->typeClass == TypeImpl::Vector)
      {
        baseType2 = boost::static_pointer_cast<TypeImpl> (type2->avmBase);
        vectorDim2 = type2->vectorDim;
      }
      else
        baseType2 = type2;
      
      if (!baseType1->CompatibleLossy (*(handler->GetFloatType().get()))
        || !baseType2->CompatibleLossy (*(handler->GetFloatType().get())))
      {
        ExpressionError (Error::OperandTypesInvalid);
        return TypeImplPtr();
      }
      
      if ((vectorDim1 != 0) && (vectorDim2 != 0) && (vectorDim1 != vectorDim2))
      {
        ExpressionError (Error::OperandTypesIncompatible);
        return TypeImplPtr();
      }
      
      // Determine type in which to perform computation
      TypeImplPtr valueType;
      
      if ((vectorDim1 != 0) && (vectorDim2 == 0))
      {
        valueType = IntermediateGeneratorSemanticsHandler::GetHigherPrecisionType (
          type1,
          boost::static_pointer_cast<TypeImpl> (handler->CreateVectorType (type2, vectorDim1)));
      }
      else if ((vectorDim2 != 0) && (vectorDim1 == 0))
      {
        valueType = IntermediateGeneratorSemanticsHandler::GetHigherPrecisionType (
          boost::static_pointer_cast<TypeImpl> (handler->CreateVectorType (type1, vectorDim2)),
          type2);
      }
      else // (((vectorDim1 != 0) && (vectorDim2 != 0)) || ((vectorDim1 == 0) && (vectorDim2 == 0)))
      {
        valueType = IntermediateGeneratorSemanticsHandler::GetHigherPrecisionType (
          operand1->GetValueType(), operand2->GetValueType());
      }
        
      if (!valueType)
      {
        ExpressionError (Error::OperandTypesIncompatible);
        return TypeImplPtr();
      }
      
      return valueType;
    }
    
    RegisterPtr IntermediateGeneratorSemanticsHandler::ArithmeticExpressionImpl::AddToSequence (BlockImpl& block,
                                                                                                RegisterClassification classify,
                                                                                                bool asLvalue)
    {
      if (asLvalue) return RegisterPtr();

      boost::shared_ptr<TypeImpl> valueType = GetValueType ();
      if (!valueType) return RegisterPtr(); // Assume error already handled

      SequenceBuilder& seq (*(block.GetSequenceBuilder()));
      
      // Set up registers for operand values
      auto operandRegs = GetSourceRegisters (block, valueType);
      if (!operandRegs) return RegisterPtr(); // Assume error already handled
      auto reg1 = std::get<0> (*operandRegs).reg;
      auto reg2 = std::get<1> (*operandRegs).reg;
      
      RegisterPtr destination (handler->AllocateRegister (seq, GetValueType(), classify));
      
      // Create actual sequence operation
      SequenceOpPtr seqOp;
      switch (op)
      {
      case Add:
        seqOp = SequenceOpPtr (new SequenceOpArith (destination, SequenceVisitor::Add, reg1, reg2));
        break;
      case Sub:
        seqOp = SequenceOpPtr (new SequenceOpArith (destination, SequenceVisitor::Sub, reg1, reg2));
        break;
      case Mul:
        seqOp = SequenceOpPtr (new SequenceOpArith (destination, SequenceVisitor::Mul, reg1, reg2));
        break;
      case Div:
        seqOp = SequenceOpPtr (new SequenceOpArith (destination, SequenceVisitor::Div, reg1, reg2));
        break;
      case Mod:
        seqOp = SequenceOpPtr (new SequenceOpArith (destination, SequenceVisitor::Mod, reg1, reg2));
        break;
      }
      assert (seqOp);
      seq.AddOp (seqOp);
      
      operand1->AddToSequencePostAction (block, std::get<0> (*operandRegs).orig, false);
      operand2->AddToSequencePostAction (block, std::get<1> (*operandRegs).orig, false);
      
      return destination;
    }
  } // namespace intermediate
} // namespace s1
