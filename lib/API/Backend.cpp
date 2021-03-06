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
#include "s1/ResultCode_defs_backend.h"

#include "s1/Backend.h"
#include "s1/BackendOptions.h"

#include "compiler/Backend.h"
#include "compiler/BackendProgram.h"
#include "CompiledProgram.h"
#include "Program.h"

s1_CompiledProgram* s1_backend_generate_program (s1_Backend* backend,
                                                 s1_Program* program,
                                                 s1_CompileTarget target,
                                                 s1_BackendOptions* options)
{
  S1_ASSERT_MSG(backend, "NULL Backend", nullptr);
  s1::Compiler::Backend* backend_impl (s1::EvilUpcast<s1::Compiler::Backend> (backend));
  s1::ScopedThreadDebugMessageHandler setMsgHandler (backend_impl->GetDebugMessageHandler ());

  return backend_impl->Return (backend_impl->Try (
    [=]() -> s1::Result<s1_CompiledProgram*>
    {
      if (!program)
      {
        return S1_E_INVALID_ARG_N (0);
      }
      s1::api_impl::Program* program_impl (s1::EvilUpcast<s1::api_impl::Program> (program));

      s1::Compiler::Backend::CompileTarget impl_target;
      switch (target)
      {
      case S1_TARGET_VP:        impl_target = s1::Compiler::Backend::targetVP; break;
      case S1_TARGET_FP:        impl_target = s1::Compiler::Backend::targetFP; break;
      case S1_TARGET_UNSPLIT:   impl_target = s1::Compiler::Backend::targetUnsplit; break;
      default:
        return S1_E_INVALID_ARG_N (1);
      }

      auto options_impl (s1::EvilUpcast<s1::Compiler::Backend::Options> (options));
      auto compiled_program (
        program_impl->GetCompiledProgram (backend_impl, impl_target, options_impl));
      if (!compiled_program)
      {
        return S1_E_GENERATE_FAILED;
      }

      compiled_program->AddRef ();
      return compiled_program->DowncastEvil<s1_CompiledProgram> ();
    }), nullptr);
}

s1_BackendOptions* s1_backendoptions_create (s1_Backend* backend)
{
  S1_ASSERT_MSG(backend, "NULL Backend", nullptr);
  s1::Compiler::Backend* backend_impl (s1::EvilUpcast<s1::Compiler::Backend> (backend));
  s1::ScopedThreadDebugMessageHandler setMsgHandler (backend_impl->GetDebugMessageHandler ());

  return backend_impl->Return (backend_impl->Try (
    [=]()
    {
      s1::Compiler::Backend::OptionsPtr options (backend_impl->CreateOptions ());
      options->AddRef();
      return options->DowncastEvil<s1_BackendOptions> ();
    }), nullptr);
}
