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
#include "FunctionImpl.h"
#include "ScopeImpl.h"

#include "intermediate/Diagnostics.h"
#include "parser/Diagnostics.h"
#include "ExpressionImpl.h"
#include "NameImpl.h"

#include <boost/make_shared.hpp>
#include <limits.h>

namespace s1
{
  namespace intermediate
  {
    typedef semantics::NamePtr NamePtr;
    typedef semantics::BlockPtr BlockPtr;
    typedef semantics::FunctionPtr FunctionPtr;

    bool IntermediateGeneratorSemanticsHandler::ScopeImpl::CheckIdentifierUnique (const uc::String& identifier)
    {
      IdentifierMap::iterator ident = identifiers.find (identifier);
      if (ident != identifiers.end())
      {
        return false;
      }
      if (parent)
        return parent->CheckIdentifierUnique (identifier);
      return true;
    }

    IntermediateGeneratorSemanticsHandler::ScopeImpl::result_NamePtr
    IntermediateGeneratorSemanticsHandler::ScopeImpl::CheckIdentifierIsFunction (const uc::String& identifier)
    {
      IdentifierMap::iterator ident = identifiers.find (identifier);
      if ((ident != identifiers.end()) && (ident->second->GetType() != semantics::Name::Function))
      {
        return OUTCOME_V2_NAMESPACE::failure (Error::IdentifierAlreadyDeclared);
      }
      if ((ident != identifiers.end()) && (ident->second)) return ident->second;
      if (parent)
        return parent->CheckIdentifierIsFunction (identifier);
      return NamePtr ();
    }

    IntermediateGeneratorSemanticsHandler::ScopeImpl::ScopeImpl (IntermediateGeneratorSemanticsHandler* handler,
                                                                 ScopeImpl* parent,
                                                                 semantics::ScopeLevel level,
                                                                 semantics::Type* funcReturnType)
     : handler (handler), parent (parent), level (level), funcReturnType (funcReturnType)
    {}

    FunctionPtr IntermediateGeneratorSemanticsHandler::ScopeImpl::CreateFunction (FunctionInfoPtr funcInfo, const BlockPtr& block)
    {
      return FunctionPtr (new FunctionImpl (funcInfo, block));
    }

    void IntermediateGeneratorSemanticsHandler::ScopeImpl::AddParameter (const FunctionFormalParameter& param)
    {
      // TODO: Keep? Or allow params shadow global vars?
      if (!CheckIdentifierUnique (param.identifier))
      {
        handler->ExpressionError (ExpressionContext(), Error::IdentifierAlreadyDeclared);
        return;
      }
      NamePtr newName = new NameImpl (this, param);
      identifiers[param.identifier] = newName;
      newVars.push_back (newName);

      if ((param.dir & dirOut) != 0)
        outputParams.push_back (param.identifier);
    }

    NamePtr IntermediateGeneratorSemanticsHandler::ScopeImpl::AddVariable (semantics::TypePtr type, const uc::String& identifier,
                                                                           ExpressionPtr initialValue, bool constant)
    {
      if (!CheckIdentifierUnique (identifier))
      {
        handler->ExpressionError (ExpressionContext(), Error::IdentifierAlreadyDeclared);
        return NamePtr();
      }
      NamePtr newName = new NameImpl (this, identifier, type.get(), initialValue, constant);
      identifiers[identifier] = newName;
      newVars.push_back (newName);
      varsInDeclOrder.push_back (newName);
      return newName;
    }

    NamePtr IntermediateGeneratorSemanticsHandler::ScopeImpl::AddTypeAlias (semantics::TypePtr aliasedType, const uc::String& identifier)
    {
      if (!CheckIdentifierUnique (identifier))
      {
        handler->ExpressionError (ExpressionContext(), Error::IdentifierAlreadyDeclared);
        return NamePtr();
      }
      NamePtr newName = new NameImpl (this, identifier, semantics::Name::TypeAlias, aliasedType.get());
      identifiers[identifier] = newName;
      return newName;
    }

