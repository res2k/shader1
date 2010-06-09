#include "base/common.h"

#include "TernaryExpressionImpl.h"

#include "BlockImpl.h"
#include "intermediate/Exception.h"
/*#include "intermediate/SequenceOp/SequenceOpArithAdd.h"
#include "intermediate/SequenceOp/SequenceOpArithDiv.h"
#include "intermediate/SequenceOp/SequenceOpArithMod.h"
#include "intermediate/SequenceOp/SequenceOpArithMul.h"
#include "intermediate/SequenceOp/SequenceOpArithSub.h"*/
#include "TypeImpl.h"

#include <boost/make_shared.hpp>

namespace s1
{
  namespace intermediate
  {
    IntermediateGeneratorSemanticsHandler::TernaryExpressionImpl::TernaryExpressionImpl (
      IntermediateGeneratorSemanticsHandler* handler,
      const boost::shared_ptr<ExpressionImpl>& condition,
      const boost::shared_ptr<ExpressionImpl>& ifExpr,
      const boost::shared_ptr<ExpressionImpl>& elseExpr)
       : ExpressionImpl (handler), condition (condition), ifExpr (ifExpr), elseExpr (elseExpr)
    {
    }

    RegisterID IntermediateGeneratorSemanticsHandler::TernaryExpressionImpl::GetRegister (BlockImpl& block,
											  bool writeable)
    {
      // Let operands grab registers ...
      condition->GetRegister (block, false);
      ifExpr->GetRegister (block, false);
      elseExpr->GetRegister (block, false);
      return RegisterID ();
    }
      
    void IntermediateGeneratorSemanticsHandler::TernaryExpressionImpl::InvalidateRegister ()
    {
      condition->InvalidateRegister ();
      ifExpr->InvalidateRegister ();
      elseExpr->InvalidateRegister ();
    }
      
    IntermediateGeneratorSemanticsHandler::NameImplSet
    IntermediateGeneratorSemanticsHandler::TernaryExpressionImpl::QueryWrittenNames (bool asLvalue)
    {
      NameImplSet set;
      {
	NameImplSet op1Set (ifExpr->QueryWrittenNames (asLvalue));
	set.insert (op1Set.begin(), op1Set.end());
      }
      {
	NameImplSet op2Set (elseExpr->QueryWrittenNames (asLvalue));
	set.insert (op2Set.begin(), op2Set.end());
      }
      return set;
    }
      
    boost::shared_ptr<IntermediateGeneratorSemanticsHandler::TypeImpl>
    IntermediateGeneratorSemanticsHandler::TernaryExpressionImpl::GetValueType()
    {
      // Determine type in which to perform computation
      boost::shared_ptr<TypeImpl> valueType = IntermediateGeneratorSemanticsHandler::GetHigherPrecisionType (
	ifExpr->GetValueType(), elseExpr->GetValueType());
	
      if (!valueType)
      {
	throw Exception (TernaryExpressionTypesIncompatible);
      }
      
      return valueType;
    }
    
    void IntermediateGeneratorSemanticsHandler::TernaryExpressionImpl::AddToSequence (BlockImpl& block,
										      RegisterID destination)
    {
      /* Spec says: if condition is true, evaluate 'if' expression;
	 if condition is false, evaluate 'else' expression.
	 Between the lines, don't evaluate the _other_ expression in either case.
	 To get that effect, synthesize a branching op. */
      
      // Set up a Name object for the ternary op result
      Sequence::RegisterBankPtr destRegBank;
      Sequence::RegisterPtr destRegPtr (block.GetSequence()->QueryRegisterPtrFromID (destination, destRegBank));
      UnicodeString destNameStr (destRegPtr->GetName());
      destNameStr.append ("$tr");
      NamePtr destName (block.GetInnerScope()->AddVariable (destRegBank->GetOriginalType(), destNameStr, ExpressionPtr(), false));
      
      // Synthesize assignment for 'true' case
      BlockPtr ifBlock (handler->CreateBlock (block.GetInnerScope()));
      {
	ExpressionPtr destNameExpr (handler->CreateVariableExpression (destName));
	ExpressionPtr ifAssignExpr (handler->CreateAssignExpression (destNameExpr, ifExpr));
	ifBlock->AddExpressionCommand (ifAssignExpr);
      }
      // Synthesize assignment for 'false' case
      BlockPtr elseBlock (handler->CreateBlock (block.GetInnerScope()));
      {
	ExpressionPtr destNameExpr (handler->CreateVariableExpression (destName));
	ExpressionPtr elseAssignExpr (handler->CreateAssignExpression (destNameExpr, elseExpr));
	elseBlock->AddExpressionCommand (elseAssignExpr);
      }
      // Add branch operation to sequence
      block.AddBranching (condition, ifBlock, elseBlock);
      // Assign ternary result to destination
      {
	ExpressionPtr destNameExpr (handler->CreateVariableExpression (destName));
	boost::shared_ptr<ExpressionImpl> destNameExprImpl (boost::shared_static_cast<ExpressionImpl> (destNameExpr));
	destNameExprImpl->AddToSequence (block, destination);
      }
    }
  } // namespace intermediate
} // namespace s1
