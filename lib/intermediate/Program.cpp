#include <boost/cstdint.hpp>
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
    
    void Program::AddTransferValue (const IntermediateGeneratorSemanticsHandler::TypePtr& type,
				    const UnicodeString& name)
    {
      transferValues.push_back (std::make_pair (type, name));
    }
  } // namespace intermediate
} // namespace s1

