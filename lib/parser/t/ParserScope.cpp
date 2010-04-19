#include <cxxtest/TestSuite.h>

#include "base/hash_UnicodeString.h"

#include "lexer/LexerErrorHandler.h"
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
			    UnicodeString ("a"), 
			    SemanticsHandler::ExpressionPtr(), false)
    );
    SemanticsHandler::NamePtr varRequested;
    TS_ASSERT_THROWS_NOTHING(
      varRequested =
	scope->ResolveIdentifier (UnicodeString ("a"))
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
			  UnicodeString ("a"), params)
    );
    SemanticsHandler::NamePtr varRequested;
    TS_ASSERT_THROWS_NOTHING(
      varRequested =
	scope->ResolveIdentifier (UnicodeString ("a"))
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
			     UnicodeString ("a"))
    );
    SemanticsHandler::NamePtr varRequested;
    TS_ASSERT_THROWS_NOTHING(
      varRequested =
	scope->ResolveIdentifier (UnicodeString ("a"))
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
	scope->ResolveIdentifier (UnicodeString ("foo")),
      const s1::parser::Exception& e,
      e.GetCode() == s1::parser::IdentifierUndeclared
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
			    UnicodeString ("a"), 
			    SemanticsHandler::ExpressionPtr(), false)
    );
    TS_ASSERT_THROWS_ASSERT(
      varAdded =
	scope->AddVariable (semanticsHandler.CreateType (SemanticsHandler::Int),
			    UnicodeString ("a"), 
			    SemanticsHandler::ExpressionPtr(), false),
      const s1::parser::Exception& e,
      e.GetCode() == s1::parser::IdentifierAlreadyDeclared
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
				 UnicodeString ("a"), 
				 SemanticsHandler::ExpressionPtr(), false)
    );
    SemanticsHandler::ScopePtr scopeInner (
      semanticsHandler.CreateScope (scopeOuter,
				    SemanticsHandler::Function));
    SemanticsHandler::NamePtr varRequested;
    TS_ASSERT_THROWS_NOTHING(
      varRequested =
	scopeInner->ResolveIdentifier (UnicodeString ("a"))
    );
    TS_ASSERT_EQUALS (varAdded, varRequested);
    TS_ASSERT_EQUALS (varRequested->GetType(), SemanticsHandler::Name::Variable);
  }  
  
};
