/*
    Shader1
    Copyright (c) 2017 Frank Richter


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
 * API functions string argument support
 */
#ifndef __S1_STRINGARG_H__
#define __S1_STRINGARG_H__

#include "s1/Object.h"

/**
 * Structure representing a string argument to an API function.
 * Generate using s1_u8(), s1_u16(), s1_u32(), s1_wcs(), s1_strobj(),
 * s1_u8_n(), s1_u16_n(), s1_u32_n(), or s1_wcs_n().
 */
typedef struct s1_StringArg_s
{
  /**\internal Pointer to string */
  uintptr_t p;
  /**\internal Type & size of string */
  uintptr_t ts;
} s1_StringArg;

/* StringArg type: bit 1 (MSB) */
#define _S1_SA_TS_TYPE_BIT_1      (~(UINTPTR_MAX >> 1))
/* StringArg type: bit 0 (below MSB) */
#define _S1_SA_TS_TYPE_BIT_0      (_S1_SA_TS_TYPE_BIT_1 >> 1)
/* String arg: type part of type & size */
#define _S1_SA_TS_TYPE_MASK       (_S1_SA_TS_TYPE_BIT_1 | _S1_SA_TS_TYPE_BIT_0)
/* String arg: size part of type & size */
#define _S1_SA_TS_SIZE_MASK       (~(_S1_SA_TS_TYPE_BIT_1 | _S1_SA_TS_TYPE_BIT_0))

/* Chosen to not collide with _S1_SA_TS_SIZE_AUTO or _S1_SA_TS_SIZE_INVALID. */
#define _S1_SA_TS_SIZE_MAX        (_S1_SA_TS_SIZE_MASK-2)
/**\def S1_STRING_ARG_MAX_N
 * Maximum length of a string argument.
 * \hideinitializer
 * \internal \sa s1_string_size_type
 */
#define S1_STRING_ARG_MAX_N       (_S1_SA_TS_SIZE_MAX < (UINT32_MAX-1) ? _S1_SA_TS_SIZE_MAX : (UINT32_MAX-1))

/* StringArg type: UTF-8 */
#define _S1_SA_TS_TYPE_UTF8       0
/* StringArg type: UTF-16 */
#define _S1_SA_TS_TYPE_UTF16      (_S1_SA_TS_TYPE_BIT_0)
/* StringArg type: UTF-32 */
#define _S1_SA_TS_TYPE_UTF32      (_S1_SA_TS_TYPE_BIT_1)
/* StringArg type: wchar_t */
#define _S1_SA_TS_TYPE_WCS        (_S1_SA_TS_TYPE_BIT_1 | _S1_SA_TS_TYPE_BIT_0)
/* StringArg size: string too long */
#define _S1_SA_TS_SIZE_INVALID    _S1_SA_TS_SIZE_MASK
/* StringArg size: compute string length */
#define _S1_SA_TS_SIZE_AUTO       (_S1_SA_TS_SIZE_INVALID-1)

/**\internal
 * Common logic to construct an s1_StringArg.
 */
static inline s1_StringArg _s1_stringarg (uintptr_t type, const void* str, size_t len)
{
  s1_StringArg arg = { (uintptr_t)str, type };
  if (str)
  {
    if (len == (size_t)-1)
    {
      arg.ts |= _S1_SA_TS_SIZE_AUTO;
    }
    else if (len > S1_STRING_ARG_MAX_N)
    {
      arg.ts |= _S1_SA_TS_SIZE_INVALID;
      arg.p = 0; /* See s1_strobj() for importance */
    }
    else
    {
      arg.ts |= (uintptr_t)len;
    }
  }
  else
  {
    arg.ts = 0;
  }
  return arg;
}

/**
 * Pass a string argument for an UTF-8 string.
 * \param str %String to pass.
 * \param len Length of the string. Use <tt>(size_t)-1</tt> to determine length automatically
 */
