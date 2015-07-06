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
 * UTF-8 encoder
 */
#ifndef __BASE_UC_UTF8ENCODER_H__
#define __BASE_UC_UTF8ENCODER_H__

#include "Char.h"

namespace s1
{
  namespace uc
  {
    /// Stateful UTF-8 encoder
    class UTF8Encoder
    {
      /// Pending output bytes (packed)
      uint32_t pendingOutput;
    public:
      /**
       * Returns how many bytes are needed to encode \a ch.
       * \returns Value between 0 and 4, inclusive. 0 means the character can not be represented in UTF-8.
       */
      static unsigned int EncodedSize (Char32 ch)
      {
        if (ch > MaxChar)
          return 0;
        else if (ch >= 0x10000)
          return 4;
        else if (ch >= 0x800)
          return 3;
        else if (ch >= 0x80)
          return 2;
        else
          return 1;
      }

      UTF8Encoder () { Reset(); }

      /// Encoding result
      enum EncodeResult
      {
        /// Success (encoded a character)
        erSuccess = 0,
        /// Output buffer is does not sufficient to encode a character
        erOutputOverflow = -1,
        /// Invalid character (surrogate half)
        erCharacterInvalid = 1,
        /// Character can not be represented in UTF-8
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
      EncodeResult operator () (Char32 ch, char*& outputBuf, char* outputEnd)
      {
        // Generate output bytes
        if (pendingOutput == (uint32_t)~0)
        {
          if (ch > MaxChar)
            return erCharacterUnsupported;
          else if ((ch >= LeadSurrogateFirst) && (ch <= TailSurrogateLast))
            return erCharacterInvalid;
          else if (ch <= 0x7f)
          {
            pendingOutput = ch;
          }
          else
          {
            pendingOutput = 0;
            int numTail = 0;
            unsigned char leadMax = 0x3f;
            while (ch > leadMax)
            {
              unsigned char tailByte = (ch & 0x3f) | 0x80;
              pendingOutput = (pendingOutput | tailByte) << 8;
              ch = ch >> 6;
              numTail++;
              leadMax = leadMax >> 1;
            }
            unsigned char leadByte = static_cast<unsigned char> (ch) | ((0xff << (7 - numTail)) & 0xff);
            pendingOutput |= leadByte;
          }
        }
        
        while (outputBuf < outputEnd)
        {
          *outputBuf++ = pendingOutput & 0xff;
          pendingOutput = pendingOutput >> 8;
          if (pendingOutput == 0)
          {
            pendingOutput = (uint32_t)~0;
            return erSuccess;
          }
        }

        return erOutputOverflow;
      }

      /// Reset encoder state
      void Reset()
      {
        pendingOutput = (uint32_t)~0;
      }
    };
  } // namespace uc
} // namespace s1

#endif // __BASE_UC_UTF8ENCODER_H__

