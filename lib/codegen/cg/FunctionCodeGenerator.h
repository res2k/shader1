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

#ifndef __CODEGEN_FUNCTIONCODEGENERATOR_H__
#define __CODEGEN_FUNCTIONCODEGENERATOR_H__

#include "codegen/cg/CgGenerator.h"
#include "codegen/common/StringsArray.h"
#include "intermediate/forwarddecl.h"
#include "SequenceCodeGenerator.h"

namespace s1
{
  namespace codegen
  {
    class CgOptions;

    class CgGenerator::FunctionCodeGenerator
    {
      const CgOptions& options;

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
    public:
      FunctionCodeGenerator (const CgOptions& options);

      StringsArrayPtr Generate (const char* identifier,
                                const intermediate::ProgramFunctionPtr& func,
				const intermediate::Program::OutputParameters& output,
				const intermediate::Program::ParameterArraySizes& paramArraySizes,
				bool doTransfer,
				int frequency);
    };
  } // namespace codegen
} // namespace s1

#endif // __CODEGEN_FUNCTIONCODEGENERATOR_H__
