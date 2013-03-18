/**\file
 * Base class for objects created by Library instances
 */
#ifndef __S1_LIBRARYOBJECT_H__
#define __S1_LIBRARYOBJECT_H__

#include "s1/warn_off.h"

#include "s1/Library.h"

#define S1TYPE_INFO_s1_LibraryObject   (s1_LibraryObject, S1TYPE_INFO_s1_Object)
S1TYPE_DECLARE(S1TYPE_INFO_s1_LibraryObject);

/**
 * Query the library used to create an object.
 * \remarks Does not add a reference to the returned object.
 */
S1_API s1_Library* s1_libraryobject_get_library (s1_LibraryObject* obj);

#if defined(__cplusplus)
namespace s1
{
  namespace cxxapi
  {
    class LibraryObject : public Rebadge<LibraryObject, s1_LibraryObject, Object>
    {
    public:
      typedef Ptr<LibraryObject> Pointer;

      Library* GetLibrary()
      {
        return Library::FromC (s1_libraryobject_get_library (Cpointer()));
      }
    };
  } // namespace cxxapi

#if !defined(S1_BUILD)
  typedef cxxapi::LibraryObject LibraryObject;
#endif // !defined(S1_BUILD)
} // namespace s1

#endif // defined(__cplusplus)

#include "s1/warn_on.h"

#endif // __S1_LIBRARYOBJECT_H__
