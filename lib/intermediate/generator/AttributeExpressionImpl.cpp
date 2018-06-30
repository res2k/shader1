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
#include "base/intrusive_ptr.h"

#include "AttributeExpressionImpl.h"

#include "intermediate/Diagnostics.h"
#include "intermediate/SequenceBuilder.h"
#include "intermediate/SequenceOp/SequenceOpExtractVectorComponent.h"
#include "intermediate/SequenceOp/SequenceOpGetArrayLength.h"
#include "intermediate/SequenceOp/SequenceOpMakeVector.h"

#include "BlockImpl.h"

namespace s1
{
  namespace intermediate
  {
    IntermediateGeneratorSemanticsHandler::AttributeExpressionImpl::AttributeExpressionImpl (IntermediateGeneratorSemanticsHandler* handler,
                                                                                             ExpressionContext&& context,
                                                                                             Expression* baseExpr,
                                                                                             const IntermediateGeneratorSemanticsHandler::Attribute& attr)
     : ExpressionImpl (handler, std::move (context)), baseExpr (baseExpr), attr (attr)
    {}
    
    semantics::TypePtr IntermediateGeneratorSemanticsHandler::AttributeExpressionImpl::GetValueType ()
    {
      auto exprImpl = get_static_ptr<ExpressionImpl> (baseExpr);
      auto exprValueType = exprImpl->GetValueType();
      if (!exprValueType) return nullptr; // Assume error already handled
      return handler->GetAttributeType (exprValueType.get(), attr);
    }
    
    RegisterPtr IntermediateGeneratorSemanticsHandler::AttributeExpressionImpl::AddToSequence (BlockImpl& block,
                                                                                               RegisterClassification classify,
                                                                                               bool asLvalue)
    {
      SequenceBuilder& seq (*(block.GetSequenceBuilder()));
      
      switch (attr.attrClass)
      {
      default:
      case IntermediateGeneratorSemanticsHandler::Attribute::Unknown:
        S1_ASSERT_NOT_REACHED (RegisterPtr ());
      case IntermediateGeneratorSemanticsHandler::Attribute::arrayLength:
        {
          if (asLvalue) return RegisterPtr ();
          
          RegisterPtr targetReg (handler->AllocateRegister (seq, GetValueType(), Intermediate));
          auto exprImpl = get_static_ptr<ExpressionImpl> (baseExpr);
          RegisterPtr exprValueReg (exprImpl->AddToSequence (block, Intermediate, false));
          if (!exprValueReg) return RegisterPtr(); // Assume error already handled
          
          SequenceOpPtr seqOp (new SequenceOpGetArrayLength (targetReg, exprValueReg));
          seq.AddOp (seqOp);
          
          exprImpl->AddToSequencePostAction (block, exprValueReg, false);
          
          return targetReg;
        }
        break;
      case IntermediateGeneratorSemanticsHandler::Attribute::matrixRow:
      case IntermediateGeneratorSemanticsHandler::Attribute::matrixCol:
      case IntermediateGeneratorSemanticsHandler::Attribute::matrixTranspose:
      case IntermediateGeneratorSemanticsHandler::Attribute::matrixInvert:
        {
          if (asLvalue) return RegisterPtr ();
          
          return RegisterPtr ();
        }
        break;
      case IntermediateGeneratorSemanticsHandler::Attribute::vectorSwizzle:
        {
          if (asLvalue)
          {
            RegisterPtr targetReg (handler->AllocateRegister (seq, GetValueType(), Intermediate));
            return targetReg;
          }
          else
          {
            auto exprImpl = get_static_ptr<ExpressionImpl> (baseExpr);
            RegisterPtr exprValueReg (exprImpl->AddToSequence (block, Intermediate, false));
            if (!exprValueReg) return RegisterPtr(); // Assume error already handled
            
            auto valueType = GetValueType();
            RegisterPtr targetReg;
            if (attr.swizzleCompNum > 1)
            {
              // multi-component swizzle
              targetReg = handler->AllocateRegister (seq, valueType, classify);
              auto valueCompType = valueType->GetAVMBase();
              std::vector<RegisterPtr> compRegs;
              for (unsigned int c = 0; c < attr.swizzleCompNum; c++)
              {
                RegisterPtr compReg (handler->AllocateRegister (seq, valueCompType, classify));
                SequenceOpPtr seqOp (new SequenceOpExtractVectorComponent (compReg, exprValueReg, attr.GetSwizzleComp (c)));
                seq.AddOp (seqOp);
                compRegs.push_back (compReg);
              }
              BasicType vecType;
              switch (valueCompType->GetBaseType())
              {
              case semantics::BaseType::Bool: 	vecType = intermediate::BasicType::Bool; break;
              case semantics::BaseType::Int: 	vecType = intermediate::BasicType::Int; break;
              case semantics::BaseType::UInt: 	vecType = intermediate::BasicType::UInt; break;
              case semantics::BaseType::Float: vecType = intermediate::BasicType::Float; break;
              default:	return RegisterPtr();
              }
              SequenceOpPtr seqOp (new SequenceOpMakeVector (targetReg, vecType, compRegs));
              seq.AddOp (seqOp);
            }
            else
            {
              RegisterPtr compReg (handler->AllocateRegister (seq, valueType, classify));
              SequenceOpPtr seqOp (new SequenceOpExtractVectorComponent (compReg, exprValueReg, attr.GetSwizzleComp (0)));
              seq.AddOp (seqOp);
              targetReg = compReg;
            }
            
            exprImpl->AddToSequencePostAction (block, exprValueReg, false);
            
            return targetReg;
          }
        }
        break;
      }
    }

