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

#include "BackendCg.h"
#include "BackendGLSL.h"
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
    case beCg:
      return BackendPtr (new compiler::BackendCg (lib));
    case beGLSL:
      return BackendPtr (new compiler::BackendGLSL (lib));
    }
    return BackendPtr ();
  }

  class IStreamSource : public uc::Stream::Source
  {
    std::istream& input;

    enum
    {
      /// Size of internal unicode characters buffer
      UCBufferSize = 1024
    };
    /**
     * Internal buffer for data from input stream.
     */
    char streamInBuffer[UCBufferSize];
  public:
    IStreamSource (std::istream& input) : input (input) {}

    bool HaveMoreData () override
    {
      return input.good () && !input.eof ();
    }
    size_t NextData (const char*& data) override
    {
      input.read (streamInBuffer, sizeof (streamInBuffer));
      data = streamInBuffer;
      return size_t (input.gcount());
    }
  };

  Compiler::ProgramPtr Compiler::CreateProgram (std::istream& input)
  {
    IStreamSource source (input);
    uc::Stream uniStream (source);
    return ProgramPtr (new Program (&uniStream));
  }
} // namespace s1
