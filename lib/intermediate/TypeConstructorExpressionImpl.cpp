#include "base/common.h"

#include "intermediate/Exception.h"
#include "intermediate/SequenceOp/SequenceOpAssign.h"
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

    void IntermediateGeneratorSemanticsHandler::TypeConstructorExpressionImpl::ExtractBaseExpressions (ExpressionVector& baseExpr)
    {
      for (ExpressionVector::const_iterator expr (params.begin());
	   expr != params.end();
	   ++expr)
      {
	boost::shared_ptr<ExpressionImpl> exprImpl (boost::shared_static_cast<ExpressionImpl> (*expr));
	TypeImplPtr exprType (exprImpl->GetValueType());
	
	switch (exprType->typeClass)
	{
	default:
	  {
	    // Add expression as-is
	    baseExpr.push_back (*expr);
	  }
	  break;
	case TypeImpl::Vector:
	  // ... extract components
	  break;
	}
      }
    }

    RegisterID
    IntermediateGeneratorSemanticsHandler::TypeConstructorExpressionImpl::AddToSequence (BlockImpl& block, 
											 RegisterClassification classify,
											 bool asLvalue)
    {
      if (asLvalue) return RegisterID();
      
      Sequence& seq (*(block.GetSequence()));
      
      switch (type->typeClass)
      {
      case TypeImpl::Base:
	{
	  if (params.size() > 1)
	    throw Exception (TooManyTypeCtorArgs);
	  if (params.size() < 1)
	    throw Exception (TooFewTypeCtorArgs);
	  
	  RegisterID targetReg (handler->AllocateRegister (seq, type, classify));
	  ExpressionPtr srcExpr (params[0]);
	  boost::shared_ptr<ExpressionImpl> srcExprImpl (boost::shared_static_cast<ExpressionImpl> (srcExpr));
	  RegisterID srcReg (srcExprImpl->AddToSequence (block, Intermediate, false));
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
	  return targetReg;
	}
	break;
      case TypeImpl::Matrix:
      case TypeImpl::Vector:
	{
	  // Extract operands of base type from params (extract vector comps etc.)
	  ExpressionVector baseExpr;
	  ExtractBaseExpressions (baseExpr);
	  
	  unsigned int desiredDim;
	  if (type->typeClass == TypeImpl::Vector)
	    desiredDim = type->vectorDim;
	  else
	    desiredDim = type->matrixCols * type->matrixRows;
	  
	  if (params.size() > desiredDim)
	    throw Exception (TooManyTypeCtorArgs);
	  if (params.size() < desiredDim)
	    throw Exception (TooFewTypeCtorArgs);
	  
	  TypeImplPtr targetBaseType (boost::shared_static_cast<TypeImpl> (type->avmBase));
	  std::vector<RegisterID> srcRegs;
	  for (ExpressionVector::const_iterator srcExpr (baseExpr.begin());
	       srcExpr != baseExpr.end();
	       ++srcExpr)
	  {
	    boost::shared_ptr<ExpressionImpl> srcExprImpl (boost::shared_static_cast<ExpressionImpl> (*srcExpr));
	    TypeImplPtr srcExprType (srcExprImpl->GetValueType());
	    RegisterID srcExprReg (srcExprImpl->AddToSequence (block, Intermediate, false));
	    if (targetBaseType->IsEqual (*srcExprType))
	    {
	      srcRegs.push_back (srcExprReg);
	    }
	    else
	    {
	      RegisterID srcReg (handler->AllocateRegister (seq, type, Intermediate));
	      handler->GenerateCast (seq, srcReg, targetBaseType, srcExprReg, srcExprType);
	      srcRegs.push_back (srcReg);
	    }
	  }
	  
	  RegisterID targetReg (handler->AllocateRegister (seq, type, classify));
	  BasicType vecType;
	  switch (targetBaseType->base)
	  {
	  case Bool: 	vecType = intermediate::Bool; break;
	  case Int: 	vecType = intermediate::Int; break;
	  case UInt: 	vecType = intermediate::UInt; break;
	  case Float: 	vecType = intermediate::Float; break;
	  default:	return RegisterID();
	  }
	  
	  SequenceOpPtr seqOp;
	  if (type->typeClass == TypeImpl::Vector)
	    seqOp = boost::make_shared<SequenceOpMakeVector> (targetReg, vecType, srcRegs);
	  else
	    seqOp = boost::make_shared<SequenceOpMakeMatrix> (targetReg, vecType, 
							      type->matrixRows, type->matrixCols, srcRegs);
	  seq.AddOp (seqOp);
	  return targetReg;
	}
	break;
      case TypeImpl::Sampler:
	// ...
      case TypeImpl::Array:
	// ...
      default:
	return RegisterID();
      }
    }
  } // namespace intermediate
} // namespace s1
