/**\file
 * Shader1 Public API definitions.
 */
#ifndef __S1_DEFS_H__
#define __S1_DEFS_H__

#ifdef __cplusplus
#  define S1_EXTERN_C           extern "C"
#  if defined(__GNUC__)
#    define S1_NOTHROW          __attribute__((nothrow))
#  elif defined(_MSC_VER)
#    define S1_NOTHROW          __declspec(nothrow)
#  else
#    define S1_NOTHROW
#  endif
#else
#  define S1_EXTERN_C           extern
#  define S1_NOTHROW
#endif

typedef int s1_bool;

// TODO Later: dllexport/visibility declspec
#define S1_DECLSPEC

#define S1_API          S1_EXTERN_C S1_DECLSPEC S1_NOTHROW

#endif // __S1_DEFS_H__
