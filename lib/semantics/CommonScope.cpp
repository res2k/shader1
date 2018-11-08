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
     : Scope (parent, level), handler (handler)
    {}

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
      NameFunctionPtr newName (new NameFunction (this, identifier));
      identifiers[identifier] = newName;
      ScopePtr funcScope;
      funcScope = handler->CreateScope (this, ScopeLevel::Function);
      auto newBlock = handler->CreateBlock (funcScope);
      FunctionPtr newFunction = newName->AddOverload (returnType.get(), params, funcScope.get(), newBlock.get());
      return newFunction;
    }
  } // namespace semantics
} // namespace s1
