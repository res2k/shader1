#include <cxxtest/TestSuite.h>

#include "s1/Library.h"

class ApiObjectTestSuite : public CxxTest::TestSuite 
{
public:
  void testCreateLibraryC (void)
  {
    // Make sure object declarations produce the expected memory layout
    TS_ASSERT_EQUALS(sizeof(s1_Object), sizeof(s1_Library));
    // Check object creation
    s1_Library* lib;
    s1_ErrorCode err = s1_create_library (&lib);
    TS_ASSERT(S1_SUCCESSFUL(err));
    TS_ASSERT_DIFFERS(lib, (s1_Library*)0);
    int rc;
    rc = s1_get_ref_count(lib);
    TS_ASSERT_EQUALS(rc, 1);
    rc = s1_add_ref (lib);
    TS_ASSERT_EQUALS(rc, 2);
    rc = s1_release (lib);
    TS_ASSERT_EQUALS(rc, 1);
    rc = s1_release (lib);
    TS_ASSERT_EQUALS(rc, 0);
  }

  void testWrongCreateLibraryC (void)
  {
    s1_ErrorCode err = s1_create_library (0);
    TS_ASSERT(S1_ERROR_EQUAL(err, S1_E_INVALID_ARG));
  }

  void testCreateLibraryCXX (void)
  {
    // Make sure object declarations produce the expected memory layout
    TS_ASSERT_EQUALS(sizeof(s1::Object), sizeof(s1::Library));
    // Check object creation
    s1::Library::Pointer lib;
    s1_ErrorCode err = s1::Library::Create (lib);
    TS_ASSERT(S1_SUCCESSFUL(err));
    TS_ASSERT_DIFFERS(lib, (s1::Library*)0);
    int rc;
    rc = lib->GetRefCount();
    TS_ASSERT_EQUALS(rc, 1);
    {
      s1::Library::Pointer lib2 (lib);
      rc = lib->GetRefCount();
      TS_ASSERT_EQUALS(rc, 2);
    }
    rc = lib->GetRefCount();
    TS_ASSERT_EQUALS(rc, 1);
  }
};
