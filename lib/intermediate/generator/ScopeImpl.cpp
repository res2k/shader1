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

#include "BlockImpl.h"
#include "Builtin.h"
#include "ScopeImpl.h"

#include "intermediate/Diagnostics.h"
#include "parser/Diagnostics.h"
#include "semantics/Function.h"
#include "semantics/Name.h"
#include "ExpressionImpl.h"

#include <boost/make_shared.hpp>
#include <limits.h>

namespace s1
{
  namespace intermediate
  {
    typedef semantics::NamePtr NamePtr;
    typedef semantics::BlockPtr BlockPtr;
    typedef semantics::FunctionPtr FunctionPtr;

    IntermediateGeneratorSemanticsHandler::ScopeImpl::result_NameFunctionPtr
    IntermediateGeneratorSemanticsHandler::ScopeImpl::CheckIdentifierIsFunction (const uc::String& identifier)
    {
      IdentifierMap::iterator ident = identifiers.find (identifier);
      if (ident != identifiers.end())
      {
        auto funcName = semantics::NameFunction::upcast (ident->second.get());
        if (!funcName)
        {
          return OUTCOME_V2_NAMESPACE::failure (Error::IdentifierAlreadyDeclared);
        }
        return funcName;
      }
      if (parent)
        return static_cast<ScopeImpl*> (parent)->CheckIdentifierIsFunction (identifier);
      return semantics::NameFunctionPtr ();
    }

    uc::String
    IntermediateGeneratorSemanticsHandler::ScopeImpl::DecorateIdentifier (const uc::String& identifier,
                                                                          const semantics::FunctionFormalParameters& params)
    {
      auto identifierDecorated = identifier;
      identifierDecorated.append ("$");
      int lastDir = -1;
      for (const auto& param : params)
      {
        if (param.dir != lastDir)
        {
          char dirStr[3] = { 0, 0, 0 };
          int c = 0;
          if (lastDir != -1) dirStr[c++] = '_';
          dirStr[c] = param.dir + '0';
          identifierDecorated.append (dirStr);
          lastDir = param.dir;
        }
        auto typeImpl = param.type.get();
        identifierDecorated.append (IntermediateGeneratorSemanticsHandler::GetTypeString (typeImpl));
      }
      return identifierDecorated;
    }

    IntermediateGeneratorSemanticsHandler::ScopeImpl::ScopeImpl (IntermediateGeneratorSemanticsHandler* handler,
                                                                 ScopeImpl* parent,
                                                                 semantics::ScopeLevel level,
                                                                 semantics::Type* funcReturnType)
     : Scope (parent), handler (handler), level (level), funcReturnType (funcReturnType)
    {}

    void IntermediateGeneratorSemanticsHandler::ScopeImpl::AddParameter (const semantics::FunctionFormalParameter& param)
    {
      // TODO: Keep? Or allow params shadow global vars?
      if (!CheckIdentifierUnique (param.identifier))
      {
        handler->ExpressionError (ExpressionContext(), Error::IdentifierAlreadyDeclared);
        return;
      }
      semantics::NameVariablePtr newName = new semantics::NameVariable (this, param);
      identifiers[param.identifier] = newName;
      newVars.push_back (newName);

      if ((param.dir & semantics::FunctionFormalParameter::dirOut) != 0)
        outputParams.push_back (param.identifier);
    }

    semantics::NameVariablePtr
    IntermediateGeneratorSemanticsHandler::ScopeImpl::AddVariable (semantics::TypePtr type, const uc::String& identifier,
                                                                   ExpressionPtr initialValue, bool constant)
    {
      if (!CheckIdentifierUnique (identifier))
      {
        handler->ExpressionError (ExpressionContext(), Error::IdentifierAlreadyDeclared);
        return semantics::NameVariablePtr();
      }
      semantics::NameVariablePtr newName = new semantics::NameVariable (this, identifier, type.get(), initialValue.get(), constant);
      identifiers[identifier] = newName;
      newVars.push_back (newName);
      varsInDeclOrder.push_back (newName);
      return newName;
    }

