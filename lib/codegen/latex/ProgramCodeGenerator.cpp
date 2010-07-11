#include <boost/cstdint.hpp>

#include "ProgramCodeGenerator.h"

#include "intermediate/Program.h"
#include "FunctionCodeGenerator.h"

#include <boost/make_shared.hpp>
#include <string>

namespace s1
{
  namespace codegen
  {
    StringsArrayPtr LatexGenerator::ProgramCodeGenerator::Generate (const intermediate::ProgramPtr& prog)
    {
      StringsArrayPtr resultStrings (boost::make_shared<StringsArray> ());
      
      for (size_t i = 0; i < prog->GetNumFunctions(); i++)
      {
	intermediate::ProgramFunctionPtr func (prog->GetFunction (i));
	FunctionCodeGenerator funcGen;
	resultStrings->AddStrings (*(funcGen.Generate (func)));
	resultStrings->AddString (std::string ());
      }
      
      return resultStrings;
    }
  } // namespace codegen
} // namespace s1

