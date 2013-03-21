#include "s1/Library.h"

#include "base/common.h"
#include "base/Library.h"

#include <new>

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
                S1_MAKE_ERROR(_S1_ERROR_COMP_BASE, 0xbad1));
  return s1::EvilUpcast<s1::Library> (lib)->GetLastError();
}

void s1_library_clear_last_error (s1_Library* lib)
{
  S1_ASSERT_MSG(lib, "NULL Library", S1_ASSERT_RET_VOID);
  s1::EvilUpcast<s1::Library> (lib)->SetLastError (S1_SUCCESS);
}
