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
#include "optimize/UnusedFunctionArgumentRemoval.h"

#include "CloningHelper.h"
#include "CommonSequenceVisitor.h"

#include "intermediate/Program.h"
#include "intermediate/ProgramFunction.h"

#include <boost/dynamic_bitset.hpp>
#include <boost/make_shared.hpp>

namespace s1
{
  namespace optimize
  {
    struct UnusedFunctionArgumentRemoval::UsedArgsBits
    {
      /// Bits for parameters (function defn. formal parameter order)
      boost::dynamic_bitset<> all;
      /// Bits for input parameters
      boost::dynamic_bitset<> input;
      /// Bits for output parameters
      boost::dynamic_bitset<> output;
    };

    class UnusedFunctionArgumentRemoval::ArgRemovalVisitor
      : public CommonSequenceVisitor
    {
    public:
      ArgRemovalVisitor (const UnusedFunctionArgumentRemoval& remover,
                         const intermediate::SequenceBuilderPtr& newSequenceBuilder,
                         bool& seqChanged)
        : CommonSequenceVisitor (newSequenceBuilder),
          remover (remover),
          seqChanged (seqChanged)
      {}

      CommonSequenceVisitor* Clone (const intermediate::SequenceBuilderPtr& newSequenceBuilder,
                                    const RegisterMap& regMap) override
      {
        return new ArgRemovalVisitor (remover, newSequenceBuilder, seqChanged);
      }

      void OpFunctionCall (const uc::String& funcIdent,
                           const std::vector<RegisterPtr>& inParams,
                           const std::vector<RegisterPtr>& outParams) override;
    private:
      const UnusedFunctionArgumentRemoval& remover;
      bool& seqChanged;
    };

    void UnusedFunctionArgumentRemoval::ArgRemovalVisitor::OpFunctionCall (const uc::String& funcIdent,
                                                                           const std::vector<RegisterPtr>& inParams,
                                                                           const std::vector<RegisterPtr>& outParams)
    {
      const auto usedArgsIt = remover.funcUsedArgs.find (funcIdent);
      if ((usedArgsIt == remover.funcUsedArgs.end ())
          || (usedArgsIt->second.all.all()))
      {
        // Nothing to do
        CommonSequenceVisitor::OpFunctionCall (funcIdent, inParams, outParams);
        return;
      }

      const auto& funcUsedArgs = usedArgsIt->second;
      std::vector<RegisterPtr> newInParams, newOutParams;
      {
        newInParams.reserve (funcUsedArgs.input.count());
        size_t paramIndex = 0;
        for (const auto& inParam : inParams)
        {
          if (funcUsedArgs.input[paramIndex]) newInParams.push_back (inParam);
          paramIndex++;
        }
      }
      {
        newOutParams.reserve (funcUsedArgs.output.count());
        size_t paramIndex = 0;
        for (const auto& outParam : outParams)
        {
          if (funcUsedArgs.output[paramIndex]) newOutParams.push_back (outParam);
          paramIndex++;
        }
      }
      CommonSequenceVisitor::OpFunctionCall (funcIdent, newInParams, newOutParams);
      seqChanged = true;
    }

    //-----------------------------------------------------------------------

    UnusedFunctionArgumentRemoval::UnusedFunctionArgumentRemoval (intermediate::ProgramPtr inputProg)
    {
      // Collect used input/written output args for each function
      size_t num_f = inputProg->GetNumFunctions ();
      funcUsedArgs.reserve (num_f);
      for (size_t f = 0; f < num_f; f++)
      {
        CollectFunctionArguments (inputProg->GetFunction (f));
      }
    }

    UnusedFunctionArgumentRemoval::~UnusedFunctionArgumentRemoval ()
    {}

    intermediate::ProgramFunctionPtr UnusedFunctionArgumentRemoval::Apply (intermediate::ProgramFunctionPtr func)
    {
      bool paramsChanged = false;
      bool seqChanged = false;

      auto params = func->GetParams ();

      // Trim parameters
      const auto usedArgsIt = funcUsedArgs.find (func->GetIdentifier ());
      if ((usedArgsIt != funcUsedArgs.end ()) && (!usedArgsIt->second.all.all ()))
      {
        const auto& paramsUsedFlags = usedArgsIt->second;

        auto paramIt = params.begin ();
        size_t paramIndex = 0;
        while (paramIt != params.end ())
        {
          if (!paramsUsedFlags.all[paramIndex])
          {
            paramIt = params.erase (paramIt);
          }
          else
          {
            ++paramIt;
          }
          paramIndex++;
        }

        paramsChanged = true;
      }
      // else: nothing would change

      // Change call ops in sequence
      auto bodySeq = func->GetBody ();
      auto newSeq = bodySeq;
      {
        intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<intermediate::SequenceBuilder> ());
        newSeqBuilder->AddImports (bodySeq->GetImports ());
        newSeqBuilder->AddExports (bodySeq->GetExports ());
      
        bool seqChanged = false;
        ArgRemovalVisitor visitor (*this, newSeqBuilder, seqChanged);
        bodySeq->Visit (visitor);
        if (seqChanged) newSeq = newSeqBuilder->GetSequence ();
      }
      seqChanged = newSeq != bodySeq;

