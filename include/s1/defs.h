/**\file
 * Shader1 Public API definitions.
 */
#ifndef __S1_DEFS_H__
#define __S1_DEFS_H__

#include <stddef.h>

#ifdef __cplusplus
  /**\def S1_EXTERN_C
   * \internal
   * Storage class for public API methods
   */
#  define S1_EXTERN_C           extern "C"
#  if defined(__GNUC__)
#    define S1_NOTHROW          __attribute__((nothrow))
#  elif defined(_MSC_VER)
#    define S1_NOTHROW          __declspec(nothrow)
#  else
    /**\def S1_NOTHROW
     * \internal
     * Attribute to mark public API methods as non-throwing
     */
#    define S1_NOTHROW
#  endif
  /**\def S1_ARG_DEFAULT(val)
   * \internal
   * Macro for default arguments in API methods
   */
#  define S1_ARG_DEFAULT(val)   = val
#else
#  define S1_EXTERN_C           extern
#  define S1_NOTHROW
#  define S1_ARG_DEFAULT(val)
#endif

#ifdef __GNUC__
#  define S1_FORCEINLINE        __attribute__((always_inline))
#else
  /**\def S1_FORCEINLINE
   * \internal
   * Force a method inline
   */
#  define S1_FORCEINLINE
#endif

/// Boolean type.
typedef int s1_bool;

// TODO Later: dllexport/visibility declspec
/**\def S1_DECLSPEC
 * \internal
 * Calling convention, visibility specification etc for public API methods
 */
#define S1_DECLSPEC

/**\def S1_API
 * \internal
 * Combined public API method attributes
 */
#define S1_API          S1_EXTERN_C S1_DECLSPEC S1_NOTHROW

// Macros so they can be overridden for Doxygen
/**\def S1_NS_CXXAPI_BEGIN
 * \internal
 * Enter namespace for public C++ API wrapper classes
 */
#define S1_NS_CXXAPI_BEGIN    namespace cxxapi {
/**\def S1_NS_CXXAPI_END
 * \internal
 * Leave namespace for public C++ API wrapper classes
 */
#define S1_NS_CXXAPI_END      }

#ifdef DOXYGEN_RUN
/// Main Shader1 API namespace.
namespace s1 {}
#endif

#endif // __S1_DEFS_H__
