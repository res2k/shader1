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
 * UTF-8 decoder
 */
#ifndef __BASE_UC_UTF8DECODER_H__
#define __BASE_UC_UTF8DECODER_H__

#include "Char.h"

namespace s1
{
  namespace uc
  {
    /// Stateful UTF-8 Decoder
    class UTF8Decoder
    {
      /// Currently decoded character
      s1::uc::Char32 decodeChar;
      /// Remaining trail bytes
      unsigned short trailRemaining;
      /// Minimum MSB of decodeChar (to detect overlong encoding)
      unsigned short minDecodeBits;
    public:
      UTF8Decoder () { Reset(); }

      /// Decoding result
      enum DecodeResult
      {
        /// Success (decoded a character)
        drSuccess = 0,
        /// Input buffer does not suffice to decode a character
        drInputUnderrun = -1,
        /// Incomplete byte sequence
        drCharacterIncomplete = 1,
        /// Invalid character (surrogate half or beyond last allowed code point)
        drCharacterInvalid = 2,
        /// Encoding invalid for character (overlong)
        drEncodingInvalid = 3
      };
      /**
       * Decode a single character.
       * \param input Start of input buffer. Updated to point past the last consumed byte(s).
       * \param inputEnd End of input buffer.
       * \param outChar Output character. A character is returned for any return value >= 0.
       * \returns Decoding success. 
       */
      DecodeResult operator() (const char*& input, const char* inputEnd, Char32& outChar)
      {
        s1::uc::Char32 finalChar = (s1::uc::Char32)~0;
        while (input < inputEnd)
        {
          unsigned char ch = static_cast<unsigned char> (*input++);
          if (trailRemaining > 0)
          {
            // Expect a trail byte
            if ((ch & 0xc0) != 0x80)
            {
              // Unexpected byte
              outChar = ReplacementChar;
              --input;
              Reset();
              return drCharacterIncomplete;
            }
            decodeChar = (decodeChar << 6) | (ch & 0x3f);
            trailRemaining--;
            if (trailRemaining == 0)
            {
              finalChar = decodeChar;
              break;
            }
            continue;
          }
          if (ch < 0x80)
          {
            // ASCII char
            minDecodeBits = 0;
            finalChar = ch;
            break;
          }
          else
          {
            // Expect a lead byte
            if ((ch & 0xc0) != 0xc0)
            {
              outChar = ReplacementChar;
              Reset();
              return drCharacterIncomplete;
            }
            trailRemaining = 1;
            while ((ch & (0x40 >> trailRemaining)) != 0)
            {
              trailRemaining++;
            }
            // In UTF-8 we have at max 6 bytes for an encoded character
            if (trailRemaining > 5)
            {
              outChar = ReplacementChar;
              Reset();
              return drEncodingInvalid;
            }
            decodeChar = ch & ((1 << (6 - trailRemaining))-1);
            minDecodeBits = (trailRemaining > 1) ? 1 + trailRemaining*5 : 7;
          }
        }
        
        if (finalChar != (s1::uc::Char32)~0)
        {
          // We produced a character, verify it
          if ((finalChar >= LeadSurrogateFirst) && (finalChar <= TailSurrogateLast))
          {
            // Character is a surrogate half
            outChar = ReplacementChar;
            return drCharacterInvalid;
          }
          else if (finalChar < (1 << minDecodeBits))
          {
            // Overlong encoding
            outChar = ReplacementChar;
            return drEncodingInvalid;
          }
          else if (finalChar > MaxChar)
          {
            // Out of Unicode range
            outChar = ReplacementChar;
            return drCharacterInvalid;
          }
          // Char looks good
          outChar = finalChar;
          return drSuccess;
        }
        
        return drInputUnderrun;
      }
      
      /// Reset internal state
      void Reset()
      {
        decodeChar = 0;
        trailRemaining = 0;
      }
    };
  } // namespace uc
} // namespace s1

#endif // __BASE_UC_UTF8DECODER_H__

