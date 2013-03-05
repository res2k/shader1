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
    return OptionsPtr (new Options);
  }
  
  Compiler::BackendPtr Compiler::CreateBackendCg ()
  {
    return boost::make_shared<compiler::BackendCg> ();
  }

  Compiler::ProgramPtr Compiler::CreateProgram (const OptionsPtr& compilerOptions,
						std::istream& input)
  {
    UnicodeStream uniStream (input, "utf-8");
    return ProgramPtr (new Program (compilerOptions, &uniStream));
  }
} // namespace s1
