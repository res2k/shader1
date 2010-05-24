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
	FunctionParamsToIdentifier paramMap;
      public:
	std::string GetImportedNameIdentifier (const UnicodeString& name)
	{ 
	  return paramMap[name];
	}
	std::string GetExportedNameIdentifier (const UnicodeString& name)
	{ 
	  return paramMap[name];
	}
      };
    public:
      StringsArrayPtr Generate (const intermediate::ProgramFunctionPtr& func);
    };
  } // namespace codegen
} // namespace s1

#endif // __CODEGEN_FUNCTIONCODEGENERATOR_H__
