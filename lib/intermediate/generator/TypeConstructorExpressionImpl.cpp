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

#include "intermediate/Exception.h"
#include "intermediate/SequenceOp/SequenceOpAssign.h"
#include "intermediate/SequenceOp/SequenceOpExtractVectorComponent.h"
#include "intermediate/SequenceOp/SequenceOpMakeArray.h"
#include "intermediate/SequenceOp/SequenceOpMakeMatrix.h"
#include "intermediate/SequenceOp/SequenceOpMakeVector.h"

#include "BlockImpl.h"
#include "TypeConstructorExpressionImpl.h"
#include "TypeImpl.h"

#include <boost/make_shared.hpp>

namespace s1
{
  namespace intermediate
  {
    IntermediateGeneratorSemanticsHandler::TypeConstructorExpressionImpl::TypeConstructorExpressionImpl (
      IntermediateGeneratorSemanticsHandler* handler, const TypeImplPtr& type,
      const ExpressionVector& params)
     : ExpressionImpl (handler), type (type), params (params)
    {
    }

    void IntermediateGeneratorSemanticsHandler::TypeConstructorExpressionImpl::ExtractBaseExpressionRegs (BlockImpl& block,
													  std::vector<RegisterPtr>& regs,
													  PostActionList& postActions)
    {
      SequenceBuilder& seq (*(block.GetSequenceBuilder()));
      
      TypeImplPtr targetBaseType (boost::static_pointer_cast<TypeImpl> (type->avmBase));
	  
      for (ExpressionVector::const_iterator expr (params.begin());
	   expr != params.end();
	   ++expr)
      {
	boost::shared_ptr<ExpressionImpl> exprImpl (boost::static_pointer_cast<ExpressionImpl> (*expr));
	TypeImplPtr exprType (exprImpl->GetValueType());
	RegisterPtr srcExprReg (exprImpl->AddToSequence (block, Intermediate, false));
	postActions.push_back(std::make_pair<> (exprImpl, srcExprReg));
	
	switch (exprType->typeClass)
	{
	default:
	  {
	    // Add expression as-is
	    if (targetBaseType->IsEqual (*exprType))
	    {
	      regs.push_back (srcExprReg);
	    }
	    else
	    {
	      RegisterPtr srcReg (handler->AllocateRegister (seq, targetBaseType, Intermediate));
	      handler->GenerateCast (seq, srcReg, targetBaseType, srcExprReg, exprType);
	      regs.push_back (srcReg);
	    }
	  }
	  break;
	case TypeImpl::Vector:
	  {
	    // extract components
	    TypeImplPtr exprCompType (boost::static_pointer_cast<TypeImpl> (exprType->avmBase));
	    for (unsigned int c = 0; c < exprType->vectorDim; c++)
	    {
	      RegisterPtr compReg (handler->AllocateRegister (seq, exprCompType, Intermediate));
	      SequenceOpPtr extractOp (boost::make_shared<SequenceOpExtractVectorComponent> (compReg, srcExprReg, c));
	      seq.AddOp (extractOp);
	      if (targetBaseType->IsEqual (*exprCompType))
	      {
		regs.push_back (compReg);
	      }
	      else
	      {
		RegisterPtr srcReg (handler->AllocateRegister (seq, targetBaseType, Intermediate));
		handler->GenerateCast (seq, srcReg, targetBaseType, compReg, exprCompType);
		regs.push_back (srcReg);
	      }
	    }
	  }
	  break;
	}
      }
    }

