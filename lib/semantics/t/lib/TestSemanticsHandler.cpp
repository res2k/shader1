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

s1::semantics::NameVariablePtr
TestSemanticsHandler::TestScope::AddVariable (s1::semantics::TypePtr type,
                                              const s1::uc::String& identifier,
                                              s1::semantics::ExpressionPtr initialValue,
                                              bool constant)
{
  if (!CheckIdentifierUnique (identifier))
  {
    return s1::semantics::NameVariablePtr();
  }
  s1::semantics::NameVariablePtr newName (new s1::semantics::NameVariable (this, identifier, type.get(), initialValue.get(), constant));
  identifiers[identifier] = newName;
  return newName;
}

s1::semantics::NameTypeAliasPtr
TestSemanticsHandler::TestScope::AddTypeAlias (s1::semantics::TypePtr aliasedType, const s1::uc::String& identifier)
{
  if (!CheckIdentifierUnique (identifier))
  {
    // TODO: Error handling
    return s1::semantics::NameTypeAliasPtr();
  }
  s1::semantics::NameTypeAliasPtr newName (new s1::semantics::NameTypeAlias (this, identifier, aliasedType.get()));
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
  NamePtr newName (new s1::semantics::NameFunction (this, identifier, returnType.get()));
  identifiers[identifier] = newName;
  s1::semantics::ScopePtr funcScope;
  funcScope = handler->CreateScope (this, s1::semantics::ScopeLevel::Function);
  BlockPtr newBlock (handler->CreateBlock (funcScope));
  FunctionPtr newFunction (new s1::semantics::Function (funcScope.get(), newBlock.get()));
  return newFunction;
}
