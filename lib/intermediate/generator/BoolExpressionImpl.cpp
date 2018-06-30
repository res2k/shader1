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
#include "intermediate/SequenceBuilder.h"
#include "intermediate/SequenceOp/SequenceOpConst.h"

namespace s1
{
  namespace intermediate
  {
    IntermediateGeneratorSemanticsHandler::BoolExpressionImpl::BoolExpressionImpl (
      IntermediateGeneratorSemanticsHandler* handler,
      ExpressionContext&& context,
      bool value)
       : ExpressionImpl (handler, std::move (context)), value (value)
    {
    }
      
    semantics::TypePtr IntermediateGeneratorSemanticsHandler::BoolExpressionImpl::GetValueType()
    {
      return handler->GetBoolType();
    }
    
    RegisterPtr IntermediateGeneratorSemanticsHandler::BoolExpressionImpl::AddToSequence (BlockImpl& block,
                                                                                          RegisterClassification classify,
                                                                                          bool asLvalue)
    {
      if (asLvalue) return RegisterPtr();
      
      RegisterPtr destination (handler->AllocateRegister (*(block.GetSequenceBuilder()), GetValueType().get(), classify));
      SequenceOpPtr seqOp (new SequenceOpConst (destination, value));
      assert (seqOp);
      block.GetSequenceBuilder()->AddOp (seqOp);
      
      return destination;
    }
  } // namespace intermediate
} // namespace s1
