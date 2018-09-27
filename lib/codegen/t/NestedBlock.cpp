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

#include <boost/test/unit_test.hpp>

#include "base/common.h"
#include "base/intrusive_ptr.h"

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

BOOST_AUTO_TEST_SUITE(NestedBlock)

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

BOOST_AUTO_TEST_CASE(SimpleNestedBlock)
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
  s1::semantics::ScopePtr globalScope (
    semanticsHandler.CreateScope (s1::semantics::ScopePtr(),
                                  s1::semantics::ScopeLevel::Global,
                                  semanticsHandler.GetVoidType()));
  s1::semantics::BlockPtr block (
    semanticsHandler.CreateBlock (globalScope));
  BOOST_CHECK_NO_THROW(parser.ParseBlock (block));
  BOOST_CHECK(errorHandler.parseErrors.empty());

  auto testBlockImpl = s1::get_static_ptr<TestSemanticsHandler::TestBlockImpl> (block);

  SequencePtr sequence (testBlockImpl->sequenceBuilder->GetSequence());
  TestImportedNameResolver nameRes;
  TestCodeGenerator::TestSequenceCodeGenerator seqGen (*sequence, &nameRes);
  StringsArrayPtr generateResult (seqGen.Generate ());

  StringStringMap substMap;
  glsl::Traits traits;
  traits.ConvertIdentifier (testBlockImpl->sequenceBuilder->GetIdentifierRegister ("a")->GetName ())
    .toUTF8String (substMap["A"]);
  BOOST_CHECK_EQUAL(generateResult->Size(), 4);
  unsigned int l = 0;
  if (l >= generateResult->Size()) return;
  BOOST_CHECK_EQUAL(generateResult->Get (l++),
                   StringSubstitute ("int $A;", substMap));
  if (l >= generateResult->Size()) return;
  BOOST_CHECK_EQUAL(generateResult->Get (l++),
                   StringSubstitute ("{", substMap));
  if (l >= generateResult->Size()) return;
  BOOST_CHECK_EQUAL(generateResult->Get (l++),
                   StringSubstitute ("  $A = 1;", substMap));
  if (l >= generateResult->Size()) return;
  BOOST_CHECK_EQUAL(generateResult->Get (l++),
                   StringSubstitute ("}", substMap));
}

BOOST_AUTO_TEST_SUITE_END()
