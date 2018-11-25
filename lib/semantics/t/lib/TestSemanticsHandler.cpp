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

#include "base/common.h"
#include "../TestSemanticsHandler.h"

#include "parser/Diagnostics.h"
#include "semantics/SimpleDiagnostics.h"

using namespace s1;

typedef semantics::NamePtr NamePtr;
typedef semantics::FunctionPtr FunctionPtr;
typedef semantics::BlockPtr BlockPtr;

TestSemanticsHandler::TestScope::TestScope (TestSemanticsHandler* handler,
                                            TestScope* parent,
                                            s1::semantics::ScopeLevel level)
  : Scope (parent, level), handler (handler)
{}

namespace
{
  class DummyDiagnosticsImpl : public s1::semantics::SimpleDiagnostics
  {
  public:
    void Error (s1::semantics::Error) override { /* FIXME */ }
  };
} // anonymous namespace

FunctionPtr TestSemanticsHandler::CreateFunction (s1::semantics::Scope* parentScope,
                                                  s1::semantics::Type* returnType,
                                                  const s1::uc::String& identifier,
                                                  const s1::semantics::FunctionFormalParameters& params)
{
  semantics::ScopePtr funcScope;
  funcScope = CreateScope (parentScope, s1::semantics::ScopeLevel::Function);
  BlockPtr newBlock (CreateBlock (funcScope));

  DummyDiagnosticsImpl diag;
  return parentScope->AddFunction (diag, returnType, identifier, params, funcScope.get(), newBlock.get());
}
