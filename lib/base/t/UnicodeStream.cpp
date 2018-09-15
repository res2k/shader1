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

#include "base/uc/SimpleBufferStreamSource.h"
#include "base/uc/Stream.h"

#include "boost_check_equal_ch.h"

namespace s1
{
  namespace uc
  {
    static std::ostream& operator<< (std::ostream& stream, Stream::Error x)
    {
      stream << static_cast<unsigned int> (x);
      return stream;
    }
  } // namespace uc
} // namespace s1

BOOST_AUTO_TEST_SUITE(UnicodeStream)

BOOST_AUTO_TEST_CASE(EmptyStream)
{
  std::string empty;
  s1::uc::SimpleBufferStreamSource in (empty.data (), empty.size ());
  s1::uc::Stream ustream (in);

  // Empty stream should report "no chars"
  BOOST_CHECK_EQUAL ((bool)ustream, false);
  // Any attempt to access should throw an exception
  BOOST_CHECK_EQUAL ((*ustream).error(), s1::uc::Stream::Error::EndOfInput);
  // Trying to forward never throws
  BOOST_REQUIRE_NO_THROW (++ustream);
  // Still at end
  BOOST_CHECK_EQUAL ((*ustream).error(), s1::uc::Stream::Error::EndOfInput);
}

BOOST_AUTO_TEST_CASE(GetASCII)
{
  std::string str ("a");
  s1::uc::SimpleBufferStreamSource in (str.data (), str.size ());
  s1::uc::Stream ustream (in);
  s1::uc::Char32 ch = 0;
  
  BOOST_CHECK_EQUAL ((bool)ustream, true);
  // ASCII is UTF-8 is encoded as identity
  BOOST_REQUIRE_NO_THROW ((ch = (*ustream).value()));
  BOOST_CHECK_EQUAL_CH (ch, 'a');
  BOOST_REQUIRE_NO_THROW (++ustream);
  // Check end is end
  BOOST_CHECK_EQUAL ((bool)ustream, false);
  BOOST_CHECK_EQUAL ((*ustream).error(), s1::uc::Stream::Error::EndOfInput);
}

BOOST_AUTO_TEST_CASE(GetUTF)
{
  std::string str ("\xE2\x98\xBA");
  s1::uc::SimpleBufferStreamSource in (str.data (), str.size ());
  s1::uc::Stream ustream (in);
  s1::uc::Char32 ch = 0;
  
  BOOST_CHECK_EQUAL ((bool)ustream, true);
  // Test UTF-8 encoded Unicode character
  BOOST_REQUIRE_NO_THROW ((ch = (*ustream).value()));
  BOOST_CHECK_EQUAL_CH (ch, 0x263a);
  BOOST_REQUIRE_NO_THROW (++ustream);
  // Check end is end
  BOOST_CHECK_EQUAL ((bool)ustream, false);
  BOOST_CHECK_EQUAL ((*ustream).error(), s1::uc::Stream::Error::EndOfInput);

}

BOOST_AUTO_TEST_CASE(GetUTFNonBMP)
{
  std::string str ("\xF0\x9D\x94\xBD");
  s1::uc::SimpleBufferStreamSource in (str.data (), str.size ());
  s1::uc::Stream ustream (in);
  s1::uc::Char32 ch = 0;
  
  BOOST_CHECK_EQUAL ((bool)ustream, true);
  // Test UTF-8 encoded Unicode character from beyond the BMP (>= 0x10000)
  BOOST_REQUIRE_NO_THROW ((ch = (*ustream).value()));
  BOOST_CHECK_EQUAL_CH (ch, 0x1d53d);
  BOOST_REQUIRE_NO_THROW (++ustream);
  // Check end is end
  BOOST_CHECK_EQUAL ((bool)ustream, false);
  BOOST_CHECK_EQUAL ((*ustream).error(), s1::uc::Stream::Error::EndOfInput);

}

BOOST_AUTO_TEST_CASE(GetUTFIncomplete1)
{
  std::string str ("\xE2\x98");
  s1::uc::SimpleBufferStreamSource in (str.data (), str.size ());
  s1::uc::Stream ustream (in);
  
  BOOST_CHECK_EQUAL ((bool)ustream, true);
  // Incomplete UTF-8 encoded char should be an error
  BOOST_CHECK_EQUAL ((*ustream).error(), s1::uc::Stream::Error::utf8CharacterIncomplete);
  BOOST_REQUIRE_NO_THROW (++ustream);
  // Check end is end
  BOOST_CHECK_EQUAL ((bool)ustream, false);
  BOOST_CHECK_EQUAL ((*ustream).error(), s1::uc::Stream::Error::EndOfInput);

}