    FunctionPtr IntermediateGeneratorSemanticsHandler::ScopeImpl::AddFunction (semantics::TypePtr returnType,
                                                                               const uc::String& identifier,
                                                                               const FunctionFormalParameters& params)
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
      NamePtr funcName = std::move (funcIdentResult.value());
      if (funcName == NamePtr ())
      {
        NamePtr newName = new NameImpl (this, identifier, semantics::Name::Function, nullptr);
        identifiers[identifier] = newName;
      }

      semantics::ScopePtr funcScope;
      funcScope = handler->CreateScope (this, semantics::ScopeLevel::Function, returnType);
      auto funcScopeImpl = get_static_ptr<ScopeImpl> (funcScope);
      for (const auto& param : params)
      {
        funcScopeImpl->AddParameter (param);
      }
      BlockPtr newBlock (handler->CreateBlock (funcScope));
      funcScope = semantics::ScopePtr();

      FunctionInfoVector& functions = this->functions[identifier];
      FunctionInfoPtr funcInfo (boost::make_shared<FunctionInfo> ());
      funcInfo->originalIdentifier = identifier;
      // Decorate identifier with type info (so each overload gets a unique name)
      uc::String identifierDecorated (identifier);
      identifierDecorated.append ("$");
      int lastDir = -1;
      for (FunctionFormalParameters::const_iterator param (params.begin());
           param != params.end();
           ++param)
      {
        std::string decorationString;
        if (param->dir != lastDir)
        {
          char dirStr[3] = { 0, 0, 0 };
          int c = 0;
          if (lastDir != -1) dirStr[c++] = '_';
          dirStr[c] = param->dir + '0';
          decorationString.append (dirStr);
          lastDir = param->dir;
        }
        auto typeImpl = param->type.get();
        decorationString.append (handler->GetTypeString (typeImpl));
        identifierDecorated.append (decorationString.c_str());
      }
      funcInfo->identifier = identifierDecorated;
      funcInfo->returnType = returnType;
      funcInfo->params = params;
      funcInfo->block = newBlock;
      functions.push_back (funcInfo);

      functionsInDeclOrder.push_back (funcInfo);

      FunctionPtr newFunction (CreateFunction (funcInfo, newBlock));
      return newFunction;
    }

    semantics::Scope::result_NamePtr
    IntermediateGeneratorSemanticsHandler::ScopeImpl::ResolveIdentifier (const uc::String& identifier)
    {
      NameImplPtr name (ResolveIdentifierInternal (identifier));
      if (!name)
        return parser::Error::IdentifierUndeclared;
      return name;
    }

    IntermediateGeneratorSemanticsHandler::NameImplPtr
    IntermediateGeneratorSemanticsHandler::ScopeImpl::ResolveIdentifierInternal (const uc::String& identifier)
    {
      IdentifierMap::iterator ident = identifiers.find (identifier);
      if (ident != identifiers.end())
      {
        return boost::static_pointer_cast<NameImpl> (ident->second);
      }
      if (parent)
        return parent->ResolveIdentifierInternal (identifier);
      return NameImplPtr ();
    }

