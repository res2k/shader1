#include <boost/cstdint.hpp>
#include "base/hash_UnicodeString.h"
#include "intermediate/Program.h"

#include "splitter/Frequency.h"

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
    
    void Program::SetOutputParameter (const UnicodeString& name, ParameterTarget target)
    {
      outputParams[name] = target;
    }
    
    const Program::OutputParameters& Program::GetOutputParameters () const
    {
      return outputParams;
    }
    
    int Program::GetTargetFrequency (ParameterTarget target)
    {
      switch (target)
      {
      case Position:	return splitter::freqVertex;
      case Color:	return splitter::freqFragment;
      }
      assert (false);
      return -1;
    }
    
  } // namespace intermediate
} // namespace s1

