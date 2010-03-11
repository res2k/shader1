#include <cxxtest/TestSuite.h>

#include "base/UnicodeStream.h"
#include "base/UnicodeStreamEndOfInputException.h"
#include "base/UnicodeStreamInvalidCharacterException.h"

class UnicodeStreamTestSuite : public CxxTest::TestSuite 
{
public:
  void testBogusEncoding (void)
  {
    std::string empty;
    std::istringstream in (empty);
    // Specifying a bogus encoding name should throw an exception
    TS_ASSERT_THROWS (s1::UnicodeStream ustream (in, "qwertz"),
		      s1::UnicodeStreamException);
  }
  
  void testEmptyStream (void)
  {
    std::string empty;
    std::istringstream in (empty);
    s1::UnicodeStream ustream (in, "utf-8");

    // Empty stream should report "no chars"
    TS_ASSERT_EQUALS ((bool)ustream, false);
    // Any attempt to access should throw an exception
    TS_ASSERT_THROWS (*ustream, s1::UnicodeStreamEndOfInputException);
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++ustream);
    // Still at end
    TS_ASSERT_THROWS (*ustream, s1::UnicodeStreamEndOfInputException);
  }
  
  void testGetASCII (void)
  {
    std::string str ("a");
    std::istringstream in (str);
    s1::UnicodeStream ustream (in, "utf-8");
    UChar32 ch;
    
    TS_ASSERT_EQUALS ((bool)ustream, true);
    // ASCII is UTF-8 is encoded as identity
    TS_ASSERT_THROWS_NOTHING ((ch = *ustream));
    TS_ASSERT_EQUALS (ch, 'a');
    TS_ASSERT_THROWS_NOTHING (++ustream);
    // Check end is end
    TS_ASSERT_EQUALS ((bool)ustream, false);
    TS_ASSERT_THROWS (*ustream, s1::UnicodeStreamEndOfInputException);
  }
  
  void testGetUTF (void)
  {
    std::string str ("\xE2\x98\xBA");
    std::istringstream in (str);
    s1::UnicodeStream ustream (in, "utf-8");
    UChar32 ch;
    
    TS_ASSERT_EQUALS ((bool)ustream, true);
    // Test UTF-8 encoded Unicode character
    TS_ASSERT_THROWS_NOTHING ((ch = *ustream));
    TS_ASSERT_EQUALS (ch, 0x263a);
    TS_ASSERT_THROWS_NOTHING (++ustream);
    // Check end is end
    TS_ASSERT_EQUALS ((bool)ustream, false);
    TS_ASSERT_THROWS (*ustream, s1::UnicodeStreamEndOfInputException);

  }
  
  void testGetUTFNonBMP (void)
  {
    std::string str ("\xF0\x9D\x94\xBD");
    std::istringstream in (str);
    s1::UnicodeStream ustream (in, "utf-8");
    UChar32 ch;
    
    TS_ASSERT_EQUALS ((bool)ustream, true);
    // Test UTF-8 encoded Unicode character from beyond the BMP (>= 0x10000)
    TS_ASSERT_THROWS_NOTHING ((ch = *ustream));
    TS_ASSERT_EQUALS (ch, 0x1d53d);
    TS_ASSERT_THROWS_NOTHING (++ustream);
    // Check end is end
    TS_ASSERT_EQUALS ((bool)ustream, false);
    TS_ASSERT_THROWS (*ustream, s1::UnicodeStreamEndOfInputException);

  }
  
  void testGetUTFIncomplete1 (void)
  {
    std::string str ("\xE2\x98");
    std::istringstream in (str);
    s1::UnicodeStream ustream (in, "utf-8");
    
    TS_ASSERT_EQUALS ((bool)ustream, true);
    // Incomplete UTF-8 encoded char should be an error
    TS_ASSERT_THROWS (*ustream, s1::UnicodeStreamInvalidCharacterException);
    TS_ASSERT_THROWS_NOTHING (++ustream);
    // Check end is end
    TS_ASSERT_EQUALS ((bool)ustream, false);
    TS_ASSERT_THROWS (*ustream, s1::UnicodeStreamEndOfInputException);

  }
  
  void testGetUTFIncomplete2 (void)
  {
    std::string str ("a\xE2\x98");
    std::istringstream in (str);
    s1::UnicodeStream ustream (in, "utf-8");
    UChar32 ch;
    
    TS_ASSERT_EQUALS ((bool)ustream, true);
    TS_ASSERT_THROWS_NOTHING ((ch = *ustream));
    // First char should work w/o problems
    TS_ASSERT_EQUALS (ch, 'a');
    TS_ASSERT_THROWS_NOTHING (++ustream);
    // Incomplete UTF-8 encoded char should be an error
    TS_ASSERT_THROWS (*ustream, s1::UnicodeStreamInvalidCharacterException);
    TS_ASSERT_THROWS_NOTHING (++ustream);
    // Check end is end
    TS_ASSERT_EQUALS ((bool)ustream, false);
    TS_ASSERT_THROWS (*ustream, s1::UnicodeStreamEndOfInputException);

  }
  
  void testGetUTFIncomplete3 (void)
  {
    std::string str ("\xE2\x98" "a");
    std::istringstream in (str);
    s1::UnicodeStream ustream (in, "utf-8");
    UChar32 ch;
    
    TS_ASSERT_EQUALS ((bool)ustream, true);
    // Incomplete UTF-8 encoded char should be an error
    TS_ASSERT_THROWS (*ustream, s1::UnicodeStreamInvalidCharacterException);
    TS_ASSERT_THROWS_NOTHING (++ustream);
    // After that, input should recover
    TS_ASSERT_THROWS_NOTHING ((ch = *ustream));
    TS_ASSERT_EQUALS (ch, 'a');
    TS_ASSERT_THROWS_NOTHING (++ustream);
    // Check end is end
    TS_ASSERT_EQUALS ((bool)ustream, false);
    TS_ASSERT_THROWS (*ustream, s1::UnicodeStreamEndOfInputException);

  }
  
  void testGetUTFInvalid (void)
  {
    std::string str ("\xC0" "a");
    std::istringstream in (str);
    s1::UnicodeStream ustream (in, "utf-8");
    
    TS_ASSERT_EQUALS ((bool)ustream, true);
    // Invalid UTF-8 encoded char should be an error
    TS_ASSERT_THROWS (*ustream, s1::UnicodeStreamInvalidCharacterException);
  }
  
  // Hack to make UnicodeStream internal buffer size available
  struct MyUnicodeStream : public s1::UnicodeStream
  {
    using s1::UnicodeStream::UCBufferSize;
  };
  
  void testGetASCIILargeBuffer (void)
  {
    /* Check that input exceeding UnicodeStream's internal buffer size
       is handled correctly */
    size_t testSize = MyUnicodeStream::UCBufferSize + 1;
    std::string str;
    for (size_t i = 0; i < testSize; i++) str.append ("a");
    std::istringstream in (str);
    s1::UnicodeStream ustream (in, "utf-8");
    
    for (size_t i = 0; i < testSize; i++)
    {
      TS_ASSERT_EQUALS ((bool)ustream, true);
      
      UChar32 ch;
      TS_ASSERT_THROWS_NOTHING ((ch = *ustream));
      TS_ASSERT_EQUALS (ch, 'a');
      TS_ASSERT_THROWS_NOTHING (++ustream);
    }
    // Check end is end
    TS_ASSERT_EQUALS ((bool)ustream, false);
    TS_ASSERT_THROWS (*ustream, s1::UnicodeStreamEndOfInputException);
  }
  
  void testGetUTFStraddleLargeBuffer (void)
  {
    /* Check that an UTF-8 encoded character straddling the boundary of
       UnicodeStream's internal buffer size is handled correctly */
    size_t testSize = MyUnicodeStream::UCBufferSize - 2;
    std::string str;
    for (size_t i = 0; i < testSize; i++) str.append ("a");
    str.append ("\xE2\x98\xBA");
    std::istringstream in (str);
    s1::UnicodeStream ustream (in, "utf-8");
    
    UChar32 ch;
    for (size_t i = 0; i < testSize; i++)
    {
      TS_ASSERT_EQUALS ((bool)ustream, true);
      
      TS_ASSERT_THROWS_NOTHING ((ch = *ustream));
      TS_ASSERT_EQUALS (ch, 'a');
      TS_ASSERT_THROWS_NOTHING (++ustream);
    }
    TS_ASSERT_EQUALS ((bool)ustream, true);
    // Test UTF-8 encoded Unicode character
    TS_ASSERT_THROWS_NOTHING ((ch = *ustream));
    TS_ASSERT_EQUALS (ch, 0x263a);
    TS_ASSERT_THROWS_NOTHING (++ustream);
    // Check end is end
    TS_ASSERT_EQUALS ((bool)ustream, false);
    TS_ASSERT_THROWS (*ustream, s1::UnicodeStreamEndOfInputException);
  }
};
