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
S1TYPE_DECLARE(S1TYPE_INFO_s1_Library);

/**
 * Create a library object. A library object generally serves as a factory
 * for all available object types.
 */
S1_API s1_ErrorCode s1_create_library (s1_Library** out);

/// Return the last error code set by a failed function
// TODO: More docs on error handling, link
S1_API s1_ErrorCode s1_library_get_last_error (s1_Library* lib);
/// Reset the last error code (to #S1_SUCCESS)
S1_API void s1_library_clear_last_error (s1_Library* lib);

S1TYPE_DECLARE_FWD(s1_Options);
/**
 * Create a compiler options objects.
 * \param lib Parent library.
 * \returns The new compiler options objects.
 *   The returned object will already have a reference, release the reference
 *   using s1_release().
 * In case of an error, \NULL is returned and the error status is saved in the library's
 * last error code.
 */
S1_API s1_Options* s1_options_create (s1_Library* lib);

S1TYPE_DECLARE_FWD(s1_Program);
/**
 * Create a program object from a string.
 * \param lib Parent library.
 * \param source Program source code. Must be encoded in UTF-8.
 * \param sourceSize Program source code size.
 * \returns A new program object.
 *   The returned object will already have a reference, release the reference
 *   using s1_release().
 * In case of an error, \NULL is returned and the error status is saved in the library's
 * last error code.
 */
S1_API s1_Program* s1_program_create_from_string (s1_Library* lib, const char* source,
                                                  size_t sourceSize);

S1TYPE_DECLARE_FWD(s1_Backend);
/**
 * Create a backend object.
 * \param lib Parent library.
 * \param backend Name of the backend to create.
 *   Currently only <tt>"cg"</tt> is supported.
 * \returns A new backend object.
 *   The returned object will already have a reference, release the reference
 *   using s1_release().
 * In case of an error, \NULL is returned and the error status is saved in the library's
 * last error code.
 */
S1_API s1_Backend* s1_backend_create (s1_Library* lib, const char* backend);

#if defined(__cplusplus)
namespace s1
{
  S1_NS_CXXAPI_BEGIN
    class Library : public S1_REBADGE(Library, s1_Library, Object)
    {
    public:
      typedef Ptr<Library> Pointer;

      static ErrorCode Create (Pointer& lib) throw()
      {
        s1_Library* p (0);
        ErrorCode err = s1_create_library (&p);
        if (S1_SUCCESSFUL(err))
        {
          lib.take (FromC (p));
        }
        return err;
      }
      
      ErrorCode GetLastError()
      {
        return s1_library_get_last_error (Cpointer());
      }
      void ClearLastError ()
      {
        s1_library_clear_last_error (Cpointer());
      }
      
      CPtr<s1_Options> CreateOptions ()
      {
        return CPtr<s1_Options> (s1_options_create (Cpointer()), CPtr<s1_Options>::TakeReference ());
      }
      
      CPtr<s1_Program> CreateProgramFromString (const char* source, size_t sourceSize)
      {
        return CPtr<s1_Program> (s1_program_create_from_string (Cpointer(), source, sourceSize),
                                 CPtr<s1_Program>::TakeReference ());
      }
      CPtr<s1_Backend> CreateBackend (const char* backend)
      {
        return CPtr<s1_Backend> (s1_backend_create (Cpointer(), backend),
                                 CPtr<s1_Backend>::TakeReference ());
      }
    };
  S1_NS_CXXAPI_END

#if !defined(S1_BUILD) && !defined(DOXYGEN_RUN)
  typedef cxxapi::Library Library;
#endif // !defined(S1_BUILD)
} // namespace s1

#endif // defined(__cplusplus)

#include "s1/warn_on.h"

#endif // __S1_LIBRARY_H__
