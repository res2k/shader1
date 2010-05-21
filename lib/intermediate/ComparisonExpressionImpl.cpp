#include "base/common.h"

#include "ComparisonExpressionImpl.h"

#include "BlockImpl.h"
#include "intermediate/Exception.h"
#include "intermediate/SequenceOp/SequenceOpCompareEq.h"
#include "intermediate/SequenceOp/SequenceOpCompareNE.h"
#include "intermediate/SequenceOp/SequenceOpCompareGE.h"
#include "intermediate/SequenceOp/SequenceOpCompareGT.h"
#include "intermediate/SequenceOp/SequenceOpCompareLE.h"
#include "intermediate/SequenceOp/SequenceOpCompareLT.h"
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
											 RegisterID destination)
    {
      Sequence& seq (*(block.GetSequence()));
      boost::shared_ptr<TypeImpl> type1 = operand1->GetValueType();
      boost::shared_ptr<TypeImpl> type2 = operand2->GetValueType();
      
      boost::shared_ptr<TypeImpl> comparisonType = handler->GetHigherPrecisionType (type1, type2);
	
      // Set up registers for operand values
      RegisterID reg1;
      reg1 = operand1->GetRegister (block, false);
      if (!reg1.IsValid())
      {
	reg1 = handler->AllocateRegister (seq, type1, Intermediate);
	operand1->AddToSequence (block, reg1);
      }
      if (!comparisonType->IsEqual (*(type1.get())))
      {
	// Insert cast op
	RegisterID newReg1 (handler->AllocateRegister (seq, comparisonType, Intermediate));
	handler->GenerateCast (seq, newReg1, comparisonType,
			       reg1, type1);
	reg1 = newReg1;
      }
      RegisterID reg2;
      reg2 = operand2->GetRegister (block, false);
      if (!reg2.IsValid())
      {
	reg2 = handler->AllocateRegister (seq, type2, Intermediate);
	operand2->AddToSequence (block, reg2);
      }
      if (!comparisonType->IsEqual (*(type2.get())))
      {
	// Insert cast op
	RegisterID newReg2 (handler->AllocateRegister (seq, comparisonType, Intermediate));
	handler->GenerateCast (seq, newReg2, comparisonType,
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
