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

#ifndef __CODEGEN_SL_FUNCTIONCODEGENERATOR_H__
#define __CODEGEN_SL_FUNCTIONCODEGENERATOR_H__

#include "codegen/common/StringsArray.h"
#include "codegen/sl/Traits.h"
#include "intermediate/forwarddecl.h"
#include "intermediate/Program.h"

#include "SequenceCodeGenerator.h"

namespace s1
{
  namespace codegen
  {
    namespace sl
    {
      class Options;

      class FunctionCodeGenerator
      {
      protected:
        const Traits& traits;
        const Options& options;

        typedef parser::SemanticsHandler::Scope Scope;
        typedef Scope::FunctionFormalParameters FunctionFormalParameters;

        typedef boost::unordered_map<uc::String, uc::String> FunctionParamsToIdentifier;
        class BlockNameResolver : public ImportedNameResolver
        {
          friend class FunctionCodeGenerator;

          FunctionParamsToIdentifier inParamMap;
          FunctionParamsToIdentifier outParamMap;
        public:
          uc::String GetImportedNameIdentifier (const uc::String& name) override
          {
            return inParamMap[name];
          }
          uc::String GetExportedNameIdentifier (const uc::String& name) override
          {
            return outParamMap[name];
          }
        };

        struct ParamAdder
        {
          bool firstParam;
          uc::String paramStr;

          ParamAdder () : firstParam (true) { }
          void Add (const char* attr, const uc::String& attrStr);
        };

        struct HandleParamResult
        {
          uc::String inParamStr;
          uc::String inParamIdent;
          uc::String outParamStr;
          uc::String outParamIdent;
        };
        HandleParamResult DefaultHandleParameter (const Scope::FunctionFormalParameter& param,
                                                  const size_t* arraySize) const;

        virtual void GenerateFunctionParams (const intermediate::ProgramFunctionPtr& func,
                                             const intermediate::Program::OutputParameters& output,
                                             const intermediate::Program::ParameterArraySizes& paramArraySizes,
                                             ParamAdder& funcParams,
                                             const StringsArrayPtr& preamble,
                                             FunctionParamsToIdentifier& inParamMap,
                                             FunctionParamsToIdentifier& outParamMap) const = 0;

        virtual std::unique_ptr<SequenceCodeGenerator> CreateSeqGen (const intermediate::Sequence& seq,
                                                                     ImportedNameResolver* nameRes,
                                                                     const intermediate::ProgramFunction::TransferMappings& transferIn,
                                                                     const intermediate::ProgramFunction::TransferMappings& transferOut,
                                                                     const std::vector<uc::String>& outParams) const = 0;
      public:
        FunctionCodeGenerator (const Traits& traits, const Options& options);

        StringsArrayPtr Generate (const char* identifier,
                                  const intermediate::ProgramFunctionPtr& func,
                                  const intermediate::Program::OutputParameters& output,
                                  const intermediate::Program::ParameterArraySizes& paramArraySizes,
                                  bool doTransfer,
                                  int frequency);
      };
    } // namespace sl
  } // namespace codegen
} // namespace s1

#endif // __CODEGEN_SL_FUNCTIONCODEGENERATOR_H__
