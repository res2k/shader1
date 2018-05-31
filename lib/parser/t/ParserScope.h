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

#include <cxxtest/TestSuite.h>

#include "base/common.h"

#include "parser/Parser.h"

#include "TestSemanticsHandler.h"

#include "ParserTestTraits.h"

class ParserScopeTestSuite : public CxxTest::TestSuite 
{
public:
  void testIdentifierAddVar (void)
  {
    using namespace s1::parser;
    
    TestSemanticsHandler semanticsHandler;
    s1::semantics::ScopePtr scope (
      semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                    s1::semantics::ScopeLevel::Global));
    s1::semantics::NamePtr varAdded;
    TS_ASSERT_THROWS_NOTHING(
      varAdded =
        scope->AddVariable (semanticsHandler.CreateType (s1::semantics::BaseType::Int),
                            s1::uc::String ("a"), 
                            s1::semantics::ExpressionPtr(), false)
    );
    s1::semantics::NamePtr varRequested;
    TS_ASSERT_THROWS_NOTHING(
      varRequested =
        scope->ResolveIdentifier (s1::uc::String ("a")).value()
    );
    TS_ASSERT_EQUALS (varAdded, varRequested);
    TS_ASSERT_EQUALS (varRequested->GetType(), s1::semantics::Name::Variable);
  }  
  
  void testIdentifierAddFunc (void)
  {
    using namespace s1::parser;
    
    TestSemanticsHandler semanticsHandler;
    s1::semantics::ScopePtr scope (
      semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                    s1::semantics::ScopeLevel::Global));
    s1::semantics::Scope::FunctionFormalParameters params;
    TS_ASSERT_THROWS_NOTHING(
      scope->AddFunction (semanticsHandler.CreateType (s1::semantics::BaseType::Int),
                          s1::uc::String ("a"), params)
    );
    s1::semantics::NamePtr varRequested;
    TS_ASSERT_THROWS_NOTHING(
      varRequested =
        scope->ResolveIdentifier (s1::uc::String ("a")).value()
    );
    TS_ASSERT_DIFFERS (varRequested, s1::semantics::NamePtr ());
    TS_ASSERT_EQUALS (varRequested->GetType(), s1::semantics::Name::Function);
  }  
  
  void testIdentifierAddType (void)
  {
    using namespace s1::parser;
    
    TestSemanticsHandler semanticsHandler;
    s1::semantics::ScopePtr scope (
      semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                    s1::semantics::ScopeLevel::Global));
    s1::semantics::NamePtr varAdded;
    TS_ASSERT_THROWS_NOTHING(
      varAdded =
        scope->AddTypeAlias (semanticsHandler.CreateType (s1::semantics::BaseType::Int),
                             s1::uc::String ("a"))
    );
    s1::semantics::NamePtr varRequested;
    TS_ASSERT_THROWS_NOTHING(
      varRequested =
        scope->ResolveIdentifier (s1::uc::String ("a")).value()
    );
    TS_ASSERT_EQUALS (varAdded, varRequested);
    TS_ASSERT_EQUALS (varRequested->GetType(), s1::semantics::Name::TypeAlias);
  }  
  
  void testIdentifierUnknown (void)
  {
    using namespace s1::parser;
    
    TestSemanticsHandler semanticsHandler;
    s1::semantics::ScopePtr scope (
      semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                    s1::semantics::ScopeLevel::Global));
    s1::semantics::Scope::result_NamePtr varRequested = s1::semantics::NamePtr ();
    TS_ASSERT_THROWS_NOTHING(
      varRequested =
        scope->ResolveIdentifier (s1::uc::String ("foo"))
    );
    TS_ASSERT_EQUALS(varRequested.error(), s1::parser::Error::IdentifierUndeclared);
  }
  
  void testIdentifierDeclareMultiple (void)
  {
    using namespace s1::parser;
    
    TestSemanticsHandler semanticsHandler;
    s1::semantics::ScopePtr scope (
      semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                    s1::semantics::ScopeLevel::Global));
    s1::semantics::NamePtr varAdded1, varAdded2;
    TS_ASSERT_THROWS_NOTHING(
      varAdded1 =
        scope->AddVariable (semanticsHandler.CreateType (s1::semantics::BaseType::Int),
                            s1::uc::String ("a"), 
                            s1::semantics::ExpressionPtr(), false)
    );
    TS_ASSERT_THROWS_NOTHING(
      varAdded2 =
        scope->AddVariable (semanticsHandler.CreateType (s1::semantics::BaseType::Int),
                            s1::uc::String ("a"), 
                            s1::semantics::ExpressionPtr(), false)
    );
    TS_ASSERT(!varAdded2);
  }  
  
  void testIdentifierAddVarHigher (void)
  {
    using namespace s1::parser;
    
    TestSemanticsHandler semanticsHandler;
    s1::semantics::ScopePtr scopeOuter (
      semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                    s1::semantics::ScopeLevel::Global));
    s1::semantics::NamePtr varAdded;
    TS_ASSERT_THROWS_NOTHING(
      varAdded =
        scopeOuter->AddVariable (semanticsHandler.CreateType (s1::semantics::BaseType::Int),
                                 s1::uc::String ("a"), 
                                 s1::semantics::ExpressionPtr(), false)
    );
    s1::semantics::ScopePtr scopeInner (
      semanticsHandler.CreateScope (scopeOuter,
                                    s1::semantics::ScopeLevel::Function));
    s1::semantics::NamePtr varRequested;
    TS_ASSERT_THROWS_NOTHING(
      varRequested =
        scopeInner->ResolveIdentifier (s1::uc::String ("a")).value()
    );
    TS_ASSERT_EQUALS (varAdded, varRequested);
    TS_ASSERT_EQUALS (varRequested->GetType(), s1::semantics::Name::Variable);
  }  
  
};
