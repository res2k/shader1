#include "ComparisonExpressionImpl.h"

#include "intermediate/Exception.h"
#include "intermediate/SequenceOpCompareEq.h"
#include "intermediate/SequenceOpCompareNE.h"
#include "intermediate/SequenceOpCompareGE.h"
#include "intermediate/SequenceOpCompareGT.h"
#include "intermediate/SequenceOpCompareLE.h"
#include "intermediate/SequenceOpCompareLT.h"
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
      
    boost::shared_ptr<IntermediateGeneratorSemanticsHandler::TypeImpl>
    IntermediateGeneratorSemanticsHandler::ComparisonExpressionImpl::GetValueType()
    {
      // Comparison has always a bool result
      return handler->GetBoolType();
    }
    
    void IntermediateGeneratorSemanticsHandler::ComparisonExpressionImpl::AddToSequence (BlockImpl& block,
											 Sequence& seq,
											 Sequence::RegisterID destination)
    {
      boost::shared_ptr<TypeImpl> type1 = operand1->GetValueType();
      boost::shared_ptr<TypeImpl> type2 = operand2->GetValueType();
      
      boost::shared_ptr<TypeImpl> valueType = GetValueType ();
	
      // Set up registers for operand values
      Sequence::RegisterID reg1;
      reg1 = operand1->GetRegister (seq, false);
      if (!reg1.IsValid())
      {
	reg1 = handler->AllocateRegister (seq, type1, Intermediate);
	operand1->AddToSequence (block, seq, reg1);
      }
      if (!valueType->IsEqual (*(type1.get())))
      {
	// Insert cast op
	Sequence::RegisterID newReg1 (handler->AllocateRegister (seq, valueType, Intermediate));
	handler->GenerateCast (seq, newReg1, valueType,
			       reg1, type1);
	reg1 = newReg1;
      }
      Sequence::RegisterID reg2;
      reg2 = operand1->GetRegister (seq, false);
      if (!reg2.IsValid())
      {
	reg2 = handler->AllocateRegister (seq, type2, Intermediate);
	operand2->AddToSequence (block, seq, reg2);
      }
      if (!valueType->IsEqual (*(type2.get())))
      {
	// Insert cast op
	Sequence::RegisterID newReg2 (handler->AllocateRegister (seq, valueType, Intermediate));
	handler->GenerateCast (seq, newReg2, valueType,
			       reg2, type2);
	reg2 = newReg2;
      }
      
      // Create actual sequence operation
      SequenceOpPtr seqOp;
      switch (op)
      {
      case Equals:
	seqOp = SequenceOpPtr (boost::make_shared<SequenceOpCompareEq> (destination, reg1, reg2));
	break;
      case NotEquals:
	seqOp = SequenceOpPtr (boost::make_shared<SequenceOpCompareNE> (destination, reg1, reg2));
	break;
      case Smaller:
	seqOp = SequenceOpPtr (boost::make_shared<SequenceOpCompareLT> (destination, reg1, reg2));
	break;
      case SmallerEqual:
	seqOp = SequenceOpPtr (boost::make_shared<SequenceOpCompareLE> (destination, reg1, reg2));
	break;
      case Larger:
	seqOp = SequenceOpPtr (boost::make_shared<SequenceOpCompareGT> (destination, reg1, reg2));
	break;
      case LargerEqual:
	seqOp = SequenceOpPtr (boost::make_shared<SequenceOpCompareGE> (destination, reg1, reg2));
	break;
      }
      assert (seqOp);
      seq.AddOp (seqOp);
    }
  } // namespace intermediate
} // namespace s1
