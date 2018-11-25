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
#include "semantics/SimpleDiagnostics.h"

namespace s1
{
  namespace intermediate
  {
    class IntermediateGeneratorSemanticsHandler::ScopeImpl : public semantics::Scope
    {
    private:
      friend class IntermediateGeneratorSemanticsHandler;

      std::vector<uc::String> outputParams;

      IntermediateGeneratorSemanticsHandler* handler;
      semantics::TypePtr funcReturnType;

      void AddParameter (const semantics::FunctionFormalParameter& param);
    public:
      ScopeImpl (IntermediateGeneratorSemanticsHandler* handler,
                 ScopeImpl* parent, semantics::ScopeLevel level,
                 semantics::Type* funcReturnType);

      semantics::TypePtr GetFunctionReturnType() const
      {
        if (funcReturnType) return funcReturnType;
        if (GetParent()) return static_cast<const ScopeImpl*> (GetParent())->GetFunctionReturnType();
        return semantics::TypePtr ();
      }
      const std::vector<uc::String>& GetFunctionOutputParams () const
      {
        auto parent_impl = static_cast<const ScopeImpl*> (GetParent());
        if (!parent_impl || (parent_impl->level < semantics::ScopeLevel::Function))
          return outputParams;
        else
          return parent_impl->outputParams;
      }
      int DistanceToScope (Scope* scope);

      void AddBuiltinFunction (semantics::Builtin which,
                               semantics::Type* returnType,
                               const uc::String& identifier,
                               const semantics::FunctionFormalParameters& formalParameters);
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SCOPEIMPL_H__
