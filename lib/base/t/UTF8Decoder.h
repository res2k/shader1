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

#include <cxxtest/TestSuite.h>

#include "base/uc/UTF8Decoder.h"

class UTF8DecoderSuite : public CxxTest::TestSuite 
{
public:
  void testEmpty (void)
  {
    const char nullStr[] = "";
    s1::uc::UTF8Decoder decoder;
    const char* input = nullStr;
    const char* inputEnd = nullStr + sizeof(nullStr) - 1;
    s1::uc::Char32 ch (0x12345678);
    s1::uc::UTF8Decoder::DecodeResult result = decoder (input, inputEnd, ch);
    TS_ASSERT_EQUALS(input, inputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8Decoder::drInputUnderrun);
    TS_ASSERT_EQUALS(ch, 0x12345678);
  }

  // Test UTF8Decoder with ASCII input
  void testUTF8Simple ()
  {
    const char chars_utf8[] = "X";
    s1::uc::UTF8Decoder decoder;
    const char* input = chars_utf8;
    const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
    s1::uc::Char32 ch (0);
    s1::uc::UTF8Decoder::DecodeResult result = decoder (input, inputEnd, ch);
    TS_ASSERT_EQUALS(input, inputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8Decoder::drSuccess);
    TS_ASSERT_EQUALS(ch, 'X');
  }

  // Test UTF8Decoder with a character encoded in 2 bytes
  void testUTF8Encoded2 ()
  {
    const char chars_utf8[] = "\xc3\xab";
    s1::uc::UTF8Decoder decoder;
    const char* input = chars_utf8;
    const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
    s1::uc::Char32 ch (0);
    s1::uc::UTF8Decoder::DecodeResult result = decoder (input, inputEnd, ch);
    TS_ASSERT_EQUALS(input, inputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8Decoder::drSuccess);
    TS_ASSERT_EQUALS(ch, 0xeb);
  }

  // Test UTF8Decoder with characters encoded in 3 bytes
  void testUTF8Encoded3 ()
  {
    const char chars_utf8[] = "\xe1\xb8\xb7";
    s1::uc::UTF8Decoder decoder;
    const char* input = chars_utf8;
    const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
    s1::uc::Char32 ch (0);
    s1::uc::UTF8Decoder::DecodeResult result = decoder (input, inputEnd, ch);
    TS_ASSERT_EQUALS(input, inputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8Decoder::drSuccess);
    TS_ASSERT_EQUALS(ch, 0x1e37);
  }

  // Test UTF8Decoder with characters encoded in 4 bytes
  void testUTF8Encoded4 ()
  {
    const char chars_utf8[] = "\xf0\x9f\x98\x80";
    s1::uc::UTF8Decoder decoder;
    const char* input = chars_utf8;
    const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
    s1::uc::Char32 ch (0);
    s1::uc::UTF8Decoder::DecodeResult result = decoder (input, inputEnd, ch);
    TS_ASSERT_EQUALS(input, inputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8Decoder::drSuccess);
    TS_ASSERT_EQUALS(ch, 0x1f600);
  }

  // Test UTF8Decoder with malformed input (overlong encoding)
  void testUTF8Malformed1 ()
  {
    const char chars_utf8[] = "\xc0\x8a";
    s1::uc::UTF8Decoder decoder;
    const char* input = chars_utf8;
    const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
    s1::uc::Char32 ch (0);
    s1::uc::UTF8Decoder::DecodeResult result = decoder (input, inputEnd, ch);
    TS_ASSERT_EQUALS(input, inputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8Decoder::drEncodingInvalid);
    TS_ASSERT_EQUALS(ch, 0xfffd);
  }

