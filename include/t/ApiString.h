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

#include <cxxtest/TestSuite.h>

#include "s1/Library.h"
#include "s1/String.h"

static const char testString1_u8[] = { 'H', 'e', 'l', 'l', 'o', 0 };
static const s1_char16 testString1_u16[] = { 'H', 'e', 'l', 'l', 'o', 0 };
static const s1_char32 testString1_u32[] = { 'H', 'e', 'l', 'l', 'o', 0 };
static const wchar_t testString1_wcs[] = { 'H', 'e', 'l', 'l', 'o', 0 };

#define ASSERT_EQUALS_STR1_U8(strData)                                  \
  TS_ASSERT_EQUALS(std::char_traits<char>::length ((strData)), 5);      \
  TS_ASSERT_EQUALS((strData)[0], testString1_u8[0]);                    \
  TS_ASSERT_EQUALS((strData)[1], testString1_u8[1]);                    \
  TS_ASSERT_EQUALS((strData)[2], testString1_u8[2]);                    \
  TS_ASSERT_EQUALS((strData)[3], testString1_u8[3]);                    \
  TS_ASSERT_EQUALS((strData)[4], testString1_u8[4]);                    \
  TS_ASSERT_EQUALS((strData)[5], testString1_u8[5]);

#define ASSERT_EQUALS_STR1_U16(strData)                                 \
  TS_ASSERT_EQUALS(std::char_traits<s1_char16>::length ((strData)), 5); \
  TS_ASSERT_EQUALS((strData)[0], testString1_u16[0]);                   \
  TS_ASSERT_EQUALS((strData)[1], testString1_u16[1]);                   \
  TS_ASSERT_EQUALS((strData)[2], testString1_u16[2]);                   \
  TS_ASSERT_EQUALS((strData)[3], testString1_u16[3]);                   \
  TS_ASSERT_EQUALS((strData)[4], testString1_u16[4]);                   \
  TS_ASSERT_EQUALS((strData)[5], testString1_u16[5]);

#define ASSERT_EQUALS_STR1_U32(strData)                                 \
  TS_ASSERT_EQUALS(std::char_traits<s1_char32>::length ((strData)), 5); \
  TS_ASSERT_EQUALS((strData)[0], testString1_u32[0]);                   \
  TS_ASSERT_EQUALS((strData)[1], testString1_u32[1]);                   \
  TS_ASSERT_EQUALS((strData)[2], testString1_u32[2]);                   \
  TS_ASSERT_EQUALS((strData)[3], testString1_u32[3]);                   \
  TS_ASSERT_EQUALS((strData)[4], testString1_u32[4]);                   \
  TS_ASSERT_EQUALS((strData)[5], testString1_u32[5]);

#define ASSERT_EQUALS_STR1_WCS(strData)                                  \
  TS_ASSERT_EQUALS(std::char_traits<wchar_t>::length ((strData)), 5);   \
  TS_ASSERT_EQUALS((strData)[0], testString1_wcs[0]);                    \
  TS_ASSERT_EQUALS((strData)[1], testString1_wcs[1]);                    \
  TS_ASSERT_EQUALS((strData)[2], testString1_wcs[2]);                    \
  TS_ASSERT_EQUALS((strData)[3], testString1_wcs[3]);                    \
  TS_ASSERT_EQUALS((strData)[4], testString1_wcs[4]);                    \
  TS_ASSERT_EQUALS((strData)[5], testString1_wcs[5]);

static const char testString2_u8[] = { 'H', (char)0xc3, (char)0xab, 'l', 'l', 'o', 0 };
static const s1_char16 testString2_u16[] = { 'H', 0xeb, 'l', 'l', 'o', 0 };
static const s1_char32 testString2_u32[] = { 'H', 0xeb, 'l', 'l', 'o', 0 };
static const wchar_t testString2_wcs[] = { 'H', 0xeb, 'l', 'l', 'o', 0 };

