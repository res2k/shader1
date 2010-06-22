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
													  std::vector<RegisterID>& regs)
    {
      Sequence& seq (*(block.GetSequence()));
      
      TypeImplPtr targetBaseType (boost::shared_static_cast<TypeImpl> (type->avmBase));
	  
      for (ExpressionVector::const_iterator expr (params.begin());
	   expr != params.end();
	   ++expr)
      {
	boost::shared_ptr<ExpressionImpl> exprImpl (boost::shared_static_cast<ExpressionImpl> (*expr));
	TypeImplPtr exprType (exprImpl->GetValueType());
	RegisterID srcExprReg (exprImpl->AddToSequence (block, Intermediate, false));
	
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
	      RegisterID srcReg (handler->AllocateRegister (seq, targetBaseType, Intermediate));
	      handler->GenerateCast (seq, srcReg, targetBaseType, srcExprReg, exprType);
	      regs.push_back (srcReg);
	    }
	  }
	  break;
	case TypeImpl::Vector:
	  {
	    // extract components
	    TypeImplPtr exprCompType (boost::shared_static_cast<TypeImpl> (exprType->avmBase));
	    for (unsigned int c = 0; c < exprType->vectorDim; c++)
	    {
	      RegisterID compReg (handler->AllocateRegister (seq, exprCompType, Intermediate));
	      SequenceOpPtr extractOp (boost::make_shared<SequenceOpExtractVectorComponent> (compReg, srcExprReg, c));
	      seq.AddOp (extractOp);
	      if (targetBaseType->IsEqual (*exprCompType))
	      {
		regs.push_back (compReg);
	      }
	      else
	      {
		RegisterID srcReg (handler->AllocateRegister (seq, targetBaseType, Intermediate));
		handler->GenerateCast (seq, srcReg, targetBaseType, compReg, exprCompType);
		regs.push_back (srcReg);
	      }
	    }
	  }
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
	  std::vector<RegisterID> srcRegs;
	  ExtractBaseExpressionRegs (block, srcRegs);
	  
	  unsigned int desiredDim;
	  if (type->typeClass == TypeImpl::Vector)
	    desiredDim = type->vectorDim;
	  else
	    desiredDim = type->matrixCols * type->matrixRows;
	  
	  if (srcRegs.size() > desiredDim)
	    throw Exception (TooManyTypeCtorArgs);
	  if (srcRegs.size() < desiredDim)
	    throw Exception (TooFewTypeCtorArgs);
	  
	  TypeImplPtr targetBaseType (boost::shared_static_cast<TypeImpl> (type->avmBase));
	  
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
	return RegisterID();
      case TypeImpl::Array:
	{
	  RegisterID targetReg (handler->AllocateRegister (seq, type, classify));
	  std::vector<RegisterID> srcRegs;
	  
	  TypeImplPtr targetBaseType (boost::shared_static_cast<TypeImpl> (type->avmBase));
	  
	  for (ExpressionVector::const_iterator expr (params.begin());
	      expr != params.end();
	      ++expr)
	  {
	    boost::shared_ptr<ExpressionImpl> exprImpl (boost::shared_static_cast<ExpressionImpl> (*expr));
	    TypeImplPtr exprType (exprImpl->GetValueType());
	    RegisterID srcExprReg (exprImpl->AddToSequence (block, Intermediate, false));
	    
	    // Add expression as-is
	    if (targetBaseType->IsEqual (*exprType))
	    {
	      srcRegs.push_back (srcExprReg);
	    }
	    else
	    {
	      RegisterID srcReg (handler->AllocateRegister (seq, targetBaseType, Intermediate));
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
	return RegisterID();
      }
    }
  } // namespace intermediate
} // namespace s1
