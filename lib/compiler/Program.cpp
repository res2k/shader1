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

#include "base/common.h"

#include "compiler/Options.h"
#include "compiler/Program.h"

#include "base/uc/Stream.h"
#include "intermediate/Program.h"
#include "intermediate/ProgramFunction.h"
#include "lexer/Lexer.h"
#include "optimize/Optimizer.h"
#include "parser/Parser.h"
#include "splitter/ProgramSplitter.h"

#include "ErrorHandler.h"

#include <iostream>

namespace s1
{
  Compiler::Program::Program (uc::Stream* inputStream)
  {
    compiler::ErrorHandler errorHandler; // TODO: real error handler
    Lexer lexer (*inputStream, errorHandler);
    
    Parser parser (lexer, intermediateHandler, errorHandler);
    parser.Parse ();
    intermediateHandler.CompleteProgram();
  }

  void Compiler::Program::SetProgramOutputParameters ()
  {
    // Determine output values
    uc::String vertexOutput;
    uc::String fragmentOutput;
    
    for (size_t i = 0; i < intermediateProg->GetNumFunctions(); i++)
    {
      intermediate::ProgramFunctionPtr func = intermediateProg->GetFunction (i);
      if (!func->IsEntryFunction()) continue;
      
      const parser::SemanticsHandler::Scope::FunctionFormalParameters& funcParams = func->GetParams();
      for(const parser::SemanticsHandler::Scope::FunctionFormalParameter& param : funcParams)
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
    
    intermediateProg->SetOutputParameter (vertexOutput, intermediate::Program::Position);
    intermediateProg->SetOutputParameter (fragmentOutput, intermediate::Program::Color);
  }

  Compiler::Backend::ProgramPtr Compiler::Program::GetCompiledProgram (const uc::String& entryFunction,
                                                                       const OptionsPtr& compilerOptions,
                                                                       const FreqFlagMap& inputParamFreqs,
                                                                       const ArraySizeMap& arraySizes,
                                                                       const Compiler::BackendPtr& backend,
								       Backend::CompileTarget target,
                                                                       Backend::OptionsPtr options)
  {
    if (!intermediateProg)
    {
      intermediateProg = intermediateHandler.GetProgram(entryFunction);
      SetProgramOutputParameters ();
      
      for (unsigned int n = 0; n < splitter::freqNum; n++)
	splitProgs[n] = intermediate::ProgramPtr();
    }

    intermediateProg->SetParameterArraySizes (arraySizes);

    optimize::Optimizer opt;
    opt.SetInlineBlocks (compilerOptions->GetOptimizationFlag (Options::optBlockInlining));
    opt.SetDeadCodeElimination (compilerOptions->GetOptimizationFlag (Options::optDeadCodeElimination));
    opt.SetConstantFolding (compilerOptions->GetOptimizationFlag (Options::optConstantFolding));
    intermediate::ProgramPtr optProg = opt.ApplyOptimizations (intermediateProg);

    switch (target)
    {
    case Backend::targetVP:
    case Backend::targetFP:
      {
	if (!splitProgs[splitter::freqVertex] || !splitProgs[splitter::freqFragment])
	{
	  splitter::ProgramSplitter splitter;
	  
	  typedef std::pair<uc::String, unsigned int> FreqFlagPair;
	  for(FreqFlagPair freqFlag : inputParamFreqs)
	  {
	    splitter.SetInputFreqFlags (freqFlag.first, freqFlag.second);
	  }
	  
	  splitter.SetInputProgram (optProg);
	  splitter.PerformSplit();
	  
	  optimize::Optimizer opt;
	  opt.SetInlineBlocks (compilerOptions->GetOptimizationFlag (Options::optBlockInlining));
	  opt.SetDeadCodeElimination (compilerOptions->GetOptimizationFlag (Options::optDeadCodeElimination));
	  opt.SetConstantFolding (compilerOptions->GetOptimizationFlag (Options::optConstantFolding));
	  splitProgs[splitter::freqVertex] = opt.ApplyOptimizations (splitter.GetOutputProgram (splitter::freqVertex));
	  splitProgs[splitter::freqFragment] = opt.ApplyOptimizations (splitter.GetOutputProgram (splitter::freqFragment));
	}
	
	return backend->GenerateProgram (target,
	  target == Backend::targetVP ? splitProgs[splitter::freqVertex] : splitProgs[splitter::freqFragment],
          options);
      }
      break;
    case Backend::targetUnsplit:
      return backend->GenerateProgram (target, optProg, options);
    }
    
    assert (false);
    return Backend::ProgramPtr();
  }
} // namespace s1
