#include "s1/CompiledProgram.h"

#include "base/common.h"

#include "compiler/Backend.h"

const char* s1_compiledprogram_get_string (s1_CompiledProgram* program)
{
  S1_ASSERT_MSG(program, "NULL CompiledProgram", nullptr);
  s1::Compiler::Backend::Program* program_impl (s1::EvilUpcast<s1::Compiler::Backend::Program> (program));

  return program_impl->ReturnSuccess (program_impl->GetProgramString().c_str());
}
