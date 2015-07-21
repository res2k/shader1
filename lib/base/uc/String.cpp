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

#include "base/common.h"

#include "base/uc/UTF16Decoder.h"
#include "base/uc/UTF16Encoder.h"
#include "base/uc/UTF16to8Transcoder.h"
#include "base/uc/UTF8to16Transcoder.h"
#include "base/uc/Exception.h"
#include "base/uc/String.h"

#include <algorithm>
#include <boost/container/vector.hpp>
#include <boost/foreach.hpp>

#include <stddef.h>

#include "UCDDataLookup.h"

namespace s1
{
  namespace uc
  {
    // Helper functions
    namespace utf16
    {
      /* Return how many UTF-16 code units are encoding the next (valid) character.
       * Note that for an invalid character 0 is returned */
      static inline unsigned int encodingSize (const Char* src, String::size_type nSrc)
      {
        assert (nSrc > 0);
        Char ch1 = *src++;
        if ((ch1 >= LeadSurrogateFirst) && (ch1 <= LeadSurrogateLast))
        {
          Char ch2 = TailSurrogateLast+1; // Trigger invalid character path if only lead is given
          if (nSrc >= 2)
          {
            ch2 = *src++;
          }
          if ((ch2 >= TailSurrogateFirst) && (ch2 <= TailSurrogateLast))
          {
            // Complete surrogate pair, hurrah
            return 2;
          }
          else
          {
            // Incomplete surrogate
            return 0;
          }
        }
        else if ((ch1 >= TailSurrogateFirst) && (ch1 <= TailSurrogateLast))
        {
          // Tail surrogate can't stand alone
          return 0;
        }
        else
        {
          return 1;
        }
      }
    } // namespace utf16

    //-----------------------------------------------------------------------

    String::~String()
    {
      FreeBuffer();
    }

    String& String::append (const char* s, size_type n)
    {
      if (n == 0) return *this;

      reserve (d.length + n);
      Char* dest = d.buffer + d.length;
      for (size_type i = 0; i < n; i++)
      {
        *dest++ = *s++;
      }
      d.length += n;
      return *this;
    }

    String& String::append (const Char* s, size_type n)
    {
      if (n == 0) return *this;

      reserve (d.length + n);
      memcpy (d.buffer + d.length, s, n * sizeof(Char));
      d.length += n;
      return *this;
    }

    String& String::append (const Char32* s, size_type n)
    {
      if (n == 0) return *this;

      size_type needExtra (0);
      for (size_type i = 0; i < n; i++)
      {
        needExtra += UTF16Encoder::EncodedSize (SanitizeChar (s[i]));
      }
      reserve (length() + needExtra);
      Char* dst = d.buffer + length();
      Char* dstEnd = dst + needExtra;
      UTF16Encoder enc;
      for (size_type i = 0; i < n; i++)
      {
        UTF16Encoder::EncodeResult result = enc (s[i], dst, dstEnd);
        assert (result >= 0);
      }
      d.length += needExtra;
      return *this;
    }

    String& String::append (Char c)
    {
      c = static_cast<Char> (SanitizeChar (c));
      reserve (d.length + 1);
      d.buffer[d.length] = c;
      d.length += 1;
      return *this;
    }

    String& String::append (Char32 c)
    {
      c = SanitizeChar (c);
      unsigned char n = UTF16Encoder::EncodedSize (c);
      reserve (d.length + n);
      Char* dst = d.buffer + d.length;
      Char* dstEnd = dst + n;
      UTF16Encoder enc;
      UTF16Encoder::EncodeResult result = enc (c, dst, dstEnd);
      assert (result >= 0);
      assert (dst == dstEnd);
      d.length += n;
      return *this;
    }

    String& String::insert (size_type before, const char* s)
    {
      size_t n = strlen (s);
      if (n == 0) return *this;

      if (before > length()) before = length();
      reserve (d.length + n);
      memmove (data() + before + n, data() + before, (length() - before) * sizeof (Char));
      Char* dest = d.buffer + before;
      for (size_type i = 0; i < n; i++)
      {
        *dest++ = *s++;
      }
      d.length += n;
      return *this;
    }

