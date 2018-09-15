/*
    Shader1
    Copyright (c) 2010-2018 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#include <boost/test/unit_test.hpp>

#include "base/uc/UTF8to16Transcoder.h"

BOOST_AUTO_TEST_SUITE(UTF8to16Transcoder)

BOOST_AUTO_TEST_CASE(Empty)
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
  BOOST_CHECK_EQUAL(input, inputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output, outputBuf);
  result = transcoder.Flush (output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output, outputBuf);
}

BOOST_AUTO_TEST_CASE(OneChar)
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
  BOOST_CHECK_EQUAL(input, inputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 1);
  BOOST_CHECK_EQUAL(outputBuf[0], 'a');
  result = transcoder.Flush (output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 1);
}

BOOST_AUTO_TEST_CASE(MultiChar)
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
  BOOST_CHECK_EQUAL(input, inputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 5);
  BOOST_CHECK_EQUAL(outputBuf[0], 'H');
  BOOST_CHECK_EQUAL(outputBuf[1], 'e');
  BOOST_CHECK_EQUAL(outputBuf[2], 'l');
  BOOST_CHECK_EQUAL(outputBuf[3], 'l');
  BOOST_CHECK_EQUAL(outputBuf[4], 'o');
  result = transcoder.Flush (output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 5);
}

// Test with a character encoded in 2 bytes
BOOST_AUTO_TEST_CASE(UTF8Encoded1)
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
  BOOST_CHECK_EQUAL(input, inputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 5);
  BOOST_CHECK_EQUAL(outputBuf[0], 'H');
  BOOST_CHECK_EQUAL(outputBuf[1], 0xeb);
  BOOST_CHECK_EQUAL(outputBuf[2], 'l');
  BOOST_CHECK_EQUAL(outputBuf[3], 'l');
  BOOST_CHECK_EQUAL(outputBuf[4], 'o');
  result = transcoder.Flush (output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 5);
}

// Test with characters encoded in 3 bytes
BOOST_AUTO_TEST_CASE(UTF8Encoded2)
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
  BOOST_CHECK_EQUAL(input, inputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 5);
  BOOST_CHECK_EQUAL(outputBuf[0], 'H');
  BOOST_CHECK_EQUAL(outputBuf[1], 'e');
  BOOST_CHECK_EQUAL(outputBuf[2], 0x1e37);
  BOOST_CHECK_EQUAL(outputBuf[3], 0x1e37);
  BOOST_CHECK_EQUAL(outputBuf[4], 'o');
  result = transcoder.Flush (output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 5);
}

// Test with characters encoded in 4 bytes
BOOST_AUTO_TEST_CASE(UTF8Encoded3)
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
  BOOST_CHECK_EQUAL(input, inputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 5);
  BOOST_CHECK_EQUAL(outputBuf[0], 0xd83d);
  BOOST_CHECK_EQUAL(outputBuf[1], 0xde00);
  BOOST_CHECK_EQUAL(outputBuf[2], ' ');
  BOOST_CHECK_EQUAL(outputBuf[3], 0xd83d);
  BOOST_CHECK_EQUAL(outputBuf[4], 0xdca9);
  result = transcoder.Flush (output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 5);
}

// Test with malformed input (overlong encoding)
BOOST_AUTO_TEST_CASE(UTF8Malformed1)
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
  BOOST_CHECK_EQUAL(input - chars_utf8, 3);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trEncodingInvalid);
  BOOST_CHECK_EQUAL(output - outputBuf, 2);
  BOOST_CHECK_EQUAL(outputBuf[0], 'a');
  BOOST_CHECK_EQUAL(outputBuf[1], 0xfffd);
  result = transcoder.Flush (output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 2);
}

// Test with malformed input (broken encoding)
BOOST_AUTO_TEST_CASE(UTF8Malformed2)
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
  BOOST_CHECK_EQUAL(input - chars_utf8, 2);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trCharacterIncomplete);
  BOOST_CHECK_EQUAL(output - outputBuf, 2);
  BOOST_CHECK_EQUAL(outputBuf[0], 'a');
  BOOST_CHECK_EQUAL(outputBuf[1], 0xfffd);
  result = transcoder.Flush (output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 2);
}

// Test with malformed input (broken encoding)
BOOST_AUTO_TEST_CASE(UTF8Malformed2b)
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
  BOOST_CHECK_EQUAL(input - chars_utf8, 2);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trCharacterIncomplete);
  BOOST_CHECK_EQUAL(output - outputBuf, 2);
  BOOST_CHECK_EQUAL(outputBuf[0], 'a');
  BOOST_CHECK_EQUAL(outputBuf[1], 0xfffd);
  result = transcoder.Flush (output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 2);
}

// Test with malformed input (broken encoding) at end
BOOST_AUTO_TEST_CASE(UTF8Malformed2c)
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
  BOOST_CHECK_EQUAL(input - chars_utf8, 2);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trInputUnderrun);
  BOOST_CHECK_EQUAL(output - outputBuf, 1);
  BOOST_CHECK_EQUAL(outputBuf[0], 'a');
  result = transcoder.Flush (output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trCharacterIncomplete);
  BOOST_CHECK_EQUAL(outputBuf[1], 0xfffd);
  BOOST_CHECK_EQUAL(output - outputBuf, 2);
}

// Test with malformed input (broken encoding) at end
BOOST_AUTO_TEST_CASE(UTF8Malformed2d)
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
  BOOST_CHECK_EQUAL(input - chars_utf8, 2);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trCharacterIncomplete);
  BOOST_CHECK_EQUAL(output - outputBuf, 2);
  BOOST_CHECK_EQUAL(outputBuf[0], 'a');
  BOOST_CHECK_EQUAL(outputBuf[1], 0xfffd);
  result = transcoder.Flush (output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trSuccess);
}

// Test with malformed input (encoded surrogate half)
BOOST_AUTO_TEST_CASE(UTF8Malformed3)
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
  BOOST_CHECK_EQUAL(input - chars_utf8, 4);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trCharacterInvalid);
  BOOST_CHECK_EQUAL(output - outputBuf, 2);
  BOOST_CHECK_EQUAL(outputBuf[0], 'a');
  BOOST_CHECK_EQUAL(outputBuf[1], 0xfffd);
  result = transcoder.Flush (output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 2);
}

// Test with malformed input (encoded surrogate half)
BOOST_AUTO_TEST_CASE(UTF8Malformed4)
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
  BOOST_CHECK_EQUAL(input - chars_utf8, 4);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trCharacterInvalid);
  BOOST_CHECK_EQUAL(output - outputBuf, 2);
  BOOST_CHECK_EQUAL(outputBuf[0], 'a');
  BOOST_CHECK_EQUAL(outputBuf[1], 0xfffd);
  result = transcoder.Flush (output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 2);
}

// Test input underrun (in encoded 2 byte char)
BOOST_AUTO_TEST_CASE(InputUnderrun1)
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
  BOOST_CHECK_EQUAL(input - chars_utf8, 2);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trInputUnderrun);
  BOOST_CHECK_EQUAL(output - outputBuf, 1);
  BOOST_CHECK_EQUAL(outputBuf[0], 'H');
  result = transcoder (input, inputEnd, output, outputEnd);
  BOOST_CHECK_EQUAL(input, inputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 5);
  BOOST_CHECK_EQUAL(outputBuf[1], 0xeb);
  BOOST_CHECK_EQUAL(outputBuf[2], 'l');
  BOOST_CHECK_EQUAL(outputBuf[3], 'l');
  BOOST_CHECK_EQUAL(outputBuf[4], 'o');
  result = transcoder.Flush (output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 5);
}

// Test input underrun (in encoded 3 byte char)
BOOST_AUTO_TEST_CASE(InputUnderrun2)
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
  BOOST_CHECK_EQUAL(input - chars_utf8, 4);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trInputUnderrun);
  BOOST_CHECK_EQUAL(output - outputBuf, 2);
  BOOST_CHECK_EQUAL(outputBuf[0], 'H');
  BOOST_CHECK_EQUAL(outputBuf[1], 'e');
  result = transcoder (input, inputEnd, output, outputEnd);
  BOOST_CHECK_EQUAL(input, inputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 5);
  BOOST_CHECK_EQUAL(outputBuf[2], 0x1e37);
  BOOST_CHECK_EQUAL(outputBuf[3], 0x1e37);
  BOOST_CHECK_EQUAL(outputBuf[4], 'o');
  result = transcoder.Flush (output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 5);
}

// Test output overflow
BOOST_AUTO_TEST_CASE(OutputOverflow1)
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
  BOOST_CHECK_EQUAL(input - chars_utf8, 4);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trOutputOverflow);
  BOOST_CHECK_EQUAL(output - outputBuf, 3);
  BOOST_CHECK_EQUAL(outputBuf[0], 'H');
  BOOST_CHECK_EQUAL(outputBuf[1], 'e');
  BOOST_CHECK_EQUAL(outputBuf[2], 'l');
  result = transcoder (input, inputEnd, output, outputEnd);
  BOOST_CHECK_EQUAL(input, inputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 5);
  BOOST_CHECK_EQUAL(outputBuf[3], 'l');
  BOOST_CHECK_EQUAL(outputBuf[4], 'o');
  result = transcoder.Flush (output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 5);
}

// Test output overflow (straddling surrogate)
BOOST_AUTO_TEST_CASE(OutputOverflow2)
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
  BOOST_CHECK_EQUAL(input - chars_utf8, 6);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trOutputOverflow);
  BOOST_CHECK_EQUAL(output - outputBuf, 3);
  BOOST_CHECK_EQUAL(outputBuf[0], '-');
  BOOST_CHECK_EQUAL(outputBuf[1], '-');
  BOOST_CHECK_EQUAL(outputBuf[2], 0xd83d);
  result = transcoder (input, inputEnd, output, outputBuf+5);
  BOOST_CHECK_EQUAL(input - chars_utf8, 11);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trOutputOverflow);
  BOOST_CHECK_EQUAL(output - outputBuf, 5);
  BOOST_CHECK_EQUAL(outputBuf[3], 0xde00);
  BOOST_CHECK_EQUAL(outputBuf[4], ' ');
  result = transcoder (input, inputEnd, output, outputBuf+6);
  BOOST_CHECK_EQUAL(input - chars_utf8, 11);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trOutputOverflow);
  BOOST_CHECK_EQUAL(output - outputBuf, 6);
  BOOST_CHECK_EQUAL(outputBuf[5], 0xd83d);
  result = transcoder (input, inputEnd, output, outputEnd);
  BOOST_CHECK_EQUAL(input, inputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 9);
  BOOST_CHECK_EQUAL(outputBuf[6], 0xdca9);
  BOOST_CHECK_EQUAL(outputBuf[7], '-');
  BOOST_CHECK_EQUAL(outputBuf[8], '-');
  result = transcoder.Flush (output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 9);
}

// Test output overflow (straddling surrogate)
BOOST_AUTO_TEST_CASE(OutputOverflow2b)
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
  BOOST_CHECK_EQUAL(input - chars_utf8, 6);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trOutputOverflow);
  BOOST_CHECK_EQUAL(output - outputBuf, 3);
  BOOST_CHECK_EQUAL(outputBuf[0], '-');
  BOOST_CHECK_EQUAL(outputBuf[1], '-');
  BOOST_CHECK_EQUAL(outputBuf[2], 0xd83d);
  result = transcoder.Flush (output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 4);
  BOOST_CHECK_EQUAL(outputBuf[3], 0xde00);
}

// Test output overflow (after decode issue)
BOOST_AUTO_TEST_CASE(OutputOverflow3)
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
  BOOST_CHECK_EQUAL(input - chars_utf8, 3);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trOutputOverflow);
  BOOST_CHECK_EQUAL(output - outputBuf, 2);
  BOOST_CHECK_EQUAL(outputBuf[0], 'H');
  BOOST_CHECK_EQUAL(outputBuf[1], 'e');
  result = transcoder (input, inputEnd, output, outputEnd);
  BOOST_CHECK_EQUAL(input - chars_utf8, 3);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trCharacterIncomplete);
  BOOST_CHECK_EQUAL(output - outputBuf, 3);
  BOOST_CHECK_EQUAL(outputBuf[2], 0xfffd);
  result = transcoder (input, inputEnd, output, outputEnd);
  BOOST_CHECK_EQUAL(input, inputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 5);
  BOOST_CHECK_EQUAL(outputBuf[3], 'l');
  BOOST_CHECK_EQUAL(outputBuf[4], 'o');
  result = transcoder.Flush (output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 5);
}

// Test output overflow in Flush()
BOOST_AUTO_TEST_CASE(FlustOutputOverflow)
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
  BOOST_CHECK_EQUAL(input - chars_utf8, 2);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trInputUnderrun);
  BOOST_CHECK_EQUAL(output - outputBuf, 1);
  BOOST_CHECK_EQUAL(outputBuf[0], 'a');
  result = transcoder.Flush (output, output);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trOutputOverflow);
  result = transcoder.Flush (output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8to16Transcoder::trCharacterIncomplete);
  BOOST_CHECK_EQUAL(outputBuf[1], 0xfffd);
  BOOST_CHECK_EQUAL(output - outputBuf, 2);
}

BOOST_AUTO_TEST_SUITE_END()
