#include "codegen/CgGenerator.h"

#include "ProgramCodeGenerator.h"

namespace s1
{
  namespace codegen
  {
    CgGenerator::CgGenerator ()
    {
    }
    
    StringsArrayPtr CgGenerator::Generate (const intermediate::ProgramPtr& program)
    {
      ProgramCodeGenerator progGen;
      return progGen.Generate (program);
    }
  } // namespace codegen
} // namespace s1
