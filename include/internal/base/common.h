/*
    Shader1
    Copyright (c) 2010-2018 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#ifndef __BASE_COMMON_H__
#define __BASE_COMMON_H__

/**\file
 * Common definitions
 */

#include "base/config.h"
#include <boost/cstdint.hpp> // must be include before any unicode/ headers to avoid macro conflict

/**\def S1_APPROX_DIGITS(Type)
 * Approximate number of characters needed to display a value of type
 * \a Type in decimal system.
 */
/* (sizeof(x)*25)/10+1 is an approximation of the number of characters
  * needed to display x in decimal system. (x can be at most 256^sizeof(x).
  * You need log10(256^sizeof(x)) characters, becoming
  * sizeof(x)*log10(256). 25/10 is an (over-)approximation of log10(256).) */
#define S1_APPROX_DIGITS(Type)          ((sizeof(Type) * 25) / 10)
static const size_t charsToFormatUint = S1_APPROX_DIGITS(unsigned int);
// Add 1 for '-'
static const size_t charsToFormatInt = S1_APPROX_DIGITS(unsigned int) + 1;

//
// Debugging helpers
//

// Provide _DEBUG on all platforms
#if !defined(NDEBUG) && !defined(_DEBUG)
  #define _DEBUG
#endif

#ifdef __GNUC__
#  define S1_NOINLINE        __attribute__((noinline))
#elif defined(_MSC_VER)
#  define S1_NOINLINE        __declspec(noinline)
#else
  /**\def S1_NOINLINE
   * \internal
   * Avoid inlinining of a function
   */
#  define S1_NOINLINE
#endif

namespace s1
{
  namespace detail
  {
  #if defined(_WIN32)
    typedef wchar_t assert_char;
    #define _S1_ASSERT_STR(S)     _S1_ASSERT_STR_2(S)
    #define _S1_ASSERT_STR_2(S)   L ## S
  #else
    typedef char assert_char;
    #define _S1_ASSERT_STR(S)     S
  #endif

    /// Print assertion using current debug message handler
    // NOINLINE to avoid LTO inlining it
    void S1_NOINLINE PrintAssert (const assert_char* filename, int line, const assert_char* condition, const assert_char* message = nullptr);
  } // namespace detail
} // namespace s1

#include <assert.h>
/// \internal For use as S1_ASSERT \a "ret" value for functions returning \c void
#define S1_ASSERT_RET_VOID
#define _S1_ASSERT_MSG_PRINT(x, msg)                          \
  if (!(x))                                                   \
  {                                                           \
    ::s1::detail::PrintAssert (_S1_ASSERT_STR(__FILE__),      \
                                __LINE__,                     \
                                _S1_ASSERT_STR(#x),           \
                                msg);                         \
  }
#define _S1_ASSERT_MSG_RET(x, ret)      if (!(x)) { return ret; }
#define S1_ASSERT(x, ret)                                     \
  do {                                                        \
    _S1_ASSERT_MSG_PRINT(x, nullptr)                          \
    assert (x);                                               \
    _S1_ASSERT_MSG_RET(x, ret)                                \
  } while(0)
#define S1_ASSERT_MSG(x, msg, ret)                            \
  do {                                                        \
    _S1_ASSERT_MSG_PRINT(x, _S1_ASSERT_STR(msg))              \
    assert ((x) && msg);                                      \
    _S1_ASSERT_MSG_RET(x, ret)                                \
  } while(0)
//@{
/// \internal Assertion for code that is not expected to be reached
#define S1_ASSERT_NOT_REACHED_MSG(msg, ret)                   \
  do {                                                        \
    ::s1::detail::PrintAssert (_S1_ASSERT_STR(__FILE__),      \
                                __LINE__,                     \
                                _S1_ASSERT_STR("false"),      \
                                _S1_ASSERT_STR(msg));         \
    assert ((msg) && false);                                  \
    return ret;                                               \
  } while(0)
#define S1_ASSERT_NOT_REACHED(ret)                            \
  S1_ASSERT_NOT_REACHED_MSG("code should not be reached", ret)
//@}

// Definitions for wchar_t encoding.
// Assumes a world where wchar_t is always some UTF.
#if CXX_SIZEOF_WCHAR_T == 2
#define S1_WCHAR_IS_UTF16
#elif CXX_SIZEOF_WCHAR_T == 4
#define S1_WCHAR_IS_UTF32
#else
#error Unsupported wchar_t size
#endif

// Used all over the place
#include "uc/String.h"

// Customize boost::math error handlers
#define BOOST_MATH_DOMAIN_ERROR_POLICY user_error
#define BOOST_MATH_POLE_ERROR_POLICY user_error
#define BOOST_MATH_OVERFLOW_ERROR_POLICY user_error
#define BOOST_MATH_EVALUATION_ERROR_POLICY user_error
#define BOOST_MATH_ROUNDING_ERROR_POLICY user_error

// Include math_error_handler.h to get actual declarations for error handlers

#endif // __BASE_COMMON_H__
