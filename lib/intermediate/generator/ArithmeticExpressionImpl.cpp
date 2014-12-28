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

#include "ArithmeticExpressionImpl.h"

#include "BlockImpl.h"
#include "intermediate/Exception.h"
#include "intermediate/SequenceOp/SequenceOpArith.h"
#include "TypeImpl.h"

#include <boost/make_shared.hpp>

namespace s1
{
  namespace intermediate
  {
    IntermediateGeneratorSemanticsHandler::ArithmeticExpressionImpl::ArithmeticExpressionImpl (
      IntermediateGeneratorSemanticsHandler* handler,
      ArithmeticOp op,
      const boost::shared_ptr<ExpressionImpl>& operand1,
      const boost::shared_ptr<ExpressionImpl>& operand2)
       : ExpressionImpl (handler), op (op), operand1 (operand1), operand2 (operand2)
    {
    }

    IntermediateGeneratorSemanticsHandler::NameImplSet
    IntermediateGeneratorSemanticsHandler::ArithmeticExpressionImpl::QueryWrittenNames (bool asLvalue)
    {
      NameImplSet set;
      {
	NameImplSet op1Set (operand1->QueryWrittenNames (asLvalue));
	set.insert (op1Set.begin(), op1Set.end());
      }
      {
	NameImplSet op2Set (operand2->QueryWrittenNames (asLvalue));
	set.insert (op2Set.begin(), op2Set.end());
      }
      return set;
    }
      
    IntermediateGeneratorSemanticsHandler::TypeImplPtr
    IntermediateGeneratorSemanticsHandler::ArithmeticExpressionImpl::GetValueType()
    {
      TypeImplPtr type1 = operand1->GetValueType();
      TypeImplPtr type2 = operand2->GetValueType();
      
      TypeImplPtr baseType1, baseType2;
      unsigned int vectorDim1 = 0, vectorDim2 = 0;
      
      if (type1->typeClass == TypeImpl::Vector)
      {
	baseType1 = boost::static_pointer_cast<TypeImpl> (type1->avmBase);
	vectorDim1 = type1->vectorDim;
      }
      else
	baseType1 = type1;
      if (type2->typeClass == TypeImpl::Vector)
      {
	baseType2 = boost::static_pointer_cast<TypeImpl> (type2->avmBase);
	vectorDim2 = type2->vectorDim;
      }
      else
	baseType2 = type2;
      
      if (!baseType1->CompatibleLossy (*(handler->GetFloatType().get()))
	|| !baseType2->CompatibleLossy (*(handler->GetFloatType().get())))
      {
	throw Exception (OperandTypesInvalid);
      }
      
      if ((vectorDim1 != 0) && (vectorDim2 != 0) && (vectorDim1 != vectorDim2))
      {
	throw Exception (OperandTypesIncompatible);
      }
      
      // Determine type in which to perform computation
      TypeImplPtr valueType;
      
      if ((vectorDim1 != 0) && (vectorDim2 == 0))
      {
	valueType = IntermediateGeneratorSemanticsHandler::GetHigherPrecisionType (
	  type1,
	  boost::static_pointer_cast<TypeImpl> (handler->CreateVectorType (type2, vectorDim1)));
      }
      else if ((vectorDim2 != 0) && (vectorDim1 == 0))
      {
	valueType = IntermediateGeneratorSemanticsHandler::GetHigherPrecisionType (
	  boost::static_pointer_cast<TypeImpl> (handler->CreateVectorType (type1, vectorDim2)),
	  type2);
      }
      else // (((vectorDim1 != 0) && (vectorDim2 != 0)) || ((vectorDim1 == 0) && (vectorDim2 == 0)))
      {
	valueType = IntermediateGeneratorSemanticsHandler::GetHigherPrecisionType (
	  operand1->GetValueType(), operand2->GetValueType());
      }
	
      if (!valueType)
      {
	throw Exception (OperandTypesIncompatible);
      }
      
      return valueType;
    }
    
    RegisterPtr IntermediateGeneratorSemanticsHandler::ArithmeticExpressionImpl::AddToSequence (BlockImpl& block,
												RegisterClassification classify,
												bool asLvalue)
    {
      if (asLvalue) return RegisterPtr();
      
      SequenceBuilder& seq (*(block.GetSequenceBuilder()));
      boost::shared_ptr<TypeImpl> type1 = operand1->GetValueType();
      boost::shared_ptr<TypeImpl> type2 = operand2->GetValueType();
      
      boost::shared_ptr<TypeImpl> valueType = GetValueType ();
	
      // Set up registers for operand values
      RegisterPtr orgReg1, reg1;
      orgReg1 = reg1 = operand1->AddToSequence (block, Intermediate);
      if (!valueType->IsEqual (*(type1.get())))
      {
	// Insert cast op
	RegisterPtr newReg1 (handler->AllocateRegister (seq, valueType, Intermediate));
	handler->GenerateCast (seq, newReg1, valueType,
			       reg1, type1);
	reg1 = newReg1;
      }
      RegisterPtr orgReg2, reg2;
      orgReg2 = reg2 = operand2->AddToSequence (block, Intermediate);
      if (!valueType->IsEqual (*(type2.get())))
      {
	// Insert cast op
	RegisterPtr newReg2 (handler->AllocateRegister (seq, valueType, Intermediate));
	handler->GenerateCast (seq, newReg2, valueType,
			       reg2, type2);
	reg2 = newReg2;
      }
      
      RegisterPtr destination (handler->AllocateRegister (seq, GetValueType(), classify));
      
      // Create actual sequence operation
      SequenceOpPtr seqOp;
      switch (op)
      {
      case Add:
	seqOp = SequenceOpPtr (boost::make_shared<SequenceOpArith> (destination, SequenceVisitor::Add, reg1, reg2));
	break;
      case Sub:
	seqOp = SequenceOpPtr (boost::make_shared<SequenceOpArith> (destination, SequenceVisitor::Sub, reg1, reg2));
	break;
      case Mul:
	seqOp = SequenceOpPtr (boost::make_shared<SequenceOpArith> (destination, SequenceVisitor::Mul, reg1, reg2));
	break;
      case Div:
	seqOp = SequenceOpPtr (boost::make_shared<SequenceOpArith> (destination, SequenceVisitor::Div, reg1, reg2));
	break;
      case Mod:
	seqOp = SequenceOpPtr (boost::make_shared<SequenceOpArith> (destination, SequenceVisitor::Mod, reg1, reg2));
	break;
      }
      assert (seqOp);
      seq.AddOp (seqOp);
      
      operand1->AddToSequencePostAction (block, orgReg1, false);
      operand2->AddToSequencePostAction (block, orgReg2, false);
      
      return destination;
    }
  } // namespace intermediate
} // namespace s1
