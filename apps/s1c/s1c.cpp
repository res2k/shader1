#include "base/common.h"

#include <fstream>
#include <iostream>

#include "compiler/Compiler.h"
#include "compiler/Options.h"
#include "compiler/Program.h"

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
  const char* backendStr = "cg";
  bool doSplit = true;
  typedef boost::unordered_map<std::string, unsigned int> ParamMap;
  ParamMap paramFlags;
  typedef boost::unordered_map<std::string, size_t> ParamArraySizeMap;
  ParamArraySizeMap paramArraySizes;
  
  Compiler compiler;
  Compiler::OptionsPtr compilerOpts = compiler.CreateOptions();
  enum
  {
    // Optimization level compiler picks by default
    defaultOptimizationLevel = 1,
    // Optimization level compiler picks if optimization is requested, but no particular options (-O)s
    defaultEnableOptimizationLevel = 2
  };
  compilerOpts->SetOptimizationLevel (defaultOptimizationLevel);
  
  //OptimizationFlags optFlags;
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
    else if (strcmp (arg, "--param-size") == 0)
    {
      argNum++;
      std::string paramStr;
      int arraySize = -1;
      if (argNum < argc)
	paramStr = argv[argNum];
      argNum++;
      if (argNum < argc)
      {
	char dummy;
	if (sscanf (argv[argNum], "%d%c", &arraySize, &dummy) != 1)
	  arraySize = -1;
      }
      if (!paramStr.empty() && (arraySize > 0))
      {
	paramArraySizes[paramStr] = arraySize;
      }
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
      const char* optStr = arg+2;
      if (!*optStr)
	compilerOpts->SetOptimizationLevel (defaultEnableOptimizationLevel);
      else if ((*optStr >= '0') && (*optStr <= '9'))
	compilerOpts->SetOptimizationLevel (*optStr - '0');
      else
	compilerOpts->SetOptimizationFlagFromStr (optStr);
    }
    else if (strcmp (arg, "--backend") == 0)
    {
      argNum++;
      if (argNum < argc)
	backendStr = argv[argNum];
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
  
  Compiler::BackendPtr compilerBackend;
  if (strcasecmp (backendStr, "cg") == 0)
    compilerBackend = compiler.CreateBackendCg();
  else if (strcasecmp (backendStr, "latex") == 0)
    compilerBackend = compiler.CreateBackendLatex();
  else
  {
    std::cerr << "Invalid backend: " << backendStr << std::endl;
  }
  
  std::ifstream inputFile (inputFileName, std::ios_base::in | std::ios_base::binary);
  // Check for BOM
  {
    unsigned char bomBuf[3];
    inputFile.read ((char*)bomBuf, 3);
    if (inputFile.eof() || (bomBuf[0] != 0xEF) || (bomBuf[1] != 0xBB) || (bomBuf[2] != 0xBF))
    {
      // Not a BOM, reset
      inputFile.clear ();
      inputFile.seekg (0);
    }
  }
  Compiler::ProgramPtr compilerProg (compiler.CreateProgram (compilerOpts, inputFile));
  compilerProg->SetEntryFunctionName (entryName);
  
  for (ParamMap::const_iterator paramFlag = paramFlags.begin();
	paramFlag != paramFlags.end();
	++paramFlag)
  {
    compilerProg->SetInputParameterFrequencyFlags (paramFlag->first.c_str(), paramFlag->second);
  }
  for (ParamArraySizeMap::const_iterator paramSize = paramArraySizes.begin();
	paramSize != paramArraySizes.end();
	++paramSize)
  {
    compilerProg->SetInputArrayParameterSize (paramSize->first.c_str(), paramSize->second);
  }

  if (doSplit)
  {
    Compiler::Backend::ProgramPtr compiledVP (
      compilerProg->GetCompiledProgram (compilerBackend, s1::Compiler::Backend::targetVP));
    std::cout << compiledVP->GetProgramString () << std::endl;
    Compiler::Backend::ProgramPtr compiledFP (
      compilerProg->GetCompiledProgram (compilerBackend, s1::Compiler::Backend::targetFP));
    std::cout << compiledFP->GetProgramString () << std::endl;
  }
  else
  {
    Compiler::Backend::ProgramPtr compiledProgram (
      compilerProg->GetCompiledProgram (compilerBackend, s1::Compiler::Backend::targetUnsplit));
    std::cout << compiledProgram->GetProgramString () << std::endl;
  }
  
  return 0;
}
