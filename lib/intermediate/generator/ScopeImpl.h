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

#include <outcome/outcome.hpp>

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
        semantics::TypePtr returnType;
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

      typedef std::unordered_map<uc::String, semantics::NamePtr> IdentifierMap;
      IdentifierMap identifiers;
      std::vector<semantics::NamePtr> newVars;
      std::vector<uc::String> outputParams;
      std::vector<semantics::NamePtr> varsInDeclOrder;

      typedef std::unordered_map<uc::String, FunctionInfoVector> FunctionsMap;
      FunctionsMap functions;
      std::vector<FunctionInfoPtr> functionsInDeclOrder;

      bool CheckIdentifierUnique (const uc::String& identifier);
      typedef OUTCOME_V2_NAMESPACE::result<semantics::NamePtr, Error> result_NamePtr;
      result_NamePtr CheckIdentifierIsFunction (const uc::String& identifier);

      IntermediateGeneratorSemanticsHandler* handler;
      boost::shared_ptr<ScopeImpl> parent;
      ScopeLevel level;
      semantics::TypePtr funcReturnType;

      semantics::FunctionPtr CreateFunction (ScopeImpl::FunctionInfoPtr funcInfo, const BlockPtr& block);

      void AddParameter (const FunctionFormalParameter& param);
    public:
      ScopeImpl (IntermediateGeneratorSemanticsHandler* handler,
                 const boost::shared_ptr<ScopeImpl>& parent, ScopeLevel level,
                 const semantics::TypePtr& funcReturnType);
      ScopeLevel GetLevel() const { return level; }

      semantics::NamePtr AddVariable (semantics::TypePtr type,
        const uc::String& identifier,
        ExpressionPtr initialValue,
        bool constant);

      semantics::NamePtr AddTypeAlias (semantics::TypePtr aliasedType,
        const uc::String& identifier);

      semantics::FunctionPtr AddFunction (semantics::TypePtr returnType,
        const uc::String& identifier,
        const FunctionFormalParameters& params);

      Scope::result_NamePtr ResolveIdentifier (const uc::String& identifier);
      NameImplPtr ResolveIdentifierInternal (const uc::String& identifier);

      semantics::TypePtr GetFunctionReturnType() const
      {
        if (funcReturnType) return funcReturnType;
        if (parent) return parent->GetFunctionReturnType();
        return semantics::TypePtr ();
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
      FunctionInfoVector CollectOverloadCandidates (const semantics::NamePtr& functionName, const ExpressionVector& params) const;

      std::vector<semantics::NamePtr> FlushNewVars ();
      const std::vector<semantics::NamePtr>& GetAllVars ();
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SCOPEIMPL_H__
