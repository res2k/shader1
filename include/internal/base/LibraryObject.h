/**\file
 * Base class for objects created by a library.
 */
#ifndef __BASE_LIBRARYOBJECT_H__
#define __BASE_LIBRARYOBJECT_H__

#include "s1/LibraryObject.h"
#include "base/Library.h"

namespace s1
{
  class LibraryObject : public Object
  {
    LibraryPtr lib;
  public:
    LibraryObject (Library* lib) : lib (lib) {}
    
    Library* GetLibrary() { return lib.get(); }
  }; 
} // namespace s1

#endif // __BASE_LIBRARYOBJECT_H__