  // Test UTF8Decoder with malformed input (broken encoding)
  void testUTF8Malformed2 ()
  {
    const char chars_utf8[] = "\xc3";
    s1::uc::UTF8Decoder decoder;
    const char* input = chars_utf8;
    const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
    s1::uc::Char32 ch (0x12345678);
    s1::uc::UTF8Decoder::DecodeResult result = decoder (input, inputEnd, ch);
    TS_ASSERT_EQUALS(input, inputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8Decoder::drInputUnderrun);
    TS_ASSERT_EQUALS(ch, 0x12345678);
  }

  // Test UTF8Decoder with malformed input (broken encoding)
  void testUTF8Malformed2b ()
  {
    const char chars_utf8[] = "\xab";
    s1::uc::UTF8Decoder decoder;
    const char* input = chars_utf8;
    const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
    s1::uc::Char32 ch (0);
    s1::uc::UTF8Decoder::DecodeResult result = decoder (input, inputEnd, ch);
    TS_ASSERT_EQUALS(input, inputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8Decoder::drCharacterIncomplete);
    TS_ASSERT_EQUALS(ch, 0xfffd);
  }

  // Test UTF8Decoder with malformed input (encoded surrogate half)
  void testUTF8Malformed3 ()
  {
    const char chars_utf8[] = "\xed\xa0\x80";
    s1::uc::UTF8Decoder decoder;
    const char* input = chars_utf8;
    const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
    s1::uc::Char32 ch (0);
    s1::uc::UTF8Decoder::DecodeResult result = decoder (input, inputEnd, ch);
    TS_ASSERT_EQUALS(input, inputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8Decoder::drCharacterInvalid);
    TS_ASSERT_EQUALS(ch, 0xfffd);
  }

  // Test UTF8Decoder with malformed input (encoded surrogate half)
  void testUTF8Malformed4 ()
  {
    const char chars_utf8[] = "\xed\xb0\x80";
    s1::uc::UTF8Decoder decoder;
    const char* input = chars_utf8;
    const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
    s1::uc::Char32 ch (0);
    s1::uc::UTF8Decoder::DecodeResult result = decoder (input, inputEnd, ch);
    TS_ASSERT_EQUALS(input, inputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8Decoder::drCharacterInvalid);
    TS_ASSERT_EQUALS(ch, 0xfffd);
  }

