#include <cxxtest/TestSuite.h>

#include "s1/Library.h"

class ApiLibraryTestSuite : public CxxTest::TestSuite 
{
public:
  void testCreateLibraryC (void)
  {
    // Check object creation
    s1_Library* lib;
    s1_ResultCode err = s1_create_library (&lib);
    TS_ASSERT(S1_SUCCESSFUL(err));
    TS_ASSERT_DIFFERS(lib, (s1_Library*)0);
    int rc;
    rc = s1_get_ref_count(lib);
    TS_ASSERT_EQUALS(rc, 1);
    rc = s1_release (lib);
    TS_ASSERT_EQUALS(rc, 0);
  }

  void testWrongCreateLibraryC (void)
  {
    s1_ResultCode err = s1_create_library (0);
    TS_ASSERT(S1_ERROR_EQUAL(err, S1_E_INVALID_ARG));
  }

  void testCreateLibraryCXX (void)
  {
    // Check object creation
    s1::Library::Pointer lib;
    s1_ResultCode err = s1::Library::Create (lib);
    TS_ASSERT(S1_SUCCESSFUL(err));
    TS_ASSERT_DIFFERS(lib, (s1::Library*)0);
    int rc;
    rc = lib->GetRefCount();
    TS_ASSERT_EQUALS(rc, 1);
  }
};
