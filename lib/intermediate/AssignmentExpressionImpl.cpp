#include "AssignmentExpressionImpl.h"

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
    
    void IntermediateGeneratorSemanticsHandler::AssignmentExpressionImpl::AddToSequence (BlockImpl& block,
											 Sequence& seq)
    {
      AddToSequence (block, seq, Sequence::RegisterID ());
    }
    
    void IntermediateGeneratorSemanticsHandler::AssignmentExpressionImpl::AddToSequence (BlockImpl& block,
											 Sequence& seq,
											 Sequence::RegisterID destination)
    {
      boost::shared_ptr<TypeImpl> targetType = target->GetValueType();
      boost::shared_ptr<TypeImpl> valueType = value->GetValueType();
	
      // Set up registers for left-side value
      Sequence::RegisterID targetReg;
      targetReg = target->GetRegister (seq, true);
      if (!targetReg.IsValid())
      {
	// GetRegister() should only work on L-values
	throw Exception (AssignmentTargetIsNotAnLValue);
      }
      Sequence::RegisterID exprDestinationReg;
      if (!valueType->IsEqual (*(targetType.get())))
      {
	// Let right-side evaluate to a (temp) register, for cast
	exprDestinationReg = handler->AllocateRegister (seq, valueType, Intermediate);
      }
      else
	exprDestinationReg = targetReg;
      // Instruct 'value' to write to exprDestinationReg
      value->AddToSequence (block, seq, exprDestinationReg);
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
