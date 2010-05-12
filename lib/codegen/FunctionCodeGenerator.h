#ifndef __CODEGEN_FUNCTIONCODEGENERATOR_H__
#define __CODEGEN_FUNCTIONCODEGENERATOR_H__

#include "codegen/CgGenerator.h"
#include "codegen/StringsArray.h"
#include "intermediate/forwarddecl.h"

namespace s1
{
  namespace codegen
  {
    class CgGenerator::FunctionCodeGenerator
    {
    public:
      StringsArrayPtr Generate (const intermediate::ProgramFunctionPtr& func);
    };
  } // namespace codegen
} // namespace s1

#endif // __CODEGEN_FUNCTIONCODEGENERATOR_H__
