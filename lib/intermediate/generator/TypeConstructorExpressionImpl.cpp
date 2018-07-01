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

#include "intermediate/Diagnostics.h"
#include "intermediate/SequenceBuilder.h"
#include "intermediate/SequenceOp/SequenceOpAssign.h"
#include "intermediate/SequenceOp/SequenceOpExtractVectorComponent.h"
#include "intermediate/SequenceOp/SequenceOpMakeArray.h"
#include "intermediate/SequenceOp/SequenceOpMakeMatrix.h"
#include "intermediate/SequenceOp/SequenceOpMakeVector.h"

#include "BlockImpl.h"
#include "TypeConstructorExpressionImpl.h"

namespace s1
{
  namespace intermediate
  {
    IntermediateGeneratorSemanticsHandler::TypeConstructorExpressionImpl::TypeConstructorExpressionImpl (
      IntermediateGeneratorSemanticsHandler* handler,
      ExpressionContext&& context,
      semantics::Type* type,
      const ExpressionVector& params)
     : ExpressionImpl (handler, std::move (context)), type (type), params (params)
    {
    }

    bool IntermediateGeneratorSemanticsHandler::TypeConstructorExpressionImpl::ExtractBaseExpressionRegs (BlockImpl& block,
                                                                                                          std::vector<RegisterPtr>& regs,
                                                                                                          PostActionList& postActions)
    {
      bool successful = true;
      SequenceBuilder& seq (*(block.GetSequenceBuilder()));
      
      auto targetBaseType = handler->CreateType (type->GetVMBase());
          
      for (const auto& expr : params)
      {
        auto exprImpl = get_static_ptr<ExpressionImpl> (expr);
        auto exprType = exprImpl->GetValueType();
        if (!exprType) { successful = false; continue; } // Assume error already handled
        RegisterPtr srcExprReg (exprImpl->AddToSequence (block, Intermediate, false));
        if (!srcExprReg) { successful = false; continue; } // Assume error already handled
        postActions.emplace_back (exprImpl, srcExprReg);
        
        switch (exprType->GetTypeClass())
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
              RegisterPtr srcReg (handler->AllocateRegister (seq, targetBaseType, Intermediate));
              auto srcCast = handler->GenerateCast (seq, srcReg, targetBaseType.get(), srcExprReg, exprType.get());
              if (srcCast.has_error ())
              {
                ExpressionError (srcCast.error ());
                successful = false;
                continue;
              }
              regs.push_back (srcReg);
            }
          }
          break;
        case semantics::Type::Vector:
          {
            // extract components
            auto exprCompType = handler->CreateType (exprType->GetVMBase());
            for (unsigned int c = 0; c < exprType->GetVectorTypeComponents(); c++)
            {
              RegisterPtr compReg (handler->AllocateRegister (seq, exprCompType, Intermediate));
              SequenceOpPtr extractOp (new SequenceOpExtractVectorComponent (compReg, srcExprReg, c));
              seq.AddOp (extractOp);
              if (targetBaseType->IsEqual (*exprCompType))
              {
                regs.push_back (compReg);
              }
              else
              {
                RegisterPtr srcReg (handler->AllocateRegister (seq, targetBaseType, Intermediate));
                auto srcCast = handler->GenerateCast (seq, srcReg, targetBaseType.get(), compReg, exprCompType.get());
                if (srcCast.has_error ())
                {
                  ExpressionError (srcCast.error ());
                  successful = false;
                  continue;
                }
                regs.push_back (srcReg);
              }
            }
          }
          break;
        }
      }
      return successful;
    }

    RegisterPtr
    IntermediateGeneratorSemanticsHandler::TypeConstructorExpressionImpl::AddToSequence (BlockImpl& block, 
                                                                                         RegisterClassification classify,
                                                                                         bool asLvalue)
    {
      if (asLvalue) return RegisterPtr();
      
      SequenceBuilder& seq (*(block.GetSequenceBuilder()));
      
      switch (type->GetTypeClass())
      {
      case semantics::Type::Base:
        {
          if (params.size() > 1)
          {
            ExpressionError (Error::TooManyTypeCtorArgs);
            return RegisterPtr();
          }
          if (params.size() < 1)
          {
            ExpressionError (Error::TooFewTypeCtorArgs);
            return RegisterPtr();
          }
          
          RegisterPtr targetReg (handler->AllocateRegister (seq, type, classify));
          ExpressionPtr srcExpr (params[0]);
          auto srcExprImpl = get_static_ptr<ExpressionImpl> (srcExpr);
          RegisterPtr srcReg (srcExprImpl->AddToSequence (block, Intermediate, false));
          if (!srcReg) return RegisterPtr(); // Assume error already handled
          auto srcType = srcExprImpl->GetValueType();
          if (!srcType) return RegisterPtr(); // Assume error already handled
          if (type->IsEqual (*srcType))
          {
            // If type is the same, just generate assignment
            SequenceOpPtr seqOp (new SequenceOpAssign (targetReg, srcReg));
            seq.AddOp (seqOp);
          }
          else
          {
            // otherwise, generate cast
            auto targetCast = handler->GenerateCast (seq, targetReg, type.get(), srcReg, srcType.get());
            if (targetCast.has_error ())
            {
              ExpressionError (targetCast.error ());
              return RegisterPtr ();
            }
          }
          srcExprImpl->AddToSequencePostAction (block, srcReg, false);
          return targetReg;
        }
        break;
      case semantics::Type::Matrix:
      case semantics::Type::Vector:
        {
          // Extract operands of base type from params (extract vector comps etc.)
          std::vector<RegisterPtr> srcRegs;
          PostActionList postActions;
          if (!ExtractBaseExpressionRegs (block, srcRegs, postActions)) return RegisterPtr(); // Assume error already handled
          
          unsigned int desiredDim;
          if (type->GetTypeClass() == semantics::Type::Vector)
            desiredDim = type->GetVectorTypeComponents();
          else
            desiredDim = type->GetMatrixTypeCols() * type->GetMatrixTypeRows();
          
          if ((type->GetTypeClass() == semantics::Type::Vector) && (srcRegs.size() == 1))
          {
            // Replicate input
            for (unsigned int i = 1; i < desiredDim; i++)
              srcRegs.push_back (srcRegs[0]);
          }
          else
          {
            if (srcRegs.size() > desiredDim)
            {
              ExpressionError (Error::TooManyTypeCtorArgs);
              return RegisterPtr();
            }
            if (srcRegs.size() < desiredDim)
            {
              ExpressionError (Error::TooFewTypeCtorArgs);
              return RegisterPtr ();
            }
          }
          
          auto targetBaseType = type->GetVMBase();
          
          RegisterPtr targetReg (handler->AllocateRegister (seq, type, classify));
          BasicType vecType;
          switch (targetBaseType)
          {
          case semantics::BaseType::Bool: 	vecType = intermediate::BasicType::Bool; break;
          case semantics::BaseType::Int: 	vecType = intermediate::BasicType::Int; break;
          case semantics::BaseType::UInt: 	vecType = intermediate::BasicType::UInt; break;
          case semantics::BaseType::Float: 	vecType = intermediate::BasicType::Float; break;
          default:	return RegisterPtr();
          }
          
          SequenceOpPtr seqOp;
          if (type->GetTypeClass() == semantics::Type::Vector)
            seqOp = new SequenceOpMakeVector (targetReg, vecType, srcRegs);
          else
            seqOp = new SequenceOpMakeMatrix (targetReg, vecType, 
                                              type->GetMatrixTypeRows(),
                                              type->GetMatrixTypeCols(),
                                              srcRegs);
          seq.AddOp (seqOp);
          
          for (PostActionList::const_iterator postAction (postActions.begin());
               postAction != postActions.end();
               ++postAction)
          {
            postAction->first->AddToSequencePostAction (block, postAction->second, false);
          }
          
          return targetReg;
        }
        break;
      case semantics::Type::Sampler:
        // ...
        return RegisterPtr();
      case semantics::Type::Array:
        {
          RegisterPtr targetReg (handler->AllocateRegister (seq, type, classify));
          std::vector<RegisterPtr> srcRegs;
          
          auto targetBaseType = type->GetArrayBase();

          bool sourcesOk = true;
          for (const auto& expr : params)
          {
            auto exprImpl = get_static_ptr<ExpressionImpl> (expr);
            auto exprType = exprImpl->GetValueType();
            if (!exprType) { sourcesOk = false; continue; } // Assume error already handled
            RegisterPtr srcExprReg (exprImpl->AddToSequence (block, Intermediate, false));
            if (!srcExprReg) { sourcesOk = false; continue; } ; // Assume error already handled
            
            // Add expression as-is
            if (targetBaseType->IsEqual (*exprType))
            {
              srcRegs.push_back (srcExprReg);
            }
            else
            {
              RegisterPtr srcReg (handler->AllocateRegister (seq, targetBaseType, Intermediate));
              auto srcCast = handler->GenerateCast (seq, srcReg, targetBaseType, srcExprReg, exprType.get());
              if (srcCast.has_error ())
              {
                ExpressionError (srcCast.error ());
                sourcesOk = false;
                continue;
              }
              srcRegs.push_back (srcReg);
            }
          }
          if (!sourcesOk) return RegisterPtr();

          SequenceOpPtr seqOp (new SequenceOpMakeArray (targetReg, srcRegs));
          seq.AddOp (seqOp);
          return targetReg;
        }
        break;
      default:
        return RegisterPtr();
      }
    }
  } // namespace intermediate
} // namespace s1
