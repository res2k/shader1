#include "base/common.h"

#include "compiler/Compiler.h"

#include "BackendCg.h"
#include "base/UnicodeStream.h"
#include "compiler/Options.h"
#include "compiler/Program.h"

#include <boost/make_shared.hpp>

namespace s1
{
  Compiler::OptionsPtr Compiler::CreateOptions ()
  {
    return OptionsPtr (new Options (lib));
  }
  
  Compiler::BackendPtr Compiler::CreateBackendCg ()
  {
    return BackendPtr (new compiler::BackendCg (lib));
  }

  Compiler::ProgramPtr Compiler::CreateProgram (std::istream& input, const UnicodeString& entryFunction)
  {
    UnicodeStream uniStream (input, "utf-8");
    return ProgramPtr (new Program (&uniStream, entryFunction));
  }
} // namespace s1
