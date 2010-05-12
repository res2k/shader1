#ifndef __INTERMEDIATE_PROGRAM_H__
#define __INTERMEDIATE_PROGRAM_H__

#include "forwarddecl.h"

#include <vector>

namespace s1
{
  namespace intermediate
  {
    class Program
    {
    protected:
      friend class IntermediateGeneratorSemanticsHandler;
      
      std::vector<ProgramFunctionPtr> functions;
      void AddFunction (const ProgramFunctionPtr& func);
    public:
      size_t GetNumFunctions () const;
      ProgramFunctionPtr GetFunction (size_t n) const;
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_PROGRAM_H__