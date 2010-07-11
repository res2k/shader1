#ifndef __CODEGEN_PROGRAMCODEGENERATOR_H__
#define __CODEGEN_PROGRAMCODEGENERATOR_H__

#include "codegen/LatexGenerator.h"
#include "codegen/StringsArray.h"
#include "intermediate/forwarddecl.h"

namespace s1
{
  namespace codegen
  {
    class LatexGenerator::ProgramCodeGenerator
    {
    public:
      StringsArrayPtr Generate (const intermediate::ProgramPtr& prog);
    };
  } // namespace codegen
} // namespace s1

#endif // __CODEGEN_PROGRAMCODEGENERATOR_H__
