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

#include <cxxtest/TestSuite.h>

#include "base/uc/UTF8to16Transcoder.h"

class UTF8to16TranscoderTestSuite : public CxxTest::TestSuite 
{
public:
  void testEmpty (void)
  {
    const char nullStr[] = "";
    s1::uc::UTF8to16Transcoder transcoder;
    const char* input = nullStr;
    const char* inputEnd = nullStr + sizeof(nullStr) - 1;
    s1::uc::Char outputBuf[16];
    s1::uc::Char* output = outputBuf;
    s1::uc::Char* outputEnd = outputBuf + (sizeof(outputBuf)/sizeof(outputBuf[0]));
    s1::uc::UTF8to16Transcoder::TranscodeResult result = transcoder (input, inputEnd,
                                                                     output, outputEnd);
    TS_ASSERT_EQUALS(input, inputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trSuccess);
    TS_ASSERT_EQUALS(output, outputBuf);
    result = transcoder.Flush (output, outputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trSuccess);
    TS_ASSERT_EQUALS(output, outputBuf);
  }

  void testOneChar (void)
  {
    const char str[] = "a";
    s1::uc::UTF8to16Transcoder transcoder;
    const char* input = str;
    const char* inputEnd = str + sizeof(str) - 1;
    s1::uc::Char outputBuf[16];
    s1::uc::Char* output = outputBuf;
    s1::uc::Char* outputEnd = outputBuf + (sizeof(outputBuf)/sizeof(outputBuf[0]));
    s1::uc::UTF8to16Transcoder::TranscodeResult result = transcoder (input, inputEnd,
                                                                     output, outputEnd);
    TS_ASSERT_EQUALS(input, inputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trSuccess);
    TS_ASSERT_EQUALS(output - outputBuf, 1);
    TS_ASSERT_EQUALS(outputBuf[0], 'a');
    result = transcoder.Flush (output, outputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trSuccess);
    TS_ASSERT_EQUALS(output - outputBuf, 1);
  }

  void testMultiChar (void)
  {
    const char str[] = "Hello";
    s1::uc::UTF8to16Transcoder transcoder;
    const char* input = str;
    const char* inputEnd = str + sizeof(str) - 1;
    s1::uc::Char outputBuf[16];
    s1::uc::Char* output = outputBuf;
    s1::uc::Char* outputEnd = outputBuf + (sizeof(outputBuf)/sizeof(outputBuf[0]));
    s1::uc::UTF8to16Transcoder::TranscodeResult result = transcoder (input, inputEnd,
                                                                     output, outputEnd);
    TS_ASSERT_EQUALS(input, inputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trSuccess);
    TS_ASSERT_EQUALS(output - outputBuf, 5);
    TS_ASSERT_EQUALS(outputBuf[0], 'H');
    TS_ASSERT_EQUALS(outputBuf[1], 'e');
    TS_ASSERT_EQUALS(outputBuf[2], 'l');
    TS_ASSERT_EQUALS(outputBuf[3], 'l');
    TS_ASSERT_EQUALS(outputBuf[4], 'o');
    result = transcoder.Flush (output, outputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trSuccess);
    TS_ASSERT_EQUALS(output - outputBuf, 5);
  }

  // Test with a character encoded in 2 bytes
  void testUTF8Encoded1 ()
  {
    const char chars_utf8[] = "H\xc3\xabllo";
    s1::uc::UTF8to16Transcoder transcoder;
    const char* input = chars_utf8;
    const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
    s1::uc::Char outputBuf[16];
    s1::uc::Char* output = outputBuf;
    s1::uc::Char* outputEnd = outputBuf + (sizeof(outputBuf)/sizeof(outputBuf[0]));
    s1::uc::UTF8to16Transcoder::TranscodeResult result = transcoder (input, inputEnd,
                                                                     output, outputEnd);
    TS_ASSERT_EQUALS(input, inputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trSuccess);
    TS_ASSERT_EQUALS(output - outputBuf, 5);
    TS_ASSERT_EQUALS(outputBuf[0], 'H');
    TS_ASSERT_EQUALS(outputBuf[1], 0xeb);
    TS_ASSERT_EQUALS(outputBuf[2], 'l');
    TS_ASSERT_EQUALS(outputBuf[3], 'l');
    TS_ASSERT_EQUALS(outputBuf[4], 'o');
    result = transcoder.Flush (output, outputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trSuccess);
    TS_ASSERT_EQUALS(output - outputBuf, 5);
  }

