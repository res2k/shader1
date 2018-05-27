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
#include "codegen/sl/SLTraits.h"
#include "intermediate/forwarddecl.h"
#include "intermediate/Program.h"

#include "SLSequenceCodeGenerator.h"

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

        typedef semantics::Handler::Scope Scope;
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

        // type, identifier, suffix
        struct ParamInfo
        {
          uc::String type;
          uc::String identifier;
          uc::String suffix;
        };
        struct ParamAdder
        {
          bool firstParam;
          uc::String paramStr;

          ParamAdder () : firstParam (true) { }
          void Add (const char* attr, const uc::String& type, const uc::String& identifier,
                    const uc::String& suffix = uc::String ());
          void Add (const char* attr, const ParamInfo& info)
          {
            Add (attr, info.type, info.identifier, info.suffix);
          }
        };

        struct HandleParamResult
        {
          ParamInfo inParam;
          ParamInfo outParam;
        };
        HandleParamResult DefaultHandleParameter (const Scope::FunctionFormalParameter& param,
                                                  const size_t* arraySize,
                                                  bool userInputVerbatim) const;

        virtual void GenerateFunctionParams (const intermediate::ProgramFunctionPtr& func,
                                             const intermediate::ProgramPtr& prog,
                                             int frequency,
                                             ParamAdder& funcParams,
                                             FunctionParamsToIdentifier& inParamMap,
                                             FunctionParamsToIdentifier& outParamMap) = 0;

        virtual std::unique_ptr<SequenceCodeGenerator> CreateSeqGen (const intermediate::Sequence& seq,
                                                                     ImportedNameResolver* nameRes,
                                                                     const intermediate::ProgramFunction::TransferMappings& transferIn,
                                                                     const intermediate::ProgramFunction::TransferMappings& transferOut,
                                                                     const std::vector<uc::String>& outParams) const = 0;
      public:
        FunctionCodeGenerator (const Traits& traits, const Options& options);

        StringsArrayPtr Generate (const intermediate::ProgramFunctionPtr& func,
                                  const intermediate::ProgramPtr& prog,
                                  int frequency);
        StringsArrayPtr Generate (const char* identifier,
                                  const intermediate::ProgramFunctionPtr& func,
                                  const intermediate::ProgramPtr& prog,
                                  int frequency);
      };
    } // namespace sl
  } // namespace codegen
} // namespace s1

#endif // __CODEGEN_SL_FUNCTIONCODEGENERATOR_H__
