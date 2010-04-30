#include <cxxtest/TestSuite.h>

#include "codegen/CgGenerator.h"
#include "../SequenceCodeGenerator.h"

using namespace s1::codegen;

class NameToCgIdentifierTestSuite : public CxxTest::TestSuite 
{
  class TestCodeGenerator : public CgGenerator
  {
  public:
    typedef CgGenerator Superclass;
    
    class TestSequenceCodeGenerator : public SequenceCodeGenerator
    {
    public:
      TestSequenceCodeGenerator (const Sequence& seq)
       : SequenceCodeGenerator (seq) {}
       
       using SequenceCodeGenerator::RegisterNameToCgIdentifier;
    };
  };  
public:
  void testNameASCII (void)
  {
    Sequence seq;
    TestCodeGenerator::TestSequenceCodeGenerator scg (seq);
    std::string cgIdentifier = scg.RegisterNameToCgIdentifier (UnicodeString ("a"));
    for (std::string::iterator it = cgIdentifier.begin(); it != cgIdentifier.end(); ++it)
    {
      char ch = *it;
      TS_ASSERT((ch < 128) && (isalnum (ch) || (ch == '_')));
    }
  }
  
  void testNameUnicode (void)
  {
    Sequence seq;
    TestCodeGenerator::TestSequenceCodeGenerator scg (seq);
    static const UChar ustr[] = {0xe4, 0};
    std::string cgIdentifier = scg.RegisterNameToCgIdentifier (UnicodeString (ustr));
    for (std::string::iterator it = cgIdentifier.begin(); it != cgIdentifier.end(); ++it)
    {
      char ch = *it;
      TS_ASSERT((ch < 128) && (isalnum (ch) || (ch == '_')));
    }
  }
};