BOOST_AUTO_TEST_CASE(GetUTFIncomplete2)
{
  std::string str ("a\xE2\x98");
  s1::uc::SimpleBufferStreamSource in (str.data (), str.size ());
  s1::uc::Stream ustream (in);
  s1::uc::Char32 ch = 0;
  
  BOOST_CHECK_EQUAL ((bool)ustream, true);
  BOOST_REQUIRE_NO_THROW ((ch = (*ustream).value()));
  // First char should work w/o problems
  BOOST_CHECK_EQUAL_CH (ch, 'a');
  BOOST_REQUIRE_NO_THROW (++ustream);
  // Incomplete UTF-8 encoded char should be an error
  BOOST_CHECK_EQUAL ((*ustream).error(), s1::uc::Stream::Error::utf8CharacterIncomplete);
  BOOST_REQUIRE_NO_THROW (++ustream);
  // Check end is end
  BOOST_CHECK_EQUAL ((bool)ustream, false);
  BOOST_CHECK_EQUAL ((*ustream).error(), s1::uc::Stream::Error::EndOfInput);

}

BOOST_AUTO_TEST_CASE(GetUTFIncomplete3)
{
  std::string str ("\xE2\x98" "a");
  s1::uc::SimpleBufferStreamSource in (str.data (), str.size ());
  s1::uc::Stream ustream (in);
  s1::uc::Char32 ch = 0;
  
  BOOST_CHECK_EQUAL ((bool)ustream, true);
  // Incomplete UTF-8 encoded char should be an error
  BOOST_CHECK_EQUAL ((*ustream).error(), s1::uc::Stream::Error::utf8CharacterIncomplete);
  BOOST_REQUIRE_NO_THROW (++ustream);
  // After that, input should recover
  BOOST_REQUIRE_NO_THROW ((ch = (*ustream).value()));
  BOOST_CHECK_EQUAL_CH (ch, 'a');
  BOOST_REQUIRE_NO_THROW (++ustream);
  // Check end is end
  BOOST_CHECK_EQUAL ((bool)ustream, false);
  BOOST_CHECK_EQUAL ((*ustream).error(), s1::uc::Stream::Error::EndOfInput);

}

BOOST_AUTO_TEST_CASE(GetUTFInvalid)
{
  std::string str ("\xC0\x8a" "a");
  s1::uc::SimpleBufferStreamSource in (str.data (), str.size ());
  s1::uc::Stream ustream (in);
  s1::uc::Char32 ch = 0;
  
  BOOST_CHECK_EQUAL ((bool)ustream, true);
  // Overlong UTF-8 encoded char should be an error
  BOOST_CHECK_EQUAL ((*ustream).error(), s1::uc::Stream::Error::utf8EncodingInvalid);
  BOOST_REQUIRE_NO_THROW (++ustream);
  // After that, input should recover
  BOOST_REQUIRE_NO_THROW ((ch = (*ustream).value()));
  BOOST_CHECK_EQUAL_CH (ch, 'a');
  BOOST_REQUIRE_NO_THROW (++ustream);
  // Check end is end
  BOOST_CHECK_EQUAL ((bool)ustream, false);
  BOOST_CHECK_EQUAL ((*ustream).error(), s1::uc::Stream::Error::EndOfInput);
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

BOOST_AUTO_TEST_CASE(GetASCIILargeBuffer)
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
    BOOST_CHECK_EQUAL ((bool)ustream, true);
    
    s1::uc::Char32 ch = 0;
    BOOST_REQUIRE_NO_THROW ((ch = (*ustream).value()));
    BOOST_CHECK_EQUAL_CH (ch, 'a');
    BOOST_REQUIRE_NO_THROW (++ustream);
  }
  // Check end is end
  BOOST_CHECK_EQUAL ((bool)ustream, false);
  BOOST_CHECK_EQUAL ((*ustream).error(), s1::uc::Stream::Error::EndOfInput);
}

BOOST_AUTO_TEST_CASE(GetUTFStraddleLargeBuffer)
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
    BOOST_CHECK_EQUAL ((bool)ustream, true);
    
    BOOST_REQUIRE_NO_THROW ((ch = (*ustream).value()));
    BOOST_CHECK_EQUAL_CH (ch, 'a');
    BOOST_REQUIRE_NO_THROW (++ustream);
  }
  BOOST_CHECK_EQUAL ((bool)ustream, true);
  // Test UTF-8 encoded Unicode character
  BOOST_REQUIRE_NO_THROW ((ch = (*ustream).value()));
  BOOST_CHECK_EQUAL_CH (ch, 0x263a);
  BOOST_REQUIRE_NO_THROW (++ustream);
  // Check end is end
  BOOST_CHECK_EQUAL ((bool)ustream, false);
  BOOST_CHECK_EQUAL ((*ustream).error(), s1::uc::Stream::Error::EndOfInput);
}

BOOST_AUTO_TEST_CASE(GetUTFSurrogate)
{
  std::string str ("\xED\xA0\xB5\xED\xB4\xBD");
  s1::uc::SimpleBufferStreamSource in (str.data (), str.size ());
  s1::uc::Stream ustream (in);
  
  BOOST_CHECK_EQUAL ((bool)ustream, true);
  // Test that a surrogate, encoded separately in UTF-8, are invalid
  BOOST_CHECK_EQUAL ((*ustream).error(), s1::uc::Stream::Error::utf8CharacterInvalid);
  BOOST_REQUIRE_NO_THROW (++ustream);
}

BOOST_AUTO_TEST_SUITE_END()