#define ASSERT_EQUALS_STR2_U8(strData)                                  \
  TS_ASSERT_EQUALS(std::char_traits<char>::length ((strData)), 6);      \
  TS_ASSERT_EQUALS((strData)[0], testString2_u8[0]);                    \
  TS_ASSERT_EQUALS((strData)[1], testString2_u8[1]);                    \
  TS_ASSERT_EQUALS((strData)[2], testString2_u8[2]);                    \
  TS_ASSERT_EQUALS((strData)[3], testString2_u8[3]);                    \
  TS_ASSERT_EQUALS((strData)[4], testString2_u8[4]);                    \
  TS_ASSERT_EQUALS((strData)[5], testString2_u8[5]);                    \
  TS_ASSERT_EQUALS((strData)[6], testString2_u8[6]);

#define ASSERT_EQUALS_STR2_U16(strData)                                 \
  TS_ASSERT_EQUALS(std::char_traits<s1_char16>::length ((strData)), 5); \
  TS_ASSERT_EQUALS((strData)[0], testString2_u16[0]);                   \
  TS_ASSERT_EQUALS((strData)[1], testString2_u16[1]);                   \
  TS_ASSERT_EQUALS((strData)[2], testString2_u16[2]);                   \
  TS_ASSERT_EQUALS((strData)[3], testString2_u16[3]);                   \
  TS_ASSERT_EQUALS((strData)[4], testString2_u16[4]);                   \
  TS_ASSERT_EQUALS((strData)[5], testString2_u16[5]);

#define ASSERT_EQUALS_STR2_U32(strData)                                 \
  TS_ASSERT_EQUALS(std::char_traits<s1_char32>::length ((strData)), 5); \
  TS_ASSERT_EQUALS((strData)[0], testString2_u32[0]);                   \
  TS_ASSERT_EQUALS((strData)[1], testString2_u32[1]);                   \
  TS_ASSERT_EQUALS((strData)[2], testString2_u32[2]);                   \
  TS_ASSERT_EQUALS((strData)[3], testString2_u32[3]);                   \
  TS_ASSERT_EQUALS((strData)[4], testString2_u32[4]);                   \
  TS_ASSERT_EQUALS((strData)[5], testString2_u32[5]);

#define ASSERT_EQUALS_STR2_WCS(strData)                                 \
  TS_ASSERT_EQUALS(std::char_traits<wchar_t>::length ((strData)), 5);   \
  TS_ASSERT_EQUALS((strData)[0], testString2_wcs[0]);                   \
  TS_ASSERT_EQUALS((strData)[1], testString2_wcs[1]);                   \
  TS_ASSERT_EQUALS((strData)[2], testString2_wcs[2]);                   \
  TS_ASSERT_EQUALS((strData)[3], testString2_wcs[3]);                   \
  TS_ASSERT_EQUALS((strData)[4], testString2_wcs[4]);                   \
  TS_ASSERT_EQUALS((strData)[5], testString2_wcs[5]);

static const char testString3_u8[] = { (char)0xf0, (char)0x9f, (char)0x90, (char)0xb5, 0 };
static const s1_char16 testString3_u16[] = { 0xd83d, 0xdc35, 0 };
static const s1_char32 testString3_u32[] = { 0x1F435, 0 };

#define ASSERT_EQUALS_STR3_U8(strData)                                  \
  TS_ASSERT_EQUALS(std::char_traits<char>::length ((strData)), 4);      \
  TS_ASSERT_EQUALS((strData)[0], testString3_u8[0]);                    \
  TS_ASSERT_EQUALS((strData)[1], testString3_u8[1]);                    \
  TS_ASSERT_EQUALS((strData)[2], testString3_u8[2]);                    \
  TS_ASSERT_EQUALS((strData)[3], testString3_u8[3]);                    \
  TS_ASSERT_EQUALS((strData)[4], testString3_u8[4]);

#define ASSERT_EQUALS_STR3_U16(strData)                                 \
  TS_ASSERT_EQUALS(std::char_traits<s1_char16>::length ((strData)), 2); \
  TS_ASSERT_EQUALS((strData)[0], testString3_u16[0]);                   \
  TS_ASSERT_EQUALS((strData)[1], testString3_u16[1]);                   \
  TS_ASSERT_EQUALS((strData)[2], testString3_u16[2]);

