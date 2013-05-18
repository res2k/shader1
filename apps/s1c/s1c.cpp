#include "base/common.h"

#include <fstream>
#include <iostream>

#include "s1/Backend.h"
#include "s1/CompiledProgram.h"
#include "s1/Error.h"
#include "s1/Library.h"
#include "s1/Options.h"

#include <boost/unordered_map.hpp>

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
  typedef boost::unordered_map<std::string, s1_InputFrequency> ParamMap;
  ParamMap paramFlags;
  typedef boost::unordered_map<std::string, size_t> ParamArraySizeMap;
  ParamArraySizeMap paramArraySizes;
  
  Ptr<Library> lib;
  ErrorCode libErr (Library::Create (lib));
  if (!S1_SUCCESSFUL(libErr))
  {
    // TODO: Print error code
    return 2;
  }
  
  Options::Pointer compilerOpts = lib->CreateOptions ();
  enum
  {
    // Optimization level compiler picks by default
    defaultOptimizationLevel = 1,
    // Optimization level compiler picks if optimization is requested, but no particular options (-O)s
    defaultEnableOptimizationLevel = 2
  };
  compilerOpts->SetOptLevel (defaultOptimizationLevel);
  
  int argNum = 1;
  while (argNum < argc)
  {
    const char* arg = argv[argNum];
    if (strcmp (arg, "--param-vertex") == 0)
    {
      argNum++;
      if (argNum < argc)
	paramFlags[argv[argNum]] = S1_FREQ_VERTEX;
    }
    else if (strcmp (arg, "--param-uniform") == 0)
    {
      argNum++;
      if (argNum < argc)
	paramFlags[argv[argNum]] = S1_FREQ_UNIFORM;
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
    else if (strncmp (arg, "-O", 2) == 0)
    {
      const char* optStr = arg+2;
      if (!*optStr)
	compilerOpts->SetOptLevel (defaultEnableOptimizationLevel);
      else if ((*optStr >= '0') && (*optStr <= '9'))
	compilerOpts->SetOptLevel (*optStr - '0');
      else
	compilerOpts->SetOptFlagFromStr (optStr);
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
  
  Backend::Pointer compilerBackend (lib->CreateBackend (backendStr));
  if (!compilerBackend)
  {
    std::cerr << "Invalid backend: " << backendStr << std::endl;
    return 2;
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
  std::string sourceStr;
  {
    inputFile.seekg (0, std::ios_base::end);
    size_t fileSize (inputFile.tellg());
    inputFile.seekg (0, std::ios_base::beg);
    
    sourceStr.reserve (fileSize);
    while (fileSize > 0)
    {
      char buf[256];
      inputFile.read (buf, sizeof (buf));
      size_t nRead (inputFile.gcount());
      if (nRead == 0) break;
      sourceStr.append (buf, nRead);
      fileSize -= nRead;
    }
    inputFile.close();
  }
  
  Program::Pointer compilerProg (lib->CreateProgramFromString (sourceStr.c_str(), sourceStr.size()));
  if (!compilerProg)
  {
    std::cerr << "Error creating program" << std::endl;
    return 3;
  }
  compilerProg->SetEntryFunction (entryName);
  compilerProg->SetOptions (compilerOpts);
  
  for (ParamMap::const_iterator paramFlag = paramFlags.begin();
	paramFlag != paramFlags.end();
	++paramFlag)
  {
    compilerProg->SetInputFrequency (paramFlag->first.c_str(), paramFlag->second);
    // TODO: Error checking
  }
  for (ParamArraySizeMap::const_iterator paramSize = paramArraySizes.begin();
	paramSize != paramArraySizes.end();
	++paramSize)
  {
    compilerProg->SetInputArraySize (paramSize->first.c_str(), paramSize->second);
    // TODO: Error checking
  }

  CompiledProgram::Pointer compiledVP (
    compilerBackend->GenerateProgram (compilerProg, S1_TARGET_VP));
  std::cout << compiledVP->GetString() << std::endl;
  CompiledProgram::Pointer compiledFP (
    compilerBackend->GenerateProgram (compilerProg, S1_TARGET_FP));
  std::cout << compiledFP->GetString () << std::endl;
  return 0;
}
