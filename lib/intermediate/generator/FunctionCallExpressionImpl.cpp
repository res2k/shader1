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
#include "Builtin.h"
#include "intermediate/Diagnostics.h"
#include "intermediate/SequenceBuilder.h"
#include "intermediate/SequenceOp/SequenceOpBuiltinCall.h"
#include "intermediate/SequenceOp/SequenceOpFunctionCall.h"
#include "ScopeImpl.h"

namespace s1
{
  namespace intermediate
  {
    IntermediateGeneratorSemanticsHandler::FunctionCallExpressionImpl::FunctionCallExpressionImpl (
      IntermediateGeneratorSemanticsHandler* handler,
      ExpressionContext&& context,
      semantics::Name* functionName,
      const ExpressionVector& params)
       : ExpressionImpl (handler, std::move (context)), functionName (functionName), params (params), overloadSelected (false)
    {
    }

    bool IntermediateGeneratorSemanticsHandler::FunctionCallExpressionImpl::SelectOverload ()
    {
      if (overloadSelected) return (bool)overload;
      overloadSelected = true;

      auto nameImpl = get_static_ptr<NameImpl> (functionName);
      auto funcScopeImpl = get_static_ptr<ScopeImpl> (nameImpl->GetOwnerScope());

      // Collect overload candidates
      ScopeImpl::FunctionInfoVector candidates (funcScopeImpl->CollectOverloadCandidates (functionName.get(), params));

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

      for (size_t formal = 0, actual = 0; formal < overload->params.size(); formal++)
      {
        const ScopeImpl::FunctionFormalParameter& param (overload->params[formal]);

        ExpressionPtr paramExpr;
        assert (param.paramType == semantics::Scope::ptUser);
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
      for (size_t i = 0; i < actualParams.size(); i++)
      {
        RegisterPtr reg1, reg2;
        auto paramExprImpl = get_static_ptr<ExpressionImpl> (actualParams[i]);
        if (overload->params[i].dir & ScopeImpl::dirIn)
        {
          reg1 = paramExprImpl->AddToSequence (block, Intermediate, false);
          if (!reg1) { result = false; continue; } // Assume error already handled
          postActions.emplace_back (paramExprImpl, reg1, false);
        }
        if (overload->params[i].dir & ScopeImpl::dirOut)
        {
          reg2 = paramExprImpl->AddToSequence (block, Intermediate, true);
          if (!reg2) { result = false; continue; } // Assume error already handled
          postActions.emplace_back (paramExprImpl, reg2, false);
        }
        fetchedRegs.emplace_back (reg1, reg2);
      }

      return result;
    }

    semantics::TypePtr IntermediateGeneratorSemanticsHandler::FunctionCallExpressionImpl::GetValueType ()
    {
      if (!SelectOverload ()) return nullptr; // Assume error already handled

      return overload->returnType;
    }

    RegisterPtr IntermediateGeneratorSemanticsHandler::FunctionCallExpressionImpl::AddToSequence (BlockImpl& block,
                                                                                                  RegisterClassification classify,
                                                                                                  bool asLvalue)
    {
      if (asLvalue) return RegisterPtr();

      if (!SelectOverload ()) return RegisterPtr(); // Assume error already handled

      if (overload->identifier.isEmpty()) return RegisterPtr();

      FetchedRegs fetchedRegs;
      PostActions postActions;
      if (!FetchRegisters (block, fetchedRegs, postActions)) return RegisterPtr(); // Assume error already handled

      bool paramsOkay = true;
      std::vector<RegisterPtr> inParams;
      std::vector<RegisterPtr> outParams;
      for (size_t i = 0; i < overload->params.size(); i++)
      {
        const ScopeImpl::FunctionFormalParameter& param (overload->params[i]);
        if (param.dir & ScopeImpl::dirIn)
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
        if (param.dir & ScopeImpl::dirOut)
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
      if (overload->builtin)
      {
        seqOp = overload->builtin->GetSeqOpFactory () (destination, inParams);
      }
      else
      {
        if (destination)
          outParams.insert (outParams.begin(), destination);
        seqOp = new SequenceOpFunctionCall (overload->identifier, inParams, outParams);
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
