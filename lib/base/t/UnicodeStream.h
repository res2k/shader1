/*
    Shader1
    Copyright (c) 2010-2014 Frank Richter


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

#include "base/uc/SimpleBufferStreamSource.h"
#include "base/uc/Stream.h"

template<typename T>
static inline typename std::make_unsigned<T>::type as_unsigned (T x)
{
  return static_cast<typename std::make_unsigned<T>::type> (x);
}

#define TS_ASSERT_EQUALS_CH(A, B)                                     \
  TS_ASSERT_EQUALS(static_cast<uint_least32_t> (as_unsigned (A)),     \
                   static_cast<uint_least32_t> (B))

class UnicodeStreamTestSuite : public CxxTest::TestSuite 
{
public:
  void testEmptyStream (void)
  {
    std::string empty;
    s1::uc::SimpleBufferStreamSource in (empty.data (), empty.size ());
    s1::uc::Stream ustream (in);

    // Empty stream should report "no chars"
    TS_ASSERT_EQUALS ((bool)ustream, false);
    // Any attempt to access should throw an exception
    TS_ASSERT_EQUALS ((*ustream).error(), s1::uc::Stream::Error::EndOfInput);
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++ustream);
    // Still at end
    TS_ASSERT_EQUALS ((*ustream).error(), s1::uc::Stream::Error::EndOfInput);
  }
  
  void testGetASCII (void)
  {
    std::string str ("a");
    s1::uc::SimpleBufferStreamSource in (str.data (), str.size ());
    s1::uc::Stream ustream (in);
    s1::uc::Char32 ch = 0;
    
    TS_ASSERT_EQUALS ((bool)ustream, true);
    // ASCII is UTF-8 is encoded as identity
    TS_ASSERT_THROWS_NOTHING ((ch = (*ustream).value()));
    TS_ASSERT_EQUALS_CH (ch, 'a');
    TS_ASSERT_THROWS_NOTHING (++ustream);
    // Check end is end
    TS_ASSERT_EQUALS ((bool)ustream, false);
    TS_ASSERT_EQUALS ((*ustream).error(), s1::uc::Stream::Error::EndOfInput);
  }
  
  void testGetUTF (void)
  {
    std::string str ("\xE2\x98\xBA");
    s1::uc::SimpleBufferStreamSource in (str.data (), str.size ());
    s1::uc::Stream ustream (in);
    s1::uc::Char32 ch = 0;
    
    TS_ASSERT_EQUALS ((bool)ustream, true);
    // Test UTF-8 encoded Unicode character
    TS_ASSERT_THROWS_NOTHING ((ch = (*ustream).value()));
    TS_ASSERT_EQUALS_CH (ch, 0x263a);
    TS_ASSERT_THROWS_NOTHING (++ustream);
    // Check end is end
    TS_ASSERT_EQUALS ((bool)ustream, false);
    TS_ASSERT_EQUALS ((*ustream).error(), s1::uc::Stream::Error::EndOfInput);

  }
  
  void testGetUTFNonBMP (void)
  {
    std::string str ("\xF0\x9D\x94\xBD");
    s1::uc::SimpleBufferStreamSource in (str.data (), str.size ());
    s1::uc::Stream ustream (in);
    s1::uc::Char32 ch = 0;
    
    TS_ASSERT_EQUALS ((bool)ustream, true);
    // Test UTF-8 encoded Unicode character from beyond the BMP (>= 0x10000)
    TS_ASSERT_THROWS_NOTHING ((ch = (*ustream).value()));
    TS_ASSERT_EQUALS_CH (ch, 0x1d53d);
    TS_ASSERT_THROWS_NOTHING (++ustream);
    // Check end is end
    TS_ASSERT_EQUALS ((bool)ustream, false);
    TS_ASSERT_EQUALS ((*ustream).error(), s1::uc::Stream::Error::EndOfInput);

  }
  
  void testGetUTFIncomplete1 (void)
  {
    std::string str ("\xE2\x98");
    s1::uc::SimpleBufferStreamSource in (str.data (), str.size ());
    s1::uc::Stream ustream (in);
    
    TS_ASSERT_EQUALS ((bool)ustream, true);
    // Incomplete UTF-8 encoded char should be an error
    TS_ASSERT_EQUALS ((*ustream).error(), s1::uc::Stream::Error::utf8CharacterIncomplete);
    TS_ASSERT_THROWS_NOTHING (++ustream);
    // Check end is end
    TS_ASSERT_EQUALS ((bool)ustream, false);
    TS_ASSERT_EQUALS ((*ustream).error(), s1::uc::Stream::Error::EndOfInput);

  }
  
  void testGetUTFIncomplete2 (void)
  {
    std::string str ("a\xE2\x98");
    s1::uc::SimpleBufferStreamSource in (str.data (), str.size ());
    s1::uc::Stream ustream (in);
    s1::uc::Char32 ch = 0;
    
    TS_ASSERT_EQUALS ((bool)ustream, true);
    TS_ASSERT_THROWS_NOTHING ((ch = (*ustream).value()));
    // First char should work w/o problems
    TS_ASSERT_EQUALS_CH (ch, 'a');
    TS_ASSERT_THROWS_NOTHING (++ustream);
    // Incomplete UTF-8 encoded char should be an error
    TS_ASSERT_EQUALS ((*ustream).error(), s1::uc::Stream::Error::utf8CharacterIncomplete);
    TS_ASSERT_THROWS_NOTHING (++ustream);
    // Check end is end
    TS_ASSERT_EQUALS ((bool)ustream, false);
    TS_ASSERT_EQUALS ((*ustream).error(), s1::uc::Stream::Error::EndOfInput);

  }
  
  void testGetUTFIncomplete3 (void)
  {
    std::string str ("\xE2\x98" "a");
    s1::uc::SimpleBufferStreamSource in (str.data (), str.size ());
    s1::uc::Stream ustream (in);
    s1::uc::Char32 ch = 0;
    
    TS_ASSERT_EQUALS ((bool)ustream, true);
    // Incomplete UTF-8 encoded char should be an error
    TS_ASSERT_EQUALS ((*ustream).error(), s1::uc::Stream::Error::utf8CharacterIncomplete);
    TS_ASSERT_THROWS_NOTHING (++ustream);
    // After that, input should recover
    TS_ASSERT_THROWS_NOTHING ((ch = (*ustream).value()));
    TS_ASSERT_EQUALS_CH (ch, 'a');
    TS_ASSERT_THROWS_NOTHING (++ustream);
    // Check end is end
    TS_ASSERT_EQUALS ((bool)ustream, false);
    TS_ASSERT_EQUALS ((*ustream).error(), s1::uc::Stream::Error::EndOfInput);

  }
  
  void testGetUTFInvalid (void)
  {
    std::string str ("\xC0\x8a" "a");
    s1::uc::SimpleBufferStreamSource in (str.data (), str.size ());
    s1::uc::Stream ustream (in);
    s1::uc::Char32 ch = 0;
    
    TS_ASSERT_EQUALS ((bool)ustream, true);
    // Overlong UTF-8 encoded char should be an error
    TS_ASSERT_EQUALS ((*ustream).error(), s1::uc::Stream::Error::utf8EncodingInvalid);
    TS_ASSERT_THROWS_NOTHING (++ustream);
    // After that, input should recover
    TS_ASSERT_THROWS_NOTHING ((ch = (*ustream).value()));
    TS_ASSERT_EQUALS_CH (ch, 'a');
    TS_ASSERT_THROWS_NOTHING (++ustream);
    // Check end is end
    TS_ASSERT_EQUALS ((bool)ustream, false);
    TS_ASSERT_EQUALS ((*ustream).error(), s1::uc::Stream::Error::EndOfInput);
  }
  
  // Hack to make UnicodeStream internal buffer size available
  class MyUnicodeStream : public s1::uc::Stream::Source
  {
    const char* inputData;
    size_t inputRemaining;
  public:
    enum  { UCBufferSize = 1024 };

    MyUnicodeStream (const char* inputData, size_t inputSize)
      : inputData (inputData), inputRemaining (inputSize) {}

    size_t NextData (const char*& data) override
    {
      if (inputRemaining == 0)
      {
        data = nullptr;
        return 0;
      }
      size_t serveAmount = std::min<size_t> (inputRemaining, UCBufferSize);
      data = inputData;
      inputData += serveAmount;
      inputRemaining -= serveAmount;
      return serveAmount;
    }
  };
  
  void testGetASCIILargeBuffer (void)
  {
    /* Check that input exceeding UnicodeStream's internal buffer size
       is handled correctly */
    size_t testSize = MyUnicodeStream::UCBufferSize + 1;
    std::string str;
    for (size_t i = 0; i < testSize; i++) str.append ("a");
    MyUnicodeStream in (str.data(), str.size());
    s1::uc::Stream ustream (in);
    
    for (size_t i = 0; i < testSize; i++)
    {
      TS_ASSERT_EQUALS ((bool)ustream, true);
      
      s1::uc::Char32 ch = 0;
      TS_ASSERT_THROWS_NOTHING ((ch = (*ustream).value()));
      TS_ASSERT_EQUALS_CH (ch, 'a');
      TS_ASSERT_THROWS_NOTHING (++ustream);
    }
    // Check end is end
    TS_ASSERT_EQUALS ((bool)ustream, false);
    TS_ASSERT_EQUALS ((*ustream).error(), s1::uc::Stream::Error::EndOfInput);
  }
  
  void testGetUTFStraddleLargeBuffer (void)
  {
    /* Check that an UTF-8 encoded character straddling the boundary of
       UnicodeStream's internal buffer size is handled correctly */
    size_t testSize = MyUnicodeStream::UCBufferSize - 2;
    std::string str;
    for (size_t i = 0; i < testSize; i++) str.append ("a");
    str.append ("\xE2\x98\xBA");
    MyUnicodeStream in (str.data(), str.size());
    s1::uc::Stream ustream (in);
    
    s1::uc::Char32 ch = 0;
    for (size_t i = 0; i < testSize; i++)
    {
      TS_ASSERT_EQUALS ((bool)ustream, true);
      
      TS_ASSERT_THROWS_NOTHING ((ch = (*ustream).value()));
      TS_ASSERT_EQUALS_CH (ch, 'a');
      TS_ASSERT_THROWS_NOTHING (++ustream);
    }
    TS_ASSERT_EQUALS ((bool)ustream, true);
    // Test UTF-8 encoded Unicode character
    TS_ASSERT_THROWS_NOTHING ((ch = (*ustream).value()));
    TS_ASSERT_EQUALS_CH (ch, 0x263a);
    TS_ASSERT_THROWS_NOTHING (++ustream);
    // Check end is end
    TS_ASSERT_EQUALS ((bool)ustream, false);
    TS_ASSERT_EQUALS ((*ustream).error(), s1::uc::Stream::Error::EndOfInput);
  }
  
  void testGetUTFSurrogate (void)
  {
    std::string str ("\xED\xA0\xB5\xED\xB4\xBD");
    s1::uc::SimpleBufferStreamSource in (str.data (), str.size ());
    s1::uc::Stream ustream (in);
    
    TS_ASSERT_EQUALS ((bool)ustream, true);
    // Test that a surrogate, encoded separately in UTF-8, are invalid
    TS_ASSERT_EQUALS ((*ustream).error(), s1::uc::Stream::Error::utf8CharacterInvalid);
    TS_ASSERT_THROWS_NOTHING (++ustream);
  }
};
