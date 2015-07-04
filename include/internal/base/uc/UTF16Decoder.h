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
 * UTF-16 decoder
 */
#ifndef __BASE_UC_UTF16DECODER_H__
#define __BASE_UC_UTF16DECODER_H__

#include "Char.h"

namespace s1
{
  namespace uc
  {
    /// Stateful UTF-8 Decoder
    class UTF16Decoder
    {
      /// Currently pending lead surrogate
      s1::uc::Char16 leadSurrogate;
    public:
      UTF16Decoder () { Reset(); }

      /// Decoding result
      enum DecodeResult
      {
        /// Success (decoded a character)
        drSuccess = 0,
        /// Input buffer does not suffice to decode a character
        drInputUnderrun = -1,
        /// Incomplete byte sequence (surrogate half)
        drCharacterIncomplete = 1
      };
      /**
       * Decode a single character.
       * \param input Start of input buffer. Updated to point past the last consumed character(s).
       * \param inputEnd End of input buffer.
       * \param outChar Output character. A character is returned for any return value >= 0.
       * \returns Decoding success. 
       */
      DecodeResult operator() (const Char16*& input, const Char16* inputEnd, Char32& outChar)
      {
        while (input < inputEnd)
        {
          Char16 ch = *input++;
          if (leadSurrogate != 0)
          {
            if ((ch >= TailSurrogateFirst) && (ch <= TailSurrogateLast))
            {
              outChar = 0x10000 + (((leadSurrogate & 0x3ff) << 10) | (ch & 0x3ff));
              leadSurrogate = 0;
              return drSuccess;
            }
            else
            {
              // Lead surrogate w/o tail surrogate
              outChar = ReplacementChar;
              --input;
              Reset();
              return drCharacterIncomplete;
            }
          }
          else
          {
            if ((ch >= LeadSurrogateFirst) && (ch <= LeadSurrogateLast))
            {
              leadSurrogate = ch;
            }
            else if ((ch >= TailSurrogateFirst) && (ch <= TailSurrogateLast))
            {
              // Tail surrogate w/o lead surrogate
              outChar = ReplacementChar;
              Reset();
              return drCharacterIncomplete;
            }
            else
            {
              outChar = ch;
              return drSuccess;
            }
          }
        }
        return drInputUnderrun;
      }
      
      /// Reset internal state
      void Reset()
      {
        leadSurrogate = 0;
      }
    };
  } // namespace uc
} // namespace s1

#endif // __BASE_UC_UTF16DECODER_H__