static inline s1_StringArg s1_u8_n (const char* str, size_t len)
{
  return _s1_stringarg (_S1_SA_TS_TYPE_UTF8, str, len);
}

/**
 * Pass a string argument for an UTF-8 string.
 * \param str %String to pass.
 */
static inline s1_StringArg s1_u8 (const char* str)
{
  return s1_u8_n (str, (size_t)-1);
}

/**
 * Pass a string argument for an UTF-16 string.
 * \param str %String to pass.
 * \param len Length of the string. Use <tt>(size_t)-1</tt> to determine length automatically
 */
static inline s1_StringArg s1_u16_n (const s1_char16* str, size_t len)
{
  return _s1_stringarg (_S1_SA_TS_TYPE_UTF16, str, len);
}

/**
 * Pass a string argument for an UTF-16 string.
 * \param str %String to pass.
 */
static inline s1_StringArg s1_u16 (const s1_char16* str)
{
  return s1_u16_n (str, (size_t)-1);
}

/**
 * Pass a string argument for an UTF-32 string.
 * \param str %String to pass.
 * \param len Length of the string. Use <tt>(size_t)-1</tt> to determine length automatically
 */
static inline s1_StringArg s1_u32_n (const s1_char32* str, size_t len)
{
  return _s1_stringarg (_S1_SA_TS_TYPE_UTF32, str, len);
}

/**
 * Pass a string argument for an UTF-32 string.
 * \param str %String to pass.
 */
static inline s1_StringArg s1_u32 (const s1_char32* str)
{
  return s1_u32_n (str, (size_t)-1);
}

/**
 * Pass a string argument for an wide character string.
 * \param str %String to pass.
 * \param len Length of the string. Use <tt>(size_t)-1</tt> to determine length automatically
 */
static inline s1_StringArg s1_wcs_n (const wchar_t* str, size_t len)
{
  return _s1_stringarg (_S1_SA_TS_TYPE_WCS, str, len);
}

/**
 * Pass a string argument for an wide character string.
 * \param str %String to pass.
 */
static inline s1_StringArg s1_wcs (const wchar_t* str)
{
  return s1_wcs_n (str, (size_t)-1);
}

S1TYPE_DECLARE_FWD(String);

/**
 * Pass a string argument for an s1_String object.
 * \param str %String to pass.
 */
static inline s1_StringArg s1_strobj (s1_String* str)
{
  /* Special syntax: UINTPTR_MAX size&type and non-null pointer
   * indicate s1_String.
   * Thus for other types, an invalid size means pointer must be 0 */
  s1_StringArg arg = { (uintptr_t)str, str ? UINTPTR_MAX : 0 };
  return arg;
}

#if defined(__cplusplus)
namespace s1
{
  S1_NS_CXXAPI_BEGIN
  /**
   * Structure representing a string argument to an API function.
   * Generate using u8(), u16(), u32(), wcs(), strobj(),
   * or construction from a string.
   */
  struct StringArg : public s1_StringArg
  {
  public:
    StringArg (const s1_StringArg& arg): s1_StringArg (arg) {}

