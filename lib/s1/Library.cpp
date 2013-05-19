#include "s1/Library.h"

#include "base/common.h"
#include "base/Library.h"

#include "compiler/Backend.h"
#include "compiler/Options.h"

#include "Program.h"

#include <new>
#include <string.h>

s1_ErrorCode s1_create_library (s1_Library** out)
{
  if (!out) return S1_E_INVALID_ARG_N(0);
  
  s1::Library* new_lib (new (std::nothrow) s1::Library);
  if (!new_lib) return S1_E_OUT_OF_MEMORY;
  new_lib->AddRef();
  *out = new_lib->DowncastEvil<s1_Library> ();
  return S1_SUCCESS;
}

s1_ErrorCode s1_library_get_last_error (s1_Library* lib)
{
  S1_ASSERT_MSG(lib, "NULL Library",
                S1_MAKE_ERROR(S1_ERROR_COMP_BASE, 0xbad1));
  return s1::EvilUpcast<s1::Library> (lib)->GetLastError();
}

void s1_library_clear_last_error (s1_Library* lib)
{
  S1_ASSERT_MSG(lib, "NULL Library", S1_ASSERT_RET_VOID);
  s1::EvilUpcast<s1::Library> (lib)->SetLastError (S1_SUCCESS);
}

s1_Options* s1_options_create (s1_Library* obj)
{
  S1_ASSERT_MSG(obj, "NULL Library", nullptr);
  s1::Library* lib (s1::EvilUpcast<s1::Library> (obj));
  
  s1::Compiler::OptionsPtr options (lib->GetCompiler().CreateOptions());
  if (!options)
  {
    lib->SetLastError (S1_E_OUT_OF_MEMORY);
    return nullptr;
  }
  options->AddRef();
  lib->SetLastError (S1_SUCCESS);
  return options->DowncastEvil<s1_Options> ();
}

s1_Program* s1_program_create_from_string (s1_Library* obj, const char* source,
                                           size_t sourceSize, unsigned int compatLevel)
{
  S1_ASSERT_MSG(obj, "NULL Library", nullptr);
  s1::Library* lib (s1::EvilUpcast<s1::Library> (obj));
  
  if (!source)
  {
    lib->SetLastError (S1_E_INVALID_ARG_N (0));
    return nullptr;
  }
  
  if (compatLevel > S1_COMPATIBILITY_LATEST)
  {
    lib->SetLastError (S1_E_INCOMPATIBLE);
    return nullptr;
  }
  
  std::string sourceStr (source, sourceSize);
  boost::intrusive_ptr<s1::api_impl::Program> program (
    new (std::nothrow) s1::api_impl::Program (lib, lib->GetCompiler(), sourceStr));
  if (!program)
  {
    lib->SetLastError (S1_E_OUT_OF_MEMORY);
    return nullptr;
  }
  program->AddRef();
  lib->SetLastError (S1_SUCCESS);
  return program->DowncastEvil<s1_Program> ();
}

s1_Backend* s1_backend_create (s1_Library* obj, const char* backend)
{
  S1_ASSERT_MSG(obj, "NULL Library", nullptr);
  s1::Library* lib (s1::EvilUpcast<s1::Library> (obj));
  
  if (!backend)
  {
    lib->SetLastError (S1_E_INVALID_ARG_N (0));
    return nullptr;
  }
  if (strcmp (backend, "cg") != 0)
  {
    lib->SetLastError (S1_E_UNKNOWN_BACKEND);
    return nullptr;
  }
  
  s1::Compiler::BackendPtr backend_obj (lib->GetCompiler().CreateBackendCg());
  if (!backend_obj)
  {
    lib->SetLastError (S1_E_OUT_OF_MEMORY);
    return nullptr;
  }
  backend_obj->AddRef();
  lib->SetLastError (S1_SUCCESS);
  return backend_obj->DowncastEvil<s1_Backend> ();
}

