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
    public:
      struct FunctionInfo
      {
	uc::String originalIdentifier;
	uc::String identifier;
	TypePtr returnType;
	FunctionFormalParameters params;
	/// Function body (for user-defined functions)
	BlockPtr block;
	/// Builtin function information
	BuiltinPtr builtin;
      };
      typedef boost::shared_ptr<FunctionInfo> FunctionInfoPtr;
      typedef std::vector<FunctionInfoPtr> FunctionInfoVector;
    private:
      friend class IntermediateGeneratorSemanticsHandler;
      
      typedef boost::unordered_map<uc::String, NamePtr> IdentifierMap;
      IdentifierMap identifiers;
      std::vector<NamePtr> newVars;
      std::vector<uc::String> outputParams;
      
      typedef boost::unordered_map<uc::String, FunctionInfoVector> FunctionsMap;
      FunctionsMap functions;
      std::vector<FunctionInfoPtr> functionsInDeclOrder;
      
      void CheckIdentifierUnique (const uc::String& identifier);
      NamePtr CheckIdentifierIsFunction (const uc::String& identifier);
      
      IntermediateGeneratorSemanticsHandler* handler;
      boost::shared_ptr<ScopeImpl> parent;
      ScopeLevel level;
      TypePtr funcReturnType;
      
      FunctionPtr CreateFunction (ScopeImpl::FunctionInfoPtr funcInfo, const BlockPtr& block);

      void AddParameter (const FunctionFormalParameter& param);
    public:
      ScopeImpl (IntermediateGeneratorSemanticsHandler* handler,
		 const boost::shared_ptr<ScopeImpl>& parent, ScopeLevel level,
		 const TypePtr& funcReturnType);
      ScopeLevel GetLevel() const { return level; }
      
      NamePtr AddVariable (TypePtr type,
	const uc::String& identifier,
	ExpressionPtr initialValue,
	bool constant);
	
      NamePtr AddTypeAlias (TypePtr aliasedType,
	const uc::String& identifier);
	
      FunctionPtr AddFunction (TypePtr returnType,
	const uc::String& identifier,
	const FunctionFormalParameters& params);
    
      NamePtr ResolveIdentifier (const uc::String& identifier);
      NameImplPtr ResolveIdentifierInternal (const uc::String& identifier);
      
      TypePtr GetFunctionReturnType() const
      {
	if (funcReturnType) return funcReturnType;
	if (parent) return parent->GetFunctionReturnType();
	return TypePtr ();
      }
      const std::vector<uc::String>& GetFunctionOutputParams () const
      {
	if (!parent || (parent->level < Function))
	  return outputParams;
	else
	  return parent->outputParams;
      }
      int DistanceToScope (const boost::shared_ptr<ScopeImpl>& scope);
      
      void AddBuiltinFunction (const BuiltinPtr& builtin);
      FunctionInfoVector GetFunctions () const;
      FunctionInfoVector CollectOverloadCandidates (const NamePtr& functionName, const ExpressionVector& params) const;
      
      std::vector<NamePtr> FlushNewVars ();
      std::vector<NamePtr> GetAllVars ();
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SCOPEIMPL_H__
