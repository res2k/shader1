#include <cxxtest/TestSuite.h>

#include "base/common.h"
#include "base/UnicodeStream.h"
#include "intermediate/IntermediateGeneratorSemanticsHandler.h"
#include "intermediate/Program.h"
#include "intermediate/ProgramFunction.h"
#include "intermediate/SequenceOp/SequenceOp.h"
#include "lexer/Lexer.h"
#include "lexer/LexerErrorHandler.h"
#include "parser/Parser.h"
#include "splitter/ProgramSplitter.h"

#include "../../intermediate/t/CommonChecks.h"

class SplitterNestedBlockTestSuite : public CxxTest::TestSuite 
{
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
