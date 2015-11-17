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

        class BlockNameResolver : public ImportedNameResolver
        {
          friend class FunctionCodeGenerator;

          typedef boost::unordered_map<uc::String, std::string> FunctionParamsToIdentifier;
          FunctionParamsToIdentifier inParamMap;
          FunctionParamsToIdentifier outParamMap;
        public:
          std::string GetImportedNameIdentifier (const uc::String& name)
          {
            return inParamMap[name];
          }
          std::string GetExportedNameIdentifier (const uc::String& name)
          {
            return outParamMap[name];
          }
        };

        struct ParamAdder
        {
          bool firstParam;
          std::string paramStr;

          ParamAdder () : firstParam (true) { }
          void Add (const char* attr, const std::string& attrStr);
        };

        virtual std::unique_ptr<SequenceCodeGenerator> CreateSeqGen (const intermediate::Sequence& seq,
                                                                     ImportedNameResolver* nameRes,
                                                                     const intermediate::ProgramFunction::TransferMappings& transferIn,
                                                                     const intermediate::ProgramFunction::TransferMappings& transferOut,
                                                                     const std::vector<std::string>& outParams) const = 0;
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