/*
    Shader1
    Copyright (c) 2010-2014 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#ifndef __COMPILER_PROGRAM_H__
#define __COMPILER_PROGRAM_H__

#include "Compiler.h"

#include "intermediate/IntermediateGeneratorSemanticsHandler.h"
#include "splitter/Frequency.h"

#include "Backend.h"

namespace s1
{
  namespace uc
  {
    class Stream;
  } // namespace uc
  
  class Compiler::Program : public Object
  {
  protected:
    friend class Compiler;
    
    Program (uc::Stream* inputStream, const uc::String& entryFunction);
    
    OptionsPtr compilerOptions;
    
    intermediate::IntermediateGeneratorSemanticsHandler intermediateHandler;
    intermediate::ProgramPtr intermediateProg;
    
    boost::unordered_map<uc::String, unsigned int> freqFlagMap;
    boost::unordered_map<uc::String, size_t> arraySizeMap;
    intermediate::ProgramPtr splitProgs[splitter::freqNum];
    
    void SetProgramOutputParameters ();
  public:
    void SetInputParameterFrequencyFlags (const uc::String& param,
					  unsigned int frequencyFlags);
    void SetInputArrayParameterSize (const uc::String& param, size_t size);
    void SetEntryFunctionName (const uc::String& name);
    
    Compiler::Backend::ProgramPtr GetCompiledProgram (const OptionsPtr& compilerOptions,
                                                      const Compiler::BackendPtr& backend,
						      Backend::CompileTarget target);
    
    // @@@ Output: a "backend program"
    
    // Probably also: query input parameters
    // Also need to query: main function names,
    //   mappings between original and output param names,
    //   (...Backend dependent)
  };
} // namespace s1

#endif // __COMPILER_PROGRAM_H__
