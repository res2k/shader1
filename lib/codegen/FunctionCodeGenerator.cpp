#include <boost/cstdint.hpp>

#include "FunctionCodeGenerator.h"

#include "intermediate/ProgramFunction.h"
#include "SequenceCodeGenerator.h"

#include <boost/make_shared.hpp>
#include <string>

namespace s1
{
  namespace codegen
  {
    StringsArrayPtr CgGenerator::FunctionCodeGenerator::Generate (const intermediate::ProgramFunctionPtr& func)
    {
      StringsArrayPtr resultStrings (boost::make_shared<StringsArray> ());
      resultStrings->AddString (std::string ("// TODO: proper function declaration"));
      resultStrings->AddString (std::string ("{"));
      
      SequenceCodeGenerator seqGen (*(func->GetBody()));
      resultStrings->AddStrings (*(seqGen.Generate()), 2);
      
      resultStrings->AddString (std::string ("}"));
      return resultStrings;
    }
  } // namespace codegen
} // namespace s1

