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

#include "AssignmentExpressionImpl.h"

#include "BlockImpl.h"
#include "intermediate/Diagnostics.h"
#include "intermediate/SequenceBuilder.h"
#include "intermediate/SequenceOp/SequenceOpAssign.h"

namespace s1
{
  namespace intermediate
  {
    IntermediateGeneratorSemanticsHandler::AssignmentExpressionImpl::AssignmentExpressionImpl (
      IntermediateGeneratorSemanticsHandler* handler,
      ExpressionContext&& context,
      ExpressionImpl* target,
      ExpressionImpl* value)
       : ExpressionImpl (handler, std::move (context)), target (target), value (value)
    {
    }
      
    IntermediateGeneratorSemanticsHandler::NameImplSet
    IntermediateGeneratorSemanticsHandler::AssignmentExpressionImpl::QueryWrittenNames (bool asLvalue)
    {
      NameImplSet set;
      NameImplSet targetSet (target->QueryWrittenNames (true));
      set.insert (targetSet.begin(), targetSet.end());
      NameImplSet valueSet (value->QueryWrittenNames (false));
      set.insert (valueSet.begin(), valueSet.end());
      return set;
    }
      
    semantics::TypePtr IntermediateGeneratorSemanticsHandler::AssignmentExpressionImpl::GetValueType()
    {
      auto targetType = target->GetValueType();
      auto valueType = value->GetValueType();

      if (!valueType->CompatibleLossy (*(targetType.get())))
      {
        ExpressionError (Error::AssignmentTypesIncompatible);
        return nullptr;
      }
      
      return targetType;
    }
    
    void IntermediateGeneratorSemanticsHandler::AssignmentExpressionImpl::AddToSequence (BlockImpl& block)
    {
      AddToSequence (block, Dummy);
    }
    
    RegisterPtr IntermediateGeneratorSemanticsHandler::AssignmentExpressionImpl::AddToSequence (BlockImpl& block,
                                                                                                RegisterClassification classify,
                                                                                                bool asLvalue)
    {
      if (asLvalue) return RegisterPtr ();
      
      SequenceBuilder& seq (*(block.GetSequenceBuilder()));
      auto targetType = target->GetValueType();
      auto valueType = value->GetValueType();
      if (!targetType || !valueType) return RegisterPtr(); // Assume error already handled

      RegisterPtr exprDestinationReg;
      // Evaluate 'value'
      exprDestinationReg = value->AddToSequence (block, Intermediate);
      if (!exprDestinationReg) return RegisterPtr(); // Assume error already handled
      NameImplPtr targetName (target->GetExpressionName());
      NameImplPtr valueName (value->GetExpressionName());
      if (!valueType->IsEqual (*(targetType.get())))
      {
        // Set up register for left-side value
        RegisterPtr targetReg (target->AddToSequence (block, Intermediate, true));
        if (!targetReg) return RegisterPtr(); // Assume error already handled
        // Generate cast to targetReg
        auto targetCast = handler->GenerateCast (seq, targetReg, targetType.get(),
                                                 exprDestinationReg, valueType.get());
        if (targetCast.has_error())
        {
          ExpressionError (targetCast.error ());
          return RegisterPtr ();
        }

        value->AddToSequencePostAction (block, exprDestinationReg, false);
        target->AddToSequencePostAction (block, targetReg, true);
                               
        return targetReg;
      }
      else
      {
        if (targetName && !valueName)
        {
          // See if we can 'force' the target register for the name's register
          if (block.OverrideNameRegister (targetName.get(), exprDestinationReg))
          {
            value->AddToSequencePostAction (block, exprDestinationReg, false);
            return exprDestinationReg;
          }
        }
        // Set up register for left-side value
        RegisterPtr targetReg (target->AddToSequence (block, Intermediate, true));
        if (!targetReg) return RegisterPtr(); // Assume error already handled
        // Generate another assignment from exprDestinationReg to targetReg
        SequenceOpPtr seqOp;
        seqOp = SequenceOpPtr (new SequenceOpAssign (targetReg, exprDestinationReg));
        seq.AddOp (seqOp);
        
        value->AddToSequencePostAction (block, exprDestinationReg, false);
        target->AddToSequencePostAction (block, targetReg, true);
        
        return targetReg;
      }      
    }
  } // namespace intermediate
} // namespace s1
