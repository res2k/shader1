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

#include "splitter/ProgramSplitter.h"

#include "base/format/Formatter.h"
#include "base/format/uc_String.h"
#include "intermediate/Program.h"
#include "intermediate/ProgramFunction.h"
#include "intermediate/SequenceBuilder.h"
#include "intermediate/SequenceOp/SequenceOpBlock.h"
#include "splitter/SequenceSplitter.h"

#include <boost/make_shared.hpp>

#include "base/format/Formatter.ipp"

namespace s1
{
  namespace splitter
  {
    static std::string FreqFlagsSignature (const std::vector<unsigned int>& inputParamFreqFlags)
    {
      std::string sig;
      for (unsigned int freqFlag : inputParamFreqFlags)
      {
        char ch = 0;
        for (int f = 0; f < freqNum; f++)
        {
          if ((freqFlag & (1 << f)) != 0)
          {
            ch = '0' + f;
            break;
          }
        }
        assert (ch != 0);
        sig.push_back (ch);
      }
      return sig;
    }

    DECLARE_STATIC_FORMATTER(FormatTransferIdent, "tf${0}");
    DECLARE_STATIC_FORMATTER(FormatTransferIdentN, "tf${0}${1}");
    
    ProgramSplitter::SplitFunctionsResult ProgramSplitter::GetSplitFunctions (
      const uc::String& originalIdent, const std::vector<unsigned int>& inputParamFreqFlags)
    {
      SplitFunctionsResult result;

      intermediate::ProgramFunctionPtr progFunc = FindProgramFunction (originalIdent);
      bool isRecursive = CheckFuncRecursive (progFunc);
      std::string freqSig;

      /* Recursion is tricky.
         For now, just force recursive functions to fragment frequency.
         (This will be suboptimal in certain cases, but it is comparatively
         easy to implement ...)
      */
      if (isRecursive)
      {
        std::vector<unsigned int> overrideInputParamFreqFlags (inputParamFreqFlags.size (), freqFlagF);
        freqSig = FreqFlagsSignature (overrideInputParamFreqFlags);
      }
      else
      {
        freqSig = FreqFlagsSignature (inputParamFreqFlags);
      }
      
      uc::String decoratedIdent (originalIdent);
      decoratedIdent.append ("$");
      decoratedIdent.append (freqSig.c_str());
      
      SplitFunctionInfoMap::const_iterator splitFuncInfo = splitFunctions.find (decoratedIdent);
      if (splitFuncInfo != splitFunctions.end())
      {
        SplitFunctionInfoPtr funcInfo (splitFuncInfo->second);
        for (int f = 0; f < freqNum; f++)
          result.idents[f] = funcInfo->funcName[f];
        result.outputParamFreqs = funcInfo->outputParamFreqs;
        for (int f = 0; f < freqNum-1; f++)
          result.transferValues[f] = funcInfo->transferValues[f];
      }
      else
      {
        SplitFunctionInfoPtr newFunc (boost::make_shared<SplitFunctionInfo> ());
        splitFunctions[decoratedIdent] = newFunc;
        
        // FIXME: it's rather desireable to also emit 'uniform' functions
        SequenceSplitter seqSplit (*this, true);
        seqSplit.SetInputSequence (progFunc->GetBody());
        
        if (isRecursive)
        {
          /* Fake output parameter frequencies to 'fragment'
             (so the recursive call will have something) */
          const semantics::FunctionFormalParameters& funcParams = progFunc->GetParams();
          const intermediate::Sequence::RegisterExpMappings& seqExports = progFunc->GetBody()->GetExports();
          for(const semantics::FunctionFormalParameter& funcParam : funcParams)
          {
            if ((int (funcParam.dir) & semantics::FunctionFormalParameter::dirOut) == 0) continue;
            
            #if 0
            intermediate::Sequence::RegisterExpMappings::const_iterator exp = seqExports.find (funcParam.identifier);
            assert (exp != seqExports.end());
            
            intermediate::RegisterPtr seqRegID (exp->second);
            //outputParamFreqs.push_back (seqSplit.GetLocalRegFreqFlags (seqRegID));
            #endif
            result.outputParamFreqs.push_back (freqFlagF);
          }
          newFunc->outputParamFreqs = result.outputParamFreqs;
      
          // Generate 'split' functions
          uc::String funcFName ("fragment_");
          funcFName.append (decoratedIdent);
          result.idents[freqFragment] = funcFName;
          newFunc->funcName[freqFragment] = funcFName;
          
          // Fake all inputs to fragment frequency
          for (size_t i = 0, f = 0; i < inputParamFreqFlags.size (); f++)
          {
            S1_ASSERT(f < funcParams.size (), result);
            if (funcParams[f].dir & semantics::FunctionFormalParameter::dirIn)
            {
              seqSplit.SetInputFreqFlags (funcParams[f].identifier,
                                          (inputParamFreqFlags[i] & freqFlagU) | freqFlagF);
              i++;
            }
          }

          seqSplit.PerformSplit();
          
          // Generate 'split' functions
          semantics::FunctionFormalParameters extraParamsF;
          AddFreqFunction (funcFName, progFunc, extraParamsF, seqSplit.GetOutputFragmentSequence()->GetSequence(), freqFragment);

          result.forceInputPropagation = true;
        }
        else
        {
          // Connect the parameter frequencies to actual inputs
          const semantics::FunctionFormalParameters& funcParams = progFunc->GetParams();
          for (size_t i = 0, f = 0; i < inputParamFreqFlags.size (); f++)
          {
            S1_ASSERT(f < funcParams.size (), result);
            if (funcParams[f].dir & semantics::FunctionFormalParameter::dirIn)
            {
              seqSplit.SetInputFreqFlags (funcParams[f].identifier, inputParamFreqFlags[i]);
              i++;
            }
          }

          seqSplit.PerformSplit();
          
          // Turn values 'transferred' by the function into extra output/input paramerts
          semantics::FunctionFormalParameters extraParams[freqNum];
          const std::vector<intermediate::RegisterPtr>& transfers = seqSplit.GetTransferRegs (freqVertex);
          std::unordered_set<uc::String> seenTransferIdents;
          for (const intermediate::RegisterPtr& reg : transfers)
          {
            // Generate unique parameter identifier
            uc::String transferIdent;
            FormatTransferIdent (transferIdent, reg->GetName());
            unsigned int n = 0;
            while (seenTransferIdents.find (transferIdent) != seenTransferIdents.end ())
            {
              FormatTransferIdentN (transferIdent, reg->GetName (), n++);
            }
            
            // Synthesize new parameters
            {
              semantics::FunctionFormalParameter paramV;
              paramV.paramType = semantics::FunctionFormalParameter::ptAutoTransfer;
              paramV.dir = semantics::FunctionFormalParameter::dirOut;
              paramV.identifier = transferIdent;
              paramV.type = reg->GetOriginalType();
              extraParams[freqVertex].push_back (paramV);
              
              seqSplit.GetOutputVertexSequence()->SetExport (transferIdent, reg);
            }
            {
              semantics::FunctionFormalParameter paramF;
              paramF.paramType = semantics::FunctionFormalParameter::ptAutoTransfer;
              paramF.dir = semantics::FunctionFormalParameter::dirIn;
              paramF.identifier = transferIdent;
              paramF.type = reg->GetOriginalType();
              extraParams[freqFragment].push_back (paramF);
              
              seqSplit.GetOutputFragmentSequence()->SetImport (reg, transferIdent);
            }
            
            // Also return transfer info
            FunctionTransferValues newTransferValue;
            newTransferValue.identifier = reg->GetName ();
            newTransferValue.valueType = reg->GetOriginalType();
            result.transferValues[freqVertex].push_back (newTransferValue);
          }
          newFunc->transferValues[freqVertex] = result.transferValues[freqVertex];
          
          // Extract output parameters, to return output value frequencies
          const intermediate::Sequence::RegisterExpMappings& seqExports = progFunc->GetBody()->GetExports();
          for(const auto& funcParam : funcParams)
          {
            if ((int (funcParam.dir) & semantics::FunctionFormalParameter::dirOut) == 0) continue;
            
            intermediate::Sequence::RegisterExpMappings::const_iterator exp = seqExports.find (funcParam.identifier);
            if (exp == seqExports.end())
            {
              // Output param is never written...
              result.outputParamFreqs.push_back (freqFlagU);
            }
            else
            {
              intermediate::RegisterPtr seqRegID (exp->second);
              result.outputParamFreqs.push_back (seqSplit.GetRegAvailability (seqRegID));
            }
          }
          newFunc->outputParamFreqs = result.outputParamFreqs;
      
          // Generate 'split' functions
          for (int f = 0; f < freqNum; f++)
          {
            intermediate::SequencePtr seq (seqSplit.GetOutputSequence (f)->GetSequence());
            if (!((seq->GetNumOps() > 0) || (extraParams[f].size() > 0)))
              continue;
            
            static const char* const freqPrefix[freqNum] = { "uniform_", "vertex_", "fragment_" };
              
            uc::String funcName (freqPrefix[f]);
            funcName.append (decoratedIdent);
            AddFreqFunction (funcName, progFunc, extraParams[f], seq, f);
            result.idents[f] = funcName;
            newFunc->funcName[f] = funcName;
          }
        }
      }
      return result;
    }
    
