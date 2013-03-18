#include "s1/LibraryObject.h"

#include "base/common.h"
#include "base/LibraryObject.h"

#include <new>

s1_Library* s1_libraryobject_get_library (s1_LibraryObject* obj)
{
  if (!obj) return nullptr;
  s1::LibraryObject* libObj (s1::EvilUpcast<s1::LibraryObject> (obj));
  return libObj->GetLibrary()->DowncastEvil<s1_Library> ();
}
