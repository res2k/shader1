/*
    Shader1
    Copyright (c) 2015 Frank Richter


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
 * UTF-16 encoder
 */
#ifndef __BASE_UC_UTF16ENCODER_H__
#define __BASE_UC_UTF16ENCODER_H__

#include "Char.h"

namespace s1
{
  namespace uc
  {
    /// Stateful UTF-16 encoder
    class UTF16Encoder
    {
      /// Surrogate half pending output
      Char16 pendingHalf;
    public:
      /**
       * Returns how many Char16 values are needed to encode \a ch.
       * \returns 0, 1 or 2. 0 means the character can not be represented in UTF-16.
       */
      static unsigned int EncodedSize (Char32 ch)
      {
        if (ch <= MaxChar16)
          return 1;
        else if (ch <= MaxChar)
          return 2;
        else
          return 0;
      }

      UTF16Encoder () { Reset(); }

      /// Encoding result
      enum EncodeResult
      {
        /// Success (encoded a character)
        erSuccess = 0,
        /// Output buffer is does not sufficient to encode a character
        erOutputOverflow = -1,
        /// Invalid character (surrogate half)
        erCharacterInvalid = 1,
        /// Character can not be represented in UTF-16
        erCharacterUnsupported = 2
      };
      /**
       * Encode a single character.
       * \param ch Character to encode.
       * \param output Start of output buffer. Updated to point past the last written Char16 value.
       * \param outputEnd End of output buffer.
       * \returns Encoding success. Values >= 0 mean that the next character can be encoded.
       *   Negative values mean that there's pending output, call the function again to obtain the
       *   remaining output, but do not advance the input.
       */
      EncodeResult operator () (Char32 ch, Char16*& outputBuf, Char16* outputEnd)
      {
        if (outputBuf >= outputEnd) return erOutputOverflow;
        
        if (pendingHalf != 0)
        {
          *outputBuf++ = pendingHalf;
          pendingHalf = 0;
          return erSuccess;
        }

        if (ch > MaxChar)
          return erCharacterUnsupported;
        else if ((ch >= LeadSurrogateFirst) && (ch <= TailSurrogateLast))
          return erCharacterInvalid;
        else if (ch > MaxChar16)
        {
          ch -= (MaxChar16 + 1);
          s1::uc::Char16 surrogate1 = LeadSurrogateFirst | static_cast<Char16> (ch >> 10);
          s1::uc::Char16 surrogate2 = TailSurrogateFirst | static_cast<Char16> (ch & 0x3ff);
          *outputBuf++ = surrogate1;
          if (outputBuf < outputEnd)
          {
            *outputBuf++ = surrogate2;
            return erSuccess;
          }
          else
          {
            pendingHalf = surrogate2;
            return erOutputOverflow;
          }
        }
          
        *outputBuf++ = static_cast<s1::uc::Char16> (ch);
        return erSuccess;
      }

      /// Reset encoder state
      void Reset()
      {
        pendingHalf = 0;
      }
    };
  } // namespace uc
} // namespace s1

#endif // __BASE_UC_UTF16ENCODER_H__

