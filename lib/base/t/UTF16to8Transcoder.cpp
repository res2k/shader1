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

#include "base/uc/UTF16to8Transcoder.h"

BOOST_AUTO_TEST_SUITE(UTF16to8Transcoder)

BOOST_AUTO_TEST_CASE(Empty)
{
  const s1::uc::Char16 nullStr[] = { 0 };
  s1::uc::UTF16to8Transcoder transcoder;
  const s1::uc::Char16* input = nullStr;
  const s1::uc::Char16* inputEnd = nullStr;
  char outputBuf[16];
  char* output = outputBuf;
  char* outputEnd = outputBuf + sizeof(outputBuf);
  s1::uc::UTF16to8Transcoder::TranscodeResult result = transcoder (input, inputEnd,
                                                                    output, outputEnd);
  BOOST_CHECK_EQUAL(input, inputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16to8Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output, outputBuf);
  result = transcoder.Flush (output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16to8Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output, outputBuf);
}

BOOST_AUTO_TEST_CASE(OneChar)
{
  const s1::uc::Char16 str[] = { 'a' };
  s1::uc::UTF16to8Transcoder transcoder;
  const s1::uc::Char16* input = str;
  const s1::uc::Char16* inputEnd = str + sizeof(str)/sizeof(str[0]);
  char outputBuf[16];
  char* output = outputBuf;
  char* outputEnd = outputBuf + sizeof(outputBuf);
  s1::uc::UTF16to8Transcoder::TranscodeResult result = transcoder (input, inputEnd,
                                                                    output, outputEnd);
  BOOST_CHECK_EQUAL(input, inputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16to8Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 1);
  *output = 0;
  BOOST_CHECK_EQUAL(outputBuf, "a");
  result = transcoder.Flush (output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16to8Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 1);
}

BOOST_AUTO_TEST_CASE(MultiChar)
{
  const s1::uc::Char16 str[] = { 'H', 'e', 'l', 'l', 'o' };
  s1::uc::UTF16to8Transcoder transcoder;
  const s1::uc::Char16* input = str;
  const s1::uc::Char16* inputEnd = str + sizeof(str)/sizeof(str[0]);
  char outputBuf[16];
  char* output = outputBuf;
  char* outputEnd = outputBuf + sizeof(outputBuf);
  s1::uc::UTF16to8Transcoder::TranscodeResult result = transcoder (input, inputEnd,
                                                                    output, outputEnd);
  BOOST_CHECK_EQUAL(input, inputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16to8Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 5);
  *output = 0;
  BOOST_CHECK_EQUAL(outputBuf, "Hello");
  result = transcoder.Flush (output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16to8Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 5);
}

// Test with a character encoded in 2 bytes
BOOST_AUTO_TEST_CASE(UTF8Encoded1)
{
  const s1::uc::Char16 str[] = { 'H', 0xeb, 'l', 'l', 'o' };
  s1::uc::UTF16to8Transcoder transcoder;
  const s1::uc::Char16* input = str;
  const s1::uc::Char16* inputEnd = str + sizeof(str)/sizeof(str[0]);
  char outputBuf[16];
  char* output = outputBuf;
  char* outputEnd = outputBuf + sizeof(outputBuf);
  s1::uc::UTF16to8Transcoder::TranscodeResult result = transcoder (input, inputEnd,
                                                                    output, outputEnd);
  BOOST_CHECK_EQUAL(input, inputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16to8Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 6);
  *output = 0;
  BOOST_CHECK_EQUAL(outputBuf, "H\xc3\xabllo");
  result = transcoder.Flush (output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16to8Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 6);
}

// Test with characters encoded in 3 bytes
BOOST_AUTO_TEST_CASE(UTF8Encoded2)
{
  const s1::uc::Char16 str[] = { 'H', 'e', 0x1e37, 0x1e37, 'o' };
  s1::uc::UTF16to8Transcoder transcoder;
  const s1::uc::Char16* input = str;
  const s1::uc::Char16* inputEnd = str + sizeof(str)/sizeof(str[0]);
  char outputBuf[16];
  char* output = outputBuf;
  char* outputEnd = outputBuf + sizeof(outputBuf);
  s1::uc::UTF16to8Transcoder::TranscodeResult result = transcoder (input, inputEnd,
                                                                    output, outputEnd);
  BOOST_CHECK_EQUAL(input, inputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16to8Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 9);
  *output = 0;
  BOOST_CHECK_EQUAL(outputBuf, "He\xe1\xb8\xb7\xe1\xb8\xb7o");
  result = transcoder.Flush (output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16to8Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 9);
}

