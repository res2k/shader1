/*
    Shader1
    Copyright (c) 2010-2016 Frank Richter


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

#include "Program.h"

#include "base/uc/String_optional.h"
#include "compiler/BackendProgram.h"
#include "compiler/Program.h"
#include "splitter/Frequency.h"

#include "CompiledProgram.h"
#include "StringArg.h"
#include "StringObj.h"

namespace s1
{
  namespace api_impl
  {
    Program::Program(s1::Library* lib, Compiler& compiler)
      : LibraryObject (lib),
        compiler (compiler),
        options (new s1::Compiler::Options (lib))
    {
      auto stringCreateResult = String::Create (lib, "main");
      s1::ResultCode err = std::get<0> (stringCreateResult);
      if (!S1_SUCCESSFUL(err))
      {
        throw Exception (err);
      }
      entryFunction = std::get<1> (stringCreateResult);
    }

    Program::Program(s1::Library* lib, Compiler& compiler, const std::string& source)
      : Program (lib, compiler)
    {
      this->source = source;

      wrapped_program = compiler.CreateProgram (source.data(), source.size());
    }

    Program::Program (s1::Library* lib, Compiler& compiler, std::function<size_t (const char*&)> streamFunc)
      : Program (lib, compiler)
    {
      // FIXME: Kind of stupid... Should (eventually) be able to pass stream directly to CreateProgram()
      const char* data;
      size_t dataSize;
      while ((dataSize = streamFunc (data)) != 0)
      {
        source.append (data, dataSize);
      }

      wrapped_program = compiler.CreateProgram (source.data(), source.size());
    }

    boost::intrusive_ptr<CompiledProgram> Program::GetCompiledProgram (const Compiler::BackendPtr& backend,
                                                                       Compiler::Backend::CompileTarget target,
                                                                       Compiler::Backend::OptionsPtr backendOptions)
    {
      Compiler::Program::FreqFlagMap inputFreqFlags;
      for(const InputFreqMapType::value_type& inputFreq : inputFreqMap)
      {
        inputFreqFlags.emplace (inputFreq.first, 1 << inputFreq.second);
      }
      Compiler::Program::ArraySizeMap inputArraySizes;
      for(const InputSizeMapType::value_type& inputSize : inputSizeMap)
      {
        inputArraySizes.emplace (inputSize.first, inputSize.second);
      }
      auto compiledProgram =
        wrapped_program->GetCompiledProgram (entryFunction->StrUCS(), options, inputFreqFlags, inputArraySizes, backend, target,
                                             backendOptions);
      if (!compiledProgram) return nullptr;
      return new api_impl::CompiledProgram (GetLibrary(), compiledProgram);
    }

    s1::ResultCode Program::SetOptions (const s1::Compiler::OptionsPtr& options)
    {
      this->options.reset (new s1::Compiler::Options (GetLibrary(), *(options.get())));
      return S1_SUCCESS;
    }
    const s1::Compiler::OptionsPtr& Program::GetOptions() const { return options; }
    
    s1::ResultCode Program::SetEntry (boost::intrusive_ptr<String> entry)
    {
      // TODO: Function name validation
      entryFunction = entry;
      return S1_SUCCESS;
    }
    Result<String*> Program::GetEntry () const
    {
      return entryFunction.get();
    }
    
    s1::ResultCode Program::SetInputFrequency (const uc::String& param, s1_InputFrequency freq)
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
      return S1_SUCCESS;
    }
    Result<InputFrequency> Program::GetInputFrequency (const uc::String& param) const
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
      case s1::splitter::freqUniform:   return S1_FREQ_UNIFORM; break;
      case s1::splitter::freqVertex:    return S1_FREQ_VERTEX; break;
      }
      return S1_E_FAILURE;
    }

    s1::ResultCode Program::SetInputArraySize (const uc::String& param, size_t size)
    {
      // TODO: Parameter validation
      inputSizeMap[param] = size;
      return S1_SUCCESS;
    }
    Result<size_t> Program::GetInputArraySize (const uc::String& param) const
    {
      InputSizeMapType::const_iterator it (inputSizeMap.find (param));
      if (it == inputSizeMap.end())
      {
        // FIXME: Return default size instead
        return S1_E_UNKNOWN_PARAMETER;
      }
      return it->second;
    }
  } // namespace api_impl
} // namespace s1

s1_bool s1_program_set_options (s1_Program* program, s1_Options* options)
{
  S1_ASSERT_MSG(program, "NULL Program", false);
  s1::api_impl::Program* program_impl (s1::EvilUpcast<s1::api_impl::Program> (program));
  s1::ScopedThreadDebugMessageHandler setMsgHandler (program_impl->GetDebugMessageHandler ());

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
  s1::ScopedThreadDebugMessageHandler setMsgHandler (program_impl->GetDebugMessageHandler ());

  s1::Compiler::OptionsPtr options_impl (program_impl->GetOptions());
  return program_impl->ReturnSuccess (options_impl->DowncastEvil<s1_Options> ());
}

s1_bool s1_program_set_entry_function (s1_Program* program, s1_StringArg string)
{
  S1_ASSERT_MSG(program, "NULL Program", false);
  s1::api_impl::Program* program_impl (s1::EvilUpcast<s1::api_impl::Program> (program));
  s1::ScopedThreadDebugMessageHandler setMsgHandler (program_impl->GetDebugMessageHandler ());

  s1::api_impl::ResolveStringArg resolve_str (string, 0);
  if (auto strObj = resolve_str.GetStringObj ())
  {
    return program_impl->ReturnErrorCode (program_impl->SetEntry (strObj));
  }

  return program_impl->Return (program_impl->Try (
    [=]() -> s1::Result<bool> {
      auto createRes =
        s1::api_impl::String::Create (program_impl->GetLibrary (), resolve_str.GetString ());
      auto createResCode = std::get<0> (createRes);
      if (S1_FAILED(createResCode)) return createResCode;
      const auto& strObj = std::get<1> (createRes);
      auto result = program_impl->SetEntry (strObj);
      return s1::Result<bool> (S1_SUCCESSFUL (result), result);
    }), false);
}

s1_String* s1_program_get_entry_function (s1_Program* program)
{
  S1_ASSERT_MSG(program, "NULL Program", nullptr);
  s1::api_impl::Program* program_impl (s1::EvilUpcast<s1::api_impl::Program> (program));
  s1::ScopedThreadDebugMessageHandler setMsgHandler (program_impl->GetDebugMessageHandler ());

  return program_impl->Return (
    program_impl->GetEntry ().filter ([=](s1::api_impl::String* str) { return str->DowncastEvil<s1_String> (); }),
    nullptr);
}

s1_bool s1_program_set_input_frequency (s1_Program* program, s1_StringArg param, s1_InputFrequency freq)
{
  S1_ASSERT_MSG(program, "NULL Program", false);
  s1::api_impl::Program* program_impl (s1::EvilUpcast<s1::api_impl::Program> (program));
  s1::ScopedThreadDebugMessageHandler setMsgHandler (program_impl->GetDebugMessageHandler ());

  return program_impl->Return (program_impl->Try (
    [=]() {
      s1::ResultCode result = program_impl->SetInputFrequency (s1::api_impl::ResolveStringArg (param, 0), freq);
      return s1::Result<bool> (S1_SUCCESSFUL (result), result);
    }), false);
}

s1_InputFrequency s1_program_get_input_frequency (s1_Program* program, s1_StringArg param)
{
  s1_InputFrequency errorFreq (S1_FREQ_INVALID);
  S1_ASSERT_MSG(program, "NULL Program", errorFreq);
  s1::api_impl::Program* program_impl (s1::EvilUpcast<s1::api_impl::Program> (program));
  s1::ScopedThreadDebugMessageHandler setMsgHandler (program_impl->GetDebugMessageHandler ());

  return program_impl->Return (program_impl->Try (
      [=]() -> s1::Result<s1_InputFrequency> {
      return program_impl->GetInputFrequency (s1::api_impl::ResolveStringArg (param, 0));
    }), errorFreq);
}

s1_bool s1_program_set_input_array_size (s1_Program* program, s1_StringArg param, size_t size)
{
  S1_ASSERT_MSG(program, "NULL Program", false);
  s1::api_impl::Program* program_impl (s1::EvilUpcast<s1::api_impl::Program> (program));
  s1::ScopedThreadDebugMessageHandler setMsgHandler (program_impl->GetDebugMessageHandler ());

  return program_impl->Return (program_impl->Try (
    [=]() -> s1::Result<bool> {
      auto result = program_impl->SetInputArraySize (s1::api_impl::ResolveStringArg (param, 0), size);
      return s1::Result<bool> (S1_SUCCESSFUL (result), result);
    }), false);
}

size_t s1_program_get_input_array_size (s1_Program* program, s1_StringArg param)
{
  const size_t errorSize ((size_t)~0);
  S1_ASSERT_MSG(program, "NULL Program", errorSize);
  s1::api_impl::Program* program_impl (s1::EvilUpcast<s1::api_impl::Program> (program));
  s1::ScopedThreadDebugMessageHandler setMsgHandler (program_impl->GetDebugMessageHandler ());

  return program_impl->Return (program_impl->Try (
    [=]() -> s1::Result<size_t> {
      return program_impl->GetInputArraySize (s1::api_impl::ResolveStringArg (param, 0));
    }), errorSize);
}
