#include "s1/Library.h"

#include "base/Object.h"

#include <new>

namespace s1
{
  class Library : public Object
  {
  public:
    Library() {}
    
    // TODO: Store stuff like memory allocator, global options etc... here
  }; 
}

s1_ErrorCode s1_create_library (s1_Library** out)
{
  if (!out) return S1_E_INVALID_ARG_N(0);
  
  s1::Library* new_lib (new (std::nothrow) s1::Library);
  if (!new_lib) return S1_E_OUT_OF_MEMORY;
  new_lib->AddRef();
  *out = new_lib->DowncastEvil<s1_Library> ();
  return S1_SUCCESS;
}
