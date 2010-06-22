#include "base/common.h"

#include "AttributeExpressionImpl.h"

#include "intermediate/Exception.h"
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
	    RegisterID targetReg (handler->AllocateRegister (seq, GetValueType(), Intermediate));
	    return targetReg;
	  }
	  else
	  {
	    boost::shared_ptr<ExpressionImpl> exprImpl (boost::shared_static_cast<ExpressionImpl> (baseExpr));
	    RegisterID exprValueReg (exprImpl->AddToSequence (block, Intermediate, false));
	    
	    TypeImplPtr valueType (GetValueType());
	    RegisterID targetReg;
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

    void IntermediateGeneratorSemanticsHandler::AttributeExpressionImpl::AddToSequencePostAction (BlockImpl& block,
												  const RegisterID& target,
												  bool wasLvalue)
    {
      if (!wasLvalue) return;
      
      Sequence& seq (*(block.GetSequence()));
      
      // Generate assignment from register to actual target
      boost::shared_ptr<ExpressionImpl> exprImpl (boost::shared_static_cast<ExpressionImpl> (baseExpr));
      
      RegisterID originalTarget (exprImpl->AddToSequence (block, Intermediate, false));
      exprImpl->AddToSequencePostAction (block, originalTarget, false);
      
      RegisterID actualTarget (exprImpl->AddToSequence (block, Intermediate, true));
      if (!actualTarget.IsValid())
	throw Exception (SwizzledExpressionNotAnLValue);
      
      TypeImplPtr originalValueType (exprImpl->GetValueType());
      TypeImplPtr originalValueCompType (boost::shared_static_cast<TypeImpl> (originalValueType->avmBase));
      TypeImplPtr valueType (GetValueType());
      
      unsigned int compDefined = 0;
      std::vector<RegisterID> compRegs;
      compRegs.insert (compRegs.begin(), originalValueType->vectorDim, RegisterID ());
      
      for (unsigned int c = 0; c < attr.swizzleCompNum; c++)
      {
	unsigned int comp = attr.GetSwizzleComp (c);
	if (compDefined & (1 << comp))
	  throw Exception (MultipleUseOfComponentInLValueSwizzle);

	if (valueType->typeClass == TypeImpl::Vector)
	{
	  RegisterID compReg (handler->AllocateRegister (seq, originalValueCompType, Intermediate));
	  SequenceOpPtr seqOp (boost::make_shared<SequenceOpExtractVectorComponent> (compReg,
										     target,
										     c));
	  seq.AddOp (seqOp);
	  compRegs[comp] = compReg;
	}
	else
	{
	  compRegs[comp] = target;
	}
	
	compDefined |= 1 << comp;
      }
      
      for (unsigned int c = 0; c < originalValueType->vectorDim; c++)
      {
	if (!(compDefined & (1 << c)))
	{
	  RegisterID compReg (handler->AllocateRegister (seq, originalValueCompType, Intermediate));
	  SequenceOpPtr seqOp (boost::make_shared<SequenceOpExtractVectorComponent> (compReg,
										     originalTarget,
										     c));
	  seq.AddOp (seqOp);
	  compRegs[c] = compReg;
	}
      }
      BasicType vecType;
      switch (originalValueCompType->base)
      {
      case Bool: 	vecType = intermediate::Bool; break;
      case Int: 	vecType = intermediate::Int; break;
      case UInt: 	vecType = intermediate::UInt; break;
      case Float: 	vecType = intermediate::Float; break;
      default:		return;
      }
      SequenceOpPtr seqOp (boost::make_shared<SequenceOpMakeVector> (actualTarget, vecType, compRegs));
      seq.AddOp (seqOp);
      
      exprImpl->AddToSequencePostAction (block, actualTarget, true);
    }
  } // namespace intermediate
} // namespace s1
