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
          return outcome::failure (Error::IdentifierAlreadyDeclared);
        }
        return funcName;
      }
      if (parent)
        return static_cast<ScopeImpl*> (parent)->CheckIdentifierIsFunction (identifier);
      return semantics::NameFunctionPtr ();
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

      functionsInDeclOrder.push_back (newFunction);

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

      auto functionObj = funcName->AddBuiltin (returnType, formalParameters, which);

      functionsInDeclOrder.push_back (functionObj);
    }

    std::vector<semantics::BaseFunctionPtr> IntermediateGeneratorSemanticsHandler::ScopeImpl::GetFunctions () const
    {
      return functionsInDeclOrder;
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
