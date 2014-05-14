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

#include "UnaryExpressionImpl.h"

#include "BlockImpl.h"
#include "intermediate/Exception.h"
#include "intermediate/SequenceOp/SequenceOpUnaryOp.h"
#include "TypeImpl.h"

#include <boost/make_shared.hpp>

namespace s1
{
  namespace intermediate
  {
    IntermediateGeneratorSemanticsHandler::UnaryExpressionImpl::UnaryExpressionImpl (
			   IntermediateGeneratorSemanticsHandler* handler,
			   UnaryOp op,
			   const boost::shared_ptr<ExpressionImpl>& operand)
       : ExpressionImpl (handler), op (op), operand (operand)
    {
    }

    IntermediateGeneratorSemanticsHandler::NameImplSet
    IntermediateGeneratorSemanticsHandler::UnaryExpressionImpl::QueryWrittenNames (bool asLvalue)
    {
      return operand->QueryWrittenNames (asLvalue);
    }
      
    boost::shared_ptr<IntermediateGeneratorSemanticsHandler::TypeImpl>
    IntermediateGeneratorSemanticsHandler::UnaryExpressionImpl::GetValueType()
    {
      boost::shared_ptr<TypeImpl> operandType = operand->GetValueType();
      boost::shared_ptr<TypeImpl> valueType;
      
      switch (op)
      {
      case Neg:
	if (operandType->IsEqual (*(handler->GetFloatType()))
	    || operandType->IsEqual (*(handler->GetIntType())))
	  valueType = operandType;
	else if (operandType->IsEqual (*(handler->GetUintType())))
	  valueType = handler->GetIntType();
	else
	  throw Exception (OperandTypesInvalid);
	break;
      case Inv:
	if (operandType->IsEqual (*(handler->GetUintType()))
	    || operandType->IsEqual (*(handler->GetIntType())))
	  valueType = operandType;
	else
	  throw Exception (OperandTypesInvalid);
	break;
      case Not:
	if (operandType->IsEqual (*(handler->GetBoolType())))
	  valueType = operandType;
	else
	  throw Exception (OperandTypesInvalid);
	break;
      }
      
      return valueType;
    }
    
    RegisterPtr IntermediateGeneratorSemanticsHandler::UnaryExpressionImpl::AddToSequence (BlockImpl& block,
											   RegisterClassification classify,
											   bool asLvalue)
    {
      if (asLvalue) return RegisterPtr();
      
      Sequence& seq (*(block.GetSequence()));
      boost::shared_ptr<TypeImpl> operandType = operand->GetValueType();
      
      boost::shared_ptr<TypeImpl> valueType = GetValueType ();

      // Set up register for operand value
      RegisterPtr orgReg, reg;
      orgReg = reg = operand->AddToSequence (block, Intermediate);
      if (!valueType->IsEqual (*(operandType.get())))
      {
	// Insert cast op
	RegisterPtr newReg (handler->AllocateRegister (seq, valueType, Intermediate));
	handler->GenerateCast (seq, newReg, valueType,
			       reg, operandType);
	reg = newReg;
      }
      
      RegisterPtr destination (handler->AllocateRegister (*(block.GetSequence()), GetValueType(), classify));
      
      // Create actual sequence operation
      SequenceOpPtr seqOp;
      switch (op)
      {
      case Neg:
	seqOp = SequenceOpPtr (boost::make_shared<SequenceOpUnaryOp> (destination, SequenceVisitor::Neg, reg));
	break;
      case Inv:
	seqOp = SequenceOpPtr (boost::make_shared<SequenceOpUnaryOp> (destination, SequenceVisitor::Inv, reg));
	break;
      case Not:
	seqOp = SequenceOpPtr (boost::make_shared<SequenceOpUnaryOp> (destination, SequenceVisitor::Not, reg));
	break;
      }
      assert (seqOp);
      seq.AddOp (seqOp);
      
      operand->AddToSequencePostAction (block, orgReg, false);
      
      return destination;
    }
  } // namespace intermediate
} // namespace s1
