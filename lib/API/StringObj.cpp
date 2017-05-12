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

#include "ResultCodeHelper.h"
#include "StringObj.h"

#include "StringArg.h"

namespace s1
{
  namespace api_impl
  {
    String::CreateResultType String::Create (s1::Library* lib, uc::String&& str)
    {
      boost::intrusive_ptr<String> strObj;
      strObj.reset (new String (std::move (str), lib));
      return std::make_tuple (S1_SUCCESS, strObj, (size_t)~0);
    }

    String::CreateResultType String::Create (s1::Library* lib, const uc::String& str)
    {
      boost::intrusive_ptr<String> strObj;
      strObj.reset (new String (str, lib));
      return std::make_tuple (S1_SUCCESS, strObj, (size_t)~0);
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

    String::CreateResultType String::Create (s1::Library* lib, const char* str, size_t len)
    {
      auto result = uc::String::convertUTF8 (str, len);
      boost::intrusive_ptr<String> strObj;
      strObj.reset (new String (std::move (result.str), lib));
      size_t invalidPos = result.invalidPos ? result.invalidPos - str : (size_t)~0;
      return std::make_tuple (TranslateStringConversionError (result.error), strObj, invalidPos);
    }

    String::CreateResultType String::Create (s1::Library* lib, const s1_char16* str, size_t len)
    {
      auto result = uc::String::convertUTF16 (str, len);
      boost::intrusive_ptr<String> strObj;
      strObj.reset (new String (std::move (result.str), lib));
      size_t invalidPos = result.invalidPos ? result.invalidPos - str : (size_t)~0;
      return std::make_tuple (TranslateStringConversionError (result.error), strObj, invalidPos);
    }

    String::CreateResultType String::Create (s1::Library* lib, const s1_char32* str, size_t len)
    {
      auto result = uc::String::convertUTF32 (str, len);
      boost::intrusive_ptr<String> strObj;
      strObj.reset (new String (std::move (result.str), lib));
      size_t invalidPos = result.invalidPos ? result.invalidPos - str : (size_t)~0;
      return std::make_tuple (TranslateStringConversionError (result.error), strObj, invalidPos);
    }

    String::CreateResultType String::Create (s1::Library* lib, const wchar_t* str, size_t len)
    {
#if defined(S1_WCHAR_IS_UTF16)
      auto result = uc::String::convertUTF16 (reinterpret_cast<const s1_char16*> (str), len);
#elif defined(S1_WCHAR_IS_UTF32)
      auto result = uc::String::convertUTF32 (reinterpret_cast<const s1_char32*> (str), len);
#endif
      boost::intrusive_ptr<String> strObj;
      strObj.reset (new String (std::move (result.str), lib));
      size_t invalidPos = result.invalidPos ? reinterpret_cast<const wchar_t*> (result.invalidPos) - str : (size_t)~0;
      return std::make_tuple (TranslateStringConversionError (result.error), strObj, invalidPos);
    }

    class StringCreateVisitor
    {
      s1::Library* lib;
    public:
      typedef String::CreateResultType result_type;

      StringCreateVisitor (s1::Library* lib) : lib (lib) {}

      template<typename Ch>
      String::CreateResultType operator() (const Ch* s, size_t len)
      {
        return String::Create (lib, s, len);
      }
      String::CreateResultType operator() (cxxapi::String* s)
      {
        // Create new object with same string data
        s1::api_impl::String* string_impl (s1::EvilUpcast<s1::api_impl::String> (s));
        return String::Create (lib, string_impl->StrUCS ());
      }
      String::CreateResultType operator() (ResultCode error)
      {
        return std::make_tuple (error, boost::intrusive_ptr<String>(), (size_t)~0);
      }
    };

    String::CreateResultType String::Create (s1::Library* lib, cxxapi::StringArg str)
    {
      return VisitStringArg (str, StringCreateVisitor (lib));
    }
  } // namespace api_impl
} // namespace s1

S1_API(s1_ResultCode) s1_string_independent_create (s1_String** newStrObj, s1_StringArg string, size_t* invalidPos)
{
  if (!newStrObj) return S1_E_INVALID_ARG_N (0);

  *newStrObj = nullptr;

  return s1::api_impl::String::Try (
    [=]() -> s1::Result<nullptr_t> {
      auto createResult = s1::api_impl::String::Create (nullptr, string);
      auto createResultCode = static_cast<s1::ResultCode> (s1::detail::ChangeResultCodeArgumentIndex (std::get<0> (createResult), 1));
      boost::intrusive_ptr<s1::api_impl::String> newString = std::get<1> (createResult);
      if (newString) newString->AddRef ();
      *newStrObj = newString ? newString->DowncastEvil<s1_String> () : nullptr;
      if (invalidPos && (std::get<2> (createResult) != (size_t)~0)) *invalidPos = std::get<2> (createResult);
      return createResultCode;
    }).code();
}

template<typename Ch>
static s1_ResultCode s1_string_independent_create_internal (s1_String** newStrObj, const Ch* string, const Ch** invalidPos)
{
  if (!newStrObj) return S1_E_INVALID_ARG_N (0);

  *newStrObj = nullptr;
  if (!string) return S1_E_INVALID_ARG_N (1);

  return s1::api_impl::String::Try (
    [=]() -> s1::Result<nullptr_t> {
      auto createResult = s1::api_impl::String::Create (nullptr, string);
      auto createResultCode = static_cast<s1::ResultCode> (s1::detail::ChangeResultCodeArgumentIndex (std::get<0> (createResult), 1));
      boost::intrusive_ptr<s1::api_impl::String> newString = std::get<1> (createResult);
      if (newString) newString->AddRef ();
      *newStrObj = newString ? (newString->DowncastEvil<s1_String> ()) : nullptr;
      if (invalidPos && (std::get<2> (createResult) != (size_t)~0)) *invalidPos = string + std::get<2> (createResult);
      return createResultCode;
    }).code();
}

S1_API(s1_ResultCode) s1_string_independent_create_u8 (s1_String** newStrObj, const char* string, const char** invalidPos)
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
  S1_ASSERT_MSG(string, "NULL String", nullptr);
  s1::api_impl::String* string_impl (s1::EvilUpcast<s1::api_impl::String> (string));

  return string_impl->Str ();
}

const wchar_t* s1_string_wcs (s1_String* string)
{
  S1_ASSERT_MSG(string, "NULL String", nullptr);
  s1::api_impl::String* string_impl (s1::EvilUpcast<s1::api_impl::String> (string));

  return string_impl->StrWCS ();
}

const s1_char16* s1_string_u16 (s1_String* string)
{
  S1_ASSERT_MSG(string, "NULL String", nullptr);
  s1::api_impl::String* string_impl (s1::EvilUpcast<s1::api_impl::String> (string));

  return string_impl->StrU16 ();
}

const s1_char32* s1_string_u32 (s1_String* string)
{
  S1_ASSERT_MSG(string, "NULL String", nullptr);
  s1::api_impl::String* string_impl (s1::EvilUpcast<s1::api_impl::String> (string));

  return string_impl->StrU32 ();
}
