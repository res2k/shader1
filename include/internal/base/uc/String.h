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

#include <string>
#include <string.h>

#include <boost/config.hpp>

namespace s1
{
  namespace uc
  {
    class String
    {
    protected:
      String NormalizeNFD ();
    public:
      typedef size_t size_type;
      typedef Char value_type;
      BOOST_STATIC_CONSTANT(size_type, npos = (size_type)~0);

      inline String ();
      inline String (const char* s);
      inline String (const Char* s);
      inline String (const String& s);
      inline String (const String& s, size_type start);
      inline String (Char32 c);
      inline String (const Char32* s);
      ~String ();

      inline String& append (const char* s);
      String& append (const char* s, size_type n);
      String& append (const Char* s);
      String& append (const Char* s, size_type n);
      inline String& append (const Char32* s);
      String& append (const Char32* s, size_type n);
      inline String& append (const String& s);
      String& append (Char c);
      String& append (Char32 c);

      String& insert (size_type before, const char* s);

      size_type length() const { return d.length; }
      size_type size() const { return length(); }
      bool isEmpty() const { return length() == 0; }
      size_type countChar32() const;

      void reserve (size_type minCapacity)
      {
        if (d.capacity >= minCapacity) return;
        ResizeBuffer (minCapacity);
      }
      void shrink_to_fit()
      {
        ResizeBuffer (length());
      }
      const Char* data() const { return d.buffer; }

      String& operator= (const String& other);
      friend String operator+ (const String& s1, const String& s2);
      String& operator+= (Char32 ch) { return append (ch); }
      bool operator==(const String& other) const;
      Char operator[] (size_type index) const
      {
        assert (index < d.length);
        return d.buffer[index];
      }

      void toUTF8String (std::string& dest) const;

      static String fromUTF8 (const std::string& s)
      {
        return fromUTF8 (s.c_str());
      }
      static String fromUTF8 (const char* s, size_type len = (size_type)~0);
      static String fromUTF32 (const Char32* s, size_type len = (size_type)~0);

      bool startsWith (const String& s) const;
      size_type indexOf (Char32 ch) const;

      enum NormalizationMode
      {
        normNFD
      };
      /// \throws UnicodeException in case of error
      String Normalized (NormalizationMode mode);

      /// Iterator for visiting every code point in a string
      class CharacterIterator
      {
        const String& str;
        size_type pos;
      public:
        /// Initializer
        CharacterIterator (const String& str) : str (str), pos (0) {}

        /// Set iterator to start
        void setToStart () { pos = 0; }
        /// Whether additional characters are available
        bool hasNext() const { return pos < str.length(); }
        /// Get currently pointed-to character, then advance to next char
        Char32 next32PostInc();
      };
    private:
      friend class CharacterIterator;

      struct Data
      {
        size_type length;
        size_type capacity;
        value_type* buffer;

        Data (size_type length, size_type capacity, value_type* buffer)
          : length (length), capacity (capacity), buffer (buffer) { }
      } d;

      // Desired size of a string instance
      BOOST_STATIC_CONSTANT(size_type, DesiredInstanceSize = 64);
      BOOST_STATIC_CONSTANT(size_type, InternalBufferSize = (DesiredInstanceSize - sizeof(Data))/sizeof(value_type));

      value_type internalBuffer[InternalBufferSize];
      struct AllocatedBufferData
      {
        value_type data[1];
      };

      /// Free the character buffer
      void FreeBuffer ();
      /// Resize the character buffer
      void ResizeBuffer (size_type capacity);

      /// Get pointer to AllocatedBufferData object
      AllocatedBufferData* BufferDataPtr();
      AllocatedBufferData* AllocBufferData (size_type numChars);
      AllocatedBufferData* ReallocBufferData (AllocatedBufferData* p, size_type numChars);

      Char* data() { return d.buffer; }
      void setLength (size_type len) { d.length = len; }
    };
  } // namespace uc
} // namespace s1

// Boost hash function for UnicodeString
namespace boost
{
  std::size_t hash_value (const s1::uc::String& s);
}

//
// Provide inline methods
//
namespace s1
{
  namespace uc
  {
    String::String() : d (0, 0, internalBuffer)
    {
    }

    String::String (const char* s) : d (0, 0, internalBuffer)
    {
      append (s);
    }

    String::String (const Char* s) : d (0, 0, internalBuffer)
    {
      append (s);
    }

    String::String (const String& s) : d (0, 0, internalBuffer)
    {
      // TODO: Share buffer data, if applicable
      append (s);
    }
    
    String::String (const String& s, size_type start) : d (0, 0, internalBuffer)
    {
      if (start < s.length())
      {
        append (s.data() + start, s.length() - start);
      }
    }

    String::String (Char32 c) : d (0, 0, internalBuffer)
    {
      append (c);
    }

    String::String (const Char32* s) : d (0, 0, internalBuffer)
    {
      append (s);
    }

    String& String::append (const char* s)
    {
      return append (s, std::char_traits<char>::length (s));
    }

    String& String::append (const Char32* s)
    {
      return append (s, std::char_traits<Char32>::length (s));
    }

    String& String::append (const String& s)
    {
      return append (s.data(), s.length());
    }
  } // namespace uc
} // namespace s1

#endif // __BASE_UC_STRING_H__
