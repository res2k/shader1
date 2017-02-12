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
    const char* StringWrapper::GetUTF8 () const
    {
      if (!haveUTF8)
      {
        new (storeStrUTF8) std::string;
        str.toUTF8String (GetStrUTF8());
        haveUTF8 = true;
      }
      return GetStrUTF8().c_str ();
    }

    const s1_char16* StringWrapper::GetUTF16 () const
    {
      return str.data ();
    }

    const s1_char32* StringWrapper::GetUTF32 () const
    {
      if (!haveUTF32)
      {
        new (storeStrUTF32) c32string;
        GetStrUTF32().reserve (str.size ());
        uc::String::CharacterIterator charIt (str);
        while (charIt.hasNext ())
        {
          GetStrUTF32().push_back (charIt.next32PostInc ());
        }
        haveUTF32 = true;
      }
      return GetStrUTF32().c_str ();
    }

    const wchar_t* StringWrapper::GetWS () const
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
