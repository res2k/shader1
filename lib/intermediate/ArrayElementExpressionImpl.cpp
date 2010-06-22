#include "base/common.h"

#include "ArrayElementExpressionImpl.h"

#include "intermediate/Exception.h"
#include "intermediate/SequenceOp/SequenceOpExtractArrayElement.h"

#include "BlockImpl.h"
#include "TypeImpl.h"

#include <boost/make_shared.hpp>

namespace s1
{
  namespace intermediate
  {
    IntermediateGeneratorSemanticsHandler::ArrayElementExpressionImpl::ArrayElementExpressionImpl (IntermediateGeneratorSemanticsHandler* handler,
												   const ExpressionPtr& arrayExpr,
												   const ExpressionPtr& indexExpr)
     : ExpressionImpl (handler), arrayExpr (arrayExpr), indexExpr (indexExpr)
    {}
    
    IntermediateGeneratorSemanticsHandler::TypeImplPtr
    IntermediateGeneratorSemanticsHandler::ArrayElementExpressionImpl::GetValueType ()
    {
      boost::shared_ptr<ExpressionImpl> exprImpl (boost::shared_static_cast<ExpressionImpl> (arrayExpr));
      TypeImplPtr exprType (exprImpl->GetValueType());
      if (exprType->typeClass != TypeImpl::Array)
	throw Exception (NotAnArray);
      return boost::shared_static_cast<TypeImpl> (exprType->avmBase);
    }
    
    RegisterID IntermediateGeneratorSemanticsHandler::ArrayElementExpressionImpl::AddToSequence (BlockImpl& block,
												 RegisterClassification classify,
												 bool asLvalue)
    {
      Sequence& seq (*(block.GetSequence()));

      boost::shared_ptr<ExpressionImpl> arrayExprImpl (boost::shared_static_cast<ExpressionImpl> (arrayExpr));
      boost::shared_ptr<ExpressionImpl> indexExprImpl (boost::shared_static_cast<ExpressionImpl> (indexExpr));
      
      TypeImplPtr indexType (indexExprImpl->GetValueType());
      if (!indexType->CompatibleLossless (*(handler->GetUintType())))
	throw Exception (IndexNotAnInteger);
      
      RegisterID indexReg (indexExprImpl->AddToSequence (block, Index));
      if (!indexType->IsEqual (*(handler->GetUintType())))
      {
	RegisterID newIndexReg (handler->AllocateRegister (seq, handler->GetUintType(), Index));
	handler->GenerateCast (seq, newIndexReg, handler->GetUintType(), indexReg, indexType);
	indexReg = newIndexReg;
      }
      
      RegisterID targetReg (handler->AllocateRegister (seq, GetValueType(), classify));
      RegisterID arrayReg (arrayExprImpl->AddToSequence (block, Intermediate, asLvalue));
      
      // May happen if array value is not an L-value
      if (!arrayReg.IsValid()) return RegisterID ();
      
      if (asLvalue)
      {
	// TODO: assignment to array element
	return RegisterID ();
      }
      else
      {
	SequenceOpPtr seqOp (boost::make_shared<SequenceOpExtractArrayElement> (targetReg, arrayReg, indexReg));
	seq.AddOp (seqOp);
	return targetReg;
      }
      
    }

  } // namespace intermediate
} // namespace s1
