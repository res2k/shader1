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
#include "base/ResultCode_internal.h"

#include "compiler/Compiler.h"

#include "BackendGLSL.h"
#include "base/uc/SimpleBufferStreamSource.h"
#include "base/uc/Stream.h"
#include "compiler/Options.h"
#include "compiler/Program.h"

#include <boost/make_shared.hpp>

namespace s1
{
  Compiler::OptionsPtr Compiler::CreateOptions ()
  {
    return OptionsPtr (new Options (lib));
  }
  
  Compiler::BackendPtr Compiler::CreateBackend (SupportedBackend backend)
  {
    switch (backend)
    {
    case beGLSL:
      return BackendPtr (new compiler::BackendGLSL (lib));
    }
    return BackendPtr ();
  }

  Compiler::ProgramPtr Compiler::CreateProgram (uc::Stream::Source& inputStream)
  {
    uc::Stream uniStream (inputStream);
    return ProgramPtr (new Program (lib, &uniStream));
  }
} // namespace s1