    String::size_type String::countChar32() const
    {
      if (d.length == 0) return 0;
      size_type total (0);
      const Char* p = d.buffer;
      const Char* pEnd = p + d.length;
      while (p < pEnd)
      {
        unsigned int n = utf16::encodingSize (p, pEnd - p);
        if (n == 0) n = 1; // Invalid encodings result in a replacement char
        total++;
        p += n;
      }
      return total;
    }

    String& String::operator= (const String& other)
    {
      // TODO: Share buffer
      ResizeBuffer (other.d.length);
      memcpy (d.buffer, other.d.buffer, other.d.length * sizeof(Char));
      d.length = other.d.length;
      return *this;
    }

    bool String::operator==(const String& other) const
    {
      if (length() != other.length()) return false;
      if (data() == other.data()) return true;
      return memcmp (data(), other.data(), sizeof(Char) * length()) == 0;
    }

    void String::toUTF8String (std::string& dest) const
    {
      dest.reserve (dest.size() + d.length);
      char buf[256];
      char* output;
      char* outputEnd = buf + sizeof (buf);
      const Char16* input = d.buffer;
      const Char16* inputEnd = input + d.length;
      UTF16to8Transcoder transcoder;
      UTF16to8Transcoder::TranscodeResult result;
      while (input < inputEnd)
      {
        output = buf;
        transcoder (input, inputEnd, output, outputEnd);
        dest.append (buf, output - buf);
      }
      do
      {
        output = buf;
        result = transcoder.Flush (output, outputEnd);
        dest.append (buf, output - buf);
      }
      while (result != UTF16to8Transcoder::trSuccess);
    }

    String String::fromUTF8 (const char* utf8_str, size_type len)
    {
      if (len == (size_type)~0) len = strlen (utf8_str);

      String s;
      /* When converting from UTF-8, the string in UTF-16 will never have
       * more code points than bytes in the original string */
      s.reserve (len);
      const char* input = utf8_str;
      const char* inputEnd = utf8_str + len;
      Char* outputStart = s.data();
      Char* output = outputStart;
      Char* outputEnd = output + len;
      
      UTF8to16Transcoder transcoder;
      UTF8to16Transcoder::TranscodeResult result;
      while (input < inputEnd)
      {
        result = transcoder (input, inputEnd, output, outputEnd);
        assert (result != UTF8to16Transcoder::trOutputOverflow);
        /* An output overflow can, by definition, not happen (see comment
         * above). But if the output buffer size approximation ever 
         * changes (and loses above guarantee), below is the code
         * you need to deal with an overflow. */
      #if 0
        if (result == UTF8to16Transcoder::trOutputOverflow)
        {
          size_type outputPos = output - outputStart; 
          size_type newLen = outputPos + 2;
          s.reserve (newLen);
          outputStart = s.data();
          output = outputStart + outputPos;
          outputEnd = outputStart + newLen;
        }
      #endif
      }
      do
      {
        result = transcoder.Flush (output, outputEnd);
        // See above
        assert (result != UTF8to16Transcoder::trOutputOverflow);
      #if 0
        if (result == UTF8to16Transcoder::trOutputOverflow)
        {
          size_type outputPos = output - outputStart; 
          size_type newLen = outputPos + 2;
          s.reserve (newLen);
          outputStart = s.data();
          output = outputStart + outputPos;
          outputEnd = outputStart + newLen;
        }
      #endif
      }
      while (false/*result < 0*/);
      s.setLength (output - outputStart);
      s.shrink_to_fit();
      
      return s;
    }

    String String::fromUTF32 (const Char32* utf32_str, size_type len)
    {
      if (len == (size_type)~0)
      {
        const Char32* c = utf32_str;
        while (*c != 0)
        {
          c++;
        }
        len = c - utf32_str;
      }
      
      String s;
      const Char32* p = utf32_str;
      const Char32* end = utf32_str + len;
      while (p < end)
      {
        s.reserve (s.length() + (end - p));
        s.append (*p++);
      }
      s.shrink_to_fit();
      return s;
    }

    bool String::startsWith (const String& s) const
    {
      if (s.length() > length()) return false;
      return memcmp (data(), s.data(), s.length() * sizeof(Char)) == 0;
    }
    
