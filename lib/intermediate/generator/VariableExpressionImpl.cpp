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

#include "VariableExpressionImpl.h"

#include "BlockImpl.h"
#include "intermediate/SequenceOp/SequenceOpAssign.h"
#include "semantics/Name.h"
#include "ScopeImpl.h"

namespace s1
{
  namespace intermediate
  {
    IntermediateGeneratorSemanticsHandler::VariableExpressionImpl::VariableExpressionImpl (
      IntermediateGeneratorSemanticsHandler* handler,
      ExpressionContext&& context,
      semantics::NameVariable* name)
       : ExpressionImpl (handler, std::move (context)), name (name)
    {
    }
      
    IntermediateGeneratorSemanticsHandler::NameVariableSet
    IntermediateGeneratorSemanticsHandler::VariableExpressionImpl::QueryWrittenNames (bool asLvalue)
    {
      NameVariableSet set;
      if (asLvalue) set.insert (name);
      return set;
    }
      
    semantics::TypePtr IntermediateGeneratorSemanticsHandler::VariableExpressionImpl::GetValueType()
    {
      return name->GetValueType ();
    }
    
    RegisterPtr IntermediateGeneratorSemanticsHandler::VariableExpressionImpl::AddToSequence (BlockImpl& block,
                                                                                              RegisterClassification classify,
                                                                                              bool asLvalue)
    {
      auto blockReg = block.GetRegisterForName (this->name.get(), asLvalue);
      if (!blockReg)
      {
        ExpressionError (blockReg.error ());
        return RegisterPtr ();
      }
      return blockReg.value ();
    }
  } // namespace intermediate
} // namespace s1
