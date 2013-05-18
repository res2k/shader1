/**\file
 * Shader1 Public API definitions.
 */
#ifndef __S1_DEFS_H__
#define __S1_DEFS_H__

#include <stddef.h>

#ifdef __cplusplus
#  define S1_EXTERN_C           extern "C"
#  if defined(__GNUC__)
#    define S1_NOTHROW          __attribute__((nothrow))
#  elif defined(_MSC_VER)
#    define S1_NOTHROW          __declspec(nothrow)
#  else
#    define S1_NOTHROW
#  endif
#  define S1_ARG_DEFAULT(val)   = val
#else
#  define S1_EXTERN_C           extern
#  define S1_NOTHROW
#  define S1_ARG_DEFAULT(val)
#endif

#ifdef __GNUC__
#  define S1_FORCEINLINE        __attribute__((always_inline))
#else
#  define S1_FORCEINLINE
#endif

typedef int s1_bool;

// TODO Later: dllexport/visibility declspec
#define S1_DECLSPEC

#define S1_API          S1_EXTERN_C S1_DECLSPEC S1_NOTHROW

// Macro so it can be overridden for Doxygen
#define S1_NS_CXXAPI_BEGIN    namespace cxxapi {
#define S1_NS_CXXAPI_END      }

#endif // __S1_DEFS_H__
