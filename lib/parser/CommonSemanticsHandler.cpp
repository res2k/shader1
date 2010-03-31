#include "base/hash_UnicodeString.h"

#include "parser/CommonSemanticsHandler.h"
#include "parser/Exception.h"

namespace s1
{
  namespace parser
  {
    void CommonSemanticsHandler::CommonScope::CheckIdentifierUnique (const UnicodeString& identifier)
    {
      IdentifierMap::iterator ident = identifiers.find (identifier);
      if (ident != identifiers.end())
      {
	throw Exception (parser::IdentifierAlreadyDeclared);
      }
      if (parent)
	parent->CheckIdentifierUnique (identifier);
    }
      
    CommonSemanticsHandler::CommonScope::CommonScope (CommonScope* parent, ScopeLevel level)
     : parent (parent), level (level)
    {}
      
    CommonSemanticsHandler::NamePtr
    CommonSemanticsHandler::CommonScope::AddVariable (TypePtr type, const UnicodeString& identifier,
						      ExpressionPtr initialValue, bool constant)
    {
      CheckIdentifierUnique (identifier);
      NamePtr newName (new CommonName (identifier, Name::Variable));
      identifiers[identifier] = newName;
      return newName;
    }
      
    CommonSemanticsHandler::NamePtr
    CommonSemanticsHandler::CommonScope::AddTypeAlias (TypePtr aliasedType, const UnicodeString& identifier)
    {
      CheckIdentifierUnique (identifier);
      NamePtr newName (new CommonName (identifier, Name::TypeAlias));
      identifiers[identifier] = newName;
      return newName;
    }
      
    CommonSemanticsHandler::NamePtr
    CommonSemanticsHandler::CommonScope::AddFunction (TypePtr returnType, const UnicodeString& identifier)
    {
      if (level >= Function)
	throw Exception (DeclarationNotAllowedInScope);
      CheckIdentifierUnique (identifier);
      NamePtr newName (new CommonName (identifier, Name::Function));
      identifiers[identifier] = newName;
      return newName;
    }

    CommonSemanticsHandler::NamePtr
    CommonSemanticsHandler::CommonScope::ResolveIdentifier (const UnicodeString& identifier)
    {
      IdentifierMap::iterator ident = identifiers.find (identifier);
      if (ident != identifiers.end())
      {
	return ident->second;
      }
      if (parent)
	return parent->ResolveIdentifier (identifier);
      throw Exception (IdentifierUndeclared);
    }
    
    CommonSemanticsHandler::ScopePtr CommonSemanticsHandler::CreateScope (ScopePtr parentScope,
									  ScopeLevel scopeLevel)
    {
      return ScopePtr (new CommonScope (static_cast<CommonScope*> (parentScope.get()),
	scopeLevel));
    }
    
  } // namespace parser
} // namespace s1
