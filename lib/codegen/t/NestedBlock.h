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

#include "base/uc/SimpleBufferStreamSource.h"
#include "intermediate/IntermediateGeneratorSemanticsHandler.h"
#include "intermediate/SequenceBuilder.h"
#include "intermediate/SequenceOp/SequenceOpBlock.h"
#include "parser/Parser.h"
#include "BlockImpl.h"
#include "NameImpl.h"

#include "codegen/glsl/GLSLGenerator.h"
#include "codegen/glsl/GLSLOptions.h"
#include "../glsl/GLSLSequenceCodeGenerator.h"
#include "../glsl/GLSLTraits.h"

#include "StringSubstitute.h"

#include "../../diagnostics/t/TestDiagnosticsHandler.h"
#include "../../parser/t/TestParser.h"

using namespace s1::intermediate;
using namespace s1::codegen;

class NestedBlockTestSuite : public CxxTest::TestSuite 
{
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
  
  class TestImportedNameResolver : public sl::ImportedNameResolver
  {
  public:
    s1::uc::String GetImportedNameIdentifier (const s1::uc::String& name)
    { return s1::uc::String(); }	
    s1::uc::String GetExportedNameIdentifier (const s1::uc::String& name)
    { return s1::uc::String(); }	
  };
  
  class TestCodeGenerator : public glsl::Generator
  {
  public:
    typedef glsl::Generator Superclass;
    
    class TestSequenceCodeGenerator : public glsl::SequenceCodeGenerator
    {
      static const ProgramFunction::TransferMappings& EmptyMappings()
      {
        static const ProgramFunction::TransferMappings m;
        return m;
      }
      static const glsl::Options& DefaultOptions()
      {
        static const glsl::Options o (false);
        return o;
      }
      static const glsl::Traits& DefaultTraits()
      {
        static const glsl::Traits t;
        return t;
      }
    public:
      typedef SequenceCodeGenerator Superclass;
    
      TestSequenceCodeGenerator (const Sequence& seq, sl::ImportedNameResolver* nameRes)
       : SequenceCodeGenerator (seq, nameRes,
				EmptyMappings(), EmptyMappings(),
				std::vector<s1::uc::String> (),
                                DefaultTraits (), DefaultOptions ()) {}
       
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
    
    s1::uc::SimpleBufferStreamSource in (blockSource, strlen (blockSource));
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    
    // global scope is required so BlockImpl can create some unique var names
    SemanticsHandler::ScopePtr globalScope (semanticsHandler.CreateScope (SemanticsHandler::ScopePtr(),
					    TestSemanticsHandler::Global,
					    semanticsHandler.GetVoidType()));
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (globalScope));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TS_ASSERT_EQUALS (errorHandler.parseError.code, 0);
    
    boost::shared_ptr<TestSemanticsHandler::TestBlockImpl> testBlockImpl (
      boost::static_pointer_cast<TestSemanticsHandler::TestBlockImpl> (block));
    
    SequencePtr sequence (testBlockImpl->sequenceBuilder->GetSequence());
    TestImportedNameResolver nameRes;
    TestCodeGenerator::TestSequenceCodeGenerator seqGen (*sequence, &nameRes);
    StringsArrayPtr generateResult (seqGen.Generate ());
    
    StringStringMap substMap;
    glsl::Traits traits;
    traits.ConvertIdentifier (testBlockImpl->sequenceBuilder->GetIdentifierRegister ("a")->GetName ())
      .toUTF8String (substMap["A"]);
    TS_ASSERT_EQUALS(generateResult->Size(), 4);
    unsigned int l = 0;
    if (l >= generateResult->Size()) return;
    TS_ASSERT_EQUALS(generateResult->Get (l++),
		     StringSubstitute ("int $A;", substMap));
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
