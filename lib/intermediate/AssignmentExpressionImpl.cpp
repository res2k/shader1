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
											       bool asLvalue)
    {
      if (asLvalue) return RegisterID ();
      
      Sequence& seq (*(block.GetSequence()));
      boost::shared_ptr<TypeImpl> targetType = target->GetValueType();
      boost::shared_ptr<TypeImpl> valueType = value->GetValueType();

      RegisterID exprDestinationReg;
      // Evaluate 'value'
      exprDestinationReg = value->AddToSequence (block, Intermediate);
      NameImplPtr targetName (target->GetExpressionName());
      NameImplPtr valueName (value->GetExpressionName());
      if (!valueType->IsEqual (*(targetType.get())))
      {
	// Set up register for left-side value
	RegisterID targetReg (target->AddToSequence (block, Intermediate, true));
	// Generate cast to targetReg
	handler->GenerateCast (seq, targetReg, targetType,
			       exprDestinationReg, valueType);
			       
	value->AddToSequencePostAction (block, exprDestinationReg, false);
	target->AddToSequencePostAction (block, targetReg, true);
			       
	return targetReg;
      }
      else
      {
	if (targetName && !valueName)
	{
	  // See if we can 'force' the target register for the name's register
	  if (block.OverrideNameRegister (targetName, exprDestinationReg))
	  {
	    value->AddToSequencePostAction (block, exprDestinationReg, false);
	    return exprDestinationReg;
	  }
	}
	// Set up register for left-side value
	RegisterID targetReg (target->AddToSequence (block, Intermediate, true));
	// Generate another assignment from exprDestinationReg to targetReg
	SequenceOpPtr seqOp;
	seqOp = SequenceOpPtr (new SequenceOpAssign (targetReg, exprDestinationReg));
	seq.AddOp (seqOp);
	
	value->AddToSequencePostAction (block, exprDestinationReg, false);
	target->AddToSequencePostAction (block, targetReg, true);
	
	return targetReg;
      }      
    }
  } // namespace intermediate
} // namespace s1
