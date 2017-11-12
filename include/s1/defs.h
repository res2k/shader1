/*
    Shader1
    Copyright (c) 2010-2014 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

/**\file
 * Shader1 Public API definitions.
 */
#ifndef __S1_DEFS_H__
#define __S1_DEFS_H__

#include "s1/s1config.h"
#include <stddef.h>
#include <stdint.h>

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
#elif defined(_MSC_VER)
#  define S1_FORCEINLINE        __forceinline
#else
  /**\def S1_FORCEINLINE
   * \internal
   * Force a method inline
   */
#  define S1_FORCEINLINE
#endif

#ifdef __GNUC__
#  define S1_FUNCTION_NAME    __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
#  define S1_FUNCTION_NAME    __FUNCSIG__
#else
  /**\def S1_FORCEINLINE
   * \internal
   * Resolved to name of current function (or 0)
   */
#  define S1_FUNCTION_NAME    0
#endif

#if defined(DOXYGEN_RUN) \
  || (defined(__cplusplus) && (__cplusplus >= 201103L)) \
  || (defined(__cplusplus) && defined(_MSC_VER) && (_MSC_VER >= 1600))
/**\def S1_HAVE_RVALUES
 * Defined if compiler supports rvalue types.
 */
#define S1_HAVE_RVALUES
/**\def S1_HAVE_NULLPTR
 * Defined if compiler supports nullptr.
 */
#define S1_HAVE_NULLPTR

// On some MSVC versions we need to pull in some header to get charXX_t
#if defined(_MSC_VER)
#if _MSC_VER >= 1900
#include <uchar.h>
#else
#include <cstdint>
#endif
#endif

 /**\def S1_HAVE_CHARXX_T
 * \internal
 * Defined if compiler natively supports \c charXX_t types.
 */
#define S1_HAVE_CHARXX_T
#endif

/**\def S1_NULL
 * \NULL pointer value for arguments etc.
 */
#if defined(S1_HAVE_NULLPTR)
#define S1_NULL    nullptr
#else
#define S1_NULL    NULL
#endif

/// Boolean type.
typedef int s1_bool;

#ifdef S1_HAVE_CHARXX_T
/// UTF-16 code unit type
typedef char16_t s1_char16;
/// UTF-32 code unit type
typedef char32_t s1_char32;
#else
typedef uint16_t s1_char16;
typedef uint32_t s1_char32;
#endif

/// String size type.
typedef uint32_t s1_string_size_type;

/**\def S1_VISIBILITY
 * \internal
 * Visibility specification for public API methods
 */
#ifdef _WIN32
#  if defined(S1_BUILD_SHARED)
#    ifdef S1_BUILD
#      define S1_VISIBILITY    _declspec(dllexport)
#    else
#      define S1_VISIBILITY    _declspec(dllimport)
#    endif
#  else
#    define S1_VISIBILITY
#  endif
#else
#  ifdef __GNUC__
#    define S1_VISIBILITY      __attribute__((visibility("default")))
#  else
#    define S1_VISIBILITY
#  endif
#endif

/**\def S1_API(T)
 * \internal
 * Combined public API method attributes. \a T is the function return type.
 */
#define S1_API(T)          S1_EXTERN_C S1_VISIBILITY S1_NOTHROW T

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
