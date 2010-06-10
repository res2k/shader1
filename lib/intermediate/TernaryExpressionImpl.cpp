#include "base/common.h"

#include "TernaryExpressionImpl.h"

#include "BlockImpl.h"
#include "intermediate/Exception.h"
#include "TypeImpl.h"

#include <boost/make_shared.hpp>
#include <unicode/ustdio.h>

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
      // @@@ Somewhat fragile, relying on GetRegister() and AddToSequence() being called with the same block
      if (!ifBlock) ifBlock = handler->CreateBlock (block.GetInnerScope());
      if (!elseBlock) elseBlock = handler->CreateBlock (block.GetInnerScope());
      
      // Let operands grab registers ...
      condition->GetRegister (block, false);
      ifExpr->GetRegister (*(boost::shared_static_cast<BlockImpl> (ifBlock)), false);
      elseExpr->GetRegister (*(boost::shared_static_cast<BlockImpl> (elseBlock)), false);
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
    
    RegisterID IntermediateGeneratorSemanticsHandler::TernaryExpressionImpl::AddToSequence (BlockImpl& block,
											    RegisterClassification classify,
											    const UnicodeString& name,
											    bool asLvalue)
    {
      if (asLvalue) return RegisterID();
      
      /* Spec says: if condition is true, evaluate 'if' expression;
	 if condition is false, evaluate 'else' expression.
	 Between the lines, don't evaluate the _other_ expression in either case.
	 To get that effect, synthesize a branching op. */
      
      // Set up a Name object for the ternary op result
      static unsigned int ternaryResultNum = 0;
      UChar ternaryResultName[charsToFormatUint + 4];
      u_snprintf (ternaryResultName, sizeof (ternaryResultName)/sizeof (UChar),
		  "tr$%u", ternaryResultNum++); // FIXME: better name
      UnicodeString destNameStr (ternaryResultName);
      NamePtr destName (block.GetInnerScope()->AddVariable (GetValueType(), destNameStr, ExpressionPtr(), false));

      BlockPtr ifBlock (handler->CreateBlock (block.GetInnerScope()));
      // Synthesize assignment for 'true' case
      {
	ExpressionPtr destNameExpr (handler->CreateVariableExpression (destName));
	ExpressionPtr ifAssignExpr (handler->CreateAssignExpression (destNameExpr, ifExpr));
	ifBlock->AddExpressionCommand (ifAssignExpr);
      }
      BlockPtr elseBlock (handler->CreateBlock (block.GetInnerScope()));
      // Synthesize assignment for 'false' case
      {
	ExpressionPtr destNameExpr (handler->CreateVariableExpression (destName));
	ExpressionPtr elseAssignExpr (handler->CreateAssignExpression (destNameExpr, elseExpr));
	elseBlock->AddExpressionCommand (elseAssignExpr);
      }
      // Add branch operation to sequence
      condition->InvalidateRegister();
      block.AddBranching (condition, ifBlock, elseBlock);
      // Assign ternary result to destination
      {
	ExpressionPtr destNameExpr (handler->CreateVariableExpression (destName));
	boost::shared_ptr<ExpressionImpl> destNameExprImpl (boost::shared_static_cast<ExpressionImpl> (destNameExpr));
	return destNameExprImpl->AddToSequence (block, Intermediate);
      }
    }
  } // namespace intermediate
} // namespace s1