// Test with characters encoded in 4 bytes
BOOST_AUTO_TEST_CASE(UTF8Encoded3)
{
  const s1::uc::Char16 str[] = { 0xd83d, 0xde00, ' ', 0xd83d, 0xdca9 };
  s1::uc::UTF16to8Transcoder transcoder;
  const s1::uc::Char16* input = str;
  const s1::uc::Char16* inputEnd = str + sizeof(str)/sizeof(str[0]);
  char outputBuf[16];
  char* output = outputBuf;
  char* outputEnd = outputBuf + sizeof(outputBuf);
  s1::uc::UTF16to8Transcoder::TranscodeResult result = transcoder (input, inputEnd,
                                                                    output, outputEnd);
  BOOST_CHECK_EQUAL(input, inputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16to8Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 9);
  *output = 0;
  BOOST_CHECK_EQUAL(outputBuf, "\xf0\x9f\x98\x80 \xf0\x9f\x92\xa9");
  result = transcoder.Flush (output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16to8Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 9);
}

// Test with malformed input (surrogate half)
BOOST_AUTO_TEST_CASE(UTF8Malformed1)
{
  const s1::uc::Char16 str[] = { 'a', 0xd83d, 'b' };
  s1::uc::UTF16to8Transcoder transcoder;
  const s1::uc::Char16* input = str;
  const s1::uc::Char16* inputEnd = str + sizeof(str)/sizeof(str[0]);
  char outputBuf[16];
  char* output = outputBuf;
  char* outputEnd = outputBuf + sizeof(outputBuf);
  s1::uc::UTF16to8Transcoder::TranscodeResult result = transcoder (input, inputEnd,
                                                                    output, outputEnd);
  BOOST_CHECK_EQUAL(input - str, 2);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16to8Transcoder::trCharacterIncomplete);
  BOOST_CHECK_EQUAL(output - outputBuf, 4);
  *output = 0;
  BOOST_CHECK_EQUAL(outputBuf, "a\xef\xbf\xbd");
  result = transcoder (input, inputEnd, output, outputEnd);
  BOOST_CHECK_EQUAL(input, inputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16to8Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 5);
  *output = 0;
  BOOST_CHECK_EQUAL(outputBuf, "a\xef\xbf\xbd" "b");
  result = transcoder.Flush (output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16to8Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 5);
}

// Test with malformed input (surrogate half)
BOOST_AUTO_TEST_CASE(UTF8Malformed2)
{
  const s1::uc::Char16 str[] = { 'a', 0xdc9a, 'b' };
  s1::uc::UTF16to8Transcoder transcoder;
  const s1::uc::Char16* input = str;
  const s1::uc::Char16* inputEnd = str + sizeof(str)/sizeof(str[0]);
  char outputBuf[16];
  char* output = outputBuf;
  char* outputEnd = outputBuf + sizeof(outputBuf);
  s1::uc::UTF16to8Transcoder::TranscodeResult result = transcoder (input, inputEnd,
                                                                    output, outputEnd);
  BOOST_CHECK_EQUAL(input - str, 2);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16to8Transcoder::trCharacterIncomplete);
  BOOST_CHECK_EQUAL(output - outputBuf, 4);
  *output = 0;
  BOOST_CHECK_EQUAL(outputBuf, "a\xef\xbf\xbd");
  result = transcoder (input, inputEnd, output, outputEnd);
  BOOST_CHECK_EQUAL(input, inputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16to8Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 5);
  *output = 0;
  BOOST_CHECK_EQUAL(outputBuf, "a\xef\xbf\xbd" "b");
  result = transcoder.Flush (output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16to8Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 5);
}