  // Test with characters encoded in 3 bytes
  void testUTF8Encoded2 ()
  {
    const char chars_utf8[] = "He\xe1\xb8\xb7\xe1\xb8\xb7o";
    s1::uc::UTF8to16Transcoder transcoder;
    const char* input = chars_utf8;
    const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
    s1::uc::Char outputBuf[16];
    s1::uc::Char* output = outputBuf;
    s1::uc::Char* outputEnd = outputBuf + (sizeof(outputBuf)/sizeof(outputBuf[0]));
    s1::uc::UTF8to16Transcoder::TranscodeResult result = transcoder (input, inputEnd,
                                                                     output, outputEnd);
    TS_ASSERT_EQUALS(input, inputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trSuccess);
    TS_ASSERT_EQUALS(output - outputBuf, 5);
    TS_ASSERT_EQUALS(outputBuf[0], 'H');
    TS_ASSERT_EQUALS(outputBuf[1], 'e');
    TS_ASSERT_EQUALS(outputBuf[2], 0x1e37);
    TS_ASSERT_EQUALS(outputBuf[3], 0x1e37);
    TS_ASSERT_EQUALS(outputBuf[4], 'o');
    result = transcoder.Flush (output, outputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trSuccess);
    TS_ASSERT_EQUALS(output - outputBuf, 5);
  }

  // Test with characters encoded in 4 bytes
  void testUTF8Encoded3 ()
  {
    const char chars_utf8[] = "\xf0\x9f\x98\x80 \xf0\x9f\x92\xa9";
    s1::uc::UTF8to16Transcoder transcoder;
    const char* input = chars_utf8;
    const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
    s1::uc::Char outputBuf[16];
    s1::uc::Char* output = outputBuf;
    s1::uc::Char* outputEnd = outputBuf + (sizeof(outputBuf)/sizeof(outputBuf[0]));
    s1::uc::UTF8to16Transcoder::TranscodeResult result = transcoder (input, inputEnd,
                                                                     output, outputEnd);
    TS_ASSERT_EQUALS(input, inputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trSuccess);
    TS_ASSERT_EQUALS(output - outputBuf, 5);
    TS_ASSERT_EQUALS(outputBuf[0], 0xd83d);
    TS_ASSERT_EQUALS(outputBuf[1], 0xde00);
    TS_ASSERT_EQUALS(outputBuf[2], ' ');
    TS_ASSERT_EQUALS(outputBuf[3], 0xd83d);
    TS_ASSERT_EQUALS(outputBuf[4], 0xdca9);
    result = transcoder.Flush (output, outputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trSuccess);
    TS_ASSERT_EQUALS(output - outputBuf, 5);
  }

