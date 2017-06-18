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

#include "BinaryExpressionImpl.h"

#include "BlockImpl.h"
#include "TypeImpl.h"

namespace s1
{
  namespace intermediate
  {
    IntermediateGeneratorSemanticsHandler::BinaryExpressionImpl::BinaryExpressionImpl (
      IntermediateGeneratorSemanticsHandler* handler,
      ExpressionContext&& context,
      const boost::shared_ptr<ExpressionImpl>& operand1,
      const boost::shared_ptr<ExpressionImpl>& operand2)
       : ExpressionImpl (handler, std::move (context)), operand1 (operand1), operand2 (operand2)
    {
    }

    IntermediateGeneratorSemanticsHandler::NameImplSet
    IntermediateGeneratorSemanticsHandler::BinaryExpressionImpl::QueryWrittenNames (bool asLvalue)
    {
      NameImplSet set;
      {
        NameImplSet op1Set (operand1->QueryWrittenNames (asLvalue));
        set.insert (op1Set.begin(), op1Set.end());
      }
      {
        NameImplSet op2Set (operand2->QueryWrittenNames (asLvalue));
        set.insert (op2Set.begin(), op2Set.end());
      }
      return set;
    }

    boost::optional<IntermediateGeneratorSemanticsHandler::BinaryExpressionImpl::RegisterPtrTuple>
    IntermediateGeneratorSemanticsHandler::BinaryExpressionImpl::GetSourceRegisters (BlockImpl& block,
                                                                                     const TypeImplPtr& asType)
    {
      SequenceBuilder& seq (*(block.GetSequenceBuilder()));
      boost::shared_ptr<TypeImpl> type1 = operand1->GetValueType();
      boost::shared_ptr<TypeImpl> type2 = operand2->GetValueType();
      if (!type1 || !type2) return boost::none; // Assume error already handled

      // Set up registers for operand values
      OperandReg r1 = operand1->AddToSequence (block, Intermediate);
      OperandReg r2 = operand2->AddToSequence (block, Intermediate);
      if (!r1.reg || !r2.reg) return boost::none; // Assume error already handled

      if (!asType->IsEqual (*(type1.get())))
      {
        // Insert cast op
        RegisterPtr newReg1 (handler->AllocateRegister (seq, asType, Intermediate));
        handler->GenerateCast (seq, newReg1, asType,
                               r1.reg, type1);
        r1.reg = newReg1;
      }
      if (!asType->IsEqual (*(type2.get())))
      {
        // Insert cast op
        RegisterPtr newReg2 (handler->AllocateRegister (seq, asType, Intermediate));
        handler->GenerateCast (seq, newReg2, asType,
                               r2.reg, type2);
        r2.reg = newReg2;
      }
      return std::make_tuple (r1, r2);
    }
  } // namespace intermediate
} // namespace s1