    void IntermediateGeneratorSemanticsHandler::ScopeImpl::AddBuiltinFunction (const BuiltinPtr& builtin)
    {
      if (level >= semantics::ScopeLevel::Function)
      {
        handler->ExpressionError (ExpressionContext(), Error::DeclarationNotAllowedInScope);
        return;
      }

      const uc::String& identifier = builtin->GetIdentifier();
      const FunctionFormalParameters& params = builtin->GetFormalParameters();
      auto funcIdentResult = CheckIdentifierIsFunction (identifier);
      if (!funcIdentResult)
      {
        handler->ExpressionError (ExpressionContext(), funcIdentResult.error());
        return;
      }
      NamePtr funcName = std::move (funcIdentResult.value());
      if (funcName == NamePtr ())
      {
        NamePtr newName = new NameImpl (this,identifier, semantics::Name::Function, nullptr);
        identifiers[identifier] = newName;
      }

      FunctionInfoVector& functions = this->functions[identifier];
      FunctionInfoPtr funcInfo (boost::make_shared<FunctionInfo> ());
      funcInfo->originalIdentifier = identifier;
      // Decorate identifier with type info (so each overload gets a unique name)
      uc::String identifierDecorated (identifier);
      identifierDecorated.append ("$");
      for (const auto& param : params)
      {
        identifierDecorated.append (handler->GetTypeString (param.type.get()).c_str());
      }
      funcInfo->identifier = identifierDecorated;
      funcInfo->returnType = builtin->GetReturnType();
      funcInfo->params = params;
      funcInfo->builtin = builtin;
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
      auto nameImpl = get_static_ptr<NameImpl> (functionName);

      FunctionInfoVector vec;
      FunctionsMap::const_iterator funcIt = functions.find (nameImpl->identifier);
      if (funcIt != functions.end())
      {
        // First, look for an exact parameters type match
        for (FunctionInfoVector::const_iterator vecIt = funcIt->second.begin();
             vecIt != funcIt->second.end();
             ++vecIt)
        {
          if (params.size() > (*vecIt)->params.size()) continue;

          bool abort = false;
          size_t formal = 0, actual = 0;
          for (; actual < params.size(); formal++)
          {
            // Only consider user-specified parameters for matching
            if ((*vecIt)->params[formal].paramType != ptUser) continue;

            auto exprImpl = get_static_ptr<ExpressionImpl> (params[actual]);
            auto paramType = exprImpl->GetValueType ();
            auto formalParamType = (*vecIt)->params[formal].type.get();
            // No exact type match? Skip
            if (!paramType->IsEqual (*formalParamType))
            {
              abort = true;
              break;
            }
            actual++;
          }
          if (abort) continue;
          for (; formal < (*vecIt)->params.size(); formal++)
          {
            // Only consider user-specified parameters for matching
            if ((*vecIt)->params[formal].paramType != ptUser) continue;

            // Leftover parameter + no default value? Skip
            if (!(*vecIt)->params[formal].defaultValue)
            {
              abort = true;
              break;
            }
          }
          if (abort) continue;

          vec.push_back (*vecIt);
        }

        // Second, look for a lossless parameters type match
        if (vec.size() == 0)
        {
          for (FunctionInfoVector::const_iterator vecIt = funcIt->second.begin();
              vecIt != funcIt->second.end();
              ++vecIt)
          {
            if (params.size() > (*vecIt)->params.size()) continue;

            bool abort = false;
            size_t formal = 0, actual = 0;
            for (; actual < params.size(); formal++)
            {
              // Only consider user-specified parameters for matching
              if ((*vecIt)->params[formal].paramType != ptUser) continue;

              auto exprImpl = get_static_ptr<ExpressionImpl> (params[actual]);
              auto paramType = exprImpl->GetValueType ();
              auto formalParamType = (*vecIt)->params[formal].type.get();
              bool match;
              if ((*vecIt)->params[formal].dir & dirOut)
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
            for (; formal < (*vecIt)->params.size(); formal++)
            {
              // Only consider user-specified parameters for matching
              if ((*vecIt)->params[formal].paramType != ptUser) continue;

              // Leftover parameter + no default value? Skip
              if (!(*vecIt)->params[formal].defaultValue)
              {
                abort = true;
                break;
              }
            }
            if (abort) continue;

            vec.push_back (*vecIt);
          }
        }
      }
      return vec;
    }

    std::vector<NamePtr> IntermediateGeneratorSemanticsHandler::ScopeImpl::FlushNewVars ()
    {
      std::vector<NamePtr> ret (newVars);
      newVars.erase (newVars.begin(), newVars.end());
      return ret;
    }

    const std::vector<NamePtr>& IntermediateGeneratorSemanticsHandler::ScopeImpl::GetAllVars ()
    {
      return varsInDeclOrder;
    }

    int IntermediateGeneratorSemanticsHandler::ScopeImpl::DistanceToScope (ScopeImpl* scope)
    {
      if (!scope) return INT_MAX;

      auto parentScope = this->parent;
      int n = 0;
      while (parentScope)
      {
        if (parentScope == scope) return n;
        parentScope = parentScope->parent;
        n++;
      }
      return -1;
    }

  } // namespace intermediate
} // namespace s1
