/*
    Shader1
    Copyright (c) 2010-2018 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#ifndef SEMANTICS_FUNCTION_H_
#define SEMANTICS_FUNCTION_H_

#include "Base.h"
#include "Block.h"
#include "FunctionFormalParameter.h"
#include "Scope.h"

namespace s1
{
  namespace semantics
  {
    /// Function base
    class BaseFunction : public Base
    {
    protected:
      NameFunction* name;
      /// type of return value
      TypePtr returnType;
      FunctionFormalParameters params;

      BaseFunction (NameFunction* name,
                    Type* returnType,
                    const FunctionFormalParameters& params)
        : name (name), returnType (returnType), params (params) {}
    public:
      virtual bool IsBuiltin() const = 0;
      /// Return name the function is associated with
      NameFunction* GetName() const { return name; }
      /// Return type of variable value
      TypePtr GetReturnType () { return returnType; }
      /// Get formal parameters for function
      const FunctionFormalParameters& GetParameters() const { return params; }
    };

    /// User-defined Function
    class Function : public BaseFunction
    {
    protected:
      ScopePtr scope;
      BlockPtr body;

      friend class NameFunction;
      Function (NameFunction* name,
                Type* returnType,
                const FunctionFormalParameters& params,
                Scope* scope, Block* body)
        : BaseFunction (name, returnType, params), scope (scope), body (body) {}
    public:
      bool IsBuiltin() const override { return false; }

      /// Get function block to add commands to.
      Block* GetBody() const { return body.get(); }

      static Function* upcast (BaseFunction* f) { return (f && !f->IsBuiltin()) ? static_cast<Function*> (f) : nullptr; }
      static const Function* upcast (const BaseFunction* f) { return (f && !f->IsBuiltin()) ? static_cast<const Function*> (f) : nullptr; }
    };

    /// Built-in Function
    class BuiltinFunction : public BaseFunction
    {
    public:
      Builtin which;
    protected:
      friend class NameFunction;
      BuiltinFunction (NameFunction* name,
                       Type* returnType,
                       const FunctionFormalParameters& params,
                       Builtin which)
        : BaseFunction (name, returnType, params), which (which)  {}
    public:
      bool IsBuiltin() const override { return true; }

      /// Return which built-in function is represented
      Builtin GetBuiltin() const { return which; }

      static BuiltinFunction* upcast (BaseFunction* f) { return (f && f->IsBuiltin()) ? static_cast<BuiltinFunction*> (f) : nullptr; }
      static const BuiltinFunction* upcast (const BaseFunction* f) { return (f && f->IsBuiltin()) ? static_cast<const BuiltinFunction*> (f) : nullptr; }
    };
  } // namespace semantics
} // namespace s1

#endif // SEMANTICS_FUNCTION_H_
