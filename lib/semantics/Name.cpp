/*
    Shader1
    Copyright (c) 2018 Frank Richter


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
#include "semantics/Name.h"

#include "semantics/Function.h"

namespace s1
{
  namespace semantics
  {
    NameFunction::NameFunction (Scope* ownerScope, const uc::String& identifier)
      : Name (ownerScope, identifier) {}

    NameFunction::~NameFunction () {}

    semantics::Function* NameFunction::AddOverload (Type* returnType, const FunctionFormalParameters& params, Scope* funcScope, Block* body)
    {
      FunctionPtr newFunc = new semantics::Function (this, returnType, params, funcScope, body);
      overloads.push_back (newFunc);
      return newFunc.get();
    }

    semantics::BuiltinFunction* NameFunction::AddBuiltin (Type* returnType, const FunctionFormalParameters& params, Builtin which)
    {
      BuiltinFunctionPtr newFunc = new semantics::BuiltinFunction (this, returnType, params, which);
      overloads.push_back (newFunc);
      return newFunc.get();
    }

    NameFunction::BaseFunctionPtrVec NameFunction::CollectOverloadCandidates (const std::vector<Type*>& paramTypes) const
    {
      BaseFunctionPtrVec candidates;

      // First, look for an exact parameters type match
      for (const auto& candidate : overloads)
      {
        const auto& candidateParams = candidate->GetParameters();
        if (paramTypes.size() > candidateParams.size()) continue;

        bool abort = false;
        size_t formal = 0, actual = 0;
        for (; actual < paramTypes.size(); formal++)
        {
          // Only consider user-specified parameters for matching
          if (candidateParams[formal].paramType != FunctionFormalParameter::ptUser) continue;

          auto paramType = paramTypes[actual];
          auto formalParamType = candidateParams[formal].type.get();
          // No exact type match? Skip
          if (!paramType->IsEqual (*formalParamType))
          {
            abort = true;
            break;
          }
          actual++;
        }
        if (abort) continue;
        for (; formal < candidateParams.size(); formal++)
        {
          // Only consider user-specified parameters for matching
          if (candidateParams[formal].paramType != FunctionFormalParameter::ptUser) continue;

          // Leftover parameter + no default value? Skip
          if (!candidateParams[formal].defaultValue)
          {
            abort = true;
            break;
          }
        }
        if (abort) continue;

        candidates.push_back (candidate);
      }

      // Second, look for a lossless parameters type match
      if (candidates.size() == 0)
      {
        for (const auto& candidate : overloads)
        {
          const auto& candidateParams = candidate->GetParameters();
          if (paramTypes.size() > candidateParams.size()) continue;

          bool abort = false;
          size_t formal = 0, actual = 0;
          for (; actual < paramTypes.size(); formal++)
          {
            // Only consider user-specified parameters for matching
            if (candidateParams[formal].paramType != FunctionFormalParameter::ptUser) continue;

            auto paramType = paramTypes[actual];
            auto formalParamType = candidateParams[formal].type.get();
            bool match;
            if (candidateParams[formal].dir & FunctionFormalParameter::dirOut)
              // Output parameters must _always_ match exactly
              match = paramType->IsEqual (*formalParamType);
            else
              // Input parameters can match losslessy
              match = paramType->CompatibleLossless (*formalParamType);
            // No type match? Skip
            if (!match)
            {
              abort = true;
              break;
            }
            actual++;
          }
          if (abort) continue;
          for (; formal < candidateParams.size(); formal++)
          {
            // Only consider user-specified parameters for matching
            if (candidateParams[formal].paramType != FunctionFormalParameter::ptUser) continue;

            // Leftover parameter + no default value? Skip
            if (!candidateParams[formal].defaultValue)
            {
              abort = true;
              break;
            }
          }
          if (abort) continue;

          candidates.push_back (candidate);
        }
      }

      return candidates;
    }
  } // namespace semantics
} // namespace s1