  // Test with malformed input (overlong encoding)
  void testUTF8Malformed1 ()
  {
    const char chars_utf8[] = "a\xc0\x8a" "b";
    s1::uc::UTF8to16Transcoder transcoder;
    const char* input = chars_utf8;
    const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
    s1::uc::Char outputBuf[16];
    s1::uc::Char* output = outputBuf;
    s1::uc::Char* outputEnd = outputBuf + (sizeof(outputBuf)/sizeof(outputBuf[0]));
    s1::uc::UTF8to16Transcoder::TranscodeResult result = transcoder (input, inputEnd,
                                                                     output, outputEnd);
    TS_ASSERT_EQUALS(input - chars_utf8, 3);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trEncodingInvalid);
    TS_ASSERT_EQUALS(output - outputBuf, 2);
    TS_ASSERT_EQUALS(outputBuf[0], 'a');
    TS_ASSERT_EQUALS(outputBuf[1], 0xfffd);
    result = transcoder.Flush (output, outputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trSuccess);
    TS_ASSERT_EQUALS(output - outputBuf, 2);
  }

  // Test with malformed input (broken encoding)
  void testUTF8Malformed2 ()
  {
    const char chars_utf8[] = "a\xc3" "b";
    s1::uc::UTF8to16Transcoder transcoder;
    const char* input = chars_utf8;
    const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
    s1::uc::Char outputBuf[16];
    s1::uc::Char* output = outputBuf;
    s1::uc::Char* outputEnd = outputBuf + (sizeof(outputBuf)/sizeof(outputBuf[0]));
    s1::uc::UTF8to16Transcoder::TranscodeResult result = transcoder (input, inputEnd,
                                                                     output, outputEnd);
    TS_ASSERT_EQUALS(input - chars_utf8, 2);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trCharacterIncomplete);
    TS_ASSERT_EQUALS(output - outputBuf, 2);
    TS_ASSERT_EQUALS(outputBuf[0], 'a');
    TS_ASSERT_EQUALS(outputBuf[1], 0xfffd);
    result = transcoder.Flush (output, outputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trSuccess);
    TS_ASSERT_EQUALS(output - outputBuf, 2);
  }

  // Test with malformed input (broken encoding)
  void testUTF8Malformed2b ()
  {
    const char chars_utf8[] = "a\xab" "b";
    s1::uc::UTF8to16Transcoder transcoder;
    const char* input = chars_utf8;
    const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
    s1::uc::Char outputBuf[16];
    s1::uc::Char* output = outputBuf;
    s1::uc::Char* outputEnd = outputBuf + (sizeof(outputBuf)/sizeof(outputBuf[0]));
    s1::uc::UTF8to16Transcoder::TranscodeResult result = transcoder (input, inputEnd,
                                                                     output, outputEnd);
    TS_ASSERT_EQUALS(input - chars_utf8, 2);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trCharacterIncomplete);
    TS_ASSERT_EQUALS(output - outputBuf, 2);
    TS_ASSERT_EQUALS(outputBuf[0], 'a');
    TS_ASSERT_EQUALS(outputBuf[1], 0xfffd);
    result = transcoder.Flush (output, outputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trSuccess);
    TS_ASSERT_EQUALS(output - outputBuf, 2);
  }

  // Test with malformed input (broken encoding) at end
  void testUTF8Malformed2c ()
  {
    const char chars_utf8[] = "a\xc3";
    s1::uc::UTF8to16Transcoder transcoder;
    const char* input = chars_utf8;
    const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
    s1::uc::Char outputBuf[16];
    s1::uc::Char* output = outputBuf;
    s1::uc::Char* outputEnd = outputBuf + (sizeof(outputBuf)/sizeof(outputBuf[0]));
    s1::uc::UTF8to16Transcoder::TranscodeResult result = transcoder (input, inputEnd,
                                                                     output, outputEnd);
    TS_ASSERT_EQUALS(input - chars_utf8, 2);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trInputUnderrun);
    TS_ASSERT_EQUALS(output - outputBuf, 1);
    TS_ASSERT_EQUALS(outputBuf[0], 'a');
    result = transcoder.Flush (output, outputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trCharacterIncomplete);
    TS_ASSERT_EQUALS(outputBuf[1], 0xfffd);
    TS_ASSERT_EQUALS(output - outputBuf, 2);
  }

  // Test with malformed input (broken encoding) at end
  void testUTF8Malformed2d ()
  {
    const char chars_utf8[] = "a\xab";
    s1::uc::UTF8to16Transcoder transcoder;
    const char* input = chars_utf8;
    const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
    s1::uc::Char outputBuf[16];
    s1::uc::Char* output = outputBuf;
    s1::uc::Char* outputEnd = outputBuf + (sizeof(outputBuf)/sizeof(outputBuf[0]));
    s1::uc::UTF8to16Transcoder::TranscodeResult result = transcoder (input, inputEnd,
                                                                     output, outputEnd);
    TS_ASSERT_EQUALS(input - chars_utf8, 2);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trCharacterIncomplete);
    TS_ASSERT_EQUALS(output - outputBuf, 2);
    TS_ASSERT_EQUALS(outputBuf[0], 'a');
    TS_ASSERT_EQUALS(outputBuf[1], 0xfffd);
    result = transcoder.Flush (output, outputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trSuccess);
  }

  // Test with malformed input (encoded surrogate half)
  void testUTF8Malformed3 ()
  {
    const char chars_utf8[] = "a\xed\xa0\x80" "b";
    s1::uc::UTF8to16Transcoder transcoder;
    const char* input = chars_utf8;
    const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
    s1::uc::Char outputBuf[16];
    s1::uc::Char* output = outputBuf;
    s1::uc::Char* outputEnd = outputBuf + (sizeof(outputBuf)/sizeof(outputBuf[0]));
    s1::uc::UTF8to16Transcoder::TranscodeResult result = transcoder (input, inputEnd,
                                                                     output, outputEnd);
    TS_ASSERT_EQUALS(input - chars_utf8, 4);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trCharacterInvalid);
    TS_ASSERT_EQUALS(output - outputBuf, 2);
    TS_ASSERT_EQUALS(outputBuf[0], 'a');
    TS_ASSERT_EQUALS(outputBuf[1], 0xfffd);
    result = transcoder.Flush (output, outputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trSuccess);
    TS_ASSERT_EQUALS(output - outputBuf, 2);
  }

  // Test with malformed input (encoded surrogate half)
  void testUTF8Malformed4 ()
  {
    const char chars_utf8[] = "a\xed\xb0\x80" "b";
    s1::uc::UTF8to16Transcoder transcoder;
    const char* input = chars_utf8;
    const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
    s1::uc::Char outputBuf[16];
    s1::uc::Char* output = outputBuf;
    s1::uc::Char* outputEnd = outputBuf + (sizeof(outputBuf)/sizeof(outputBuf[0]));
    s1::uc::UTF8to16Transcoder::TranscodeResult result = transcoder (input, inputEnd,
                                                                     output, outputEnd);
    TS_ASSERT_EQUALS(input - chars_utf8, 4);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trCharacterInvalid);
    TS_ASSERT_EQUALS(output - outputBuf, 2);
    TS_ASSERT_EQUALS(outputBuf[0], 'a');
    TS_ASSERT_EQUALS(outputBuf[1], 0xfffd);
    result = transcoder.Flush (output, outputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trSuccess);
    TS_ASSERT_EQUALS(output - outputBuf, 2);
  }

  // Test input underrun (in encoded 2 byte char)
  void testInputUnderrun1 ()
  {
    const char chars_utf8[] = "H\xc3\xabllo";
    s1::uc::UTF8to16Transcoder transcoder;
    const char* input = chars_utf8;
    const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
    s1::uc::Char outputBuf[16];
    s1::uc::Char* output = outputBuf;
    s1::uc::Char* outputEnd = outputBuf + (sizeof(outputBuf)/sizeof(outputBuf[0]));
    s1::uc::UTF8to16Transcoder::TranscodeResult result = transcoder (input, chars_utf8 + 2,
                                                                     output, outputEnd);
    TS_ASSERT_EQUALS(input - chars_utf8, 2);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trInputUnderrun);
    TS_ASSERT_EQUALS(output - outputBuf, 1);
    TS_ASSERT_EQUALS(outputBuf[0], 'H');
    result = transcoder (input, inputEnd, output, outputEnd);
    TS_ASSERT_EQUALS(input, inputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trSuccess);
    TS_ASSERT_EQUALS(output - outputBuf, 5);
    TS_ASSERT_EQUALS(outputBuf[1], 0xeb);
    TS_ASSERT_EQUALS(outputBuf[2], 'l');
    TS_ASSERT_EQUALS(outputBuf[3], 'l');
    TS_ASSERT_EQUALS(outputBuf[4], 'o');
    result = transcoder.Flush (output, outputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trSuccess);
    TS_ASSERT_EQUALS(output - outputBuf, 5);
  }

  // Test input underrun (in encoded 3 byte char)
  void testInputUnderrun2 ()
  {
    const char chars_utf8[] = "He\xe1\xb8\xb7\xe1\xb8\xb7o";
    s1::uc::UTF8to16Transcoder transcoder;
    const char* input = chars_utf8;
    const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
    s1::uc::Char outputBuf[16];
    s1::uc::Char* output = outputBuf;
    s1::uc::Char* outputEnd = outputBuf + (sizeof(outputBuf)/sizeof(outputBuf[0]));
    s1::uc::UTF8to16Transcoder::TranscodeResult result = transcoder (input, chars_utf8 + 4,
                                                                     output, outputEnd);
    TS_ASSERT_EQUALS(input - chars_utf8, 4);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trInputUnderrun);
    TS_ASSERT_EQUALS(output - outputBuf, 2);
    TS_ASSERT_EQUALS(outputBuf[0], 'H');
    TS_ASSERT_EQUALS(outputBuf[1], 'e');
    result = transcoder (input, inputEnd, output, outputEnd);
    TS_ASSERT_EQUALS(input, inputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trSuccess);
    TS_ASSERT_EQUALS(output - outputBuf, 5);
    TS_ASSERT_EQUALS(outputBuf[2], 0x1e37);
    TS_ASSERT_EQUALS(outputBuf[3], 0x1e37);
    TS_ASSERT_EQUALS(outputBuf[4], 'o');
    result = transcoder.Flush (output, outputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trSuccess);
    TS_ASSERT_EQUALS(output - outputBuf, 5);
  }

  // Test output overflow
  void testOutputOverflow1 ()
  {
    const char chars_utf8[] = "Hello";
    s1::uc::UTF8to16Transcoder transcoder;
    const char* input = chars_utf8;
    const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
    s1::uc::Char outputBuf[16];
    s1::uc::Char* output = outputBuf;
    s1::uc::Char* outputEnd = outputBuf + (sizeof(outputBuf)/sizeof(outputBuf[0]));
    s1::uc::UTF8to16Transcoder::TranscodeResult result = transcoder (input, inputEnd,
                                                                     output, outputBuf + 3);
    TS_ASSERT_EQUALS(input - chars_utf8, 4);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trOutputOverflow);
    TS_ASSERT_EQUALS(output - outputBuf, 3);
    TS_ASSERT_EQUALS(outputBuf[0], 'H');
    TS_ASSERT_EQUALS(outputBuf[1], 'e');
    TS_ASSERT_EQUALS(outputBuf[2], 'l');
    result = transcoder (input, inputEnd, output, outputEnd);
    TS_ASSERT_EQUALS(input, inputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trSuccess);
    TS_ASSERT_EQUALS(output - outputBuf, 5);
    TS_ASSERT_EQUALS(outputBuf[3], 'l');
    TS_ASSERT_EQUALS(outputBuf[4], 'o');
    result = transcoder.Flush (output, outputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trSuccess);
    TS_ASSERT_EQUALS(output - outputBuf, 5);
  }

  // Test output overflow (straddling surrogate)
  void testOutputOverflow2 ()
  {
    const char chars_utf8[] = "--\xf0\x9f\x98\x80 \xf0\x9f\x92\xa9--";
    s1::uc::UTF8to16Transcoder transcoder;
    const char* input = chars_utf8;
    const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
    s1::uc::Char outputBuf[16];
    s1::uc::Char* output = outputBuf;
    s1::uc::Char* outputEnd = outputBuf + (sizeof(outputBuf)/sizeof(outputBuf[0]));
    s1::uc::UTF8to16Transcoder::TranscodeResult result = transcoder (input, inputEnd,
                                                                     output, outputBuf + 3);
    TS_ASSERT_EQUALS(input - chars_utf8, 6);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trOutputOverflow);
    TS_ASSERT_EQUALS(output - outputBuf, 3);
    TS_ASSERT_EQUALS(outputBuf[0], '-');
    TS_ASSERT_EQUALS(outputBuf[1], '-');
    TS_ASSERT_EQUALS(outputBuf[2], 0xd83d);
    result = transcoder (input, inputEnd, output, outputBuf+5);
    TS_ASSERT_EQUALS(input - chars_utf8, 11);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trOutputOverflow);
    TS_ASSERT_EQUALS(output - outputBuf, 5);
    TS_ASSERT_EQUALS(outputBuf[3], 0xde00);
    TS_ASSERT_EQUALS(outputBuf[4], ' ');
    result = transcoder (input, inputEnd, output, outputBuf+6);
    TS_ASSERT_EQUALS(input - chars_utf8, 11);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trOutputOverflow);
    TS_ASSERT_EQUALS(output - outputBuf, 6);
    TS_ASSERT_EQUALS(outputBuf[5], 0xd83d);
    result = transcoder (input, inputEnd, output, outputEnd);
    TS_ASSERT_EQUALS(input, inputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trSuccess);
    TS_ASSERT_EQUALS(output - outputBuf, 9);
    TS_ASSERT_EQUALS(outputBuf[6], 0xdca9);
    TS_ASSERT_EQUALS(outputBuf[7], '-');
    TS_ASSERT_EQUALS(outputBuf[8], '-');
    result = transcoder.Flush (output, outputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trSuccess);
    TS_ASSERT_EQUALS(output - outputBuf, 9);
  }

  // Test output overflow (straddling surrogate)
  void testOutputOverflow2b ()
  {
    const char chars_utf8[] = "--\xf0\x9f\x98\x80";
    s1::uc::UTF8to16Transcoder transcoder;
    const char* input = chars_utf8;
    const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
    s1::uc::Char outputBuf[16];
    s1::uc::Char* output = outputBuf;
    s1::uc::Char* outputEnd = outputBuf + (sizeof(outputBuf)/sizeof(outputBuf[0]));
    s1::uc::UTF8to16Transcoder::TranscodeResult result = transcoder (input, inputEnd,
                                                                     output, outputBuf + 3);
    TS_ASSERT_EQUALS(input - chars_utf8, 6);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trOutputOverflow);
    TS_ASSERT_EQUALS(output - outputBuf, 3);
    TS_ASSERT_EQUALS(outputBuf[0], '-');
    TS_ASSERT_EQUALS(outputBuf[1], '-');
    TS_ASSERT_EQUALS(outputBuf[2], 0xd83d);
    result = transcoder.Flush (output, outputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trSuccess);
    TS_ASSERT_EQUALS(output - outputBuf, 4);
    TS_ASSERT_EQUALS(outputBuf[3], 0xde00);
  }

  // Test output overflow (after decode issue)
  void testOutputOverflow3 ()
  {
    const char chars_utf8[] = "He\xc3lo";
    s1::uc::UTF8to16Transcoder transcoder;
    const char* input = chars_utf8;
    const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
    s1::uc::Char outputBuf[16];
    s1::uc::Char* output = outputBuf;
    s1::uc::Char* outputEnd = outputBuf + (sizeof(outputBuf)/sizeof(outputBuf[0]));
    s1::uc::UTF8to16Transcoder::TranscodeResult result = transcoder (input, inputEnd,
                                                                     output, outputBuf + 2);
    TS_ASSERT_EQUALS(input - chars_utf8, 3);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trOutputOverflow);
    TS_ASSERT_EQUALS(output - outputBuf, 2);
    TS_ASSERT_EQUALS(outputBuf[0], 'H');
    TS_ASSERT_EQUALS(outputBuf[1], 'e');
    result = transcoder (input, inputEnd, output, outputEnd);
    TS_ASSERT_EQUALS(input - chars_utf8, 3);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trCharacterIncomplete);
    TS_ASSERT_EQUALS(output - outputBuf, 3);
    TS_ASSERT_EQUALS(outputBuf[2], 0xfffd);
    result = transcoder (input, inputEnd, output, outputEnd);
    TS_ASSERT_EQUALS(input, inputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trSuccess);
    TS_ASSERT_EQUALS(output - outputBuf, 5);
    TS_ASSERT_EQUALS(outputBuf[3], 'l');
    TS_ASSERT_EQUALS(outputBuf[4], 'o');
    result = transcoder.Flush (output, outputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trSuccess);
    TS_ASSERT_EQUALS(output - outputBuf, 5);
  }

  // Test output overflow in Flush()
  void testFlustOutputOverflow ()
  {
    const char chars_utf8[] = "a\xc3";
    s1::uc::UTF8to16Transcoder transcoder;
    const char* input = chars_utf8;
    const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
    s1::uc::Char outputBuf[16];
    s1::uc::Char* output = outputBuf;
    s1::uc::Char* outputEnd = outputBuf + (sizeof(outputBuf)/sizeof(outputBuf[0]));
    s1::uc::UTF8to16Transcoder::TranscodeResult result = transcoder (input, inputEnd,
                                                                     output, outputEnd);
    TS_ASSERT_EQUALS(input - chars_utf8, 2);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trInputUnderrun);
    TS_ASSERT_EQUALS(output - outputBuf, 1);
    TS_ASSERT_EQUALS(outputBuf[0], 'a');
    result = transcoder.Flush (output, output);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trOutputOverflow);
    result = transcoder.Flush (output, outputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8to16Transcoder::trCharacterIncomplete);
    TS_ASSERT_EQUALS(outputBuf[1], 0xfffd);
    TS_ASSERT_EQUALS(output - outputBuf, 2);
  }
};