#define ASSERT_EQUALS_STR3_U32(strData)                                 \
  TS_ASSERT_EQUALS(std::char_traits<s1_char32>::length ((strData)), 1); \
  TS_ASSERT_EQUALS((strData)[0], testString3_u32[0]);                   \
  TS_ASSERT_EQUALS((strData)[1], testString3_u32[1]);

class ApiStringTestSuite : public CxxTest::TestSuite
{
public:
  void testCreateNullStrC (void)
  {
    // Check object creation
    s1_Library* lib;
    s1_ResultCode err = s1_create_library (&lib);
    TS_ASSERT(S1_SUCCESSFUL(err));
    TS_ASSERT_DIFFERS(lib, (s1_Library*)0);

    s1_String* str;
    str = s1_string_create (lib, 0, 0);
    TS_ASSERT_EQUALS(str, (s1_String*)0);
    TS_ASSERT(S1_ERROR_EQUAL(s1_library_get_last_error (lib), S1_E_INVALID_ARG));

    s1_release (lib);
  }

  void testCreateNullStrCXX (void)
  {
    s1::Library::Pointer lib;
    s1_ResultCode err = s1::Library::Create (lib);
    TS_ASSERT(S1_SUCCESSFUL(err));
    TS_ASSERT_DIFFERS(lib, (s1::Library*)nullptr);
    s1::String::Pointer str = lib->CreateString ((const char*)nullptr);
    TS_ASSERT_EQUALS(str, (s1::String*)nullptr);
    TS_ASSERT(S1_ERROR_EQUAL(lib->GetLastError(), S1_E_INVALID_ARG));
  }

  void testCreateAsciiCXX (void)
  {
    s1::Library::Pointer lib;
    s1_ResultCode err = s1::Library::Create (lib);
    TS_ASSERT(S1_SUCCESSFUL (err));
    TS_ASSERT_DIFFERS(lib, (s1::Library*)nullptr);

    {
      s1::String::Pointer str = lib->CreateString (testString1_u8);
      TS_ASSERT_DIFFERS (str, (s1::String*)nullptr);
      const char* str_utf8 = str->U8 ();
      ASSERT_EQUALS_STR1_U8(str_utf8);
      const s1_char16* str_utf16 = str->U16 ();
      ASSERT_EQUALS_STR1_U16(str_utf16);
      const s1_char32* str_utf32 = str->U32 ();
      ASSERT_EQUALS_STR1_U32(str_utf32);
      const wchar_t* str_wcs = str->WCS ();
      ASSERT_EQUALS_STR1_WCS(str_wcs);
    }
    {
      s1::String::Pointer str = lib->CreateString (testString1_u16);
      TS_ASSERT_DIFFERS (str, (s1::String*)nullptr);
      const char* str_utf8 = str->U8 ();
      ASSERT_EQUALS_STR1_U8(str_utf8);
      const s1_char16* str_utf16 = str->U16 ();
      ASSERT_EQUALS_STR1_U16(str_utf16);
      const s1_char32* str_utf32 = str->U32 ();
      ASSERT_EQUALS_STR1_U32(str_utf32);
      const wchar_t* str_wcs = str->WCS ();
      ASSERT_EQUALS_STR1_WCS(str_wcs);
    }
    {
      s1::String::Pointer str = lib->CreateString (testString1_u32);
      TS_ASSERT_DIFFERS (str, (s1::String*)nullptr);
      const char* str_utf8 = str->U8 ();
      ASSERT_EQUALS_STR1_U8(str_utf8);
      const s1_char16* str_utf16 = str->U16 ();
      ASSERT_EQUALS_STR1_U16(str_utf16);
      const s1_char32* str_utf32 = str->U32 ();
      ASSERT_EQUALS_STR1_U32(str_utf32);
      const wchar_t* str_wcs = str->WCS ();
      ASSERT_EQUALS_STR1_WCS(str_wcs);
    }
    {
      s1::String::Pointer str = lib->CreateString (testString1_wcs);
      TS_ASSERT_DIFFERS (str, (s1::String*)nullptr);
      const char* str_utf8 = str->U8 ();
      ASSERT_EQUALS_STR1_U8(str_utf8);
      const s1_char16* str_utf16 = str->U16 ();
      ASSERT_EQUALS_STR1_U16(str_utf16);
      const s1_char32* str_utf32 = str->U32 ();
      ASSERT_EQUALS_STR1_U32(str_utf32);
      const wchar_t* str_wcs = str->WCS ();
      ASSERT_EQUALS_STR1_WCS(str_wcs);
    }
  }

