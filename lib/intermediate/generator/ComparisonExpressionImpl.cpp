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
#include "intermediate/Exception.h"
#include "intermediate/SequenceBuilder.h"
#include "intermediate/SequenceOp/SequenceOpCompare.h"
#include "TypeImpl.h"

#include <boost/make_shared.hpp>

namespace s1
{
  namespace intermediate
  {
    IntermediateGeneratorSemanticsHandler::ComparisonExpressionImpl::ComparisonExpressionImpl (
      IntermediateGeneratorSemanticsHandler* handler,
      CompareOp op,
      const boost::shared_ptr<ExpressionImpl>& operand1,
      const boost::shared_ptr<ExpressionImpl>& operand2)
       : ExpressionImpl (handler), op (op), operand1 (operand1), operand2 (operand2)
    {
    }
      
    IntermediateGeneratorSemanticsHandler::NameImplSet
    IntermediateGeneratorSemanticsHandler::ComparisonExpressionImpl::QueryWrittenNames (bool asLvalue)
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
      
    boost::shared_ptr<IntermediateGeneratorSemanticsHandler::TypeImpl>
    IntermediateGeneratorSemanticsHandler::ComparisonExpressionImpl::GetValueType()
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
      boost::shared_ptr<TypeImpl> type1 = operand1->GetValueType();
      boost::shared_ptr<TypeImpl> type2 = operand2->GetValueType();
      
      boost::shared_ptr<TypeImpl> comparisonType = handler->GetHigherPrecisionType (type1, type2);
        
      // Set up registers for operand values
      RegisterPtr orgReg1, reg1;
      orgReg1 = reg1 = operand1->AddToSequence (block, Intermediate);
      if (!comparisonType->IsEqual (*(type1.get())))
      {
        // Insert cast op
        RegisterPtr newReg1 (handler->AllocateRegister (seq, comparisonType, Intermediate));
        handler->GenerateCast (seq, newReg1, comparisonType,
                               reg1, type1);
        reg1 = newReg1;
      }
      RegisterPtr orgReg2, reg2;
      orgReg2 = reg2 = operand2->AddToSequence (block, Intermediate);
      if (!comparisonType->IsEqual (*(type2.get())))
      {
        // Insert cast op
        RegisterPtr newReg2 (handler->AllocateRegister (seq, comparisonType, Intermediate));
        handler->GenerateCast (seq, newReg2, comparisonType,
                               reg2, type2);
        reg2 = newReg2;
      }
      
      RegisterPtr destination (handler->AllocateRegister (seq, GetValueType(), classify));
      
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
      
      operand1->AddToSequencePostAction (block, orgReg1, false);
      operand2->AddToSequencePostAction (block, orgReg2, false);
      
      return destination;
    }
  } // namespace intermediate
} // namespace s1
