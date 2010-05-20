#include "base/common.h"

#include "LogicExpressionImpl.h"

#include "BlockImpl.h"
#include "intermediate/Exception.h"
#include "intermediate/SequenceOp/SequenceOpLogicAnd.h"
#include "intermediate/SequenceOp/SequenceOpLogicOr.h"
#include "TypeImpl.h"

#include <boost/make_shared.hpp>

namespace s1
{
  namespace intermediate
  {
    IntermediateGeneratorSemanticsHandler::LogicExpressionImpl::LogicExpressionImpl (
      IntermediateGeneratorSemanticsHandler* handler,
      LogicOp op,
      const boost::shared_ptr<ExpressionImpl>& operand1,
      const boost::shared_ptr<ExpressionImpl>& operand2)
       : ExpressionImpl (handler), op (op), operand1 (operand1), operand2 (operand2)
    {
    }
      
    boost::shared_ptr<IntermediateGeneratorSemanticsHandler::TypeImpl>
    IntermediateGeneratorSemanticsHandler::LogicExpressionImpl::GetValueType()
    {
      boost::shared_ptr<TypeImpl> type1 = operand1->GetValueType();
      boost::shared_ptr<TypeImpl> type2 = operand2->GetValueType();
      
      // Operands have to be bools
      if (!type1->IsEqual (*(handler->GetBoolType().get()))
	|| !type2->IsEqual (*(handler->GetBoolType().get())))
      {
	throw Exception (OperandTypesInvalid);
      }
      
      // Logic ops have always a bool result
      return handler->GetBoolType();
    }
    
    void IntermediateGeneratorSemanticsHandler::LogicExpressionImpl::AddToSequence (BlockImpl& block,
										    RegisterID destination)
    {
      Sequence& seq (*(block.GetSequence()));
	
      // Set up registers for operand values
      RegisterID reg1;
      reg1 = operand1->GetRegister (block, false);
      if (!reg1.IsValid())
      {
	reg1 = handler->AllocateRegister (seq, handler->GetBoolType(), Intermediate);
	operand1->AddToSequence (block, reg1);
      }
      RegisterID reg2;
      reg2 = operand1->GetRegister (block, false);
      if (!reg2.IsValid())
      {
	reg2 = handler->AllocateRegister (seq, handler->GetBoolType(), Intermediate);
	operand2->AddToSequence (block, reg2);
      }
      
      // Create actual sequence operation
      SequenceOpPtr seqOp;
      switch (op)
      {
      case And:
	seqOp = SequenceOpPtr (boost::make_shared<SequenceOpLogicAnd> (destination, reg1, reg2));
	break;
      case Or:
	seqOp = SequenceOpPtr (boost::make_shared<SequenceOpLogicOr> (destination, reg1, reg2));
	break;
      }
      assert (seqOp);
      seq.AddOp (seqOp);
    }
  } // namespace intermediate
} // namespace s1