  void testCreateUnicode1C (void)
  {
    s1_Library* lib;
    s1_ResultCode err = s1_create_library (&lib);
    TS_ASSERT(S1_SUCCESSFUL(err));
    TS_ASSERT_DIFFERS(lib, (s1_Library*)0);

    {
      s1_String* str;
      str = s1_string_create (lib, testString2_u8, 0);
      TS_ASSERT_DIFFERS(str, (s1_String*)0);
      const char* str_utf8 = s1_string_u8 (str);
      ASSERT_EQUALS_STR2_U8(str_utf8);
      const s1_char16* str_utf16 = s1_string_u16 (str);
      ASSERT_EQUALS_STR2_U16(str_utf16);
      const s1_char32* str_utf32 = s1_string_u32 (str);
      ASSERT_EQUALS_STR2_U32(str_utf32);
      const wchar_t* str_wcs = s1_string_wcs (str);
      ASSERT_EQUALS_STR2_WCS(str_wcs);
      s1_release (str);
    }
    {
      s1_String* str;
      str = s1_string_create_u16 (lib, testString2_u16, 0);
      TS_ASSERT_DIFFERS(str, (s1_String*)0);
      const char* str_utf8 = s1_string_u8 (str);
      ASSERT_EQUALS_STR2_U8(str_utf8);
      const s1_char16* str_utf16 = s1_string_u16 (str);
      ASSERT_EQUALS_STR2_U16(str_utf16);
      const s1_char32* str_utf32 = s1_string_u32 (str);
      ASSERT_EQUALS_STR2_U32(str_utf32);
      const wchar_t* str_wcs = s1_string_wcs (str);
      ASSERT_EQUALS_STR2_WCS(str_wcs);
    }
    {
      s1_String* str;
      str = s1_string_create_u32 (lib, testString2_u32, 0);
      TS_ASSERT_DIFFERS(str, (s1_String*)0);
      const char* str_utf8 = s1_string_u8 (str);
      ASSERT_EQUALS_STR2_U8(str_utf8);
      const s1_char16* str_utf16 = s1_string_u16 (str);
      ASSERT_EQUALS_STR2_U16(str_utf16);
      const s1_char32* str_utf32 = s1_string_u32 (str);
      ASSERT_EQUALS_STR2_U32(str_utf32);
      const wchar_t* str_wcs = s1_string_wcs (str);
      ASSERT_EQUALS_STR2_WCS(str_wcs);
    }
    {
      s1_String* str;
      str = s1_string_create_wcs (lib, testString2_wcs, 0);
      TS_ASSERT_DIFFERS(str, (s1_String*)0);
      const char* str_utf8 = s1_string_u8 (str);
      ASSERT_EQUALS_STR2_U8(str_utf8);
      const s1_char16* str_utf16 = s1_string_u16 (str);
      ASSERT_EQUALS_STR2_U16(str_utf16);
      const s1_char32* str_utf32 = s1_string_u32 (str);
      ASSERT_EQUALS_STR2_U32(str_utf32);
      const wchar_t* str_wcs = s1_string_wcs (str);
      ASSERT_EQUALS_STR2_WCS(str_wcs);
      s1_release (str);
    }
    s1_release (lib);
  }

