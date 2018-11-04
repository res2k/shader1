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

#include "FunctionCallExpressionImpl.h"

#include "BlockImpl.h"
#include "intermediate/Diagnostics.h"
#include "intermediate/SequenceBuilder.h"
#include "intermediate/SequenceOp/SequenceOpBuiltinCall.h"
#include "intermediate/SequenceOp/SequenceOpFunctionCall.h"
#include "intermediate/SequenceOp/SequenceOpMatrixLinAlgMul.h"
#include "intermediate/SequenceOp/SequenceOpSampleTexture.h"
#include "intermediate/SequenceOp/SequenceOpVectorCross.h"
#include "intermediate/SequenceOp/SequenceOpVectorDot.h"
#include "intermediate/SequenceOp/SequenceOpVectorLength.h"
#include "intermediate/SequenceOp/SequenceOpVectorNormalize.h"
#include "semantics/Function.h"
#include "semantics/Name.h"
#include "ScopeImpl.h"

namespace s1
{
  namespace intermediate
  {
    IntermediateGeneratorSemanticsHandler::FunctionCallExpressionImpl::FunctionCallExpressionImpl (
      IntermediateGeneratorSemanticsHandler* handler,
      ExpressionContext&& context,
      semantics::NameFunction* functionName,
      const ExpressionVector& params)
       : ExpressionImpl (handler, std::move (context)), functionName (functionName), params (params), overloadSelected (false)
    {
    }

    bool IntermediateGeneratorSemanticsHandler::FunctionCallExpressionImpl::SelectOverload ()
    {
      if (overloadSelected) return (bool)overload;
      overloadSelected = true;

      auto funcScopeImpl = get_static_ptr<ScopeImpl> (functionName->GetOwnerScope());

      // Collect overload candidates
      std::vector<semantics::Type*> paramTypes;
      paramTypes.reserve (params.size());
      for (const auto& param : params)
      {
        auto paramExprImpl = get_static_ptr<ExpressionImpl> (param);
        paramTypes.push_back (paramExprImpl->GetValueType ().get());
      }
      auto candidates = functionName->CollectOverloadCandidates (paramTypes);

      if (candidates.size() == 0)
      {
        // No candidates
        ExpressionError (Error::NoMatchingFunctionOverload);
        return false;
      }
      else if (candidates.size() > 1)
      {
        // Ambiguous overload
        ExpressionError (Error::AmbiguousFunctionOverload);
        return false;
      }
      overload = candidates[0];

      const auto& formalParams = overload->GetParameters();
      for (size_t formal = 0, actual = 0; formal < formalParams.size(); formal++)
      {
        const auto& param (formalParams[formal]);

        ExpressionPtr paramExpr;
        assert (param.paramType == semantics::FunctionFormalParameter::ptUser);
        if (actual < params.size())
          paramExpr = params[actual++];
        else
          paramExpr = param.defaultValue;
        actualParams.push_back (paramExpr);
      }

      return true;
    }

    bool IntermediateGeneratorSemanticsHandler::FunctionCallExpressionImpl::FetchRegisters (BlockImpl& block,
                                                                                            FetchedRegs& fetchedRegs,
                                                                                            PostActions& postActions)
    {
      bool result = true;
      const auto& formalParams = overload->GetParameters();
      for (size_t i = 0; i < actualParams.size(); i++)
      {
        RegisterPtr reg1, reg2;
        auto paramExprImpl = get_static_ptr<ExpressionImpl> (actualParams[i]);
        if (formalParams[i].dir & semantics::FunctionFormalParameter::dirIn)
        {
          reg1 = paramExprImpl->AddToSequence (block, Intermediate, false);
          if (!reg1) { result = false; continue; } // Assume error already handled
          postActions.emplace_back (paramExprImpl, reg1, false);
        }
        if (formalParams[i].dir & semantics::FunctionFormalParameter::dirOut)
        {
          reg2 = paramExprImpl->AddToSequence (block, Intermediate, true);
          if (!reg2) { result = false; continue; } // Assume error already handled
          postActions.emplace_back (paramExprImpl, reg2, false);
        }
        fetchedRegs.emplace_back (reg1, reg2);
      }

      return result;
    }

