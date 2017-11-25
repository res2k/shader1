/*
    Shader1
    Copyright (c) 2010-2015 Frank Richter


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

#include "base/uc/UTF8to16Transcoder.h"

namespace s1
{
  namespace uc
  {
    UTF8to16Transcoder::UTF8to16Transcoder() : pendingChar (0),
      lastDecodeResult (UTF8Decoder::drSuccess)
    {
    }
    
    // Translate character-producing decode result
    static inline UTF8to16Transcoder::TranscodeResult TranslateDecodeResult (UTF8Decoder::DecodeResult result)
    {
      switch (result)
      {
      case UTF8Decoder::drCharacterIncomplete:      return UTF8to16Transcoder::trCharacterIncomplete;
      case UTF8Decoder::drCharacterInvalid:         return UTF8to16Transcoder::trCharacterInvalid;
      case UTF8Decoder::drEncodingInvalid:          return UTF8to16Transcoder::trEncodingInvalid;
      default:
        S1_ASSERT_NOT_REACHED (UTF8to16Transcoder::trSuccess); // Should already be handled
        break;
      }
      return UTF8to16Transcoder::trSuccess;
    }

    UTF8to16Transcoder::TranscodeResult
    UTF8to16Transcoder::operator() (const char*& srcStart, const char* srcEnd,
                                    Char16*& dstStart, Char16* dstEnd)
    {
      // Flush encoder if previous output was pending
      if (pendingChar)
      {
        UTF16Encoder::EncodeResult outResult = encoder (pendingChar, dstStart, dstEnd);
        if (outResult < 0)
        {
          // Still not enough buffer...
          return trOutputOverflow;
        }
        pendingChar = 0;
        // Also take previous inResult into account, return if appropriate
        if (lastDecodeResult > 0)
        {
          return TranslateDecodeResult (lastDecodeResult);
        }
      }
      
      while (srcStart < srcEnd)
      {
        Char32 ch;
        UTF8Decoder::DecodeResult inResult = decoder (srcStart, srcEnd, ch);
        lastDecodeResult = inResult;
        if (inResult < 0)
        {
          // No character was read
          return trInputUnderrun;
        }
        UTF16Encoder::EncodeResult outResult = encoder (ch, dstStart, dstEnd);
        if (outResult < 0)
        {
          // Pending output
          pendingChar = ch;
          return trOutputOverflow;
        }
        if (inResult > 0)
        {
          // Conversion problem
          return TranslateDecodeResult (inResult);
        }
      }
      return trSuccess;
    }

    UTF8to16Transcoder::TranscodeResult
    UTF8to16Transcoder::Flush (Char16*& dstStart, Char16* dstEnd)
    {
      // Flush encoder if previous output was pending
      if (pendingChar)
      {
        UTF16Encoder::EncodeResult outResult = encoder (pendingChar, dstStart, dstEnd);
        if (outResult < 0)
        {
          // Still not enough buffer...
          return trOutputOverflow;
        }
        pendingChar = 0;
        if (lastDecodeResult > 0)
        {
          return TranslateDecodeResult (lastDecodeResult);
        }
      }
      if (lastDecodeResult == UTF8Decoder::drInputUnderrun)
      {
        /* Flush()ing with the last decode result being an underrun means
         * the input buffer ends with an incomplete char.
         * Emit a replacer for it */
        lastDecodeResult = UTF8Decoder::drCharacterIncomplete;
        pendingChar = ReplacementChar;
        return Flush (dstStart, dstEnd);
      }
      return trSuccess;
    }
  } // namespace uc
} // namespace s1
