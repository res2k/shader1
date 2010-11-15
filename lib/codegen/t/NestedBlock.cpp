#include <cxxtest/TestSuite.h>
#include <boost/cstdint.hpp>

#include "intermediate/IntermediateGeneratorSemanticsHandler.h"
#include "intermediate/SequenceOp/SequenceOpBlock.h"
#include "lexer/LexerErrorHandler.h"
#include "parser/Parser.h"
#include "../../intermediate/BlockImpl.h"

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
    
      using Superclass::sequence;
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
    std::string GetImportedNameIdentifier (const UnicodeString& name)
    { return std::string(); }	
    std::string GetExportedNameIdentifier (const UnicodeString& name)
    { return std::string(); }	
  };
  
  class TestCodeGenerator : public CgGenerator
  {
  public:
    typedef CgGenerator Superclass;
    
    class TestSequenceCodeGenerator : public SequenceCodeGenerator
    {
    public:
      typedef SequenceCodeGenerator Superclass;
    
      TestSequenceCodeGenerator (const Sequence& seq, ImportedNameResolver* nameRes)
       : SequenceCodeGenerator (seq, nameRes,
				ProgramFunction::TransferMappings(),
				ProgramFunction::TransferMappings(),
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
      boost::shared_static_cast<TestSemanticsHandler::TestBlockImpl> (block));
    
    TestImportedNameResolver nameRes;
    TestCodeGenerator::TestSequenceCodeGenerator seqGen (*(testBlockImpl->sequence), &nameRes);
    StringsArrayPtr generateResult (seqGen.Generate ());
    
    // First (and only) op in sequence is a block op
    boost::shared_ptr<s1::intermediate::SequenceOpBlock> blockOp (
      boost::shared_static_cast<s1::intermediate::SequenceOpBlock> (testBlockImpl->sequence->GetOp (0)));
    
    StringStringMap substMap;
    substMap["A"] = CgGenerator::NameToCgIdentifier (
      testBlockImpl->sequence->GetIdentifierRegister ("a")->GetName ());
    substMap["A_in_block"] = CgGenerator::NameToCgIdentifier (
      blockOp->GetSequence()->GetIdentifierRegister ("a")->GetName());
    int l = 0;
    TS_ASSERT_EQUALS(generateResult->Size(), 6);
    TS_ASSERT_EQUALS(generateResult->Get (l++),
		     StringSubstitute ("unsigned int $A;", substMap));
    TS_ASSERT_EQUALS(generateResult->Get (l++),
		     StringSubstitute ("{", substMap));
    TS_ASSERT_EQUALS(generateResult->Get (l++),
		     StringSubstitute ("  unsigned int $A_in_block;", substMap));
    TS_ASSERT_EQUALS(generateResult->Get (l++),
		     StringSubstitute ("  $A_in_block = 1;", substMap));
    TS_ASSERT_EQUALS(generateResult->Get (l++),
		     StringSubstitute ("  $A = $A_in_block;", substMap));
    TS_ASSERT_EQUALS(generateResult->Get (l++),
		     StringSubstitute ("}", substMap));
  }
  
};
