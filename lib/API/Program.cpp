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

#include "Program.h"

#include "compiler/Program.h"
#include "splitter/Frequency.h"

#include <sstream>

namespace s1
{
  namespace api_impl
  {
    void Program::Dirty()
    {
      wrapped_program.reset();
    }

    Compiler::Backend::ProgramPtr Program::GetCompiledProgram (const Compiler::BackendPtr& backend,
                                                               Compiler::Backend::CompileTarget target)
    {
      // FIXME!!!: This is rather inefficient!
      if (!wrapped_program)
      {
        std::istringstream inputStream (source, std::ios_base::in | std::ios_base::binary);
        uc::String entryFunctionU (uc::String::fromUTF8 (entryFunction));
        wrapped_program = compiler.CreateProgram (inputStream, entryFunctionU);
        wrapped_program->SetEntryFunctionName (entryFunctionU);
        
        for(const InputFreqMapType::value_type& inputFreq : inputFreqMap)
        {
          uc::String ucParam (uc::String::fromUTF8 (inputFreq.first));
          wrapped_program->SetInputParameterFrequencyFlags (ucParam, 1 << inputFreq.second);
        }
        for(const InputSizeMapType::value_type& inputSize : inputSizeMap)
        {
          uc::String ucParam (uc::String::fromUTF8 (inputSize.first));
          wrapped_program->SetInputArrayParameterSize (ucParam, 1 << inputSize.second);
        }
      }
      
      return wrapped_program->GetCompiledProgram (options, backend, target);
    }

    s1_ResultCode Program::SetOptions (const s1::Compiler::OptionsPtr& options)
    {
      this->options.reset (new s1::Compiler::Options (GetLibrary(), *(options.get())));
      Dirty ();
      return S1_SUCCESS;
    }
    const s1::Compiler::OptionsPtr& Program::GetOptions() const { return options; }
    
    s1_ResultCode Program::SetEntry (const char* entry)
    {
      // TODO: Function name validation
      entryFunction = entry;
      Dirty();
      return S1_SUCCESS;
    }
    s1_ResultCode Program::GetEntry (const char*& entry) const
    {
      entry = entryFunction.c_str();
      return S1_SUCCESS;
    }
    
    s1_ResultCode Program::SetInputFrequency (const char* param, s1_InputFrequency freq)
    {
      // TODO: Parameter validation
      s1::splitter::Frequency new_freq;
      switch (freq)
      {
      case S1_FREQ_UNIFORM:     new_freq = s1::splitter::freqUniform; break;
      case S1_FREQ_VERTEX:      new_freq = s1::splitter::freqVertex; break;
      default:
        return S1_E_INVALID_FREQUENCY;
      }
      inputFreqMap[param] = new_freq;
      Dirty();
      return S1_SUCCESS;
    }
    s1_ResultCode Program::GetInputFrequency (const char* param, s1_InputFrequency& freq) const
    {
      InputFreqMapType::const_iterator it (inputFreqMap.find (param));
      if (it == inputFreqMap.end())
      {
        // FIXME: Return default frequency instead
        return S1_E_UNKNOWN_PARAMETER;
      }
      switch (it->second)
      {
      default:
      case s1::splitter::freqUniform:   freq = S1_FREQ_UNIFORM; break;
      case s1::splitter::freqVertex:    freq = S1_FREQ_VERTEX; break;
      }
      return S1_SUCCESS;
    }

    s1_ResultCode Program::SetInputArraySize (const char* param, size_t size)
    {
      // TODO: Parameter validation
      inputSizeMap[param] = size;
      Dirty();
      return S1_SUCCESS;
    }
    s1_ResultCode Program::GetInputArraySize (const char* param, size_t& size) const
    {
      InputSizeMapType::const_iterator it (inputSizeMap.find (param));
      if (it == inputSizeMap.end())
      {
        // FIXME: Return default size instead
        return S1_E_UNKNOWN_PARAMETER;
      }
      size = it->second;
      return S1_SUCCESS;
    }
  } // namespace api_impl
} // namespace s1

