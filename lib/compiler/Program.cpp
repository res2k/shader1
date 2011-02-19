#include "base/common.h"

#include "compiler/Program.h"

#include "base/UnicodeStream.h"
#include "compiler/Options.h"
#include "intermediate/Program.h"
#include "intermediate/ProgramFunction.h"
#include "lexer/Lexer.h"
#include "lexer/LexerErrorHandler.h"
#include "optimize/Optimizer.h"
#include "parser/Parser.h"
#include "splitter/ProgramSplitter.h"

#include <boost/foreach.hpp>

#include <iostream>

namespace s1
{
  Compiler::Program::Program (const OptionsPtr& compilerOptions,
			      UnicodeStream* inputStream)
   : compilerOptions (compilerOptions)
  {
    LexerErrorHandler lexerErrorHandler; // TODO: real error handler
    Lexer lexer (*inputStream, lexerErrorHandler);
    
    //intermediateHandler.SetEntryFunction (entryName);
    parser::ErrorHandler parserErrorHandler; // TODO: real error handler
    Parser parser (lexer, intermediateHandler, parserErrorHandler);
    parser.Parse ();
  }

  void Compiler::Program::SetProgramOutputParameters ()
  {
    // Determine output values
    UnicodeString vertexOutput;
    UnicodeString fragmentOutput;
    
    for (size_t i = 0; i < intermediateProg->GetNumFunctions(); i++)
    {
      intermediate::ProgramFunctionPtr func = intermediateProg->GetFunction (i);
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
    
    intermediateProg->SetOutputParameter (vertexOutput, intermediate::Program::Position);
    intermediateProg->SetOutputParameter (fragmentOutput, intermediate::Program::Color);
  }

  void Compiler::Program::SetInputParameterFrequencyFlags (const UnicodeString& param,
							   unsigned int frequencyFlags)
  {
    freqFlagMap[param] = frequencyFlags;
  }
  
  void Compiler::Program::SetEntryFunctionName (const UnicodeString& name)
  {
    intermediateHandler.SetEntryFunction (name);
    intermediateProg = intermediate::ProgramPtr();
  }

  Compiler::Backend::ProgramPtr Compiler::Program::GetCompiledProgram (const Compiler::BackendPtr& backend,
								       Backend::CompileTarget target)
  {
    if (!intermediateProg)
    {
      intermediateProg = intermediateHandler.GetProgram();
      SetProgramOutputParameters ();
      
      optimize::Optimizer opt;
      opt.SetInlineBlocks (compilerOptions->GetOptimizationFlag (Options::optBlockInlining));
      opt.SetDeadCodeElimination (compilerOptions->GetOptimizationFlag (Options::optDeadCodeElimination));
      opt.SetConstantFolding (compilerOptions->GetOptimizationFlag (Options::optConstantFolding));
      intermediateProg = opt.ApplyOptimizations (intermediateProg);
      
      for (unsigned int n = 0; n < splitter::freqNum; n++)
	splitProgs[n] = intermediate::ProgramPtr();
    }

    switch (target)
    {
    case Backend::targetUnsplit:
      return backend->GenerateProgram (target, intermediateProg);
    case Backend::targetVP:
    case Backend::targetFP:
      {
	if (!splitProgs[splitter::freqVertex] || !splitProgs[splitter::freqFragment])
	{
	  splitter::ProgramSplitter splitter;
	  
	  typedef std::pair<UnicodeString, unsigned int> FreqFlagPair;
	  BOOST_FOREACH(FreqFlagPair freqFlag, freqFlagMap)
	  {
	    splitter.SetInputFreqFlags (freqFlag.first, freqFlag.second);
	  }
	  
	  splitter.SetInputProgram (intermediateProg);
	  splitter.PerformSplit();
	  
	  optimize::Optimizer opt;
	  opt.SetInlineBlocks (compilerOptions->GetOptimizationFlag (Options::optBlockInlining));
	  opt.SetDeadCodeElimination (compilerOptions->GetOptimizationFlag (Options::optDeadCodeElimination));
	  opt.SetConstantFolding (compilerOptions->GetOptimizationFlag (Options::optConstantFolding));
	  splitProgs[splitter::freqVertex] = opt.ApplyOptimizations (splitter.GetOutputProgram (splitter::freqVertex));
	  splitProgs[splitter::freqFragment] = opt.ApplyOptimizations (splitter.GetOutputProgram (splitter::freqFragment));
	}
	
	return backend->GenerateProgram (target,
	  target == Backend::targetVP ? splitProgs[splitter::freqVertex] : splitProgs[splitter::freqFragment]);
      }
      break;
    }
    
    assert (false);
    return Backend::ProgramPtr();
  }
} // namespace s1
