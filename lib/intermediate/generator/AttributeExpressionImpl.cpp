/*
    Shader1
    Copyright (c) 2010-2014 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#include "base/common.h"

#include "AttributeExpressionImpl.h"

#include "intermediate/Exception.h"
#include "intermediate/SequenceOp/SequenceOpExtractVectorComponent.h"
#include "intermediate/SequenceOp/SequenceOpGetArrayLength.h"
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
      boost::shared_ptr<ExpressionImpl> exprImpl (boost::static_pointer_cast<ExpressionImpl> (baseExpr));
      return handler->GetAttributeType (exprImpl->GetValueType(), attr);
    }
    
    RegisterPtr IntermediateGeneratorSemanticsHandler::AttributeExpressionImpl::AddToSequence (BlockImpl& block,
											       RegisterClassification classify,
											       bool asLvalue)
    {
      SequenceBuilder& seq (*(block.GetSequenceBuilder()));
      
      switch (attr.attrClass)
      {
      default:
      case IntermediateGeneratorSemanticsHandler::Attribute::Unknown:
	assert (false);
	return RegisterPtr ();
      case IntermediateGeneratorSemanticsHandler::Attribute::arrayLength:
	{
	  if (asLvalue) return RegisterPtr ();
	  
	  RegisterPtr targetReg (handler->AllocateRegister (seq, GetValueType(), Intermediate));
	  boost::shared_ptr<ExpressionImpl> exprImpl (boost::static_pointer_cast<ExpressionImpl> (baseExpr));
	  RegisterPtr exprValueReg (exprImpl->AddToSequence (block, Intermediate, false));
	  
	  SequenceOpPtr seqOp (new SequenceOpGetArrayLength (targetReg, exprValueReg));
	  seq.AddOp (seqOp);
	  
	  exprImpl->AddToSequencePostAction (block, exprValueReg, false);
	  
	  return targetReg;
	}
	break;
      case IntermediateGeneratorSemanticsHandler::Attribute::matrixRow:
      case IntermediateGeneratorSemanticsHandler::Attribute::matrixCol:
      case IntermediateGeneratorSemanticsHandler::Attribute::matrixTranspose:
      case IntermediateGeneratorSemanticsHandler::Attribute::matrixInvert:
	{
	  if (asLvalue) return RegisterPtr ();
	  
	  return RegisterPtr ();
	}
	break;
      case IntermediateGeneratorSemanticsHandler::Attribute::vectorSwizzle:
	{
	  if (asLvalue)
	  {
	    RegisterPtr targetReg (handler->AllocateRegister (seq, GetValueType(), Intermediate));
	    return targetReg;
	  }
	  else
	  {
	    boost::shared_ptr<ExpressionImpl> exprImpl (boost::static_pointer_cast<ExpressionImpl> (baseExpr));
	    RegisterPtr exprValueReg (exprImpl->AddToSequence (block, Intermediate, false));
	    
	    TypeImplPtr valueType (GetValueType());
	    RegisterPtr targetReg;
	    if (attr.swizzleCompNum > 1)
	    {
	      // multi-component swizzle
	      targetReg = handler->AllocateRegister (seq, valueType, classify);
	      TypeImplPtr valueCompType (boost::static_pointer_cast<TypeImpl> (valueType->avmBase));
	      std::vector<RegisterPtr> compRegs;
	      for (unsigned int c = 0; c < attr.swizzleCompNum; c++)
	      {
		RegisterPtr compReg (handler->AllocateRegister (seq, valueCompType, classify));
		SequenceOpPtr seqOp (new SequenceOpExtractVectorComponent (compReg, exprValueReg, attr.GetSwizzleComp (c)));
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
	      default:	return RegisterPtr();
	      }
	      SequenceOpPtr seqOp (new SequenceOpMakeVector (targetReg, vecType, compRegs));
	      seq.AddOp (seqOp);
	    }
	    else
	    {
	      RegisterPtr compReg (handler->AllocateRegister (seq, valueType, classify));
	      SequenceOpPtr seqOp (new SequenceOpExtractVectorComponent (compReg, exprValueReg, attr.GetSwizzleComp (0)));
	      seq.AddOp (seqOp);
	      targetReg = compReg;
	    }
	    
	    exprImpl->AddToSequencePostAction (block, exprValueReg, false);
	    
	    return targetReg;
	  }
	}
	break;
      }
    }

    void IntermediateGeneratorSemanticsHandler::AttributeExpressionImpl::AddToSequencePostAction (BlockImpl& block,
												  const RegisterPtr& target,
												  bool wasLvalue)
    {
      if (!wasLvalue) return;
      
      SequenceBuilder& seq (*(block.GetSequenceBuilder()));
      
      // Generate assignment from register to actual target
      boost::shared_ptr<ExpressionImpl> exprImpl (boost::static_pointer_cast<ExpressionImpl> (baseExpr));
      
      RegisterPtr originalTarget (exprImpl->AddToSequence (block, Intermediate, false));
      exprImpl->AddToSequencePostAction (block, originalTarget, false);
      
      RegisterPtr actualTarget (exprImpl->AddToSequence (block, Intermediate, true));
      if (!actualTarget)
	throw Exception (SwizzledExpressionNotAnLValue);
      
      TypeImplPtr originalValueType (exprImpl->GetValueType());
      TypeImplPtr originalValueCompType (boost::static_pointer_cast<TypeImpl> (originalValueType->avmBase));
      TypeImplPtr valueType (GetValueType());
      
      unsigned int compDefined = 0;
      std::vector<RegisterPtr> compRegs;
      compRegs.insert (compRegs.begin(), originalValueType->vectorDim, RegisterPtr ());
      
      for (unsigned int c = 0; c < attr.swizzleCompNum; c++)
      {
	unsigned int comp = attr.GetSwizzleComp (c);
	if (compDefined & (1 << comp))
	  throw Exception (MultipleUseOfComponentInLValueSwizzle);

	if (valueType->typeClass == TypeImpl::Vector)
	{
	  RegisterPtr compReg (handler->AllocateRegister (seq, originalValueCompType, Intermediate));
	  SequenceOpPtr seqOp (new SequenceOpExtractVectorComponent (compReg, target, c));
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
	  RegisterPtr compReg (handler->AllocateRegister (seq, originalValueCompType, Intermediate));
	  SequenceOpPtr seqOp (new SequenceOpExtractVectorComponent (compReg, originalTarget, c));
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
      SequenceOpPtr seqOp (new SequenceOpMakeVector (actualTarget, vecType, compRegs));
      seq.AddOp (seqOp);
      
      exprImpl->AddToSequencePostAction (block, actualTarget, true);
    }
  } // namespace intermediate
} // namespace s1
