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

#include "BoolExpressionImpl.h"

#include "BlockImpl.h"
#include "intermediate/SequenceOp/SequenceOpConst.h"

#include <boost/make_shared.hpp>

namespace s1
{
  namespace intermediate
  {
    IntermediateGeneratorSemanticsHandler::BoolExpressionImpl::BoolExpressionImpl (
      IntermediateGeneratorSemanticsHandler* handler,
      bool value)
       : ExpressionImpl (handler), value (value)
    {
    }
      
    boost::shared_ptr<IntermediateGeneratorSemanticsHandler::TypeImpl>
    IntermediateGeneratorSemanticsHandler::BoolExpressionImpl::GetValueType()
    {
      return handler->GetBoolType();
    }
    
    RegisterPtr IntermediateGeneratorSemanticsHandler::BoolExpressionImpl::AddToSequence (BlockImpl& block,
											  RegisterClassification classify,
											  bool asLvalue)
    {
      if (asLvalue) return RegisterPtr();
      
      RegisterPtr destination (handler->AllocateRegister (*(block.GetSequence()), GetValueType(), classify));
      SequenceOpPtr seqOp (boost::make_shared<SequenceOpConst> (destination, value));
      assert (seqOp);
      block.GetSequence()->AddOp (seqOp);
      
      return destination;
    }
  } // namespace intermediate
} // namespace s1
