#include "ArithmeticExpressionImpl.h"

#include "intermediate/Exception.h"
#include "TypeImpl.h"

namespace s1
{
  namespace intermediate
  {
    IntermediateGeneratorSemanticsHandler::ArithmeticExpressionImpl::ArithmeticExpressionImpl (
      IntermediateGeneratorSemanticsHandler* handler,
      ArithmeticOp op,
      const boost::shared_ptr<ExpressionImpl>& operand1,
      const boost::shared_ptr<ExpressionImpl>& operand2)
       : ExpressionImpl (handler), op (op), operand1 (operand1), operand2 (operand2)
    {
    }
      
    boost::shared_ptr<IntermediateGeneratorSemanticsHandler::TypeImpl>
    IntermediateGeneratorSemanticsHandler::ArithmeticExpressionImpl::GetValueType()
    {
      boost::shared_ptr<TypeImpl> type1 = operand1->GetValueType();
      boost::shared_ptr<TypeImpl> type2 = operand2->GetValueType();
      
      if (!type1->CompatibleLossy (*(handler->GetFloatType().get()))
	|| !type2->CompatibleLossy (*(handler->GetFloatType().get())))
      {
	throw Exception (OperandTypesInvalid);
      }
      
      // Determine type in which to perform computation
      boost::shared_ptr<TypeImpl> valueType = IntermediateGeneratorSemanticsHandler::GetHigherPrecisionType (
	operand1->GetValueType(), operand2->GetValueType());
	
      if (!valueType)
      {
	throw Exception (OperandTypesIncompatible);
      }
      
      return valueType;
    }
    
    void IntermediateGeneratorSemanticsHandler::ArithmeticExpressionImpl::AddToSequence (BlockImpl& block,
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
	reg1 = handler->AllocateRegister (seq, valueType, Intermediate);
	operand1->AddToSequence (block, seq, reg1);
      }
      if (!valueType->IsEqual (*(type1.get())))
      {
	// Insert cast op
      }
      Sequence::RegisterID reg2;
      reg2 = operand1->GetRegister (seq, false);
      if (!reg2.IsValid())
      {
	reg2 = handler->AllocateRegister (seq, valueType, Intermediate);
	operand2->AddToSequence (block, seq, reg2);
      }
      if (!valueType->IsEqual (*(type2.get())))
      {
	// Insert cast op
      }
      
      // Create actual sequence operation
      // SequenceOpArithmetic seqOp (op, reg1, reg2);
      // seqOp.SetDestination (destination);
      // seq.AddOp (seq);
    }
  } // namespace intermediate
} // namespace s1
