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
 * Wrapper for API access to strings
 */
#ifndef __S1_CXXAPI_STRINGWRAPPER_HPP__
#define __S1_CXXAPI_STRINGWRAPPER_HPP__

#include "base/uc/String.h"

namespace s1
{
  namespace api_impl
  {
    /**
     * Takes an uc::String and provides access in various encodings.
     */
    class StringWrapper
    {
      /// Original string
      uc::String str;

      /// Whether an UTF-8 encoded version is cached
      mutable bool haveUTF8 : 1;
      /// Whether an UTF-32 encoded version is cached
      mutable bool haveUTF32 : 1;

      /// UTF-8 encoded version
      mutable alignas(std::string) uint8_t storeStrUTF8[sizeof(std::string)];
      typedef std::basic_string<s1_char32> c32string;
      /// UTF-32 encoded version
      mutable alignas(c32string) uint8_t storeStrUTF32[sizeof (c32string)];

      /// Get UTF-8 string instance
      std::string& GetStrUTF8 () const
      { return *(reinterpret_cast<std::string*> (storeStrUTF8)); }
      /// Get UTF-32 string instance
      c32string& GetStrUTF32 () const
      { return *(reinterpret_cast<c32string*> (storeStrUTF32)); }

      /// Clear all encoded strings
      void Clear ()
      {
        if (haveUTF8)
        {
          GetStrUTF8().~basic_string ();
          haveUTF8 = false;
        }
        if (haveUTF32)
        {
          GetStrUTF32 ().~c32string ();
          haveUTF32 = false;
        }
      }
    public:
      StringWrapper () : haveUTF8 (false), haveUTF32 (false) {}
      StringWrapper (const uc::String& s) : str (s), haveUTF8 (false), haveUTF32 (false) {}
      StringWrapper (uc::String&& s) : str (std::move (s)), haveUTF8 (false), haveUTF32 (false) {}
      ~StringWrapper () { Clear (); }

      StringWrapper& operator= (const uc::String& s)
      {
        if (str != s)
        {
          str = s;
          Clear ();
        }
        return *this;
      }
      StringWrapper& operator= (uc::String&& s)
      {
        if (str != s)
        {
          str = s;
          Clear ();
        }
        return *this;
      }

      /// Get original string
      const uc::String& GetUCS () const { return str; }
      /// Get string, UTF-8 encoded
      const char* GetUTF8 () const;
      /// Get string, UTF-16 encoded
      const s1_char16* GetUTF16 () const;
      /// Get string, UTF-32 encoded
      const s1_char32* GetUTF32 () const;
      /// Get string, as wide string
      const wchar_t* GetWS () const;
    };
  } // namespace api_impl
} // namespace s1

#endif // __S1_CXXAPI_STRINGWRAPPER_HPP__
