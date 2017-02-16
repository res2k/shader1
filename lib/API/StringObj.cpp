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

#include "base/common.h"
#include "base/ResultCode_internal.h"

#include "StringObj.h"

namespace s1
{
  namespace api_impl
  {
    ResultCode String::Create (boost::intrusive_ptr<String>& strObj, s1::Library* lib, uc::String&& str)
    {
      strObj.reset (new String (std::move (str), lib));
      if (!strObj) return S1_E_OUT_OF_MEMORY;
      return S1_SUCCESS;
    }

    ResultCode String::Create (boost::intrusive_ptr<String>& strObj, s1::Library* lib, const uc::String& str)
    {
      strObj.reset (new String (str, lib));
      if (!strObj) return S1_E_OUT_OF_MEMORY;
      return S1_SUCCESS;
    }

    static ResultCode TranslateStringConversionError (uc::String::ConversionError error)
    {
      switch (error)
      {
      case uc::String::ceSuccess:             return S1_SUCCESS;
      case uc::String::ceCharacterIncomplete: return S1_S_INPUT_INCOMPLETE;
      case uc::String::ceCharacterInvalid:    return S1_S_CODE_POINT_INVALID;
      case uc::String::ceEncodingInvalid:     return S1_S_ENCODING_INVALID;
      }
      // Fallback
      return S1_SUCCESS;
    }

    ResultCode String::Create (boost::intrusive_ptr<String>& strObj, s1::Library* lib, const char* str, const char** invalidPos)
    {
      auto result = uc::String::convertUTF8 (str);
      strObj.reset (new String (std::move (result.str), lib));
      if (!strObj) return S1_E_OUT_OF_MEMORY;
      if (invalidPos) *invalidPos = result.invalidPos;
      return TranslateStringConversionError (result.error);
    }

    ResultCode String::Create (boost::intrusive_ptr<String>& strObj, s1::Library* lib, const s1_char16* str, const s1_char16** invalidPos)
    {
      auto result = uc::String::convertUTF16 (str);
      strObj.reset (new String (std::move (result.str), lib));
      if (!strObj) return S1_E_OUT_OF_MEMORY;
      if (invalidPos) *invalidPos = result.invalidPos;
      return TranslateStringConversionError (result.error);
    }

    ResultCode String::Create (boost::intrusive_ptr<String>& strObj, s1::Library* lib, const s1_char32* str, const s1_char32** invalidPos)
    {
      auto result = uc::String::convertUTF32 (str);
      strObj.reset (new String (std::move (result.str), lib));
      if (!strObj) return S1_E_OUT_OF_MEMORY;
      if (invalidPos) *invalidPos = result.invalidPos;
      return TranslateStringConversionError (result.error);
    }

    ResultCode String::Create (boost::intrusive_ptr<String>& strObj, s1::Library* lib, const wchar_t* str, const wchar_t** invalidPos)
    {
#if defined(S1_WCHAR_IS_UTF16)
      auto result = uc::String::convertUTF16 (reinterpret_cast<const s1_char16*> (str));
#elif defined(S1_WCHAR_IS_UTF32)
      auto result = uc::String::convertUTF32 (reinterpret_cast<const s1_char32*> (str));
#endif
      strObj.reset (new String (std::move (result.str), lib));
      if (!strObj) return S1_E_OUT_OF_MEMORY;
      if (invalidPos) *invalidPos = reinterpret_cast<const wchar_t*> (result.invalidPos);
      return TranslateStringConversionError (result.error);
    }
  } // namespace api_impl
} // namespace s1

template<typename Ch>
static s1_ResultCode s1_string_independent_create_internal (s1_String** newStrObj, const Ch* string, const Ch** invalidPos)
{
  if (!newStrObj) return S1_E_INVALID_ARG_N (0);

  *newStrObj = nullptr;
  if (!string) return S1_E_INVALID_ARG_N (1);

  return s1::api_impl::String::Try (
    [=]() -> s1::Result<nullptr_t> {
      boost::intrusive_ptr<s1::api_impl::String> newString;
      s1::ResultCode createResult = s1::api_impl::String::Create (newString, nullptr, string, invalidPos);
      if (newString) newString->AddRef ();
      *newStrObj = newString->DowncastEvil<s1_String> ();
      return createResult;
    }).code();
}

S1_API(s1_ResultCode) s1_string_independent_create (s1_String** newStrObj, const char* string, const char** invalidPos)
{
  return s1_string_independent_create_internal (newStrObj, string, invalidPos);
}

S1_API(s1_ResultCode) s1_string_independent_create_wcs (s1_String** newStrObj, const wchar_t* string, const wchar_t** invalidPos)
{
  return s1_string_independent_create_internal (newStrObj, string, invalidPos);
}

S1_API(s1_ResultCode) s1_string_independent_create_u16 (s1_String** newStrObj, const s1_char16* string, const s1_char16** invalidPos)
{
  return s1_string_independent_create_internal (newStrObj, string, invalidPos);
}

S1_API(s1_ResultCode) s1_string_independent_create_u32 (s1_String** newStrObj, const s1_char32* string, const s1_char32** invalidPos)
{
  return s1_string_independent_create_internal (newStrObj, string, invalidPos);
}

const char* s1_string_u8 (s1_String* string)
{
  S1_ASSERT_MSG(string, "NULL String", false);
  s1::api_impl::String* string_impl (s1::EvilUpcast<s1::api_impl::String> (string));

  return string_impl->Str ();
}

const wchar_t* s1_string_wcs (s1_String* string)
{
  S1_ASSERT_MSG(string, "NULL String", false);
  s1::api_impl::String* string_impl (s1::EvilUpcast<s1::api_impl::String> (string));

  return string_impl->StrWCS ();
}

const s1_char16* s1_string_u16 (s1_String* string)
{
  S1_ASSERT_MSG(string, "NULL String", false);
  s1::api_impl::String* string_impl (s1::EvilUpcast<s1::api_impl::String> (string));

  return string_impl->StrU16 ();
}

const s1_char32* s1_string_u32 (s1_String* string)
{
  S1_ASSERT_MSG(string, "NULL String", false);
  s1::api_impl::String* string_impl (s1::EvilUpcast<s1::api_impl::String> (string));

  return string_impl->StrU32 ();
}
