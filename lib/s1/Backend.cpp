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
  case S1_TARGET_VP:    impl_target = s1::Compiler::Backend::targetVP; break;
  case S1_TARGET_FP:    impl_target = s1::Compiler::Backend::targetFP; break;
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

