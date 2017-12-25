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

#include "base/MultiOptional.h"
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

      typedef std::basic_string<s1_char32> c32string;
      /// UTF-8 and UTF-32 encoded versions
      mutable MultiOptional<std::string, c32string> storeStrUTF;

      /// Clear all encoded strings
      void Clear ()
      {
        storeStrUTF.reset<0> ();
        storeStrUTF.reset<1> ();
      }
    public:
      StringWrapper () {}
      StringWrapper (const uc::String& s) : str (s) {}
      StringWrapper (uc::String&& s) : str (std::move (s)) {}
      StringWrapper (StringWrapper&& s);
      StringWrapper (const StringWrapper& s) : StringWrapper (s.GetUCS()) {}
      ~StringWrapper () { }

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
      StringWrapper& operator= (StringWrapper&& s);
      StringWrapper& operator= (const StringWrapper& s)
      {
        return operator=(s.GetUCS());
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
      const wchar_t* GetWCS () const;
    };
  } // namespace api_impl
} // namespace s1

#endif // __S1_CXXAPI_STRINGWRAPPER_HPP__
