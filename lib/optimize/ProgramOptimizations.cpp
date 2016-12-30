/*
    Shader1
    Copyright (c) 2016 Frank Richter


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
#include "ProgramOptimizations.h"

#include "CloningHelper.h"
#include "SequenceOptimizations.h"

#include "intermediate/Program.h"
#include "intermediate/ProgramFunction.h"
#include "optimize/Optimizer.h"
#include "optimize/UnusedFunctionArgumentRemoval.h"

#include <boost/make_shared.hpp>
#include <boost/optional.hpp>
#include <boost/utility/in_place_factory.hpp>

namespace s1
{
  namespace optimize
  {
    intermediate::ProgramPtr ProgramOptimizations::Apply (unsigned int optimizations,
                                                          intermediate::ProgramPtr inputProg)
    {
      auto program = inputProg;
      bool optimizeSequences = true;
      while (optimizeSequences)
      {
        // Whole program optimizers
        boost::optional<UnusedFunctionArgumentRemoval> ufar;
        if (optimizations & Optimizer::optUnusedArgumentRemoval)
          ufar = boost::in_place (program);
        bool programOptApplied = false;

        auto newProgram = CloningHelper::CloneProgramMeta (program);

        for (size_t f = 0; f < program->GetNumFunctions (); f++)
        {
          intermediate::ProgramFunctionPtr func (program->GetFunction (f));

          intermediate::RegisterSet usedRegs;
          const intermediate::Sequence::RegisterExpMappings seqExports (func->GetBody ()->GetExports ());
          // Collect function outputs, mark them as 'used' for DCE
          for (const parser::SemanticsHandler::Scope::FunctionFormalParameter& param : func->GetParams ())
          {
            if ((param.dir & parser::SemanticsHandler::Scope::dirOut) != 0)
            {
              const intermediate::Sequence::RegisterExpMappings::const_iterator exp = seqExports.find (param.identifier);
              if (exp == seqExports.end ()) continue;
              usedRegs.insert (exp->second);
            }
          }
          // Also, all transferred regs must be considered 'used'
          for (const intermediate::ProgramFunction::TransferMappingPair& tmp : func->GetTransferMappings ())
          {
            usedRegs.insert (tmp.second);
          }

          intermediate::SequencePtr newBody;
          SequenceOptimizations::Apply (optimizations, newBody, func->GetBody (), usedRegs);

          intermediate::ProgramFunctionPtr newFunc (
            boost::make_shared<intermediate::ProgramFunction> (func->GetOriginalIdentifier (),
                                                               func->GetIdentifier (),
                                                               func->GetParams (),
                                                               newBody,
                                                               func->IsEntryFunction ()));

          newFunc->AddTransferMappings (func->GetTransferMappings ());
          newFunc->AddParameterFrequencies (func->GetParameterFrequencies ());

          // Apply whole program optimizations
          if (ufar)
          {
            auto optFunc = ufar->Apply (newFunc);
            programOptApplied |= optFunc != newFunc;
            newFunc = optFunc;
          }

          newProgram->AddFunction (newFunc);
        }

        program = newProgram;
        optimizeSequences = programOptApplied;
      }
      return program;
    }
  } // namespace optimize
} // namespace s1
