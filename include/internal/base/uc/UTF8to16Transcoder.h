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
 * Transcode UTF-8 to UTF-16
 */
#ifndef __BASE_UC_UTF8TO16TRANSCODER_H__
#define __BASE_UC_UTF8TO16TRANSCODER_H__

#include "UTF16Encoder.h"
#include "UTF8Decoder.h"

namespace s1
{
  namespace uc
  {
    /**
     * Transcode UTF-8 to UTF-16
     */
    class UTF8to16Transcoder
    {
      /// UTF-8 decoder
      UTF8Decoder decoder;
      /// UTF-16 encoder
      UTF16Encoder encoder;
      /// Pending output after output overflow
      Char32 pendingChar;
      /// Result for last decode
      UTF8Decoder::DecodeResult lastDecodeResult;
    public:
      /// Transcoding result
      enum TranscodeResult
      {
        /// Success (all input transcoded)
        trSuccess = 0,
        /// Input buffer does not suffice to decode a character
        trInputUnderrun = -1,
        /// Output buffer is does not suffice to encode all characters
        trOutputOverflow = -2,
        /// Incomplete byte sequence
        trCharacterIncomplete = 1,
        /// Invalid character (surrogate half)
        trCharacterInvalid = 2,
        /// Encoding invalid for character (overlong)
        trEncodingInvalid = 3
      };

      UTF8to16Transcoder();
      
      /**
       * Transcode characters from UTF-8 to UTF-16.
       * \param srcStart Start of source buffer. Updated to point past the last
       *    consumed byte(s). Points past problematic bytes in case of an error.
       * \param srcEnd End of source buffer.
       * \param dstStart Start of output buffer. Updated to point past the last written Char16 value.
       * \param dstEnd End of output buffer.
       * \returns Transcoding success. Values >= 0 mean that a problem was encountered and
       *   a replacement character was written. You can, however, immediately continue with
       *   transcoding.
       *   Negative values mean that you need to provide more input data (trInputUnderrun)
       *   or enlarge the output buffer (drOutputOverflow). Note that in the latter case
       *   some input bytes will have been consumed already without their output being
       *   written.
       */
      TranscodeResult operator() (const char*& srcStart, const char* srcEnd,
                                  Char16*& dstStart, Char16* dstEnd);
      /**
       * Flush the internal state. Deals with incomplete output.
       * \param dstStart Start of output buffer. Updated to point past the last written Char16 value.
       * \param dstEnd End of output buffer.
       * \returns Transcoding success. Values >= 0 mean that a problem was encountered and
       *   a replacement character was written.
       *   Negative values mean that you need to enlarge the output buffer.
       */
      TranscodeResult Flush (Char16*& dstStart, Char16* dstEnd);
    };
  } // namespace uc
} // namespace s1

#endif // __BASE_UC_UTF8TO16TRANSCODER_H__

