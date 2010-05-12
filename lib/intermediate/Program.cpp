#include "intermediate/Program.h"

namespace s1
{
  namespace intermediate
  {
    void Program::AddFunction (const ProgramFunctionPtr& func)
    {
      functions.push_back (func);
    }
      
    size_t Program::GetNumFunctions () const
    {
      return functions.size();
    }
    
    ProgramFunctionPtr Program::GetFunction (size_t n) const
    {
      return functions[n];
    }
  } // namespace intermediate
} // namespace s1