    void IntermediateGeneratorSemanticsHandler::AttributeExpressionImpl::AddToSequencePostAction (BlockImpl& block,
                                                                                                  const RegisterPtr& target,
                                                                                                  bool wasLvalue)
    {
      if (!wasLvalue) return;
      
      SequenceBuilder& seq (*(block.GetSequenceBuilder()));
      
      // Generate assignment from register to actual target
      auto exprImpl = get_static_ptr<ExpressionImpl> (baseExpr);
      
      RegisterPtr originalTarget (exprImpl->AddToSequence (block, Intermediate, false));
      if (!originalTarget) return; // Assume error already handled
      exprImpl->AddToSequencePostAction (block, originalTarget, false);
      
      RegisterPtr actualTarget (exprImpl->AddToSequence (block, Intermediate, true));
      if (!actualTarget)
      {
        ExpressionError (Error::SwizzledExpressionNotAnLvalue);
        return;
      }
      
      auto originalValueType = exprImpl->GetValueType();
      if (!originalValueType) return; // Assume error already handled
      auto originalValueCompType = originalValueType->GetAVMBase();
      auto valueType = GetValueType();
      
      unsigned int compDefined = 0;
      std::vector<RegisterPtr> compRegs;
      compRegs.insert (compRegs.begin(), originalValueType->GetVectorTypeComponents(), RegisterPtr ());
      
      for (unsigned int c = 0; c < attr.swizzleCompNum; c++)
      {
        unsigned int comp = attr.GetSwizzleComp (c);
        if (compDefined & (1 << comp))
        {
          ExpressionError (Error::MultipleUseOfComponentInLvalueSwizzle);
          return;
        }

        if (valueType->GetTypeClass() == semantics::Type::Vector)
        {
          RegisterPtr compReg (handler->AllocateRegister (seq, originalValueCompType, Intermediate));
          SequenceOpPtr seqOp (new SequenceOpExtractVectorComponent (compReg, target, c));
          seq.AddOp (seqOp);
          compRegs[comp] = compReg;
        }
        else
        {
          compRegs[comp] = target;
        }
        
        compDefined |= 1 << comp;
      }
      
      for (unsigned int c = 0; c < originalValueType->GetVectorTypeComponents(); c++)
      {
        if (!(compDefined & (1 << c)))
        {
          RegisterPtr compReg (handler->AllocateRegister (seq, originalValueCompType, Intermediate));
          SequenceOpPtr seqOp (new SequenceOpExtractVectorComponent (compReg, originalTarget, c));
          seq.AddOp (seqOp);
          compRegs[c] = compReg;
        }
      }
      BasicType vecType;
      switch (originalValueCompType->GetBaseType())
      {
      case semantics::BaseType::Bool: 	vecType = intermediate::BasicType::Bool; break;
      case semantics::BaseType::Int: 	vecType = intermediate::BasicType::Int; break;
      case semantics::BaseType::UInt: 	vecType = intermediate::BasicType::UInt; break;
      case semantics::BaseType::Float: 	vecType = intermediate::BasicType::Float; break;
      default:		return;
      }
      SequenceOpPtr seqOp (new SequenceOpMakeVector (actualTarget, vecType, compRegs));
      seq.AddOp (seqOp);
      
      exprImpl->AddToSequencePostAction (block, actualTarget, true);
    }
  } // namespace intermediate
} // namespace s1