  void testCreateUnicode1CXX (void)
  {
    s1::Library::Pointer lib;
    s1_ResultCode err = s1::Library::Create (lib);
    TS_ASSERT(S1_SUCCESSFUL (err));
    TS_ASSERT_DIFFERS(lib, (s1::Library*)nullptr);

    {
      s1::String::Pointer str = lib->CreateString (testString2_u8);
      TS_ASSERT_DIFFERS (str, (s1::String*)nullptr);
      const char* str_utf8 = str->U8 ();
      ASSERT_EQUALS_STR2_U8(str_utf8);
      const s1_char16* str_utf16 = str->U16 ();
      ASSERT_EQUALS_STR2_U16(str_utf16);
      const s1_char32* str_utf32 = str->U32 ();
      ASSERT_EQUALS_STR2_U32(str_utf32);
      const wchar_t* str_wcs = str->WCS ();
      ASSERT_EQUALS_STR2_WCS(str_wcs);
    }
    {
      s1::String::Pointer str = lib->CreateString (testString2_u16);
      TS_ASSERT_DIFFERS (str, (s1::String*)nullptr);
      const char* str_utf8 = str->U8 ();
      ASSERT_EQUALS_STR2_U8(str_utf8);
      const s1_char16* str_utf16 = str->U16 ();
      ASSERT_EQUALS_STR2_U16(str_utf16);
      const s1_char32* str_utf32 = str->U32 ();
      ASSERT_EQUALS_STR2_U32(str_utf32);
      const wchar_t* str_wcs = str->WCS ();
      ASSERT_EQUALS_STR2_WCS(str_wcs);
    }
    {
      s1::String::Pointer str = lib->CreateString (testString2_u32);
      TS_ASSERT_DIFFERS (str, (s1::String*)nullptr);
      const char* str_utf8 = str->U8 ();
      ASSERT_EQUALS_STR2_U8(str_utf8);
      const s1_char16* str_utf16 = str->U16 ();
      ASSERT_EQUALS_STR2_U16(str_utf16);
      const s1_char32* str_utf32 = str->U32 ();
      ASSERT_EQUALS_STR2_U32(str_utf32);
      const wchar_t* str_wcs = str->WCS ();
      ASSERT_EQUALS_STR2_WCS(str_wcs);
    }
    {
      s1::String::Pointer str = lib->CreateString (testString2_wcs);
      TS_ASSERT_DIFFERS (str, (s1::String*)nullptr);
      const char* str_utf8 = str->U8 ();
      ASSERT_EQUALS_STR2_U8(str_utf8);
      const s1_char16* str_utf16 = str->U16 ();
      ASSERT_EQUALS_STR2_U16(str_utf16);
      const s1_char32* str_utf32 = str->U32 ();
      ASSERT_EQUALS_STR2_U32(str_utf32);
      const wchar_t* str_wcs = str->WCS ();
      ASSERT_EQUALS_STR2_WCS(str_wcs);
    }
  }

  void testCreateUnicode2CXX (void)
  {
    s1::Library::Pointer lib;
    s1_ResultCode err = s1::Library::Create (lib);
    TS_ASSERT(S1_SUCCESSFUL (err));
    TS_ASSERT_DIFFERS(lib, (s1::Library*)nullptr);

    {
      s1::String::Pointer str = lib->CreateString (testString3_u8);
      TS_ASSERT_DIFFERS (str, (s1::String*)nullptr);
      const char* str_utf8 = str->U8 ();
      ASSERT_EQUALS_STR3_U8(str_utf8);
      const s1_char16* str_utf16 = str->U16 ();
      ASSERT_EQUALS_STR3_U16(str_utf16);
      const s1_char32* str_utf32 = str->U32 ();
      ASSERT_EQUALS_STR3_U32(str_utf32);
    }
    {
      s1::String::Pointer str = lib->CreateString (testString3_u16);
      TS_ASSERT_DIFFERS (str, (s1::String*)nullptr);
      const char* str_utf8 = str->U8 ();
      ASSERT_EQUALS_STR3_U8(str_utf8);
      const s1_char16* str_utf16 = str->U16 ();
      ASSERT_EQUALS_STR3_U16(str_utf16);
      const s1_char32* str_utf32 = str->U32 ();
      ASSERT_EQUALS_STR3_U32(str_utf32);
    }
    {
      s1::String::Pointer str = lib->CreateString (testString3_u32);
      TS_ASSERT_DIFFERS (str, (s1::String*)nullptr);
      const char* str_utf8 = str->U8 ();
      ASSERT_EQUALS_STR3_U8(str_utf8);
      const s1_char16* str_utf16 = str->U16 ();
      ASSERT_EQUALS_STR3_U16(str_utf16);
      const s1_char32* str_utf32 = str->U32 ();
      ASSERT_EQUALS_STR3_U32(str_utf32);
    }
  }

