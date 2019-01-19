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

#include "GLSLProgramCodeGenerator.h"

#include "GLSLTraits.h"
#include "GLSLFunctionCodeGenerator.h"

#include "base/format/Formatter.h"
#include "base/format/uc_String.h"
#include "codegen/glsl/GLSLOptions.h"
#include "splitter/Frequency.h"

#include "boost/make_shared.hpp"

#include "base/format/Formatter.ipp"

namespace s1
{
  namespace codegen
  {
    namespace glsl
    {
      ProgramCodeGenerator::ProgramCodeGenerator (const Traits& traits, const Options& options)
        : sl::ProgramCodeGenerator (traits, options)
      {
      }

      const Options& ProgramCodeGenerator::GetOptions () const
      {
        return static_cast<const Options&> (options);
      }
      
      DECLARE_STATIC_FORMATTER(FormatTransferVal, "varying {0} v2f_{1}{2};");

      StringsArrayPtr ProgramCodeGenerator::GeneratePreamble (const intermediate::ProgramPtr& prog, int frequency)
      {
        StringsArrayPtr resultStrings = boost::make_shared<StringsArray> ();

        // Obtain entry function
        for (size_t f = 0, f_num = prog->GetNumFunctions (); f < f_num; f++)
        {
          auto func = prog->GetFunction (f);
          if (func->IsEntryFunction ())
          {
            GenerateEntryFunctionInputs (*resultStrings, frequency, prog, func);
            break;
          }
        }

        // Generate globals for values passed from VP to FP
        const auto& transfers = prog->GetTransferValues ();
        for (const auto& transferVal : transfers)
        {
          uc::String typeSuffix;
          uc::String paramStrBase;
          {
            auto typeStrings = traits.TypeString (transferVal.first, nullptr);
            paramStrBase = typeStrings.first;
            typeSuffix = typeStrings.second;
          }
          uc::String transferIdent = traits.ConvertIdentifier (transferVal.second);
          resultStrings->AddString (FormatTransferVal.to<uc::String> (paramStrBase, transferIdent, typeSuffix));
        }
        return resultStrings;
      }

      DECLARE_STATIC_FORMATTER(FormatInputVarU, "uniform {0} {1}{2};");
      DECLARE_STATIC_FORMATTER(FormatInputVarV, "attribute {0} {1}{2};");

      void ProgramCodeGenerator::GenerateEntryFunctionInputs (StringsArray& strings,
                                                              int progFreq,
                                                              const intermediate::ProgramPtr& prog,
                                                              intermediate::ProgramFunctionPtr& func)
      {
        const auto& paramArraySizes = prog->GetParameterArraySizes ();
        const auto& params = func->GetParams ();
        const auto& paramFreqs = func->GetParameterFrequencies ();
        for (const auto& param : params)
        {
          if (param.dir & semantics::Scope::dirIn)
          {
            // TODO: Emit global vars
            if (param.paramType < semantics::Scope::ptAutoGlobal)
            {
              boost::optional<size_t> arraySize;
              {
                auto paramSize (paramArraySizes.find (param.identifier));
                if (paramSize != paramArraySizes.end ())
                {
                  arraySize = paramSize->second;
                }
              }
              uc::String typeSuffix;
              uc::String paramStrBase;
              {
                auto typeStrings = traits.TypeString (param.type, arraySize ? &(*arraySize) : nullptr);
                paramStrBase = typeStrings.first;
                typeSuffix = typeStrings.second;
              }
              uc::String paramIdent = traits.ConvertIdentifier (param.identifier, true);
              auto paramFreqIt = paramFreqs.find (param.identifier);
              int freqFlags = paramFreqIt != paramFreqs.end () ? paramFreqIt->second : splitter::freqFlagU;
              if ((freqFlags & splitter::freqFlagU) != 0)
              {
                strings.AddString (FormatInputVarU.to<uc::String> (paramStrBase, paramIdent, typeSuffix));
              }
              else if ((progFreq == splitter::freqVertex) && ((freqFlags & splitter::freqFlagV) != 0))
              {
                strings.AddString (FormatInputVarV.to<uc::String> (paramStrBase, paramIdent, typeSuffix));
              }
              else if ((progFreq == splitter::freqFragment) && ((freqFlags & splitter::freqFlagF) != 0))
              {
                strings.AddString (FormatInputVarU.to<uc::String> (paramStrBase, paramIdent, typeSuffix));
              }
            }
            //result.inParam = ParamInfo{ paramStrBase, paramIdent, typeSuffix };
          }
        }
      }

      std::unique_ptr<sl::FunctionCodeGenerator> ProgramCodeGenerator::CreateFunctionCodeGenerator ()
      {
        return std::unique_ptr<sl::FunctionCodeGenerator> (
          new FunctionCodeGenerator (traits, GetOptions ()));
      }
    } // namespace glsl
  } // namespace codegen
} // namespace s1

