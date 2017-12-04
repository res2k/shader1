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

#include "LogicExpressionImpl.h"

#include "BlockImpl.h"
#include "intermediate/Diagnostics.h"
#include "intermediate/SequenceBuilder.h"
#include "intermediate/SequenceOp/SequenceOpLogic.h"
#include "TypeImpl.h"

#include <boost/make_shared.hpp>

namespace s1
{
  namespace intermediate
  {
    IntermediateGeneratorSemanticsHandler::LogicExpressionImpl::LogicExpressionImpl (
      IntermediateGeneratorSemanticsHandler* handler,
      ExpressionContext&& context,
      LogicOp op,
      const boost::shared_ptr<ExpressionImpl>& operand1,
      const boost::shared_ptr<ExpressionImpl>& operand2)
       : BinaryExpressionImpl (handler, std::move (context), operand1, operand2), op (op)
    {
    }

    boost::shared_ptr<IntermediateGeneratorSemanticsHandler::TypeImpl>
    IntermediateGeneratorSemanticsHandler::LogicExpressionImpl::GetValueType()
    {
      boost::shared_ptr<TypeImpl> type1 = operand1->GetValueType();
      boost::shared_ptr<TypeImpl> type2 = operand2->GetValueType();
      if (!type1 || !type2) return TypeImplPtr(); // Assume error already handled
      
      // Operands have to be bools
      if (!type1->IsEqual (*(handler->GetBoolType().get()))
        || !type2->IsEqual (*(handler->GetBoolType().get())))
      {
        ExpressionError (Error::OperandTypesInvalid);
        return TypeImplPtr();
      }
      
      // Logic ops have always a bool result
      return handler->GetBoolType();
    }
    
    RegisterPtr IntermediateGeneratorSemanticsHandler::LogicExpressionImpl::AddToSequence (BlockImpl& block,
                                                                                           RegisterClassification classify,
                                                                                           bool asLvalue)
    {
      if (asLvalue) return RegisterPtr();
      
      SequenceBuilder& seq (*(block.GetSequenceBuilder()));

      // Set up registers for operand values
      auto valueType = GetValueType();
      if (!valueType) return RegisterPtr(); // Assume error already handled
      auto operandRegs = GetSourceRegisters (block, valueType);
      auto reg1 = std::get<0> (*operandRegs).reg;
      auto reg2 = std::get<1> (*operandRegs).reg;

      RegisterPtr destination (handler->AllocateRegister (seq, valueType, classify));
      
      // Create actual sequence operation
      SequenceOpPtr seqOp;
      switch (op)
      {
      case And:
        seqOp = SequenceOpPtr (new SequenceOpLogic (destination, SequenceVisitor::And, reg1, reg2));
        break;
      case Or:
        seqOp = SequenceOpPtr (new SequenceOpLogic (destination, SequenceVisitor::Or, reg1, reg2));
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
