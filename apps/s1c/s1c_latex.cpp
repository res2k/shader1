#include "base/common.h"

#include <fstream>
#include <iostream>

#include "base/UnicodeStream.h"
#include "codegen/LatexGenerator.h"
#include "intermediate/Exception.h"
#include "intermediate/IntermediateGeneratorSemanticsHandler.h"
#include "intermediate/Program.h"
#include "intermediate/ProgramFunction.h" // @@@ Temp.
#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "optimize/Optimizer.h"
#include "splitter/Frequency.h"
#include "splitter/ProgramSplitter.h"

#include "ErrorHandler.h"
#include "OptimizationFlags.h"

#include <boost/make_shared.hpp>
#include <boost/foreach.hpp>
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
  bool doSplit = true;
  typedef boost::unordered_map<std::string, unsigned int> ParamMap;
  ParamMap paramFlags;
  OptimizationFlags optFlags;
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
    else if (strcmp (arg, "--nosplit") == 0)
    {
      doSplit = false;
    }
    else if (strncmp (arg, "-O", 2) == 0)
    {
      if (!optFlags.ParseFlag (arg+2))
      {
	PrintSyntax (argv[0]);
	return 1;
      }
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
  
  optimize::Optimizer opt;
  optFlags.ApplyFlags (opt);
  
  intermediate::ProgramPtr prog = intermediateHandler.GetProgram ();
  // Determine output values
  {
    UnicodeString vertexOutput;
    UnicodeString fragmentOutput;
    
    for (size_t i = 0; i < prog->GetNumFunctions(); i++)
    {
      intermediate::ProgramFunctionPtr func = prog->GetFunction (i);
      if (!func->IsEntryFunction()) continue;
      
      const parser::SemanticsHandler::Scope::FunctionFormalParameters& funcParams = func->GetParams();
      BOOST_FOREACH(const parser::SemanticsHandler::Scope::FunctionFormalParameter& param, funcParams)
      {
	if (!(param.dir & parser::SemanticsHandler::Scope::dirOut)) continue;
	
	// Look for float4 output
	if (param.type->GetTypeClass() != parser::SemanticsHandler::Type::Vector) continue;
	if (param.type->GetArrayVectorMatrixBaseType()->GetBaseType() != parser::SemanticsHandler::Float) continue;
	if (param.type->GetVectorTypeComponents() != 4) continue;
	
	// Parameter qualifies
	if (vertexOutput.isEmpty())
	  vertexOutput = param.identifier;
	else if (fragmentOutput.isEmpty())
	  fragmentOutput = param.identifier;
	else
	{
	  // Make 'proper' warning
	  std::cerr << "Entry function has too many 'float4' outputs" << std::endl;
	}
      }
    }
    
    if (vertexOutput.isEmpty() || fragmentOutput.isEmpty())
    {
      // Make 'proper' warning
      std::cerr << "Entry function has not enough 'float4' outputs" << std::endl;
    }

    prog->SetOutputParameter (vertexOutput, intermediate::Program::Position);
    prog->SetOutputParameter (fragmentOutput, intermediate::Program::Color);
  }
  prog = opt.ApplyOptimizations (prog);
  
  if (doSplit)
  {
    splitter::ProgramSplitter splitter;

    splitter.SetInputProgram (prog);
    for (ParamMap::const_iterator paramFlag = paramFlags.begin();
	  paramFlag != paramFlags.end();
	  ++paramFlag)
    {
      splitter.SetInputFreqFlags (paramFlag->first.c_str(), paramFlag->second);
    }
    splitter.PerformSplit();

    for (int f = 0; f < splitter::freqNum; f++)
    {
      if (f > 0) std::cout << "\\vspace{0.5em}" << std::endl;
      
      prog = opt.ApplyOptimizations (splitter.GetOutputProgram (f));
      codegen::LatexGenerator codegen;
      codegen::StringsArrayPtr progOutput (codegen.Generate (prog));
      
      for (size_t i = 0; i < progOutput->Size(); i++)
      {
	std::cout << progOutput->Get (i) << std::endl;
      }
      
      if ((f < splitter::freqFragment)
	&& (!splitter.GetOutputProgram (f)->GetTransferValues().empty()))
      {
	std::cout << "\\sOtransfer{" << std::endl;
	const intermediate::Program::TransferValues& transferValues = splitter.GetOutputProgram (f)->GetTransferValues();
	for (intermediate::Program::TransferValues::const_iterator transferVal = transferValues.begin();
	    transferVal != transferValues.end();
	    ++transferVal)
	{
	  std::string str;
	  transferVal->second.toUTF8String (str);
	  std::cout << "\\sOreg{" << codegen::LatexGenerator::LatexEscape (str) << "}\\ ";
	}
	std::cout << std::endl << "}" << std::endl;
      }
    }
  }
  else
  {
    codegen::LatexGenerator codegen;
    codegen::StringsArrayPtr progOutput (codegen.Generate (prog));
    
    for (size_t i = 0; i < progOutput->Size(); i++)
    {
      std::cout << progOutput->Get (i) << std::endl;
    }
  }
  
  return 0;
}
