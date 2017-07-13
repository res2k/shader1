/*
    Shader1
    Copyright (c) 2010-2017 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

/**\file
 * Object types forward declarations
 */
#ifndef __S1_FWD_H__
#define __S1_FWD_H__

#include "s1/warn_off.h"

#include "s1/Object.h"

S1TYPE_DECLARE_FWD(Backend);
S1TYPE_DECLARE_FWD(BackendOptions);
S1TYPE_DECLARE_FWD(ByteStream);
S1TYPE_DECLARE_FWD(CompiledProgram);
S1TYPE_DECLARE_FWD(Library);
S1TYPE_DECLARE_FWD(LibraryObject);
S1TYPE_DECLARE_FWD(Options);
S1TYPE_DECLARE_FWD(Program);
S1TYPE_DECLARE_FWD(String);

#if defined(__cplusplus)
namespace s1
{
#if !defined(S1_BUILD) && !defined(DOXYGEN_RUN)
  typedef cxxapi::Backend Backend;
  typedef cxxapi::BackendOptions BackendOptions;
  typedef cxxapi::ByteStream ByteStream;
  typedef cxxapi::CompiledProgram CompiledProgram;
  typedef cxxapi::Library Library;
  typedef cxxapi::LibraryObject LibraryObject;
  typedef cxxapi::Options Options;
  typedef cxxapi::Program Program;
  typedef cxxapi::String String;
#endif // !defined(S1_BUILD)
} // namespace s1
#endif // defined(__cplusplus)

#include "s1/warn_on.h"

#endif // __S1_FWD_H__