  #if defined(DOXYGEN_RUN) || defined(S1_CHAR_STRING_IS_UTF8)
    /**
     * Construct an UTF-8 string argument.
     * \param str %String to pass.
     * \param len Length of the string. Use <tt>(size_t)-1</tt> to determine length automatically
     * \remarks This constructor is only available if the \c S1_CHAR_STRING_IS_UTF8
     * preprocessor macro is defined.
     * Define it if you're storing UTF-8 encoded strings in your \c char* or \c std::string
     * strings.
     */
    StringArg (const char* str, size_t len = (size_t)-1) : s1_StringArg (s1_u8 (str, len)) {}
    /**
    * Construct an UTF-8 string argument.
    * \param str %String to pass.
    * \remarks This constructor is only available if the \c S1_CHAR_STRING_IS_UTF8
    * preprocessor macro is defined.
    * Define it if you're storing UTF-8 encoded strings in your \c char* or \c std::string
    * strings.
    */
    StringArg (const std::string& str) : s1_StringArg (s1_u8 (str.c_str(), str.size())) {}
  #endif
    /**
     * Construct an UTF-16 string argument.
     * \param str %String to pass.
     * \param len Length of the string. Use <tt>(size_t)-1</tt> to determine length automatically
     */
    StringArg (const s1_char16* str, size_t len = (size_t)-1) : s1_StringArg (s1_u16_n (str, len)) {}
    /**
     * Construct an UTF-16 string argument.
     * \param str %String to pass.
     */
    StringArg (const std::basic_string<s1_char16>& str) : s1_StringArg (s1_u16_n (str.c_str(), str.size())) {}
    /**
     * Construct an UTF-32 string argument.
     * \param str %String to pass.
     * \param len Length of the string. Use <tt>(size_t)-1</tt> to determine length automatically
     */
    StringArg (const s1_char32* str, size_t len = (size_t)-1) : s1_StringArg (s1_u32_n (str, len)) {}
    /**
     * Construct an UTF-32 string argument.
     * \param str %String to pass.
     */
    StringArg (const std::basic_string<s1_char32>& str) : s1_StringArg (s1_u32_n (str.c_str(), str.size())) {}
    /**
     * Construct a wide character string argument.
     * \param str %String to pass.
     * \param len Length of the string. Use <tt>(size_t)-1</tt> to determine length automatically
     */
    StringArg (const wchar_t* str, size_t len = (size_t)-1) : s1_StringArg (s1_wcs_n (str, len)) {}
    /**
     * Construct a wide character string argument.
     * \param str %String to pass.
     */
    StringArg (const std::wstring& str) : s1_StringArg (s1_wcs_n (str.c_str(), str.size())) {}
    /**
     * Construct a string argument from a String object.
     * \param str %String to pass.
     */
    StringArg (String* str) : s1_StringArg (s1_strobj (str)) {}
  };

  /// \copydoc s1_u8_n
  static inline StringArg u8 (const char* str, size_t len = (size_t)-1)
  {
    return s1_u8_n (str, len);
  }
  /// \copydoc s1_u8
  static inline StringArg u8 (const std::string& str)
  {
    return s1_u8_n (str.c_str(), str.size());
  }

  /// \copydoc s1_u16_n
  static inline StringArg u16 (const s1_char16* str, size_t len = (size_t)-1)
  {
    return s1_u16_n (str, len);
  }
  /// \copydoc s1_u16
  static inline StringArg u16 (const std::basic_string<s1_char16>& str)
  {
    return s1_u16_n (str.c_str(), str.size());
  }

  /// \copydoc s1_u32_n
  static inline StringArg u32 (const s1_char32* str, size_t len = (size_t)-1)
  {
    return s1_u32_n (str, len);
  }
  /// \copydoc s1_u32
  static inline StringArg u32 (const std::basic_string<s1_char32>& str)
  {
    return s1_u32_n (str.c_str(), str.size());
  }

  /// \copydoc s1_wcs_n
  static inline StringArg wcs (const wchar_t* str, size_t len = (size_t)-1)
  {
    return s1_wcs_n (str, len);
  }
  /// \copydoc s1_wcs
  static inline StringArg wcs (const std::wstring& str)
  {
    return s1_wcs_n (str.c_str(), str.size());
  }

  /**
  * Pass a string argument for a String object.
  * \param str %String to pass.
  */
  static inline StringArg strobj (String* str)
  {
    return s1_strobj (str);
  }
  S1_NS_CXXAPI_END

#if !defined(S1_BUILD) && !defined(DOXYGEN_RUN)
  typedef cxxapi::StringArg StringArg;

  using cxxapi::u8;
  using cxxapi::u16;
  using cxxapi::u32;
  using cxxapi::wcs;
  using cxxapi::strobj;
#endif // !defined(S1_BUILD)
} // namespace s1

#endif // defined(__cplusplus)

#endif // __S1_STRINGARG_H__
