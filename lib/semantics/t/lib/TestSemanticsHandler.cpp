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
#include "../TestSemanticsHandler.h"

#include "parser/Diagnostics.h"

using namespace s1;

typedef semantics::NamePtr NamePtr;
typedef semantics::FunctionPtr FunctionPtr;
typedef semantics::BlockPtr BlockPtr;

TestSemanticsHandler::TestScope::TestScope (TestSemanticsHandler* handler,
                                            TestScope* parent,
                                            s1::semantics::ScopeLevel level)
  : Scope (parent), handler (handler), level (level)
{}

NamePtr TestSemanticsHandler::TestScope::AddVariable (s1::semantics::TypePtr type,
                                                      const s1::uc::String& identifier,
                                                      s1::semantics::ExpressionPtr initialValue,
                                                      bool constant)
{
  if (!CheckIdentifierUnique (identifier))
  {
    return NamePtr();
  }
  NamePtr newName (new s1::semantics::Name (this, identifier, type.get(), initialValue, constant));
  identifiers[identifier] = newName;
  return newName;
}
  
NamePtr TestSemanticsHandler::TestScope::AddTypeAlias (s1::semantics::TypePtr aliasedType, const s1::uc::String& identifier)
{
  if (!CheckIdentifierUnique (identifier))
  {
    // TODO: Error handling
    return NamePtr();
  }
  NamePtr newName (new s1::semantics::Name (this, identifier, semantics::Name::TypeAlias, aliasedType.get()));
  identifiers[identifier] = newName;
  return newName;
}
  
FunctionPtr TestSemanticsHandler::TestScope::AddFunction (s1::semantics::TypePtr returnType,
							   const s1::uc::String& identifier,
							   const FunctionFormalParameters& params)
{
  if (!CheckIdentifierUnique (identifier))
  {
    // TODO: Error handling
    return FunctionPtr();
  }
  NamePtr newName (new s1::semantics::Name (this, identifier, semantics::Name::Function, returnType.get()));
  identifiers[identifier] = newName;
  s1::semantics::ScopePtr funcScope;
  funcScope = handler->CreateScope (this, s1::semantics::ScopeLevel::Function);
  BlockPtr newBlock (handler->CreateBlock (funcScope));
  FunctionPtr newFunction (new TestFunction (funcScope.get(), newBlock));
  return newFunction;
}
