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
#include "splitter/Frequency.h"
#include "splitter/ProgramSplitter.h"

#include "ErrorHandler.h"
#include <boost/make_shared.hpp>
#include <string.h>

using namespace s1;

static void PrintSyntax (const char* execName)
{
  std::cerr << "Syntax: " << execName << " [input filename]" << std::endl;
}

int main (const int argc, const char* const argv[])
{
  const char* inputFileName = 0;
  const char* entryName = "main";
  typedef std::tr1::unordered_map<std::string, unsigned int> ParamMap;
  ParamMap paramFlags;
  int argNum = 1;
  while (argNum < argc)
  {
    const char* arg = argv[argNum];
    if (strcmp (arg, "--param-vertex") == 0)
    {
      argNum++;
      if (argNum < argc)
	paramFlags[argv[argNum]] |= splitter::freqFlagV;
    }
    else if (strcmp (arg, "--param-fragment") == 0)
    {
      argNum++;
      if (argNum < argc)
	paramFlags[argv[argNum]] |= splitter::freqFlagF;
    }
    else if (strcmp (arg, "--param-uniform") == 0)
    {
      argNum++;
      if (argNum < argc)
	paramFlags[argv[argNum]] |= splitter::freqFlagU;
    }
    else if (strcmp (arg, "--entry") == 0)
    {
      argNum++;
      if (argNum < argc)
	entryName = argv[argNum];
    }
    else
    {
      if (inputFileName != 0)
      {
	PrintSyntax (argv[0]);
	return 1;
      }
      inputFileName = arg;
    }
    argNum++;
  }
  
  if (inputFileName == 0)
  {
    PrintSyntax (argv[0]);
    return 1;
  }
  
  ErrorHandler errorHandler;
  std::ifstream inputFile (inputFileName, std::ios_base::in | std::ios_base::binary);
  UnicodeStream uniStream (inputFile, "utf-8");
  // Skip BOM
  if (*uniStream == 0xFEFF) ++uniStream;
  
  Lexer lexer (uniStream, errorHandler);
  
  intermediate::IntermediateGeneratorSemanticsHandler intermediateHandler;
  intermediateHandler.SetEntryFunction (entryName);
  Parser parser (lexer, intermediateHandler, errorHandler);
  try
  {
    parser.Parse ();
  }
  catch (intermediate::Exception& e)
  {
    errorHandler.IntermediateError (e.GetCode());
  }
  
#if 0
  intermediate::ProgramPtr progV (boost::make_shared<intermediate::Program> ());
  intermediate::ProgramPtr progF (boost::make_shared<intermediate::Program> ());
  
  // TODO: Handle param flags
  
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
#else
  splitter::ProgramSplitter splitter;

  splitter.SetInputProgram (intermediateHandler.GetProgram ());
  for (ParamMap::const_iterator paramFlag = paramFlags.begin();
	paramFlag != paramFlags.end();
	++paramFlag)
  {
    splitter.SetInputFreqFlags (paramFlag->first.c_str(), paramFlag->second);
  }
  splitter.PerformSplit();

  codegen::CgGenerator codegen;
  codegen::StringsArrayPtr progOutput (codegen.Generate (splitter.GetOutputProgram()));
  
  for (size_t i = 0; i < progOutput->Size(); i++)
  {
    std::cout << progOutput->Get (i) << std::endl;
  }
#endif
  
  return 0;
}
