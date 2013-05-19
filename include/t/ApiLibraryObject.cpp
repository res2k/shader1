#include <cxxtest/TestSuite.h>

#include "s1/Library.h"

#include "lib/TestLibraryObject.h"

class ApiLibraryObjectTestSuite : public CxxTest::TestSuite 
{
public:
  void testCreateLibraryObjectC (void)
  {
    s1_Library* lib;
    s1_ErrorCode err = s1_create_library (&lib);
    TS_ASSERT(S1_SUCCESSFUL(err));
    TS_ASSERT_DIFFERS(lib, (s1_Library*)0);
    
    TestLO* lib_obj = CreateTestLO (lib);
    TS_ASSERT(lib_obj);
    s1_Library* stored_lib (s1_libraryobject_get_library (S1TYPE_CAST(s1_LibraryObject, lib_obj)));
    TS_ASSERT_EQUALS(stored_lib, lib);
    int rc = s1_release (lib_obj);
    TS_ASSERT_EQUALS(rc, 0);

    rc = s1_release (lib);
    TS_ASSERT_EQUALS(rc, 0);
  }

  void testCreateLibraryObjectCXX (void)
  {
    s1::Library::Pointer lib;
    s1_ErrorCode err = s1::Library::Create (lib);
    TS_ASSERT(S1_SUCCESSFUL(err));
    TS_ASSERT_DIFFERS(lib, (s1::Library*)0);
    int rc;
    rc = lib->GetRefCount();
    TS_ASSERT_EQUALS(rc, 1);
    
    cxxapi::TestLO::Pointer lib_obj = cxxapi::TestLO::Create (lib);
    TS_ASSERT(lib_obj);
    s1::Library::Pointer stored_lib (lib_obj->GetLibrary());
    TS_ASSERT_EQUALS(stored_lib, lib);
  }
};
