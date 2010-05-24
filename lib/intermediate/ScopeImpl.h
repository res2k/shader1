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
      std::vector<NamePtr> newVars;
      
      struct FunctionInfoInternal
      {
	TypePtr returnType;
	FunctionFormalParameters params;
	BlockPtr block;
      };
      typedef std::vector<FunctionInfoInternal> FunctionInfoInternalVector;
      typedef std::tr1::unordered_map<UnicodeString, FunctionInfoInternalVector> FunctionsMap;
      FunctionsMap functions;
      
      void CheckIdentifierUnique (const UnicodeString& identifier);
      NamePtr CheckIdentifierIsFunction (const UnicodeString& identifier);
      
      IntermediateGeneratorSemanticsHandler* handler;
      boost::shared_ptr<ScopeImpl> parent;
      ScopeLevel level;
      TypePtr funcReturnType;
    public:
      ScopeImpl (IntermediateGeneratorSemanticsHandler* handler,
		 const boost::shared_ptr<ScopeImpl>& parent, ScopeLevel level,
		 const TypePtr& funcReturnType);
      
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
      
      TypePtr GetFunctionReturnType() const
      {
	if (funcReturnType) return funcReturnType;
	if (parent) return parent->GetFunctionReturnType();
	return TypePtr ();
      }
      int DistanceToScope (const boost::shared_ptr<ScopeImpl>& scope);
      
      struct FunctionInfo
      {
	UnicodeString identifier;
	TypePtr returnType;
	FunctionFormalParameters params;
	BlockPtr block;
      };
      typedef std::vector<FunctionInfo> FunctionInfoVector;
      FunctionInfoVector GetFunctions () const;
      
      std::vector<NamePtr> FlushNewVars ();
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SCOPEIMPL_H__