  // Test UTF8Decoder with malformed input (overlong encoding) and continuation afterwards
  void testUTF8Malformed1cont ()
  {
    const char chars_utf8[] = "\xc0\x8a*";
    s1::uc::UTF8Decoder decoder;
    const char* input = chars_utf8;
    const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
    s1::uc::Char32 ch (0);
    s1::uc::UTF8Decoder::DecodeResult result = decoder (input, inputEnd, ch);
    TS_ASSERT_EQUALS(input, inputEnd - 1);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8Decoder::drEncodingInvalid);
    TS_ASSERT_EQUALS(ch, 0xfffd);
    result = decoder (input, inputEnd, ch);
    TS_ASSERT_EQUALS(input, inputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8Decoder::drSuccess);
    TS_ASSERT_EQUALS(ch, '*');
  }

  // Test UTF8Decoder with malformed input (broken encoding) and continuation afterwards
  void testUTF8Malformed2cont ()
  {
    const char chars_utf8[] = "\xc3*";
    s1::uc::UTF8Decoder decoder;
    const char* input = chars_utf8;
    const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
    s1::uc::Char32 ch (0);
    s1::uc::UTF8Decoder::DecodeResult result = decoder (input, inputEnd, ch);
    TS_ASSERT_EQUALS(input, inputEnd - 1);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8Decoder::drCharacterIncomplete);
    TS_ASSERT_EQUALS(ch, 0xfffd);
    result = decoder (input, inputEnd, ch);
    TS_ASSERT_EQUALS(input, inputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8Decoder::drSuccess);
    TS_ASSERT_EQUALS(ch, '*');
  }

  // Test UTF8Decoder with malformed input (broken encoding) and continuation afterwards
  void testUTF8Malformed2bcont ()
  {
    const char chars_utf8[] = "\xab*";
    s1::uc::UTF8Decoder decoder;
    const char* input = chars_utf8;
    const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
    s1::uc::Char32 ch (0);
    s1::uc::UTF8Decoder::DecodeResult result = decoder (input, inputEnd, ch);
    TS_ASSERT_EQUALS(input, inputEnd - 1);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8Decoder::drCharacterIncomplete);
    TS_ASSERT_EQUALS(ch, 0xfffd);
    result = decoder (input, inputEnd, ch);
    TS_ASSERT_EQUALS(input, inputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8Decoder::drSuccess);
    TS_ASSERT_EQUALS(ch, '*');
  }

  // Test UTF8Decoder with malformed input (broken encoding) and continuation afterwards
  void testUTF8Malformed2ccont ()
  {
    const char chars_utf8[] = "\xe1\xb8\xc3\xab";
    s1::uc::UTF8Decoder decoder;
    const char* input = chars_utf8;
    const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
    s1::uc::Char32 ch (0);
    s1::uc::UTF8Decoder::DecodeResult result = decoder (input, inputEnd, ch);
    TS_ASSERT_EQUALS(input, inputEnd - 2);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8Decoder::drCharacterIncomplete);
    TS_ASSERT_EQUALS(ch, 0xfffd);
    result = decoder (input, inputEnd, ch);
    TS_ASSERT_EQUALS(input, inputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8Decoder::drSuccess);
    TS_ASSERT_EQUALS(ch, 0xeb);
  }

  // Test UTF8Decoder with malformed input (broken encoding) and continuation afterwards
  void testUTF8Malformed2dcont ()
  {
    const char chars_utf8[] = "\xab\xe1\xb8\xb7";
    s1::uc::UTF8Decoder decoder;
    const char* input = chars_utf8;
    const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
    s1::uc::Char32 ch (0);
    s1::uc::UTF8Decoder::DecodeResult result = decoder (input, inputEnd, ch);
    TS_ASSERT_EQUALS(input, inputEnd - 3);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8Decoder::drCharacterIncomplete);
    TS_ASSERT_EQUALS(ch, 0xfffd);
    result = decoder (input, inputEnd, ch);
    TS_ASSERT_EQUALS(input, inputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8Decoder::drSuccess);
    TS_ASSERT_EQUALS(ch, 0x1e37);
  }

  // Test UTF8Decoder with malformed input (encoded surrogate half) and continuation afterwards
  void testUTF8Malformed3cont ()
  {
    const char chars_utf8[] = "\xed\xa0\x80*";
    s1::uc::UTF8Decoder decoder;
    const char* input = chars_utf8;
    const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
    s1::uc::Char32 ch (0);
    s1::uc::UTF8Decoder::DecodeResult result = decoder (input, inputEnd, ch);
    TS_ASSERT_EQUALS(input, inputEnd - 1);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8Decoder::drCharacterInvalid);
    TS_ASSERT_EQUALS(ch, 0xfffd);
    result = decoder (input, inputEnd, ch);
    TS_ASSERT_EQUALS(input, inputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8Decoder::drSuccess);
    TS_ASSERT_EQUALS(ch, '*');
  }

  // Test UTF8Decoder with malformed input (encoded surrogate half) and continuation afterwards
  void testUTF8Malformed4cont ()
  {
    const char chars_utf8[] = "\xed\xb0\x80*";
    s1::uc::UTF8Decoder decoder;
    const char* input = chars_utf8;
    const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
    s1::uc::Char32 ch (0);
    s1::uc::UTF8Decoder::DecodeResult result = decoder (input, inputEnd, ch);
    TS_ASSERT_EQUALS(input, inputEnd - 1);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8Decoder::drCharacterInvalid);
    TS_ASSERT_EQUALS(ch, 0xfffd);
    result = decoder (input, inputEnd, ch);
    TS_ASSERT_EQUALS(input, inputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8Decoder::drSuccess);
    TS_ASSERT_EQUALS(ch, '*');
  }
};
