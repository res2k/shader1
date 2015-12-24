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

/**\file
 * Wrapper to provide public API program objects
 */
#ifndef __S1_CXXAPI_PROGRAM_HPP__
#define __S1_CXXAPI_PROGRAM_HPP__

#include "s1/Program.h"

#include "base/LibraryObject.h"
#include "base/Result.h"

#include "compiler/Backend.h"
#include "compiler/Program.h"
#include "compiler/Options.h"

#include <boost/unordered_map.hpp>

namespace s1
{
  namespace api_impl
  {
    class Program : public s1::LibraryObject
    {
    protected:
      Compiler& compiler;
      s1::Compiler::OptionsPtr options; // TODO: Ideally, this would be copy-on-write
      std::string source;
      std::string entryFunction;
      typedef boost::unordered_map<std::string, s1::splitter::Frequency> InputFreqMapType;
      InputFreqMapType inputFreqMap;
      typedef boost::unordered_map<std::string, size_t> InputSizeMapType;
      InputSizeMapType inputSizeMap;
      
      Compiler::ProgramPtr wrapped_program;
      void Dirty();
    public:
      Program (s1::Library* lib, Compiler& compiler, const std::string& source);

      Compiler::Backend::ProgramPtr GetCompiledProgram (const Compiler::BackendPtr& backend,
                                                        Compiler::Backend::CompileTarget target,
                                                        Compiler::Backend::OptionsPtr backendOptions);

      /**\name Methods mapping to public API
       * @{ */
      s1_ResultCode SetOptions (const s1::Compiler::OptionsPtr& options);
      const s1::Compiler::OptionsPtr& GetOptions() const;
      
      s1_ResultCode SetEntry (const char* entry);
      Result<const char*> GetEntry () const;
      
      s1_ResultCode SetInputFrequency (const char* param, s1_InputFrequency freq);
      Result<InputFrequency> GetInputFrequency (const char* param) const;

      s1_ResultCode SetInputArraySize (const char* param, size_t size);
      Result<size_t> GetInputArraySize (const char* param) const;
      /** @} */
    };
  } // namespace api_impl
} // namespace s1

#endif // __S1_CXXAPI_PROGRAM_HPP__