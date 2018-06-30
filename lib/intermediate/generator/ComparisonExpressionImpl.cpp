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

#include "ComparisonExpressionImpl.h"

#include "BlockImpl.h"
#include "intermediate/SequenceBuilder.h"
#include "intermediate/SequenceOp/SequenceOpCompare.h"

namespace s1
{
  namespace intermediate
  {
    IntermediateGeneratorSemanticsHandler::ComparisonExpressionImpl::ComparisonExpressionImpl (
      IntermediateGeneratorSemanticsHandler* handler,
      ExpressionContext&& context,
      CompareOp op,
      ExpressionImpl* operand1,
      ExpressionImpl* operand2)
       : BinaryExpressionImpl (handler, std::move (context), operand1, operand2), op (op)
    {
    }

    semantics::TypePtr IntermediateGeneratorSemanticsHandler::ComparisonExpressionImpl::GetValueType()
    {
      // Comparison has always a bool result
      return handler->GetBoolType();
    }
    
    RegisterPtr IntermediateGeneratorSemanticsHandler::ComparisonExpressionImpl::AddToSequence (BlockImpl& block,
                                                                                                RegisterClassification classify,
                                                                                                bool asLvalue)
    {
      if (asLvalue) return RegisterPtr();
      
      SequenceBuilder& seq (*(block.GetSequenceBuilder()));
      auto type1 = operand1->GetValueType();
      auto type2 = operand2->GetValueType();
      if (!type1 || !type2) return RegisterPtr(); // Assume error already handled

      auto comparisonType = handler->GetHigherPrecisionType (type1.get(), type2.get());
        
      // Set up registers for operand values
      auto operandRegs = GetSourceRegisters (block, comparisonType);
      if (!operandRegs) return RegisterPtr(); // Assume error already handled
      auto reg1 = std::get<0> (*operandRegs).reg;
      auto reg2 = std::get<1> (*operandRegs).reg;

      RegisterPtr destination (handler->AllocateRegister (seq, GetValueType().get(), classify));
      
      // Create actual sequence operation
      SequenceOpPtr seqOp;
      switch (op)
      {
      case Equals:
        seqOp = SequenceOpPtr (new SequenceOpCompare (destination, SequenceVisitor::Eq, reg1, reg2));
        break;
      case NotEquals:
        seqOp = SequenceOpPtr (new SequenceOpCompare (destination, SequenceVisitor::NE, reg1, reg2));
        break;
      case Smaller:
        seqOp = SequenceOpPtr (new SequenceOpCompare (destination, SequenceVisitor::LT, reg1, reg2));
        break;
      case SmallerEqual:
        seqOp = SequenceOpPtr (new SequenceOpCompare (destination, SequenceVisitor::LE, reg1, reg2));
        break;
      case Larger:
        seqOp = SequenceOpPtr (new SequenceOpCompare (destination, SequenceVisitor::GT, reg1, reg2));
        break;
      case LargerEqual:
        seqOp = SequenceOpPtr (new SequenceOpCompare (destination, SequenceVisitor::GE, reg1, reg2));
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
