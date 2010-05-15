#include "base/common.h"

#include "AssignmentExpressionImpl.h"

#include "BlockImpl.h"
#include "intermediate/Exception.h"
#include "intermediate/SequenceOp/SequenceOpAssign.h"
#include "TypeImpl.h"

namespace s1
{
  namespace intermediate
  {
    IntermediateGeneratorSemanticsHandler::AssignmentExpressionImpl::AssignmentExpressionImpl (
      IntermediateGeneratorSemanticsHandler* handler,
      const boost::shared_ptr<ExpressionImpl>& target,
      const boost::shared_ptr<ExpressionImpl>& value)
       : ExpressionImpl (handler), target (target), value (value)
    {
    }
      
    boost::shared_ptr<IntermediateGeneratorSemanticsHandler::TypeImpl>
    IntermediateGeneratorSemanticsHandler::AssignmentExpressionImpl::GetValueType()
    {
      boost::shared_ptr<TypeImpl> targetType = target->GetValueType();
      boost::shared_ptr<TypeImpl> valueType = value->GetValueType();

      if (!valueType->CompatibleLossy (*(targetType.get())))
      {
	throw Exception (AssignmentTypesIncompatible);
      }
      
      return targetType;
    }
    
    void IntermediateGeneratorSemanticsHandler::AssignmentExpressionImpl::AddToSequence (BlockImpl& block)
    {
      AddToSequence (block, RegisterID ());
    }
    
    void IntermediateGeneratorSemanticsHandler::AssignmentExpressionImpl::AddToSequence (BlockImpl& block,
											 RegisterID destination)
    {
      Sequence& seq (*(block.GetSequence()));
      boost::shared_ptr<TypeImpl> targetType = target->GetValueType();
      boost::shared_ptr<TypeImpl> valueType = value->GetValueType();

      /* 'Prefetch' right-side registers
         (to make cases like 'a = a OP b' work, right side 'a' needs to be 'older'
         than left-side 'a') */
      value->GetRegister (block, false);
      // Set up registers for left-side value
      RegisterID targetReg;
      targetReg = target->GetRegister (block, true);
      if (!targetReg.IsValid())
      {
	// GetRegister() should only work on L-values
	throw Exception (AssignmentTargetIsNotAnLValue);
      }
      RegisterID exprDestinationReg;
      if (!valueType->IsEqual (*(targetType.get())))
      {
	Sequence::RegisterPtr targetRegPtr (seq.QueryRegisterPtrFromID (targetReg));
	// Let right-side evaluate to a (temp) register, for cast
	exprDestinationReg = handler->AllocateRegister (seq, valueType, Intermediate,
							targetRegPtr->GetName());
      }
      else
	exprDestinationReg = targetReg;
      // Instruct 'value' to write to exprDestinationReg
      value->AddToSequence (block, exprDestinationReg);
      if (targetReg != exprDestinationReg)
      {
	// Insert cast op from exprDestinationReg to targetReg
	handler->GenerateCast (seq, targetReg, targetType,
			       exprDestinationReg, valueType);
      }
      
      if (destination.IsValid())
      {
	// Generate another assignment from targetReg to destination
	SequenceOpPtr seqOp;
	seqOp = SequenceOpPtr (new SequenceOpAssign (destination, targetReg));
	seq.AddOp (seqOp);
      }
    }
  } // namespace intermediate
} // namespace s1
