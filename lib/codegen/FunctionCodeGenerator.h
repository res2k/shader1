#ifndef __CODEGEN_FUNCTIONCODEGENERATOR_H__
#define __CODEGEN_FUNCTIONCODEGENERATOR_H__

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
      public:
	std::string GetImportedNameIdentifier (const UnicodeString& name)
	{ 
	  /* TODO: Resolve function params */
	  return std::string();
	}
	std::string GetExportedNameIdentifier (const UnicodeString& name)
	{ 
	  /* TODO: Resolve function params */
	  return std::string();
	}
      };
    public:
      StringsArrayPtr Generate (const intermediate::ProgramFunctionPtr& func);
    };
  } // namespace codegen
} // namespace s1

#endif // __CODEGEN_FUNCTIONCODEGENERATOR_H__