    semantics::NameTypeAliasPtr
    IntermediateGeneratorSemanticsHandler::ScopeImpl::AddTypeAlias (semantics::TypePtr aliasedType, const uc::String& identifier)
    {
      if (!CheckIdentifierUnique (identifier))
      {
        handler->ExpressionError (ExpressionContext(), Error::IdentifierAlreadyDeclared);
        return semantics::NameTypeAliasPtr();
      }
      semantics::NameTypeAliasPtr newName = new semantics::NameTypeAlias (this, identifier, aliasedType.get());
      identifiers[identifier] = newName;
      return newName;
    }

    FunctionPtr IntermediateGeneratorSemanticsHandler::ScopeImpl::AddFunction (semantics::TypePtr returnType,
                                                                               const uc::String& identifier,
                                                                               const semantics::FunctionFormalParameters& params)
    {
      if (level >= semantics::ScopeLevel::Function)
      {
        handler->ExpressionError (ExpressionContext(), Error::DeclarationNotAllowedInScope);
        return FunctionPtr();
      }
      auto funcIdentResult = CheckIdentifierIsFunction (identifier);
      if (!funcIdentResult)
      {
        handler->ExpressionError (ExpressionContext(), funcIdentResult.error());
        return FunctionPtr();
      }
      semantics::NameFunctionPtr funcName = std::move (funcIdentResult.value());
      if (!funcName)
      {
        funcName = new semantics::NameFunction (this, identifier);
        identifiers[identifier] = funcName;
      }

      semantics::ScopePtr funcScope;
      funcScope = handler->CreateScope (this, semantics::ScopeLevel::Function, returnType);
      auto funcScopeImpl = get_static_ptr<ScopeImpl> (funcScope);
      for (const auto& param : params)
      {
        funcScopeImpl->AddParameter (param);
      }
      BlockPtr newBlock (handler->CreateBlock (funcScope));

      FunctionPtr newFunction = funcName->AddOverload (returnType.get(), params, funcScope.get(), newBlock.get());

      FunctionInfoVector& functions = this->functions[identifier];
      FunctionInfoPtr funcInfo (boost::make_shared<FunctionInfo> ());
      funcInfo->functionObj = newFunction;
      // Decorate identifier with type info (so each overload gets a unique name)
      funcInfo->decoratedIdentifier = DecorateIdentifier (identifier, params);
      functions.push_back (funcInfo);

      functionsInDeclOrder.push_back (funcInfo);

      return newFunction;
    }

    void IntermediateGeneratorSemanticsHandler::ScopeImpl::AddBuiltinFunction (semantics::Builtin which,
                                                                               semantics::Type* returnType,
                                                                               const uc::String& identifier,
                                                                               const semantics::FunctionFormalParameters& formalParameters)
    {
      if (level >= semantics::ScopeLevel::Function)
      {
        handler->ExpressionError (ExpressionContext(), Error::DeclarationNotAllowedInScope);
        return;
      }

      auto funcIdentResult = CheckIdentifierIsFunction (identifier);
      if (!funcIdentResult)
      {
        handler->ExpressionError (ExpressionContext(), funcIdentResult.error());
        return;
      }
      semantics::NameFunctionPtr funcName = std::move (funcIdentResult.value());
      if (!funcName)
      {
        funcName = new semantics::NameFunction (this, identifier);
        identifiers[identifier] = funcName;
      }

      FunctionInfoVector& functions = this->functions[identifier];
      FunctionInfoPtr funcInfo (boost::make_shared<FunctionInfo> ());
      // Decorate identifier with type info (so each overload gets a unique name)
      funcInfo->decoratedIdentifier = DecorateIdentifier (identifier, formalParameters);
      funcInfo->functionObj = funcName->AddBuiltin (returnType, formalParameters, which);
      functions.push_back (funcInfo);

      functionsInDeclOrder.push_back (funcInfo);
    }

