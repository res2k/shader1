#include "base/common.h"

#include "AttributeExpressionImpl.h"

#include "intermediate/SequenceOp/SequenceOpExtractVectorComponent.h"
#include "intermediate/SequenceOp/SequenceOpMakeVector.h"

#include "BlockImpl.h"
#include "TypeImpl.h"

#include <boost/make_shared.hpp>

namespace s1
{
  namespace intermediate
  {
    IntermediateGeneratorSemanticsHandler::AttributeExpressionImpl::AttributeExpressionImpl (IntermediateGeneratorSemanticsHandler* handler,
											     const ExpressionPtr& baseExpr,
											     const IntermediateGeneratorSemanticsHandler::Attribute& attr)
     : ExpressionImpl (handler), baseExpr (baseExpr), attr (attr)
    {}
    
    IntermediateGeneratorSemanticsHandler::TypeImplPtr
    IntermediateGeneratorSemanticsHandler::AttributeExpressionImpl::GetValueType ()
    {
      boost::shared_ptr<ExpressionImpl> exprImpl (boost::shared_static_cast<ExpressionImpl> (baseExpr));
      return handler->GetAttributeType (exprImpl->GetValueType(), attr);
    }
    
    RegisterID IntermediateGeneratorSemanticsHandler::AttributeExpressionImpl::AddToSequence (BlockImpl& block,
											      RegisterClassification classify,
											      bool asLvalue)
    {
      Sequence& seq (*(block.GetSequence()));
      
      switch (attr.attrClass)
      {
      default:
      case IntermediateGeneratorSemanticsHandler::Attribute::Unknown:
	assert (false);
	return RegisterID ();
      case IntermediateGeneratorSemanticsHandler::Attribute::arrayLength:
      case IntermediateGeneratorSemanticsHandler::Attribute::matrixRow:
      case IntermediateGeneratorSemanticsHandler::Attribute::matrixCol:
      case IntermediateGeneratorSemanticsHandler::Attribute::matrixTranspose:
      case IntermediateGeneratorSemanticsHandler::Attribute::matrixInvert:
	{
	  if (asLvalue) return RegisterID ();
	  
	  return RegisterID ();
	}
	break;
      case IntermediateGeneratorSemanticsHandler::Attribute::vectorSwizzle:
	{
	  if (asLvalue)
	  {
	    // TODO: assignment to vector components
	  }
	  else
	  {
	    boost::shared_ptr<ExpressionImpl> exprImpl (boost::shared_static_cast<ExpressionImpl> (baseExpr));
	    RegisterID exprValueReg (exprImpl->AddToSequence (block, Intermediate, false));
	    
	    TypeImplPtr valueType (GetValueType());
	    RegisterID targetReg;;
	    if (attr.swizzleCompNum > 1)
	    {
	      // multi-component swizzle
	      targetReg = handler->AllocateRegister (seq, valueType, classify);
	      TypeImplPtr valueCompType (boost::shared_static_cast<TypeImpl> (valueType->avmBase));
	      std::vector<RegisterID> compRegs;
	      for (unsigned int c = 0; c < attr.swizzleCompNum; c++)
	      {
		RegisterID compReg (handler->AllocateRegister (seq, valueCompType, classify));
		SequenceOpPtr seqOp (boost::make_shared<SequenceOpExtractVectorComponent> (compReg,
											  exprValueReg,
											  attr.GetSwizzleComp (c)));
		seq.AddOp (seqOp);
		compRegs.push_back (compReg);
	      }
	      BasicType vecType;
	      switch (valueCompType->base)
	      {
	      case Bool: 	vecType = intermediate::Bool; break;
	      case Int: 	vecType = intermediate::Int; break;
	      case UInt: 	vecType = intermediate::UInt; break;
	      case Float: vecType = intermediate::Float; break;
	      default:	return RegisterID();
	      }
	      SequenceOpPtr seqOp (boost::make_shared<SequenceOpMakeVector> (targetReg, vecType, compRegs));
	      seq.AddOp (seqOp);
	    }
	    else
	    {
	      RegisterID compReg (handler->AllocateRegister (seq, valueType, classify));
	      SequenceOpPtr seqOp (boost::make_shared<SequenceOpExtractVectorComponent> (compReg,
											 exprValueReg,
											 attr.GetSwizzleComp (0)));
	      seq.AddOp (seqOp);
	      targetReg = compReg;
	    }
	    return targetReg;
	  }
	}
	break;
      }
    }

  } // namespace intermediate
} // namespace s1
