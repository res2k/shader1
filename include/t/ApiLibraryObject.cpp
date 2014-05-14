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

#include "s1/Library.h"

#include "lib/TestLibraryObject.h"

class ApiLibraryObjectTestSuite : public CxxTest::TestSuite 
{
public:
  void testCreateLibraryObjectC (void)
  {
    s1_Library* lib;
    s1_ResultCode err = s1_create_library (&lib);
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
    s1_ResultCode err = s1::Library::Create (lib);
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
