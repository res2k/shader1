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
 * Wrapper to provide public API CompiledProgram objects
 */
#ifndef __S1_CXXAPI_COMPILEDPROGRAM_HPP__
#define __S1_CXXAPI_COMPILEDPROGRAM_HPP__

#include "s1/CompiledProgram.h"

#include "base/LibraryObject.h"

#include "compiler/Backend.h"

namespace s1
{
  namespace api_impl
  {
    class CompiledProgram : public s1::LibraryObject
    {
    public:
      CompiledProgram (s1::Library* lib, Compiler::Backend::ProgramPtr compiledProgram);

      const char* GetString ();
    protected:
      Compiler::Backend::ProgramPtr compiledProgram;
      boost::optional<std::string> flatString;
    };
  } // namespace api_impl
} // namespace s1

#endif // __S1_CXXAPI_COMPILEDPROGRAM_HPP__