    String::size_type String::indexOf (Char32 ch) const
    {
      if (ch != SanitizeChar(ch)) return npos;

      const Char* start = data();
      const Char* p = start;
      const Char* last = p + length();
      if (ch <= MaxChar16)
      {
        while (p < last)
        {
          if (*p == ch) return p - start;
          p++;
        }
        return npos;
      }
      else
      {
        Char surr[2];
        {
          UTF16Encoder enc;
          Char* dst = surr;
          UTF16Encoder::EncodeResult result = enc (ch, dst, dst+2);
          assert (result == UTF16Encoder::erSuccess);
        }
        ++p;
        while (p < last)
        {
          if ((*p == surr[1]) && (*(p-1) == surr[0])) return p - start - 1;
          p++;
        }
        return npos;
      }
    }

    void String::FreeBuffer()
    {
      if (d.buffer != internalBuffer)
      {
        free (BufferDataPtr ());
        d.buffer = internalBuffer;
      }
    }

    void String::ResizeBuffer (size_type capacity)
    {
      // TODO: We probably need buffer un-sharing here.
      if (d.capacity == capacity) return;

      bool currentIsInternal = d.buffer == internalBuffer;
      bool canUseInternal = capacity <= InternalBufferSize;
      if (canUseInternal)
      {
        if (!currentIsInternal)
        {
          assert (capacity < d.capacity); // Otherwise, couldn't use internal buffer
          memcpy (internalBuffer, d.buffer, capacity * sizeof (Char));
          FreeBuffer();
          d.buffer = internalBuffer;
        }
      }
      else
      {
        if (currentIsInternal)
        {
          assert (d.capacity < capacity);
          // Allocate new buffer
          AllocatedBufferData* newBuffer = AllocBufferData (capacity);
          memcpy (newBuffer->data, internalBuffer, d.capacity * sizeof (Char));
          d.buffer = newBuffer->data;
        }
        else
        {
          // Reallocate current buffer
          d.buffer = ReallocBufferData (BufferDataPtr(), capacity)->data;
        }
      }
      d.capacity = capacity;
    }

    String::AllocatedBufferData* String::BufferDataPtr()
    {
      assert (d.buffer != internalBuffer);
      return reinterpret_cast<AllocatedBufferData*> (
        reinterpret_cast<char*> (d.buffer) - offsetof (AllocatedBufferData, data));
    }

    String::AllocatedBufferData* String::AllocBufferData (size_type numChars)
    {
      return reinterpret_cast<AllocatedBufferData*> (
        malloc (offsetof (AllocatedBufferData, data) + numChars * sizeof (Char)));
    }

    String::AllocatedBufferData* String::ReallocBufferData (AllocatedBufferData* p, size_type numChars)
    {
      String::AllocatedBufferData* new_p = reinterpret_cast<AllocatedBufferData*> (
        realloc (p, offsetof (AllocatedBufferData, data) + numChars * sizeof (Char)));
      assert (new_p); // FIXME: Better OOM handling
      return new_p;
    }

    //
    // Normalization
    //

    #include "ucd_CanonicalDecomp.inc"
    #include "ucd_CombiningClass.inc"
    #include "ucd_NFD_QC_No.inc"

    String String::Normalized (NormalizationMode mode)
    {
      switch (mode)
      {
      case normNFD:
          return NormalizeNFD();
          break;
      }
      // Fallback case. Really, should not happen...
      return *this;
    }

    // Do a quick check if a normalization is required
    static bool NeedNormalizationNFD (const String& str)
    {
      // Algorithm is described in UAX #15
      uint8_t lastCombiningClass = 0;
      String::CharacterIterator idIt (str);
      while (idIt.hasNext())
      {
        Char32 ch = idIt.next32PostInc();
        uint8_t combiningClass = LookupProperty (ch, ucd_CombiningClass, static_cast<uint8_t> (0));
        if ((combiningClass != 0) && (lastCombiningClass > combiningClass))
          return true;
        if (IsCharInSet (ch, ucd_NFD_QC_No))
          return true;
        lastCombiningClass = combiningClass;
      }
      return false;
    }

    namespace
    {
      class CombiningMarksBuffer
      {
        boost::container::vector<Char32> vec;

