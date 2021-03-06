/*
    Shader1
    Copyright (c) 2017-2018 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#include <boost/test/unit_test.hpp>

#include "s1/Library.h"
#include "s1/String.h"

BOOST_AUTO_TEST_SUITE(ApiString)

static const char testString1_u8[] = { 'H', 'e', 'l', 'l', 'o', 0 };
static const s1_char16 testString1_u16[] = { 'H', 'e', 'l', 'l', 'o', 0 };
static const s1_char32 testString1_u32[] = { 'H', 'e', 'l', 'l', 'o', 0 };
static const wchar_t testString1_wcs[] = { 'H', 'e', 'l', 'l', 'o', 0 };

#define CHECK_EQUAL_STR1_U8(strData)                                     \
  BOOST_CHECK_EQUAL(std::char_traits<char>::length ((strData)), 5u);     \
  BOOST_CHECK_EQUAL((strData)[0], testString1_u8[0]);                    \
  BOOST_CHECK_EQUAL((strData)[1], testString1_u8[1]);                    \
  BOOST_CHECK_EQUAL((strData)[2], testString1_u8[2]);                    \
  BOOST_CHECK_EQUAL((strData)[3], testString1_u8[3]);                    \
  BOOST_CHECK_EQUAL((strData)[4], testString1_u8[4]);                    \
  BOOST_CHECK_EQUAL((strData)[5], testString1_u8[5]);

#define CHECK_EQUAL_STR1_U16(strData)                                    \
  BOOST_CHECK_EQUAL(std::char_traits<s1_char16>::length ((strData)), 5u);\
  BOOST_CHECK_EQUAL((strData)[0], testString1_u16[0]);                   \
  BOOST_CHECK_EQUAL((strData)[1], testString1_u16[1]);                   \
  BOOST_CHECK_EQUAL((strData)[2], testString1_u16[2]);                   \
  BOOST_CHECK_EQUAL((strData)[3], testString1_u16[3]);                   \
  BOOST_CHECK_EQUAL((strData)[4], testString1_u16[4]);                   \
  BOOST_CHECK_EQUAL((strData)[5], testString1_u16[5]);

#define CHECK_EQUAL_STR1_U32(strData)                                    \
  BOOST_CHECK_EQUAL(std::char_traits<s1_char32>::length ((strData)), 5u);\
  BOOST_CHECK_EQUAL((strData)[0], testString1_u32[0]);                   \
  BOOST_CHECK_EQUAL((strData)[1], testString1_u32[1]);                   \
  BOOST_CHECK_EQUAL((strData)[2], testString1_u32[2]);                   \
  BOOST_CHECK_EQUAL((strData)[3], testString1_u32[3]);                   \
  BOOST_CHECK_EQUAL((strData)[4], testString1_u32[4]);                   \
  BOOST_CHECK_EQUAL((strData)[5], testString1_u32[5]);

#define CHECK_EQUAL_STR1_WCS(strData)                                     \
  BOOST_CHECK_EQUAL(std::char_traits<wchar_t>::length ((strData)), 5u);   \
  BOOST_CHECK_EQUAL((strData)[0], testString1_wcs[0]);                    \
  BOOST_CHECK_EQUAL((strData)[1], testString1_wcs[1]);                    \
  BOOST_CHECK_EQUAL((strData)[2], testString1_wcs[2]);                    \
  BOOST_CHECK_EQUAL((strData)[3], testString1_wcs[3]);                    \
  BOOST_CHECK_EQUAL((strData)[4], testString1_wcs[4]);                    \
  BOOST_CHECK_EQUAL((strData)[5], testString1_wcs[5]);

static const char testString2_u8[] = { 'H', (char)0xc3, (char)0xab, 'l', 'l', 'o', 0 };
static const s1_char16 testString2_u16[] = { 'H', 0xeb, 'l', 'l', 'o', 0 };
static const s1_char32 testString2_u32[] = { 'H', 0xeb, 'l', 'l', 'o', 0 };
static const wchar_t testString2_wcs[] = { 'H', 0xeb, 'l', 'l', 'o', 0 };

#define CHECK_EQUAL_STR2_U8(strData)                                     \
  BOOST_CHECK_EQUAL(std::char_traits<char>::length ((strData)), 6u);     \
  BOOST_CHECK_EQUAL((strData)[0], testString2_u8[0]);                    \
  BOOST_CHECK_EQUAL((strData)[1], testString2_u8[1]);                    \
  BOOST_CHECK_EQUAL((strData)[2], testString2_u8[2]);                    \
  BOOST_CHECK_EQUAL((strData)[3], testString2_u8[3]);                    \
  BOOST_CHECK_EQUAL((strData)[4], testString2_u8[4]);                    \
  BOOST_CHECK_EQUAL((strData)[5], testString2_u8[5]);                    \
  BOOST_CHECK_EQUAL((strData)[6], testString2_u8[6]);

#define CHECK_EQUAL_STR2_U16(strData)                                    \
  BOOST_CHECK_EQUAL(std::char_traits<s1_char16>::length ((strData)), 5u);\
  BOOST_CHECK_EQUAL((strData)[0], testString2_u16[0]);                   \
  BOOST_CHECK_EQUAL((strData)[1], testString2_u16[1]);                   \
  BOOST_CHECK_EQUAL((strData)[2], testString2_u16[2]);                   \
  BOOST_CHECK_EQUAL((strData)[3], testString2_u16[3]);                   \
  BOOST_CHECK_EQUAL((strData)[4], testString2_u16[4]);                   \
  BOOST_CHECK_EQUAL((strData)[5], testString2_u16[5]);

#define CHECK_EQUAL_STR2_U32(strData)                                    \
  BOOST_CHECK_EQUAL(std::char_traits<s1_char32>::length ((strData)), 5u);\
  BOOST_CHECK_EQUAL((strData)[0], testString2_u32[0]);                   \
  BOOST_CHECK_EQUAL((strData)[1], testString2_u32[1]);                   \
  BOOST_CHECK_EQUAL((strData)[2], testString2_u32[2]);                   \
  BOOST_CHECK_EQUAL((strData)[3], testString2_u32[3]);                   \
  BOOST_CHECK_EQUAL((strData)[4], testString2_u32[4]);                   \
  BOOST_CHECK_EQUAL((strData)[5], testString2_u32[5]);

#define CHECK_EQUAL_STR2_WCS(strData)                                    \
  BOOST_CHECK_EQUAL(std::char_traits<wchar_t>::length ((strData)), 5u);  \
  BOOST_CHECK_EQUAL((strData)[0], testString2_wcs[0]);                   \
  BOOST_CHECK_EQUAL((strData)[1], testString2_wcs[1]);                   \
  BOOST_CHECK_EQUAL((strData)[2], testString2_wcs[2]);                   \
  BOOST_CHECK_EQUAL((strData)[3], testString2_wcs[3]);                   \
  BOOST_CHECK_EQUAL((strData)[4], testString2_wcs[4]);                   \
  BOOST_CHECK_EQUAL((strData)[5], testString2_wcs[5]);

static const char testString3_u8[] = { (char)0xf0, (char)0x9f, (char)0x90, (char)0xb5, 0 };
static const s1_char16 testString3_u16[] = { 0xd83d, 0xdc35, 0 };
static const s1_char32 testString3_u32[] = { 0x1F435, 0 };

#define CHECK_EQUAL_STR3_U8(strData)                                     \
  BOOST_CHECK_EQUAL(std::char_traits<char>::length ((strData)), 4u);     \
  BOOST_CHECK_EQUAL((strData)[0], testString3_u8[0]);                    \
  BOOST_CHECK_EQUAL((strData)[1], testString3_u8[1]);                    \
  BOOST_CHECK_EQUAL((strData)[2], testString3_u8[2]);                    \
  BOOST_CHECK_EQUAL((strData)[3], testString3_u8[3]);                    \
  BOOST_CHECK_EQUAL((strData)[4], testString3_u8[4]);

#define CHECK_EQUAL_STR3_U16(strData)                                    \
  BOOST_CHECK_EQUAL(std::char_traits<s1_char16>::length ((strData)), 2u);\
  BOOST_CHECK_EQUAL((strData)[0], testString3_u16[0]);                   \
  BOOST_CHECK_EQUAL((strData)[1], testString3_u16[1]);                   \
  BOOST_CHECK_EQUAL((strData)[2], testString3_u16[2]);

#define CHECK_EQUAL_STR3_U32(strData)                                   \
  BOOST_CHECK_EQUAL(std::char_traits<s1_char32>::length ((strData)), 1u);\
  BOOST_CHECK_EQUAL((strData)[0], testString3_u32[0]);                   \
  BOOST_CHECK_EQUAL((strData)[1], testString3_u32[1]);

BOOST_AUTO_TEST_CASE(CreateStrErrorC)
{
  // Check object creation
  s1_Library* lib;
  s1_ResultCode err = s1_create_library (&lib);
  BOOST_CHECK(S1_SUCCESSFUL(err));
  BOOST_CHECK_NE(lib, (s1_Library*)0);

  s1_String* str;
  str = s1_string_create_u8 (lib, 0, 0);
  BOOST_CHECK_EQUAL(str, (s1_String*)0);
  BOOST_CHECK(S1_ERROR_EQUAL(s1_library_get_last_error (lib), S1_E_INVALID_ARG));

  str = s1_string_create (lib, s1_u8 (0));
  BOOST_CHECK_EQUAL(str, (s1_String*)0);
  BOOST_CHECK(S1_ERROR_EQUAL(s1_library_get_last_error (lib), S1_E_INVALID_ARG));

  str = s1_string_create (lib, s1_u8_n ("", size_t (S1_STRING_ARG_MAX_N) + 1));
  BOOST_CHECK_EQUAL(str, (s1_String*)0);
  BOOST_CHECK(S1_ERROR_EQUAL(s1_library_get_last_error (lib), S1_E_STRING_TOO_LONG));

  s1_release (lib);
}

BOOST_AUTO_TEST_CASE(CreateStrErrorCXX)
{
  s1::Library::Pointer lib;
  s1_ResultCode err = s1::Library::Create (lib);
  BOOST_CHECK(S1_SUCCESSFUL(err));
  BOOST_CHECK_NE(lib, (s1::Library*)nullptr);
  s1::String::Pointer str = lib->CreateString ((const char*)nullptr);
  BOOST_CHECK_EQUAL(str, (s1::String*)nullptr);
  BOOST_CHECK(S1_ERROR_EQUAL(lib->GetLastError(), S1_E_INVALID_ARG));

  str = lib->CreateString (s1::u8 (nullptr));
  BOOST_CHECK_EQUAL(str, (s1::String*)nullptr);
  BOOST_CHECK(S1_ERROR_EQUAL(lib->GetLastError(), S1_E_INVALID_ARG));

  str = lib->CreateString (s1::u8 ("", size_t (S1_STRING_ARG_MAX_N) + 1));
  BOOST_CHECK_EQUAL(str, (s1::String*)nullptr);
  BOOST_CHECK(S1_ERROR_EQUAL(lib->GetLastError(), S1_E_STRING_TOO_LONG));
}

BOOST_AUTO_TEST_CASE(CreateAsciiCXX)
{
  s1::Library::Pointer lib;
  s1_ResultCode err = s1::Library::Create (lib);
  BOOST_CHECK(S1_SUCCESSFUL (err));
  BOOST_CHECK_NE(lib, (s1::Library*)nullptr);

  {
    s1::String::Pointer str = lib->CreateString (testString1_u8);
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR1_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR1_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR1_U32(str_utf32);
    const wchar_t* str_wcs = str->WCS ();
    CHECK_EQUAL_STR1_WCS(str_wcs);
  }
  {
    s1::String::Pointer str = lib->CreateString (testString1_u16);
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR1_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR1_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR1_U32(str_utf32);
    const wchar_t* str_wcs = str->WCS ();
    CHECK_EQUAL_STR1_WCS(str_wcs);
  }
  {
    s1::String::Pointer str = lib->CreateString (testString1_u32);
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR1_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR1_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR1_U32(str_utf32);
    const wchar_t* str_wcs = str->WCS ();
    CHECK_EQUAL_STR1_WCS(str_wcs);
  }
  {
    s1::String::Pointer str = lib->CreateString (testString1_wcs);
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR1_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR1_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR1_U32(str_utf32);
    const wchar_t* str_wcs = str->WCS ();
    CHECK_EQUAL_STR1_WCS(str_wcs);
  }

  {
    s1::String::Pointer str = lib->CreateString (s1::u8 (testString1_u8));
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR1_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR1_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR1_U32(str_utf32);
    const wchar_t* str_wcs = str->WCS ();
    CHECK_EQUAL_STR1_WCS(str_wcs);
  }
  {
    s1::String::Pointer str = lib->CreateString (s1::u16 (testString1_u16));
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR1_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR1_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR1_U32(str_utf32);
    const wchar_t* str_wcs = str->WCS ();
    CHECK_EQUAL_STR1_WCS(str_wcs);
  }
  {
    s1::String::Pointer str = lib->CreateString (s1::u32 (testString1_u32));
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR1_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR1_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR1_U32(str_utf32);
    const wchar_t* str_wcs = str->WCS ();
    CHECK_EQUAL_STR1_WCS(str_wcs);
  }
  {
    s1::String::Pointer str = lib->CreateString (s1::wcs (testString1_wcs));
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR1_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR1_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR1_U32(str_utf32);
    const wchar_t* str_wcs = str->WCS ();
    CHECK_EQUAL_STR1_WCS(str_wcs);
  }
}

BOOST_AUTO_TEST_CASE(CreateUnicode1C)
{
  s1_Library* lib;
  s1_ResultCode err = s1_create_library (&lib);
  BOOST_CHECK(S1_SUCCESSFUL(err));
  BOOST_CHECK_NE(lib, (s1_Library*)0);

  {
    s1_String* str;
    str = s1_string_create_u8 (lib, testString2_u8, 0);
    BOOST_CHECK_NE(str, (s1_String*)0);
    const char* str_utf8 = s1_string_u8 (str);
    CHECK_EQUAL_STR2_U8(str_utf8);
    const s1_char16* str_utf16 = s1_string_u16 (str);
    CHECK_EQUAL_STR2_U16(str_utf16);
    const s1_char32* str_utf32 = s1_string_u32 (str);
    CHECK_EQUAL_STR2_U32(str_utf32);
    const wchar_t* str_wcs = s1_string_wcs (str);
    CHECK_EQUAL_STR2_WCS(str_wcs);
    s1_release (str);
  }
  {
    s1_String* str;
    str = s1_string_create_u16 (lib, testString2_u16, 0);
    BOOST_CHECK_NE(str, (s1_String*)0);
    const char* str_utf8 = s1_string_u8 (str);
    CHECK_EQUAL_STR2_U8(str_utf8);
    const s1_char16* str_utf16 = s1_string_u16 (str);
    CHECK_EQUAL_STR2_U16(str_utf16);
    const s1_char32* str_utf32 = s1_string_u32 (str);
    CHECK_EQUAL_STR2_U32(str_utf32);
    const wchar_t* str_wcs = s1_string_wcs (str);
    CHECK_EQUAL_STR2_WCS(str_wcs);
    s1_release (str);
  }
  {
    s1_String* str;
    str = s1_string_create_u32 (lib, testString2_u32, 0);
    BOOST_CHECK_NE(str, (s1_String*)0);
    const char* str_utf8 = s1_string_u8 (str);
    CHECK_EQUAL_STR2_U8(str_utf8);
    const s1_char16* str_utf16 = s1_string_u16 (str);
    CHECK_EQUAL_STR2_U16(str_utf16);
    const s1_char32* str_utf32 = s1_string_u32 (str);
    CHECK_EQUAL_STR2_U32(str_utf32);
    const wchar_t* str_wcs = s1_string_wcs (str);
    CHECK_EQUAL_STR2_WCS(str_wcs);
    s1_release (str);
  }
  {
    s1_String* str;
    str = s1_string_create_wcs (lib, testString2_wcs, 0);
    BOOST_CHECK_NE(str, (s1_String*)0);
    const char* str_utf8 = s1_string_u8 (str);
    CHECK_EQUAL_STR2_U8(str_utf8);
    const s1_char16* str_utf16 = s1_string_u16 (str);
    CHECK_EQUAL_STR2_U16(str_utf16);
    const s1_char32* str_utf32 = s1_string_u32 (str);
    CHECK_EQUAL_STR2_U32(str_utf32);
    const wchar_t* str_wcs = s1_string_wcs (str);
    CHECK_EQUAL_STR2_WCS(str_wcs);
    s1_release (str);
  }


  {
    s1_String* str;
    str = s1_string_create (lib, s1_u8 (testString2_u8), 0);
    BOOST_CHECK_NE(str, (s1_String*)0);
    const char* str_utf8 = s1_string_u8 (str);
    CHECK_EQUAL_STR2_U8(str_utf8);
    const s1_char16* str_utf16 = s1_string_u16 (str);
    CHECK_EQUAL_STR2_U16(str_utf16);
    const s1_char32* str_utf32 = s1_string_u32 (str);
    CHECK_EQUAL_STR2_U32(str_utf32);
    const wchar_t* str_wcs = s1_string_wcs (str);
    CHECK_EQUAL_STR2_WCS(str_wcs);
    s1_release (str);
  }
  {
    s1_String* str;
    str = s1_string_create (lib, s1_u16 (testString2_u16), 0);
    BOOST_CHECK_NE(str, (s1_String*)0);
    const char* str_utf8 = s1_string_u8 (str);
    CHECK_EQUAL_STR2_U8(str_utf8);
    const s1_char16* str_utf16 = s1_string_u16 (str);
    CHECK_EQUAL_STR2_U16(str_utf16);
    const s1_char32* str_utf32 = s1_string_u32 (str);
    CHECK_EQUAL_STR2_U32(str_utf32);
    const wchar_t* str_wcs = s1_string_wcs (str);
    CHECK_EQUAL_STR2_WCS(str_wcs);
    s1_release (str);
  }
  {
    s1_String* str;
    str = s1_string_create (lib, s1_u32 (testString2_u32), 0);
    BOOST_CHECK_NE(str, (s1_String*)0);
    const char* str_utf8 = s1_string_u8 (str);
    CHECK_EQUAL_STR2_U8(str_utf8);
    const s1_char16* str_utf16 = s1_string_u16 (str);
    CHECK_EQUAL_STR2_U16(str_utf16);
    const s1_char32* str_utf32 = s1_string_u32 (str);
    CHECK_EQUAL_STR2_U32(str_utf32);
    const wchar_t* str_wcs = s1_string_wcs (str);
    CHECK_EQUAL_STR2_WCS(str_wcs);
    s1_release (str);
  }
  {
    s1_String* str;
    str = s1_string_create (lib, s1_wcs (testString2_wcs), 0);
    BOOST_CHECK_NE(str, (s1_String*)0);
    const char* str_utf8 = s1_string_u8 (str);
    CHECK_EQUAL_STR2_U8(str_utf8);
    const s1_char16* str_utf16 = s1_string_u16 (str);
    CHECK_EQUAL_STR2_U16(str_utf16);
    const s1_char32* str_utf32 = s1_string_u32 (str);
    CHECK_EQUAL_STR2_U32(str_utf32);
    const wchar_t* str_wcs = s1_string_wcs (str);
    CHECK_EQUAL_STR2_WCS(str_wcs);
    s1_release (str);
  }

  s1_release (lib);
}

BOOST_AUTO_TEST_CASE(CreateUnicode1CXX)
{
  s1::Library::Pointer lib;
  s1_ResultCode err = s1::Library::Create (lib);
  BOOST_CHECK(S1_SUCCESSFUL (err));
  BOOST_CHECK_NE(lib, (s1::Library*)nullptr);

  {
    s1::String::Pointer str = lib->CreateString (testString2_u8);
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR2_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR2_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR2_U32(str_utf32);
    const wchar_t* str_wcs = str->WCS ();
    CHECK_EQUAL_STR2_WCS(str_wcs);
  }
  {
    s1::String::Pointer str = lib->CreateString (testString2_u16);
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR2_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR2_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR2_U32(str_utf32);
    const wchar_t* str_wcs = str->WCS ();
    CHECK_EQUAL_STR2_WCS(str_wcs);
  }
  {
    s1::String::Pointer str = lib->CreateString (testString2_u32);
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR2_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR2_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR2_U32(str_utf32);
    const wchar_t* str_wcs = str->WCS ();
    CHECK_EQUAL_STR2_WCS(str_wcs);
  }
  {
    s1::String::Pointer str = lib->CreateString (testString2_wcs);
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR2_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR2_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR2_U32(str_utf32);
    const wchar_t* str_wcs = str->WCS ();
    CHECK_EQUAL_STR2_WCS(str_wcs);
  }

  {
    s1::String::Pointer str = lib->CreateString (s1::u8 (testString2_u8));
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR2_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR2_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR2_U32(str_utf32);
    const wchar_t* str_wcs = str->WCS ();
    CHECK_EQUAL_STR2_WCS(str_wcs);
  }
  {
    s1::String::Pointer str = lib->CreateString (s1::u16 (testString2_u16));
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR2_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR2_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR2_U32(str_utf32);
    const wchar_t* str_wcs = str->WCS ();
    CHECK_EQUAL_STR2_WCS(str_wcs);
  }
  {
    s1::String::Pointer str = lib->CreateString (s1::u32 (testString2_u32));
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR2_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR2_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR2_U32(str_utf32);
    const wchar_t* str_wcs = str->WCS ();
    CHECK_EQUAL_STR2_WCS(str_wcs);
  }
  {
    s1::String::Pointer str = lib->CreateString (s1::wcs (testString2_wcs));
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR2_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR2_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR2_U32(str_utf32);
    const wchar_t* str_wcs = str->WCS ();
    CHECK_EQUAL_STR2_WCS(str_wcs);
  }
}

BOOST_AUTO_TEST_CASE(CreateUnicode2CXX)
{
  s1::Library::Pointer lib;
  s1_ResultCode err = s1::Library::Create (lib);
  BOOST_CHECK(S1_SUCCESSFUL (err));
  BOOST_CHECK_NE(lib, (s1::Library*)nullptr);

  {
    s1::String::Pointer str = lib->CreateString (testString3_u8);
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR3_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR3_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR3_U32(str_utf32);
  }
  {
    s1::String::Pointer str = lib->CreateString (testString3_u16);
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR3_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR3_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR3_U32(str_utf32);
  }
  {
    s1::String::Pointer str = lib->CreateString (testString3_u32);
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR3_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR3_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR3_U32(str_utf32);
  }

  {
    s1::String::Pointer str = lib->CreateString (s1::u8 (testString3_u8));
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR3_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR3_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR3_U32(str_utf32);
  }
  {
    s1::String::Pointer str = lib->CreateString (s1::u16 (testString3_u16));
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR3_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR3_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR3_U32(str_utf32);
  }
  {
    s1::String::Pointer str = lib->CreateString (s1::u32 (testString3_u32));
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR3_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR3_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR3_U32(str_utf32);
  }
}

BOOST_AUTO_TEST_CASE(IndependenttestCreateStrErrorC)
{
  // Check object creation
  s1_String* str;
  s1_ResultCode err = s1_string_independent_create_u8 (&str, 0, 0);
  BOOST_CHECK_EQUAL(str, (s1_String*)0);
  BOOST_CHECK(S1_ERROR_EQUAL(err, S1_E_INVALID_ARG));

  err = s1_string_independent_create (&str, s1_u8 (0));
  BOOST_CHECK_EQUAL(str, (s1_String*)0);
  BOOST_CHECK(S1_ERROR_EQUAL(err, S1_E_INVALID_ARG));

  err = s1_string_independent_create (&str, s1_u8_n ("", size_t (S1_STRING_ARG_MAX_N) + 1));
  BOOST_CHECK_EQUAL(str, (s1_String*)0);
  BOOST_CHECK(S1_ERROR_EQUAL(err, S1_E_STRING_TOO_LONG));
}

BOOST_AUTO_TEST_CASE(IndependenttestCreateStrErrorCXX)
{
  s1::String::Pointer str;
  s1_ResultCode err = s1::String::IndependentCreate (str, (const char*)nullptr);
  BOOST_CHECK_EQUAL(str, (s1::String*)nullptr);
  BOOST_CHECK(S1_ERROR_EQUAL(err, S1_E_INVALID_ARG));

  err = s1::String::IndependentCreate (str, s1::u8 (nullptr));
  BOOST_CHECK_EQUAL(str, (s1::String*)nullptr);
  BOOST_CHECK(S1_ERROR_EQUAL(err, S1_E_INVALID_ARG));

  err = s1::String::IndependentCreate (str, s1::u8 ("", size_t (S1_STRING_ARG_MAX_N) + 1));
  BOOST_CHECK_EQUAL(str, (s1::String*)nullptr);
  BOOST_CHECK(S1_ERROR_EQUAL(err, S1_E_STRING_TOO_LONG));
}

BOOST_AUTO_TEST_CASE(IndependentCreateAsciiCXX)
{
  {
    s1::String::Pointer str;
    s1_ResultCode err = s1::String::IndependentCreate (str, testString1_u8);
    BOOST_CHECK(S1_SUCCESSFUL(err));
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR1_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR1_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR1_U32(str_utf32);
    const wchar_t* str_wcs = str->WCS ();
    CHECK_EQUAL_STR1_WCS(str_wcs);
  }
  {
    s1::String::Pointer str;
    s1_ResultCode err = s1::String::IndependentCreate (str, testString1_u16);
    BOOST_CHECK(S1_SUCCESSFUL(err));
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR1_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR1_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR1_U32(str_utf32);
    const wchar_t* str_wcs = str->WCS ();
    CHECK_EQUAL_STR1_WCS(str_wcs);
  }
  {
    s1::String::Pointer str;
    s1_ResultCode err = s1::String::IndependentCreate (str, testString1_u32);
    BOOST_CHECK(S1_SUCCESSFUL(err));
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR1_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR1_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR1_U32(str_utf32);
    const wchar_t* str_wcs = str->WCS ();
    CHECK_EQUAL_STR1_WCS(str_wcs);
  }
  {
    s1::String::Pointer str;
    s1_ResultCode err = s1::String::IndependentCreate (str, testString1_wcs);
    BOOST_CHECK(S1_SUCCESSFUL(err));
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR1_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR1_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR1_U32(str_utf32);
    const wchar_t* str_wcs = str->WCS ();
    CHECK_EQUAL_STR1_WCS(str_wcs);
  }

  {
    s1::String::Pointer str;
    s1_ResultCode err = s1::String::IndependentCreate (str, s1::u8 (testString1_u8));
    BOOST_CHECK(S1_SUCCESSFUL(err));
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR1_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR1_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR1_U32(str_utf32);
    const wchar_t* str_wcs = str->WCS ();
    CHECK_EQUAL_STR1_WCS(str_wcs);
  }
  {
    s1::String::Pointer str;
    s1_ResultCode err = s1::String::IndependentCreate (str, s1::u16 (testString1_u16));
    BOOST_CHECK(S1_SUCCESSFUL(err));
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR1_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR1_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR1_U32(str_utf32);
    const wchar_t* str_wcs = str->WCS ();
    CHECK_EQUAL_STR1_WCS(str_wcs);
  }
  {
    s1::String::Pointer str;
    s1_ResultCode err = s1::String::IndependentCreate (str, s1::u32 (testString1_u32));
    BOOST_CHECK(S1_SUCCESSFUL(err));
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR1_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR1_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR1_U32(str_utf32);
    const wchar_t* str_wcs = str->WCS ();
    CHECK_EQUAL_STR1_WCS(str_wcs);
  }
  {
    s1::String::Pointer str;
    s1_ResultCode err = s1::String::IndependentCreate (str, s1::wcs (testString1_wcs));
    BOOST_CHECK(S1_SUCCESSFUL(err));
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR1_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR1_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR1_U32(str_utf32);
    const wchar_t* str_wcs = str->WCS ();
    CHECK_EQUAL_STR1_WCS(str_wcs);
  }
}

BOOST_AUTO_TEST_CASE(IndependentCreateUnicode1C)
{
  {
    s1_String* str;
    s1_ResultCode err = s1_string_independent_create_u8 (&str, testString2_u8, 0);
    BOOST_CHECK(S1_SUCCESSFUL(err));
    BOOST_CHECK_NE(str, (s1_String*)0);
    const char* str_utf8 = s1_string_u8 (str);
    CHECK_EQUAL_STR2_U8(str_utf8);
    const s1_char16* str_utf16 = s1_string_u16 (str);
    CHECK_EQUAL_STR2_U16(str_utf16);
    const s1_char32* str_utf32 = s1_string_u32 (str);
    CHECK_EQUAL_STR2_U32(str_utf32);
    const wchar_t* str_wcs = s1_string_wcs (str);
    CHECK_EQUAL_STR2_WCS(str_wcs);
    s1_release (str);
  }
  {
    s1_String* str;
    s1_ResultCode err = s1_string_independent_create_u16 (&str, testString2_u16, 0);
    BOOST_CHECK(S1_SUCCESSFUL(err));
    BOOST_CHECK_NE(str, (s1_String*)0);
    const char* str_utf8 = s1_string_u8 (str);
    CHECK_EQUAL_STR2_U8(str_utf8);
    const s1_char16* str_utf16 = s1_string_u16 (str);
    CHECK_EQUAL_STR2_U16(str_utf16);
    const s1_char32* str_utf32 = s1_string_u32 (str);
    CHECK_EQUAL_STR2_U32(str_utf32);
    const wchar_t* str_wcs = s1_string_wcs (str);
    CHECK_EQUAL_STR2_WCS(str_wcs);
    s1_release (str);
  }
  {
    s1_String* str;
    s1_ResultCode err = s1_string_independent_create_u32 (&str, testString2_u32, 0);
    BOOST_CHECK(S1_SUCCESSFUL(err));
    BOOST_CHECK_NE(str, (s1_String*)0);
    const char* str_utf8 = s1_string_u8 (str);
    CHECK_EQUAL_STR2_U8(str_utf8);
    const s1_char16* str_utf16 = s1_string_u16 (str);
    CHECK_EQUAL_STR2_U16(str_utf16);
    const s1_char32* str_utf32 = s1_string_u32 (str);
    CHECK_EQUAL_STR2_U32(str_utf32);
    const wchar_t* str_wcs = s1_string_wcs (str);
    CHECK_EQUAL_STR2_WCS(str_wcs);
    s1_release (str);
  }
  {
    s1_String* str;
    s1_ResultCode err = s1_string_independent_create_wcs (&str, testString2_wcs, 0);
    BOOST_CHECK(S1_SUCCESSFUL(err));
    BOOST_CHECK_NE(str, (s1_String*)0);
    const char* str_utf8 = s1_string_u8 (str);
    CHECK_EQUAL_STR2_U8(str_utf8);
    const s1_char16* str_utf16 = s1_string_u16 (str);
    CHECK_EQUAL_STR2_U16(str_utf16);
    const s1_char32* str_utf32 = s1_string_u32 (str);
    CHECK_EQUAL_STR2_U32(str_utf32);
    const wchar_t* str_wcs = s1_string_wcs (str);
    CHECK_EQUAL_STR2_WCS(str_wcs);
    s1_release (str);
  }
}

BOOST_AUTO_TEST_CASE(IndependentCreateUnicode1CXX)
{
  {
    s1::String::Pointer str;
    s1_ResultCode err = s1::String::IndependentCreate (str, testString2_u8);
    BOOST_CHECK(S1_SUCCESSFUL(err));
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR2_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR2_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR2_U32(str_utf32);
    const wchar_t* str_wcs = str->WCS ();
    CHECK_EQUAL_STR2_WCS(str_wcs);
  }
  {
    s1::String::Pointer str;
    s1_ResultCode err = s1::String::IndependentCreate (str, testString2_u16);
    BOOST_CHECK(S1_SUCCESSFUL(err));
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR2_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR2_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR2_U32(str_utf32);
    const wchar_t* str_wcs = str->WCS ();
    CHECK_EQUAL_STR2_WCS(str_wcs);
  }
  {
    s1::String::Pointer str;
    s1_ResultCode err = s1::String::IndependentCreate (str, testString2_u32);
    BOOST_CHECK(S1_SUCCESSFUL(err));
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR2_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR2_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR2_U32(str_utf32);
    const wchar_t* str_wcs = str->WCS ();
    CHECK_EQUAL_STR2_WCS(str_wcs);
  }
  {
    s1::String::Pointer str;
    s1_ResultCode err = s1::String::IndependentCreate (str, testString2_wcs);
    BOOST_CHECK(S1_SUCCESSFUL(err));
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR2_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR2_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR2_U32(str_utf32);
    const wchar_t* str_wcs = str->WCS ();
    CHECK_EQUAL_STR2_WCS(str_wcs);
  }

  {
    s1::String::Pointer str;
    s1_ResultCode err = s1::String::IndependentCreate (str, s1::u8 (testString2_u8));
    BOOST_CHECK(S1_SUCCESSFUL(err));
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR2_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR2_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR2_U32(str_utf32);
    const wchar_t* str_wcs = str->WCS ();
    CHECK_EQUAL_STR2_WCS(str_wcs);
  }
  {
    s1::String::Pointer str;
    s1_ResultCode err = s1::String::IndependentCreate (str, s1::u16 (testString2_u16));
    BOOST_CHECK(S1_SUCCESSFUL(err));
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR2_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR2_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR2_U32(str_utf32);
    const wchar_t* str_wcs = str->WCS ();
    CHECK_EQUAL_STR2_WCS(str_wcs);
  }
  {
    s1::String::Pointer str;
    s1_ResultCode err = s1::String::IndependentCreate (str, s1::u32 (testString2_u32));
    BOOST_CHECK(S1_SUCCESSFUL(err));
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR2_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR2_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR2_U32(str_utf32);
    const wchar_t* str_wcs = str->WCS ();
    CHECK_EQUAL_STR2_WCS(str_wcs);
  }
  {
    s1::String::Pointer str;
    s1_ResultCode err = s1::String::IndependentCreate (str, s1::wcs (testString2_wcs));
    BOOST_CHECK(S1_SUCCESSFUL(err));
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR2_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR2_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR2_U32(str_utf32);
    const wchar_t* str_wcs = str->WCS ();
    CHECK_EQUAL_STR2_WCS(str_wcs);
  }
}

BOOST_AUTO_TEST_CASE(IndependentCreateUnicode2CXX)
{
  {
    s1::String::Pointer str;
    s1_ResultCode err = s1::String::IndependentCreate (str, testString3_u8);
    BOOST_CHECK(S1_SUCCESSFUL(err));
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR3_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR3_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR3_U32(str_utf32);
  }
  {
    s1::String::Pointer str;
    s1_ResultCode err = s1::String::IndependentCreate (str, testString3_u16);
    BOOST_CHECK(S1_SUCCESSFUL(err));
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR3_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR3_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR3_U32(str_utf32);
  }
  {
    s1::String::Pointer str;
    s1_ResultCode err = s1::String::IndependentCreate (str, testString3_u32);
    BOOST_CHECK(S1_SUCCESSFUL(err));
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR3_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR3_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR3_U32(str_utf32);
  }

  {
    s1::String::Pointer str;
    s1_ResultCode err = s1::String::IndependentCreate (str, s1::u8 (testString3_u8));
    BOOST_CHECK(S1_SUCCESSFUL(err));
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR3_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR3_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR3_U32(str_utf32);
  }
  {
    s1::String::Pointer str;
    s1_ResultCode err = s1::String::IndependentCreate (str, s1::u16 (testString3_u16));
    BOOST_CHECK(S1_SUCCESSFUL(err));
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR3_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR3_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR3_U32(str_utf32);
  }
  {
    s1::String::Pointer str;
    s1_ResultCode err = s1::String::IndependentCreate (str, s1::u32 (testString3_u32));
    BOOST_CHECK(S1_SUCCESSFUL(err));
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR3_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR3_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR3_U32(str_utf32);
  }
}


BOOST_AUTO_TEST_CASE(IndependentCreateUnicode3CXX)
{
  s1::String::Pointer strU8, strU16, strU32;
  {
    s1_ResultCode err = s1::String::IndependentCreate (strU8, testString3_u8);
    BOOST_CHECK(S1_SUCCESSFUL(err));
    BOOST_CHECK_NE (strU8, (s1::String*)nullptr);
  }
  {
    s1_ResultCode err = s1::String::IndependentCreate (strU16, testString3_u16);
    BOOST_CHECK(S1_SUCCESSFUL(err));
    BOOST_CHECK_NE (strU16, (s1::String*)nullptr);
  }
  {
    s1_ResultCode err = s1::String::IndependentCreate (strU32, testString3_u32);
    BOOST_CHECK(S1_SUCCESSFUL(err));
    BOOST_CHECK_NE (strU32, (s1::String*)nullptr);
  }
  {
    s1::String::Pointer str;
    s1_ResultCode err = s1::String::IndependentCreate (str, strU8);
    BOOST_CHECK(S1_SUCCESSFUL(err));
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR3_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR3_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR3_U32(str_utf32);
  }
  {
    s1::String::Pointer str;
    s1_ResultCode err = s1::String::IndependentCreate (str, strU16);
    BOOST_CHECK(S1_SUCCESSFUL(err));
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR3_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR3_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR3_U32(str_utf32);
  }
  {
    s1::String::Pointer str;
    s1_ResultCode err = s1::String::IndependentCreate (str, strU32);
    BOOST_CHECK(S1_SUCCESSFUL(err));
    BOOST_CHECK_NE (str, (s1::String*)nullptr);
    const char* str_utf8 = str->U8 ();
    CHECK_EQUAL_STR3_U8(str_utf8);
    const s1_char16* str_utf16 = str->U16 ();
    CHECK_EQUAL_STR3_U16(str_utf16);
    const s1_char32* str_utf32 = str->U32 ();
    CHECK_EQUAL_STR3_U32(str_utf32);
  }
}

BOOST_AUTO_TEST_SUITE_END()