    RegisterPtr
    IntermediateGeneratorSemanticsHandler::TypeConstructorExpressionImpl::AddToSequence (BlockImpl& block, 
											 RegisterClassification classify,
											 bool asLvalue)
    {
      if (asLvalue) return RegisterPtr();
      
      SequenceBuilder& seq (*(block.GetSequenceBuilder()));
      
      switch (type->typeClass)
      {
      case TypeImpl::Base:
	{
	  if (params.size() > 1)
	    throw Exception (TooManyTypeCtorArgs);
	  if (params.size() < 1)
	    throw Exception (TooFewTypeCtorArgs);
	  
	  RegisterPtr targetReg (handler->AllocateRegister (seq, type, classify));
	  ExpressionPtr srcExpr (params[0]);
	  boost::shared_ptr<ExpressionImpl> srcExprImpl (boost::static_pointer_cast<ExpressionImpl> (srcExpr));
	  RegisterPtr srcReg (srcExprImpl->AddToSequence (block, Intermediate, false));
	  TypeImplPtr srcType (srcExprImpl->GetValueType());
	  if (type->IsEqual (*srcType))
	  {
	    // If type is the same, just generate assignment
	    SequenceOpPtr seqOp (boost::make_shared<SequenceOpAssign> (targetReg, srcReg));
	    seq.AddOp (seqOp);
	  }
	  else
	  {
	    // otherwise, generate cast
	    handler->GenerateCast (seq, targetReg, type, srcReg, srcType);
	  }
	  srcExprImpl->AddToSequencePostAction (block, srcReg, false);
	  return targetReg;
	}
	break;
      case TypeImpl::Matrix:
      case TypeImpl::Vector:
	{
	  // Extract operands of base type from params (extract vector comps etc.)
	  std::vector<RegisterPtr> srcRegs;
	  PostActionList postActions;
	  ExtractBaseExpressionRegs (block, srcRegs, postActions);
	  
	  unsigned int desiredDim;
	  if (type->typeClass == TypeImpl::Vector)
	    desiredDim = type->vectorDim;
	  else
	    desiredDim = type->matrixCols * type->matrixRows;
	  
	  if ((type->typeClass == TypeImpl::Vector) && (srcRegs.size() == 1))
	  {
	    // Replicate input
	    for (unsigned int i = 1; i < desiredDim; i++)
	      srcRegs.push_back (srcRegs[0]);
	  }
	  else
	  {
	    if (srcRegs.size() > desiredDim)
	      throw Exception (TooManyTypeCtorArgs);
	    if (srcRegs.size() < desiredDim)
	      throw Exception (TooFewTypeCtorArgs);
	  }
	  
	  TypeImplPtr targetBaseType (boost::static_pointer_cast<TypeImpl> (type->avmBase));
	  
	  RegisterPtr targetReg (handler->AllocateRegister (seq, type, classify));
	  BasicType vecType;
	  switch (targetBaseType->base)
	  {
	  case Bool: 	vecType = intermediate::Bool; break;
	  case Int: 	vecType = intermediate::Int; break;
	  case UInt: 	vecType = intermediate::UInt; break;
	  case Float: 	vecType = intermediate::Float; break;
	  default:	return RegisterPtr();
	  }
	  
	  SequenceOpPtr seqOp;
	  if (type->typeClass == TypeImpl::Vector)
	    seqOp = boost::make_shared<SequenceOpMakeVector> (targetReg, vecType, srcRegs);
	  else
	    seqOp = boost::make_shared<SequenceOpMakeMatrix> (targetReg, vecType, 
							      type->matrixRows, type->matrixCols, srcRegs);
	  seq.AddOp (seqOp);
	  
	  for (PostActionList::const_iterator postAction (postActions.begin());
	       postAction != postActions.end();
	       ++postAction)
	  {
	    postAction->first->AddToSequencePostAction (block, postAction->second, false);
	  }
	  
	  return targetReg;
	}
	break;
      case TypeImpl::Sampler:
	// ...
	return RegisterPtr();
      case TypeImpl::Array:
	{
	  RegisterPtr targetReg (handler->AllocateRegister (seq, type, classify));
	  std::vector<RegisterPtr> srcRegs;
	  
	  TypeImplPtr targetBaseType (boost::static_pointer_cast<TypeImpl> (type->avmBase));
	  
	  for (ExpressionVector::const_iterator expr (params.begin());
	      expr != params.end();
	      ++expr)
	  {
	    boost::shared_ptr<ExpressionImpl> exprImpl (boost::static_pointer_cast<ExpressionImpl> (*expr));
	    TypeImplPtr exprType (exprImpl->GetValueType());
	    RegisterPtr srcExprReg (exprImpl->AddToSequence (block, Intermediate, false));
	    
	    // Add expression as-is
	    if (targetBaseType->IsEqual (*exprType))
	    {
	      srcRegs.push_back (srcExprReg);
	    }
	    else
	    {
	      RegisterPtr srcReg (handler->AllocateRegister (seq, targetBaseType, Intermediate));
	      handler->GenerateCast (seq, srcReg, targetBaseType, srcExprReg, exprType);
	      srcRegs.push_back (srcReg);
	    }
	  }
	  
	  SequenceOpPtr seqOp (boost::make_shared<SequenceOpMakeArray> (targetReg, srcRegs));
	  seq.AddOp (seqOp);
	  return targetReg;
	}
	break;
      default:
	return RegisterPtr();
      }
    }
  } // namespace intermediate
} // namespace s1