  void testIndependentCreateNullStrC (void)
  {
    // Check object creation
    s1_String* str;
    s1_ResultCode err = s1_string_independent_create (&str, 0, 0);
    TS_ASSERT_EQUALS(str, (s1_String*)0);
    TS_ASSERT(S1_ERROR_EQUAL(err, S1_E_INVALID_ARG));
  }

  void testIndependentCreateNullStrCXX (void)
  {
    s1::String::Pointer str;
    s1_ResultCode err = s1::String::IndependentCreate (str, (const char*)nullptr);
    TS_ASSERT_EQUALS(str, (s1::String*)nullptr);
    TS_ASSERT(S1_ERROR_EQUAL(err, S1_E_INVALID_ARG));
  }

  void testIndependentCreateAsciiCXX (void)
  {
    {
      s1::String::Pointer str;
      s1_ResultCode err = s1::String::IndependentCreate (str, testString1_u8);
      TS_ASSERT_DIFFERS (str, (s1::String*)nullptr);
      const char* str_utf8 = str->U8 ();
      ASSERT_EQUALS_STR1_U8(str_utf8);
      const s1_char16* str_utf16 = str->U16 ();
      ASSERT_EQUALS_STR1_U16(str_utf16);
      const s1_char32* str_utf32 = str->U32 ();
      ASSERT_EQUALS_STR1_U32(str_utf32);
      const wchar_t* str_wcs = str->WCS ();
      ASSERT_EQUALS_STR1_WCS(str_wcs);
    }
    {
      s1::String::Pointer str;
      s1_ResultCode err = s1::String::IndependentCreate (str, testString1_u16);
      TS_ASSERT_DIFFERS (str, (s1::String*)nullptr);
      const char* str_utf8 = str->U8 ();
      ASSERT_EQUALS_STR1_U8(str_utf8);
      const s1_char16* str_utf16 = str->U16 ();
      ASSERT_EQUALS_STR1_U16(str_utf16);
      const s1_char32* str_utf32 = str->U32 ();
      ASSERT_EQUALS_STR1_U32(str_utf32);
      const wchar_t* str_wcs = str->WCS ();
      ASSERT_EQUALS_STR1_WCS(str_wcs);
    }
    {
      s1::String::Pointer str;
      s1_ResultCode err = s1::String::IndependentCreate (str, testString1_u32);
      TS_ASSERT_DIFFERS (str, (s1::String*)nullptr);
      const char* str_utf8 = str->U8 ();
      ASSERT_EQUALS_STR1_U8(str_utf8);
      const s1_char16* str_utf16 = str->U16 ();
      ASSERT_EQUALS_STR1_U16(str_utf16);
      const s1_char32* str_utf32 = str->U32 ();
      ASSERT_EQUALS_STR1_U32(str_utf32);
      const wchar_t* str_wcs = str->WCS ();
      ASSERT_EQUALS_STR1_WCS(str_wcs);
    }
    {
      s1::String::Pointer str;
      s1_ResultCode err = s1::String::IndependentCreate (str, testString1_wcs);
      TS_ASSERT_DIFFERS (str, (s1::String*)nullptr);
      const char* str_utf8 = str->U8 ();
      ASSERT_EQUALS_STR1_U8(str_utf8);
      const s1_char16* str_utf16 = str->U16 ();
      ASSERT_EQUALS_STR1_U16(str_utf16);
      const s1_char32* str_utf32 = str->U32 ();
      ASSERT_EQUALS_STR1_U32(str_utf32);
      const wchar_t* str_wcs = str->WCS ();
      ASSERT_EQUALS_STR1_WCS(str_wcs);
    }
  }

