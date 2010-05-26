#ifndef __CODEGEN_FUNCTIONCODEGENERATOR_H__
#define __CODEGEN_FUNCTIONCODEGENERATOR_H__

#include "base/hash_UnicodeString.h"

#include "codegen/CgGenerator.h"
#include "codegen/StringsArray.h"
#include "intermediate/forwarddecl.h"
#include "SequenceCodeGenerator.h"

namespace s1
{
  namespace codegen
  {
    class CgGenerator::FunctionCodeGenerator
    {
      class BlockNameResolver : public ImportedNameResolver
      {
	friend class FunctionCodeGenerator;
	
	typedef std::tr1::unordered_map<UnicodeString, std::string> FunctionParamsToIdentifier;
	FunctionParamsToIdentifier inParamMap;
	FunctionParamsToIdentifier outParamMap;
      public:
	std::string GetImportedNameIdentifier (const UnicodeString& name)
	{ 
	  return inParamMap[name];
	}
	std::string GetExportedNameIdentifier (const UnicodeString& name)
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
      StringsArrayPtr Generate (const intermediate::ProgramFunctionPtr& func);
    };
  } // namespace codegen
} // namespace s1

#endif // __CODEGEN_FUNCTIONCODEGENERATOR_H__
