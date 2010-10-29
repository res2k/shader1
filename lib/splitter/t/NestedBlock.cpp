#include <cxxtest/TestSuite.h>

#include "base/common.h"
#include "base/UnicodeStream.h"
#include "intermediate/IntermediateGeneratorSemanticsHandler.h"
#include "intermediate/Program.h"
#include "intermediate/ProgramFunction.h"
#include "lexer/Lexer.h"
#include "lexer/LexerErrorHandler.h"
#include "parser/Parser.h"
#include "splitter/ProgramSplitter.h"

#include <boost/foreach.hpp>
#include <boost/unordered_map.hpp>

class SplitterNestedBlockTestSuite : public CxxTest::TestSuite 
{
  void VerifyRegsAssignedOnce (const s1::intermediate::SequencePtr& seq)
  {
    typedef boost::unordered_map<s1::intermediate::RegisterID, size_t> AssignCountMap;
    AssignCountMap assignCount;
    
    for (size_t n = 0; n < seq->GetNumOps(); n++)
    {
      s1::intermediate::SequenceOpPtr op (seq->GetOp (n));
      s1::intermediate::RegisterIDSet writtenRegs (op->GetWrittenRegisters());
      BOOST_FOREACH(const s1::intermediate::RegisterID& reg, writtenRegs)
      {
	++(assignCount[reg]);
      }
    }
    
    for (AssignCountMap::const_iterator it = assignCount.begin();
	 it != assignCount.end();
	 ++it)
    {
      s1::intermediate::Sequence::RegisterPtr regPtr (seq->QueryRegisterPtrFromID (it->first));
      std::string regName;
      regPtr->GetName().toUTF8String (regName);
      TSM_ASSERT_EQUALS (regName, it->second, 1);
    }
  }
  
  void VerifyRegsWrittenBeforeRead (const s1::intermediate::SequencePtr& seq)
  {
    s1::intermediate::RegisterIDSet allWrittenRegs;
    
    for (size_t n = 0; n < seq->GetNumOps(); n++)
    {
      s1::intermediate::SequenceOpPtr op (seq->GetOp (n));
      s1::intermediate::RegisterIDSet readRegs (op->GetReadRegisters());
      BOOST_FOREACH(const s1::intermediate::RegisterID& reg, readRegs)
      {
	s1::intermediate::Sequence::RegisterPtr regPtr (seq->QueryRegisterPtrFromID (reg));
	std::string regName;
	regPtr->GetName().toUTF8String (regName);
	
	s1::intermediate::RegisterIDSet::const_iterator writtenRegIt = allWrittenRegs.find (reg);
	TSM_ASSERT_DIFFERS (regName, writtenRegIt, allWrittenRegs.end());
      }
      s1::intermediate::RegisterIDSet writtenRegs (op->GetWrittenRegisters());
      allWrittenRegs.insert (writtenRegs.begin(), writtenRegs.end());
    }
  }
public:
  void testNestedBlock (void)
  {
    using namespace s1::parser;
    
    std::istringstream in (
      "void main()"
      "{"
	"float x;"
	"{"
	  "x = 0.5;"
	"}"
	"float y = x;"
      "}"
    );
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::intermediate::IntermediateGeneratorSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    s1::Parser parser (lexer, semanticsHandler, parserErrorHandler);
    
    s1::splitter::ProgramSplitter splitter;

    s1::intermediate::ProgramPtr prog = semanticsHandler.GetProgram ();
    splitter.SetInputProgram (prog);
    splitter.PerformSplit();

    s1::intermediate::ProgramPtr outProg = splitter.GetOutputProgram();
    for (size_t func = 0; func < outProg->GetNumFunctions(); func++)
    {
      s1::intermediate::ProgramFunctionPtr mainFunc = outProg->GetFunction (func);
      VerifyRegsAssignedOnce (mainFunc->GetBody());
      //VerifyRegsWrittenBeforeRead ();
    }
  }
  
  
};
