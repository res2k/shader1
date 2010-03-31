#ifndef __PARSER_COMMONSEMANTICSHANDLER_H__
#define __PARSER_COMMONSEMANTICSHANDLER_H__

#include "base/unordered_map"
#include "SemanticsHandler.h"

namespace s1
{
  namespace parser
  {
    class CommonSemanticsHandler : public SemanticsHandler
    {
    protected:
      struct CommonName : public Name
      {
	UnicodeString identifier;
	NameType type;
	
	CommonName (const UnicodeString& identifier, NameType type)
	 : identifier (identifier), type (type) {}
	
	NameType GetType() { return type; }
      };

      class CommonScope : public Scope
      {
	typedef std::tr1::unordered_map<UnicodeString, NamePtr> IdentifierMap;
	IdentifierMap identifiers;
	
	void CheckIdentifierUnique (const UnicodeString& identifier);
	
	CommonScope* parent;
	ScopeLevel level;
      public:
	CommonScope (CommonScope* parent, ScopeLevel level);
	
	NamePtr AddVariable (TypePtr type,
	  const UnicodeString& identifier,
	  ExpressionPtr initialValue,
	  bool constant);
	  
	NamePtr AddTypeAlias (TypePtr aliasedType,
	  const UnicodeString& identifier);
	  
	NamePtr AddFunction (TypePtr returnType,
	  const UnicodeString& identifier);
      
	NamePtr ResolveIdentifier (const UnicodeString& identifier);
      };
      
    public:  
      ScopePtr CreateScope (ScopePtr parentScope, ScopeLevel scopeLevel);
    };
    
  } // namespace parser
} // namespace s1
    
#endif // __PARSER_COMMONSEMANTICSHANDLER_H__