    intermediate::ProgramFunctionPtr ProgramSplitter::FindProgramFunction (const uc::String& ident)
    {
      for (size_t i = 0; i < inputProgram->GetNumFunctions(); i++)
      {
        intermediate::ProgramFunctionPtr func = inputProgram->GetFunction (i);
        if (func->GetIdentifier() == ident) return func;
      }
      return intermediate::ProgramFunctionPtr ();
    }
    
    void ProgramSplitter::AddFreqFunction (const uc::String& funcName,
                                           const intermediate::ProgramFunctionPtr& originalFunc,
                                           const semantics::FunctionFormalParameters& extraParams,
                                           const intermediate::SequencePtr& sequence,
                                           int freq)
    {
      semantics::FunctionFormalParameters params (originalFunc->GetParams());
      params.insert (params.end(), extraParams.begin(), extraParams.end());
      
      intermediate::ProgramFunctionPtr newFunc (boost::make_shared<intermediate::ProgramFunction> (originalFunc->GetOriginalIdentifier(),
                                                                                                   funcName,
                                                                                                   params,
                                                                                                   sequence,
                                                                                                   false));
      outputPrograms[freq]->AddFunction (newFunc);
    }
    
    bool ProgramSplitter::CheckFuncRecursive (const intermediate::ProgramFunctionPtr& func)
    {
      return inputProgram->GetFunctionGraph ().IsRecursive (func->GetIdentifier ());
    }
    
