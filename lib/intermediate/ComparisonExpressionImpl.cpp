#include "base/common.h"

#include "ComparisonExpressionImpl.h"

#include "BlockImpl.h"
#include "intermediate/Exception.h"
#include "intermediate/SequenceOp/SequenceOpCompare.h"
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
      
    IntermediateGeneratorSemanticsHandler::NameImplSet
    IntermediateGeneratorSemanticsHandler::ComparisonExpressionImpl::QueryWrittenNames (bool asLvalue)
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
    IntermediateGeneratorSemanticsHandler::ComparisonExpressionImpl::GetValueType()
    {
      // Comparison has always a bool result
      return handler->GetBoolType();
    }
    
    RegisterID IntermediateGeneratorSemanticsHandler::ComparisonExpressionImpl::AddToSequence (BlockImpl& block,
											       RegisterClassification classify,
											       bool asLvalue)
    {
      if (asLvalue) return RegisterID();
      
      Sequence& seq (*(block.GetSequence()));
      boost::shared_ptr<TypeImpl> type1 = operand1->GetValueType();
      boost::shared_ptr<TypeImpl> type2 = operand2->GetValueType();
      
      boost::shared_ptr<TypeImpl> comparisonType = handler->GetHigherPrecisionType (type1, type2);
	
      // Set up registers for operand values
      RegisterID orgReg1, reg1;
      orgReg1 = reg1 = operand1->AddToSequence (block, Intermediate);
      if (!comparisonType->IsEqual (*(type1.get())))
      {
	// Insert cast op
	RegisterID newReg1 (handler->AllocateRegister (seq, comparisonType, Intermediate));
	handler->GenerateCast (seq, newReg1, comparisonType,
			       reg1, type1);
	reg1 = newReg1;
      }
      RegisterID orgReg2, reg2;
      orgReg2 = reg2 = operand2->AddToSequence (block, Intermediate);
      if (!comparisonType->IsEqual (*(type2.get())))
      {
	// Insert cast op
	RegisterID newReg2 (handler->AllocateRegister (seq, comparisonType, Intermediate));
	handler->GenerateCast (seq, newReg2, comparisonType,
			       reg2, type2);
	reg2 = newReg2;
      }
      
      RegisterID destination (handler->AllocateRegister (*(block.GetSequence()), GetValueType(), classify));
      
      // Create actual sequence operation
      SequenceOpPtr seqOp;
      switch (op)
      {
      case Equals:
	seqOp = SequenceOpPtr (boost::make_shared<SequenceOpCompare> (destination, SequenceVisitor::Eq, reg1, reg2));
	break;
      case NotEquals:
	seqOp = SequenceOpPtr (boost::make_shared<SequenceOpCompare> (destination, SequenceVisitor::NE, reg1, reg2));
	break;
      case Smaller:
	seqOp = SequenceOpPtr (boost::make_shared<SequenceOpCompare> (destination, SequenceVisitor::LT, reg1, reg2));
	break;
      case SmallerEqual:
	seqOp = SequenceOpPtr (boost::make_shared<SequenceOpCompare> (destination, SequenceVisitor::LE, reg1, reg2));
	break;
      case Larger:
	seqOp = SequenceOpPtr (boost::make_shared<SequenceOpCompare> (destination, SequenceVisitor::GT, reg1, reg2));
	break;
      case LargerEqual:
	seqOp = SequenceOpPtr (boost::make_shared<SequenceOpCompare> (destination, SequenceVisitor::GE, reg1, reg2));
	break;
      }
      assert (seqOp);
      seq.AddOp (seqOp);
      
      operand1->AddToSequencePostAction (block, orgReg1, false);
      operand2->AddToSequencePostAction (block, orgReg2, false);
      
      return destination;
    }
  } // namespace intermediate
} // namespace s1
