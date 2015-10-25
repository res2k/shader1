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

#include "s1/Backend.h"

#include "compiler/Backend.h"
#include "Program.h"

s1_CompiledProgram* s1_backend_generate_program (s1_Backend* backend,
                                                 s1_Program* program,
                                                 s1_CompileTarget target)
{
  S1_ASSERT_MSG(backend, "NULL Backend", nullptr);
  s1::Compiler::Backend* backend_impl (s1::EvilUpcast<s1::Compiler::Backend> (backend));

  if (!program)
  {
    return backend_impl->ReturnErrorCode (S1_E_INVALID_ARG_N (0), nullptr);
  }
  s1::api_impl::Program* program_impl (s1::EvilUpcast<s1::api_impl::Program> (program));
  
  s1::Compiler::Backend::CompileTarget impl_target;
  switch (target)
  {
  case S1_TARGET_VP:        impl_target = s1::Compiler::Backend::targetVP; break;
  case S1_TARGET_FP:        impl_target = s1::Compiler::Backend::targetFP; break;
  case S1_TARGET_UNSPLIT:   impl_target = s1::Compiler::Backend::targetUnsplit; break;
  default:
    return backend_impl->ReturnErrorCode (S1_E_INVALID_ARG_N (1), nullptr);
  }
  
  s1::Compiler::Backend::ProgramPtr compiled_program (
    program_impl->GetCompiledProgram (backend_impl, impl_target));
  if (!compiled_program)
  {
    return backend_impl->ReturnErrorCode (S1_E_GENERATE_FAILED, nullptr);
  }
  
  compiled_program->AddRef();
  return backend_impl->ReturnSuccess (compiled_program->DowncastEvil<s1_CompiledProgram> ());
}

s1_BackendOptions* s1_backendoptions_create (s1_Backend* backend)
{
  S1_ASSERT_MSG(backend, "NULL Backend", nullptr);
  s1::Compiler::Backend* backend_impl (s1::EvilUpcast<s1::Compiler::Backend> (backend));

  s1::Compiler::Backend::OptionsPtr options (backend_impl->CreateOptions ());
  if (!options)
  {
    return backend_impl->ReturnErrorCode (S1_E_OUT_OF_MEMORY, nullptr);
  }
  options->AddRef();
  return backend_impl->ReturnSuccess (options->DowncastEvil<s1_BackendOptions> ());
}
