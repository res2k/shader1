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

#include "FunctionCallExpressionImpl.h"

#include "BlockImpl.h"
#include "Builtin.h"
#include "intermediate/Exception.h"
#include "intermediate/SequenceBuilder.h"
#include "intermediate/SequenceOp/SequenceOpBuiltinCall.h"
#include "intermediate/SequenceOp/SequenceOpFunctionCall.h"
#include "NameImpl.h"
#include "ScopeImpl.h"

#include <boost/make_shared.hpp>

namespace s1
{
  namespace intermediate
  {
    IntermediateGeneratorSemanticsHandler::FunctionCallExpressionImpl::FunctionCallExpressionImpl (
      IntermediateGeneratorSemanticsHandler* handler,
      ExpressionContext&& context,
      const NamePtr& functionName,
      const ExpressionVector& params)
       : ExpressionImpl (handler, std::move (context)), functionName (functionName), params (params), overloadSelected (false)
    {
    }

    void IntermediateGeneratorSemanticsHandler::FunctionCallExpressionImpl::SelectOverload ()
    {
      if (overloadSelected) return;
      overloadSelected = true;

      boost::shared_ptr<NameImpl> nameImpl (boost::static_pointer_cast<NameImpl> (functionName));
      boost::shared_ptr<ScopeImpl> funcScopeImpl (boost::static_pointer_cast<ScopeImpl> (ScopePtr (nameImpl->ownerScope)));

      // Collect overload candidates
      ScopeImpl::FunctionInfoVector candidates (funcScopeImpl->CollectOverloadCandidates (functionName, params));

      if (candidates.size() == 0)
      {
        // No candidates
        throw Exception (NoMatchingFunctionOverload);
      }
      else if (candidates.size() > 1)
      {
        // Ambiguous overload
        throw Exception (AmbiguousFunctionOverload);
      }
      overload = candidates[0];

      for (size_t formal = 0, actual = 0; formal < overload->params.size(); formal++)
      {
        const ScopeImpl::FunctionFormalParameter& param (overload->params[formal]);

        ExpressionPtr paramExpr;
        assert (param.paramType == SemanticsHandler::Scope::ptUser);
        if (actual < params.size())
          paramExpr = params[actual++];
        else
          paramExpr = param.defaultValue;
        actualParams.push_back (paramExpr);
      }
    }

    void IntermediateGeneratorSemanticsHandler::FunctionCallExpressionImpl::FetchRegisters (BlockImpl& block,
                                                                                            FetchedRegs& fetchedRegs,
                                                                                            PostActions& postActions)
    {
      for (size_t i = 0; i < actualParams.size(); i++)
      {
        RegisterPtr reg1, reg2;
        boost::shared_ptr<ExpressionImpl> paramExprImpl (boost::static_pointer_cast<ExpressionImpl> (actualParams[i]));
        if (overload->params[i].dir & ScopeImpl::dirIn)
        {
          reg1 = paramExprImpl->AddToSequence (block, Intermediate, false);
          postActions.emplace_back (paramExprImpl, reg1, false);
        }
        if (overload->params[i].dir & ScopeImpl::dirOut)
        {
          reg2 = paramExprImpl->AddToSequence (block, Intermediate, true);
          postActions.emplace_back (paramExprImpl, reg2, false);
        }
        fetchedRegs.emplace_back (reg1, reg2);
      }
    }

    boost::shared_ptr<IntermediateGeneratorSemanticsHandler::TypeImpl>
    IntermediateGeneratorSemanticsHandler::FunctionCallExpressionImpl::GetValueType ()
    {
      SelectOverload ();

      return boost::static_pointer_cast<TypeImpl> (overload->returnType);
    }

    RegisterPtr IntermediateGeneratorSemanticsHandler::FunctionCallExpressionImpl::AddToSequence (BlockImpl& block,
                                                                                                  RegisterClassification classify,
                                                                                                  bool asLvalue)
    {
      if (asLvalue) return RegisterPtr();

      SelectOverload ();

      if (overload->identifier.isEmpty()) return RegisterPtr();

      FetchedRegs fetchedRegs;
      PostActions postActions;
      FetchRegisters (block, fetchedRegs, postActions);

      std::vector<RegisterPtr> inParams;
      std::vector<RegisterPtr> outParams;
      for (size_t i = 0; i < overload->params.size(); i++)
      {
        const ScopeImpl::FunctionFormalParameter& param (overload->params[i]);
        if (param.dir & ScopeImpl::dirIn)
        {
          boost::shared_ptr<ExpressionImpl> paramExprImpl (boost::static_pointer_cast<ExpressionImpl> (actualParams[i]));
          boost::shared_ptr<TypeImpl> paramExprType (paramExprImpl->GetValueType());
          RegisterPtr inReg (fetchedRegs[i].first);
          assert (inReg);
          boost::shared_ptr<TypeImpl> formalParamType (boost::static_pointer_cast<TypeImpl> (param.type));
          if (!paramExprType->IsEqual (*formalParamType))
          {
            RegisterPtr targetReg (handler->AllocateRegister (*(block.GetSequenceBuilder()), formalParamType, Intermediate));
            handler->GenerateCast (*(block.GetSequenceBuilder()), targetReg, formalParamType, inReg, paramExprType);
            inReg = targetReg;
          }
          inParams.push_back (inReg);
        }
        if (param.dir & ScopeImpl::dirOut)
        {
          boost::shared_ptr<ExpressionImpl> paramExprImpl (boost::static_pointer_cast<ExpressionImpl> (actualParams[i]));
          RegisterPtr outReg (fetchedRegs[i].second);
          if (!outReg)
            throw Exception (ActualParameterNotAnLValue);
          outParams.push_back (outReg);
        }
      }

      RegisterPtr destination;
      boost::shared_ptr<TypeImpl> retType (GetValueType());
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
