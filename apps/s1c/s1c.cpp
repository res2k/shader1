/*
    Shader1
    Copyright (c) 2010-2014 Frank Richter


    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

      1. The origin of this software must not be misrepresented; you must not
      claim that you wrote the original software. If you use this software
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.

      2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original software.

      3. This notice may not be removed or altered from any source
      distribution.
*/

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
  bool noSplit = false;
  typedef boost::unordered_map<std::string, s1_InputFrequency> ParamMap;
  ParamMap paramFlags;
  typedef boost::unordered_map<std::string, size_t> ParamArraySizeMap;
  ParamArraySizeMap paramArraySizes;
  
  Ptr<Library> lib;
  ResultCode libErr (Library::Create (lib));
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
    else if (strcmp (arg, "--nosplit") == 0)
    {
      noSplit = true;
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
    size_t startPos (inputFile.tellg());
    inputFile.seekg (0, std::ios_base::end);
    size_t fileSize (inputFile.tellg());
    inputFile.seekg (startPos, std::ios_base::beg);
    
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

  if (noSplit)
  {
    CompiledProgram::Pointer compiled (
      compilerBackend->GenerateProgram (compilerProg, S1_TARGET_UNSPLIT));
    std::cout << compiled->GetString() << std::endl;
  }
  else
  {
    CompiledProgram::Pointer compiledVP (
      compilerBackend->GenerateProgram (compilerProg, S1_TARGET_VP));
    std::cout << compiledVP->GetString() << std::endl;
    CompiledProgram::Pointer compiledFP (
      compilerBackend->GenerateProgram (compilerProg, S1_TARGET_FP));
    std::cout << compiledFP->GetString () << std::endl;
  }
  return 0;
}
