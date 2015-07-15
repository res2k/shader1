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

#include "base/uc/UTF16to8Transcoder.h"

namespace s1
{
  namespace uc
  {
    UTF16to8Transcoder::UTF16to8Transcoder() : pendingChar (0),
      lastDecodeResult (UTF16Decoder::drSuccess)
    {
    }
    
    // Translate character-producing decode result
    static inline UTF16to8Transcoder::TranscodeResult TranslateDecodeResult (UTF16Decoder::DecodeResult result)
    {
      switch (result)
      {
      case UTF16Decoder::drCharacterIncomplete:      return UTF16to8Transcoder::trCharacterIncomplete;
      default:
        assert(false); // Should already be handled
        break;
      }
      return UTF16to8Transcoder::trSuccess;
    }

    UTF16to8Transcoder::TranscodeResult
    UTF16to8Transcoder::operator() (const Char16*& srcStart, const Char16* srcEnd,
                                    char*& dstStart, char* dstEnd)
    {
      // Flush encoder if previous output was pending
      if (pendingChar)
      {
        UTF8Encoder::EncodeResult outResult = encoder (pendingChar, dstStart, dstEnd);
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
        UTF16Decoder::DecodeResult inResult = decoder (srcStart, srcEnd, ch);
        lastDecodeResult = inResult;
        if (inResult < 0)
        {
          // No character was read
          return trInputUnderrun;
        }
        UTF8Encoder::EncodeResult outResult = encoder (ch, dstStart, dstEnd);
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

    UTF16to8Transcoder::TranscodeResult
    UTF16to8Transcoder::Flush (char*& dstStart, char* dstEnd)
    {
      // Flush encoder if previous output was pending
      if (pendingChar)
      {
        UTF8Encoder::EncodeResult outResult = encoder (pendingChar, dstStart, dstEnd);
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
      if (lastDecodeResult == UTF16Decoder::drInputUnderrun)
      {
        /* Flush()ing with the last decode result being an underrun means
         * the input buffer ends with an incomplete char.
         * Emit a replacer for it */
        lastDecodeResult = UTF16Decoder::drCharacterIncomplete;
        pendingChar = ReplacementChar;
        return Flush (dstStart, dstEnd);
      }
      return trSuccess;
    }
  } // namespace uc
} // namespace s1