    ProgramSplitter::ProgramSplitter ()
    {
      for (int f = 0; f < freqNum; f++)
      {
        outputPrograms[f] = boost::make_shared<intermediate::Program> ();
      }
    }
    
    void ProgramSplitter::SetInputProgram (const intermediate::ProgramPtr& program)
    {
      inputProgram = program;
    }

    void ProgramSplitter::SetInputFreqFlags (const uc::String& inpName, unsigned int flags)
    {
      paramFlags[inpName] = flags;
    }
    
    void ProgramSplitter::PerformSplit ()
    {
      for (int f = 0; f < freqNum; f++)
      {
        outputPrograms[f] = boost::make_shared<intermediate::Program> ();
      }
      const intermediate::Program::OutputParameters& progOutput = inputProgram->GetOutputParameters();
      {
        for (intermediate::Program::OutputParameters::const_iterator outParam = progOutput.begin();
             outParam != progOutput.end();
             ++outParam)
        {
          int f = intermediate::Program::GetTargetFrequency (outParam->second);
          outputPrograms[f]->SetOutputParameter (outParam->first, outParam->second);
        }
      }
      for (int f = 0; f < freqNum; f++)
      {
        outputPrograms[f]->SetParameterArraySizes (inputProgram->GetParameterArraySizes ());
      }
  
      for (size_t i = 0; i < inputProgram->GetNumFunctions(); i++)
      {
        intermediate::ProgramFunctionPtr func = inputProgram->GetFunction (i);
        if (!func->IsEntryFunction()) continue;
        
        splitter::SequenceSplitter splitter (*this);
        splitter.SetInputSequence (func->GetBody());
        
        semantics::FunctionFormalParameters vParams;
        semantics::FunctionFormalParameters fParams;
        typedef std::pair<uc::String, int> ParamFreqPair;
        std::vector<ParamFreqPair> allFrequencies;
        for(const auto& param : func->GetParams())
        {
          if (param.dir & semantics::FunctionFormalParameter::dirIn)
          {
            // Input parameter
            ParamMap::const_iterator paramFlag = paramFlags.find (param.identifier);
            S1_ASSERT (paramFlag != paramFlags.end(), S1_ASSERT_RET_VOID);
            unsigned int paramFreq = paramFlag->second;
            splitter.SetInputFreqFlags (param.identifier, paramFreq);
            if ((paramFreq & (freqFlagU | freqFlagV)) != 0)
              vParams.push_back (param);
            if ((paramFreq & (freqFlagU | freqFlagF)) != 0)
              fParams.push_back (param);
            
            allFrequencies.push_back (std::make_pair (param.identifier, paramFreq));
          }
          else
          {
            // Output parameter
            // Filter all except vertex/fragment out
            intermediate::Program::OutputParameters::const_iterator outParam = progOutput.find (param.identifier);
            if (outParam != progOutput.end())
            {
              auto targetFreq = intermediate::Program::GetTargetFrequency (outParam->second);
              switch (targetFreq)
              {
              case freqVertex:
                vParams.push_back (param);
                break;
              case freqFragment:
                fParams.push_back (param);
                break;
              }
              splitter.SetOutputFreq (param.identifier, static_cast<Frequency> (targetFreq));
            }
          }
        }
        
        splitter.PerformSplit();
        
        for (size_t i = 0; i < outputPrograms[freqUniform]->GetNumFunctions(); i++)
        {
          intermediate::ProgramFunctionPtr func = outputPrograms[freqUniform]->GetFunction (i);
          if (func->IsEntryFunction()) continue;
          outputPrograms[freqVertex]->AddFunction (func);
          /* FIXME: really, should also emit to FP... but that generates invalid Cg with the current
          * (single source) codegen */
        }

        uc::String funcVName ("vertex_");
        //funcVName.append (func->GetIdentifier());
        funcVName.append ("main");
        
        intermediate::ProgramFunctionPtr funcV (
          boost::make_shared<intermediate::ProgramFunction> (func->GetOriginalIdentifier(),
                                                             funcVName,
                                                             vParams,
                                                             splitter.GetOutputVertexSequence()->GetSequence(),
                                                             func->IsEntryFunction()));
        outputPrograms[freqVertex]->AddFunction (funcV);

        uc::String funcFName ("fragment_");
        //funcFName.append (func->GetIdentifier());
        funcFName.append ("main");
        
        intermediate::ProgramFunctionPtr funcF (
          boost::make_shared<intermediate::ProgramFunction> (func->GetOriginalIdentifier(),
                                                             funcFName,
                                                             fParams,
                                                             splitter.GetOutputFragmentSequence()->GetSequence(),
                                                             func->IsEntryFunction()));
        outputPrograms[freqFragment]->AddFunction (funcF);
        
        const std::vector<intermediate::RegisterPtr>& transferV2F = splitter.GetTransferRegs (freqVertex);
        for(const intermediate::RegisterPtr& reg : transferV2F)
        {
          outputPrograms[freqVertex]->AddTransferValue (reg->GetOriginalType(), reg->GetName());
          outputPrograms[freqFragment]->AddTransferValue (reg->GetOriginalType(), reg->GetName());
          funcV->SetTransferMapping (reg->GetName(), reg);
          funcF->SetTransferMapping (reg->GetName(), reg);
        }
        
        for(const ParamFreqPair& pfp : allFrequencies)
        {
          funcV->SetParameterFrequency (pfp.first, pfp.second);
          funcF->SetParameterFrequency (pfp.first, pfp.second);
        }
      }
    }
    
    const intermediate::ProgramPtr& ProgramSplitter::GetOutputProgram (int freq)
    {
      return outputPrograms[freq];
    }
    
  } // namespace splitter
} // namespace s1