    IntermediateGeneratorSemanticsHandler::ScopeImpl::FunctionInfoVector
    IntermediateGeneratorSemanticsHandler::ScopeImpl::GetFunctions () const
    {
      return functionsInDeclOrder;
    }

    IntermediateGeneratorSemanticsHandler::ScopeImpl::FunctionInfoVector
    IntermediateGeneratorSemanticsHandler::ScopeImpl::CollectOverloadCandidates (semantics::Name* functionName, const ExpressionVector& params) const
    {
      FunctionInfoVector vec;
      FunctionsMap::const_iterator funcIt = functions.find (functionName->GetIdentifier());
      if (funcIt != functions.end())
      {
        // First, look for an exact parameters type match
        for (const auto& candidate : funcIt->second)
        {
          const auto& candidateParams = candidate->functionObj->GetParameters();
          if (params.size() > candidateParams.size()) continue;

          bool abort = false;
          size_t formal = 0, actual = 0;
          for (; actual < params.size(); formal++)
          {
            // Only consider user-specified parameters for matching
            if (candidateParams[formal].paramType != semantics::FunctionFormalParameter::ptUser) continue;

            auto exprImpl = get_static_ptr<ExpressionImpl> (params[actual]);
            auto paramType = exprImpl->GetValueType ();
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
            if (candidateParams[formal].paramType != semantics::FunctionFormalParameter::ptUser) continue;

            // Leftover parameter + no default value? Skip
            if (!candidateParams[formal].defaultValue)
            {
              abort = true;
              break;
            }
          }
          if (abort) continue;

          vec.push_back (candidate);
        }

        // Second, look for a lossless parameters type match
        if (vec.size() == 0)
        {
          for (const auto& candidate : funcIt->second)
          {
            const auto& candidateParams = candidate->functionObj->GetParameters();
            if (params.size() > candidateParams.size()) continue;

            bool abort = false;
            size_t formal = 0, actual = 0;
            for (; actual < params.size(); formal++)
            {
              // Only consider user-specified parameters for matching
              if (candidateParams[formal].paramType != semantics::FunctionFormalParameter::ptUser) continue;

              auto exprImpl = get_static_ptr<ExpressionImpl> (params[actual]);
              auto paramType = exprImpl->GetValueType ();
              auto formalParamType = candidateParams[formal].type.get();
              bool match;
              if (candidateParams[formal].dir & semantics::FunctionFormalParameter::dirOut)
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
              if (candidateParams[formal].paramType != semantics::FunctionFormalParameter::ptUser) continue;

              // Leftover parameter + no default value? Skip
              if (!candidateParams[formal].defaultValue)
              {
                abort = true;
                break;
              }
            }
            if (abort) continue;

            vec.push_back (candidate);
          }
        }
      }
      return vec;
    }

    std::vector<semantics::NameVariablePtr> IntermediateGeneratorSemanticsHandler::ScopeImpl::FlushNewVars ()
    {
      std::vector<semantics::NameVariablePtr> ret (newVars);
      newVars.erase (newVars.begin(), newVars.end());
      return ret;
    }

    const std::vector<semantics::NameVariablePtr>& IntermediateGeneratorSemanticsHandler::ScopeImpl::GetAllVars ()
    {
      return varsInDeclOrder;
    }

    int IntermediateGeneratorSemanticsHandler::ScopeImpl::DistanceToScope (ScopeImpl* scope)
    {
      if (!scope) return INT_MAX;

      auto parentScope = static_cast<ScopeImpl*> (this->parent);
      int n = 0;
      while (parentScope)
      {
        if (parentScope == scope) return n;
        parentScope = static_cast<ScopeImpl*> (parentScope->parent);
        n++;
      }
      return -1;
    }

  } // namespace intermediate
} // namespace s1
