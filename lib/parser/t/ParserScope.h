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

#include "parser/Exception.h"
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
    SemanticsHandler::ScopePtr scope (
      semanticsHandler.CreateScope (SemanticsHandler::ScopePtr(),
                                    SemanticsHandler::Global));
    SemanticsHandler::NamePtr varAdded;
    TS_ASSERT_THROWS_NOTHING(
      varAdded =
        scope->AddVariable (semanticsHandler.CreateType (SemanticsHandler::Int),
                            s1::uc::String ("a"), 
                            SemanticsHandler::ExpressionPtr(), false)
    );
    SemanticsHandler::NamePtr varRequested;
    TS_ASSERT_THROWS_NOTHING(
      varRequested =
        scope->ResolveIdentifier (s1::uc::String ("a"))
    );
    TS_ASSERT_EQUALS (varAdded, varRequested);
    TS_ASSERT_EQUALS (varRequested->GetType(), SemanticsHandler::Name::Variable);
  }  
  
  void testIdentifierAddFunc (void)
  {
    using namespace s1::parser;
    
    TestSemanticsHandler semanticsHandler;
    SemanticsHandler::ScopePtr scope (
      semanticsHandler.CreateScope (SemanticsHandler::ScopePtr(),
                                    SemanticsHandler::Global));
    SemanticsHandler::Scope::FunctionFormalParameters params;
    TS_ASSERT_THROWS_NOTHING(
      scope->AddFunction (semanticsHandler.CreateType (SemanticsHandler::Int),
                          s1::uc::String ("a"), params)
    );
    SemanticsHandler::NamePtr varRequested;
    TS_ASSERT_THROWS_NOTHING(
      varRequested =
        scope->ResolveIdentifier (s1::uc::String ("a"))
    );
    TS_ASSERT_DIFFERS (varRequested, SemanticsHandler::NamePtr ());
    TS_ASSERT_EQUALS (varRequested->GetType(), SemanticsHandler::Name::Function);
  }  
  
  void testIdentifierAddType (void)
  {
    using namespace s1::parser;
    
    TestSemanticsHandler semanticsHandler;
    SemanticsHandler::ScopePtr scope (
      semanticsHandler.CreateScope (SemanticsHandler::ScopePtr(),
                                    SemanticsHandler::Global));
    SemanticsHandler::NamePtr varAdded;
    TS_ASSERT_THROWS_NOTHING(
      varAdded =
        scope->AddTypeAlias (semanticsHandler.CreateType (SemanticsHandler::Int),
                             s1::uc::String ("a"))
    );
    SemanticsHandler::NamePtr varRequested;
    TS_ASSERT_THROWS_NOTHING(
      varRequested =
        scope->ResolveIdentifier (s1::uc::String ("a"))
    );
    TS_ASSERT_EQUALS (varAdded, varRequested);
    TS_ASSERT_EQUALS (varRequested->GetType(), SemanticsHandler::Name::TypeAlias);
  }  
  
  void testIdentifierUnknown (void)
  {
    using namespace s1::parser;
    
    TestSemanticsHandler semanticsHandler;
    SemanticsHandler::ScopePtr scope (
      semanticsHandler.CreateScope (SemanticsHandler::ScopePtr(),
                                    SemanticsHandler::Global));
    SemanticsHandler::NamePtr varRequested;
    TS_ASSERT_THROWS_ASSERT(
      varRequested =
        scope->ResolveIdentifier (s1::uc::String ("foo")),
      const s1::parser::Exception& e,
      TS_ASSERT_EQUALS(e.GetCode(), s1::parser::Error::IdentifierUndeclared)
    );
  }
  
  void testIdentifierDeclareMultiple (void)
  {
    using namespace s1::parser;
    
    TestSemanticsHandler semanticsHandler;
    SemanticsHandler::ScopePtr scope (
      semanticsHandler.CreateScope (SemanticsHandler::ScopePtr(),
                                    SemanticsHandler::Global));
    SemanticsHandler::NamePtr varAdded;
    TS_ASSERT_THROWS_NOTHING(
      varAdded =
        scope->AddVariable (semanticsHandler.CreateType (SemanticsHandler::Int),
                            s1::uc::String ("a"), 
                            SemanticsHandler::ExpressionPtr(), false)
    );
    TS_ASSERT_THROWS_ASSERT(
      varAdded =
        scope->AddVariable (semanticsHandler.CreateType (SemanticsHandler::Int),
                            s1::uc::String ("a"), 
                            SemanticsHandler::ExpressionPtr(), false),
      const s1::parser::Exception& e,
      TS_ASSERT_EQUALS(e.GetCode(), s1::parser::Error::IdentifierAlreadyDeclared)
    );
  }  
  
  void testIdentifierAddVarHigher (void)
  {
    using namespace s1::parser;
    
    TestSemanticsHandler semanticsHandler;
    SemanticsHandler::ScopePtr scopeOuter (
      semanticsHandler.CreateScope (SemanticsHandler::ScopePtr(),
                                    SemanticsHandler::Global));
    SemanticsHandler::NamePtr varAdded;
    TS_ASSERT_THROWS_NOTHING(
      varAdded =
        scopeOuter->AddVariable (semanticsHandler.CreateType (SemanticsHandler::Int),
                                 s1::uc::String ("a"), 
                                 SemanticsHandler::ExpressionPtr(), false)
    );
    SemanticsHandler::ScopePtr scopeInner (
      semanticsHandler.CreateScope (scopeOuter,
                                    SemanticsHandler::Function));
    SemanticsHandler::NamePtr varRequested;
    TS_ASSERT_THROWS_NOTHING(
      varRequested =
        scopeInner->ResolveIdentifier (s1::uc::String ("a"))
    );
    TS_ASSERT_EQUALS (varAdded, varRequested);
    TS_ASSERT_EQUALS (varRequested->GetType(), SemanticsHandler::Name::Variable);
  }  
  
};
