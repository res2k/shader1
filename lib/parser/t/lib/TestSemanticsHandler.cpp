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
#include "parser/Exception.h"

using namespace s1;

typedef TestSemanticsHandler::NamePtr NamePtr;
typedef TestSemanticsHandler::FunctionPtr FunctionPtr;
typedef TestSemanticsHandler::BlockPtr BlockPtr;

bool TestSemanticsHandler::TestScope::CheckIdentifierUnique (const s1::uc::String& identifier)
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
  
TestSemanticsHandler::TestScope::TestScope (TestSemanticsHandler* handler,
						const boost::shared_ptr<TestScope>& parent,
						ScopeLevel level)
  : handler (handler), parent (parent), level (level)
{}

NamePtr TestSemanticsHandler::TestScope::AddVariable (TypePtr type, const s1::uc::String& identifier,
						  ExpressionPtr initialValue, bool constant)
{
  if (!CheckIdentifierUnique (identifier))
  {
    return NamePtr();
  }
  NamePtr newName (new CommonName (identifier, type, initialValue, constant));
  identifiers[identifier] = newName;
  return newName;
}
  
NamePtr TestSemanticsHandler::TestScope::AddTypeAlias (TypePtr aliasedType, const s1::uc::String& identifier)
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
  
FunctionPtr TestSemanticsHandler::TestScope::AddFunction (TypePtr returnType,
							   const s1::uc::String& identifier,
							   const FunctionFormalParameters& params)
{
  if (!CheckIdentifierUnique (identifier))
  {
    // TODO: Error handling
    return FunctionPtr();
  }
  NamePtr newName (new CommonName (identifier, Name::Function, returnType));
  identifiers[identifier] = newName;
  ScopePtr funcScope;
  funcScope = handler->CreateScope (shared_from_this(), Function);
  BlockPtr newBlock (handler->CreateBlock (funcScope));
  funcScope = ScopePtr();
  FunctionPtr newFunction (new TestFunction (newBlock));
  return newFunction;
}

NamePtr TestSemanticsHandler::TestScope::ResolveIdentifier (const s1::uc::String& identifier)
{
  IdentifierMap::iterator ident = identifiers.find (identifier);
  if (ident != identifiers.end())
  {
    return ident->second;
  }
  if (parent)
    return parent->ResolveIdentifier (identifier);
  throw parser::Exception (parser::Error::IdentifierUndeclared);
}

