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
