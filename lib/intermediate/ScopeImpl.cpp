// Must be first; otherwise, link errors may occur
#include "base/hash_UnicodeString.h"

#include "ScopeImpl.h"

#include "parser/Exception.h"
#include "NameImpl.h"

namespace s1
{
  namespace intermediate
  {
    typedef IntermediateGeneratorSemanticsHandler::NamePtr NamePtr;
    typedef IntermediateGeneratorSemanticsHandler::BlockPtr BlockPtr;
    
    void IntermediateGeneratorSemanticsHandler::ScopeImpl::CheckIdentifierUnique (const UnicodeString& identifier)
    {
      IdentifierMap::iterator ident = identifiers.find (identifier);
      if (ident != identifiers.end())
      {
	throw parser::Exception (parser::IdentifierAlreadyDeclared);
      }
      if (parent)
	parent->CheckIdentifierUnique (identifier);
    }
      
    IntermediateGeneratorSemanticsHandler::ScopeImpl::ScopeImpl (IntermediateGeneratorSemanticsHandler* handler,
								 const boost::shared_ptr<ScopeImpl>& parent,
								 ScopeLevel level)
     : handler (handler), parent (parent), level (level)
    {}

    NamePtr IntermediateGeneratorSemanticsHandler::ScopeImpl::AddVariable (TypePtr type, const UnicodeString& identifier,
									   ExpressionPtr initialValue, bool constant)
    {
      CheckIdentifierUnique (identifier);
      NamePtr newName (new NameImpl (identifier, type, initialValue, constant));
      identifiers[identifier] = newName;
      return newName;
    }
      
    NamePtr IntermediateGeneratorSemanticsHandler::ScopeImpl::AddTypeAlias (TypePtr aliasedType, const UnicodeString& identifier)
    {
      CheckIdentifierUnique (identifier);
      NamePtr newName (new NameImpl (identifier, Name::TypeAlias, aliasedType));
      identifiers[identifier] = newName;
      return newName;
    }
      
    BlockPtr IntermediateGeneratorSemanticsHandler::ScopeImpl::AddFunction (TypePtr returnType,
									    const UnicodeString& identifier,
									    const FunctionFormalParameters& params)
    {
      if (level >= Function)
	throw parser::Exception (parser::DeclarationNotAllowedInScope);
      CheckIdentifierUnique (identifier);
      NamePtr newName (new NameImpl (identifier, Name::Function, returnType));
      identifiers[identifier] = newName;
      ScopePtr funcScope;
      funcScope = handler->CreateScope (shared_from_this(), Function);
      BlockPtr newBlock (handler->CreateBlock (funcScope));
      funcScope = ScopePtr();
      return newBlock;
    }

    NamePtr IntermediateGeneratorSemanticsHandler::ScopeImpl::ResolveIdentifier (const UnicodeString& identifier)
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
    
  } // namespace intermediate
} // namespace s1