s1_bool s1_program_set_options (s1_Program* program, s1_Options* options)
{
  S1_ASSERT_MSG(program, "NULL Program", false);
  s1::api_impl::Program* program_impl (s1::EvilUpcast<s1::api_impl::Program> (program));

  if (!options)
  {
    return program_impl->ReturnErrorCode (S1_E_INVALID_ARG_N(0));
  }
  s1::Compiler::Options* options_impl (s1::EvilUpcast<s1::Compiler::Options> (options));

  return program_impl->ReturnErrorCode (program_impl->SetOptions (options_impl));
}

s1_Options* s1_program_get_options (s1_Program* program)
{
  S1_ASSERT_MSG(program, "NULL Program", nullptr);
  s1::api_impl::Program* program_impl (s1::EvilUpcast<s1::api_impl::Program> (program));

  s1::Compiler::OptionsPtr options_impl (program_impl->GetOptions());
  return program_impl->ReturnSuccess (options_impl->DowncastEvil<s1_Options> ());
}

s1_bool s1_program_set_entry_function (s1_Program* program, const char* name)
{
  S1_ASSERT_MSG(program, "NULL Program", false);
  s1::api_impl::Program* program_impl (s1::EvilUpcast<s1::api_impl::Program> (program));

  if (!name)
  {
    return program_impl->ReturnErrorCode (S1_E_INVALID_ARG_N(0));
  }

  return program_impl->ReturnErrorCode (program_impl->SetEntry (name));
}

const char* s1_program_get_entry_function (s1_Program* program)
{
  S1_ASSERT_MSG(program, "NULL Program", nullptr);
  s1::api_impl::Program* program_impl (s1::EvilUpcast<s1::api_impl::Program> (program));

  const char* entry (nullptr);
  s1_ResultCode error (program_impl->GetEntry (entry));
  return program_impl->ReturnErrorCode (error, entry);
}

s1_bool s1_program_set_input_frequency (s1_Program* program, const char* param, s1_InputFrequency freq)
{
  S1_ASSERT_MSG(program, "NULL Program", false);
  s1::api_impl::Program* program_impl (s1::EvilUpcast<s1::api_impl::Program> (program));

  if (!param)
  {
    return program_impl->ReturnErrorCode (S1_E_INVALID_ARG_N(0));
  }

  return program_impl->ReturnErrorCode (program_impl->SetInputFrequency (param, freq));
}

s1_InputFrequency s1_program_get_input_frequency (s1_Program* program, const char* param)
{
  s1_InputFrequency freq (S1_FREQ_INVALID);
  S1_ASSERT_MSG(program, "NULL Program", freq);
  s1::api_impl::Program* program_impl (s1::EvilUpcast<s1::api_impl::Program> (program));

  if (!param)
  {
    return program_impl->ReturnErrorCode (S1_E_INVALID_ARG_N(0), freq);
  }

  s1_ResultCode error (program_impl->GetInputFrequency (param, freq));
  return program_impl->ReturnErrorCode (error, freq);
}

s1_bool s1_program_set_input_array_size (s1_Program* program, const char* param, size_t size)
{
  S1_ASSERT_MSG(program, "NULL Program", false);
  s1::api_impl::Program* program_impl (s1::EvilUpcast<s1::api_impl::Program> (program));

  if (!param)
  {
    return program_impl->ReturnErrorCode (S1_E_INVALID_ARG_N(0));
  }

  return program_impl->ReturnErrorCode (program_impl->SetInputArraySize (param, size));
}

size_t s1_program_get_input_array_size (s1_Program* program, const char* param)
{
  size_t size ((size_t)~0);
  S1_ASSERT_MSG(program, "NULL Program", size);
  s1::api_impl::Program* program_impl (s1::EvilUpcast<s1::api_impl::Program> (program));

  if (!param)
  {
    return program_impl->ReturnErrorCode (S1_E_INVALID_ARG_N(0), size);
  }

  s1_ResultCode error (program_impl->GetInputArraySize (param, size));
  return program_impl->ReturnErrorCode (error, size);
}