// Test with malformed input at end
BOOST_AUTO_TEST_CASE(UTF8Malformed3)
{
  const s1::uc::Char16 str[] = { 'a', 0xd83d };
  s1::uc::UTF16to8Transcoder transcoder;
  const s1::uc::Char16* input = str;
  const s1::uc::Char16* inputEnd = str + sizeof(str)/sizeof(str[0]);
  char outputBuf[16];
  char* output = outputBuf;
  char* outputEnd = outputBuf + sizeof(outputBuf);
  s1::uc::UTF16to8Transcoder::TranscodeResult result = transcoder (input, inputEnd,
                                                                    output, outputEnd);
  BOOST_CHECK_EQUAL(input, inputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16to8Transcoder::trInputUnderrun);
  BOOST_CHECK_EQUAL(output - outputBuf, 1);
  *output = 0;
  BOOST_CHECK_EQUAL(outputBuf, "a");
  result = transcoder.Flush (output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16to8Transcoder::trCharacterIncomplete);
  BOOST_CHECK_EQUAL(output - outputBuf, 4);
  *output = 0;
  BOOST_CHECK_EQUAL(outputBuf, "a\xef\xbf\xbd");
}

// Test with malformed input (broken encoding) at end
BOOST_AUTO_TEST_CASE(UTF8Malformed4)
{
  const s1::uc::Char16 str[] = { 'a', 0xdc9a };
  s1::uc::UTF16to8Transcoder transcoder;
  const s1::uc::Char16* input = str;
  const s1::uc::Char16* inputEnd = str + sizeof(str)/sizeof(str[0]);
  char outputBuf[16];
  char* output = outputBuf;
  char* outputEnd = outputBuf + sizeof(outputBuf);
  s1::uc::UTF16to8Transcoder::TranscodeResult result = transcoder (input, inputEnd,
                                                                    output, outputEnd);
  BOOST_CHECK_EQUAL(input, inputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16to8Transcoder::trCharacterIncomplete);
  BOOST_CHECK_EQUAL(output - outputBuf, 4);
  *output = 0;
  BOOST_CHECK_EQUAL(outputBuf, "a\xef\xbf\xbd");
  result = transcoder.Flush (output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16to8Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 4);
}

// Test input underrun (in surrogate pair)
BOOST_AUTO_TEST_CASE(InputUnderrun1)
{
  const s1::uc::Char16 str[] = { 'a', 0xd83d, 0xde00, 'b' };
  s1::uc::UTF16to8Transcoder transcoder;
  const s1::uc::Char16* input = str;
  const s1::uc::Char16* inputEnd = str + sizeof(str)/sizeof(str[0]);
  char outputBuf[16];
  char* output = outputBuf;
  char* outputEnd = outputBuf + sizeof(outputBuf);
  s1::uc::UTF16to8Transcoder::TranscodeResult result = transcoder (input, input+2,
                                                                    output, outputEnd);
  BOOST_CHECK_EQUAL(input - str, 2);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16to8Transcoder::trInputUnderrun);
  BOOST_CHECK_EQUAL(output - outputBuf, 1);
  *output = 0;
  BOOST_CHECK_EQUAL(outputBuf, "a");
  result = transcoder (input, inputEnd, output, outputEnd);
  BOOST_CHECK_EQUAL(input, inputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16to8Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 6);
  *output = 0;
  BOOST_CHECK_EQUAL(outputBuf, "a\xf0\x9f\x98\x80" "b");
  result = transcoder.Flush (output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16to8Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 6);
}

// Test output overflow
BOOST_AUTO_TEST_CASE(OutputOverflow1)
{
  const s1::uc::Char16 str[] = { 'H', 'e', 'l', 'l', 'o' };
  s1::uc::UTF16to8Transcoder transcoder;
  const s1::uc::Char16* input = str;
  const s1::uc::Char16* inputEnd = str + sizeof(str)/sizeof(str[0]);
  char outputBuf[16];
  char* output = outputBuf;
  char* outputEnd = outputBuf + sizeof(outputBuf);
  s1::uc::UTF16to8Transcoder::TranscodeResult result = transcoder (input, inputEnd,
                                                                    output, output+2);
  BOOST_CHECK_EQUAL(input - str, 3);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16to8Transcoder::trOutputOverflow);
  BOOST_CHECK_EQUAL(output - outputBuf, 2);
  *output = 0;
  BOOST_CHECK_EQUAL(outputBuf, "He");
  result = transcoder (input, inputEnd, output, outputEnd);
  BOOST_CHECK_EQUAL(input, inputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16to8Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 5);
  *output = 0;
  BOOST_CHECK_EQUAL(outputBuf, "Hello");
  result = transcoder.Flush (output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16to8Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 5);
}

