/*
    Shader1
    Copyright (c) 2010-2014 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

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