  void testIndependentCreateUnicode1C (void)
  {
    {
      s1_String* str;
      s1_ResultCode err = s1_string_independent_create (&str, testString2_u8, 0);
      TS_ASSERT_DIFFERS(str, (s1_String*)0);
      const char* str_utf8 = s1_string_u8 (str);
      ASSERT_EQUALS_STR2_U8(str_utf8);
      const s1_char16* str_utf16 = s1_string_u16 (str);
      ASSERT_EQUALS_STR2_U16(str_utf16);
      const s1_char32* str_utf32 = s1_string_u32 (str);
      ASSERT_EQUALS_STR2_U32(str_utf32);
      const wchar_t* str_wcs = s1_string_wcs (str);
      ASSERT_EQUALS_STR2_WCS(str_wcs);
      s1_release (str);
    }
    {
      s1_String* str;
      s1_ResultCode err = s1_string_independent_create_u16 (&str, testString2_u16, 0);
      TS_ASSERT_DIFFERS(str, (s1_String*)0);
      const char* str_utf8 = s1_string_u8 (str);
      ASSERT_EQUALS_STR2_U8(str_utf8);
      const s1_char16* str_utf16 = s1_string_u16 (str);
      ASSERT_EQUALS_STR2_U16(str_utf16);
      const s1_char32* str_utf32 = s1_string_u32 (str);
      ASSERT_EQUALS_STR2_U32(str_utf32);
      const wchar_t* str_wcs = s1_string_wcs (str);
      ASSERT_EQUALS_STR2_WCS(str_wcs);
    }
    {
      s1_String* str;
      s1_ResultCode err = s1_string_independent_create_u32 (&str, testString2_u32, 0);
      TS_ASSERT_DIFFERS(str, (s1_String*)0);
      const char* str_utf8 = s1_string_u8 (str);
      ASSERT_EQUALS_STR2_U8(str_utf8);
      const s1_char16* str_utf16 = s1_string_u16 (str);
      ASSERT_EQUALS_STR2_U16(str_utf16);
      const s1_char32* str_utf32 = s1_string_u32 (str);
      ASSERT_EQUALS_STR2_U32(str_utf32);
      const wchar_t* str_wcs = s1_string_wcs (str);
      ASSERT_EQUALS_STR2_WCS(str_wcs);
    }
    {
      s1_String* str;
      s1_ResultCode err = s1_string_independent_create_wcs (&str, testString2_wcs, 0);
      TS_ASSERT_DIFFERS(str, (s1_String*)0);
      const char* str_utf8 = s1_string_u8 (str);
      ASSERT_EQUALS_STR2_U8(str_utf8);
      const s1_char16* str_utf16 = s1_string_u16 (str);
      ASSERT_EQUALS_STR2_U16(str_utf16);
      const s1_char32* str_utf32 = s1_string_u32 (str);
      ASSERT_EQUALS_STR2_U32(str_utf32);
      const wchar_t* str_wcs = s1_string_wcs (str);
      ASSERT_EQUALS_STR2_WCS(str_wcs);
      s1_release (str);
    }
  }