      if (!paramsChanged && !seqChanged)
      {
        // Nothing changed
        return func;
      }

      intermediate::ProgramFunctionPtr newFunc (
        boost::make_shared<intermediate::ProgramFunction> (func->GetOriginalIdentifier (),
                                                           func->GetIdentifier (),
                                                           params,
                                                           newSeq,
                                                           func->IsEntryFunction ()));

      newFunc->AddTransferMappings (func->GetTransferMappings ());
      newFunc->AddParameterFrequencies (func->GetParameterFrequencies ());

      return newFunc;
    }

    intermediate::ProgramPtr UnusedFunctionArgumentRemoval::RemoveUnusedFunctionArguments (intermediate::ProgramPtr inputProg)
    {
      UnusedFunctionArgumentRemoval remover (inputProg);
      auto newProgram = CloningHelper::CloneProgramMeta (inputProg);
      bool applied = false;

      for (size_t f = 0; f < inputProg->GetNumFunctions (); f++)
      {
        auto func (inputProg->GetFunction (f));
        auto newFunc = remover.Apply (func);
        applied |= newFunc != func;

        newFunc->AddTransferMappings (func->GetTransferMappings ());
        newFunc->AddParameterFrequencies (func->GetParameterFrequencies ());

        newProgram->AddFunction (newFunc);
      }
      return applied ? newProgram : inputProg;
    }

    void UnusedFunctionArgumentRemoval::CollectFunctionArguments (intermediate::ProgramFunctionPtr func)
    {
      UsedArgsBits paramUsage;
      const auto& params = func->GetParams ();
      const auto bodySeq = func->GetBody ();

      auto bodyRead = bodySeq->GetAllReadRegisters ();
      const boost::unordered_map<uc::String, intermediate::RegisterPtr> inputToRegs (
        bodySeq->GetImports ().begin (), bodySeq->GetImports ().end ());

      auto bodyWritten = bodySeq->GetAllWrittenRegisters ();
      const auto outputToRegs (bodySeq->GetExports ());

      /* Parameters that are used for transfers only need also to be considered used!
       * For that we consider exported regs as read and imported regs as written,
       * the transfer regs match that pattern */
      bodyRead.reserve (bodyRead.size () + outputToRegs.size ());
      for (const auto& exp : outputToRegs)
      {
        bodyRead.insert (exp.second);
      }
      bodyWritten.reserve (bodyWritten.size () + inputToRegs.size ());
      for (const auto& imp : inputToRegs)
      {
        bodyWritten.insert (imp.second);
      }

      paramUsage.all.resize (params.size ());
#if BOOST_VERSION >= 106200
      paramUsage.input.reserve (params.size () / 2);
      paramUsage.output.reserve (params.size () / 2);
#endif // BOOST_VERSION
      size_t paramIndex = 0;
      for (const auto& param : params)
      {
        bool paramUsed = false;
        if ((param.dir & parser::SemanticsHandler::Scope::dirIn) != 0)
        {
          bool used = false;
          auto inputIt = inputToRegs.find (param.identifier);
          if (inputIt != inputToRegs.end ())
          {
            used = bodyRead.find (inputIt->second) != bodyRead.end ();
          }
          paramUsage.input.push_back (used);
          paramUsed |= used;
        }
        if ((param.dir & parser::SemanticsHandler::Scope::dirOut) != 0)
        {
          bool used = false;
          auto outputIt = outputToRegs.find (param.identifier);
          if (outputIt != outputToRegs.end ())
          {
            used = bodyWritten.find (outputIt->second) != bodyWritten.end ();
          }
          paramUsage.output.push_back (used);
          paramUsed |= used;
        }
        paramUsage.all.set (paramIndex++, paramUsed);
      }

      funcUsedArgs[func->GetIdentifier ()] = std::move (paramUsage);
    }
  } // namespace optimize
} // namespace s1
