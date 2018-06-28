/*
    Shader1
    Copyright (c) 2010-2018 Frank Richter


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
#include "parser/Diagnostics.h"
#include "semantics/CommonName.h"
#include "semantics/CommonHandler.h"

#include "semantics/CommonScope.h"

namespace s1
{
  namespace semantics
  {
    bool CommonScope::CheckIdentifierUnique (const uc::String& identifier)
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

    CommonScope::CommonScope (CommonHandler* handler,
                                                      CommonScope* parent,
                                                      ScopeLevel level)
     : handler (handler), parent (parent), level (level)
    {}

    NamePtr
    CommonScope::AddVariable (TypePtr type, const uc::String& identifier,
                                                      ExpressionPtr initialValue, bool constant)
    {
      if (!CheckIdentifierUnique (identifier))
      {
        // TODO: Error handling
        return NamePtr();
      }
      NamePtr newName (new CommonName (identifier, type, initialValue, constant));
      identifiers[identifier] = newName;
      return newName;
    }

    NamePtr
    CommonScope::AddTypeAlias (TypePtr aliasedType, const uc::String& identifier)
    {
      if (!CheckIdentifierUnique (identifier))
      {
        // TODO: Error handling
        return NamePtr();
      }
      NamePtr newName (new CommonName (identifier, Name::TypeAlias, aliasedType));
      identifiers[identifier] = newName;
      return newName;
    }

    FunctionPtr
    CommonScope::AddFunction (TypePtr returnType,
                              const uc::String& identifier,
                              const FunctionFormalParameters& params)
    {
      if (level >= ScopeLevel::Function)
      {
        // TODO: Error handling
        return FunctionPtr();
      }
      if (!CheckIdentifierUnique (identifier))
      {
        // TODO: Error handling
        return FunctionPtr();
      }
      NamePtr newName (new CommonName (identifier, Name::Function, returnType));
      identifiers[identifier] = newName;
      ScopePtr funcScope;
      funcScope = handler->CreateScope (this, ScopeLevel::Function);
      auto newBlock = handler->CreateBlock (funcScope);
      funcScope = ScopePtr();
      FunctionPtr newFunction (new CommonFunction (newBlock));
      return newFunction;
    }

    CommonScope::result_NamePtr
    CommonScope::ResolveIdentifier (const uc::String& identifier)
    {
      IdentifierMap::iterator ident = identifiers.find (identifier);
      if (ident != identifiers.end())
      {
        return ident->second;
      }
      if (parent)
        return parent->ResolveIdentifier (identifier);
      return parser::Error::IdentifierUndeclared;
    }
  } // namespace semantics
} // namespace s1
