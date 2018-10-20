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
#include "semantics/Name.h"

#include "semantics/Scope.h"

namespace s1
{
  namespace semantics
  {
    Scope::Scope (Scope* parent) : parent (parent) {}

    Scope::~Scope() {}

    bool Scope::CheckIdentifierUnique (const s1::uc::String& identifier)
    {
      auto ident = identifiers.find (identifier);
      if (ident != identifiers.end()) return false;
      if (parent) return parent->CheckIdentifierUnique (identifier);
      return true;
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
