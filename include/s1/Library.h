/**\file
 * Main library entry object
 */
#ifndef __S1_LIBRARY_H__
#define __S1_LIBRARY_H__

#include "s1/warn_off.h"

#include "s1/Error.h"
#include "s1/Object.h"
#include "s1/Ptr.h"

#define S1TYPE_INFO_s1_Library   (s1_Library, S1TYPE_INFO_s1_Object)
S1TYPE_DECLARE(s1_Library);

/**
 * Create a library object. A library object generally serves as a factory
 * for all available object types.
 */
S1_API s1_ErrorCode s1_create_library (s1_Library** out);

#if defined(__cplusplus)
namespace s1
{
  namespace cxxapi
  {
    class Library : public s1_Library,
                    public Object
    {
    public:
      typedef Ptr<Library> Pointer;

      static ErrorCode Create (Pointer& lib) throw()
      {
        s1_Library* p (0);
        ErrorCode err = s1_create_library (&p);
        if (S1_SUCCESSFUL(err))
        {
          lib.take (static_cast<Library*> (p));
        }
        return err;
      }
    };
  } // namespace cxxapi

#if !defined(S1_BUILD)
  typedef cxxapi::Library Library;
#endif // !defined(S1_BUILD)
} // namespace s1

#endif // defined(__cplusplus)

#include "s1/warn_on.h"

#endif // __S1_LIBRARY_H__
