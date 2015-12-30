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

#include "codegen/sl/SLFunctionCodeGenerator.h"

#include "base/format/uc_String.h"
#include "codegen/sl/SLSequenceCodeGenerator.h"
#include "intermediate/ProgramFunction.h"
#include "splitter/Frequency.h"

#include <boost/make_shared.hpp>
#include <boost/optional.hpp>
#include <string>
#include <sstream>

#include "base/format/Formatter.tpp"

namespace s1
{
  namespace codegen
  {
    namespace sl
    {
      FunctionCodeGenerator::FunctionCodeGenerator (const Traits& traits, const Options& options)
        : traits (traits), options (options)
      {
      }

      void FunctionCodeGenerator::ParamAdder::Add (const char* attr, const uc::String& attrStr)
      {
        if (!firstParam)
          paramStr.append (", ");
        else
          firstParam = false;
        paramStr.append (attr);
        paramStr.append (attrStr);
      }

      static format::StaticFormatter FormatDefaultParamStr ("{0} {1}{2}");

      FunctionCodeGenerator::HandleParamResult
        FunctionCodeGenerator::DefaultHandleParameter (const Scope::FunctionFormalParameter& param,
                                                       const size_t* arraySize) const
      {
        HandleParamResult result;

        uc::String typeSuffix;
        uc::String paramStrBase;
        {
          auto typeStrings = traits.TypeString (param.type, arraySize);
          paramStrBase = typeStrings.first;
          typeSuffix = typeStrings.second;
        }

        if (param.dir & parser::SemanticsHandler::Scope::dirIn)
        {
          uc::String paramIdentDecorated (param.paramType == Scope::ptAutoGlobal ? "I" : "i");
          paramIdentDecorated.append (param.identifier);
          uc::String paramIdent = traits.ConvertIdentifier (paramIdentDecorated);
          result.inParamStr = FormatDefaultParamStr.to<uc::String> (paramStrBase, paramIdent, typeSuffix);
          result.inParamIdent = paramIdent;
        }

        if (param.dir & parser::SemanticsHandler::Scope::dirOut)
        {
          uc::String paramIdentDecorated (param.paramType == Scope::ptAutoGlobal ? "O" : "o");
          paramIdentDecorated.append (param.identifier);
          uc::String paramIdent = traits.ConvertIdentifier (paramIdentDecorated);
          result.outParamStr = FormatDefaultParamStr.to<uc::String> (paramStrBase, paramIdent, typeSuffix);
          result.outParamIdent = paramIdent;
        }

        return result;
      }

      static format::StaticFormatter FormatFuncDecl ("void {0} ({1})");

      StringsArrayPtr FunctionCodeGenerator::Generate (const char* identifier,
                                                       const intermediate::ProgramFunctionPtr& func,
                                                       const intermediate::ProgramPtr& prog,
                                                       int frequency)
      {
        typedef parser::SemanticsHandler::Scope Scope;
        typedef Scope::FunctionFormalParameters FunctionFormalParameters;

        const intermediate::Program::TransferValues& transferValues = prog->GetTransferValues ();
        bool doTransfer = !transferValues.empty ();

        BlockNameResolver nameRes;
        StringsArrayPtr resultStrings (boost::make_shared<StringsArray> ());
        {
          ParamAdder paramAdder;

          if (func->IsEntryFunction () && doTransfer)
          {
            if (frequency == splitter::freqVertex)
              paramAdder.Add ("out ", "V2F v2f");
            else
              paramAdder.Add ("in ", "V2F v2f");
          }

          GenerateFunctionParams (func, prog, paramAdder, nameRes.inParamMap, nameRes.outParamMap);

          if (resultStrings->Size () != 0) resultStrings->AddString (uc::String ());
          resultStrings->AddString (FormatFuncDecl.to<uc::String> (identifier, paramAdder.paramStr));
        }
        resultStrings->AddString (std::string ("{"));

        intermediate::ProgramFunction::TransferMappings emptyTransfer;
        const intermediate::ProgramFunction::TransferMappings* transferIn = &emptyTransfer;
        const intermediate::ProgramFunction::TransferMappings* transferOut = &emptyTransfer;

        if (func->IsEntryFunction () && doTransfer)
        {
          if (frequency == splitter::freqVertex)
            transferOut = &(func->GetTransferMappings ());
          else
            transferIn = &(func->GetTransferMappings ());
        }

        std::vector<uc::String> outParamIdents;
        for (const auto& outParam : nameRes.outParamMap)
        {
          outParamIdents.emplace_back (outParam.second);
        }

        auto seqGen = CreateSeqGen (*(func->GetBody ()), &nameRes, *transferIn, *transferOut, outParamIdents);
        resultStrings->AddStrings (*(seqGen->Generate ()), 2);

        resultStrings->AddString (std::string ("}"));
        return resultStrings;
      }
    } // namespace sl
  } // namespace codegen
} // namespace s1
