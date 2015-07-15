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
 * Transcode UTF-16 to UTF-8
 */
#ifndef __BASE_UC_UTF16TO8TRANSCODER_H__
#define __BASE_UC_UTF16TO8TRANSCODER_H__

#include "UTF16Decoder.h"
#include "UTF8Encoder.h"

namespace s1
{
  namespace uc
  {
    /**
     * Transcode UTF-16 to UTF-8
     */
    class UTF16to8Transcoder
    {
      /// UTF-16 decoder
      UTF16Decoder decoder;
      /// UTF-8 encoder
      UTF8Encoder encoder;
      /// Pending output after output overflow
      Char32 pendingChar;
      /// Result for last decode
      UTF16Decoder::DecodeResult lastDecodeResult;
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
        trCharacterInvalid = 2
      };

      UTF16to8Transcoder();
      
      /**
       * Transcode characters from UTF-16 to UTF-8.
       * \param srcStart Start of source buffer. Updated to point past the last
       *    consumed character. Points past problematic character in case of an error.
       * \param srcEnd End of source buffer.
       * \param dstStart Start of output buffer. Updated to point past the last written Char16 value.
       * \param dstEnd End of output buffer.
       * \returns Transcoding success. Values >= 0 mean that a problem was encountered and
       *   a replacement character was written. You can, however, immediately continue with
       *   transcoding.
       *   Negative values mean that you need to provide more input data (trInputUnderrun)
       *   or enlarge the output buffer (drOutputOverflow). Note that in the latter case
       *   some input characters will have been consumed already without their output being
       *   written.
       */
      TranscodeResult operator() (const Char16*& srcStart, const Char16* srcEnd,
                                  char*& dstStart, char* dstEnd);
      /**
       * Flush the internal state. Deals with incomplete output.
       * \param dstStart Start of output buffer. Updated to point past the last written Char16 value.
       * \param dstEnd End of output buffer.
       * \returns Transcoding success. Values >= 0 mean that a problem was encountered and
       *   a replacement character was written.
       *   Negative values mean that you need to enlarge the output buffer.
       */
      TranscodeResult Flush (char*& dstStart, char* dstEnd);
    };
  } // namespace uc
} // namespace s1

#endif // __BASE_UC_UTF16TO8TRANSCODER_H__

