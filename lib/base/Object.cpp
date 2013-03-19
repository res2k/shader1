#include "base/Object.h"

#include "base/common.h"

#undef s1_add_ref
#undef s1_release
#undef s1_get_ref_count

int s1_add_ref (s1_Object* obj)
{
  S1_ASSERT_MSG(obj, "NULL Object", 0);
  return s1::EvilUpcast<s1::Object> (obj)->AddRef();
}

int s1_release (s1_Object* obj)
{
  S1_ASSERT_MSG(obj, "NULL Object", 0);
  return s1::EvilUpcast<s1::Object> (obj)->Release();
}

int s1_get_ref_count (s1_Object* obj)
{
  S1_ASSERT_MSG(obj, "NULL Object", 0);
  return s1::EvilUpcast<s1::Object> (obj)->GetRefCount();
}
