#include "base/common.h"

#include "ArithmeticExpressionImpl.h"

#include "BlockImpl.h"
#include "intermediate/Exception.h"
#include "intermediate/SequenceOp/SequenceOpArithAdd.h"
#include "intermediate/SequenceOp/SequenceOpArithDiv.h"
#include "intermediate/SequenceOp/SequenceOpArithMod.h"
#include "intermediate/SequenceOp/SequenceOpArithMul.h"
#include "intermediate/SequenceOp/SequenceOpArithSub.h"
#include "TypeImpl.h"

#include <boost/make_shared.hpp>

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

    RegisterID IntermediateGeneratorSemanticsHandler::ArithmeticExpressionImpl::GetRegister (BlockImpl& block,
											     bool writeable)
    {
      // Let operands grab registers ...
      operand1->GetRegister (block, false);
      operand2->GetRegister (block, false);
      return RegisterID ();
    }
      
    IntermediateGeneratorSemanticsHandler::NameImplSet
    IntermediateGeneratorSemanticsHandler::ArithmeticExpressionImpl::QueryWrittenNames (bool asLvalue)
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
    
    RegisterID IntermediateGeneratorSemanticsHandler::ArithmeticExpressionImpl::AddToSequence (BlockImpl& block,
											       RegisterClassification classify,
											       const UnicodeString& name,
											       bool asLvalue)
    {
      if (asLvalue) return RegisterID();
      
      Sequence& seq (*(block.GetSequence()));
      boost::shared_ptr<TypeImpl> type1 = operand1->GetValueType();
      boost::shared_ptr<TypeImpl> type2 = operand2->GetValueType();
      
      boost::shared_ptr<TypeImpl> valueType = GetValueType ();
	
      // Set up registers for operand values
      RegisterID reg1;
      reg1 = operand1->GetRegister (block, false);
      if (!reg1.IsValid())
      {
	reg1 = operand1->AddToSequence (block, Intermediate);
      }
      if (!valueType->IsEqual (*(type1.get())))
      {
	// Insert cast op
	RegisterID newReg1 (handler->AllocateRegister (seq, valueType, Intermediate));
	handler->GenerateCast (seq, newReg1, valueType,
			       reg1, type1);
	reg1 = newReg1;
      }
      RegisterID reg2;
      reg2 = operand2->GetRegister (block, false);
      if (!reg2.IsValid())
      {
	reg2 = operand2->AddToSequence (block, Intermediate);
      }
      if (!valueType->IsEqual (*(type2.get())))
      {
	// Insert cast op
	RegisterID newReg2 (handler->AllocateRegister (seq, valueType, Intermediate));
	handler->GenerateCast (seq, newReg2, valueType,
			       reg2, type2);
	reg2 = newReg2;
      }
      
      RegisterID destination (handler->AllocateRegister (*(block.GetSequence()), GetValueType(), classify, name));
      
      // Create actual sequence operation
      SequenceOpPtr seqOp;
      switch (op)
      {
      case Add:
	seqOp = SequenceOpPtr (boost::make_shared<SequenceOpArithAdd> (destination, reg1, reg2));
	break;
      case Sub:
	seqOp = SequenceOpPtr (boost::make_shared<SequenceOpArithSub> (destination, reg1, reg2));
	break;
      case Mul:
	seqOp = SequenceOpPtr (boost::make_shared<SequenceOpArithMul> (destination, reg1, reg2));
	break;
      case Div:
	seqOp = SequenceOpPtr (boost::make_shared<SequenceOpArithDiv> (destination, reg1, reg2));
	break;
      case Mod:
	seqOp = SequenceOpPtr (boost::make_shared<SequenceOpArithMod> (destination, reg1, reg2));
	break;
      }
      assert (seqOp);
      seq.AddOp (seqOp);
      
      return destination;
    }
  } // namespace intermediate
} // namespace s1
