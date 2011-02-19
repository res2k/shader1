#ifndef __COMPILER_PROGRAM_H__
#define __COMPILER_PROGRAM_H__

#include "Compiler.h"

#include "intermediate/IntermediateGeneratorSemanticsHandler.h"
#include "splitter/Frequency.h"

#include "Backend.h"

namespace s1
{
  class UnicodeStream;
  
  class Compiler::Program
  {
  protected:
    friend class Compiler;
    
    Program (const OptionsPtr& compilerOptions,
	     UnicodeStream* inputStream);
    
    OptionsPtr compilerOptions;
    
    intermediate::IntermediateGeneratorSemanticsHandler intermediateHandler;
    intermediate::ProgramPtr intermediateProg;
    
    boost::unordered_map<UnicodeString, unsigned int> freqFlagMap;
    intermediate::ProgramPtr splitProgs[splitter::freqNum];
    
    void SetProgramOutputParameters ();
  public:
    void SetInputParameterFrequencyFlags (const UnicodeString& param,
					  unsigned int frequencyFlags);
    void SetEntryFunctionName (const UnicodeString& name);
    
    Compiler::Backend::ProgramPtr GetCompiledProgram (const Compiler::BackendPtr& backend,
						      Backend::CompileTarget target);
    
    // @@@ Output: a "backend program"
    
    // Probably also: query input parameters
    // Also need to query: main function names,
    //   mappings between original and output param names,
    //   (...Backend dependent)
  };
} // namespace s1

#endif // __COMPILER_PROGRAM_H__