    SequenceOpPtr
    IntermediateGeneratorSemanticsHandler::FunctionCallExpressionImpl::BuiltinOp (semantics::BuiltinFunction* builtin,
                                                                                  RegisterPtr destination,
                                                                                  const std::vector<RegisterPtr>& inParams)
    {
      switch (builtin->GetBuiltin())
      {
      case semantics::Builtin::Pow:
        return new SequenceOpBuiltinCall (destination, intermediate::pow, inParams);
      case semantics::Builtin::VecDot:
        S1_ASSERT (inParams.size () == 2, SequenceOpPtr ());
        return new SequenceOpVectorDot (destination, inParams[0], inParams[1]);
      case semantics::Builtin::VecCross:
        S1_ASSERT (inParams.size () == 2, SequenceOpPtr ());
        return new SequenceOpVectorCross (destination, inParams[0], inParams[1]);
      case semantics::Builtin::MatrixLinAlgMul:
        S1_ASSERT (inParams.size () == 2, SequenceOpPtr ());
        return new SequenceOpMatrixLinAlgMul (destination, inParams[0], inParams[1]);
      case semantics::Builtin::VecNormalize:
        S1_ASSERT (inParams.size () == 1, SequenceOpPtr ());
        return new SequenceOpVectorNormalize (destination, inParams[0]);
      case semantics::Builtin::VecLength:
        S1_ASSERT (inParams.size () == 1, SequenceOpPtr ());
        return new SequenceOpVectorLength (destination, inParams[0]);
      case semantics::Builtin::SampleTex1D:
        S1_ASSERT (inParams.size () == 2, SequenceOpPtr ());
        return new SequenceOpSampleTexture (destination, SequenceVisitor::tex1D, inParams[0], inParams[1]);
      case semantics::Builtin::SampleTex2D:
        S1_ASSERT (inParams.size () == 2, SequenceOpPtr ());
        return new SequenceOpSampleTexture (destination, SequenceVisitor::tex2D, inParams[0], inParams[1]);
      case semantics::Builtin::SampleTex3D:
        S1_ASSERT (inParams.size () == 2, SequenceOpPtr ());
        return new SequenceOpSampleTexture (destination, SequenceVisitor::tex3D, inParams[0], inParams[1]);
      case semantics::Builtin::SampleTexCUBE:
        S1_ASSERT (inParams.size () == 2, SequenceOpPtr ());
        return new SequenceOpSampleTexture (destination, SequenceVisitor::texCUBE, inParams[0], inParams[1]);
      case semantics::Builtin::Min:
        return new SequenceOpBuiltinCall (destination, intermediate::min, inParams);
      case semantics::Builtin::Max:
        return new SequenceOpBuiltinCall (destination, intermediate::max, inParams);
      }
      S1_ASSERT_NOT_REACHED(SequenceOpPtr());
    }

    semantics::TypePtr IntermediateGeneratorSemanticsHandler::FunctionCallExpressionImpl::GetValueType ()
    {
      if (!SelectOverload ()) return nullptr; // Assume error already handled

      return overload->GetReturnType();
    }

    RegisterPtr IntermediateGeneratorSemanticsHandler::FunctionCallExpressionImpl::AddToSequence (BlockImpl& block,
                                                                                                  RegisterClassification classify,
                                                                                                  bool asLvalue)
    {
      if (asLvalue) return RegisterPtr();

      if (!SelectOverload ()) return RegisterPtr(); // Assume error already handled

      FetchedRegs fetchedRegs;
      PostActions postActions;
      if (!FetchRegisters (block, fetchedRegs, postActions)) return RegisterPtr(); // Assume error already handled

      bool paramsOkay = true;
      std::vector<RegisterPtr> inParams;
      std::vector<RegisterPtr> outParams;
      const auto& formalParams = overload->GetParameters();
      for (size_t i = 0; i < formalParams.size(); i++)
      {
        const auto& param (formalParams[i]);
        if (param.dir & semantics::FunctionFormalParameter::dirIn)
        {
          auto paramExprImpl = get_static_ptr<ExpressionImpl> (actualParams[i]);
          auto paramExprType = paramExprImpl->GetValueType();
          if (!paramExprType) { paramsOkay = false; continue; } // Assume error already handled
          RegisterPtr inReg (fetchedRegs[i].first);
          assert (inReg);
          auto formalParamType = param.type.get();
          if (!paramExprType->IsEqual (*formalParamType))
          {
            RegisterPtr targetReg (handler->AllocateRegister (*(block.GetSequenceBuilder()), formalParamType, Intermediate));
            auto paramCast = handler->GenerateCast (*(block.GetSequenceBuilder()), targetReg, formalParamType, inReg, paramExprType.get());
            if (paramCast.has_error ())
            {
              ExpressionError (paramCast.error ());
              paramsOkay = false;
              continue;
            }
            inReg = targetReg;
          }
          inParams.push_back (inReg);
        }
        if (param.dir & semantics::FunctionFormalParameter::dirOut)
        {
          RegisterPtr outReg (fetchedRegs[i].second);
          if (!outReg)
          {
            ExpressionError (Error::ActualParameterNotAnLvalue);
            paramsOkay = false;
            continue;
          }
          outParams.push_back (outReg);
        }
      }

      if (!paramsOkay) return RegisterPtr();

      RegisterPtr destination;
      auto retType (GetValueType());
      if (!retType) return RegisterPtr(); // Assume error already handled
      if (!retType->IsEqual (*(handler->GetVoidType())))
        destination = handler->AllocateRegister (*(block.GetSequenceBuilder()), retType, classify);

      SequenceOpPtr seqOp;
      if (auto builtin = semantics::BuiltinFunction::upcast (overload.get()))
      {
        seqOp = BuiltinOp (builtin, destination, inParams);
      }
      else
      {
        if (destination)
          outParams.insert (outParams.begin(), destination);
        seqOp = new SequenceOpFunctionCall (handler->GetDecoratedIdentifier (overload.get()), inParams, outParams);
      }
      block.GetSequenceBuilder()->AddOp (seqOp);

      for (PostActions::const_iterator postAction (postActions.begin());
           postAction != postActions.end();
           ++postAction)
      {
        postAction->expr->AddToSequencePostAction (block, postAction->reg, postAction->lValue);
      }

      return destination;
    }
  } // namespace intermediate
} // namespace s1
