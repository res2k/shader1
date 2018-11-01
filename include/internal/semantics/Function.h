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
#include "Scope.h"

namespace s1
{
  namespace semantics
  {
    /// Function
    class Function : public Base
    {
    protected:
      NameFunction* name;
      Scope::FunctionFormalParameters params;
      ScopePtr scope;
      BlockPtr body;

      friend class NameFunction;
      Function (NameFunction* name,
                const Scope::FunctionFormalParameters& params,
                Scope* scope, Block* body)
        : name (name), params (params), scope (scope), body (body) {}
    public:
      /// Return name the function is associated with
      NameFunction* GetName() const { return name; }
      /// Get formal parameters for function
      const Scope::FunctionFormalParameters& GetParameters() const { return params; }
      /// Get function block to add commands to.
      Block* GetBody() const { return body.get(); }
    };
  } // namespace semantics
} // namespace s1

#endif // SEMANTICS_FUNCTION_H_
