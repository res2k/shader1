#include "base/common.h"

#include "UnaryExpressionImpl.h"

#include "BlockImpl.h"
#include "intermediate/Exception.h"
#include "intermediate/SequenceOp/SequenceOpUnaryInv.h"
#include "intermediate/SequenceOp/SequenceOpUnaryNeg.h"
#include "intermediate/SequenceOp/SequenceOpUnaryNot.h"
#include "TypeImpl.h"

#include <boost/make_shared.hpp>

namespace s1
{
  namespace intermediate
  {
    IntermediateGeneratorSemanticsHandler::UnaryExpressionImpl::UnaryExpressionImpl (
			   IntermediateGeneratorSemanticsHandler* handler,
			   UnaryOp op,
			   const boost::shared_ptr<ExpressionImpl>& operand)
       : ExpressionImpl (handler), op (op), operand (operand)
    {
    }

    RegisterID IntermediateGeneratorSemanticsHandler::UnaryExpressionImpl::GetRegister (BlockImpl& block,
											bool writeable)
    {
      // Let operand grab registers ...
      operand->GetRegister (block, false);
      return RegisterID ();
    }
      
    void IntermediateGeneratorSemanticsHandler::UnaryExpressionImpl::InvalidateRegister ()
    {
      operand->InvalidateRegister ();
    }
      
    IntermediateGeneratorSemanticsHandler::NameImplSet
    IntermediateGeneratorSemanticsHandler::UnaryExpressionImpl::QueryWrittenNames (bool asLvalue)
    {
      return operand->QueryWrittenNames (asLvalue);
    }
      
    boost::shared_ptr<IntermediateGeneratorSemanticsHandler::TypeImpl>
    IntermediateGeneratorSemanticsHandler::UnaryExpressionImpl::GetValueType()
    {
      boost::shared_ptr<TypeImpl> operandType = operand->GetValueType();
      boost::shared_ptr<TypeImpl> valueType;
      
      switch (op)
      {
      case Neg:
	if (operandType->IsEqual (*(handler->GetFloatType()))
	    || operandType->IsEqual (*(handler->GetIntType())))
	  valueType = operandType;
	else if (operandType->IsEqual (*(handler->GetUintType())))
	  valueType = handler->GetIntType();
	else
	  throw Exception (OperandTypesInvalid);
	break;
      case Inv:
	if (operandType->IsEqual (*(handler->GetUintType()))
	    || operandType->IsEqual (*(handler->GetIntType())))
	  valueType = operandType;
	else
	  throw Exception (OperandTypesInvalid);
	break;
      case Not:
	if (operandType->IsEqual (*(handler->GetBoolType())))
	  valueType = operandType;
	else
	  throw Exception (OperandTypesInvalid);
	break;
      }
      
      return valueType;
    }
    
    void IntermediateGeneratorSemanticsHandler::UnaryExpressionImpl::AddToSequence (BlockImpl& block,
										    RegisterID destination)
    {
      Sequence& seq (*(block.GetSequence()));
      boost::shared_ptr<TypeImpl> operandType = operand->GetValueType();
      
      boost::shared_ptr<TypeImpl> valueType = GetValueType ();

      // Set up register for operand value
      RegisterID reg;
      reg = operand->GetRegister (block, false);
      if (!reg.IsValid())
      {
	reg = handler->AllocateRegister (seq, operandType, Intermediate);
	operand->AddToSequence (block, reg);
      }
      if (!valueType->IsEqual (*(operandType.get())))
      {
	// Insert cast op
	RegisterID newReg (handler->AllocateRegister (seq, valueType, Intermediate));
	handler->GenerateCast (seq, newReg, valueType,
			       reg, operandType);
	reg = newReg;
      }
      
      // Create actual sequence operation
      SequenceOpPtr seqOp;
      switch (op)
      {
      case Neg:
	seqOp = SequenceOpPtr (boost::make_shared<SequenceOpUnaryNeg> (destination, reg));
	break;
      case Inv:
	seqOp = SequenceOpPtr (boost::make_shared<SequenceOpUnaryInv> (destination, reg));
	break;
      case Not:
	seqOp = SequenceOpPtr (boost::make_shared<SequenceOpUnaryNot> (destination, reg));
	break;
      }
      assert (seqOp);
      seq.AddOp (seqOp);
    }
  } // namespace intermediate
} // namespace s1