        static bool CombiningMarkCompare (Char32 ch1, Char32 ch2)
        {
          uint8_t class1 = LookupProperty (ch1, ucd_CombiningClass, static_cast<uint8_t> (0));
          uint8_t class2 = LookupProperty (ch2, ucd_CombiningClass, static_cast<uint8_t> (0));
          return (class1 < class2);
        }
      public:
        void Flush (String& str)
        {
          if (!vec.empty())
          {
            std::stable_sort (vec.begin(), vec.end(), &CombiningMarkCompare);
            BOOST_FOREACH(Char32 mark, vec)
            {
              str.append (mark);
            }
            vec.clear();
          }
        }
        void Push (Char32 ch)
        {
          vec.push_back (ch);
        }
      };

      // Hangul decomposition
      namespace hangul
      {
        static const Char32 SBase = 0xAC00;
        static const Char32 LBase = 0x1100;
        static const Char32 VBase = 0x1161;
        static const Char32 TBase = 0x11A7;
        static const int LCount = 19;
        static const int VCount = 21;
        static const int TCount = 28;
        static const int NCount = VCount * TCount;
        static const int SCount = LCount * NCount;

        static inline bool IsPrecomposed (Char32 ch)
        {
          return (ch >= SBase) && (ch < SBase + SCount);
        }

        static inline void Decompose (Char32 ch, Char32& LPart, Char32& VPart, Char32& TPart)
        {
          int SIndex = ch - SBase;
          int LIndex = SIndex / NCount;
          int VIndex = (SIndex % NCount) / TCount;
          int TIndex = SIndex % TCount;
          LPart = LBase + LIndex;
          VPart = VBase + VIndex;
          TPart = TIndex > 0 ? TBase + TIndex : 0;
        }
      } // namespace hangul
    } // anonymous namespace

    String String::NormalizeNFD ()
    {
      if (!NeedNormalizationNFD (*this))
        return *this;

      //
      // Actual normalization
      //
      struct State
      {
        String str;
        CombiningMarksBuffer combiningMarks;

        void Flush() { combiningMarks.Flush (str); }
        void HandleChar (Char32 ch)
        {
          uint8_t combiningClass = LookupProperty (ch, ucd_CombiningClass, static_cast<uint8_t> (0));
          if (combiningClass == 0)
          {
            // Character is a starter character.
            combiningMarks.Flush (str);
            str.append (ch);
          }
          else
          {
            // Combining mark
            combiningMarks.Push (ch);
          }
        }
      };
      // TODO: reserve some memory for destination str
      State normalizationState;
      CharacterIterator idIt (*this);
      while (idIt.hasNext())
      {
        Char32 ch = idIt.next32PostInc();
        if (hangul::IsPrecomposed (ch))
        {
          Char32 L, V, T;
          hangul::Decompose (ch, L, V, T);
          normalizationState.HandleChar (L);
          normalizationState.HandleChar (V);
          if (T != 0)
            normalizationState.HandleChar (T);
          continue;
        }
        const Char32* decompSeq;
        unsigned int decompSeqSize;
        if (LookupSeq (ch, ucd_CanonicalDecomp, decompSeq, decompSeqSize))
        {
          /* Note: the decomposition data is stored fully decomposed,
           * recursion is not necessary */
          for (unsigned int i = 0; i < decompSeqSize; i++)
          {
            normalizationState.HandleChar (decompSeq[i]);
          }
        }
        else
        {
          // Not decomposed.
          normalizationState.HandleChar (ch);
        }
      }
      normalizationState.Flush ();
      return normalizationState.str;
    }

    //-----------------------------------------------------------------------

    Char32 String::CharacterIterator::next32PostInc()
    {
      if (pos >= str.length()) return InvalidChar32;

      Char32 ch;
      UTF16Decoder decoder;
      const Char16* input = str.data() + pos;
      UTF16Decoder::DecodeResult result = decoder (input, str.data() + str.length(), ch);
      if (result < 0) ch = ReplacementChar;
      pos = input - str.data();
      return ch;
    }
  } // namespace uc
} // namespace s1

//-----------------------------------------------------------------------

namespace boost
{
  std::size_t hash_value (const s1::uc::String& s)
  {
    // Workaround for infinite recursion when trying to hash char16_t
    const uint16_t* data_u16 = reinterpret_cast<const uint16_t*> (s.data());
    return boost::hash_range (data_u16, data_u16 + s.length());
  }
}