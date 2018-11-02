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
    class IntermediateGeneratorSemanticsHandler::ScopeImpl : public semantics::Scope
    {
    public:
      struct FunctionInfo
      {
        semantics::FunctionPtr functionObj;
        uc::String decoratedIdentifier;
        /// Builtin function information
        BuiltinPtr builtin;
      };
      typedef boost::shared_ptr<FunctionInfo> FunctionInfoPtr;
      typedef std::vector<FunctionInfoPtr> FunctionInfoVector;
    private:
      friend class IntermediateGeneratorSemanticsHandler;

      std::vector<semantics::NameVariablePtr> newVars;
      std::vector<uc::String> outputParams;
      std::vector<semantics::NameVariablePtr> varsInDeclOrder;

      typedef std::unordered_map<uc::String, FunctionInfoVector> FunctionsMap;
      FunctionsMap functions;
      std::vector<FunctionInfoPtr> functionsInDeclOrder;

      typedef OUTCOME_V2_NAMESPACE::result<semantics::NameFunctionPtr, Error> result_NameFunctionPtr;
      result_NameFunctionPtr CheckIdentifierIsFunction (const uc::String& identifier);

      static uc::String DecorateIdentifier (const uc::String& identifier, const semantics::FunctionFormalParameters& params);

      IntermediateGeneratorSemanticsHandler* handler;
      semantics::ScopeLevel level;
      semantics::TypePtr funcReturnType;

      void AddParameter (const semantics::FunctionFormalParameter& param);
    public:
      ScopeImpl (IntermediateGeneratorSemanticsHandler* handler,
                 ScopeImpl* parent, semantics::ScopeLevel level,
                 semantics::Type* funcReturnType);
      semantics::ScopeLevel GetLevel() const { return level; }

      semantics::NameVariablePtr AddVariable (semantics::TypePtr type,
        const uc::String& identifier,
        ExpressionPtr initialValue,
        bool constant) override;

      semantics::NameTypeAliasPtr AddTypeAlias (semantics::TypePtr aliasedType,
        const uc::String& identifier) override;

      semantics::FunctionPtr AddFunction (semantics::TypePtr returnType,
        const uc::String& identifier,
        const semantics::FunctionFormalParameters& params);

      semantics::TypePtr GetFunctionReturnType() const
      {
        if (funcReturnType) return funcReturnType;
        if (parent) return static_cast<const ScopeImpl*> (parent)->GetFunctionReturnType();
        return semantics::TypePtr ();
      }
      const std::vector<uc::String>& GetFunctionOutputParams () const
      {
        auto parent_impl = static_cast<const ScopeImpl*> (parent);
        if (!parent_impl || (parent_impl->level < semantics::ScopeLevel::Function))
          return outputParams;
        else
          return parent_impl->outputParams;
      }
      int DistanceToScope (ScopeImpl* scope);

      void AddBuiltinFunction (const BuiltinPtr& builtin);
      FunctionInfoVector GetFunctions () const;
      FunctionInfoVector CollectOverloadCandidates (semantics::Name* functionName, const ExpressionVector& params) const;

      std::vector<semantics::NameVariablePtr> FlushNewVars ();
      const std::vector<semantics::NameVariablePtr>& GetAllVars ();
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SCOPEIMPL_H__
