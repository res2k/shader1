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
      
    IntermediateGeneratorSemanticsHandler::NameImplSet
    IntermediateGeneratorSemanticsHandler::AssignmentExpressionImpl::QueryWrittenNames (bool asLvalue)
    {
      NameImplSet set;
      NameImplSet targetSet (target->QueryWrittenNames (true));
      set.insert (targetSet.begin(), targetSet.end());
      NameImplSet valueSet (value->QueryWrittenNames (false));
      set.insert (valueSet.begin(), valueSet.end());
      return set;
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
      AddToSequence (block, Dummy);
    }
    
    RegisterID IntermediateGeneratorSemanticsHandler::AssignmentExpressionImpl::AddToSequence (BlockImpl& block,
											       RegisterClassification classify,
											       const UnicodeString& name,
											       bool asLvalue)
    {
      if (asLvalue) return RegisterID ();
      
      Sequence& seq (*(block.GetSequence()));
      boost::shared_ptr<TypeImpl> targetType = target->GetValueType();
      boost::shared_ptr<TypeImpl> valueType = value->GetValueType();

      RegisterID exprDestinationReg;
      // Evaluate 'value'
      exprDestinationReg = value->AddToSequence (block, Intermediate);
      // Set up registers for left-side value
      RegisterID targetReg;
      targetReg = target->AddToSequence (block, Intermediate, UnicodeString(), true);
      if (!targetReg.IsValid())
      {
	// Can only assign an L value
	throw Exception (AssignmentTargetIsNotAnLValue);
      }
      if (!valueType->IsEqual (*(targetType.get())))
      {
	// Generate cast to targetReg
	handler->GenerateCast (seq, targetReg, targetType,
			       exprDestinationReg, valueType);
      }
      else
      {
	// Generate another assignment from exprDestinationReg to targetReg
	SequenceOpPtr seqOp;
	seqOp = SequenceOpPtr (new SequenceOpAssign (targetReg, exprDestinationReg));
	seq.AddOp (seqOp);
      }
      
      return targetReg;
    }
  } // namespace intermediate
} // namespace s1
