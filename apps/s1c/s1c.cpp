#include "base/common.h"

#include <fstream>
#include <iostream>

#include "base/UnicodeStream.h"
#include "codegen/CgGenerator.h"
#include "intermediate/Exception.h"
#include "intermediate/IntermediateGeneratorSemanticsHandler.h"
#include "intermediate/ProgramFunction.h" // @@@ Temp.
#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "splitter/SequenceSplitter.h"

#include "ErrorHandler.h"
#include <boost/make_shared.hpp>

using namespace s1;

int main (const int argc, const char* const argv[])
{
  if (argc < 2)
  {
    std::cout << "Syntax: " << argv[0] << " [input filename]" << std::endl;
    return 1;
  }
  
  ErrorHandler errorHandler;
  std::ifstream inputFile (argv[1], std::ios_base::in | std::ios_base::binary);
  UnicodeStream uniStream (inputFile, "utf-8");
  // Skip BOM
  if (*uniStream == 0xFEFF) ++uniStream;
  
  Lexer lexer (uniStream, errorHandler);
  
  intermediate::IntermediateGeneratorSemanticsHandler intermediateHandler;
  intermediateHandler.SetEntryFunction ("main");
  Parser parser (lexer, intermediateHandler, errorHandler);
  try
  {
    parser.Parse ();
  }
  catch (intermediate::Exception& e)
  {
    errorHandler.IntermediateError (e.GetCode());
  }
  
  intermediate::ProgramPtr progV (boost::make_shared<intermediate::Program> ());
  intermediate::ProgramPtr progF (boost::make_shared<intermediate::Program> ());
  
  {
    intermediate::ProgramPtr prog = intermediateHandler.GetProgram ();
    
    for (size_t i = 0; i < prog->GetNumFunctions(); i++)
    {
      intermediate::ProgramFunctionPtr func = prog->GetFunction (i);
      splitter::SequenceSplitter splitter;
      splitter.SetInputSequence (func->GetBody());
      splitter.PerformSplit();
      
      intermediate::ProgramFunctionPtr funcV (boost::make_shared<intermediate::ProgramFunction> (func->GetIdentifier(),
												 func->GetReturnType(),
												 func->GetParams(),
												 splitter.GetOutputVertexSequence(),
												 func->IsEntryFunction()));
      progV->AddFunction (funcV);
      intermediate::ProgramFunctionPtr funcF (boost::make_shared<intermediate::ProgramFunction> (func->GetIdentifier(),
												 func->GetReturnType(),
												 func->GetParams(),
												 splitter.GetOutputFragmentSequence(),
												 func->IsEntryFunction()));
      progF->AddFunction (funcF);
    }
  }
  
  /*
  codegen::CgGenerator codegen;
  codegen::StringsArrayPtr progOutput (codegen.Generate (intermediateHandler.GetProgram ()));
  
  for (size_t i = 0; i < progOutput->Size(); i++)
  {
    std::cout << progOutput->Get (i) << std::endl;
  }
  */
  {
    codegen::CgGenerator codegen;
    codegen::StringsArrayPtr progOutput (codegen.Generate (progV));
    
    for (size_t i = 0; i < progOutput->Size(); i++)
    {
      std::cout << progOutput->Get (i) << std::endl;
    }
  }
  {
    codegen::CgGenerator codegen;
    codegen::StringsArrayPtr progOutput (codegen.Generate (progF));
    
    for (size_t i = 0; i < progOutput->Size(); i++)
    {
      std::cout << progOutput->Get (i) << std::endl;
    }
  }
  
  return 0;
}
