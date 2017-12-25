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
#include "StringWrapper.h"

namespace s1
{
  namespace api_impl
  {
    StringWrapper::StringWrapper (StringWrapper&& s) : StringWrapper (std::move (s.str))
    {
      storeStrUTF = std::move (s.storeStrUTF);
    }

    StringWrapper& StringWrapper::operator= (StringWrapper&& s)
    {
      str = std::move (s.str);
      storeStrUTF = std::move (s.storeStrUTF);
      return *this;
    }

    const char* StringWrapper::GetUTF8 () const
    {
      if (!storeStrUTF.has_value<0> ())
      {
        auto& strUTF8 = storeStrUTF.emplace<0> ();
        str.toUTF8String (strUTF8);
        return strUTF8.c_str ();
      }
      return storeStrUTF.value<0> ().c_str ();
    }

    const s1_char16* StringWrapper::GetUTF16 () const
    {
      return str.data ();
    }

    const s1_char32* StringWrapper::GetUTF32 () const
    {
      if (!storeStrUTF.has_value<1> ())
      {
        auto& strUTF32 = storeStrUTF.emplace<1> ();
        strUTF32.reserve (str.size ());
        uc::String::CharacterIterator charIt (str);
        while (charIt.hasNext ())
        {
          strUTF32.push_back (charIt.next32PostInc ());
        }
        return strUTF32.c_str ();
      }
      return storeStrUTF.value<1> ().c_str ();
    }

    const wchar_t* StringWrapper::GetWCS () const
    {
    #if defined(S1_WCHAR_IS_UTF16)
      return reinterpret_cast<const wchar_t*> (GetUTF16 ());
    #elif defined(S1_WCHAR_IS_UTF32)
      return reinterpret_cast<const wchar_t*> (GetUTF32 ());
    #else
      return nullptr;
    #endif
    }
  } // namespace api_impl
} // namespace s1