  void testIndependentCreateUnicode1CXX (void)
  {
    {
      s1::String::Pointer str;
      s1_ResultCode err = s1::String::IndependentCreate (str, testString2_u8);
      TS_ASSERT_DIFFERS (str, (s1::String*)nullptr);
      const char* str_utf8 = str->U8 ();
      ASSERT_EQUALS_STR2_U8(str_utf8);
      const s1_char16* str_utf16 = str->U16 ();
      ASSERT_EQUALS_STR2_U16(str_utf16);
      const s1_char32* str_utf32 = str->U32 ();
      ASSERT_EQUALS_STR2_U32(str_utf32);
      const wchar_t* str_wcs = str->WCS ();
      ASSERT_EQUALS_STR2_WCS(str_wcs);
    }
    {
      s1::String::Pointer str;
      s1_ResultCode err = s1::String::IndependentCreate (str, testString2_u16);
      TS_ASSERT_DIFFERS (str, (s1::String*)nullptr);
      const char* str_utf8 = str->U8 ();
      ASSERT_EQUALS_STR2_U8(str_utf8);
      const s1_char16* str_utf16 = str->U16 ();
      ASSERT_EQUALS_STR2_U16(str_utf16);
      const s1_char32* str_utf32 = str->U32 ();
      ASSERT_EQUALS_STR2_U32(str_utf32);
      const wchar_t* str_wcs = str->WCS ();
      ASSERT_EQUALS_STR2_WCS(str_wcs);
    }
    {
      s1::String::Pointer str;
      s1_ResultCode err = s1::String::IndependentCreate (str, testString2_u32);
      TS_ASSERT_DIFFERS (str, (s1::String*)nullptr);
      const char* str_utf8 = str->U8 ();
      ASSERT_EQUALS_STR2_U8(str_utf8);
      const s1_char16* str_utf16 = str->U16 ();
      ASSERT_EQUALS_STR2_U16(str_utf16);
      const s1_char32* str_utf32 = str->U32 ();
      ASSERT_EQUALS_STR2_U32(str_utf32);
      const wchar_t* str_wcs = str->WCS ();
      ASSERT_EQUALS_STR2_WCS(str_wcs);
    }
    {
      s1::String::Pointer str;
      s1_ResultCode err = s1::String::IndependentCreate (str, testString2_wcs);
      TS_ASSERT_DIFFERS (str, (s1::String*)nullptr);
      const char* str_utf8 = str->U8 ();
      ASSERT_EQUALS_STR2_U8(str_utf8);
      const s1_char16* str_utf16 = str->U16 ();
      ASSERT_EQUALS_STR2_U16(str_utf16);
      const s1_char32* str_utf32 = str->U32 ();
      ASSERT_EQUALS_STR2_U32(str_utf32);
      const wchar_t* str_wcs = str->WCS ();
      ASSERT_EQUALS_STR2_WCS(str_wcs);
    }
  }

  void testIndependentCreateUnicode2CXX (void)
  {
    {
      s1::String::Pointer str;
      s1_ResultCode err = s1::String::IndependentCreate (str, testString3_u8);
      TS_ASSERT_DIFFERS (str, (s1::String*)nullptr);
      const char* str_utf8 = str->U8 ();
      ASSERT_EQUALS_STR3_U8(str_utf8);
      const s1_char16* str_utf16 = str->U16 ();
      ASSERT_EQUALS_STR3_U16(str_utf16);
      const s1_char32* str_utf32 = str->U32 ();
      ASSERT_EQUALS_STR3_U32(str_utf32);
    }
    {
      s1::String::Pointer str;
      s1_ResultCode err = s1::String::IndependentCreate (str, testString3_u16);
      TS_ASSERT_DIFFERS (str, (s1::String*)nullptr);
      const char* str_utf8 = str->U8 ();
      ASSERT_EQUALS_STR3_U8(str_utf8);
      const s1_char16* str_utf16 = str->U16 ();
      ASSERT_EQUALS_STR3_U16(str_utf16);
      const s1_char32* str_utf32 = str->U32 ();
      ASSERT_EQUALS_STR3_U32(str_utf32);
    }
    {
      s1::String::Pointer str;
      s1_ResultCode err = s1::String::IndependentCreate (str, testString3_u32);
      TS_ASSERT_DIFFERS (str, (s1::String*)nullptr);
      const char* str_utf8 = str->U8 ();
      ASSERT_EQUALS_STR3_U8(str_utf8);
      const s1_char16* str_utf16 = str->U16 ();
      ASSERT_EQUALS_STR3_U16(str_utf16);
      const s1_char32* str_utf32 = str->U32 ();
      ASSERT_EQUALS_STR3_U32(str_utf32);
    }
  }

};
