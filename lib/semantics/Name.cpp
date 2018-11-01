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
    NameFunction::NameFunction (Scope* ownerScope, const uc::String& identifier, Type* returnType)
      : Name (ownerScope, identifier), returnType (returnType) {}

    NameFunction::~NameFunction () {}

    semantics::Function* NameFunction::AddOverload (const Scope::FunctionFormalParameters& params, Scope* funcScope, Block* body)
    {
      FunctionPtr newFunc = new semantics::Function (this, params, funcScope, body);
      overloads.push_back (newFunc);
      return newFunc.get();
    }
  } // namespace semantics
} // namespace s1
