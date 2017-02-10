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
#include <boost/algorithm/string/predicate.hpp>
#include <boost/container/vector.hpp>
#include <boost/functional/hash.hpp>
#include <boost/optional.hpp>

#include <stddef.h>

#include "char_traits.h"
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
      static inline unsigned int encodingSize (const Char* src, size_t nSrc)
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

    // Lazy shortcut
    typedef char_traits<Char> Char_traits;

    String::String (const String& s) : String ()
    {
      if (s.IsBufferInternal())
      {
        append (s);
      }
      else
      {
        RefBufferData(s.BufferDataPtr());
        heapBuffer = s.heapBuffer;
        d = s.d;
      }
    }
    
    String::~String()
    {
      FreeBuffer();
    }

    String& String::append (const char* s, size_t n)
    {
      if (n == 0) return *this;

      size_type newLength = OverflowCheckAdd (length(), n, max_size () - 1);
      reserveInternal (newLength + 1);
      Char* dest = bufferPtr () + d.length;
      for (size_type i = 0; i < n; i++)
      {
        *dest++ = *s++;
      }
      d.length = newLength;
      bufferPtr()[d.length] = 0;
      return *this;
    }

    String& String::append(const Char* s)
    {
      return append(s, Char_traits::length(s));
    }

    String& String::append (const Char* s, size_t n)
    {
      if (n == 0) return *this;

      size_type newLength = OverflowCheckAdd (length(), n, max_size () - 1);
      if ((s >= bufferPtr ()) && (s < bufferPtr () + d.length))
      {
        // Special case self-appending
        ptrdiff_t s_pos = s - bufferPtr ();
        reserveInternal (newLength + 1);
        // ...because the pointer may change.
        s = bufferPtr() + s_pos;
      }
      else
      {
        reserveInternal (newLength + 1);
      }
      Char_traits::copy (bufferPtr() + d.length, s, n);
      d.length = newLength;
      bufferPtr()[d.length] = 0;
      return *this;
    }

    String& String::append (const Char32* s, size_t n)
    {
      if (n == 0) return *this;

      size_type newLength = length ();
      size_type needExtra (0);
      for (size_t i = 0; i < n; i++)
      {
        needExtra += UTF16Encoder::EncodedSize (SanitizeChar16 (s[i]));
        if (needExtra >= max_size () - 2)
        {
          newLength = OverflowCheckAdd (newLength, needExtra, max_size () - 1);
          needExtra = 0;
        }
      }
      newLength = OverflowCheckAdd (newLength, needExtra, max_size () - 1);
      reserveInternal (newLength + 1);
      Char* dst = bufferPtr () + length ();
      Char* dstEnd = dst + needExtra;
      UTF16Encoder enc;
      for (size_type i = 0; i < n; i++)
      {
        UTF16Encoder::EncodeResult result = enc (s[i], dst, dstEnd);
        assert (result >= 0);
      }
      d.length = newLength;
      *dst = 0;
      return *this;
    }

    String& String::append (Char c)
    {
      c = static_cast<Char> (SanitizeChar16 (c));
      size_type newLength = OverflowCheckAdd (length(), 1u, max_size () - 1);
      reserveInternal (newLength + 1);
      auto buf = bufferPtr ();
      buf[length ()] = c;
      d.length = newLength;
      buf[d.length] = 0;
      return *this;
    }

    String& String::append (Char32 c)
    {
      c = SanitizeChar16 (c);
      unsigned char n = UTF16Encoder::EncodedSize (c);
      size_type newLength = OverflowCheckAdd (length(), n, max_size () - 1);
      reserveInternal (newLength + 1);
      Char* dst = bufferPtr () + d.length;
      Char* dstEnd = dst + n;
      UTF16Encoder enc;
      UTF16Encoder::EncodeResult result = enc (c, dst, dstEnd);
      assert (result >= 0);
      assert (dst == dstEnd);
      d.length = newLength;
      *dst = 0;
      return *this;
    }

    String& String::insert (size_type before, const char* s)
    {
      size_t n = std::char_traits<char>::length (s);
      if (n == 0) return *this;

      if (before > length()) before = length();
      size_type newLength = OverflowCheckAdd (length(), n, max_size () - 1);
      reserveInternal (newLength + 1);
      Char* dest = bufferPtr () + before;
      Char_traits::move (dest + n, dest, length () - before);
      for (size_type i = 0; i < n; i++)
      {
        *dest++ = *s++;
      }
      d.length = newLength;
      bufferPtr()[d.length] = 0;
      return *this;
    }

    String::size_type String::countChar32() const
    {
      if (d.length == 0) return 0;
      size_type total (0);
      const Char* p = bufferPtr();
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

    void String::reserveExtra (size_t additionalCapacity)
    {
      reserveInternal (OverflowCheckAdd (length (), additionalCapacity, max_size () - 1) + 1);
    }

    String& String::operator= (const String& other)
    {
      if (&other != this)
      {
        if (other.IsBufferInternal())
        {
          ResizeBuffer (other.d.length + 1);
          auto buf = bufferPtr ();
          Char_traits::copy (buf, other.bufferPtr(), other.d.length);
          d.length = other.d.length;
          buf[d.length] = 0;
        }
        else
        {
          RefBufferData(other.BufferDataPtr());
          FreeBuffer();
          d = other.d;
          heapBuffer = other.heapBuffer;
        }
      }
      return *this;
    }

    String& String::operator= (String&& other)
    {
      if (&other != this)
      {
        FreeBuffer ();
        bool otherBufferInternal = other.IsBufferInternal();
        d.capacity = other.d.capacity;
        other.d.capacity = 0;
        d.length = other.d.length;
        other.d.length = 0;
        if (otherBufferInternal)
        {
          memcpy (internalBuffer, other.internalBuffer, sizeof(internalBuffer));
        }
        else
        {
          heapBuffer = other.heapBuffer;
        }
      }
      return *this;
    }

    String operator+ (const String& s1, const String& s2)
    {
      if (s1.isEmpty ())
        return s2;
      else if (s2.isEmpty ())
        return s1;
      String newString;
      newString.reserve (s1.length () + s2.length ());
      newString.append (s1);
      newString.append (s2);
      return std::move (newString);
    }

    bool String::operator==(const String& other) const
    {
      if (length() != other.length()) return false;
      if (data() == other.data()) return true;
      return Char_traits::compare (data(), other.data(), length()) == 0;
    }

    bool String::operator!=(const String& other) const
    {
      return !(*this == other);
    }

    void String::toUTF8String (std::string& dest) const
    {
      std::string::size_type destReserve = OverflowCheckAdd (dest.size(), length(), dest.max_size ());
      dest.reserve (destReserve);
      char buf[256];
      char* output;
      char* outputEnd = buf + sizeof (buf);
      const Char16* input = bufferPtr();
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

    String String::fromUTF8 (const char* utf8_str, size_t len)
    {
      return convertUTF8 (utf8_str, len).str;
    }

    String String::fromUTF32 (const Char32* utf32_str, size_t len)
    {
      return convertUTF32 (utf32_str, len).str;
    }

    String String::fromUntrustedWS (const wchar_t* s, size_t len)
    {
    #if CXX_SIZEOF_WCHAR_T == 2
      return fromUntrustedUTF (reinterpret_cast<const Char16*> (s), len);
    #elif CXX_SIZEOF_WCHAR_T == 4
      return fromUntrustedUTF (reinterpret_cast<const Char32*> (s), len);
    #endif
    }

    String::ConversionResult<char> String::convertUTF8 (const char* utf8_str, size_t len)
    {
      if (len == (size_t)~0) len = std::char_traits<char>::length (utf8_str);

      ConversionResult<char> res;
      /* When converting from UTF-8, the string in UTF-16 will never have
      * more code units than bytes in the original string */
      res.str.reserveInternal (OverflowCheckAdd (size_type (0), len, max_size() - 1) + 1);
      const char* input = utf8_str;
      const char* inputEnd = utf8_str + len;
      Char* outputStart = res.str.bufferPtr();
      Char* output = outputStart;
      Char* outputEnd = output + len;
      boost::optional<std::pair<const char*, UTF8to16Transcoder::TranscodeResult>> firstInvalid;

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
        if ((result != UTF8to16Transcoder::trSuccess) && !firstInvalid)
        {
          firstInvalid = std::make_pair (input, result);
        }
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
        if ((result != UTF8to16Transcoder::trSuccess) && !firstInvalid)
        {
          firstInvalid = std::make_pair (input, result);
        }
      }
      while (false/*result < 0*/);
      *output = 0;
      res.str.setLength (OverflowCheckAdd (size_type (0), static_cast<size_t> (output - outputStart), max_size() - 1));
      res.str.shrink_to_fit();

      if (firstInvalid)
      {
        res.invalidPos = firstInvalid->first;
        switch (firstInvalid->second)
        {
        case UTF8to16Transcoder::trOutputOverflow:
          /* Should not happen */
        case UTF8to16Transcoder::trSuccess:
          res.error = ceSuccess;
          break;
        case UTF8to16Transcoder::trInputUnderrun:
        case UTF8to16Transcoder::trCharacterIncomplete:
          res.error = ceCharacterIncomplete;
          break;
        case UTF8to16Transcoder::trCharacterInvalid:
          res.error = ceCharacterInvalid;
          break;
        case UTF8to16Transcoder::trEncodingInvalid:
          res.error = ceEncodingInvalid;
          break;
        }
      }

      return res;
    }

    String::ConversionResult<Char16> String::convertUTF16 (const Char16* s, size_t len)
    {
      if (len == (size_t)~0) len = std::char_traits<Char16>::length (s);

      ConversionResult<Char16> res;
      res.str.reserve (len);
      res.str.setLength (len);

      const Char16* p = s;
      const Char16* end = s + len;
      Char16* out_p = res.str.bufferPtr ();
      bool expectTailSurrogate = false;
      boost::optional<std::pair<const Char16*, ConversionError>> firstInvalid;
      while (p < end)
      {
        Char16 ch = *p;
        bool isLeadSurrogate = ((ch >= LeadSurrogateFirst) && (ch <= LeadSurrogateLast));
        bool isTailSurrogate = ((ch >= TailSurrogateFirst) && (ch <= TailSurrogateLast));
        if (expectTailSurrogate)
        {
          expectTailSurrogate = false;
          if (isTailSurrogate)
          {
            *out_p++ = ch;
          }
          else
          {
            // Invalidate lead surrogate
            --out_p;
            *out_p++ = ReplacementChar;
            // Emit char we encountered
            *out_p++ = ch;
            if (!firstInvalid) firstInvalid = std::make_pair (p, ceCharacterIncomplete);
            expectTailSurrogate = isLeadSurrogate;
          }
        }
        else
        {
          if (isTailSurrogate)
          {
            // Replace unexpected tail surrogate
            *out_p++ = ReplacementChar;
            if (!firstInvalid) firstInvalid = std::make_pair (p+1, ceCharacterInvalid);
          }
          else
          {
            *out_p++ = ch;
            expectTailSurrogate = isLeadSurrogate;
          }
        }
        p++;
      }
      if (expectTailSurrogate && !firstInvalid)
      {
        firstInvalid = std::make_pair (p, ceCharacterIncomplete);
      }
      *out_p = 0;

      if (firstInvalid)
      {
        res.invalidPos = firstInvalid->first;
        res.error = firstInvalid->second;
      }

      return res;
    }

    String::ConversionResult<Char32> String::convertUTF32 (const Char32* s, size_t len)
    {
      if (len == (size_t)~0) len = std::char_traits<Char32>::length (s);

      ConversionResult<Char32> res;
      const Char32* p = s;
      const Char32* end = s + len;
      boost::optional<std::pair<const Char32*, ConversionError>> firstInvalid;
      while (p < end)
      {
        size_t remaining = end - p;
        if ((res.str.capacity () - res.str.length ()) < remaining)
          res.str.reserveExtra (remaining);
        Char32 ch = *p++;
        res.str.append (ch);
        if (!firstInvalid && (res.str.bufferPtr ()[res.str.size () - 1] != ch))
        {
          firstInvalid = std::make_pair (p, ceCharacterInvalid);
        }
      }

      res.str.shrink_to_fit();
      if (firstInvalid)
      {
        res.invalidPos = firstInvalid->first;
        res.error = firstInvalid->second;
      }

      return res;
    }

    bool String::startsWith (const String& s) const
    {
      if (s.length() > length()) return false;
      return Char_traits::compare (data(), s.data(), s.length()) == 0;
    }

    bool String::startsWith (const char* s, size_t len) const
    {
      if (len == (size_t)~0) len = std::char_traits<char>::length (s);
      return boost::algorithm::starts_with (*this, boost::make_iterator_range (s, s+len));
    }
    
    String::size_type String::indexOf (Char32 ch) const
    {
      if (ch != SanitizeChar16(ch)) return npos;

      const Char* start = data();
      if (ch <= MaxChar16)
      {
        const Char* p = Char_traits::find (start, length(), static_cast<Char16> (ch));
        return p ? static_cast<size_type> (p - start) : npos;
      }
      else
      {
	if (length() < 2) return npos;

        Char surr[2];
        {
          UTF16Encoder enc;
          Char* dst = surr;
          UTF16Encoder::EncodeResult result = enc (ch, dst, dst+2);
          assert (result == UTF16Encoder::erSuccess);
        }
        const Char* end = start + length();
        const Char* p = start + 1;
        while (p && (p < end))
        {
	  p = Char_traits::find (p, end - p, surr[1]);
	  if (p && (*(p - 1) == surr[0])) return static_cast<size_type> (p - start - 1);
        }
        return npos;
      }
    }

    void String::FreeBuffer()
    {
      if (!IsBufferInternal())
      {
        ReleaseBufferData (BufferDataPtr());
        d.capacity = InternalBufferSize;
      }
    }

    void String::ResizeBuffer (size_type capacity)
    {
      bool currentIsInternal = IsBufferInternal();
      bool canUseInternal = capacity <= InternalBufferSize;
      if (canUseInternal)
      {
        if (!currentIsInternal)
        {
          assert (capacity < d.capacity); // Otherwise, couldn't use internal buffer
          auto oldData = BufferDataPtr();
	  Char_traits::copy (reinterpret_cast<Char*> (internalBuffer), oldData->data, capacity);
          ReleaseBufferData (oldData);
        }
      }
      else
      {
        if (currentIsInternal)
        {
          assert (d.capacity < capacity);
          // Allocate new buffer
          AllocatedBufferData* newBuffer = AllocBufferData (capacity);
          RefBufferData (newBuffer);
	  Char_traits::copy (newBuffer->data, bufferPtr(), d.capacity);
          heapBuffer = reinterpret_cast<buffer_value_type*> (newBuffer->data);
        }
        else
        {
          // Reallocate current buffer
          AllocatedBufferData* data = BufferDataPtr();
          if (!IsBufferUnique())
          {
            AllocatedBufferData* newBuffer = AllocBufferData (capacity);
            RefBufferData (newBuffer);
            Char_traits::copy (newBuffer->data, data->data, std::min (d.length, capacity));
            heapBuffer = reinterpret_cast<buffer_value_type*> (newBuffer->data);
            ReleaseBufferData (data);
          }
          else
          {
            if (d.capacity == capacity) return;
            heapBuffer = reinterpret_cast<buffer_value_type*> (ReallocBufferData(BufferDataPtr(), capacity)->data);
          }
        }
      }
      d.capacity = capacity;
    }

    String::AllocatedBufferData* String::BufferDataPtr() const
    {
      assert (!IsBufferInternal());
      return reinterpret_cast<AllocatedBufferData*> (
        reinterpret_cast<char*> (heapBuffer) - offsetof (AllocatedBufferData, data));
    }
    
    String::AllocatedBufferData* String::AllocBufferData (size_type numChars)
    {
      AllocatedBufferData* p = reinterpret_cast<AllocatedBufferData*> (
        malloc (offsetof (AllocatedBufferData, data) + numChars * sizeof (Char)));
      p->refCount.store (0);
      return p;
    }

    String::AllocatedBufferData* String::ReallocBufferData (AllocatedBufferData* p, size_type numChars)
    {
      AllocatedBufferData* new_p = reinterpret_cast<AllocatedBufferData*> (
        realloc (p, offsetof (AllocatedBufferData, data) + numChars * sizeof (Char)));
      if (!new_p)
        throw std::bad_alloc ();
      return new_p;
    }

    void String::RefBufferData (AllocatedBufferData* data)
    {
      data->refCount.fetch_add (1, boost::memory_order_relaxed);
    }
    
    void String::ReleaseBufferData (AllocatedBufferData* data)
    {
      if (data->refCount.fetch_sub (1, boost::memory_order_release) == 1)
      {
        boost::atomic_thread_fence(boost::memory_order_acquire);
        free (data);
      }
    }

    //
    // Normalization
    //

    #include "ucd/CanonicalDecomp.inc"
    #include "ucd/CombiningClass.inc"
    #include "ucd/NFD_QC_No.inc"

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
            for(Char32 mark : vec)
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
      pos = static_cast<size_type> (input - str.data());
      return ch;
    }

    //-----------------------------------------------------------------------

    void swap (String& s1, String& s2)
    {
      std::swap (s1.d, s2.d);
      /* heapBuffer is in an union with internalBuffer, so swapping that 
       * takes care of external pointer as well */
      std::swap (s1.internalBuffer, s2.internalBuffer);
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
