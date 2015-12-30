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

#include "FunctionCodeGenerator.h"

#include "CgTraits.h"
#include "SequenceCodeGenerator.h"

#include "codegen/cg/CgOptions.h"
#include "splitter/Frequency.h"

namespace s1
{
  namespace codegen
  {
    CgGenerator::FunctionCodeGenerator::FunctionCodeGenerator (const CgOptions& options)
      : sl::FunctionCodeGenerator (CgTraits::instance, options)
    {
    }

    const CgOptions& CgGenerator::FunctionCodeGenerator::GetCgOptions () const
    {
      return static_cast<const CgOptions&> (options);
    }

    void CgGenerator::FunctionCodeGenerator::GenerateFunctionParams (const intermediate::ProgramFunctionPtr& func,
                                                                     const intermediate::ProgramPtr& prog,
                                                                     ParamAdder& funcParams,
                                                                     FunctionParamsToIdentifier& inParamMap,
                                                                     FunctionParamsToIdentifier& outParamMap) const
    {
      const intermediate::Program::OutputParameters& output = prog->GetOutputParameters ();
      const intermediate::Program::ParameterArraySizes& paramArraySizes = prog->GetParameterArraySizes ();
      const intermediate::ProgramFunction::ParameterFrequencyMap& paramFreqs (func->GetParameterFrequencies ());

      boost::unordered_set<uc::String> paramImports;
      std::vector<std::pair<uc::String, uc::String> > inParams;
      std::vector<uc::String> outParams;
      const FunctionFormalParameters& params (func->GetParams ());
      for (FunctionFormalParameters::const_iterator param = params.begin ();
      param != params.end ();
        ++param)
      {
        paramImports.insert (param->identifier);

        boost::optional<size_t> arraySize;
        {
          auto paramSize (paramArraySizes.find (param->identifier));
          if (paramSize != paramArraySizes.end ())
          {
            arraySize = paramSize->second;
          }
        }

        HandleParamResult handleRes = DefaultHandleParameter (*param, arraySize ? &(*arraySize) : nullptr);

        if (!handleRes.inParamStr.isEmpty())
        {
          inParams.emplace_back (handleRes.inParamStr, param->identifier);
          inParamMap[param->identifier] = handleRes.inParamIdent;
        }

        if (!handleRes.outParamStr.isEmpty())
        {
          uc::String paramStr = handleRes.outParamStr;
          intermediate::Program::OutputParameters::const_iterator outputInfo = output.find (param->identifier);
          if (outputInfo != output.end ())
          {
            switch (outputInfo->second)
            {
            case intermediate::Program::Position:
              paramStr.append (" : POSITION");
              break;
            case intermediate::Program::Color:
              paramStr.append (" : COLOR");
              break;
            }
          }
          outParams.push_back (paramStr);

          outParamMap[param->identifier] = handleRes.outParamIdent;
        }
      }

      for (const auto& inParam : inParams)
      {
        const char* variability = "in ";
        intermediate::ProgramFunction::ParameterFrequencyMap::const_iterator pf = paramFreqs.find (inParam.second);
        if (pf != paramFreqs.end ())
        {
          if (pf->second & splitter::freqFlagU)
            variability = "uniform in ";
          else if (pf->second & (splitter::freqFlagV | splitter::freqFlagF))
            variability = "varying in ";
        }

        funcParams.Add (variability, inParam.first);
      }
      for (const auto& outParam : outParams)
      {
        funcParams.Add ("out ", outParam);
      }
    }

    std::unique_ptr<sl::SequenceCodeGenerator>
      CgGenerator::FunctionCodeGenerator::CreateSeqGen (const intermediate::Sequence& seq,
                                                        sl::ImportedNameResolver* nameRes,
                                                        const intermediate::ProgramFunction::TransferMappings& transferIn,
                                                        const intermediate::ProgramFunction::TransferMappings& transferOut,
                                                        const std::vector<uc::String>& outParams) const
    {
      std::unique_ptr<SequenceCodeGenerator> p;
      p.reset (new SequenceCodeGenerator (seq, nameRes, transferIn, transferOut, outParams, GetCgOptions()));
      return std::move (p);
    }
  } // namespace codegen
} // namespace s1

