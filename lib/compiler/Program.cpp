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
#include "base/ResultCode_internal.h"

#include "compiler/BackendProgram.h"
#include "compiler/Options.h"
#include "compiler/Program.h"
#include "compiler/ProgramDiagnostics.h"

#include "base/DebugMessageHandler.h"
#include "base/uc/Stream.h"
#include "intermediate/Program.h"
#include "intermediate/ProgramFunction.h"
#include "lexer/Lexer.h"
#include "optimize/Optimizer.h"
#include "parser/Parser.h"
#include "splitter/ProgramSplitter.h"

namespace s1
{
  Compiler::Program::Program (Library* lib, uc::Stream* inputStream)
    : diagnostics (new ProgramDiagnostics (lib))
  {
    intermediateHandler.SetDiagnosticsHandler (diagnostics.get());
    Lexer lexer (*inputStream, *diagnostics);
    
    Parser parser (lexer, intermediateHandler, *diagnostics);
    parser.Parse ();
    intermediateHandler.CompleteProgram();
  }

  Compiler::Program::~Program ()
  {}

  void Compiler::Program::SetProgramOutputParameters ()
  {
    // Determine output values
    uc::String vertexOutput;
    uc::String fragmentOutput;
    
    for (size_t i = 0; i < intermediateProg->GetNumFunctions(); i++)
    {
      intermediate::ProgramFunctionPtr func = intermediateProg->GetFunction (i);
      if (!func->IsEntryFunction()) continue;
      
      const semantics::Handler::Scope::FunctionFormalParameters& funcParams = func->GetParams();
      for(const semantics::Handler::Scope::FunctionFormalParameter& param : funcParams)
      {
        if (!(param.dir & semantics::Handler::Scope::dirOut)) continue;
        
        // Look for float4 output
        if (param.type->GetTypeClass() != semantics::Handler::Type::Vector) continue;
        if (param.type->GetArrayVectorMatrixBaseType()->GetBaseType() != semantics::Handler::Float) continue;
        if (param.type->GetVectorTypeComponents() != 4) continue;
        
        // Parameter qualifies
        if (vertexOutput.isEmpty())
          vertexOutput = param.identifier;
        else if (fragmentOutput.isEmpty())
          fragmentOutput = param.identifier;
        else
        {
          // TODO: Make 'proper' warning
          s1::PrintDebugMessage ("Entry function has too many 'float4' outputs");
        }
      }
    }
    
    if (vertexOutput.isEmpty() || fragmentOutput.isEmpty())
    {
      // TODO: Make 'proper' warning
      s1::PrintDebugMessage ("Entry function has not enough 'float4' outputs");
    }
    
    intermediateProg->SetOutputParameter (vertexOutput, intermediate::Program::Position);
    intermediateProg->SetOutputParameter (fragmentOutput, intermediate::Program::Color);
  }

  static optimize::Optimizer CreateOptimizer (Compiler::OptionsPtr compilerOptions)
  {
    optimize::Optimizer opt;
    opt.SetInlineBlocks (compilerOptions->GetOptimizationFlag (Compiler::Options::optBlockInlining));
    opt.SetDeadCodeElimination (compilerOptions->GetOptimizationFlag (Compiler::Options::optDeadCodeElimination));
    opt.SetConstantFolding (compilerOptions->GetOptimizationFlag (Compiler::Options::optConstantFolding));
    opt.SetUnusedArgumentRemoval (compilerOptions->GetOptimizationFlag (Compiler::Options::optUnusedArgumentRemoval));
    return opt;
  }

  static semantics::Handler::TypePtr GetBaseType (const semantics::Handler::TypePtr& type)
  {
    if (type->GetTypeClass () == semantics::Handler::Type::Array)
      return GetBaseType (type->GetArrayVectorMatrixBaseType ());
    else
      return type;
  }

  static splitter::Frequency GetDefaultFreq (const semantics::Handler::Scope::FunctionFormalParameter param)
  {
    switch (param.freqQualifier)
    {
    case semantics::Handler::Scope::freqAuto:
      // Auto: assume 'uniform', so simply fall through
    case semantics::Handler::Scope::freqUniform:
      {
        if (GetBaseType (param.type)->GetTypeClass() == semantics::Handler::Type::Sampler)
          // Logically, samples are uniform. Internally, they're fragment (at least as long vertex textures aren't supported).
          return splitter::freqFragment;
        else
          return splitter::freqUniform;
      }
    case semantics::Handler::Scope::freqAttribute:
      {
        if (GetBaseType (param.type)->GetTypeClass() == semantics::Handler::Type::Sampler)
        {
          // TODO: We should probably emit a warning here...
          return splitter::freqFragment;
        }
        else
        {
          return splitter::freqVertex;
        }
      }
    }

    S1_ASSERT_NOT_REACHED (splitter::freqUniform);
  }

  Compiler::ProgramDiagnostics* Compiler::Program::GetDiagnostics() const
  {
    return diagnostics.get();
  }

  Compiler::Backend::ProgramPtr Compiler::Program::GetCompiledProgram (const uc::String& entryFunction,
                                                                       const OptionsPtr& compilerOptions,
                                                                       const FreqFlagMap& inputParamFreqs,
                                                                       const ArraySizeMap& arraySizes,
                                                                       const Compiler::BackendPtr& backend,
                                                                       Backend::CompileTarget target,
                                                                       Backend::OptionsPtr options)
  {
    if (!intermediateProg || (entryFunction != lastEntryFunction))
    {
      intermediateProg = intermediateHandler.GetProgram (entryFunction);
      if (!intermediateProg) return Backend::ProgramPtr ();
      lastEntryFunction = entryFunction;
      SetProgramOutputParameters ();
      
      for (unsigned int n = 0; n < splitter::freqNum; n++)
        splitProgs[n] = intermediate::ProgramPtr();
    }

    intermediateProg->SetParameterArraySizes (arraySizes);

    optimize::Optimizer opt = CreateOptimizer (compilerOptions);
    intermediate::ProgramPtr optProg = opt.ApplyOptimizations (intermediateProg);

    FreqFlagMap defaultInputParamFreqs;
    // Fill map with frequencies from input parameter qualifiers
    {
      auto entryFunc = intermediateProg->GetEntryFunction ();
      S1_ASSERT (entryFunc, Backend::ProgramPtr ());
      for (const auto& entryParam : entryFunc->GetParams ())
      {
        if ((entryParam.dir & semantics::Handler::Scope::dirOut) != 0) continue;
        defaultInputParamFreqs[entryParam.identifier] = 1 << GetDefaultFreq (entryParam);
      }
    }

    switch (target)
    {
    case Backend::targetVP:
    case Backend::targetFP:
      {
        if (!splitProgs[splitter::freqVertex] || !splitProgs[splitter::freqFragment])
        {
          splitter::ProgramSplitter splitter;
          
          for(auto freqFlag : defaultInputParamFreqs)
          {
            splitter.SetInputFreqFlags (freqFlag.first, freqFlag.second);
          }
          for(auto freqFlag : inputParamFreqs)
          {
            splitter.SetInputFreqFlags (freqFlag.first, freqFlag.second);
          }
          
          splitter.SetInputProgram (optProg);
          splitter.PerformSplit();
          
          optimize::Optimizer opt = CreateOptimizer (compilerOptions);
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
    
    S1_ASSERT_NOT_REACHED (Backend::ProgramPtr());
  }
} // namespace s1
