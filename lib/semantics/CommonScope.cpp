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

#include "base/common.h"
#include "base/intrusive_ptr.h"
#include "parser/Diagnostics.h"
#include "semantics/CommonHandler.h"
#include "semantics/Name.h"
#include "semantics/SimpleDiagnostics.h"

#include "semantics/CommonScope.h"

namespace s1
{
  namespace semantics
  {
    CommonScope::CommonScope (CommonHandler* handler,
                                                      CommonScope* parent,
                                                      ScopeLevel level)
     : Scope (parent, level), handler (handler)
    {}

    namespace
    {
      class DummyDiagnosticsImpl : public SimpleDiagnostics
      {
      public:
        void Error (semantics::Error) override { /* FIXME */ }
      };
    } // anonymous namespace

    FunctionPtr
    CommonScope::AddFunction (TypePtr returnType,
                              const uc::String& identifier,
                              const FunctionFormalParameters& params)
    {
      semantics::ScopePtr funcScope;
      funcScope = handler->CreateScope (this, ScopeLevel::Function);
      BlockPtr newBlock (handler->CreateBlock (funcScope));

      DummyDiagnosticsImpl diag;
      return Scope::AddFunction (diag, returnType.get(), identifier, params, funcScope.get(), newBlock.get());
    }
  } // namespace semantics
} // namespace s1
