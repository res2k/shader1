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

#include <boost/test/unit_test.hpp>

#include "base/common.h"

#include "parser/Diagnostics.h"
#include "parser/Parser.h"
#include "semantics/Diagnostics.h"

#include "TestSemanticsHandler.h"

#include "print_semantics_Error.h"

BOOST_AUTO_TEST_SUITE(ParserScope)

BOOST_AUTO_TEST_CASE(IdentifierAddVar)
{
  using namespace s1::parser;

  TestSemanticsHandler semanticsHandler;
  s1::semantics::ScopePtr scope (
    semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                  s1::semantics::ScopeLevel::Global));
  s1::semantics::NamePtr varAdded;
  BOOST_CHECK_NO_THROW(
    varAdded =
      scope->AddVariable (semanticsHandler.CreateType (s1::semantics::BaseType::Int),
                          s1::uc::String ("a"), 
                          s1::semantics::ExpressionPtr(), false)
  );
  s1::semantics::NamePtr varRequested;
  BOOST_CHECK_NO_THROW(
    varRequested =
      scope->ResolveIdentifier (s1::uc::String ("a")).value()
  );
  BOOST_CHECK_EQUAL (varAdded, varRequested);
  BOOST_CHECK_EQUAL (varRequested->GetType(), s1::semantics::Name::Variable);
}

BOOST_AUTO_TEST_CASE(IdentifierAddFunc)
{
  using namespace s1::parser;

  TestSemanticsHandler semanticsHandler;
  s1::semantics::ScopePtr scope (
    semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                  s1::semantics::ScopeLevel::Global));
  s1::semantics::Scope::FunctionFormalParameters params;
  BOOST_CHECK_NO_THROW(
    scope->AddFunction (semanticsHandler.CreateType (s1::semantics::BaseType::Int),
                        s1::uc::String ("a"), params)
  );
  s1::semantics::NamePtr varRequested;
  BOOST_CHECK_NO_THROW(
    varRequested =
      scope->ResolveIdentifier (s1::uc::String ("a")).value()
  );
  BOOST_CHECK_NE (varRequested, s1::semantics::NamePtr ());
  BOOST_CHECK_EQUAL (varRequested->GetType(), s1::semantics::Name::Function);
}

BOOST_AUTO_TEST_CASE(IdentifierAddType)
{
  using namespace s1::parser;

  TestSemanticsHandler semanticsHandler;
  s1::semantics::ScopePtr scope (
    semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                  s1::semantics::ScopeLevel::Global));
  s1::semantics::NamePtr varAdded;
  BOOST_CHECK_NO_THROW(
    varAdded =
      scope->AddTypeAlias (semanticsHandler.CreateType (s1::semantics::BaseType::Int),
                            s1::uc::String ("a"))
  );
  s1::semantics::NamePtr varRequested;
  BOOST_CHECK_NO_THROW(
    varRequested =
      scope->ResolveIdentifier (s1::uc::String ("a")).value()
  );
  BOOST_CHECK_EQUAL (varAdded, varRequested);
  BOOST_CHECK_EQUAL (varRequested->GetType(), s1::semantics::Name::TypeAlias);
}

BOOST_AUTO_TEST_CASE(IdentifierUnknown)
{
  using namespace s1::parser;

  TestSemanticsHandler semanticsHandler;
  s1::semantics::ScopePtr scope (
    semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                  s1::semantics::ScopeLevel::Global));
  s1::semantics::Scope::result_NamePtr varRequested = s1::semantics::NamePtr ();
  BOOST_CHECK_NO_THROW(
    varRequested =
      scope->ResolveIdentifier (s1::uc::String ("foo"))
  );
  BOOST_CHECK_EQUAL(varRequested.error(), s1::semantics::Error::IdentifierUndeclared);
}

BOOST_AUTO_TEST_CASE(IdentifierDeclareMultiple)
{
  using namespace s1::parser;

  TestSemanticsHandler semanticsHandler;
  s1::semantics::ScopePtr scope (
    semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                  s1::semantics::ScopeLevel::Global));
  s1::semantics::NamePtr varAdded1, varAdded2;
  BOOST_CHECK_NO_THROW(
    varAdded1 =
      scope->AddVariable (semanticsHandler.CreateType (s1::semantics::BaseType::Int),
                          s1::uc::String ("a"), 
                          s1::semantics::ExpressionPtr(), false)
  );
  BOOST_CHECK_NO_THROW(
    varAdded2 =
      scope->AddVariable (semanticsHandler.CreateType (s1::semantics::BaseType::Int),
                          s1::uc::String ("a"), 
                          s1::semantics::ExpressionPtr(), false)
  );
  BOOST_CHECK(!varAdded2);
}

BOOST_AUTO_TEST_CASE(IdentifierAddVarHigher)
{
  using namespace s1::parser;

  TestSemanticsHandler semanticsHandler;
  s1::semantics::ScopePtr scopeOuter (
    semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                  s1::semantics::ScopeLevel::Global));
  s1::semantics::NamePtr varAdded;
  BOOST_CHECK_NO_THROW(
    varAdded =
      scopeOuter->AddVariable (semanticsHandler.CreateType (s1::semantics::BaseType::Int),
                                s1::uc::String ("a"), 
                                s1::semantics::ExpressionPtr(), false)
  );
  s1::semantics::ScopePtr scopeInner (
    semanticsHandler.CreateScope (scopeOuter,
                                  s1::semantics::ScopeLevel::Function));
  s1::semantics::NamePtr varRequested;
  BOOST_CHECK_NO_THROW(
    varRequested =
      scopeInner->ResolveIdentifier (s1::uc::String ("a")).value()
  );
  BOOST_CHECK_EQUAL (varAdded, varRequested);
  BOOST_CHECK_EQUAL (varRequested->GetType(), s1::semantics::Name::Variable);
}

BOOST_AUTO_TEST_SUITE_END()
