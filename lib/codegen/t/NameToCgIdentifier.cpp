#include <cxxtest/TestSuite.h>

#include "codegen/CgGenerator.h"
#include "../SequenceCodeGenerator.h"

using namespace s1::codegen;

class NameToCgIdentifierTestSuite : public CxxTest::TestSuite 
{
public:
  void testNameASCII (void)
  {
    std::string cgIdentifier = CgGenerator::NameToCgIdentifier (UnicodeString ("a"));
    for (std::string::iterator it = cgIdentifier.begin(); it != cgIdentifier.end(); ++it)
    {
      char ch = *it;
      TS_ASSERT((ch < 128) && (isalnum (ch) || (ch == '_')));
    }
  }
  
  void testNameUnicode (void)
  {
    static const UChar ustr[] = {0xe4, 0};
    std::string cgIdentifier = CgGenerator::NameToCgIdentifier (UnicodeString (ustr));
    for (std::string::iterator it = cgIdentifier.begin(); it != cgIdentifier.end(); ++it)
    {
      char ch = *it;
      TS_ASSERT((ch < 128) && (isalnum (ch) || (ch == '_')));
    }
  }
};
