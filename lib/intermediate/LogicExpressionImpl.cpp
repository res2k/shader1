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
      
    IntermediateGeneratorSemanticsHandler::NameImplSet
    IntermediateGeneratorSemanticsHandler::LogicExpressionImpl::QueryWrittenNames (bool asLvalue)
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
    
    RegisterID IntermediateGeneratorSemanticsHandler::LogicExpressionImpl::AddToSequence (BlockImpl& block,
											  RegisterClassification classify,
											  const UnicodeString& name,
											  bool asLvalue)
    {
      if (asLvalue) return RegisterID();
      
      Sequence& seq (*(block.GetSequence()));
	
      // Set up registers for operand values
      RegisterID reg1;
      reg1 = operand1->AddToSequence (block, Intermediate);
      RegisterID reg2;
      reg2 = operand2->AddToSequence (block, Intermediate);
      
      RegisterID destination (handler->AllocateRegister (*(block.GetSequence()), GetValueType(), classify, name));
      
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
      
      return destination;
    }
  } // namespace intermediate
} // namespace s1
