#include <cxxtest/TestSuite.h>

#include "codegen/CgGenerator.h"
#include "../SequenceCodeGenerator.h"

using namespace s1::codegen;

class NameToCgIdentifierTestSuite : public CxxTest::TestSuite 
{
  class TestImportedNameResolver : public ImportedNameResolver
  {
  public:
    std::string GetImportedNameIdentifier (const UnicodeString& name)
    { return std::string(); }	
  };
  
  class TestCodeGenerator : public CgGenerator
  {
  public:
    typedef CgGenerator Superclass;
    
    class TestSequenceCodeGenerator : public SequenceCodeGenerator
    {
    public:
      TestSequenceCodeGenerator (const Sequence& seq, ImportedNameResolver* nameRes)
       : SequenceCodeGenerator (seq, nameRes) {}
       
       using SequenceCodeGenerator::RegisterNameToCgIdentifier;
    };
  };  
public:
  void testNameASCII (void)
  {
    Sequence seq;
    TestImportedNameResolver nameRes;
    TestCodeGenerator::TestSequenceCodeGenerator scg (seq, &nameRes);
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
    TestImportedNameResolver nameRes;
    TestCodeGenerator::TestSequenceCodeGenerator scg (seq, &nameRes);
    static const UChar ustr[] = {0xe4, 0};
    std::string cgIdentifier = scg.RegisterNameToCgIdentifier (UnicodeString (ustr));
    for (std::string::iterator it = cgIdentifier.begin(); it != cgIdentifier.end(); ++it)
    {
      char ch = *it;
      TS_ASSERT((ch < 128) && (isalnum (ch) || (ch == '_')));
    }
  }
};
