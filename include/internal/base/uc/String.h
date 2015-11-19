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

#include <limits>
#include <string>
#include <string.h>

#include <boost/atomic.hpp>
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
      typedef s1_string_size_type size_type;
      typedef Char value_type;
      BOOST_STATIC_CONSTANT(size_type, npos = (size_type)~0);

      inline String ();
      inline String (const char* s);
      inline String (const Char* s);
      String (const String& s);
      inline String (const String& s, size_type start);
      inline String (Char32 c);
      inline String (const Char32* s);
      inline String (String&& s);
      inline String (const Char* begin, const Char* end);
      ~String ();

      inline String& append (const char* s);
      String& append (const char* s, size_t n);
      String& append (const Char* s);
      String& append (const Char* s, size_t n);
      inline String& append (const Char32* s);
      String& append (const Char32* s, size_t n);
      inline String& append (const String& s);
      String& append (Char c);
      String& append (Char32 c);

      String& insert (size_type before, const char* s);

      size_type length() const { return d.length; }
      size_type size() const { return length(); }
      static size_type max_size() { return std::numeric_limits<size_type>::max () / sizeof (Char); }
      bool isEmpty() const { return length() == 0; }
      size_type countChar32() const;

      /// Make sure memory for at least \a minCapacity characters is reserved
      void reserve (size_type minCapacity)
      {
        if (IsBufferUnique() && (d.capacity >= minCapacity)) return;
        ResizeBuffer (minCapacity);
      }
      /**
       * Make sure memory for the current string length plus \a additionalCapacity characters is reserved.
       * \remarks May throw if \a additionalCapacity is too large.
       */
      void reserveExtra (size_t additionalCapacity);
      void shrink_to_fit()
      {
        ResizeBuffer (length());
      }
      const Char* data() const { return d.buffer; }

      String& operator= (const String& other);
      String& operator= (String&& other);
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
      static String fromUTF8 (const char* s, size_t len = (size_t)~0);
      static String fromUTF32 (const Char32* s, size_t len = (size_t)~0);

      /// Check if string starts with the given string.
      bool startsWith (const String& s) const;
      /// Check if string starts with the given ASCII string.
      bool startsWith (const char* s, size_t len = (size_t)~0) const;
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

      friend void swap (String& s1, String& s2);

      /**\name Simplistic STL iterator support
       * @{ */
      typedef const Char* iterator;
      typedef const Char* const_iterator;
      iterator begin () { return data (); }
      const_iterator begin () const { return data (); }
      const_iterator cbegin () const { return data (); }
      iterator end () { return data () + length (); }
      const_iterator end () const { return data () + length (); }
      const_iterator cend () const { return data () + length (); }
      /** @} */
    private:
      friend class CharacterIterator;

      struct Data
      {
        size_type length;
        size_type capacity;
        /* Hack to allow string to be readable in debugger.
         * (At least VS versions prioer to 2015.
         * The latter's debugger correctly displays char16_t strings.) */
      #if defined(_MSC_VER) && (_MSC_VER < 1900) && defined(_DEBUG)
        union
        {
          value_type* buffer;
          const wchar_t* _buffer_w;
        };
      #else
        value_type* buffer;
      #endif

        Data (size_type length, size_type capacity, value_type* buffer)
          : length (length), capacity (capacity), buffer (buffer) { }
      } d;

      typedef boost::atomic<boost::uint32_t> ref_count_type;
      // Desired size of a string instance
      BOOST_STATIC_CONSTANT(size_type, DesiredInstanceSize = 64);
      BOOST_STATIC_CONSTANT(size_type, InternalBufferSize = (DesiredInstanceSize - sizeof(Data))/sizeof(value_type));

      value_type internalBuffer[InternalBufferSize];
      struct AllocatedBufferData
      {
        ref_count_type refCount;
        value_type data[1];
      };

      /// Free the character buffer
      void FreeBuffer ();
      /// Resize the character buffer
      void ResizeBuffer (size_type capacity);

      /// Get pointer to AllocatedBufferData object
      AllocatedBufferData* BufferDataPtr() const;
      /// Returns whether buffer is internal
      bool IsBufferInternal() const
      { return (d.buffer == internalBuffer); }
      /// Returns whether we alone own the buffer pointer
      bool IsBufferUnique() const
      { return IsBufferInternal() || (BufferDataPtr()->refCount.load() == 1); }

      AllocatedBufferData* AllocBufferData (size_type numChars);
      AllocatedBufferData* ReallocBufferData (AllocatedBufferData* p, size_type numChars);
      // Add a reference to some buffer data.
      void RefBufferData (AllocatedBufferData* data);
      // Release a reference to some buffer data. Frees if necessary
      void ReleaseBufferData (AllocatedBufferData* data);

      Char* writePtr() { return d.buffer; }
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

    String::String (String&& s) : d (0, 0, internalBuffer)
    {
      bool otherBufferInternal = s.IsBufferInternal();
      std::swap (d.capacity, s.d.capacity);
      std::swap (d.length, s.d.length);
      if (otherBufferInternal)
      {
        memcpy (internalBuffer, s.internalBuffer, sizeof(internalBuffer));
      }
      else
      {
        d.buffer = s.d.buffer;
        s.d.buffer = s.internalBuffer;
      }
    }

    String::String (const Char* begin, const Char* end) : d (0, 0, internalBuffer)
    {
      append (begin, end - begin);
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
