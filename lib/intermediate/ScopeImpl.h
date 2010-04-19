#ifndef __INTERMEDIATE_SCOPEIMPL_H__
#define __INTERMEDIATE_SCOPEIMPL_H__

#include "intermediate/IntermediateGeneratorSemanticsHandler.h"

namespace s1
{
  namespace intermediate
  {
    class IntermediateGeneratorSemanticsHandler::ScopeImpl : public Scope,
      public boost::enable_shared_from_this<ScopeImpl>
    {
      friend class IntermediateGeneratorSemanticsHandler;
      
      typedef std::tr1::unordered_map<UnicodeString, NamePtr> IdentifierMap;
      IdentifierMap identifiers;
      
      void CheckIdentifierUnique (const UnicodeString& identifier);
      
      IntermediateGeneratorSemanticsHandler* handler;
      boost::shared_ptr<ScopeImpl> parent;
      ScopeLevel level;
    public:
      ScopeImpl (IntermediateGeneratorSemanticsHandler* handler,
		  const boost::shared_ptr<ScopeImpl>& parent, ScopeLevel level);
      
      NamePtr AddVariable (TypePtr type,
	const UnicodeString& identifier,
	ExpressionPtr initialValue,
	bool constant);
	
      NamePtr AddTypeAlias (TypePtr aliasedType,
	const UnicodeString& identifier);
	
      BlockPtr AddFunction (TypePtr returnType,
	const UnicodeString& identifier,
	const FunctionFormalParameters& params);
    
      NamePtr ResolveIdentifier (const UnicodeString& identifier);
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SCOPEIMPL_H__