// Test output overflow (straddling character encoded to 2 bytes)
BOOST_AUTO_TEST_CASE(OutputOverflow2)
{
  const s1::uc::Char16 str[] = { 'H', 0xeb, 'l', 'l', 'o' };
  s1::uc::UTF16to8Transcoder transcoder;
  const s1::uc::Char16* input = str;
  const s1::uc::Char16* inputEnd = str + sizeof(str)/sizeof(str[0]);
  char outputBuf[16];
  char* output = outputBuf;
  char* outputEnd = outputBuf + sizeof(outputBuf);
  s1::uc::UTF16to8Transcoder::TranscodeResult result = transcoder (input, inputEnd,
                                                                    output, output+2);

  BOOST_CHECK_EQUAL(input - str, 2);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16to8Transcoder::trOutputOverflow);
  BOOST_CHECK_EQUAL(output - outputBuf, 2);
  *output = 0;
  BOOST_CHECK_EQUAL(outputBuf, "H\xc3");
  result = transcoder (input, inputEnd, output, outputEnd);
  BOOST_CHECK_EQUAL(input, inputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16to8Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 6);
  *output = 0;
  BOOST_CHECK_EQUAL(outputBuf, "H\xc3\xabllo");
  result = transcoder.Flush (output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16to8Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 6);
}

// Test output overflow (after decode issue)
BOOST_AUTO_TEST_CASE(OutputOverflow3)
{
  const s1::uc::Char16 str[] = { 'a', 0xd83d, 'b' };
  s1::uc::UTF16to8Transcoder transcoder;
  const s1::uc::Char16* input = str;
  const s1::uc::Char16* inputEnd = str + sizeof(str)/sizeof(str[0]);
  char outputBuf[16];
  char* output = outputBuf;
  char* outputEnd = outputBuf + sizeof(outputBuf);
  s1::uc::UTF16to8Transcoder::TranscodeResult result = transcoder (input, inputEnd,
                                                                    output, output+3);
  BOOST_CHECK_EQUAL(input - str, 2);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16to8Transcoder::trOutputOverflow);
  BOOST_CHECK_EQUAL(output - outputBuf, 3);
  *output = 0;
  BOOST_CHECK_EQUAL(outputBuf, "a\xef\xbf");
  result = transcoder (input, inputEnd, output, outputEnd);
  BOOST_CHECK_EQUAL(input - str, 2);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16to8Transcoder::trCharacterIncomplete);
  BOOST_CHECK_EQUAL(output - outputBuf, 4);
  *output = 0;
  BOOST_CHECK_EQUAL(outputBuf, "a\xef\xbf\xbd");
  result = transcoder (input, inputEnd, output, outputEnd);
  BOOST_CHECK_EQUAL(input, inputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16to8Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 5);
  *output = 0;
  BOOST_CHECK_EQUAL(outputBuf, "a\xef\xbf\xbd" "b");
  result = transcoder.Flush (output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16to8Transcoder::trSuccess);
  BOOST_CHECK_EQUAL(output - outputBuf, 5);
}

// Test output overflow in Flush()
BOOST_AUTO_TEST_CASE(FlushOutputOverflow)
{
  const s1::uc::Char16 str[] = { 'a', 0xd83d };
  s1::uc::UTF16to8Transcoder transcoder;
  const s1::uc::Char16* input = str;
  const s1::uc::Char16* inputEnd = str + sizeof(str)/sizeof(str[0]);
  char outputBuf[16];
  char* output = outputBuf;
  char* outputEnd = outputBuf + sizeof(outputBuf);
  s1::uc::UTF16to8Transcoder::TranscodeResult result = transcoder (input, inputEnd,
                                                                    output, outputEnd);
  BOOST_CHECK_EQUAL(input - str, 2);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16to8Transcoder::trInputUnderrun);
  BOOST_CHECK_EQUAL(output - outputBuf, 1);
  *output = 0;
  BOOST_CHECK_EQUAL(outputBuf, "a");
  result = transcoder.Flush (output, output);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16to8Transcoder::trOutputOverflow);
  result = transcoder.Flush (output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16to8Transcoder::trCharacterIncomplete);
  BOOST_CHECK_EQUAL(output - outputBuf, 4);
  *output = 0;
  BOOST_CHECK_EQUAL(outputBuf, "a\xef\xbf\xbd");
}

BOOST_AUTO_TEST_SUITE_END()