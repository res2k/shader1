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

#include "intermediate/IntermediateGeneratorSemanticsHandler.h"
#include "intermediate/SequenceOp/SequenceOpBlock.h"
#include "lexer/LexerErrorHandler.h"
#include "parser/Parser.h"
#include "BlockImpl.h"
#include "NameImpl.h"

#include "codegen/CgGenerator.h"
#include "../cg/SequenceCodeGenerator.h"

#include "StringSubstitute.h"

using namespace s1::intermediate;
using namespace s1::codegen;

class NestedBlockTestSuite : public CxxTest::TestSuite 
{
  class TestParser : public s1::Parser
  {
  public:
    TestParser (s1::Lexer& inputLexer, s1::parser::SemanticsHandler& semanticsHandler,
		s1::parser::ErrorHandler& errorHandler)
     : Parser (inputLexer, semanticsHandler, errorHandler) {}
    
    using s1::Parser::ParseBlock;
  };
  
  class TestSemanticsHandler : public IntermediateGeneratorSemanticsHandler
  {
  public:
    typedef IntermediateGeneratorSemanticsHandler Superclass;
    
    class TestBlockImpl : public BlockImpl
    {
    public:
      typedef BlockImpl Superclass;
    
      using Superclass::sequenceBuilder;
    };
    
    class TestNameImpl : public NameImpl
    {
    public:
      typedef NameImpl Superclass;
    };
  };
  
  class TestImportedNameResolver : public ImportedNameResolver
  {
  public:
    std::string GetImportedNameIdentifier (const s1::uc::String& name)
    { return std::string(); }	
    std::string GetExportedNameIdentifier (const s1::uc::String& name)
    { return std::string(); }	
  };
  
  class TestCodeGenerator : public CgGenerator
  {
  public:
    typedef CgGenerator Superclass;
    
    class TestSequenceCodeGenerator : public SequenceCodeGenerator
    {
      static const ProgramFunction::TransferMappings& EmptyMappings()
      {
        static const ProgramFunction::TransferMappings m;
        return m;
      }
    public:
      typedef SequenceCodeGenerator Superclass;
    
      TestSequenceCodeGenerator (const Sequence& seq, ImportedNameResolver* nameRes)
       : SequenceCodeGenerator (seq, nameRes,
				EmptyMappings(), EmptyMappings(),
				std::vector<std::string> ()) {}
       
      using Superclass::GetOutputRegisterName;
    };
  }; 
public:
  void testSimpleNestedBlock (void)
  {
    static const char blockSource[] =
      "unsigned int a;"
      "{"
      "  a = 1;"
      "}"
      ;
    
    using namespace s1::parser;
    
    std::istringstream in (blockSource);
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    // global scope is required so BlockImpl can create some unique var names
    SemanticsHandler::ScopePtr globalScope (semanticsHandler.CreateScope (SemanticsHandler::ScopePtr(),
					    TestSemanticsHandler::Global,
					    semanticsHandler.GetVoidType()));
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (globalScope));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    
    boost::shared_ptr<TestSemanticsHandler::TestBlockImpl> testBlockImpl (
      boost::static_pointer_cast<TestSemanticsHandler::TestBlockImpl> (block));
    
    SequencePtr sequence (testBlockImpl->sequenceBuilder->GetSequence());
    TestImportedNameResolver nameRes;
    TestCodeGenerator::TestSequenceCodeGenerator seqGen (*sequence, &nameRes);
    StringsArrayPtr generateResult (seqGen.Generate ());
    
    StringStringMap substMap;
    substMap["A"] = CgGenerator::NameToCgIdentifier (
      testBlockImpl->sequenceBuilder->GetIdentifierRegister ("a")->GetName ());
    TS_ASSERT_EQUALS(generateResult->Size(), 4);
    unsigned int l = 0;
    if (l >= generateResult->Size()) return;
    TS_ASSERT_EQUALS(generateResult->Get (l++),
		     StringSubstitute ("unsigned int $A;", substMap));
    if (l >= generateResult->Size()) return;
    TS_ASSERT_EQUALS(generateResult->Get (l++),
		     StringSubstitute ("{", substMap));
    if (l >= generateResult->Size()) return;
    TS_ASSERT_EQUALS(generateResult->Get (l++),
		     StringSubstitute ("  $A = 1;", substMap));
    if (l >= generateResult->Size()) return;
    TS_ASSERT_EQUALS(generateResult->Get (l++),
		     StringSubstitute ("}", substMap));
  }
  
};
