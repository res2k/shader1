// Must be first; otherwise, link errors may occur
#include "base/hash_UnicodeString.h"

#include "../TestSemanticsHandler.h"

#include "parser/Exception.h"

using namespace s1;

typedef TestSemanticsHandler::NamePtr NamePtr;
typedef TestSemanticsHandler::BlockPtr BlockPtr;

void TestSemanticsHandler::TestScope::CheckIdentifierUnique (const UnicodeString& identifier)
{
  IdentifierMap::iterator ident = identifiers.find (identifier);
  if (ident != identifiers.end())
  {
    throw parser::Exception (parser::IdentifierAlreadyDeclared);
  }
  if (parent)
    parent->CheckIdentifierUnique (identifier);
}
  
TestSemanticsHandler::TestScope::TestScope (TestSemanticsHandler* handler,
						const boost::shared_ptr<TestScope>& parent,
						ScopeLevel level)
  : handler (handler), parent (parent), level (level)
{}

NamePtr TestSemanticsHandler::TestScope::AddVariable (TypePtr type, const UnicodeString& identifier,
						  ExpressionPtr initialValue, bool constant)
{
  CheckIdentifierUnique (identifier);
  NamePtr newName (new CommonName (identifier, type, initialValue, constant));
  identifiers[identifier] = newName;
  return newName;
}
  
NamePtr TestSemanticsHandler::TestScope::AddTypeAlias (TypePtr aliasedType, const UnicodeString& identifier)
{
  CheckIdentifierUnique (identifier);
  NamePtr newName (new CommonName (identifier, Name::TypeAlias, aliasedType));
  identifiers[identifier] = newName;
  return newName;
}
  
BlockPtr TestSemanticsHandler::TestScope::AddFunction (TypePtr returnType,
							   const UnicodeString& identifier,
							   const FunctionFormalParameters& params)
{
  CheckIdentifierUnique (identifier);
  NamePtr newName (new CommonName (identifier, Name::Function, returnType));
  identifiers[identifier] = newName;
  ScopePtr funcScope;
  funcScope = handler->CreateScope (shared_from_this(), Function);
  BlockPtr newBlock (handler->CreateBlock (funcScope));
  funcScope = ScopePtr();
  return newBlock;
}

NamePtr TestSemanticsHandler::TestScope::ResolveIdentifier (const UnicodeString& identifier)
{
  IdentifierMap::iterator ident = identifiers.find (identifier);
  if (ident != identifiers.end())
  {
    return ident->second;
  }
  if (parent)
    return parent->ResolveIdentifier (identifier);
  throw parser::Exception (parser::IdentifierUndeclared);
}

