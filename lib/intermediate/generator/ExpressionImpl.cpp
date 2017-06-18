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

#include "BlockImpl.h"
#include "ExpressionImpl.h"
#include "TypeImpl.h"

namespace s1
{
  namespace intermediate
  {
    IntermediateGeneratorSemanticsHandler::ExpressionImpl::ExpressionImpl (IntermediateGeneratorSemanticsHandler* handler,
                                                                           ExpressionContext&& context)
     : handler (handler), context (std::move (context))
    {
    }

    void IntermediateGeneratorSemanticsHandler::ExpressionImpl::Error (ErrorCode code)
    {
      handler->Error (context, code);
    }

    const IntermediateGeneratorSemanticsHandler::ExpressionContext&
    IntermediateGeneratorSemanticsHandler::ExpressionImpl::GetExpressionContext ()
    {
      return context;
    }

    IntermediateGeneratorSemanticsHandler::NameImplSet
    IntermediateGeneratorSemanticsHandler::ExpressionImpl::QueryWrittenNames (bool asLvalue)
    {
      return NameImplSet ();
    }
     
    void IntermediateGeneratorSemanticsHandler::ExpressionImpl::AddToSequence (BlockImpl& block)
    {
      RegisterPtr res (AddToSequence (block, Dummy));
      /* Need to generate operations even if the result isn't used, due
         to possible side effects.
         Eventually, dead code elimination should take care of removing the
         dummy and computations (if possible).
       */
      AddToSequencePostAction (block, res, false);
    }
  } // namespace intermediate
} // namespace s1
