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
        str.toUTF8String (strUTF8);
        haveUTF8 = true;
      }
      return strUTF8.c_str ();
    }

    const s1_char16* StringWrapper::GetUTF16 () const
    {
      return str.data ();
    }

    const s1_char32* StringWrapper::GetUTF32 () const
    {
      if (!haveUTF32)
      {
        strUTF32.reserve (str.size ());
        uc::String::CharacterIterator charIt (str);
        while (charIt.hasNext ())
        {
          strUTF32.push_back (charIt.next32PostInc ());
        }
        haveUTF32 = true;
      }
      return strUTF32.c_str ();
    }

    const wchar_t* StringWrapper::GetWS () const
    {
    #if CXX_SIZEOF_WCHAR_T == 2
      return reinterpret_cast<const wchar_t*> (GetUTF16 ());
    #elif CXX_SIZEOF_WCHAR_T == 4
      return reinterpret_cast<const wchar_t*> (GetUTF32 ());
    #else
      return nullptr;
    #endif
    }
  } // namespace api_impl
} // namespace s1
