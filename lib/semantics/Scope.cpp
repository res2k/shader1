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
#include "semantics/Diagnostics.h"
#include "semantics/Function.h"
#include "semantics/Name.h"
#include "semantics/SimpleDiagnostics.h"

#include "semantics/Scope.h"

namespace s1
{
  namespace semantics
  {
    Scope::Scope (Scope* parent, ScopeLevel level) : parent (parent), level (level) {}

    Scope::~Scope() {}

    bool Scope::CheckIdentifierUnique (const s1::uc::String& identifier)
    {
      auto ident = identifiers.find (identifier);
      if (ident != identifiers.end()) return false;
      if (parent) return parent->CheckIdentifierUnique (identifier);
      return true;
    }

    Scope::result_NameFunctionPtr Scope::CheckIdentifierIsFunction (const uc::String& identifier)
    {
      auto ident_it = identifiers.find (identifier);
      if (ident_it != identifiers.end())
      {
        auto funcName = semantics::NameFunction::upcast (ident_it->second.get());
        if (!funcName)
        {
          return outcome::failure (Error::IdentifierAlreadyDeclared);
        }
        return funcName;
      }
      if (GetParent())
        return GetParent()->CheckIdentifierIsFunction (identifier);
      return NameFunctionPtr ();
    }

    NameVariablePtr Scope::AddVariable (SimpleDiagnostics& diagnosticsHandler,
                                        Type* type, const uc::String& identifier,
                                        Expression* initialValue, bool constant)
    {
      NameVariablePtr newName = new NameVariable (this, identifier, type, initialValue, constant);
      if (!CheckIdentifierUnique (identifier))
      {
        diagnosticsHandler.Error (Error::IdentifierAlreadyDeclared);
        // Return newName so parsing can continue, but it won't be useable.
      }
      else
      {
        identifiers[identifier] = newName;
        newVars.push_back (newName);
        varsInDeclOrder.push_back (newName);
      }
      return newName;
    }

    std::vector<NameVariablePtr> Scope::FlushNewVars ()
    {
      std::vector<semantics::NameVariablePtr> ret (newVars);
      newVars.erase (newVars.begin(), newVars.end());
      return ret;
    }

    const std::vector<NameVariablePtr>& Scope::GetAllVars ()
    {
      return varsInDeclOrder;
    }

    NameTypeAliasPtr Scope::AddTypeAlias (SimpleDiagnostics& diagnosticsHandler,
                                          Type* aliasedType, const uc::String& identifier)
    {
      NameTypeAliasPtr newName = new NameTypeAlias (this, identifier, aliasedType);
      if (!CheckIdentifierUnique (identifier))
      {
        diagnosticsHandler.Error (Error::IdentifierAlreadyDeclared);
        // Return newName so parsing can continue, but it won't be useable.
      }
      else
      {
        identifiers[identifier] = newName;
      }
      return newName;
    }

    FunctionPtr Scope::AddFunction (SimpleDiagnostics& diagnosticsHandler,
                                    Type* returnType, const uc::String& identifier,
                                    const FunctionFormalParameters& params,
                                    Scope* funcScope, Block* funcBlock)
    {
      if (level >= semantics::ScopeLevel::Function)
      {
        diagnosticsHandler.Error (Error::DeclarationNotAllowedInScope);
        return FunctionPtr();
      }
      auto funcIdentResult = CheckIdentifierIsFunction (identifier);
      FunctionPtr newFunction;
      if (!funcIdentResult)
      {
        diagnosticsHandler.Error (funcIdentResult.error());
        // Use dummy function parsing can continue, but it won't be useable.
        NameFunctionPtr funcName = new semantics::NameFunction (this, identifier);
        newFunction = funcName->AddOverload (returnType, params, funcScope, funcBlock);
      }
      else
      {
        NameFunctionPtr funcName = std::move (funcIdentResult.value());
        if (!funcName)
        {
          funcName = new semantics::NameFunction (this, identifier);
          identifiers[identifier] = funcName;
        }

        newFunction = funcName->AddOverload (returnType, params, funcScope, funcBlock);
        functionsInDeclOrder.push_back (newFunction);
      }

      return newFunction;
    }

    std::vector<BaseFunctionPtr> Scope::GetFunctions () const
    {
      return functionsInDeclOrder;
    }

    Scope::result_NamePtr Scope::ResolveIdentifier (const uc::String& identifier)
    {
      IdentifierMap::iterator ident = identifiers.find (identifier);
      if (ident != identifiers.end())
      {
        return ident->second;
      }
      if (parent)
        return parent->ResolveIdentifier (identifier);
      return Error::IdentifierUndeclared;
    }
  } // namespace semantics
} // namespace s1
