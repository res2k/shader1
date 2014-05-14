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

#include "LogicExpressionImpl.h"

#include "BlockImpl.h"
#include "intermediate/Exception.h"
#include "intermediate/SequenceOp/SequenceOpLogic.h"
#include "TypeImpl.h"

#include <boost/make_shared.hpp>

namespace s1
{
  namespace intermediate
  {
    IntermediateGeneratorSemanticsHandler::LogicExpressionImpl::LogicExpressionImpl (
      IntermediateGeneratorSemanticsHandler* handler,
      LogicOp op,
      const boost::shared_ptr<ExpressionImpl>& operand1,
      const boost::shared_ptr<ExpressionImpl>& operand2)
       : ExpressionImpl (handler), op (op), operand1 (operand1), operand2 (operand2)
    {
    }
      
    IntermediateGeneratorSemanticsHandler::NameImplSet
    IntermediateGeneratorSemanticsHandler::LogicExpressionImpl::QueryWrittenNames (bool asLvalue)
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
      
    boost::shared_ptr<IntermediateGeneratorSemanticsHandler::TypeImpl>
    IntermediateGeneratorSemanticsHandler::LogicExpressionImpl::GetValueType()
    {
      boost::shared_ptr<TypeImpl> type1 = operand1->GetValueType();
      boost::shared_ptr<TypeImpl> type2 = operand2->GetValueType();
      
      // Operands have to be bools
      if (!type1->IsEqual (*(handler->GetBoolType().get()))
	|| !type2->IsEqual (*(handler->GetBoolType().get())))
      {
	throw Exception (OperandTypesInvalid);
      }
      
      // Logic ops have always a bool result
      return handler->GetBoolType();
    }
    
    RegisterPtr IntermediateGeneratorSemanticsHandler::LogicExpressionImpl::AddToSequence (BlockImpl& block,
											   RegisterClassification classify,
											   bool asLvalue)
    {
      if (asLvalue) return RegisterPtr();
      
      Sequence& seq (*(block.GetSequence()));
	
      // Set up registers for operand values
      RegisterPtr orgReg1, reg1;
      orgReg1 = reg1 = operand1->AddToSequence (block, Intermediate);
      RegisterPtr orgReg2, reg2;
      orgReg2 = reg2 = operand2->AddToSequence (block, Intermediate);
      
      RegisterPtr destination (handler->AllocateRegister (*(block.GetSequence()), GetValueType(), classify));
      
      // Create actual sequence operation
      SequenceOpPtr seqOp;
      switch (op)
      {
      case And:
	seqOp = SequenceOpPtr (boost::make_shared<SequenceOpLogic> (destination, SequenceVisitor::And, reg1, reg2));
	break;
      case Or:
	seqOp = SequenceOpPtr (boost::make_shared<SequenceOpLogic> (destination, SequenceVisitor::Or, reg1, reg2));
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
