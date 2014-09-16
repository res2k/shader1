/*
    Shader1
    Copyright (c) 2010-2014 Frank Richter


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
 * Unicode string class
 */
#ifndef __BASE_UC_STRING_H__
#define __BASE_UC_STRING_H__

#include "Char.h"

#include <unicode/unistr.h>
#include <unicode/schriter.h>

#include <boost/config.hpp>

namespace s1
{
  namespace uc
  {
    class String : public U_NAMESPACE_QUALIFIER UnicodeString
    {
    protected:
      explicit String (const UnicodeString& s) : UnicodeString (s) {}
    public:
      typedef int size_type;
      BOOST_STATIC_CONSTANT(size_type, npos = -1);

      String () {}
      String (const char* s) : UnicodeString (s) {}
      String (const Char* s) : UnicodeString (s) {}
      String (const String& s) : UnicodeString (s) {}
      String (const String& s, size_type n) : UnicodeString (s, n) {}
      String (Char32 c) : UnicodeString (c) {}

      friend String operator+ (const String& s1, const String& s2)
      {
        return String (static_cast<const UnicodeString&> (s1) + static_cast<const UnicodeString&> (s2));
      }

      static String fromUTF8 (const std::string& s)
      {
        return String (UnicodeString::fromUTF8 (s));
      }
      static String fromUTF8 (const char* s)
      {
        return String (UnicodeString::fromUTF8 (s));
      }
      static String fromUTF32 (const Char32* s, size_type len)
      {
        return String (UnicodeString::fromUTF32 (s, len));
      }

      enum NormalizationMode
      {
        normNFD
      };
      /// \throws UnicodeException in case of error
      String Normalized (NormalizationMode mode);

      typedef StringCharacterIterator CharacterIterator;
    };
  } // namespace uc
} // namespace s1

// Boost hash function for UnicodeString
namespace boost
{
  inline std::size_t hash_value (const s1::uc::String& s)
  {
    return s.hashCode();
  }
}

#endif // __BASE_UC_STRING_H__
