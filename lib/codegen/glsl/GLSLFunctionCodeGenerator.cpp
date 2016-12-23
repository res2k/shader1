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

#include "GLSLFunctionCodeGenerator.h"

#include "GLSLTraits.h"
#include "GLSLSequenceCodeGenerator.h"

#include "codegen/glsl/GLSLOptions.h"
#include "splitter/Frequency.h"

namespace s1
{
  namespace codegen
  {
    namespace glsl
    {
      FunctionCodeGenerator::FunctionCodeGenerator (const sl::Traits& traits, const Options& options)
        : sl::FunctionCodeGenerator (traits, options)
      {
      }

      const Options& FunctionCodeGenerator::GetOptions () const
      {
        return static_cast<const Options&> (options);
      }

      FunctionCodeGenerator::HandleParamResult
        FunctionCodeGenerator::HandleMainParameter (const intermediate::Program::OutputParameters& output,
          const Scope::FunctionFormalParameter& param,
          const size_t* arraySize) const
      {
        HandleParamResult result;

        uc::String typeSuffix;
        uc::String paramStrBase;
        {
          auto typeStrings = traits.TypeString(param.type, arraySize);
          paramStrBase = typeStrings.first;
          typeSuffix = typeStrings.second;
        }

        if (param.dir & parser::SemanticsHandler::Scope::dirIn)
        {
          // TODO: Emit global vars
          uc::String paramIdent;
          if (param.paramType >= Scope::ptAutoGlobal)
          {
            uc::String paramIdentDecorated(param.paramType == Scope::ptAutoGlobal ? "I" : "i");
            paramIdentDecorated.append(param.identifier);
            paramIdent = traits.ConvertIdentifier(paramIdentDecorated);
          }
          else
          {
            paramIdent = traits.ConvertIdentifier(param.identifier, true);
          }
          result.inParam = ParamInfo{ paramStrBase, paramIdent, typeSuffix };
        }

        if (param.dir & parser::SemanticsHandler::Scope::dirOut)
        {
          uc::String paramIdent;
          if (param.paramType >= Scope::ptAutoGlobal)
          {
            uc::String paramIdentDecorated(param.paramType == Scope::ptAutoGlobal ? "O" : "o");
            paramIdentDecorated.append(param.identifier);
            paramIdent = traits.ConvertIdentifier(paramIdentDecorated);
          }
          else
          {
            paramIdent = traits.ConvertIdentifier(param.identifier, true);
          }
          assert(!paramIdent.isEmpty());
          {
            intermediate::Program::OutputParameters::const_iterator outputInfo = output.find(param.identifier);
            if (outputInfo != output.end())
            {
              switch (outputInfo->second)
              {
              case intermediate::Program::Position:
                paramIdent = "gl_Position";
                break;
              case intermediate::Program::Color:
                paramIdent = "gl_FragColor";
                break;
              }
            }
          }
          result.outParam = ParamInfo{ paramStrBase, paramIdent, typeSuffix };
        }

        return result;
      }

      void FunctionCodeGenerator::GenerateFunctionParams (const intermediate::ProgramFunctionPtr& func,
                                                          const intermediate::ProgramPtr& prog,
                                                          int frequency,
                                                          ParamAdder& funcParams,
                                                          FunctionParamsToIdentifier& inParamMap,
                                                          FunctionParamsToIdentifier& outParamMap)
      {
        const intermediate::Program::OutputParameters& output = prog->GetOutputParameters ();
        const intermediate::Program::ParameterArraySizes& paramArraySizes = prog->GetParameterArraySizes ();
        const intermediate::ProgramFunction::ParameterFrequencyMap& paramFreqs (func->GetParameterFrequencies ());

        boost::unordered_set<uc::String> paramImports;
        std::vector<std::pair<ParamInfo, uc::String> > inParams;
        std::vector<ParamInfo> outParams;
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

          if (!func->IsEntryFunction())
          {
            HandleParamResult handleRes = DefaultHandleParameter(*param, arraySize ? &(*arraySize) : nullptr,
              false);

            if (!handleRes.inParam.identifier.isEmpty())
            {
              inParams.emplace_back(handleRes.inParam, param->identifier);
              inParamMap[param->identifier] = handleRes.inParam.identifier;
            }

            if (!handleRes.outParam.identifier.isEmpty())
            {
              outParams.push_back(handleRes.outParam);
              outParamMap[param->identifier] = handleRes.outParam.identifier;
            }
          }
          else
          {
            HandleParamResult handleRes = HandleMainParameter(output, *param, arraySize ? &(*arraySize) : nullptr);

            if (!handleRes.inParam.identifier.isEmpty())
            {
              inParamMap[param->identifier] = handleRes.inParam.identifier;
            }

            if (!handleRes.outParam.identifier.isEmpty())
            {
              outParamMap[param->identifier] = handleRes.outParam.identifier;
            }
          }
        }

        for (const auto& inParam : inParams)
        {
          const char* variability = "const in";
          funcParams.Add (variability, inParam.first);
        }
        for (const auto& outParam : outParams)
        {
          funcParams.Add ("out", outParam);
        }
      }

      std::unique_ptr<sl::SequenceCodeGenerator>
        FunctionCodeGenerator::CreateSeqGen (const intermediate::Sequence& seq,
                                             sl::ImportedNameResolver* nameRes,
                                             const intermediate::ProgramFunction::TransferMappings& transferIn,
                                             const intermediate::ProgramFunction::TransferMappings& transferOut,
                                             const std::vector<uc::String>& outParams) const
      {
        std::unique_ptr<SequenceCodeGenerator> p;
        p.reset (new SequenceCodeGenerator (seq, nameRes, transferIn, transferOut, outParams, traits, GetOptions ()));
        return std::move (p);
      }
    } // namespace glsl
  } // namespace codegen
} // namespace s1

