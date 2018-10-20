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
#include "semantics/CommonHandler.h"
#include "semantics/Name.h"

#include "semantics/CommonScope.h"

namespace s1
{
  namespace semantics
  {
    CommonScope::CommonScope (CommonHandler* handler,
                                                      CommonScope* parent,
                                                      ScopeLevel level)
     : Scope (parent), handler (handler), level (level)
    {}

    NameVariablePtr
    CommonScope::AddVariable (TypePtr type, const uc::String& identifier,
                                                      ExpressionPtr initialValue, bool constant)
    {
      if (!CheckIdentifierUnique (identifier))
      {
        // TODO: Error handling
        return NameVariablePtr();
      }
      NameVariablePtr newName (new NameVariable (this, identifier, type.get(), initialValue.get(), constant));
      identifiers[identifier] = newName;
      return newName;
    }

    NameTypeAliasPtr
    CommonScope::AddTypeAlias (TypePtr aliasedType, const uc::String& identifier)
    {
      if (!CheckIdentifierUnique (identifier))
      {
        // TODO: Error handling
        return NameTypeAliasPtr();
      }
      NameTypeAliasPtr newName (new NameTypeAlias (this, identifier, aliasedType.get()));
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
      NamePtr newName (new NameFunction (this, identifier, returnType.get()));
      identifiers[identifier] = newName;
      ScopePtr funcScope;
      funcScope = handler->CreateScope (this, ScopeLevel::Function);
      auto newBlock = handler->CreateBlock (funcScope);
      FunctionPtr newFunction (new CommonFunction (newBlock));
      return newFunction;
    }
  } // namespace semantics
} // namespace s1
