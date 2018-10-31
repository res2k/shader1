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

#include "base/outcome.h"
#include "intermediate/IntermediateGeneratorSemanticsHandler.h"

namespace s1
{
  namespace intermediate
  {
    class IntermediateGeneratorSemanticsHandler::ScopeImpl : public semantics::Scope
    {
    private:
      friend class IntermediateGeneratorSemanticsHandler;

      std::vector<uc::String> outputParams;

      std::vector<semantics::BaseFunctionPtr> functionsInDeclOrder;

      typedef outcome::unchecked<semantics::NameFunctionPtr, Error> result_NameFunctionPtr;
      result_NameFunctionPtr CheckIdentifierIsFunction (const uc::String& identifier);

      IntermediateGeneratorSemanticsHandler* handler;
      semantics::ScopeLevel level;
      semantics::TypePtr funcReturnType;

      void AddParameter (const semantics::FunctionFormalParameter& param);
    public:
      ScopeImpl (IntermediateGeneratorSemanticsHandler* handler,
                 ScopeImpl* parent, semantics::ScopeLevel level,
                 semantics::Type* funcReturnType);
      semantics::ScopeLevel GetLevel() const { return level; }

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

      void AddBuiltinFunction (semantics::Builtin which,
                               semantics::Type* returnType,
                               const uc::String& identifier,
                               const semantics::FunctionFormalParameters& formalParameters);
      std::vector<semantics::BaseFunctionPtr> GetFunctions () const;
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SCOPEIMPL_H__
